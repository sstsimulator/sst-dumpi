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