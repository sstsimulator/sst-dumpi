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

#include <dumpi/dumpiconfig.h>
#include <dumpi/common/gettime.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>

#ifdef DUMPI_ON_REDSTORM
#include <catamount/dclock.h>
#endif

#ifdef DUMPI_ON_BGP
/*#include <stdio.h>*/
#include <spi/kernel_interface.h>
#include <common/bgp_personality.h>
#include <common/bgp_personality_inlines.h>
#endif

#ifdef DUMPI_ON_BGQ
#include <personality.h>
#include <bqc/A2_inlines.h>
#endif

static inline void get_getrusage(dumpi_clock *cpu) {
#ifdef HAVE_GETRUSAGE
  struct rusage r;
  getrusage(RUSAGE_SELF,&r);
  cpu->sec = (int32_t) (r.ru_utime.tv_sec + r.ru_stime.tv_sec);
  cpu->nsec = 1000 * ( r.ru_utime.tv_usec + r.ru_stime.tv_usec );
#else /* ! HAVE_GETRUSAGE */
  cpu->sec = cpu->nsec = 0;
#endif
}

static inline void get_gettimeofday(dumpi_clock *wall) {
#ifdef HAVE_GETTIMEOFDAY
  struct timeval tod;
  gettimeofday(&tod,0);
  wall->sec = (int32_t) tod.tv_sec;
  wall->nsec = 1000 * tod.tv_usec;
#else /* ! HAVE_GETTIMEOFDAY */
  wall->sec = wall->nsec = 0;
#endif
}

#ifdef DUMPI_ON_REDSTORM
static inline void get_redstorm_timers(dumpi_clock *cpu, dumpi_clock *wall) {
  double wallt = dclock();
  wall->sec = wallt;
  wall->nsec = (wallt-wall->sec)*1e9;
  get_getrusage(cpu);
}
#endif

#ifdef DUMPI_ON_BGQ
static inline void get_bluegene_timers(dumpi_clock *cpu, dumpi_clock *wall) {

  static uint32_t bgq_clockMHz = 0;
  static double bgq_persec = 0.0;
  double wallt;
  get_getrusage(cpu);
  if (bgq_clockMHz == 0) {
      Personality_t pers;
      Kernel_GetPersonality(&pers, sizeof(pers));
      bgq_clockMHz = Personality_clockMHz(&pers);
      bgq_persec = 1e-6/(double)bgq_clockMHz;
  }
  wallt = GetTimeBase() * bgq_persec;
  wall->sec = wallt;
  wall->nsec = (wallt-wall->sec)*1e9;
}
#endif

#ifdef DUMPI_ON_BGP 
static inline void get_bluegene_timers(dumpi_clock *cpu, dumpi_clock *wall) {

  static uint32_t bgp_clockMHz = 0;
  static double bgp_persec = 0.0;
  double wallt;
  get_getrusage(cpu);
  if (bgp_clockMHz == 0) {
      _BGP_Personality_t mybgp;
      Kernel_GetPersonality(&mybgp, sizeof(_BGP_Personality_t));
      bgp_clockMHz = BGP_Personality_clockMHz(&mybgp);
      bgp_persec = 1e-6/(double)bgp_clockMHz;
#if BGP_VERBOSE_TIMER_TEST
      printf("dumpi_get_time(): bgp_clockMHz=%u bgp_persec=%e\n",bgp_clockMHz, bgp_persec);
      uint64_t t1 = _bgp_GetTimeBase();
      sleep(1);
      uint64_t t2 = _bgp_GetTimeBase();
      printf("dumpi_get_time(): bgp sleep(1) is %llu ticks, %lf sec\n",t2-t1, (t2-t1)*bgp_persec);
#endif
  }
  wallt = _bgp_GetTimeBase() * bgp_persec;
  wall->sec = wallt;
  wall->nsec = (wallt-wall->sec)*1e9;
}
#endif

#if ! defined(DUMPI_DISABLE_POSIX_TIMERS) && _POSIX_TIMERS > 0
static inline void get_posix_highres_timers(dumpi_clock *cpu, dumpi_clock *wall) {
  struct timespec tspec;
#ifdef _POSIX_CPUTIME
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &tspec);
  cpu->sec  = tspec.tv_sec;
  cpu->nsec = tspec.tv_nsec;
#else
  get_getrusage(cpu);
#endif /* ! _POSIX_CPUTIME */

#ifdef _POSIX_MONOTONIC_CLOCK
  clock_gettime(CLOCK_MONOTONIC, &tspec);
  wall->sec = tspec.tv_sec;
  wall->nsec = tspec.tv_nsec;
#else
  get_gettimeofday(wall);
#endif /* ! _POSIX_MONOTONIC_CLOCK */
}
#endif

static inline void get_lowres_timers(dumpi_clock *cpu, dumpi_clock *wall) {
  get_getrusage(cpu);
  get_gettimeofday(wall);
}

/*
 * Get time.  A little ugly.  Actually - make that "very ugly"
 */
void dumpi_get_time(dumpi_clock *cpu, dumpi_clock *wall)
{
  assert(cpu != NULL && wall != NULL);

#if defined DUMPI_ON_REDSTORM
  get_redstorm_timers(cpu, wall);
#elif defined DUMPI_ON_BGP /* ! DUMPI_ON_REDSTORM */
  get_bluegene_timers(cpu, wall);
#elif (! defined(DUMPI_DISABLE_POSIX_TIMERS)) && (_POSIX_TIMERS > 0)
  get_posix_highres_timers(cpu, wall);
#else
  get_lowres_timers(cpu, wall);
#endif /* DUMPI_ON_REDSTORM / DUMPI_ON_BGP / POSIX_TIMERS conditional */
}