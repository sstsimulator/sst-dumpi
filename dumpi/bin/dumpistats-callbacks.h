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

#ifndef DUMPI_BIN_DUMPISTATS_CALLBACKS_H
#define DUMPI_BIN_DUMPISTATS_CALLBACKS_H

#include <dumpi/bin/dumpistats-binbase.h>
#include <dumpi/bin/metadata.h>
#include <dumpi/bin/trace.h>
#include <dumpi/libundumpi/callbacks.h>
#include <map>

namespace dumpi {

  /**
   * Callbacks for dumpistats.
   */
  class callbacks {
    libundumpi_callbacks cb;
    /// Temporary stash for preparse state -- goes out of scope after parse(...).
    std::vector<trace> *trace_;
    std::vector<binbase*> *bin_;
    /// Temporary stash for current trace rank.
    int current_trace_;
    /// All valid function entry addresses
    std::map<uint64_t, std::string> labels_;

  public:
    /// Setup.
    callbacks();

    /// Run through all the traces in the given metafile
    void go(const metadata &meta, std::vector<trace> &trace,
            std::vector<binbase*> &bin);

    /// Forward a call to all bins.
    void handle(dumpi_function func, uint16_t thread,
                const dumpi_time *cpu, const dumpi_time *wall,
                const dumpi_perfinfo *perf,
                int64_t bytes_sent, int to_global_rank,
                int64_t bytes_recvd, int from_global_rank,
                const void *dumpi_arg);

    /// Given an annotation, pass it on to the handlers.
    /// Valid annotations are assumed to start with "ANNOTATE: "
    void annotate(const dumpi_func_call *prm, uint16_t thread,
                  const dumpi_time *cpu, const dumpi_time *wall,
                  const dumpi_perfinfo *perf);
  
    /// Get the size of the given type.
    int typesize(dumpi_datatype dtype, const dumpi_clock &when) const;

    /// Get the size of the given communicator.
    int commsize(dumpi_comm comm, const dumpi_clock &when) const;

    /// Get the local rank of the current node in the given communicator.
    int commrank(dumpi_comm comm, const dumpi_clock &when) const;

    /// Get the global rank of the given node in the given communicator.
    int globalrank(int localrank, dumpi_comm comm, const dumpi_clock &when)const;
  };

} // end of namespace dumpi

#endif // ! DUMPI_BIN_DUMPISTATS_CALLBACKS_H