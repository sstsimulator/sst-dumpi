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