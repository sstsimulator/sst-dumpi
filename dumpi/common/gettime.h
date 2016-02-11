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

#ifndef DUMPI_COMMON_GETTIME_H
#define DUMPI_COMMON_GETTIME_H

#include <dumpi/common/io.h>
#include <dumpi/common/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \addtogroup common
   */
  /*@{*/

  /** 
   * Use high resulution timers (clock_gettime) to retrieve current cpu
   * and wall time.
   */ 
  void dumpi_get_time(dumpi_clock *cpu, dumpi_clock *wall);

  /*@}*/ 

# ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_GETTIME_H */

