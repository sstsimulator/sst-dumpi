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

#ifndef DUMPI_LIBUNDUMPI_FREEDEFS_H
#define DUMPI_LIBUNDUMPI_FREEDEFS_H

/* Type definitions to handle freeing up memory */

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \addtogroup libundumpi_internal
   */
  /*@{*/ /* plop all subsequent comments in the callbacks group */

  /** A special token to indicate that we're working with a
   * NULL-terminated character array */
#define DUMPI_NULLTERM -1

  /** Free an array of dumpi_status objects */
#define DUMPI_FREE_STATUS(VAL) free(VAL)
  
  /** Free an array of dumpi_request objects */
#define DUMPI_FREE_REQUEST(VAL) free(VAL)

  /** Free a dynamically allocated character array */
#define DUMPI_FREE_CHAR(VAL) free(VAL)
  
  /** Free an irregular array of character arrays */
#define DUMPI_FREE_CHAR_ARRAY_2(COUNT, ARR) do {      \
  int i;                                        \
  if((COUNT) >= 0) {                            \
    for(i = 0; i < (COUNT); ++i)                \
      free((ARR)[i]);                           \
  }                                             \
  else {                                        \
    for(i = 0; ((ARR)[i]) != NULL; ++i)         \
      free((ARR)[i]);                           \
  }                                             \
} while(0)

  /** Free an irregular array-of-arrays-of-character arrays
   * (whoever created MPI_Comm_spawn_multiple was insane) */
#define DUMPI_FREE_CHAR_ARRAY_3(X, Y, ARR) do {                 \
    int i, j;                                             \
  if((X) >= 0) {                                          \
    for(i = 0; i < (X); ++i) {                            \
      if((Y) >= 0) {                                      \
        for(j = 0; j < (Y); ++j) {                        \
          free((ARR)[i][j]);                              \
        }                                                 \
        free((ARR)[i]);                                   \
      }                                                   \
      else {                                              \
        for(j = 0; ((ARR)[i][j]) != NULL; ++j) {          \
          free((ARR)[i][j]);                              \
        }                                                 \
        free((ARR)[i]);                                   \
      }                                                   \
    }                                                     \
  }                                                       \
  else {                                                  \
    for(i = 0; ((ARR)[i]) != NULL; ++i) {                 \
      if((Y) >= 0) {                                      \
        for(j = 0; j < (Y); ++j) {                        \
          free((ARR)[i][j]);                              \
        }                                                 \
        free((ARR)[i]);                                   \
      }                                                   \
      else {                                              \
        for(j = 0; ((ARR)[i][j]) != NULL; ++j) {          \
          free((ARR)[i][j]);                              \
        }                                                 \
        free((ARR)[i]);                                   \
      }                                                   \
    }                                                     \
  }                                                       \
} while(0)

  /** Free an integer array */
#define DUMPI_FREE_INT(VAL) free(VAL)

  /** Free an irregularly shaped integer array */
#define DUMPI_FREE_INT_ARRAY_2(COUNT, ARR) do {       \
  int i;                                        \
  if((COUNT) >= 0) {                            \
    for(i = 0; i < (COUNT); ++i)                \
      free((ARR)[i]);                           \
  }                                             \
  else {                                        \
    for(i = 0; ((ARR)[i]) != NULL; ++i)         \
      free((ARR)[i]);                           \
  }                                             \
} while(0)

  /** Free an array of type dumpi_datatype */
#define DUMPI_FREE_DATATYPE(VAL) free(VAL)

  /** Free an array of type dumpi_distribution */
#define DUMPI_FREE_DISTRIBUTION(VAL) free(VAL)

  /** Free an array of type dumpi_errorcode */
#define DUMPI_FREE_ERRCODE(VAL) free(VAL)

  /** Free an array of type dumpi_info */
#define DUMPI_FREE_INFO(VAL) free(VAL)

  /** Free an array of type dumpi_request 
   * (more fun originating from MPI_Comm_spawn_multiple) */
#define DUMPIO_FREE_REQUEST(VAL) free(VAL)

  /*@}*/ /* close the scope of doxygen comments */

#ifdef __cplusplus
} /* End of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBUNDUMPI_FREEDEFS_H */
