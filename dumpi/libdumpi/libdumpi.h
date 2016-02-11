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

#ifndef DUMPI_LIBDUMPI_LIBDUMPI_H
#define DUMPI_LIBDUMPI_LIBDUMPI_H

#include <dumpi/common/constants.h>
#include <dumpi/common/settings.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \defgroup libdumpi libdumpi:                          \
   *    Major parts of the dumpi profiling library.        \
   *    Dumpi can be used either as a link-level library   \
   *    or for more fine-grained instrumentation (via this \
   *   header file).
   */
  /*@{*/ /* Define scope for subsequent doxygen comments */

  /**
   * Turn off all MPI profiling (call counts are still gathered.
   */
  void libdumpi_disable_profiling();

  /**
   * Turn MPI profiling (back) to what it was before disable_profiling.
   */
  void libdumpi_enable_profiling();
  
  /**
   * Set profiling preferences for the given MPI function.
   * This can be done when profiling is disabled; the new value will take
   * effect when libdumpi_enable_profiling is called.
   * Valid values for setting are DUMPI_DISABLE, DUMPI_SUCCESS, and DUMPI_ENABLE
   */
  void libdumpi_set_profiling(dumpi_function fun, dumpi_setting setting);

  /**
   * Specify the fileroot for profile output.
   * This call is only respected if it is made before MPI_Init.
   */
  void libdumpi_set_output(const char *froot);

  /**
   * Specify whether status information should be output in the
   * trace file.  This call can be made at any time (e.g. when trying
   * to figure out status problems in select calls).
   */
  void libdumpi_set_status_output(dumpi_setting setting);

  /**
   * Access the count of MPI calls made to date.
   * Indexing is based on the dumpi_function enum
   * including DUMPI_ALL_FUNCTIONS.
   */
  const uint32_t* libdumpi_call_counts();

  /*@}*/ /* close scope of doxygen comments */

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBDUMPI_LIBDUMPI_H */
