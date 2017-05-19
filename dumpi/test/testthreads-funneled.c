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