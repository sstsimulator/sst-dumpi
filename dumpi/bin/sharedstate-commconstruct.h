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

#ifndef DUMPI_BIN_SHAREDSTATE_COMMCONSTRUCT_H
#define DUMPI_BIN_SHAREDSTATE_COMMCONSTRUCT_H

#include <dumpi/bin/sharedstate.h>
#include <vector>
#include <map>
#include <set>

namespace dumpi {

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  /**
   * Maintain information for constructing new communicators.
   * Particularly useful for MPI_Comm_split.
   */
  class sharedstate::commconstruct {
    /// The target size (number of peers).
    int target_size_;
    /// Mark whether each index has been fetched.
    std::set<int> fetched_;
  protected:
    /// Get the target size.
    int target_size() const { return target_size_; }
    /// Mark the given index as fetched.
    void mark_fetched(int global_rank);
  public:
    commconstruct(int size);
    virtual ~commconstruct();
    virtual bool is_complete() const = 0;
    virtual size_t size() const = 0;
    virtual const comm& get_comm(int global_rank) = 0;
    virtual commconstruct* clone() const = 0;
    // Query for whether this communicator is now done
    // (safe to erase from any pending list).
    bool is_fully_fetched() const {
      return (fetched_.size() == size_t(target_size_));
    }
  };

  class sharedstate::commdup : public sharedstate::commconstruct {
    /// This will be the id of communicators we're building
    const commid id_;
    /// This is the comm we're building from -- must be the same for all inputs
    commid templateid_;
    /// This is the set of communicators we're building.
    typedef std::map<int,comm> result_t;
    result_t result_;
  public:
    commdup(commid new_id, int size);
    virtual ~commdup();
    void add(const comm &incomm);
    virtual bool is_complete() const;
    virtual size_t size() const;
    virtual const comm& get_comm(int global_rank);
    virtual commconstruct* clone() const;
  };

  class sharedstate::commcreate : public sharedstate::commconstruct {
    /// Our new communicator id.
    const commid id_;
    /// This is the comm we're building from -- must be the same for all inputs
    commid templateid_;
    /// These are the nodes that need to appear in each input group.
    /// The order does not have to be consistent, but the memership does.
    std::set<int> nodes_;
    /// This is the set of communicators we're building.
    typedef std::map<int,comm> result_t;
    result_t result_;
  public:
    commcreate(commid new_id, int size);
    virtual ~commcreate();
    void add(const comm &incomm, const group &membership);
    virtual bool is_complete() const;
    virtual size_t size() const;
    virtual const comm& get_comm(int global_rank);
    virtual commconstruct* clone() const;
  };

  class sharedstate::commsplit : public sharedstate::commconstruct {
    /// The ids we will be associating with our new comms.
    const commid begin_, end_;
    /// This is the comm we're building from -- must be the same for all inputs
    commid templateid_;
    /// Keep count of how many entrants have entered so far.
    int entered_;
    /// Within each new communicator, we rank the entrants by key.
    /// May need to identify better way to handle colliding keys.
    typedef std::multimap<int, int> key_to_global_rank_t;
    /// Stash entries based on color.
    typedef std::map<int, key_to_global_rank_t> color_group_t;
    color_group_t color_group_;
    /// This is the set of communicators we're building.
    typedef std::map<int,comm> result_t;
    result_t result_;
  public:
    commsplit(commid first_id, commid not_to_reach, int size);
    virtual ~commsplit();
    void add(const comm &incomm, int color, int key);
    virtual bool is_complete() const;
    virtual size_t size() const;
    virtual const comm& get_comm(int global_rank);
    virtual commconstruct* clone() const;
  };

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_SHAREDSTATE_COMMCONSTRUCT_H