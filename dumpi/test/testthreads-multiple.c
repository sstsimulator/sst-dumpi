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
