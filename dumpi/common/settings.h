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

#ifndef DUMPI_COMMON_SETTINGS_H
#define DUMPI_COMMON_SETTINGS_H

#include <dumpi/common/funclabels.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common_internal
   */
  /*@{*/

  /**
   * The settings for whether specific functions should be profiled or not.
   * Tested as bitmasks, be careful before changing default values.
   */
  typedef enum dumpi_setting {
    DUMPI_DISABLE=0, DUMPI_SUCCESS=1, DUMPI_ALWAYS=(1<<1),
    DUMPI_ENABLE=(DUMPI_SUCCESS | DUMPI_ALWAYS)
  } dumpi_setting;

  /**
   * The controls for what timestamp information gets/was output.
   * Tested as a bitmask, be careful before changing default values.
   */
  typedef enum dumpi_timestamps {
    DUMPI_TIME_NONE=0, DUMPI_TIME_CPU=(1<<2), DUMPI_TIME_WALL=(1<<3),
    DUMPI_TIME_FULL=(DUMPI_TIME_CPU|DUMPI_TIME_WALL)
  } dumpi_timestamps;

  /** Output statuses */
#define DUMPI_ENABLE_STATUS_MASK DUMPI_ENABLE
  /** Output CPU times */
#define DUMPI_CPUTIME_MASK       DUMPI_TIME_CPU
  /** Output wall clock */
#define DUMPI_WALLTIME_MASK      DUMPI_TIME_WALL
  /** Output thread id */
#define DUMPI_THREADID_MASK      (1<<6)
  /** Output PAPI counter info */
#define DUMPI_PERFINFO_MASK      (1<<7)

  /*@}*/

#ifdef __cplusplus
} /* End of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_SETTINGS_H */