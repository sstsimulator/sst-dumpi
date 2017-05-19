/**
Copyright 2009-2017 National Technology and Engineering Solutions of Sandia, 
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S.  Government 
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly 
owned subsidiary of Honeywell International, Inc., for the U.S. Department of 
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2017, NTESS

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of Sandia Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Questions? Contact sst-macro-help@sandia.gov
*/

#include <dumpi/bin/dumpistats-bins.h>
#include <dumpi/bin/timeutils.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <regex.h>

namespace dumpi {

  /////// Bin specification pattern (any more complex, and it's lex/yacc time).
#define TIME                                                            \
  "((([[:digit:]]+):)?([[:digit:]]+):)?([[:digit:]]+)(\\.([[:digit:]]*))?"
#define NAMEDPT                                                         \
  "(begin|start|init|mpi_init|end|stop|finalize|mpi_finalize|" TIME ")"
#define STARTSTOP NAMEDPT " *((\\+|-) *(" TIME "))?"
#define UPTO " *(=>|->| upto| to) *"
#define BINWIDTH " *(( binsize | binwidth | by |%) *(" TIME "))?"
#define RANGE "(" STARTSTOP ")" UPTO "(" STARTSTOP ")"
  static const char *pat = "^ *(all|mpi|" RANGE ")" BINWIDTH " *$";

  inline dumpi_clock dtime(int64_t flattime) {
    dumpi_clock rv = {flattime/int64_t(1e9), flattime%int64_t(1e9)};
    return rv;
  }

  // Convert std::string
  inline int64_t tvalue(const std::string &val) {
    char *endptr;
    int64_t retval = 0;
    if(val != "") {
      retval = strtol(val.c_str(), &endptr, 10);
      if(*endptr != '\0') {
        std::cerr << "bins:  Invalid integer value " << val << "\n";
        throw "bins:  messed up.";
      }
    }
    return retval;
  }

  // Get a time value from a regex match.
  static int64_t get_timevalue(const std::vector<std::string> &match,
                               int lower, int upper)
  {
    assert((upper - lower) == 7);
    int64_t hours   = tvalue(match[lower+3]);
    int64_t minutes = tvalue(match[lower+4]);
    int64_t seconds = tvalue(match[lower+5]);
    // Fractions are only collected to the nearest nsec.
    std::string nsecstr = std::string(match[lower+7], 0, 9);
    while(nsecstr.size() < 9) nsecstr += '0';
    int64_t nanosec = tvalue(nsecstr);
    // All set.
    return (hours * int64_t(3600e9) + minutes * int64_t(60e9) +
            seconds * int64_t(1e9) + nanosec);
  }

  // Get time bounds from a regex match.
  static void get_boundvalue(const std::vector<std::string> &match, int lower,
                             int upper, bins::refpoint &the_ref,
                             int64_t &the_bias)
  {
    assert((upper - lower) == 18);
    // match[lower+1] may be a special keyword or a timestamp
    // special time values are (begin|start, init|mpi_init, end|stop, finalize|mi_finalize)
    the_bias = 0;
    const std::string type(match[lower+1]);
    if((type == "begin") || (type == "start"))
      the_ref = bins::BEGIN;
    else if((type == "end") || (type == "stop"))
      the_ref = bins::END;
    else if((type == "init") || (type == "mpi_init"))
      the_ref = bins::INIT;
    else if((type == "finalize") || (type == "mpi_finalize"))
      the_ref = bins::FINALIZE;
    else
      the_bias = get_timevalue(match, lower+11, lower+18);
    // any bias is given in match[lower+11] .. match[lower+18]
    the_bias += get_timevalue(match, lower+11, lower+18);
  }

  //
  // Private method to dump output.
  //
  void bins::dump_output() {
    if(callcount_ > 0) {
      if(current_rank_ >= 0) {
        if(file_.find(current_bin_) == file_.end()) {
          // Open a file for this bin.
          std::stringstream ss;
          ss << binid_ << "-" << current_bin_ << ".tbl";
          std::string fname = ss.str();
          file_[current_bin_] = new std::ofstream(fname.c_str());
          if(! *file_[current_bin_]) {
            std::cerr << "bin:  Failed to open outfile " << fname << "\n";
            throw "bins:  Failed to open outfile.";
          }
          // Write header info.
          int64_t binstart = begin_, binend = end_;
          if(bin_size_ > 0) {
            binstart = begin_ + current_bin_ * bin_size_;
            binend = begin_ + (current_bin_+1) * bin_size_;
          }
          *file_[current_bin_]
            << "########################################################\n"
            << "# Trace file statistics for call matching \"" << desc_ << "\"\n"
            << "# First timestamp considered is " << dtime(begin_) << "\n"
            << "# Last timestamp considered is " << dtime(end_) << "\n"
            << "# Bin size is " << dtime(bin_size_) << "\n"
            << "# Current bin starts at: " << dtime(binstart) << "\n"
            << "# Current bin ends at: " << dtime(binend) << "\n"
            << "#\n"
            << "# Column 1 is rank\n";
          int column = 2;
          for(size_t i = 0; i < handlers_.size(); ++i) {
            const std::vector<std::string> &desc = handlers_[i]->description();
            for(size_t j = 0; j < desc.size(); ++j, ++column)
              *file_[current_bin_] << "# Column " << column << " is "
                                   << desc.at(j) << "\n";
          }
          *file_[current_bin_] << "#\n# ";
          for(int lbl = 1; lbl < column; ++lbl)
            *file_[current_bin_] << std::setw(18) << std::setfill(' ')
                                 << lbl << " ";
          *file_[current_bin_] << "\n";
        } // end if(file_.find(...) == file_end())
        if(! file_[current_bin_]->is_open()) {
          // re-open a file for this bin.
          std::stringstream ss;
          ss << binid_ << "-" << current_bin_ << ".tbl";
          std::string fname = ss.str();
          file_[current_bin_]->open(fname.c_str(), std::ios_base::app);
          if(! *file_[current_bin_]) {
            std::cerr << "bin:  Failed to re-open " << fname << "\n";
            throw "bins:  Failed to re-open outfile\n";
          }
        }
        //*file_[current_bin_] << "# Bounds: " << begin_<< " to "<< end_<< "\n";
        *file_[current_bin_] << std::setw(20) << current_rank_ << " ";
        for(size_t i = 0; i < handlers_.size(); ++i) {
          const std::vector<std::string> &values = handlers_[i]->values();
          for(size_t j = 0; j < values.size(); ++j)
            *file_[current_bin_] << std::setw(18) << std::setfill(' ')
                                 << values.at(j) << " ";
        }
        *file_[current_bin_] << std::endl;
        callcount_ = 0;
      }
    }
  }

  //
  // Private method to clear handlers.
  //
  void bins::clear_handlers() {
    for(size_t i = 0; i < handlers_.size(); ++i)
      handlers_[i]->reset();
  }

  //
  // Compute time range.
  //
  void bins::compute_interval() {
    int rv;
    regex_t preg;
    static const int maxmatch = 60;
    regmatch_t mtc[maxmatch];
    //
    // Sanity check.
    if((rv = regcomp(&preg, pat, REG_EXTENDED|REG_ICASE)) != 0) {
      char errbuf[1024];
      regerror(rv, &preg, errbuf, 1024);
      std::cerr << "--bin: sanity_check:  Failed to compile regular expression \""
                << pat << "\": " << errbuf << "\n";
      throw "compute_interval: Failed sanity check.";
    }
    if((rv = regexec(&preg, desc_.c_str(), maxmatch, mtc, 0)) != 0) {
      std::cerr << "--bin:  Invalid time range expression \"" << desc_ << "\n";
      throw "bin:  Invalid time range.";
    }
    std::vector<std::string> match(maxmatch);
    for(int i = 0; i < maxmatch; ++i)
      if(mtc[i].rm_so < mtc[i].rm_eo)
        match[i] = std::string(desc_, mtc[i].rm_so, mtc[i].rm_eo - mtc[i].rm_so);
    //
    // match[1] is the top-level bounds definition (excluding bin width)
    if(match[1] == "all") {
      begin_ref_ = BEGIN;
      end_ref_ = END;
      begin_bias_ = end_bias_ = 0;
    }
    else if(match[1] == "mpi") {
      begin_ref_ = INIT;
      end_ref_ = FINALIZE;
      begin_bias_ = end_bias_ = 0;
    }
    else {
      // We have a time range defined.  An empty range is ok (no collection).
      //std::cerr << "Parsing range definition\n";
      get_boundvalue(match, 2, 20, begin_ref_, begin_bias_);
      get_boundvalue(match, 22, 40, end_ref_, end_bias_);
      //std::cerr << "begin_ref_ = " << begin_ref_ << ", begin_bias_ = "
      //          << begin_bias_ << "\n"
      //          << "end_ref_ = " << end_ref_ << ", end_bias_ = " 
      //          << end_bias_ << "\n";
    }
    //
    // if match[42] is defined, then match[43] through match[50] are the binsize
    if(match[42] != "") {
      bin_size_ = get_timevalue(match, 43, 50);
      //std::cerr << "DEBUG:  Bin size: " << bin_size_ << "\n";
    }
    else {
      //std::cerr << "DEBUG:  Bin size is zero\n";
      bin_size_ = 0;
    }
  }

  //
  // Create a new bin.  Syntax is a little screwy.
  //
  bins::bins(const std::string &desc) :
    callcount_(0), current_bin_(0), current_rank_(-1),
    initialized_(false), active_(false), desc_(desc)
  {
    for(size_t i = 0; i < desc.size(); ++i) desc_[i] = tolower(desc_[i]);
    this->compute_interval();
  }

  //
  // Object going out of scope -- time to clean up.
  //
  bins::~bins() {
    if(callcount_ > 0)
      this->dump_output();
    for(size_t i = 0; i < handlers_.size(); ++i)
      delete handlers_[i];
  }

  //
  // Clones all the given handlers.
  //
  void bins::init(const std::string &binid,
                  const std::vector<trace> *traces,
                  const std::vector<handlerbase*> handlers)
  {
    binid_ = binid;
    traces_ = traces;
    for(size_t i = 0; i < handlers.size(); ++i) {
      handlers_.push_back(handlers[i]->clone());
    }
  }

  //
  // Process the given MPI call type.
  //
  void bins::handle(dumpi_function func, uint16_t thread,
                    const dumpi_time *cpu, const dumpi_time *wall,
                    int64_t bytes_sent, int to_global_rank,
                    int64_t bytes_recvd, int from_global_rank,
                    const void *dumpi_arg)
  {
    // Figure out which bin (if any) this goes into.
    int64_t flattime = wall->start.sec * int64_t(1e9) + wall->start.nsec;
    int newbin = 0;
    if((flattime >= begin_) && (flattime < end_)) {
      if(bin_size_ > 0)
        newbin = (flattime - begin_) / bin_size_;
      if(newbin != current_bin_) {
        this->dump_output();
        this->clear_handlers();
        if(file_.find(current_bin_) != file_.end()) {
          *file_[current_bin_] << std::flush;
          file_[current_bin_]->close();
        }
        current_bin_ = newbin;
        callcount_ = 0;
      }
      ++callcount_;
      for(size_t i = 0; i < handlers_.size(); ++i)
        handlers_[i]->handle(func, thread, cpu, wall,
                             bytes_sent, to_global_rank,
                             bytes_recvd, from_global_rank, dumpi_arg);
    }
  }

  //
  // Figure out timestamp.
  //
  int64_t bins::resolve_time(refpoint type, int64_t bias, const trace &curr) const
  {
    dumpi_clock clk = {0, 0};
    switch(type) {
    case BEGIN:
      clk = curr.start_time();
      break;
    case END:
      clk = curr.stop_time();
      break;
    case INIT:
      clk = curr.init_time();
      break;
    case FINALIZE:
      clk = curr.finalize_time();
      break;
    case ABSOLUTE:
      break;
    }
    return (clk.sec * int64_t(1e9) + clk.nsec + bias);
  }

  //
  // Reset counters and internal state in preparation for a new trace.
  //
  void bins::start_trace(int rank) {
    this->reset_trace();
    if(rank != current_rank_) {
      current_rank_ = rank;
      // Figure out the bounds.
      const trace &curr = traces_->at(rank);
      begin_ = resolve_time(begin_ref_, begin_bias_, curr);
      end_ = resolve_time(end_ref_, end_bias_, curr);
      if(bin_size_ <= 0)
        end_ += 1;
      //std::cerr << "DEBUG:  begin_ = " << begin_ << ", end_ = " << end_ << "\n";
    }
  }

  //
  // Clear handlers in preparation for another bin.
  //
  void bins::reset_trace() {
    if(callcount_ > 0) {
      this->dump_output();
      this->clear_handlers();
      callcount_ = 0;
    }
    current_rank_ = -1;
  }

} // end of namespace dumpi