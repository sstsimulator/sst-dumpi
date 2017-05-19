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

#include <dumpi/libdumpi/mpibindings-utils.h>
#include <dumpi/dumpiconfig.h>
#include <stdlib.h>
#include <assert.h>

#ifdef DUMPI_USE_PTHREADS
#include <pthread.h>
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif /* ! DUMPI_USE_PTHREADS */

#ifdef DUMPI_USE_PTHREADS

typedef struct callarg {
  int thread_id;
  int calldepth;
} callarg;

static pthread_key_t *key = NULL;
static int next_id = 0;

static void init_stuff() {
  if(key == NULL) {
    assert((key = (pthread_key_t*)malloc(sizeof(pthread_key_t))) != NULL);
    assert(pthread_key_create(key, free) == 0);
  }
  if(pthread_getspecific(*key) == NULL) {
    callarg *carg;
    assert((carg = (callarg*)calloc(1, sizeof(callarg))) != NULL);
    carg->thread_id = next_id++;
    assert(pthread_setspecific(*key, carg) == 0);
  }
}

/* A global lock to protect access to dumpiio routines. */
int libdumpi_lock_io(void) {
  init_stuff();
#ifdef DUMPI_USE_PTHREADS
  assert(pthread_mutex_lock(&lock) == 0);
#endif
  return 1;
}

/* A global lock for io routines. */
int libdumpi_unlock_io(void) {
  init_stuff();
#ifdef DUMPI_USE_PTHREADS
  assert(pthread_mutex_unlock(&lock) == 0);
#endif
  return 1;
}

/* Get a unique thread index for this thread. */
int libdumpi_get_thread_id(void) {
  callarg *carg;
  init_stuff();
  assert((carg = (callarg*)pthread_getspecific(*key)) != NULL);
  return carg->thread_id;
}

/* Increase call depth counter for this thread. */
int libdumpi_enter_mpi(void) {
  callarg *carg;
  init_stuff();
  assert((carg = (callarg*)pthread_getspecific(*key)) != NULL);
  ++carg->calldepth;
  return carg->calldepth;
}

/* Decrease call depth counter for this thread. */
int libdumpi_exit_mpi(void) {
  callarg *carg;
  init_stuff();
  assert((carg = (callarg*)pthread_getspecific(*key)) != NULL);
  assert(carg->calldepth > 0);
  --carg->calldepth;
  return carg->calldepth;
}

/* Read current call depth. */
int libdumpi_get_call_depth(void) {
  callarg *carg;
  init_stuff();
  assert((carg = (callarg*)pthread_getspecific(*key)) != NULL);
  return carg->calldepth;
}

#else /* ! DUMPI_USE_PTHREADS */

static int calldepth = 0;

/* A global lock to protect access to dumpiio routines. */
int libdumpi_lock_io(void) {
}

/* A global lock for io routines. */
int libdumpi_unlock_io(void) {
}

/* Get a unique thread index for this thread. */
int libdumpi_get_thread_id(void) {
  return 0;
}

/* Increase call depth counter for this thread. */
int libdumpi_enter_mpi(void) {
  ++calldepth;
  return calldepth;
}

/* Decrease call depth counter for this thread. */
int libdumpi_exit_mpi(void) {
  --calldepth;
  return calldepth;
}

/* Read current call depth. */
int libdumpi_get_call_depth(void) {
  return calldepth;
}

#endif /* DUMPI_USE_PTHREADS */