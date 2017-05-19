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

#include <dumpi/common/perfctrs.h>
#include <dumpi/dumpiconfig.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#ifdef DUMPI_USE_PTHREADS
#include <pthread.h>
#endif /* ! DUMPI_USE_PTHREADS */

#ifdef DUMPI_HAVE_PAPI

#include <papi.h>

/*
 * Updated to hold accumulated values in thread-local storage.
 */
typedef long long dumpi_papi_accum_t;

static int active_counters_ = 0;
static int *papi_code_ = NULL;
static dumpi_perflabel_t *papi_label_ = NULL;

#ifdef DUMPI_USE_PTHREADS
static pthread_key_t *accum_key_ = NULL;
#else
static dumpi_papi_accum_t *accum_global_ = NULL;
#endif /* ! DUMPI_USE_PTHREADS */

#define DUMPI_MIN(A, B) (((A) < (B)) ? (A) : (B))

#ifdef DUMPI_USE_PTHREADS
#define DUMPI_LOCK_MUTEX   assert(pthread_mutex_lock(mutex) == 0)
#define DUMPI_UNLOCK_MUTEX assert(pthread_mutex_unlock(mutex) == 0)
#else /* ! DUMPI_USE_PTHREADS */
#define DUMPI_LOCK_MUTEX
#define DUMPI_UNLOCK_MUTEX
#endif /* ! DUMPI_USE_PTHREADS */

/* Initialize accumulators for the current thread */
static inline dumpi_papi_accum_t* get_accumulators(void) {
#ifdef DUMPI_USE_PTHREADS
  int ret;
  dumpi_papi_accum_t *accum;
  if(accum_key_ == NULL) {
    accum_key_ = (pthread_key_t*)malloc(sizeof(pthread_key_t));
    assert(accum_key_ != NULL);
    assert(pthread_key_create(accum_key_, free) == 0);
  }
  accum = (dumpi_papi_accum_t*)pthread_getspecific(*accum_key_);
  if(accum == NULL) {
    /* start the counters */
    if(active_counters_ > 0) {
      if((ret = PAPI_start_counters(papi_code_, active_counters_)) != PAPI_OK){
	fprintf(stderr, "Failed to start PAPI counters: %s\n",
		PAPI_strerror(ret));
	return NULL;
      }
      accum = (dumpi_papi_accum_t*)calloc(active_counters_,
					  sizeof(dumpi_papi_accum_t));
      assert(accum != NULL);
      assert(pthread_setspecific(*accum_key_, accum) == 0);
    }
  }
  return accum;
#else /* ! DUMPI_USE_PTHREADS */
  if(! accum_global_) {
    assert((accum_global_ =
            (dumpi_papi_accum_t*)calloc(1, sizeof(dumpi_papi_accum_t)))
           != NULL);
  }
  return accum_global_;
#endif /* ! DUMPI_USE_PTHREADS */
}

int dumpi_init_perfctrs(dumpi_perfinfo *ctrs) {
  int i, ret, count, maxcount;
  char errstr[PAPI_MAX_STR_LEN+1]; errstr[PAPI_MAX_STR_LEN] = '\0';
  /* Check input arguments checking and set up */
  assert(ctrs != NULL);
  count = ctrs->count;
  active_counters_ = 0;
  dumpi_init_perfctr_tags();
  /* Initialize PAPI.  Return 0 if PAPI fails. */
  ret = PAPI_library_init(PAPI_VER_CURRENT);
  if(ret != PAPI_VER_CURRENT) {
    PAPI_perror(ret, errstr, PAPI_MAX_STR_LEN);
    fprintf(stderr, "dumpi_init_perfctrs:  Error initializing PAPI:  %s\n",
	    errstr);
    assert(ret == PAPI_VER_CURRENT);
    return 0;
  }
  /* Allocate storage */
  maxcount = DUMPI_MIN(PAPI_num_counters(), DUMPI_MAX_PERFCTRS);
  papi_code_ = (int*)calloc(maxcount, sizeof(int));
  papi_label_ = (dumpi_perflabel_t*)calloc(maxcount, sizeof(dumpi_perflabel_t));
  assert(papi_code_ != NULL && papi_label_ != NULL);
  if(maxcount <= 0) {
    fprintf(stderr, "dumpi_init_perfctrs:  No PAPI counters available\n");
    return 0;
  }
  /* Convert the counter names to event codes */
  active_counters_ = 0;
  for(i = 0; (i < count && active_counters_ < maxcount); ++i) {
    int ret, evtret, event_code;
    ret = PAPI_event_name_to_code(ctrs->counter_tag[i], &event_code);
    if(ret == PAPI_OK) {
      /* Test whether this event is defined on the current platform */
      if((evtret = PAPI_query_event(event_code)) != PAPI_OK) {
	fprintf(stderr, "PAPI event %s not supported -- skipped (error: %s)\n",
		ctrs->counter_tag[i], PAPI_strerror(evtret));
	continue;
      }
      papi_code_[active_counters_] = event_code;
      strcpy(papi_label_[active_counters_], ctrs->counter_tag[i]);
      ++active_counters_;
    }
    else {
      fprintf(stderr, "PAPI_event_name_to_code failed for event %s -- "
	      "skipped (error: %s)\n",
	      ctrs->counter_tag[i], PAPI_strerror(ret));
    }
  }
  /* Initialize accumulators for the current thread */
  return (get_accumulators() != NULL);
}

void dumpi_get_perfctrs(const dumpi_profile *profile, dumpi_perfinfo *perf,
			dumpi_perfinfo_direction direction)
{
  int i;
  dumpi_papi_accum_t *accum;  
  int64_t *active_ctr = (direction == DUMPI_PERFCTR_IN ?
			 perf->invalue : perf->outvalue);
  (void)profile;
  if(active_counters_ > 0) {
    assert((accum = get_accumulators()) != NULL);
    perf->count = active_counters_;
    if(active_counters_) {
      PAPI_accum_counters(accum, active_counters_);
      for(i = 0; i < active_counters_; ++i) {
	/* THIS NEEDS TO BE FIXED */
	strncpy(perf->counter_tag[i], papi_label_[i], DUMPI_MAX_PERFCTR_NAME-1);
	perf->counter_tag[i][DUMPI_MAX_PERFCTR_NAME-1] = '\0';
	active_ctr[i] = accum[i];
      }
    }
  }
  else {
    perf->count = 0;
  }
}

/**
 * How many perfcounters are currently active?
 */
size_t dumpi_active_perfctrs(void) {
  return active_counters_;
}

/**
 * What are the labels of currently active perfctrs?
 * Array at least of length dumpi_active_perfctrs().
 */
dumpi_perflabel_t* dumpi_perfctr_labels(void) {
  return papi_label_;
}

#else /* ! DUMPI_HAVE_PAPI */

int dumpi_init_perfctrs(dumpi_perfinfo *ctrs) {
  assert(ctrs != NULL);
  return 1;
}

void dumpi_get_perfctrs(const dumpi_profile *profile, dumpi_perfinfo *perf,
			dumpi_perfinfo_direction direction)
{
  assert(profile && perf);
  assert(direction == DUMPI_PERFCTR_IN || direction == DUMPI_PERFCTR_OUT);
  perf->count = 0;
}

/**
 * How many perfcounters are currently active?
 */
size_t dumpi_active_perfctrs(void) {
  return 0;
}

/**
 * What are the labels of currently active perfctrs?
 * Array at least of length dumpi_active_perfctrs().
 */
dumpi_perflabel_t* dumpi_perfctr_labels(void) {
  return NULL;
}


#endif /* DUMPI_HAVE_PAPI */