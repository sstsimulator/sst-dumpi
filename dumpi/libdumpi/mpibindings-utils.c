/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009-2011 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
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
