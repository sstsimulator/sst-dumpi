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

/*
 * The data that gets passed to each thread (give each thread a unique id)
 */
typedef struct sdata {
  int myid;
  int threads;
  MPI_Comm *threadcomm;
  pthread_mutex_t *lock;
  pthread_cond_t *cond;
  int *shared;
} sdata;

/* Thread routine */
static void* multi_runt(void*);

/*
 * A quick series of tests with multiple threads serialized.
 */
void test_thread_serialized() {
  static const int threads = 4;
  int i, shared = 0;
  MPI_Comm *threadcomm = NULL;
  pthread_t *threadhandle = NULL;
  pthread_mutex_t lock;
  pthread_cond_t cond;
  sdata *threaddata = NULL;
  /* create "home" communicator for each receiving thread */
  assert((threadcomm = (MPI_Comm*)calloc(threads, sizeof(MPI_Comm))) != NULL);
  for(i = 0; i < threads; ++i) {
    MPI_Comm_dup(MPI_COMM_WORLD, threadcomm + i);
  }
  /* Set up data for the threads and fire them off. */
  assert(pthread_mutex_init(&lock, NULL) == 0);
  assert(pthread_cond_init(&cond, NULL) == 0);
  assert((threadhandle=(pthread_t*)calloc(threads, sizeof(pthread_t))) != NULL);
  assert((threaddata = (sdata*)calloc(threads, sizeof(sdata))) != NULL);
  for(i = 0; i < threads; ++i) {
    threaddata[i].myid = i;
    threaddata[i].threads = threads;
    threaddata[i].threadcomm = threadcomm;
    threaddata[i].lock = &lock;
    threaddata[i].cond = &cond;
    threaddata[i].shared = &shared;
    assert(pthread_create(threadhandle+i, NULL, multi_runt, threaddata+i) == 0);
  }
  for(i = 0; i < threads; ++i)
    assert(pthread_join(threadhandle[i], NULL) == 0);
  pthread_mutex_destroy(&lock);
  pthread_cond_destroy(&cond);
  free(threadcomm);
  free(threadhandle);
  free(threaddata);
}

static void multi_p2p(sdata *data);
static void multi_coll(sdata *data);

/*
 * Run routine for threads.
 */
void* multi_runt(void *args) {
  sdata *data = (sdata*)args;
  multi_p2p(data);
  multi_coll(data);
  return NULL;
}

/*
 * Test point-to-point exchanges.
 */
void multi_p2p(sdata *data) {
  int size, rank, i, j, value = 10;
  int current_request = 0;
  int *valarr = NULL;
  MPI_Request *requests = NULL;
  assert(pthread_mutex_lock(data->lock) == 0);
  MPI_Comm_size(data->threadcomm[data->myid], &size);
  MPI_Comm_rank(data->threadcomm[data->myid], &rank);
  assert(pthread_mutex_unlock(data->lock) == 0);
  /* First, send between identical thread ranks */
  assert((valarr = (int*)calloc(2*size, sizeof(int))) != NULL);
  assert((requests = (MPI_Request*)calloc(2*size*size, sizeof(MPI_Request))) != NULL);
  assert(pthread_mutex_lock(data->lock) == 0);
  /*
  fprintf(stderr, "Rank %d thread %d:  starting\n", rank, data->myid);
  */
  if((*data->shared) >= 0)
    *data->shared = -(data->threads * 2);
  assert(pthread_mutex_unlock(data->lock) == 0);
  for(i = 0; i < size; ++i) {
    if(i != rank) {
      /*
      fprintf(stderr, "Rank %d thread %d: Send to %d\n",
              rank, data->myid, i);
      */
      assert(pthread_mutex_lock(data->lock) == 0);
      MPI_Isend(&value, 1, MPI_INT, i, data->myid, data->threadcomm[data->myid],
                requests+current_request);
      assert(pthread_mutex_unlock(data->lock) == 0);
      ++current_request;
    }
    else {
      assert(pthread_mutex_lock(data->lock) == 0);
      for(j = 0; j < size; ++j) {
        if(j != rank) {
          /*
          fprintf(stderr, "Rank %d thread %d: Recv number %d\n",
                  rank, data->myid, j);
          */
          MPI_Irecv(valarr+j, 1, MPI_INT, MPI_ANY_SOURCE, data->myid,
                    data->threadcomm[data->myid], requests+current_request);
          ++current_request;
        }
      }
      assert(pthread_mutex_unlock(data->lock) == 0);
    }
  }
  /* Can't let another thread enter waitall until all local threads are done
   * (otherwise, two ranks can deadlock the whole run) */
  /* First, mark that this thread is done with the loop */
  assert(pthread_mutex_lock(data->lock) == 0);
  /*
  fprintf(stderr, "Rank %d thread %d:  Bottom of loop; shared=%d going to %d\n",
          rank, data->myid, *data->shared, (*data->shared)+1);
  */
  (*data->shared)++;
  assert(pthread_mutex_unlock(data->lock) == 0);
  /* Then, when all threads are done with the loop, walk them through */
  assert(pthread_cond_broadcast(data->cond) == 0);
  while(1) {
    assert(pthread_mutex_lock(data->lock) == 0);
    if(*data->shared >= -data->threads) {
      /*
      fprintf(stderr, "Rank %d thread %d:  Waitall (shared=%d)\n",
              rank, data->myid, *data->shared);
      */
      MPI_Waitall(current_request, requests, MPI_STATUSES_IGNORE);
      (*data->shared)++;
      assert(pthread_cond_broadcast(data->cond) == 0);
      break;
    }
    else {
      /*
      fprintf(stderr, "Rank %d thread %d:  holding for waitall (shared=%d)\n",
              rank, data->myid, *data->shared);
      */
      assert(pthread_cond_wait(data->cond, data->lock) == 0);
    }
    assert(pthread_mutex_unlock(data->lock) == 0);
  }
  assert(pthread_mutex_unlock(data->lock) == 0);
  free(valarr);
  free(requests);
}

/*
 * Test collective exchanges.
 */
void multi_coll(sdata *data) {
  int rank, size, myid, threads, value;
  assert(pthread_mutex_lock(data->lock) == 0);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  assert(pthread_mutex_unlock(data->lock) == 0);
  myid = data->myid;
  threads = data->threads;
  /* First, sync all threads 0 on world */
  if(myid == 0) {
    assert(pthread_mutex_lock(data->lock) == 0);
    MPI_Barrier(MPI_COMM_WORLD);
    assert(pthread_mutex_unlock(data->lock) == 0);
  }
  /* Then sync all equivalent ranks separately (try to get overlapping colls) */
  /* Here we need to serialize threads in a specific order */
  assert(pthread_cond_broadcast(data->cond) == 0);
  while(1) {
    assert(pthread_mutex_lock(data->lock) == 0);
    if(*data->shared != myid) {
      /*
      fprintf(stderr, "Thread %d waiting -- shared %d\n", myid, *data->shared);
      */
      pthread_cond_wait(data->cond, data->lock);
    }
    if(*data->shared == myid) {
      /*
      fprintf(stderr, "Thread %d progressing\n", myid);
      */
      MPI_Allreduce(&value, &rank, 1, MPI_INT, MPI_SUM, data->threadcomm[myid]);
      (*data->shared)++;
      assert(pthread_cond_broadcast(data->cond) == 0);
      break;
    }
    assert(pthread_mutex_unlock(data->lock) == 0);
  }
  assert(pthread_mutex_unlock(data->lock) == 0);
}