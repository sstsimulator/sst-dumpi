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
typedef struct tdata {
  int myid;
  int threads;
  MPI_Comm *threadcomm;
} tdata;

/* Thread routine */
static void* multi_runt(void*);

/*
 * A quick series of tests with multiple threads sending and receiving.
 */
void test_thread_multiple() {
  static const int threads = 4;
  int i;
  MPI_Comm *threadcomm = NULL;
  pthread_t *threadhandle = NULL;
  tdata *threaddata = NULL;
  /* create "home" communicator for each receiving thread */
  assert((threadcomm = (MPI_Comm*)calloc(threads, sizeof(MPI_Comm))) != NULL);
  for(i = 0; i < threads; ++i) {
    MPI_Comm_dup(MPI_COMM_WORLD, threadcomm + i);
  }
  /* Set up data for the threads and fire them off. */
  assert((threadhandle=(pthread_t*)calloc(threads, sizeof(pthread_t))) != NULL);
  assert((threaddata = (tdata*)calloc(threads, sizeof(tdata))) != NULL);
  for(i = 0; i < threads; ++i) {
    threaddata[i].myid = i;
    threaddata[i].threads = threads;
    threaddata[i].threadcomm = threadcomm;
    assert(pthread_create(threadhandle+i, NULL, multi_runt, threaddata+i) == 0);
  }
  for(i = 0; i < threads; ++i)
    assert(pthread_join(threadhandle[i], NULL) == 0);
  free(threadcomm);
  free(threadhandle);
  free(threaddata);
}

static void multi_p2p(tdata *data);
static void multi_coll(tdata *data);

/*
 * Run routine for threads.
 */
void* multi_runt(void *args) {
  tdata *data = (tdata*)args;
  multi_p2p(data);
  multi_coll(data);
  return NULL;
}

/*
 * Test point-to-point exchanges.
 */
void multi_p2p(tdata *data) {
  int size, rank, i, j, value = 10;
  int *valarr = NULL;
  MPI_Request *requests = NULL;
  MPI_Comm_size(data->threadcomm[data->myid], &size);
  MPI_Comm_rank(data->threadcomm[data->myid], &rank);
  /* First, send between identical thread ranks */
  assert((valarr = (int*)calloc(2*size, sizeof(int))) != NULL);
  assert((requests = (MPI_Request*)calloc(2*size, sizeof(MPI_Request))) != NULL);
  for(i = 0; i < size; ++i) {
    int current_request = 0;
    if(i != rank) {
      /*
      fprintf(stderr, "Rank %d thread %d: Send to %d\n",
              rank, data->myid, i);
      */
      MPI_Isend(&value, 1, MPI_INT, i, data->myid, data->threadcomm[data->myid],
                requests+current_request);
      ++current_request;
    }
    else {
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
    }
    MPI_Waitall(current_request, requests, MPI_STATUSES_IGNORE);
  }
  /* flood a thread with messages from other threads on the same node */
  if(data->myid != 0) {
    /*
    fprintf(stderr, "Thread %d on node %d sending to thread 0\n",
            data->myid, rank);
    */
    MPI_Send(valarr, 2*size, MPI_INT, rank, rank, data->threadcomm[0]);
  }
  else {
    MPI_Status stat;
    for(i = 1; i < data->threads; ++i) {
      /*
      fprintf(stderr, "Thread %d on node %d receive number %d\n",
              data->myid, rank, i);
      */
      MPI_Recv(valarr, 2*size, MPI_INT, rank, rank,
               data->threadcomm[0], &stat);
    }
  }
  free(valarr);
  free(requests);
}

/*
 * Test collective exchanges.
 */
void multi_coll(tdata *data) {
  int rank, size, myid, threads, value;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  myid = data->myid;
  threads = data->threads;
  /* First, sync all threads 0 on world */
  if(myid == 0) {
    MPI_Barrier(MPI_COMM_WORLD);
  }
  /* Then sync all equivalent ranks separately (try to get overlapping colls) */
  MPI_Allreduce(&value, &rank, 1, MPI_INT, MPI_SUM, data->threadcomm[myid]);
}