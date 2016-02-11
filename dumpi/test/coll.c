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

#include <mpi.h>
#include <dumpi/test/coll.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* Run through all collective tests. */
void all_collectives(int rank, int size) {
  rooted_collectives(rank, size);
  rootless_collectives(rank, size);
}

/* bcast, scatter, scatterv, gather, gatherv, reduce, reduce-scatter */
void rooted_collectives(int rank, int size) {
  int i, blen, root;
  double *sendbuf, *recvbuf;
  int *cntarr, *displarr;
  assert(rank >= 0); /* just to eliminate a compiler warning */
  blen = 1024 * size;
  sendbuf = (double*)malloc(blen * sizeof(double));
  recvbuf = (double*)malloc(blen * sizeof(double));
  cntarr = (int*)malloc(size * sizeof(int));
  displarr  = (int*)malloc(size * sizeof(int));
  assert(sendbuf != NULL && recvbuf  != NULL && 
	 cntarr  != NULL && displarr != NULL);
  for(i = 0; i < blen; ++i) sendbuf[i] = (double)i;
  for(i = 0; i < size; ++i) {
    cntarr[i] = 1024;
    displarr[i] = i * 1024;
  }
  /* Bcast. */
  for(root = 0; root < size; ++root) {
    MPI_Bcast(sendbuf, blen, MPI_DOUBLE, root, MPI_COMM_WORLD);
  }
  /* Scatter */
  for(root = 0; root < size; ++root) {
    MPI_Scatter(sendbuf, 1024, MPI_DOUBLE, recvbuf, 1024, MPI_DOUBLE,
		root, MPI_COMM_WORLD);
  }
  /* Scatterv. */
  for(root = 0; root < size; ++root) {
    MPI_Scatterv(sendbuf, cntarr, displarr, MPI_DOUBLE,
		 recvbuf, 1024, MPI_DOUBLE, root, MPI_COMM_WORLD);
  }
  /* Gather. */
  for(root = 0; root < size; ++root) {
    MPI_Gather(sendbuf, 1024, MPI_DOUBLE, recvbuf, 1024, MPI_DOUBLE,
	       root, MPI_COMM_WORLD);
  }
  /* Gatherv. */
  for(root = 0; root < size; ++root) {
    MPI_Gatherv(sendbuf, 1024, MPI_DOUBLE,
		recvbuf, cntarr, displarr, MPI_DOUBLE,
		root, MPI_COMM_WORLD);
  }
  /* Reduce */
  for(root = 0; root < size; ++root) {
    MPI_Reduce(sendbuf, recvbuf, blen, MPI_DOUBLE, MPI_MAX,
	      root, MPI_COMM_WORLD);
  }
  /* Reduce-scatter. */
  for(root = 0; root < size; ++root) {
    MPI_Reduce_scatter(sendbuf, recvbuf, cntarr, MPI_DOUBLE,
		       MPI_MAX, MPI_COMM_WORLD);
  }
  /* Done. */
  free(sendbuf);
  free(recvbuf);
  free(cntarr);
  free(displarr);
}

/* barrier, allgather, allgatherv, allreduce, alltoall, alltoallv, scan */
void rootless_collectives(int rank, int size) {
  int i, blen;
  double *sendbuf, *recvbuf;
  int *cntarr=0, *displarr=0;
  blen = 1024 * size;
  sendbuf = (double*)malloc(blen * sizeof(double));
  recvbuf = (double*)malloc(blen * sizeof(double));
  cntarr = (int*)malloc(size * sizeof(int));
  displarr  = (int*)malloc(size * sizeof(int));
  assert(sendbuf != NULL && recvbuf  != NULL && 
	 cntarr  != NULL && displarr != NULL);
  for(i = 0; i < blen; ++i) sendbuf[i] = (double)i;
  for(i = 0; i < size; ++i) {
    cntarr[i] = 1024;
    displarr[i] = i * 1024;
  }
  for(i = 0; i < 2; ++i) {
    /* Barrier */
    MPI_Barrier(MPI_COMM_WORLD);
    /* Allgather */
    MPI_Allgather(sendbuf+1024*rank, 1024, MPI_DOUBLE,
		  recvbuf, 1024, MPI_DOUBLE,
		  MPI_COMM_WORLD);
    /* Allgatherv */
    MPI_Allgatherv(sendbuf+1024*rank, 1024, MPI_DOUBLE,
		   recvbuf, cntarr, displarr, MPI_DOUBLE,
		   MPI_COMM_WORLD);
    /* Allreduce */
    MPI_Allreduce(sendbuf, recvbuf, blen,
		  MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
    /* Alltoall */
    MPI_Alltoall(sendbuf, 1024, MPI_DOUBLE,
		 recvbuf, 1024, MPI_DOUBLE, MPI_COMM_WORLD);
    /* Alltoallv */
    MPI_Alltoallv(sendbuf, cntarr, displarr, MPI_DOUBLE,
		  recvbuf, cntarr, displarr, MPI_DOUBLE,
		  MPI_COMM_WORLD);
    /* Scan */
    MPI_Scan(sendbuf, recvbuf, blen, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  }
  /* Done. */
  free(sendbuf);
  free(recvbuf);
  free(cntarr);
  free(displarr);
}
