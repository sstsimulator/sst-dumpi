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

