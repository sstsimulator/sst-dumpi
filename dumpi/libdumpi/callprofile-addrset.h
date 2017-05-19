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

#ifndef DUMPI_LIBDUMPI_CALLPROFILE_ADDRSET_H
#define DUMPI_LIBDUMPI_CALLPROFILE_ADDRSET_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */

  /**
   * \ingroup libdumpi_internal
   * \defgroup libdumpi_internal_callprofile libdumpi/internal/callprofile: \
   *    Functionality to track addresses in a call profile generated        \
   *    using -finstrument-functions
   */
  /*@{*/

  /** Allow semi-sensible iteration through the buckets */
  typedef struct dumpi_addr_iterator {
    int bucket;
    int pos;
  } dumpi_addr_iterator;

  /**
   * Insert an address on the shared address list.
   * Uses libdl (if available) to figure out the name at this address.
   */
  void libdumpi_insert_address(void *address);

  /**
   * Get an iterator to the first valid pointer on the address list. 
   */
  dumpi_addr_iterator libdumpi_addr_begin_iterator();

  /** Test whether an iterator is valid */
  int libdumpi_addr_valid_iterator(const dumpi_addr_iterator *it);

  /**
   * Increment the given iterator.
   * return non-zero if the iterator is still valid after increment.
   */
  int libdumpi_addr_increment_iterator(dumpi_addr_iterator *it);

  /* Dereference the given iterator. */
  uint64_t libdumpi_addr_iterator_value(const dumpi_addr_iterator *it);

  /*@}*/

#ifdef __cplusplus
} /* close extern "C" block */
#endif /* !__cplusplus */

#endif /* ! DUMPI_LIBDUMPI_CALLPROFILE_ADDRSET_H */