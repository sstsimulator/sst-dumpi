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

#ifndef DUMPI_BIN_DUMPISTATS_GATHERBIN_H
#define DUMPI_BIN_DUMPISTATS_GATHERBIN_H

#include <dumpi/bin/dumpistats-binbase.h>
#include <dumpi/bin/trace.h>
#include <fstream>
#include <vector>
#include <map>
#include <regex.h>

// Important types.
class dumpi_clock;

namespace dumpi {

  /**
   * Manage binning of data based on annotations for dumpistats.
   */
  class gatherbin : public binbase {
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
    int current_rank_;

    /// The identifier for this bin.
    std::string binid_;

    /// The number of dumps.  This gets suffixed to the identifier,
    /// so each dump will be stored in <outroot>-<binid>-<dumpid>.dat
    int dumpid_;

    /// State flags.
    bool initialized_, active_;

    /// The number of intervals collected for this bin.
    std::map<int, int> intervals_;

    /// The first and last timestamp in the currently active bin
    /// and the total time interval we have collected over.
    std::map<int, dumpi_clock> first_stamp_, last_stamp_;
    std::map<int, dumpi_clock> start_gather_, stop_gather_, tot_act_;

    /// The total number of intervals accumulated into this container.
    std::map<int, int> accumulated_;

    /// Store the description patterns.
    std::string start_pattern_, stop_pattern_;

    /// The regular expressions for start and stop.
    regex_t start_regex_, stop_regex_;

    /// State flag set to true if results are to be accumulated.
    bool accumulate_;

    /// Only bins with non-zero call counts are stored.
    std::map<int, std::string> fname_;
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

    /// Private method to parse the input expression into start and
    /// end patterns.
    /// \return any extra regex flags (e.g. REG_ICASE) or 0 if no extra flags.
    int parse_patterns(const std::string &expression);

    /// Blocked assignment operator.
    void operator=(const gatherbin&) {}

  public:
    /// Construct a new bin and give it "starting" and "stopping" patterns.
    /// \param start_patt  The annotation regex to turn collection on. If this
    ///                    pattern is blank, the bin will start active.
    /// \param end_patt    The annotation regex to turn collection off.  If this
    ///                    pattern is blan, the bin will not turn off.
    /// \param accumulate  If set to true, results will be accumulated across
    ///                    all active regions without resetting handlers.
    gatherbin(const std::string &expression, bool accumulate);

    /// Bye.
    virtual ~gatherbin();

    /// Clones all the given handlers.
    /// Handlers will be deallocated when this object goes out of scope.
    virtual void init(const std::string &binid,
                      const std::vector<trace> *traces,
                      const std::vector<handlerbase*> handlers);

    /// Process the given annotation call.
    virtual void annotate(const char *message, uint16_t thread,
                          const dumpi_time *cpu,
                          const dumpi_time *wall,
                          const dumpi_perfinfo *perf);

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

#endif // ! DUMPI_BIN_DUMPISTATS_GATHERBIN_H