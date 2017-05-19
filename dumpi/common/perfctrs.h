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