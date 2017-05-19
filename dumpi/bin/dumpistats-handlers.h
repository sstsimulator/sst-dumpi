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

#ifndef DUMPI_BIN_DUMPISTATS_HANDLERS_H
#define DUMPI_BIN_DUMPISTATS_HANDLERS_H

#include <dumpi/common/funclabels.h>
#include <dumpi/common/types.h>
#include <iostream>
#include <vector>
#include <map>
#include <stdint.h>

namespace dumpi {

  /**
   * Base class for handlers that manage statistics gathering for dumpistats.
   */
  class handlerbase {
  protected:
    mutable std::vector<std::string> desc_, values_;
    bool collect_[DUMPI_END_OF_STREAM];
    int  world_size_;

  public:
    handlerbase(const std::string &pattern);
    virtual ~handlerbase();

    /// Define world size.
    void set_world_size(int wsize) { world_size_ = wsize; }

    /// Reset all counters.
    virtual void reset() = 0;

    /// Get description(s) of the field(s) output by this handler.
    virtual const std::vector<std::string>& description() const = 0;

    /// Get the value(s) gathered so far.
    virtual const std::vector<std::string>& values() const = 0;

    /// Clone this object.
    virtual handlerbase* clone() const = 0;

    /// Mark the handler active starting at the given point
    virtual void mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                             const dumpi_perfinfo *perf) = 0;

    /// Mark the handler active starting as of the given point
    virtual void mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                               const dumpi_perfinfo *perf) = 0;

    /// New incoming call.
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg) = 0;
  };

  class counter : public handlerbase {
    int64_t count_;
  public:
    counter(const std::string &pattern);
    virtual ~counter();
    virtual void reset();
    virtual const std::vector<std::string>& description() const;
    virtual const std::vector<std::string>& values() const;
    virtual handlerbase* clone() const;
    virtual void mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                             const dumpi_perfinfo *perf);
    virtual void mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                               const dumpi_perfinfo *perf);
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg);
  };

  class timer : public handlerbase {
    dumpi_time last_cpu_, last_wall_;
    int64_t cpu_inside_mpi_, cpu_outside_mpi_;
    int64_t wall_inside_mpi_, wall_outside_mpi_;
  public:
    timer(const std::string &pattern);
    virtual ~timer();
    virtual void reset();
    virtual const std::vector<std::string>& description() const;
    virtual const std::vector<std::string>& values() const;
    virtual handlerbase* clone() const;
    virtual void mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                             const dumpi_perfinfo *perf);
    virtual void mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                               const dumpi_perfinfo *perf);
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg);
  };

  class sender : public handlerbase {
    int64_t bytes_;
  public:
    sender(const std::string &pattern);
    virtual ~sender();
    virtual void reset();
    virtual const std::vector<std::string>& description() const;
    virtual const std::vector<std::string>& values() const;
    virtual handlerbase* clone() const;
    virtual void mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                             const dumpi_perfinfo *perf);
    virtual void mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                               const dumpi_perfinfo *perf);
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg);
  };

  class recver : public handlerbase {
    int64_t bytes_;
  public:
    recver(const std::string &pattern);
    virtual ~recver();
    virtual void reset();
    virtual const std::vector<std::string>& description() const;
    virtual const std::vector<std::string>& values() const;
    virtual handlerbase* clone() const;
    virtual void mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                             const dumpi_perfinfo *perf);
    virtual void mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                               const dumpi_perfinfo *perf);
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg);
  };

  class exchanger : public handlerbase {
    std::string pat_;
    std::vector<int64_t> sent_;
    int collsent_, collrecvd_;
  public:
    exchanger(const std::string &pattern);
    virtual ~exchanger();
    virtual void reset();
    virtual const std::vector<std::string>& description() const;
    virtual const std::vector<std::string>& values() const;
    virtual handlerbase* clone() const;
    virtual void mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                             const dumpi_perfinfo *perf);
    virtual void mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                               const dumpi_perfinfo *perf);
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg);
  };

  class perfcounter : public handlerbase {
    typedef std::map<std::string, int64_t> tracker_t;
    tracker_t last_out_, inside_mpi_, outside_mpi_;
  public:
    perfcounter(const std::string &pattern);
    virtual ~perfcounter();
    virtual void reset();
    virtual const std::vector<std::string>& description() const;
    virtual const std::vector<std::string>& values() const;
    virtual handlerbase* clone() const;
    virtual void mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                             const dumpi_perfinfo *perf);
    virtual void mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                               const dumpi_perfinfo *perf);
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg);
  };

  class lumper : public handlerbase {
    static const int maxbin_;
    typedef std::vector<int> lump_t;
    lump_t sends_, recvs_;
    std::string pat_;
  public:
    lumper(const std::string &pattern);
    virtual ~lumper();
    virtual void reset();
    virtual const std::vector<std::string>& description() const;
    virtual const std::vector<std::string>& values() const;
    virtual handlerbase* clone() const;
    virtual void mark_active(const dumpi_time *cpu, const dumpi_time *wall,
                             const dumpi_perfinfo *perf);
    virtual void mark_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                               const dumpi_perfinfo *perf);
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg);
  };

} // end of namespace dumpi

#endif // ! DUMPI_BIN_DUMPISTATS_HANDLERS_H