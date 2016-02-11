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

#ifndef DUMPI_LIBDUMPI_CALLPROFILE_H
#define DUMPI_LIBDUMPI_CALLPROFILE_H

#include <dumpi/dumpiconfig.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

#ifdef DUMPI_ENABLE_INSTRUMENTATION

  /**
   * \addtogroup libdumpi_internal_callprofile
   */
  /*@{*/

  /**
   * Indicate entry into a profiled function.
   * gcc and icc (in gcc compatibility mode) call this function
   * at entry into any function.
   */
  void DUMPI_SKIP_INSTRUMENTING
  __cyg_profile_func_enter(void *fn, void *site);
  
  /**
   * Indicate exit from a profiled function.
   * If this reduces the call depth to zero, force output to a trace file.
   */
  void DUMPI_SKIP_INSTRUMENTING
  __cyg_profile_func_exit(void *fn, void *site);
  
#endif /* ! DUMPI_ENABLE_INSTRUMENTATION */

  /**
   * Insert an arbitrary annotation.
   */
  void libdumpi_insert_data(uint64_t key, const char *data);

  /**
   * Get the function name associated with the given program position.
   * Only works on systems that support dladdr -- otherwise returns the
   * std::string "(none)".
   */
  const char* DUMPI_SKIP_INSTRUMENTING dumpi_function_name(uint64_t fn);

  /**
   * Get the number of unique function addresses profiled so far.
   */
  int DUMPI_SKIP_INSTRUMENTING dumpi_profile_unique_address_count(void);
  
  /**
   * Get a listing of all unique function addresses profiled so far.
   * It is the responsibility of the caller to free the allocated array.
   *
   * The number of valid entries in the array is
   * dumpi_profile_unique_address_count().
   *
   * For convenience, the array is also NULL terminated (the entry at
   * [dumpi_profile_unique_address_count()] has the value NULL).
   */
  uint64_t* DUMPI_SKIP_INSTRUMENTING dumpi_profile_unique_addresses(void);

  /*@}*/

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBDUMPI_CALLPROFILE_H */

