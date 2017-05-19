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

#include <dumpi/bin/dumpistats-handlers.h>
#include <dumpi/bin/timeutils.h>
#include <iomanip>
#include <sstream>
#include <string.h>
#include <regex.h>

namespace dumpi {

  inline int64_t flatten(const dumpi_clock &tt) {
    return (int64_t(tt.sec * int64_t(1e9)) + tt.nsec);
  }

  inline dumpi_clock dtime(int64_t flattime) {
    //explicit cast needed for c++11 compatibility
    int32_t arg1 = (int32_t) flattime/int64_t(1e9);
    int32_t arg2 = (int32_t) flattime%int64_t(1e9);
    dumpi_clock rv = {arg1, arg2};
    return rv;
  }

  static const int collectives[] = {
    DUMPI_Barrier, DUMPI_Bcast, DUMPI_Gather, DUMPI_Gatherv, DUMPI_Scatter,
    DUMPI_Scatterv, DUMPI_Allgather, DUMPI_Allgatherv, DUMPI_Alltoall,
    DUMPI_Alltoallv, DUMPI_Reduce, DUMPI_Allreduce, DUMPI_Reduce_scatter,
    DUMPI_Scan, DUMPI_Comm_dup, DUMPI_Comm_create, DUMPI_Comm_split,
    DUMPI_Comm_free, DUMPI_Intercomm_create, DUMPI_Init, DUMPI_Finalize,
    DUMPI_Comm_spawn, DUMPI_Comm_spawn_multiple, DUMPI_Alltoallw,
    DUMPI_Exscan, DUMPI_Init_thread, DUMPI_File_open, DUMPI_File_close,
    DUMPI_File_set_size, DUMPI_File_preallocate, DUMPI_File_set_info,
    DUMPI_File_set_view, DUMPI_File_read_at_all, DUMPI_File_write_at_all,
    DUMPI_File_read_all, DUMPI_File_write_all, DUMPI_File_read_ordered,
    DUMPI_File_seek_shared, DUMPI_File_read_at_all_begin,
    DUMPI_File_read_at_all_end, DUMPI_File_write_at_all_begin,
    DUMPI_File_write_at_all_end, DUMPI_File_read_all_begin,
    DUMPI_File_read_all_end, DUMPI_File_write_all_begin,
    DUMPI_File_write_all_end, DUMPI_File_read_ordered_begin,
    DUMPI_File_read_ordered_end, DUMPI_File_write_ordered_begin,
    DUMPI_File_write_ordered_end, DUMPI_File_set_atomicity,
    DUMPI_File_sync, -1
  };

  static const int waits[] = {
    DUMPI_Wait, DUMPI_Waitany, DUMPI_Waitsome, DUMPI_Waitall, DUMPI_Probe,
    DUMPI_Start, DUMPI_Startall, DUMPI_Win_wait, DUMPIO_Wait, DUMPIO_Waitany,
    DUMPIO_Waitsome, DUMPIO_Waitall, -1
  };

  static int sends[] = {
    DUMPI_Send, DUMPI_Bsend, DUMPI_Rsend, DUMPI_Ssend,
    DUMPI_Isend, DUMPI_Ibsend, DUMPI_Irsend, DUMPI_Issend,
    DUMPI_Send_init, DUMPI_Bsend_init, DUMPI_Rsend_init, DUMPI_Ssend_init,
    DUMPI_Sendrecv, DUMPI_Sendrecv_replace, -1
  };

  static const int recvs[] = {
    DUMPI_Recv, DUMPI_Irecv, DUMPI_Recv_init,
    DUMPI_Sendrecv, DUMPI_Sendrecv_replace, -1
  };

  static const int ios[] = {
    DUMPI_File_open, DUMPI_File_close, DUMPI_File_delete,
    DUMPI_File_set_size, DUMPI_File_preallocate, DUMPI_File_get_size,
    DUMPI_File_get_group, DUMPI_File_get_amode, DUMPI_File_set_info,
    DUMPI_File_get_info, DUMPI_File_set_view, DUMPI_File_get_view,
    DUMPI_File_read_at, DUMPI_File_read_at_all, DUMPI_File_write_at,
    DUMPI_File_write_at_all, DUMPI_File_iread_at, DUMPI_File_iwrite_at,
    DUMPI_File_read, DUMPI_File_read_all, DUMPI_File_write,
    DUMPI_File_write_all, DUMPI_File_iread, DUMPI_File_iwrite,
    DUMPI_File_seek, DUMPI_File_get_position,
    DUMPI_File_get_byte_offset, DUMPI_File_read_shared,
    DUMPI_File_write_shared, DUMPI_File_iread_shared,
    DUMPI_File_iwrite_shared, DUMPI_File_read_ordered,
    DUMPI_File_write_ordered, DUMPI_File_seek_shared,
    DUMPI_File_get_position_shared, DUMPI_File_read_at_all_begin,
    DUMPI_File_read_at_all_end, DUMPI_File_write_at_all_begin,
    DUMPI_File_write_at_all_end, DUMPI_File_read_all_begin,
    DUMPI_File_read_all_end, DUMPI_File_write_all_begin,
    DUMPI_File_write_all_end, DUMPI_File_read_ordered_begin,
    DUMPI_File_read_ordered_end, DUMPI_File_write_ordered_begin,
    DUMPI_File_write_ordered_end, DUMPI_File_get_type_extent,
    DUMPI_Register_datarep, DUMPI_File_set_atomicity,
    DUMPI_File_get_atomicity, DUMPI_File_sync, DUMPIO_Test, DUMPIO_Wait,
    DUMPIO_Testall, DUMPIO_Waitall, DUMPIO_Testany, DUMPIO_Waitany,
    DUMPIO_Waitsome, DUMPIO_Testsome, -1
  };

  // Counter.
  handlerbase::handlerbase(const std::string &pattern) {
    memset(collect_, 0, DUMPI_END_OF_STREAM*sizeof(bool));
    if(pattern == "all") {
      for(int i = 0; i < DUMPI_END_OF_STREAM; ++i)
        collect_[i] = true;
    }
    else if(pattern == "mpi") {
      for(int i = 0; i < DUMPI_ALL_FUNCTIONS; ++i)
        collect_[i] = true;
    }
    else if(pattern == "non-mpi") {
      for(int i = DUMPI_ALL_FUNCTIONS; i < DUMPI_END_OF_STREAM; ++i)
        collect_[i] = true;
    }
    else if(pattern == "coll") {
      for(int i = 0; collectives[i] >= 0; ++i)
        collect_[collectives[i]] = true;
    }
    else if(pattern == "wait") {
      for(int i = 0; waits[i] >= 0; ++i)
        collect_[waits[i]] = true;
    }
    else if(pattern == "io") {
      for(int i = 0; ios[i] >= 0; ++i)
        collect_[ios[i]] = true;
    }
    else if(pattern == "sends") {
      for(int i = 0; sends[i] >= 0; ++i)
        collect_[sends[i]] = true;
    }
    else if(pattern == "recvs") {
      for(int i = 0; recvs[i] >= 0; ++i)
        collect_[recvs[i]] = true;
    }
    else {
      std::string pat(pattern);
      // Only match patterns that describe the whole std::string.
      // This should also work using REG_STARTEND, but that's not portable.
      if(pat.size() <= 0 || pat[0] != '^') pat = "^" + pat;
      if(pat.size() <= 0 || pat[pat.size()-1] != '$') pat = pat + "$";
      regex_t preg;
      int error = regcomp(&preg, pat.c_str(),
                          REG_EXTENDED | REG_ICASE | REG_NOSUB);
      if(error) {
        char errbuf[1024]; errbuf[1023] = '\0';
        regerror(error, &preg, errbuf, 1024);
        std::cerr << "--count:  Invalid regular expression \"" << pattern
                  << "\":  " << errbuf << "\n";
        throw "bad_regex";
      }
      for(int i = 0; i < DUMPI_END_OF_STREAM; ++i) {
        error = regexec(&preg, dumpi_function_label(dumpi_function(i)), 0,NULL,0);
        if((error != 0) && (error != REG_NOMATCH)) {
          char errbuf[1024]; errbuf[1023] = '\0';
          regerror(error, &preg, errbuf, 1024);
          std::cerr << "--count:  Bad match in pattern \"" << pattern
                    << "\" and token " << dumpi_function_label(dumpi_function(i))
                    << ":  " << errbuf << "\n";
          throw "bad_regex";
        }
        if(error == 0) { // match
          collect_[i] = true;
        }
      }
    }
  }

  handlerbase::~handlerbase() {
  }

  // Counter.
  counter::counter(const std::string &pattern) :
    handlerbase(pattern), count_(0)
  {
    std::stringstream s;
    std::string tmp("Call count for pattern \"");
    tmp += pattern;
    tmp += "\"";
    s << std::setw(19) << std::setfill(' ') << tmp;
    desc_.push_back(s.str());
  }
  counter::~counter() {}
  void counter::reset() { count_ = 0; }
  const std::vector<std::string>& counter::description() const {
    return desc_;
  }
  const std::vector<std::string>& counter::values() const {
    values_.clear();
    std::stringstream s;
    s << count_;
    values_.push_back(s.str());
    return values_;
  }
  handlerbase* counter::clone() const {
    return new counter(*this);
  }
  void counter::mark_active(const dumpi_time *, const dumpi_time *,
                            const dumpi_perfinfo *)
  {}
  void counter::mark_inactive(const dumpi_time*, const dumpi_time*,
                              const dumpi_perfinfo*)
  {}
  void counter::handle(dumpi_function func, uint16_t,
                       const dumpi_time*, const dumpi_time*,
                       const dumpi_perfinfo*,
                       int64_t, int, int64_t, int, const void*) 
  {
    if(collect_[func]) ++count_;
  }

  timer::timer(const std::string &pattern) :
    handlerbase(pattern), cpu_inside_mpi_(0), cpu_outside_mpi_(0),
    wall_inside_mpi_(0), wall_outside_mpi_(0)
  {
    std::string tmp;

    tmp = "profiled CPU time for pattern \"";
    tmp += pattern;
    tmp += "\"";
    desc_.push_back(tmp);

    tmp = "unprofiled CPU time for pattern \"";
    tmp += pattern;
    tmp += "\"";
    desc_.push_back(tmp);

    tmp = "profiled wall time for pattern \"";
    tmp += pattern;
    tmp += "\"";
    desc_.push_back(tmp);

    tmp = "unprofiled wall time for pattern \"";
    tmp += pattern;
    tmp += "\"";
    desc_.push_back(tmp);

    dumpi_time neg1 = {{-1, -1}, {-1, -1}};
    last_cpu_ = last_wall_ = neg1;
  }
  timer::~timer() {
  }
  void timer::reset() {
    cpu_inside_mpi_ = cpu_outside_mpi_ = 0;
    wall_inside_mpi_ = wall_outside_mpi_ = 0;
  }
  const std::vector<std::string>& timer::description() const {
    return desc_;
  }
  
  const std::vector<std::string>& timer::values() const {
    values_.clear();
    std::stringstream s;
    s << dtime(cpu_inside_mpi_);   values_.push_back(s.str());   s.str("");
    s << dtime(cpu_outside_mpi_);  values_.push_back(s.str());   s.str("");
    s << dtime(wall_inside_mpi_);  values_.push_back(s.str());   s.str("");
    s << dtime(wall_outside_mpi_); values_.push_back(s.str());   s.str("");
    return values_;
  }
  handlerbase* timer::clone() const {
    return new timer(*this);
  }
  void timer::mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                          const dumpi_perfinfo*)
  {
    last_cpu_ = *cpu;
    last_wall_ = *wall;
  }
  void timer::mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                            const dumpi_perfinfo*)
  {
      int64_t flat_cpu_stop = flatten(cpu->stop);
      int64_t flat_wall_stop = flatten(wall->stop);
      int64_t flat_last_cpu_stop = flatten(last_cpu_.stop);
      int64_t flat_last_wall_stop = flatten(last_wall_.stop);
      cpu_outside_mpi_ += flat_cpu_stop - flat_last_cpu_stop;
      wall_outside_mpi_ += flat_wall_stop - flat_last_wall_stop;
      last_cpu_ = *cpu;
      last_wall_ = *wall;    
  }
  void timer::handle(dumpi_function func, uint16_t,
                     const dumpi_time *cpu, const dumpi_time *wall,
                     const dumpi_perfinfo*,
                     int64_t, int, int64_t, int, const void*) 
  {
    if(last_cpu_.stop.sec < 0)
      last_cpu_ = *cpu;
    if(last_wall_.stop.sec < 0)
      last_wall_ = *wall;
    if(collect_[func]) {
      int64_t flat_cpu_start = flatten(cpu->start);
      int64_t flat_cpu_stop = flatten(cpu->stop);
      int64_t flat_wall_start = flatten(wall->start);
      int64_t flat_wall_stop = flatten(wall->stop);
      int64_t flat_last_cpu_stop = flatten(last_cpu_.stop);
      int64_t flat_last_wall_stop = flatten(last_wall_.stop);
      cpu_inside_mpi_ += flat_cpu_stop - flat_cpu_start;
      if(flat_cpu_start > flat_last_cpu_stop)
        cpu_outside_mpi_ += flat_cpu_start - flat_last_cpu_stop;
      wall_inside_mpi_ += flat_wall_stop - flat_wall_start;
      if(flat_wall_start > flat_last_wall_stop)
        wall_outside_mpi_ += flat_wall_start - flat_last_wall_stop;
      last_cpu_ = *cpu;
      last_wall_ = *wall;
    }
  }

  // Sender.
  sender::sender(const std::string &pattern) :
    handlerbase(pattern), bytes_(0)
  {
    std::string tmp("total sends for pattern \"");
    tmp += pattern;
    tmp += "\"";
    desc_.push_back(tmp);
  }
  sender::~sender() {}
  void sender::reset() { bytes_ = 0; }
  const std::vector<std::string>& sender::description() const {
    return desc_;
  }
  const std::vector<std::string>& sender::values() const {
    values_.clear();
    std::stringstream s;
    s << bytes_;
    values_.push_back(s.str());
    return values_;
  }
  handlerbase* sender::clone() const {
    return new sender(*this);
  }
  void sender::mark_active(const dumpi_time *, const dumpi_time *,
                           const dumpi_perfinfo *)
  {}
  void sender::mark_inactive(const dumpi_time*, const dumpi_time*,
                             const dumpi_perfinfo*)
  {}
  void sender::handle(dumpi_function func, uint16_t,
                      const dumpi_time*, const dumpi_time*,
                      const dumpi_perfinfo *perf,
                      int64_t bytes_sent, int, int64_t, int, const void*) 
  {
    if(collect_[func])
      bytes_ += bytes_sent;
  }

  // Recver.
  recver::recver(const std::string &pattern) :
    handlerbase(pattern), bytes_(0)
  {
    std::string tmp("total receives for pattern \"");
    tmp += pattern;
    tmp += "\"";
    desc_.push_back(tmp);
  }
  recver::~recver() {}
  void recver::reset() { bytes_ = 0; }
  const std::vector<std::string>& recver::description() const {
    return desc_;
  }
  const std::vector<std::string>& recver::values() const {
    values_.clear();
    std::stringstream s;
    s << bytes_;
    values_.push_back(s.str());
    return values_;
  }
  handlerbase* recver::clone() const {
    return new recver(*this);
  }
  void recver::mark_active(const dumpi_time *, const dumpi_time *,
                           const dumpi_perfinfo *)
  {}
  void recver::mark_inactive(const dumpi_time*, const dumpi_time*,
                              const dumpi_perfinfo*)
  {}
  void recver::handle(dumpi_function func, uint16_t,
                      const dumpi_time*, const dumpi_time*,
                      const dumpi_perfinfo*,
                      int64_t, int, int64_t bytes_recvd, int, const void*) 
  {
    if(collect_[func])
      bytes_ += bytes_recvd;
  }

  // Monitor full set of exchanges.
  exchanger::exchanger(const std::string &pattern) :
    handlerbase(pattern), pat_(pattern), collsent_(0), collrecvd_(0)
  {}
  exchanger::~exchanger() {
  }
  void exchanger::reset() {
    sent_.clear();
    collsent_ = collrecvd_ = 0;
  }
  const std::vector<std::string>& exchanger::description() const {
    desc_.clear();
    std::stringstream ss;
    for(int i = 0; i < world_size_; ++i) {
      ss << "bytes sent by funcs matching \"" << pat_ << "\" to node " << i;
      desc_.push_back(ss.str());
      ss.str("");
    }
    // Skipping collectives -- the actual data sent is too dependent
    // on the algorithm used to implement the collective.
    //ss << "xchg{" << pat_ << "}{coll-send}";
    //ss << "xchg{" << pat_ << "}{coll-recv} ";
    return desc_;
  }
  const std::vector<std::string>& exchanger::values() const {
    values_.clear();
    std::stringstream ss;
    for(int i = 0; i < world_size_; ++i) {
      int64_t sc = (size_t(i) < sent_.size() ? sent_[i] : 0);
      ss << sc; values_.push_back(ss.str()); ss.str("");
    }
    // Skip collectives.
    //ss << collsent_;
    //ss << collrecvd_;
    return values_;
  }
  handlerbase* exchanger::clone() const {
    return new exchanger(*this);
  }
  void exchanger::mark_active(const dumpi_time *, const dumpi_time *,
                              const dumpi_perfinfo *)
  {}
  void exchanger::mark_inactive(const dumpi_time*, const dumpi_time*,
                                const dumpi_perfinfo*)
  {}
  void exchanger::handle(dumpi_function func, uint16_t,
                         const dumpi_time*, const dumpi_time*,
                         const dumpi_perfinfo*,
                         int64_t bytes_sent, int to_global_rank,
                         int64_t bytes_recvd, int from_global_rank, const void*) 
  {
    if(int(sent_.size()) < world_size_) sent_.resize(world_size_);
    if((to_global_rank >= world_size_) || (from_global_rank >= world_size_)) {
      std::cerr << "exchanger::handle:  Invalid rank entered.  World size: "
                << world_size_ << ", sending to " << to_global_rank
                << " and receiving from " << from_global_rank << "\n";
    }
    if(collect_[func]) {
      if(to_global_rank >= 0)
        sent_.at(to_global_rank) += bytes_sent;
      else
        collsent_ += bytes_sent;
      if(from_global_rank < 0)
        collrecvd_ += bytes_recvd;
    }
  }

  perfcounter::perfcounter(const std::string &pattern) :
    handlerbase(pattern)
  {}
  perfcounter::~perfcounter() {
  }
  void perfcounter::reset() {
    inside_mpi_.clear();
    outside_mpi_.clear();
  }
  const std::vector<std::string>& perfcounter::description() const {
    desc_.clear();
    std::stringstream ss;
    for(tracker_t::const_iterator in = inside_mpi_.begin(),
          out = outside_mpi_.begin();
        in != inside_mpi_.end() && out != outside_mpi_.end();
        ++in, ++out)
      {
        ss << "performance counter " << in->first << " inside MPI";
        desc_.push_back(ss.str());
        ss.str("");
        ss << "performance counter " << out->first << " outside MPI";
        desc_.push_back(ss.str());
        ss.str("");
      }
    return desc_;
  }
  
  const std::vector<std::string>& perfcounter::values() const {
    values_.clear();
    std::stringstream ss;
    for(tracker_t::const_iterator in = inside_mpi_.begin(),
          out = outside_mpi_.begin();
        in != inside_mpi_.end() && out != outside_mpi_.end();
        ++in, ++out)
      {
        ss << in->second;
        values_.push_back(ss.str());
        ss.str("");
        ss << out->second;
        values_.push_back(ss.str());
        ss.str("");
      }
    return values_;
  }
  handlerbase* perfcounter::clone() const {
    return new perfcounter(*this);
  }
  void perfcounter::mark_active(const dumpi_time*, const dumpi_time*,
                                const dumpi_perfinfo *perf)
  {
    if(perf) {
      for(int i = 0; i < perf->count; ++i) {
        last_out_[perf->counter_tag[i]] = perf->outvalue[i];
      }
    }
  }
  
  void perfcounter::mark_inactive(const dumpi_time *, const dumpi_time *,
                                    const dumpi_perfinfo *perf)
  {
    if(perf) {
      for(int i = 0; i < perf->count; ++i) {
        const std::string label = perf->counter_tag[i];
        if(last_out_.find(label) != last_out_.end())
          outside_mpi_[label] += perf->invalue[i] - last_out_[label];
      }
    }
  }
  void perfcounter::handle(dumpi_function func, uint16_t,
                           const dumpi_time*, const dumpi_time*,
                           const dumpi_perfinfo *perf,
                           int64_t, int, int64_t, int, const void*) 
  {
    if(perf) {
      if(collect_[func]) {
        for(int i = 0; i < perf->count; ++i) {
          const std::string label = perf->counter_tag[i];
          if(last_out_.find(label) != last_out_.end())
            outside_mpi_[label] += perf->invalue[i] - last_out_[label];
          inside_mpi_[label] += perf->outvalue[i] - perf->invalue[i];
          last_out_[label] = perf->outvalue[i];
        }
      }
    }
  }

  const int lumper::maxbin_ = 40;

  lumper::lumper(const std::string &pattern) :
    handlerbase(pattern), sends_(maxbin_), recvs_(maxbin_), pat_(pattern)
  {
    int64_t one(1);
    desc_.clear();
    std::stringstream ss;
    for(size_t i = 0; i < maxbin_; ++i) {
      int64_t low = (i > 0 ? ((one << (i-1)) + 1) : 1);
      int64_t high = one << i;
      ss << "sends from " << low << " up to " << high << " bytes for " << pat_;
      desc_.push_back(ss.str());
      ss.str("");
      ss << "recvs from " << low << " up to " << high << " bytes for " << pat_;
      desc_.push_back(ss.str());
      ss.str("");
    }
  }

  lumper::~lumper() {
  }

  void lumper::reset() {
    sends_.clear();
    recvs_.clear();
    sends_.resize(maxbin_);
    recvs_.resize(maxbin_);
  }
  const std::vector<std::string>& lumper::description() const {
    return desc_;
  }
  
  const std::vector<std::string>& lumper::values() const {
    values_.clear();
    std::stringstream ss;
    for(size_t i = 0; i < maxbin_; ++i) {
      ss << sends_.at(i);
      values_.push_back(ss.str());
      ss.str("");
      ss << recvs_.at(i);
      values_.push_back(ss.str());
      ss.str("");
    }
    return values_;
  }
  handlerbase* lumper::clone() const {
    return new lumper(*this);
  }
  void lumper::mark_active(const dumpi_time*, const dumpi_time*,
                           const dumpi_perfinfo*)
  {}
  
  void lumper::mark_inactive(const dumpi_time *, const dumpi_time *,
                             const dumpi_perfinfo*)
  {}
  inline int binid(int64_t value) {
    int64_t one(1);
    int x = 0;
    while((one << x) < value) x++;
    return (x - 1);
  }
  void lumper::handle(dumpi_function func, uint16_t,
                      const dumpi_time*, const dumpi_time*,
                      const dumpi_perfinfo*,
                      int64_t bytes_sent, int,
                      int64_t bytes_recvd, int, const void*) 
  {
    if(collect_[func]) {
      if(bytes_sent > 0) {
        int bin = binid(bytes_sent);
        if(bin >= maxbin_) bin = maxbin_ - 1;
        sends_.at(bin)++;
      }
      if(bytes_recvd > 0) {
        int bin = binid(bytes_sent);
        if(bin >= maxbin_) bin = maxbin_ - 1;
        recvs_.at(bin)++;
      }
    }
  }

} // end of namespace dumpi