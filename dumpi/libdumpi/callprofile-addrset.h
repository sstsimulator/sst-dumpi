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
