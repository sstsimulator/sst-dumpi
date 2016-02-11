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

#ifndef DUMPI_BIN_COMM_H
#define DUMPI_BIN_COMM_H

#include <dumpi/bin/group.h>

namespace dumpi {

  typedef int commid;

  /**
   * \addtogroup dumpi_utilities  dumpi_utilities:  C++ analysis utilities.
   */

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  /**
   * A wrapper class for communicators.
   * Intended for C++-based parsing/analysis utilities,
   * not used by libdumpi or libundumpi.
   */
  class comm {
    /// The unique identifier for this comm.
    commid index_;
    /// The group underlying this comm.
    group group_;

  public:
    /// Create an empty (NULL) communicator.
  comm() :
    index_(-1)
      {}

    /// Create a communicator around a group.
  comm(commid index, const group &gg) :
    index_(index), group_(gg)
    {
      if(group_.get_local_rank() < 0) {
	// This node is not a member of the group -- turn this into a
	// null communicator.
	index_ = -1;
	group_ = group();
      }
    }

    /// Get the unique communicator index.
    commid get_unique_index() const {
      return index_;
    }

    /// Get my group.
    const group& get_group() const {
      return group_;
    }
  
    /// Get the unique identifier for this communicator.
    commid get_id() const {
      return index_;
    }

    /// Test whether this is a null communicator.
    bool is_null_comm() const {
      return index_ < 0;
    }
  };

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_COMM_H
