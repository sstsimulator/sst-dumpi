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