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

#ifndef DUMPI_BIN_OTFCOMPLETE_H
#define DUMPI_BIN_OTFCOMPLETE_H

#include <dumpi/common/types.h>
#include <otf.h>
#include <map>

namespace dumpi {

  /**
   * \defgroup dumpi2otf  dumpi2otf:  Convert DUMPI traces to OTF format.
   */
  /*@{*/

  /**
   * Keep track of outstanding irecv requests.
   */
  class otfcomplete {
    /// Store our outstanding requests.
    struct args {
      uint32_t recver, sender, procgroup, tag, length, source;
    args() :
      recver(0), sender(0), procgroup(0), tag(0), length(0), source(0)
      {}
    args(uint32_t r, uint32_t s, uint32_t p, uint32_t t, uint32_t l, uint32_t x):
      recver(r), sender(s), procgroup(p), tag(t), length(l), source(x)
      {}
    };

    /// The list of outstanding requests.
    typedef std::map<dumpi_request, args> pending_t;
    pending_t pending_;

  public:
    /// Add another outstanding request.
    void add(dumpi_request requestid, uint32_t recver, uint32_t sender,
	     uint32_t procgroup, uint32_t tag, uint32_t length, uint32_t source);

    /// Complete an outstanding request.
    /// This is a no-op if the request is not defined (e.g. isend).
    void complete(OTF_Writer *writer, uint64_t stop, dumpi_request requestid);

    /// Complete a list of outstanding requests (waitany or waitall).
    void complete_all(OTF_Writer *writer, uint64_t stop,
		      int count, const dumpi_request *requests);
  };

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_OTFCOMPLETE_H
