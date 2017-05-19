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