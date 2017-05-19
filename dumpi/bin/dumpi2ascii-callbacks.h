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