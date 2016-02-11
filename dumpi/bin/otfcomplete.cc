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

#include <dumpi/bin/otfcomplete.h>

namespace dumpi {

  //
  // Add another outstanding request.
  //
  void otfcomplete::add(dumpi_request requestid, uint32_t recver, uint32_t sender,
			uint32_t procgroup, uint32_t tag, uint32_t length,
			uint32_t source)
  {
    if(pending_.find(requestid) != pending_.end())
      throw "otfcomplete::add:  Attempt to redefine active request id.";
    pending_[requestid] = args(recver, sender, procgroup, tag, length, source);
  }

  //
  // Complete an outstanding request.
  //
  void otfcomplete::complete(OTF_Writer *writer, uint64_t stop,
			     dumpi_request requestid)
  {
    pending_t::iterator it = pending_.find(requestid);
    if(it != pending_.end()) {
      args a = it->second;
      OTF_Writer_writeRecvMsg(writer, stop, a.recver, a.sender, a.procgroup,
			      a.tag, a.length, a.source);
      pending_.erase(it);
    }
  }

  //
  // Complete a list of outstanding requests (waitany or waitall).
  //
  void otfcomplete::complete_all(OTF_Writer *writer, uint64_t stop,
				 int count, const dumpi_request *requests)
  {
    for(int i = 0; i < count; ++i)
      this->complete(writer, stop, requests[i]);
  }

} // end of namespace dumpi
