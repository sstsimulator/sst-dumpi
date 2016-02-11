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

#ifndef DUMPI_COMMON_SETTINGS_H
#define DUMPI_COMMON_SETTINGS_H

#include <dumpi/common/funclabels.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common_internal
   */
  /*@{*/

  /**
   * The settings for whether specific functions should be profiled or not.
   * Tested as bitmasks, be careful before changing default values.
   */
  typedef enum dumpi_setting {
    DUMPI_DISABLE=0, DUMPI_SUCCESS=1, DUMPI_ALWAYS=(1<<1),
    DUMPI_ENABLE=(DUMPI_SUCCESS | DUMPI_ALWAYS)
  } dumpi_setting;

  /**
   * The controls for what timestamp information gets/was output.
   * Tested as a bitmask, be careful before changing default values.
   */
  typedef enum dumpi_timestamps {
    DUMPI_TIME_NONE=0, DUMPI_TIME_CPU=(1<<2), DUMPI_TIME_WALL=(1<<3),
    DUMPI_TIME_FULL=(DUMPI_TIME_CPU|DUMPI_TIME_WALL)
  } dumpi_timestamps;

  /** Output statuses */
#define DUMPI_ENABLE_STATUS_MASK DUMPI_ENABLE
  /** Output CPU times */
#define DUMPI_CPUTIME_MASK       DUMPI_TIME_CPU
  /** Output wall clock */
#define DUMPI_WALLTIME_MASK      DUMPI_TIME_WALL
  /** Output thread id */
#define DUMPI_THREADID_MASK      (1<<6)
  /** Output PAPI counter info */
#define DUMPI_PERFINFO_MASK      (1<<7)

  /*@}*/

#ifdef __cplusplus
} /* End of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_SETTINGS_H */

