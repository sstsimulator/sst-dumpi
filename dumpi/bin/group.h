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
