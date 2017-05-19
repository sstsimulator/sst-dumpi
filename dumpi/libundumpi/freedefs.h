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