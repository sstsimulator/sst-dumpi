/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
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
