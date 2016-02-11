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

#ifndef DUMPI_BIN_SHAREDSTATE_H
#define DUMPI_BIN_SHAREDSTATE_H

#include <dumpi/bin/comm.h>
#include <dumpi/bin/group.h>
#include <map>
#include <vector>

namespace dumpi {

  typedef int commhandle;

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  /**
   * Shared back-end state for constructing communicators.
   * Intended for C++-based utilities for analyzing DUMPI traces.
   */
  class sharedstate {
    /// Unique identifer for the "next" communicator.
    /// World is number 1.  Negative indices are not used except as
    /// uninitialized values.
    commid nextid_;

    /// The nodes in my world group
    std::vector<int> ranks_;

    /// This is the handle for the next comm creation request.
    commhandle nexthandle_;

    /// Nested types used in managing communicator construction.
    class commconstruct;
    class commdup;    // derived from commconstruct.
    class commcreate; // ditto
    class commsplit;  // ditto

    /// Communicators under construction.  Mapped by the unique id of the
    /// input communicator.
    typedef std::map< commid, commconstruct* > pending_t;
    pending_t pending_;

    /// Allow query by handle.
    struct entrant {
    entrant() : rank(-1) {}
    entrant(int rr, commconstruct* pp) : rank(rr), ptr(pp) {}
      int rank;
      commconstruct* ptr;
    };
    typedef std::map<commhandle, entrant> handle_t;
    handle_t handles_;

  public:
    /// Create a new shared state. Constructs world.
    sharedstate(int size);
  
    /// Get the world communicator for the given rank.
    comm retrieve_world(int rank);

    /// Get a 'self' communicator for the given rank.
    comm retrieve_self(int rank);

    /// Get the newly constructed communicator corresponding to the given handle.
    comm retrieve_comm(commhandle handle);

    /// Test whether the given communicator is complete.
    bool is_complete(commhandle handle) const;

    /// Duplicate an existing communicator (MPI_Comm_dup).
    /// Returns a handle to retrieve the completed communicator.
    commhandle comm_dup(const comm &incomm);

    /// Create a communicator (MPI_Comm_create) from the given 
    /// input communicator.  Returns a handle that can be used to
    /// retrieve the completed communicator once it is complete.
    commhandle comm_create(const comm &incomm, const group &membership);

    /// Create a group of communicators (MPI_Comm_split) at the same time.
    /// Returns a handle to retrieve the completed communicator.
    commhandle comm_split(const comm &incomm, int color, int key);
  };

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_SHAREDSTATE_H
