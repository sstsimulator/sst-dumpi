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