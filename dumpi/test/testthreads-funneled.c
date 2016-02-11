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

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

typedef struct fdata {
  pthread_cond_t cond;
  pthread_mutex_t lock;
  int exiting;
} fdata;

void* message_thread(void*);
void* worker_thread(void*);

/*
 * A quick series of tests with funneled access to the MPI library.
 * The easiest way to test this is to have a single thread do comm
 * and let other threads do non-productive work in the interim.
 */
void test_thread_funneled() {
  static const int threads = 4;
  int i;
  pthread_t commthread;
  pthread_t *threadhandle = NULL;
  fdata workerdata;
  /* Initialize data */
  assert((threadhandle=(pthread_t*)calloc(threads, sizeof(pthread_t))) != NULL);
  assert(pthread_mutex_init(&workerdata.lock, NULL) == 0);
  assert(pthread_cond_init(&workerdata.cond, NULL) == 0);
  workerdata.exiting = 0;
  /* First start the messaging thread. */
  assert(pthread_create(&commthread, NULL, message_thread, &workerdata) == 0);
  /* Then set up and fire off the other threads. */
  for(i = 0; i < threads; ++i) {
    assert(pthread_create(threadhandle+i, NULL, worker_thread, &workerdata) == 0);
  }
  /* join threads. */
  assert(pthread_join(commthread, NULL) == 0);
  for(i = 0; i < threads; ++i)
    assert(pthread_join(threadhandle[i], NULL) == 0);
  /* clean up */
  assert(pthread_mutex_destroy(&workerdata.lock) == 0);
  assert(pthread_cond_destroy(&workerdata.cond) == 0);
  free(threadhandle);
}

void* message_thread(void *arg) {
  int i, j, size, rank;
  fdata *data = (fdata*)arg;
  /* a little bit of chatter. */
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(rank) {
    MPI_Send(&rank, 1, MPI_INT, 0, 100, MPI_COMM_WORLD);
  }
  else {
    MPI_Status stat;
    for(i = 0; i < size-1; ++i)
      MPI_Recv(&j, 1, MPI_INT, MPI_ANY_SOURCE, 100, MPI_COMM_WORLD, &stat);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  assert(pthread_mutex_lock(&data->lock) == 0);
  data->exiting = 1;
  assert(pthread_mutex_unlock(&data->lock) == 0);
  return NULL;
}

void* worker_thread(void *arg) {
  int done = 0;
  fdata *data = (fdata*)arg;
  /* just spin -- waste time and energy */
  while(! done) {
    assert(pthread_mutex_lock(&data->lock) == 0);
    done = (data->exiting == 0);
    assert(pthread_mutex_unlock(&data->lock) == 0);
  }
  return NULL;
}

