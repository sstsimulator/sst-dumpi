/**
Copyright 2009-2022 National Technology and Engineering Solutions of Sandia,
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S. Government
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly
owned subsidiary of Honeywell International, Inc., for the U.S. Department of
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2022, NTESS

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of the copyright holder nor the names of its
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

void run_comm_test(MPI_Comm comm)
{
  if (comm == MPI_COMM_NULL) return;

  int size; MPI_Comm_size(comm, &size);
  int rank; MPI_Comm_rank(comm, &rank);
  int worldRank; MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);
  char sendBuf[1024];
  char recvBuf[1024];
  int root = 0;
  MPI_Bcast(rank == root ? sendBuf : recvBuf,
    100, MPI_INT, root, comm);

  MPI_Request* req = (MPI_Request*) malloc(2*size*sizeof(MPI_Request));

  int tag = 100;
  int i;
  for (i=1; i < size; ++i){
    int partner = (rank+i) % size;
    MPI_Isend(sendBuf, 100, MPI_INT, partner,
              tag, comm, &req[2*i]);
    MPI_Irecv(recvBuf, 100, MPI_INT, partner,
              tag, comm, &req[2*i+1]);
  }
  MPI_Waitall(2*size-2, req+2, MPI_STATUSES_IGNORE); //first 2 are null

  free(req);

  printf("Rank %d (world %d) finished test\n",
    rank, worldRank);

}

MPI_Comm run_create_test(MPI_Comm comm, int* subs, int nranks)
{
  MPI_Group worldGrp; MPI_Comm_group(comm, &worldGrp);
  MPI_Group subGrp;
  MPI_Group_incl(worldGrp, nranks, subs, &subGrp);

  MPI_Comm subComm;
  MPI_Comm_create(comm, subGrp, &subComm);

  run_comm_test(subComm);

  return subComm;
}

MPI_Comm run_split_test(MPI_Comm comm, int* colors, int* keys)
{
  if (comm == MPI_COMM_NULL) return MPI_COMM_NULL;

  int rank;
  MPI_Comm_rank(comm, &rank);

  MPI_Comm newComm;
  MPI_Comm_split(comm, colors[rank], keys[rank], &newComm);

  run_comm_test(newComm);
  return newComm;
}


#define sstmac_app_name convertotf2

int main(int argc, char** argv)
{
  MPI_Init(&argc, &argv);

  int size; MPI_Comm_size(MPI_COMM_WORLD, &size);
  int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (size != 10){
    fprintf(stderr, "Converstion test must be run with 10 MPI ranks\n");
    return 1;
  }

  int subcomm1[] = {4,0,6,7,8};
  int subcomm2[] = {1,2,5,3,9};
  int subcomm3[] = {0,1,4};
  int subcomm4[] = {3,2};


  MPI_Comm comm1 = run_create_test(MPI_COMM_WORLD, subcomm1, sizeof(subcomm1)/sizeof(int));
  MPI_Comm comm2 = run_create_test(MPI_COMM_WORLD, subcomm2, sizeof(subcomm2)/sizeof(int));
  MPI_Comm parent = comm1 == MPI_COMM_NULL ? comm2 : comm1;
  MPI_Comm comm3 = run_create_test(parent, subcomm3, sizeof(subcomm3)/sizeof(int));
  MPI_Comm comm4 = run_create_test(parent, subcomm4, sizeof(subcomm4)/sizeof(int));

#define free_comm(x) if (x != MPI_COMM_NULL) MPI_Comm_free(&x)
  free_comm(comm1);
  free_comm(comm2);
  free_comm(comm3);
  free_comm(comm4);

  int colors1[] = {0,1,4,4,4,1,0,1,1,0};
  int keys1[] =   {9,8,7,6,5,4,3,2,1,0};
  MPI_Comm split1 = run_split_test(MPI_COMM_WORLD, colors1, keys1);

  int colors2[] = {0,1,4,MPI_UNDEFINED,4,1,0,MPI_UNDEFINED,1,0};
  int keys2[] =   {9,8,7,6,5,4,3,2,1,0};
  MPI_Comm split2 = run_split_test(MPI_COMM_WORLD, colors2, keys2);


  MPI_Comm split3 = run_split_test(split1, colors1, keys1);
  free_comm(split3);
  MPI_Comm split4 = run_split_test(split1, colors2, keys2);
  free_comm(split4);
  MPI_Comm split5 = run_split_test(split2, colors1, keys1);
  free_comm(split5);
  MPI_Comm split6 = run_split_test(split2, colors2, keys1);
  free_comm(split6);

  free_comm(split1);
  free_comm(split2);
  free_comm(split3);

  MPI_Finalize();
  return 0;
}

