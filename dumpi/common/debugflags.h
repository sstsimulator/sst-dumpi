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

#ifndef DUMPI_COMMON_DEBUGFLAGS
#define DUMPI_COMMON_DEBUGFLAGS

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common
   * \defgroup common_debugflags common/debugflags: \
   *      Debug flags for printf-style debugging.   \
   *      Remarkably, many HPC platforms don't have functional debuggers.
   */
  /*@{*/

  /** A shared variable controlling extend of debug output */
  extern uint64_t dumpi_debug;

  /*
   * The value of dumpi_debug is tested against the following bitmasks 
   * to decide whether to print debug messages to stderr or not
   */

  /** Turn off all debugging */
#define DUMPI_DEBUG_NONE     (0)
  /** Turn on all debugging */
#define DUMPI_DEBUG_ALL     (~0)
  /** Enable debugging of trace IO */
#define DUMPI_DEBUG_TRACEIO  (1)
  /** Enable more extensive trace IO debugging */
#define DUMPI_DEBUG_TRACEIO_VERBOSE ((1<<1) | DUMPI_DEBUG_TRACEIO)
  /** Enable debugging of MPI calls */
#define DUMPI_DEBUG_MPICALLS (1<<2)
  /** Enable debugging from within libdumpi */
#define DUMPI_DEBUG_LIBDUMPI (1<<3)

  /*@}*/

#ifdef __cplusplus
} /* end of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_DEBUGFLAGS */