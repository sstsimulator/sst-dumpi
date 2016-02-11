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

#ifndef DUMPI_BIN_DUMPI2ASCII_CALLBACKS_H
#define DUMPI_BIN_DUMPI2ASCII_CALLBACKS_H

#include <dumpi/libundumpi/libundumpi.h>

#ifdef __cplusplus
//extern "C" {
#endif /* ! __cplusplus */

/**
 * \ingroup dumpi2ascii
 * \defgroup dumpi2ascii_callbacks dumpi2ascii/callbacks: \
 *    callback definitions (used in libundumpi) for converting \
 *    a binary DUMPI trace to ASCII format.
 */
/*@{*/

/**
 * A debugging sanity check -- used in place of a more "real" user arg.
 * If we need to, we could use the shared containers (static variables)
 * instead of this fake user arg.
 */
#define DUMPI_UARG (void*)0xFF00FF00 /* debugging sanity check */

/**
 * The address mapping is stored in a shared container.
 */
typedef struct d2a_addrmap {
  int        count;
  uint64_t  *address;
  char     **name;
} d2a_addrmap;

/** The shared container for address mapping */
extern d2a_addrmap *d2a_addr;

/** Define all our callbacks. */
void set_callbacks(libundumpi_callbacks *cbacks);

/*@}*/

#ifdef __cplusplus
//} /* End of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_BIN_DUMPI2ASCII_CALLBACKS_H */
