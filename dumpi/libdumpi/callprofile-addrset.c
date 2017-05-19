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

#include <dumpi/libdumpi/callprofile-addrset.h>
#include <stdio.h>
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

  /**
   * Set up a simple hash of the addresses seen so we can later
   * write their names to a footer record.
   */
  typedef struct dumpi_addrlist {
    uint64_t *addr;
    int count, length;
  } dumpi_addrlist;

  /** Note that the number of buckets goes with the size of HASH_TYPE
   * HASH_TYPE unsigned short results in 32Ki buckets */
#define HASH_TYPE unsigned short

  /** Figure out how many buckets we can fit */
#define BUCKETS (1<<(8*sizeof(HASH_TYPE)))

  //typedef dumpi_addrlist addr_set[BUCKETS];
  /** Shared listing of addresses encountered so for.
   * This might be better managed using a set of init and free functions.
   */
  static dumpi_addrlist *addresses = NULL;

  /**
   * Insert an address on the shared address list.
   * This might be better handled without the shared back-end list.
   */
  static void insert_address(uint64_t address) {
    const HASH_TYPE *hash_view = NULL;
    int seen, i, range;
    HASH_TYPE hash = 0;
    range = sizeof(uint64_t) / sizeof(HASH_TYPE);
    /* Compute a hash of the address */
    hash_view = (const HASH_TYPE*)(address);
    for(i = 0; i < range; ++i) {
      hash ^= hash_view[i];
    }
    /* This may be a first time we enter this function */
    if(addresses == NULL) {
      addresses = (dumpi_addrlist*) calloc(BUCKETS, sizeof(dumpi_addrlist));
      assert(addresses != NULL);
    }
    /* and this may be the first time we see this hash */
    if(addresses[hash].length == 0) {
      addresses[hash].length = 4;
      addresses[hash].addr = (uint64_t*)calloc(addresses[hash].length,sizeof(uint64_t));
      assert(addresses[hash].addr != NULL);
      addresses[hash].count = 0; /* redundant, of course */
    }
    /* OK, figure out whether we've seen this address before */
    seen = 0;
    for(i = 0; i < addresses[hash].count; ++i) {
      if(addresses[hash].addr[i] == address) {
	seen = 1;
	break;
      }
    }
    /* If we have not seen this address before, it is time to insert it */
    if(seen == 0) {
      /* Do we need to make more space? */
      if(addresses[hash].count >= addresses[hash].length) {
	addresses[hash].length *= 2;
	addresses[hash].addr =
	  (uint64_t*)realloc(addresses[hash].addr,
			  addresses[hash].length * sizeof(uint64_t));
	assert(addresses[hash].addr != NULL);
      }
      /* Insert this address. */
      addresses[hash].addr[addresses[hash].count] = address;
      ++addresses[hash].count;
    }
  }

  /**
   * Get an iterator to the first valid pointer on the address list. 
   */
  dumpi_addr_iterator libdumpi_addr_begin_iterator() {
    dumpi_addr_iterator retval = {0, 0};
    if(addresses != NULL) {
      for(retval.bucket = 0; retval.bucket < BUCKETS; ++retval.bucket) {
	if(addresses[retval.bucket].count > 0) {
	  goto loop_done;
	}
      }
    }
    else {
      retval.bucket = BUCKETS;
    }
  loop_done:
    return retval;
  }

  /** Is this iterator valid? */
  int libdumpi_addr_valid_iterator(const dumpi_addr_iterator *it) {
    assert(it != NULL);
    return it->bucket < BUCKETS;
  }

  /**
   * Increment the given iterator.
   * return non-zero if the iterator is still valid after increment.
   */
  int libdumpi_addr_increment_iterator(dumpi_addr_iterator *it) {
    if(it->bucket < BUCKETS) {
      ++it->pos;
      if(it->pos >= addresses[it->bucket].count) {
	it->pos = 0;
	for(++it->bucket ; it->bucket < BUCKETS; ++it->bucket) {
	  if(addresses[it->bucket].count > 0) {
	    goto loop_done;
	  }
	}
      }
    }
  loop_done:
    return (it->bucket < BUCKETS);
  }

  /* Dereference the iterator. */
  uint64_t libdumpi_addr_iterator_value(const dumpi_addr_iterator *it) {
    assert(it->bucket<BUCKETS && it->pos<addresses[it->bucket].count);
    return addresses[it->bucket].addr[it->pos];
  }

  /*@}*/

#ifdef __cplusplus
} /* close extern "C" block */
#endif /* !__cplusplus */