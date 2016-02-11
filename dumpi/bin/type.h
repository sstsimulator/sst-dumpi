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

#ifndef DUMPI_BIN_TYPE_H
#define DUMPI_BIN_TYPE_H

#include <string>

namespace dumpi {

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  /**
   * Store info about an MPI type.
   * Intended for C++-based analysis utilities.
   */
  class type {
    /// The size of the type in bytes.
    int size_;
    /// A std::string description of the type, if available.
    std::string name_;

  public:
    /// Create a new type.
    explicit type(int size = -1, const std::string &name = "") :
    size_(size), name_(name)
    {}

    /// Access the type size.
    int get_size() const { 
      return size_;
    }

    /// Access the type name.
    const std::string& get_name() const {
      return name_;
    }
  };

  /*@}*/

} // end of namespace dumpi

#endif // !DUMPI_BIN_TYPE_H
