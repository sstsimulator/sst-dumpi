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

#ifndef DUMPI_BIN_DUMPISTATS_TIMEBIN_H
#define DUMPI_BIN_DUMPISTATS_TIMEBIN_H

#include <dumpi/bin/dumpistats-binbase.h>
#include <dumpi/bin/trace.h>
#include <fstream>
#include <vector>
#include <map>

namespace dumpi {

  /**
   * Manage binning of data based on time stamps for dumpistats.
   */
  class timebin : public binbase {
  public:
    /// The reference point for bounds -- some of these may need to be
    /// re-assessed for each trace file.
    enum refpoint {BEGIN, END, INIT, FINALIZE, ABSOLUTE};

  private:
    /// The statistics handlers/collectors for this bin.
    std::vector<handlerbase*> handlers_;

    /// The preparsed trace info.
    const std::vector<trace> *traces_;

    /// The number of MPI calls encountered during a collection.
    int callcount_;

    /// Current bin index.
    int current_bin_;

    /// Current trace rank.
    int64_t current_rank_;

    /// The identifier for this bin.
    std::string binid_;

    /// The number of dumps.  This gets suffixed to the identifier,
    /// so each dump will be stored in <outroot>-<binid>-<dumpid>.dat
    int dumpid_;

    /// State flags.
    bool initialized_, active_;

    /// First timestamp seen in a bin.
    dumpi_time first_t_;
    /// Last active stamps encountered -- used to wrap up last trace
    /// if the bin exist while active.
    dumpi_time last_cpu_, last_wall_;
    dumpi_perfinfo last_perf_;

    refpoint  begin_ref_, end_ref_;

    /// The bias for the start and end points.
    /// If refpoint is ABSOLUTE, this is the actual reference point.
    int64_t begin_bias_, end_bias_;

    /// The bin size for binned runs.  A value of zero means no binning.
    int64_t bin_size_;

    /// The time bounds we are collecting over.
    /// These are re-evaluated every time a new trace is started.
    int64_t begin_, end_;

    /// Store the description pattern -- some options cannot be fully resolved
    /// until the preparse information is available.
    std::string desc_;

    /// Only bins with non-zero call counts are stored.
    std::map<int, std::ofstream*> file_;

    /// Private method to dump output.
    void dump_output();

    /// Private method to clear counters.
    void clear_handlers();

    /// Private method to activate all handlers.
    void mark_handlers_active(const dumpi_time *cpu, const dumpi_time *wall,
                              const dumpi_perfinfo *perf);

    /// Private method to deactivate all handlers.
    void mark_handlers_inactive(const dumpi_time *cpu, const dumpi_time *wall,
                                const dumpi_perfinfo *perf);

    /// Private method to turn a description pattern into a time window.
    void compute_interval();

    /// Private method to figure out time stamps.
    int64_t resolve_time(refpoint type, int64_t bias, const trace &curr) const;

    /// Blocked assignment operator.
    void operator=(const timebin&) {}

  public:
    /// Construct a new bin and give it a task.
    timebin(const std::string &desc);

    /// Bye.
    virtual ~timebin();

    /// Clones all the given handlers.
    /// Handlers will be deallocated when this object goes out of scope.
    virtual void init(const std::string &binid,
                      const std::vector<trace> *traces,
                      const std::vector<handlerbase*> handlers);

    /// Process the given MPI call type.
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg);

    /// Reset counters and internal state in preparation for a new trace.
    /// Forces a flush of all active handlers unless callcount_ is zero.
    virtual void start_trace(int rank);

    /// Reset current trace rank.
    virtual void reset_trace();
  };

} // end of namespace dumpi

#endif // ! DUMPI_BIN_DUMPISTATS_TIMEBIN_H