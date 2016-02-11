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

#ifndef DUMPI_BIN_METADATA_H
#define DUMPI_BIN_METADATA_H

#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <errno.h>

namespace dumpi {

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  /**
   * Maintain metadata from a dumpi metafile.
   * Intended for C++-based utilities for parsing DUMPI information.
   */
  class metadata {
    int numprocs_;
    std::string fileprefix_;
    /// This gets expanded to be the sprintf-format needed to open trace files.
    std::string tracefmt_;
    /// The number of digit in the filename.
    int width_;

  public:
    /// Create new metadata based on the given metafile.
    metadata(const std::string &metafile);

    /// Number of traces.
    int traces() const {
      return numprocs_;
    }

    /// Get the full filename corresponding to the given trace index.
    std::string tracename(int index) const {
      static char buf[1024];
      if(index >= numprocs_)
	throw "metadata::tracename:  Invalid rank";
      if(snprintf(buf, 1024, tracefmt_.c_str(), index) > 1023)
	throw "metadata::tracename:  Buffer overflow.";
      return std::string(buf);
    }
  };

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_METADATA_H
