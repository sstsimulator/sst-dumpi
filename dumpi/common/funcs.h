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

#ifndef DUMPI_COMMON_FUNCS_H
#define DUMPI_COMMON_FUNCS_H

#include <dumpi/common/settings.h>
#include <stdio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \addtogroup common_internal
   */
  /*@{*/

  /**
   * Version numbering.  Will probably replace this with autotools versioning.
   */
  extern const int8_t dumpi_version;
  extern const int8_t dumpi_subversion;
  extern const int8_t dumpi_subsubversion;

  /**
   * Test whether we meet at least the given version number.
   */
  inline static int dumpi_have_version(const char version_array[3],
				       int min_version,
				       int min_subversion,
				       int min_subsubversion)
  {
    int retval;
    if(version_array[0] != min_version) {
      retval = (version_array[0] > min_version);
    }
    else if(version_array[1] != min_subversion) {
      retval = (version_array[1] > min_subversion);
    }
    else {
      retval = (version_array[2] >= min_subsubversion);
    }
    return retval;
  }

  /**
   * String names for the functions -- used for ascii output
   * The last entry (DUMPI_END_OF_STREAM) is NULL (sentinel).
   */
  extern const char *dumpi_function_names[DUMPI_END_OF_STREAM+1];

  /**
   * Get the name of the given function.
   * Same as calling dumpi_function_names[...], except with bounds checking.
   * Returns "DUMPI_NO_SUCH_FUNCTION" if function name is out of bounds. 
   */
  const char* dumpi_get_function_name(int funcid);

  /*@}*/

#ifdef __cplusplus
}
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_FUNCS_H */
