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

#include <dumpi/bin/dumpistats-gatherbin.h>
#include <dumpi/bin/timeutils.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

namespace dumpi {

  //
  // Private method to dump output.
  //
  void gatherbin::dump_output() {
    //std::cerr << "gatherbin::dump_output() for bin " << current_bin_ << ".  Callcount=" << callcount_ << ", current_rank=" << current_rank_ << "\n";
    if(callcount_ > 0) {
      if(current_rank_ >= 0) {
        if(file_.find(current_bin_) == file_.end()) {
          // Open a file for this bin.
          std::stringstream ss;
          ss << binid_ << "-" << current_bin_ << ".tbl";
          std::string fname = ss.str();
          //std::cerr << "Writing output to " << fname << "\n";
          file_[current_bin_] = new std::ofstream(fname.c_str());
          if(! *file_[current_bin_]) {
            std::cerr << "bin:  Failed to open outfile " << fname << "\n";
            throw "bins:  Failed to open outfile.";
          }
          // Write header info.
          *file_[current_bin_]
            << "########################################################\n"
            << "# Trace file statistics for calls marked by the annotations\n"
            << "#     regex_t(" << start_pattern_ << ")\n"
            << "# and\n"
            << "#     regex_t(" << stop_pattern_ << ")\n"
            << "# with accumulate = " << std::boolalpha << accumulate_ << "\n"
            << "#\n"
            << "# Column 1 is rank\n"
            << "# Column 2 is total number of intervals collected\n"
            << "# Column 3 is first timestamp at which collection was active\n"
            << "# Column 4 is last timestamp at which collection was active\n"
            << "# Column 5 is total time during which collection was active\n";
          int column = 6;
          for(size_t i = 0; i < handlers_.size(); ++i) {
            const std::vector<std::string> &desc = handlers_[i]->description();
            for(size_t j = 0; j < desc.size(); ++j, ++column)
              *file_[current_bin_] << "# Column " << column << " is "
                                   << desc.at(j) << "\n";
          }
          *file_[current_bin_] << "#\n# ";
          for(int lbl = 1; lbl < column; ++lbl)
            *file_[current_bin_] << std::setw(20) << std::setfill(' ')
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
        *file_[current_bin_] << std::setw(22) << current_rank_ << " "
                             << std::setw(20) <<intervals_[current_rank_] << " "
                             << std::setw(20) << first_stamp_[current_rank_]<<" "
                             << std::setw(20) << last_stamp_[current_rank_]<<" "
                             << std::setw(20) << tot_act_[current_rank_] << " ";
        for(size_t i = 0; i < handlers_.size(); ++i) {
          const std::vector<std::string> &values = handlers_[i]->values();
          for(size_t j = 0; j < values.size(); ++j)
            *file_[current_bin_] << std::setw(20) << std::setfill(' ')
                                 << values.at(j) << " ";
        }
        *file_[current_bin_] << std::endl;
        //file_[current_bin_]->close();
        callcount_ = 0;
      }
    }
  }

  //
  // Private method to clear handlers.
  //
  void gatherbin::clear_handlers() {
    for(size_t i = 0; i < handlers_.size(); ++i)
      handlers_[i]->reset();
  }

  //
  // Private method to activate all handlers.
  //
  void gatherbin::mark_handlers_active(const dumpi_time *cpu,
                                       const dumpi_time *wall,
                                       const dumpi_perfinfo *perf)
  {
    for(size_t i = 0; i < handlers_.size(); ++i)
      handlers_[i]->mark_active(cpu, wall, perf);
  }

  //
  // Private method to deactivate all handlers.
  //
  void gatherbin::mark_handlers_inactive(const dumpi_time *cpu,
                                         const dumpi_time *wall,
                                         const dumpi_perfinfo *perf)
  {
    for(size_t i = 0; i < handlers_.size(); ++i)
      handlers_[i]->mark_inactive(cpu, wall, perf);
  }

  //
  // Utility.
  //
  inline std::string get_regex(const std::string &expression,
                                 size_t start, char delim, size_t &stop)
  {
    std::stringstream ss;
    bool escaped = false;
    size_t pos;
    if(expression[0] != delim) {
      std::cerr << "gatherbin:  Bogus expression \"" << expression << "\"\n";
      throw "bad regex";
    }
    for(pos = start+1; pos < expression.size(); ++pos) {
      if((! escaped) && (expression[pos] == delim)) {
        break;
      }
      else if((! escaped) && (expression[pos] == '\\')) {
        escaped = true;
      }
      else {
        if(escaped && (expression[pos] != delim)) {
          // pass this escape on through
          ss << '\\';
        }
        escaped = false;
        ss << expression[pos];
      }
    }
    stop = pos;
    // Sanity check.
    if((pos >= expression.size()) || (expression[pos] != delim)) {
      std::cerr << "gatherbin: Pattern expression not correctly terminated\n";
      throw "bad regex";
    }
    return ss.str();
  }

  //
  // Parse regular expressions.
  //
  int gatherbin::parse_patterns(const std::string &expression) {
    // First character is delimiter.  As in sed, it must not be '\\' or '\n'
    char delim = '/';
    if(expression.size() < 3) {
      std::cerr << "gatherbin:  Invalid regular expression pattern \""
                << expression << "\": Please use '/start-pat/end-pat/'\n";
      throw "bad regex";
    }
    if(expression[0] == '\\' || expression[0] == '\n') {
      std::cerr << "gatherbin:  Pattern expression cannot start with backslash"
                << " or newline.\n";
      throw "bad regex";
    }
    delim = expression[0];
    start_pattern_ = stop_pattern_ = "";
    // Populate the start pattern.
    size_t pos;
    start_pattern_ = get_regex(expression, 0, delim, pos);
    stop_pattern_ = get_regex(expression, pos, delim, pos);
    std::string flags = expression.substr(pos + 1);
    int rv = 0;
    for(size_t i = 0; i < flags.size(); ++i) {
      switch(flags[i]) {
      case 'I':
      case 'i':
        rv = REG_ICASE;
        break;
      default:
        std::cerr << "gatherbin:  Invalid expression flag " << flags[i] << "\n";
        throw "bad regex";
      }
    }
    return rv;
  }

  //
  // Create a new bin.
  //
  gatherbin::gatherbin(const std::string &expression, bool accumulate) :
    callcount_(0), current_bin_(-1), current_rank_(-1),
    initialized_(false), active_(false), accumulate_(accumulate)
  {
    //std::cerr << "gatherbin(" << start_pat << ", " << stop_pat
    //          << ", " << accumulate << "): '" << start_pattern_ << "', '"
    //          << stop_pattern_ << "'\n";
    int rv;
    // Parse out the start and end patterns.
    int icase = parse_patterns(expression);
    static const int reflags = REG_EXTENDED | REG_NOSUB | icase;
    if(start_pattern_ != "") {
      if((rv = regcomp(&start_regex_, start_pattern_.c_str(), reflags)) != 0) {
        std::cerr << "gatherbin:  Failed to compile regex for \""
                  << start_pattern_ << "\": " << strerror(rv) << "\n";
        throw "gatherbin:  Bad regex.";
      }
    }
    else {
      current_bin_ = 0;
      active_ = true;
    }
    if(accumulate_) current_bin_ = 0;
    if(stop_pattern_ != "") {
      if((rv = regcomp(&stop_regex_, stop_pattern_.c_str(), reflags)) != 0) {
        std::cerr << "gatherbin:  Failed to compile regex for \""
                  << stop_pattern_ << "\": " << strerror(rv) << "\n";
        throw "gatherbin:  Bad regex.";
      }
    }
  }

  //
  // Object going out of scope -- time to clean up.
  //
  gatherbin::~gatherbin() {
    //std::cerr << "~gatherbin().  active=" << active_ << ", callcount_="
    //          << callcount_ << ".  current_rank=" << current_rank_ << "\n";
    if(active_) {
      last_stamp_[current_rank_] = traces_->at(current_rank_).stop_time();
      tot_act_[current_rank_] += (stop_gather_[current_rank_] -
                                  start_gather_[current_rank_]);
      //std::cerr <<"dump output:  start_pattern_ = " << start_pattern_ << "\n";
    }
    if(callcount_ > 0) {
      this->dump_output();
    }
    for(size_t i = 0; i < handlers_.size(); ++i)
      delete handlers_[i];
  }

  //
  // Clones all the given handlers.
  //
  void gatherbin::init(const std::string &binid,
                  const std::vector<trace> *traces,
                  const std::vector<handlerbase*> handlers)
  {
    binid_ = binid;
    traces_ = traces;
    for(size_t i = 0; i < handlers.size(); ++i) {
      handlers_.push_back(handlers[i]->clone());
    }
    initialized_ = true;
  }

  //
  // Process the given annotation call.
  //
  void gatherbin::annotate(const char *message, uint16_t thread,
                           const dumpi_time *cpu, const dumpi_time *wall,
                           const dumpi_perfinfo *perf)
  {
    //std::cerr << "Annotate:  start_pattern_=\"" << start_pattern_ 
    //          << "\", stop_pattern_=\"" << stop_pattern_ << "\", message=\""
    //          << message << "\"\n";
    if(! active_) {
      if(start_pattern_ != "") {
        //std::cerr << "Comparing start pattern " << start_pattern_
        //          << " to message " << message << "\n";
        if(regexec(&start_regex_, message, 0, NULL, 0) == 0) {
          //std::cerr << "MATCH\n";
          start_gather_[current_rank_] = wall->start;
          stop_gather_[current_rank_] = wall->start;
          if(first_stamp_[current_rank_] > wall->start)
            first_stamp_[current_rank_] = wall->start;
          // Match.
          intervals_[current_rank_] += 1;
          if(wall->start > last_stamp_[current_rank_])
            last_stamp_[current_rank_] = wall->start;
          if(! accumulate_) {
            first_stamp_.clear();
            last_stamp_.clear();
            start_gather_.clear();
            stop_gather_.clear();
            tot_act_.clear();
            accumulated_.clear();
            first_stamp_[current_rank_] = wall->start;
            // Notify the handlers so we get good timings
            ++current_bin_;
            //std::cerr << "Bumped bin to " << current_bin_ << "\n";
            //std::cerr << "Rank: " << current_rank_ << ".  First stamp: "
            //          << first_stamp_[current_rank_] << "\n";
          }
          else {
            if(wall->start < first_stamp_[current_rank_])
              first_stamp_[current_rank_] = wall->start;
          }
          this->mark_handlers_active(cpu, wall, perf);
          active_ = true;
        }
      }
    }
    else {
      if(stop_pattern_ != "") {
        //std::cerr << "Comparing stop pattern " << stop_pattern_
        //          << " to message " << message << "\n";
        if(regexec(&stop_regex_, message, 0, NULL, 0) == 0) {
          //std::cerr << "MATCH\n";
          stop_gather_[current_rank_] = wall->start;
          if(wall->start < first_stamp_[current_rank_])
            first_stamp_[current_rank_] = wall->start;
          if(wall->start > last_stamp_[current_rank_])
            last_stamp_[current_rank_] = wall->start;
          this->mark_handlers_inactive(cpu, wall, perf);
          if(! accumulate_) {
            tot_act_[current_rank_] = 
              stop_gather_[current_rank_] - start_gather_[current_rank_];
            dump_output();
            clear_handlers();
          }
          else {
            tot_act_[current_rank_] += (stop_gather_[current_rank_] -
                                        start_gather_[current_rank_]);
          }
          active_ = false;
        }
      }
    }
  }

  //
  // Process the given MPI call type.
  //
  void gatherbin::handle(dumpi_function func, uint16_t thread,
                         const dumpi_time *cpu, const dumpi_time *wall,
                         const dumpi_perfinfo *perf,
                         int64_t bytes_sent, int to_global_rank,
                         int64_t bytes_recvd, int from_global_rank,
                         const void *dumpi_arg)
  {
    // Figure out which bin (if any) this goes into.
    //std::cerr << "handle(" << func << ")\n";
    if(active_) {
      ++callcount_;
      //std::cerr << " -- handler active.  Bumped call count to " << callcount_ << "\n";
      for(size_t i = 0; i < handlers_.size(); ++i)
        handlers_[i]->handle(func, thread, cpu, wall, perf,
                             bytes_sent, to_global_rank,
                             bytes_recvd, from_global_rank, dumpi_arg);
    }
  }

  //
  // Reset counters and internal state in preparation for a new trace.
  //
  void gatherbin::start_trace(int rank) {
    if(rank != current_rank_) {
      if(current_rank_ >= 0) {
        tot_act_[current_rank_] += (stop_gather_[current_rank_] -
                                    start_gather_[current_rank_]);
        dump_output();
      }
      clear_handlers();
      current_rank_ = rank;
      current_bin_ = 0;
      // Figure out the bounds.
      if(first_stamp_.find(rank) == first_stamp_.end()) {
        if(start_pattern_ != "")
          first_stamp_[rank].sec = (unsigned(1) << 31) - 1;
        else {
          first_stamp_[rank] = traces_->at(rank).start_time();
          active_ = true;
        }
      }
      if(last_stamp_.find(rank) == last_stamp_.end()) {
        if(stop_pattern_ != "")
          last_stamp_[rank].sec = -1;
        else
          last_stamp_[rank] = traces_->at(rank).stop_time();
      }
      if(tot_act_.find(rank) == tot_act_.end()) {
        dumpi_clock zero = {0, 0};
        tot_act_[rank] = zero;
      }
      if(accumulated_.find(rank) == accumulated_.end()) {
        accumulated_[rank] = 1;
      }
      else {
        ++accumulated_[rank];
      }
    }
  }

  //
  // Reset current rank.
  //
  void gatherbin::reset_trace() {
    if(current_rank_ >= 0) {
      tot_act_[current_rank_] += (stop_gather_[current_rank_] -
                                  start_gather_[current_rank_]);
      dump_output();
    }
    clear_handlers();
    current_rank_ = -1;
    first_stamp_.clear();
    last_stamp_.clear();
    start_gather_.clear();
    stop_gather_.clear();
    tot_act_.clear();
    accumulated_.clear();
  }

} // end of namespace dumpi