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

#ifndef DUMPI_BIN_GROUP_H
#define DUMPI_BIN_GROUP_H

#include <stddef.h>
#include <vector>
#include <cstring>

namespace dumpi {

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  /**
   * Keep lifetime and state information for an MPI group.
   * Intended for C++-based utilities for parsing DUMPI information.
   */
  class group {
    /// My local rank within the group.
    int rank_;

    /// The global ranks in my group.
    std::vector<int> peers_;

  public:
    /// Create a null group.
  group() : rank_(-1)
      {}

    /// Create a group with the given membership for the given local rank.
    /// If the local rank is less than zero, the current node is not a
    /// member in the group.
  group(int local_rank, std::vector<int> peers) :
    rank_(local_rank), peers_(peers)
    {
      if(rank_ >= 0 && size_t(rank_) >= peers_.size())
	throw "group(local_rank, peers):  Invalid local rank";
      if(rank_ < 0) rank_ = -1;
    }

    /// Get the local rank of this node.
    int get_local_rank() const {
      return rank_;
    }

    /// Get the global rank of this node.
    int get_global_rank() const {
      if(rank_ >= 0) return peers_.at(rank_);
      else return -1;
    }

    /// Get the size of the group.
    int get_size() const {
      return static_cast<int>(peers_.size());
    }

    /// Get the underlying list of peers.
    const std::vector<int> get_peers() const {
      return peers_;
    }

    /// Get the global rank of one of our peers.
    int get_global_peer_rank(int local_rank) const {
      return peers_.at(local_rank);
    }

    /// Test whether two groups are the same.
    bool is_identical_to(const group &other) const {
      bool rv = (rank_ == other.rank_ &&
		 peers_.size() == other.peers_.size());
      if(rv) {
	for(size_t i = 0; i < peers_.size(); ++i)
	  if(peers_.at(i) != other.peers_.at(i)) {
	    rv = false;
	    break;
	  }
      }
      return rv;
    }
  };

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_GROUP_H