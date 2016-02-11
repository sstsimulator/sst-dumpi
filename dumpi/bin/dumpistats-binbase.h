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

#ifndef DUMPI_BIN_DUMPISTATS_BINBASE_H
#define DUMPI_BIN_DUMPISTATS_BINBASE_H

#include <dumpi/bin/dumpistats-handlers.h>
#include <dumpi/bin/trace.h>
#include <fstream>
#include <vector>
#include <map>

namespace dumpi {

  /**
   * Manage binning of data for dumpistats.
   */
  class binbase {
    /// Blocked copy constructor and assignment operator.
    binbase(const binbase&) {}
    void operator=(const binbase&) {}

  public:
    /// Default constructor needed (since copy constructor is blocked).
    binbase() {}

    /// Bye.
    virtual ~binbase() {}

    /// Clones all the given handlers.
    /// Handlers will be deallocated when this object goes out of scope.
    virtual void init(const std::string &binid,
                      const std::vector<trace> *traces,
                      const std::vector<handlerbase*> handlers) = 0;

    /// Process the given annotation call.  Ignored by default.
    virtual void annotate(const char* /*message*/,
                          uint16_t /*thread*/,
                          const dumpi_time* /*cpu*/,
                          const dumpi_time* /*wall*/,
                          const dumpi_perfinfo* /*perf*/)
    {}

    /// Process the given MPI call type.
    virtual void handle(dumpi_function func, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf,
                        int64_t bytes_sent, int to_global_rank,
                        int64_t bytes_recvd, int from_global_rank,
                        const void *dumpi_arg) = 0;

    /// Reset counters and internal state in preparation for a new trace.
    /// Forces a flush of all active handlers unless callcount_ is zero.
    virtual void start_trace(int rank) = 0;

    /// Reset current trace rank.
    virtual void reset_trace() = 0;
  };

} // end of namespace dumpi

#endif // ! DUMPI_BIN_DUMPISTATS_BINBASE_H
