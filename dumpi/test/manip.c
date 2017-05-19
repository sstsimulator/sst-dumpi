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
#include <dumpi/test/manip.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#define DUMPI_VERBOSE 0

/* Run through all probing/waiting tests. */
void all_manips(int rank, int size) {
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "  queries\n");
  queries(rank, size);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "  probes\n");
  probes(rank, size);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "  tests\n");
  tests(rank, size);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "  types\n");
  types(rank, size);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "  ops\n");
  ops(rank, size);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "  groups\n");
  groups(rank, size);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "  comms\n");
  comms(rank, size);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "manips done\n");
}

/* Sizes, ranks, etc.
 * MPI_Wtime, MPI_Wtick, MPI_Comm_rank, MPI_Comm_size,
 */
void queries(int rank, int size) {
  int r, s;
  MPI_Wtime();
  MPI_Wtick();
  MPI_Comm_rank(MPI_COMM_WORLD, &r);
  MPI_Comm_size(MPI_COMM_WORLD, &s);
  assert(r == rank && s == size);
}

/* Just probe calls.
 * MPI_Probe, MPI_Iprobe, MPI_Get_count, 
 */
void probes(int rank, int size) {
  int i, peer, flag, count;
  MPI_Request *req;
  MPI_Status status, status2;
  req = (MPI_Request*)malloc(size * sizeof(MPI_Request));
  for(i = 0; i < size; ++i)
    req[i] = MPI_REQUEST_NULL;
  assert(req != NULL);
  for(peer = 0; peer < size; ++peer) {
    if(peer != rank) {
      MPI_Isend(&rank, 1, MPI_INT, peer, rank, MPI_COMM_WORLD, req+peer);
    }
  }
  /* probe/iprobe */
  for(peer = 0; peer < size; ++peer) {
    if(peer != rank) {
      if(peer % 2) {
	MPI_Probe(peer, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      }
      else {
	while(1) {
	  MPI_Iprobe(peer, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
	  if(flag) break;
	  usleep(50);
	}
      }
      MPI_Get_count(&status, MPI_INT, &count);
      /* Complete the call */
      MPI_Recv(&i, 1, MPI_INT, peer, status.MPI_TAG, MPI_COMM_WORLD, &status2);
    }
  }
  free(req);
}

/* Tests, waits, and cancels */
void tests(int rank, int size) {
  int peer, live_sends, reqid, index, flag, outcount;
  int matching_tag, nonmatching_send_tag, nonmatching_recv_tag;
  int *matching_value, *nonmatching_value, *indices;
  MPI_Request *matching_send, *nonmatching_send;
  MPI_Request *matching_recv, *nonmatching_recv;;
  MPI_Status *statuses;
  if(size < 2)
    return;
  live_sends = 0;
  nonmatching_send_tag = rank;
  nonmatching_recv_tag = rank+size;
  matching_value = (int*)malloc(size * sizeof(int));
  nonmatching_value = (int*)malloc(size * sizeof(int));
  indices = (int*)malloc(size * sizeof(int));
  matching_tag = size*2;
  matching_send = (MPI_Request*)malloc(size * sizeof(MPI_Request));
  nonmatching_send = (MPI_Request*)malloc(size * sizeof(MPI_Request));
  matching_recv = (MPI_Request*)malloc(size * sizeof(MPI_Request));
  nonmatching_recv = (MPI_Request*)malloc(size * sizeof(MPI_Request));
  statuses = (MPI_Status*)malloc(size * sizeof(MPI_Status));
  for(peer = 0; peer < size; ++peer) {
    matching_send[peer] = nonmatching_send[peer] = MPI_REQUEST_NULL;
    matching_recv[peer] = nonmatching_recv[peer] = MPI_REQUEST_NULL;
    if(peer != rank) {
      MPI_Isend(&rank, 1, MPI_INT, peer, matching_tag,
		MPI_COMM_WORLD,	matching_send + peer);
      MPI_Isend(&rank, 1, MPI_INT, peer, nonmatching_send_tag,
		MPI_COMM_WORLD,	nonmatching_send + peer);
      MPI_Irecv(matching_value+peer, 1, MPI_INT, peer, matching_tag,
		MPI_COMM_WORLD, matching_recv + peer);
      MPI_Irecv(nonmatching_value+peer, 1, MPI_INT, peer, nonmatching_recv_tag,
		MPI_COMM_WORLD, nonmatching_recv + peer);
      ++live_sends;
    }
  }
  /* MPI_Test */
  flag = 0;
  reqid = (rank+1)%size;
  MPI_Test(nonmatching_recv+reqid, &flag, statuses);
  assert(flag == 0);
  MPI_Test(matching_send+reqid, &flag, statuses);
  if(flag)
    --live_sends;

  /* MPI_Testany */
  MPI_Testany(size, nonmatching_recv, &index, &flag, statuses);
  assert(flag == 0);
  MPI_Testany(size, matching_send, &index, &flag, statuses);
  if(flag)
    --live_sends;
  
  /* MPI_Testsome */
  MPI_Testsome(size, nonmatching_recv, &outcount, indices, statuses);
  assert(outcount == 0);
  MPI_Testsome(size/2, matching_send, &outcount, indices, statuses);
  live_sends -= outcount;

  /* MPI_Testall */
  MPI_Testall(size, nonmatching_recv, &flag, statuses);
  assert(flag == 0);
  MPI_Testall(size, matching_send, &flag, statuses);
  if(flag)
    live_sends = 0;
  
  /* MPI_Wait */
  MPI_Wait(matching_recv+reqid, statuses);

  /* MPI_Waitany */
  MPI_Waitany(size, matching_recv, &index, statuses);

  /* MPI_Waitsome */
  MPI_Waitsome(size/2, matching_recv, &outcount, indices, statuses);
  
  /* MPI_Waitall */
  MPI_Waitall(size, matching_recv, statuses);
  
  /* MPI_Cancel */
  for(peer = 0; peer < size; ++peer) {
    if(peer != rank) {
      MPI_Cancel(nonmatching_send+peer);
      MPI_Cancel(nonmatching_recv+peer);
    }
  }

  /* MPI_Test_cancelled */
  for(peer = 0; peer < size; ++peer) {
    if(peer != rank) {
      MPI_Test(nonmatching_recv+peer, &flag, statuses);
      MPI_Test_cancelled(statuses, &flag);
      /* assert(flag == 1); */
    }
  }
  
  /* Done. */
  for(peer = 0; peer < size; ++peer) {
    MPI_Status status;
    if(matching_send[peer] != MPI_REQUEST_NULL) {
      MPI_Wait(matching_send+peer, &status);
      MPI_Test_cancelled(&status, &flag);
    }
    if(matching_recv[peer] != MPI_REQUEST_NULL) {
      MPI_Wait(matching_recv+peer, &status);
      MPI_Test_cancelled(&status, &flag);
    }
    if(nonmatching_send[peer] != MPI_REQUEST_NULL) {
      MPI_Wait(nonmatching_send+peer, &status);
      MPI_Test_cancelled(&status, &flag);
    }
    if(nonmatching_recv[peer] != MPI_REQUEST_NULL) {
      MPI_Wait(nonmatching_recv+peer, &status);
      MPI_Test_cancelled(&status, &flag);
    }
  }
  free(matching_value);
  free(nonmatching_value);
  free(indices);
  free(matching_send);
  free(nonmatching_send);
  free(matching_recv);
  free(nonmatching_recv);
  free(statuses);
}

/* Create and destroy datatypes
 *
 * MPI_Address (well, it has to go somewhere), 
 * MPI_Type_contiguous, MPI_Type_vector, MPI_Type_hvector, MPI_Type_indexed,
 * MPI_Type_hindexed, MPI_Type_create_struct, MPI_Type_create_darray,
 * MPI_Type_create_resized, MPI_Type_create_subarray, MPI_Type_size,
 * MPI_Type_lb, MPI_Type_ub, MPI_Type_commit, MPI_Type_free, MPI_Get_elements,
 * MPI_Pack, MPI_Unpack, MPI_Pack_size, 
 */
void types(int rank, int size) {
  int typesize;
  MPI_Datatype cont_type, vec_type, hvec_type, ind_type, hind_type;
  MPI_Datatype struct_type, darray_type, resized_type, subarray_type;
  /* int indexed_blocklens[] = {2, 3}, indexed_indices[] = {0, 8}; */
  assert(rank >= 0 && size > 0);
  /* Contiguous block of identical elements */
  MPI_Barrier(MPI_COMM_WORLD);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_contiguous test started\n");
  MPI_Type_contiguous(5, MPI_INT, &cont_type);
  MPI_Type_commit(&cont_type);
  MPI_Type_size(cont_type, &typesize);
  MPI_Barrier(MPI_COMM_WORLD);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_contiguous test complete\n");
  /* Non-contigous blocks-of-contigous-blocks-of identical elements
   * (Type_vector is really weird -- decent example at
   * http://mpi.deino.net/mpi_functions/MPI_Type_vector.html)  */
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_vector test started\n");
  MPI_Type_vector(2, 3, 4, cont_type, &vec_type);
  MPI_Type_commit(&vec_type);
  MPI_Type_size(vec_type, &typesize);
  /* Hvector is the same as vector, except with stride given as MPI_Aint */
  MPI_Barrier(MPI_COMM_WORLD);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_vector test complete\n");
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_hvector test started\n");
  MPI_Type_hvector(4, 3, 2, cont_type, &hvec_type);
  MPI_Type_commit(&hvec_type);
  MPI_Type_size(hvec_type, &typesize);
  MPI_Barrier(MPI_COMM_WORLD);
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_hvector test complete\n");
  /* And if you thought MPI_Type_vector was weird, you're going to love this: */
  {
    int indexed_blen[] = {2, 3}, indexed_ind[] = {0, 8};
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_indexed test started\n");
    MPI_Type_indexed(2, indexed_blen, indexed_ind, vec_type, &ind_type);
    MPI_Type_commit(&ind_type);
    MPI_Type_size(ind_type, &typesize);
    MPI_Barrier(MPI_COMM_WORLD);
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_indexed test complete\n");
    /* This is just like MPI_Type_indexed, but flavored with MPI_Aint */
    {
      MPI_Aint hindexed_ind[] = {0, 8};
      if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_hindexed test started\n");
      MPI_Type_hindexed(2, indexed_blen, hindexed_ind, hvec_type, &hind_type);
      MPI_Type_commit(&hind_type);
      MPI_Type_size(hind_type, &typesize);
      MPI_Barrier(MPI_COMM_WORLD);
      if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_hindexed test complete\n");
    }
  }
  /* MPI_Type_create_struct supercedes MPI_Type_struct */
  {
    MPI_Datatype struct_memb[3];
    MPI_Aint struct_disp[3];
    int struct_blen[] = {2, 3, 1};
    struct_disp[0] = 0;
    struct_memb[0] = MPI_INT;
    struct_disp[1] = struct_blen[0]*sizeof(int);
    struct_memb[1] = MPI_DOUBLE;
    struct_disp[2] = struct_disp[1] + struct_blen[1]*sizeof(double);
    struct_memb[2] = MPI_INT;
#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_STRUCT
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_create_struct test started\n");
    MPI_Type_create_struct(3, struct_blen, struct_disp,
			   struct_memb, &struct_type);
    MPI_Type_commit(&struct_type);
    MPI_Type_size(struct_type, &typesize);
    MPI_Barrier(MPI_COMM_WORLD);
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_create_struct test complete\n");
#else
    (void)struct_type;
#endif
  }
  /* MPI_Type_create_darray has 4 1/2 pages of documentation in the MPI-1.2
  * standard, most of them completely unintelligible gibberish */#
#if 0 /* Skipping darray stuff because it's not working on Doug's machine */
#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_DARRAY
  {
    MPI_Datatype intype = MPI_INT;
    int xs, ys, zs, remain, surf, bestsurf, typesize;
    MPI_Aint typeextent;
    int gsizes[3] = {99, 101, 103};
    int distribs[3] = {
      MPI_DISTRIBUTE_BLOCK, MPI_DISTRIBUTE_CYCLIC, MPI_DISTRIBUTE_CYCLIC
    };
    int dargs[3] = {
      MPI_DISTRIBUTE_DFLT_DARG, MPI_DISTRIBUTE_DFLT_DARG, 75
    };
    int psizes[3] = {1, 1, size};
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_create_darray test started\n");
    /* Compute a good grid (psizes) for an MPI communicator of size 'size' */
    bestsurf = size + 10;
    for(xs = 1; xs < size; ++xs) {
      if((size % xs) == 0) {
	remain = size / xs;
	for(ys = 1; ys < remain; ++ys) {
	  if((remain % ys) == 0) {
	    zs = remain / ys;
	    surf = xs + ys + zs;
	    if(surf < bestsurf) {
	      bestsurf = surf;
	      psizes[0] = xs; psizes[1] = ys; psizes[2] = zs;
	    }
	  }
	}
      }
    }
    /* Create the actual type. */
    MPI_Type_create_darray(size, rank, 3, gsizes, distribs, dargs, psizes,
			   MPI_ORDER_C, intype, &darray_type);
    MPI_Type_commit(&darray_type);
    MPI_Type_size(darray_type, &typesize);
    MPI_Type_extent(darray_type, &typeextent);
    MPI_Barrier(MPI_COMM_WORLD);
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_create_darray test complete\n");
  }
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_DARRAY */
#endif /* !0 to skip darray stuff*/
#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_RESIZED
  {
    int typesize;
    MPI_Aint typeextent;
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_create_resized test started\n");
    MPI_Type_create_resized(MPI_INT, 0, 3*sizeof(int), &resized_type);
    MPI_Type_commit(&resized_type);
    MPI_Type_size(resized_type, &typesize);
    MPI_Type_extent(resized_type, &typeextent);
    MPI_Barrier(MPI_COMM_WORLD);
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_create_resized test complete\n");
  }
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_RESIZED */
#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_SUBARRAY
  {
    int ndims = 3, sizes[] = {101, 101, 101}, subsizes[] = {2, 3, 5};
    int starts[] = {10, 20, 30}, order = MPI_ORDER_C;
    int typesize;
    MPI_Aint typeextent;
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_create_subarray test started\n");
    MPI_Type_create_subarray(ndims, sizes, subsizes, starts, order,
			     MPI_INT, &subarray_type);
    MPI_Type_commit(&subarray_type);
    MPI_Type_size(subarray_type, &typesize);
    MPI_Type_extent(subarray_type, &typeextent);
    MPI_Barrier(MPI_COMM_WORLD);
    if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    MPI_Type_create_subarray test complete\n");
  }
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_SUBARRAY */
  /* Clean up */
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    Cleaning up custom types\n");
  MPI_Type_free(&cont_type);
  MPI_Type_free(&vec_type);
  MPI_Type_free(&hvec_type);
  MPI_Type_free(&ind_type);
#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_STRUCT
  MPI_Type_free(&struct_type);
#endif
#if 0
#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_DARRAY
  MPI_Type_free(&darray_type);
#endif
#endif
#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_RESIZED
  MPI_Type_free(&resized_type);
#endif
#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_SUBARRAY
  MPI_Type_free(&subarray_type);
#endif
  if(DUMPI_VERBOSE && rank == 0) fprintf(stderr, "    Type cleanup complete\n");
}

/* A user defined type being tested for user-defined functions */
static MPI_Datatype *static_aggregate_type = NULL;

/* A dummy MPI user function -- doesn't do anything terribly interesting */
static void sum_bottom_16(void *invec, void *outvec, int *len, MPI_Datatype *dt)
{
  int i;
  int *iarr, *oarr;
  int mask = 0xffff;
  assert(invec != NULL && outvec != NULL && len != NULL && dt != NULL);
  assert(static_aggregate_type != NULL);
  assert(*dt == *static_aggregate_type);
  iarr = (int*)invec;
  oarr = (int*)outvec;
  for(i = 0; i < *len; ++i) {
    oarr[i] += (iarr[i] & mask);
  }
}
/* A second dummy MPI user function -- also not very interesting */
static void max_top_16(void *invec, void *outvec, int *len, MPI_Datatype *dt)
{
  int i, tmp;
  int *iarr, *oarr;
  int mask = 0xffff0000;
  assert(invec != NULL && outvec != NULL && len != NULL && dt != NULL);
  assert(static_aggregate_type != NULL);
  assert(*dt == *static_aggregate_type);
  iarr = (int*)invec;
  oarr = (int*)outvec;
#define MAX(A, B) ((A) > (B) ? (A) : (B))
  for(i = 0; i < *len; ++i) {
    tmp = iarr[i] >> 16;
    oarr[i] = MAX(tmp, oarr[i]&mask);
  }
}

/* Create and destroy ops.
 *
 * MPI_Op_create, MPI_Op_free
 */
void ops(int rank, int size) {
  int i;
  MPI_Op sumfunc, maxfunc;
#define DATACOUNT 10
  int indata[DATACOUNT], outdata[DATACOUNT];
  memset(outdata, 0, DATACOUNT);
  assert(static_aggregate_type == NULL);
  static_aggregate_type = (MPI_Datatype*)malloc(sizeof(MPI_Datatype));
  assert(static_aggregate_type != NULL);
  for(i = 0; i < DATACOUNT; ++i) {
    indata[i] = (rank << 16) + i;
    outdata[i] = 0;
  }
  (void)rank;
  (void)size;
#ifndef DUMPI_SKIP_MPI_TYPE_DUP
  assert(MPI_Type_dup(MPI_INT, static_aggregate_type) == MPI_SUCCESS);
  assert(MPI_Type_commit(static_aggregate_type) == MPI_SUCCESS);
#else
  *static_aggregate_type = MPI_INT;
#endif
  assert(MPI_Op_create(sum_bottom_16, 1, &sumfunc) == MPI_SUCCESS);
  assert(MPI_Op_create(max_top_16, 1, &maxfunc) == MPI_SUCCESS);
  /* Do the actual collective ops */
  assert(MPI_Reduce(indata, outdata, DATACOUNT, *static_aggregate_type,
		    sumfunc, 0, MPI_COMM_WORLD) == MPI_SUCCESS);
  if(DUMPI_VERBOSE && rank == 0) {
    for(i = 0; i < DATACOUNT; ++i) {
      assert((outdata[i] & 0xffff) == size * i);
    }
  }
  memset(outdata, 0, DATACOUNT);
  assert(MPI_Allreduce(indata, outdata, DATACOUNT, *static_aggregate_type,
		       maxfunc, MPI_COMM_WORLD) == MPI_SUCCESS);
  for(i = 0; i < DATACOUNT; ++i) {
    ; /*assert((outdata[i] >> 16) == size-1); */
  }
#ifndef DUMPI_SKIP_MPI_TYPE_DUP
  assert(MPI_Type_free(static_aggregate_type) == MPI_SUCCESS);
#endif
  assert(MPI_Op_free(&sumfunc) == MPI_SUCCESS);
  assert(MPI_Op_free(&maxfunc) == MPI_SUCCESS);
  free(static_aggregate_type);
  static_aggregate_type = NULL;
#undef DATACOUNT
}

/* Report MPI comparison results (debug) */
static const char* mpi_comp_name(int comp) {
  switch(comp) {
  case MPI_IDENT:
    return "MPI_IDENT";
  case MPI_CONGRUENT:
    return "MPI_CONGRUENT";
  case MPI_SIMILAR:
    return "MPI_SIMILAR";
  case MPI_UNEQUAL:
    return "MPI_UNEQUAL";
  default:
    return "ERROR -- not a valid comparison";
  }
}

/* Create and destroy groups. */
void groups(int rank, int size) {
#define DO_GROUP_DEBUG 0
  MPI_Comm  dupworld;
  MPI_Group worldgroup, dupworldgroup, group_a, group_b, group_c, group_d;
  MPI_Group tweakrank_a, tweakrank_b, group_diff, rangegroup_a, rangegroup_b;
  int comp, groupsize, ranksize;
  typedef int range_t[3];
  range_t *rankrange = NULL;  /* crazy range_incl and range_excl methods */
  int *ranks = (int*)calloc(size, sizeof(int));
  assert(ranks != NULL);
  /* We're ready to roll */
  assert(MPI_Comm_dup(MPI_COMM_WORLD, &dupworld) == MPI_SUCCESS);
  assert(MPI_Comm_group(MPI_COMM_WORLD, &worldgroup) == MPI_SUCCESS);
  assert(MPI_Comm_group(dupworld, &dupworldgroup) == MPI_SUCCESS);
  assert(MPI_Group_compare(worldgroup, dupworldgroup, &comp) == MPI_SUCCESS);
  if(DO_GROUP_DEBUG && rank == 0)
    fprintf(stderr, "Comparing world and dupworld yields %s\n",
	    mpi_comp_name(comp));
  assert(comp == MPI_IDENT);
  /* Create a group that contains only the even-numbered members from world */
  for(ranksize = 0; 2*ranksize < size; ++ranksize)
    ranks[ranksize] = 2*ranksize;
  if(DO_GROUP_DEBUG && rank == 0) {
    int i;
    fprintf(stderr, "ranks[%d] = [", ranksize);
    for(i = 0; i < ranksize; ++i)
      fprintf(stderr, "%d%s", ranks[i], ((i < ranksize-1) ? ", " : ""));
    fprintf(stderr, "]\n");
  }
  assert(MPI_Group_incl(dupworldgroup,ranksize,ranks,&group_a) == MPI_SUCCESS);
  if(group_a != MPI_GROUP_NULL) {
    assert(MPI_Group_size(group_a, &groupsize) == MPI_SUCCESS);
    assert(MPI_Group_compare(worldgroup, group_a, &comp) == MPI_SUCCESS);
  }
  if(DO_GROUP_DEBUG && rank == 0)
    fprintf(stderr, "Comparing world and subgroup with %d even nodes: %s\n",
	    groupsize, mpi_comp_name(comp));
  /* And a group that contains the rest of the nodes */
  assert(MPI_Group_excl(dupworldgroup,ranksize,ranks,&group_b) == MPI_SUCCESS);
  if(group_b != MPI_GROUP_NULL) {
    int vv;
    assert(MPI_Group_size(group_b, &vv) == MPI_SUCCESS);
    assert(MPI_Group_rank(group_b, &vv) == MPI_SUCCESS);
  }
  assert(MPI_Group_compare(worldgroup, group_b, &comp) == MPI_SUCCESS);
  if(DO_GROUP_DEBUG && rank == 0)
    fprintf(stderr, "Comparing world and subgroup with %d odd nodes: %s\n",
	    groupsize, mpi_comp_name(comp));
  /* The difference between world and group A must be group B */
  assert(MPI_Group_difference(worldgroup, group_a, &group_diff) == MPI_SUCCESS);
  if(group_diff != MPI_GROUP_NULL) {
    int vv;
    assert(MPI_Group_size(group_diff, &vv) == MPI_SUCCESS);
    assert(MPI_Group_rank(group_diff, &vv) == MPI_SUCCESS);
  }
  assert(MPI_Group_compare(group_b, group_diff, &comp) == MPI_SUCCESS);
  assert(comp == MPI_IDENT);
  /* The union of A and B should have the same size as the world group */
  assert(MPI_Group_union(group_a, group_b, &group_c) == MPI_SUCCESS);
  if(group_c != MPI_GROUP_NULL) {
    int vv;
    assert(MPI_Group_size(group_c, &vv) == MPI_SUCCESS);
    assert(MPI_Group_rank(group_c, &vv) == MPI_SUCCESS);
  }
  assert(MPI_Group_compare(worldgroup, group_c, &comp) == MPI_SUCCESS);
  assert(MPI_Group_size(group_c, &groupsize) == MPI_SUCCESS);
  assert(groupsize == size);
  if(DO_GROUP_DEBUG && rank == 0)
    fprintf(stderr, "Comparing world and odd/even union (%d nodes): %s\n",
	    groupsize, mpi_comp_name(comp));
  /* The intersection of groups a and b should have zero size */
  assert(MPI_Group_intersection(group_a, group_b, &group_d) == MPI_SUCCESS);
  if(group_d != MPI_GROUP_NULL) {
    int vv;
    assert(MPI_Group_size(group_d, &vv) == MPI_SUCCESS);
    assert(MPI_Group_rank(group_d, &vv) == MPI_SUCCESS);
  }
  assert(MPI_Group_size(group_d, &groupsize) == MPI_SUCCESS);
  assert(MPI_Group_compare(worldgroup, group_d, &comp) == MPI_SUCCESS);
  assert(groupsize == 0);
  if(DO_GROUP_DEBUG && rank == 0)
    fprintf(stderr, "Comparing world and empty intersect (%d nodes): %s\n",
	    groupsize, mpi_comp_name(comp));
  /* Create a group where we tweak the rank order (test comparisons) */
  for(ranksize = 0; ranksize < size; ++ranksize)
    ranks[ranksize] = (ranksize + size - 1) % size;
  assert(MPI_Group_incl(worldgroup,ranksize,ranks,&tweakrank_a) == MPI_SUCCESS);
  if(tweakrank_a != MPI_GROUP_NULL) {
    int vv;
    assert(MPI_Group_size(tweakrank_a, &vv) == MPI_SUCCESS);
    assert(MPI_Group_rank(tweakrank_a, &vv) == MPI_SUCCESS);
  }
  assert(MPI_Group_compare(worldgroup, tweakrank_a, &comp) == MPI_SUCCESS);
  if(DO_GROUP_DEBUG && rank == 0)
    fprintf(stderr, "Comparing world and tweaked rank group (%d nodes): %s\n",
	    groupsize, mpi_comp_name(comp));
  /* Tweak rank order again */
  for(ranksize = 0; ranksize < size; ++ranksize)
    ranks[ranksize] = (ranksize % 2 ? ranksize / 2 : (size + ranksize)/2);
  if(DO_GROUP_DEBUG && rank == 0) {
    int i;
    fprintf(stderr, "ranks[%d] = [", ranksize);
    for(i = 0; i < ranksize; ++i)
      fprintf(stderr, "%d%s", ranks[i], ((i < ranksize-1) ? ", " : ""));
    fprintf(stderr, "]\n");
  }
  assert(MPI_Group_incl(worldgroup,ranksize,ranks,&tweakrank_b) == MPI_SUCCESS);
  if(tweakrank_b != MPI_GROUP_NULL) {
    int vv;
    assert(MPI_Group_size(tweakrank_b, &vv) == MPI_SUCCESS);
    assert(MPI_Group_rank(tweakrank_b, &vv) == MPI_SUCCESS);
  }
  assert(MPI_Group_compare(worldgroup, tweakrank_b, &comp) == MPI_SUCCESS);
  if(DO_GROUP_DEBUG && rank == 0)
    fprintf(stderr, "Comparing world and tweaked rank group (%d nodes): %s\n",
	    groupsize, mpi_comp_name(comp));
  /* range_incl and range_excl are completely insane */
  {
    int windows = (size > 10 ? 3 : (size > 4 ? 2 : 1));
    rankrange = (range_t*)calloc(windows, sizeof(range_t));
    /* We define some funky ranges for testing */
#define SETRANGE(ARR, S, E, I) do {ARR[0] = S; ARR[1] = E; ARR[2] = I;} while(0)
    SETRANGE(rankrange[0], 0, (size > 2 ? 2 : size-1), 1);
    if(windows > 1) SETRANGE(rankrange[1], 4, (size > 8 ? 8 : size-1), 2);
    if(windows > 2) SETRANGE(rankrange[2], 10, size-1, 3);
    assert(MPI_Group_range_incl(worldgroup, windows, rankrange, &rangegroup_a)
	   == MPI_SUCCESS);
    if(rangegroup_a != MPI_GROUP_NULL) {
      int vv;
      assert(MPI_Group_size(rangegroup_a, &vv) == MPI_SUCCESS);
      assert(MPI_Group_rank(rangegroup_a, &vv) == MPI_SUCCESS);
    }
    assert(MPI_Group_range_excl(worldgroup, windows, rankrange, &rangegroup_b)
	   == MPI_SUCCESS);
    if(rangegroup_b != MPI_GROUP_NULL) {
      int vv;
      assert(MPI_Group_size(rangegroup_b, &vv) == MPI_SUCCESS);
      assert(MPI_Group_rank(rangegroup_b, &vv) == MPI_SUCCESS);
    }
  }
  /* MPI_Group_translate_ranks is a query function */
  /* fprintf(stderr, "NOT TESTED YET:  MPI_Group_translate_ranks\n"); */
  /* All done. */
  if(group_a != MPI_GROUP_NULL)
    assert(MPI_Group_free(&group_a) == MPI_SUCCESS);
  if(group_b != MPI_GROUP_NULL)
    assert(MPI_Group_free(&group_b) == MPI_SUCCESS);
  if(group_c != MPI_GROUP_NULL)
    assert(MPI_Group_free(&group_c) == MPI_SUCCESS);  
  if(group_d != MPI_GROUP_NULL)
    assert(MPI_Group_free(&group_d) == MPI_SUCCESS);
  if(tweakrank_a != MPI_GROUP_NULL)
    assert(MPI_Group_free(&tweakrank_a) == MPI_SUCCESS);
  if(tweakrank_b != MPI_GROUP_NULL)
    assert(MPI_Group_free(&tweakrank_b) == MPI_SUCCESS);
  if(rangegroup_a != MPI_GROUP_NULL)
    assert(MPI_Group_free(&rangegroup_a) == MPI_SUCCESS);
  if(rangegroup_b != MPI_GROUP_NULL)
    assert(MPI_Group_free(&rangegroup_b) == MPI_SUCCESS);
  (void)rank;
}

/* Create and destroy communicators. */
void comms(int rank, int size) {
  MPI_Comm comma = MPI_COMM_NULL, commb = MPI_COMM_NULL;
  MPI_Group worldgroup, groupa;
  int i, color, key, s, r, *incl = (int*)malloc((size/2+1)*sizeof(int));
  assert(incl != NULL);
  for(i = 0; i < size/2+1; ++i)
    incl[i] = i;
  assert(MPI_Comm_group(MPI_COMM_WORLD, &worldgroup) == MPI_SUCCESS);
  assert(MPI_Group_incl(worldgroup, size/2+1, incl, &groupa) == MPI_SUCCESS);
  assert(MPI_Comm_create(MPI_COMM_WORLD, groupa, &comma) == MPI_SUCCESS);
  /* Use MPI_Comm_split to create 2 communicators (odds and evens),
   * reverse the ranking (key),
   * and leave every third rank out of the new communicator
   */
  color = rank % 2;
  key = rank;
  if(((rank+1) % 3) == 0) {
    color = MPI_UNDEFINED;
  }
  if(comma != MPI_COMM_NULL) {
    assert(MPI_Comm_split(comma, color, key, &commb) == MPI_SUCCESS);
    if(commb != MPI_COMM_NULL) {
      assert(MPI_Comm_size(commb, &s) == MPI_SUCCESS);
      assert(MPI_Comm_rank(commb, &r) == MPI_SUCCESS);
    }
  }
  if(comma != MPI_COMM_NULL)
    assert(MPI_Comm_free(&comma) == MPI_SUCCESS);
  if(commb != MPI_COMM_NULL)
    assert(MPI_Comm_free(&commb) == MPI_SUCCESS);
}