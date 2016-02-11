/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009-2011 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
 */

#ifndef DUMPI_COMMON_BYTESWAP_H
#define DUMPI_COMMON_BYTESWAP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * Defgroup byteutils:  Basic byte swap functionality which was, for some
   * reason, omitted on Catamount compute nodes.
   *
   * Written for legibility -- mask-and-shift is likely a little faster
   * but simply isn't worth the effort for this platform.
   */
  /*@{*/

  /**
   * Swap byte order on a 32-bit entry.  Assumes 8-bit bytes.
   */
  static inline uint32_t dumpi_swap32(uint32_t val) {
    uint32_t v2;
    const char *inp = (const char*)(&val);
    char *outp = (char*)(&v2);
    outp[0] = inp[3];
    outp[1] = inp[2];
    outp[2] = inp[1];
    outp[3] = inp[0];
    return v2;
  }

  /**
   * Swap byte order on a 16-bit entry.  Assumes 8-bit bytes.
   */
  static inline uint16_t dumpi_swap16(uint16_t val) {
    uint16_t v2;
    const char *inp = (const char*)(&val);
    char *outp = (char*)(&v2);
    outp[0] = inp[1];
    outp[1] = inp[0];
    return v2;
  }

  /*@}*/

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_BYTESWAP_H */
