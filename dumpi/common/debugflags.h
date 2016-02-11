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
