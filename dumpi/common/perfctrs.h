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

#ifndef DUMPI_COMMON_PERFCTRS_H
#define DUMPI_COMMON_PERFCTRS_H

#include <dumpi/common/types.h>
#include <dumpi/common/perfctrtags.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common
   * \defgroup common_perfctrs common/perfctrs:  \
   *      Support for PAPI performance monitors
   */
  /*@{*/

  /**
   * Control whether we are getting the start (in) value or stop (out) value.
   */
  typedef enum dumpi_perfinfo_direction {
    DUMPI_PERFCTR_IN, DUMPI_PERFCTR_OUT
  } dumpi_perfinfo_direction;

  typedef char dumpi_perflabel_t[DUMPI_MAX_PERFCTR_NAME];

  /**
   * Create a perfinfo object that requests as many of the given
   * performance counters as possible (up to the value provided by PAPI
   * or DUMPI_MAX_PERFCTRS, whichever is smaller).
   *
   * Counters that are not provided by the current system are ignored.
   * All other counters are processed in first-in order, so if the system
   * supports (for example) 2 concurrent counters but 4 are requested
   * the first 2 supported ones will be turned on.
   */
  int dumpi_init_perfctrs(dumpi_perfinfo *ctrs);

  /**
   * Read PAPI performance counters.
   */
  void dumpi_get_perfctrs(const dumpi_profile *profile, dumpi_perfinfo *perf,
                          dumpi_perfinfo_direction direction);

  /**
   * How many perfcounters are currently active?
   */
  size_t dumpi_active_perfctrs(void);

  /**
   * What are the labels of currently active perfctrs?
   * Array at least of length dumpi_active_perfctrs().
   */
  dumpi_perflabel_t* dumpi_perfctr_labels(void);

  /*@}*/

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* DUMPI_COMMON_PERFCTRS_H */
