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

#ifndef DUMPI_LIBDUMPI_TOF77_H
#define DUMPI_LIBDUMPI_TOF77_H

#include <dumpi/dumpiconfig.h>
#include <mpi.h>

typedef void (dumpi_f77_mpi_op)(void*, void*, MPI_Fint*, MPI_Fint*);

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /* Callbacks used by Fortran to send info about mpi_status_ignore and mpi_statuses_ignore.
   * The real problem is mpi_statuses_ignore, which is defined in an exteremely screwy way
   * by OpenMPI.  The only semi-sane way to deal with this seems to be to assume that
   * mpi_status_ignore and mpi_statuses_ignore are constant values, and hence always
   * have the same pointer value.
   */
  void F77_FUNC_(libdumpi_f77_put_status_info, LIBDUMPI_F77_PUT_STATUS_INFO)(MPI_Fint *status_ignore, MPI_Fint *statuses_ignore);
  void F77_FUNC_(libdumpi_get_f77_constants, LIBDUMPI_GET_F77_CONSTANTS)(MPI_Fint *status_size, MPI_Fint *compare_ident, MPI_Fint *compare_similar, MPI_Fint *compare_unequal);

  void F77_FUNC_(mpi_send, MPI_SEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_recv, MPI_RECV)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_get_count, MPI_GET_COUNT)(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr);
  void F77_FUNC_(mpi_bsend, MPI_BSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_ssend, MPI_SSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_rsend, MPI_RSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_buffer_attach, MPI_BUFFER_ATTACH)(void *buf, MPI_Fint *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_buffer_detach, MPI_BUFFER_DETACH)(void *buf, MPI_Fint *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_isend, MPI_ISEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_ibsend, MPI_IBSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_issend, MPI_ISSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_irsend, MPI_IRSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_irecv, MPI_IRECV)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_wait, MPI_WAIT)(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_test, MPI_TEST)(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_request_free, MPI_REQUEST_FREE)(MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_waitany, MPI_WAITANY)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_testany, MPI_TESTANY)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *index, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_waitall, MPI_WAITALL)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *statuses, MPI_Fint *ierr);
  void F77_FUNC_(mpi_testall, MPI_TESTALL)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *flag, MPI_Fint *statuses, MPI_Fint *ierr);
  void F77_FUNC_(mpi_waitsome, MPI_WAITSOME)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *outcount, MPI_Fint *indices, MPI_Fint *statuses, MPI_Fint *ierr);
  void F77_FUNC_(mpi_testsome, MPI_TESTSOME)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *outcount, MPI_Fint *indices, MPI_Fint *statuses, MPI_Fint *ierr);
  void F77_FUNC_(mpi_iprobe, MPI_IPROBE)(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_probe, MPI_PROBE)(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cancel, MPI_CANCEL)(MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_test_cancelled, MPI_TEST_CANCELLED)(MPI_Fint *status, MPI_Fint *cancelled, MPI_Fint *ierr);
  void F77_FUNC_(mpi_send_init, MPI_SEND_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_bsend_init, MPI_BSEND_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_ssend_init, MPI_SSEND_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_rsend_init, MPI_RSEND_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_recv_init, MPI_RECV_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_start, MPI_START)(MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_startall, MPI_STARTALL)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *ierr);
  void F77_FUNC_(mpi_sendrecv, MPI_SENDRECV)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_sendrecv_replace, MPI_SENDRECV_REPLACE)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_contiguous, MPI_TYPE_CONTIGUOUS)(MPI_Fint *count, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_vector, MPI_TYPE_VECTOR)(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_hvector, MPI_TYPE_HVECTOR)(MPI_Fint *count, MPI_Fint *blocklength, MPI_Aint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_indexed, MPI_TYPE_INDEXED)(MPI_Fint *count, MPI_Fint *lengths, MPI_Fint *indices, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_hindexed, MPI_TYPE_HINDEXED)(MPI_Fint *count, MPI_Fint *lengths, MPI_Aint *indices, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_struct, MPI_TYPE_STRUCT)(MPI_Fint *count, MPI_Fint *lengths, MPI_Aint *indices, MPI_Fint *oldtypes, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_address, MPI_ADDRESS)(void *buf, MPI_Aint *address, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_extent, MPI_TYPE_EXTENT)(MPI_Fint *datatype, MPI_Aint *extent, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_size, MPI_TYPE_SIZE)(MPI_Fint *datatype, MPI_Fint *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_lb, MPI_TYPE_LB)(MPI_Fint *datatype, MPI_Aint *lb, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_ub, MPI_TYPE_UB)(MPI_Fint *datatype, MPI_Aint *ub, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_commit, MPI_TYPE_COMMIT)(MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_free, MPI_TYPE_FREE)(MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_get_elements, MPI_GET_ELEMENTS)(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *elements, MPI_Fint *ierr);
  void F77_FUNC_(mpi_pack, MPI_PACK)(void *inbuf, MPI_Fint *incount, MPI_Fint *datatype, void *outbuf, MPI_Fint *outcount, MPI_Fint *position, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_unpack, MPI_UNPACK)(void *inbuf, MPI_Fint *incount, MPI_Fint *position, void *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_pack_size, MPI_PACK_SIZE)(MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_barrier, MPI_BARRIER)(MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_bcast, MPI_BCAST)(void *sendbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_gather, MPI_GATHER)(void *inbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_gatherv, MPI_GATHERV)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_scatter, MPI_SCATTER)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_scatterv, MPI_SCATTERV)(void *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_allgather, MPI_ALLGATHER)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_allgatherv, MPI_ALLGATHERV)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_alltoall, MPI_ALLTOALL)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_alltoallv, MPI_ALLTOALLV)(void *sendbuf, MPI_Fint *sendcounts, MPI_Fint *senddispls, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *recvdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_reduce, MPI_REDUCE)(void *sendbuf, void *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_op_create, MPI_OP_CREATE)(dumpi_f77_mpi_op *fptr, MPI_Fint *commute, MPI_Fint *op, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_op_free, MPI_OP_FREE)(MPI_Fint *op, MPI_Fint *ierr);
  void F77_FUNC_(mpi_allreduce, MPI_ALLREDUCE)(void *sendbuf, void *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_reduce_scatter, MPI_REDUCE_SCATTER)(void *sendbuf, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_scan, MPI_SCAN)(void *sendbuf, void *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_size, MPI_GROUP_SIZE)(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_rank, MPI_GROUP_RANK)(MPI_Fint *group, MPI_Fint *rank, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_translate_ranks, MPI_GROUP_TRANSLATE_RANKS)(MPI_Fint *group1, MPI_Fint *count, MPI_Fint *ranks1, MPI_Fint *group2, MPI_Fint *ranks2, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_compare, MPI_GROUP_COMPARE)(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *result, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_group, MPI_COMM_GROUP)(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_union, MPI_GROUP_UNION)(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_intersection, MPI_GROUP_INTERSECTION)(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_difference, MPI_GROUP_DIFFERENCE)(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_incl, MPI_GROUP_INCL)(MPI_Fint *group, MPI_Fint *count, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_excl, MPI_GROUP_EXCL)(MPI_Fint *group, MPI_Fint *count, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_range_incl, MPI_GROUP_RANGE_INCL)(MPI_Fint *group, MPI_Fint *count, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_range_excl, MPI_GROUP_RANGE_EXCL)(MPI_Fint *group, MPI_Fint *count, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr);
  void F77_FUNC_(mpi_group_free, MPI_GROUP_FREE)(MPI_Fint *group, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_size, MPI_COMM_SIZE)(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_rank, MPI_COMM_RANK)(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_compare, MPI_COMM_COMPARE)(MPI_Fint *comm1, MPI_Fint *comm2, MPI_Fint *result, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_dup, MPI_COMM_DUP)(MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_create, MPI_COMM_CREATE)(MPI_Fint *oldcomm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_split, MPI_COMM_SPLIT)(MPI_Fint *oldcomm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_free, MPI_COMM_FREE)(MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_test_inter, MPI_COMM_TEST_INTER)(MPI_Fint *comm, MPI_Fint *inter, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_remote_size, MPI_COMM_REMOTE_SIZE)(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_remote_group, MPI_COMM_REMOTE_GROUP)(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr);
  void F77_FUNC_(mpi_intercomm_create, MPI_INTERCOMM_CREATE)(MPI_Fint *localcomm, MPI_Fint *localleader, MPI_Fint *remotecomm, MPI_Fint *remoteleader, MPI_Fint *tag, MPI_Fint *newcomm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_intercomm_merge, MPI_INTERCOMM_MERGE)(MPI_Fint *comm, MPI_Fint *high, MPI_Fint *newcomm, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_keyval_create, MPI_KEYVAL_CREATE)(MPI_Fint *copyfunc, MPI_Fint *delfunc, MPI_Fint *key, void *state, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_keyval_free, MPI_KEYVAL_FREE)(MPI_Fint *key, MPI_Fint *ierr);
  void F77_FUNC_(mpi_attr_put, MPI_ATTR_PUT)(MPI_Fint *comm, MPI_Fint *key, void *attr, MPI_Fint *ierr);
  void F77_FUNC_(mpi_attr_get, MPI_ATTR_GET)(MPI_Fint *comm, MPI_Fint *key, void *attr, MPI_Fint *flag, MPI_Fint *ierr);
  void F77_FUNC_(mpi_attr_delete, MPI_ATTR_DELETE)(MPI_Fint *comm, MPI_Fint *key, MPI_Fint *ierr);
  void F77_FUNC_(mpi_topo_test, MPI_TOPO_TEST)(MPI_Fint *comm, MPI_Fint *topo, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cart_create, MPI_CART_CREATE)(MPI_Fint *oldcomm, MPI_Fint *ndim, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *newcomm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_dims_create, MPI_DIMS_CREATE)(MPI_Fint *nodes, MPI_Fint *ndim, MPI_Fint *dims, MPI_Fint *ierr);
  void F77_FUNC_(mpi_graph_create, MPI_GRAPH_CREATE)(MPI_Fint *oldcomm, MPI_Fint *nodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *reorder, MPI_Fint *newcomm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_graphdims_get, MPI_GRAPHDIMS_GET)(MPI_Fint *comm, MPI_Fint *nodes, MPI_Fint *edges, MPI_Fint *ierr);
  void F77_FUNC_(mpi_graph_get, MPI_GRAPH_GET)(MPI_Fint *comm, MPI_Fint *maxindex, MPI_Fint *maxedges, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cartdim_get, MPI_CARTDIM_GET)(MPI_Fint *comm, MPI_Fint *ndim, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cart_get, MPI_CART_GET)(MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *coords, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cart_rank, MPI_CART_RANK)(MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cart_coords, MPI_CART_COORDS)(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, MPI_Fint *coords, MPI_Fint *ierr);
  void F77_FUNC_(mpi_graph_neighbors_count, MPI_GRAPH_NEIGHBORS_COUNT)(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *nneigh, MPI_Fint *ierr);
  void F77_FUNC_(mpi_graph_neighbors, MPI_GRAPH_NEIGHBORS)(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxneighbors, MPI_Fint *neighbors, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cart_shift, MPI_CART_SHIFT)(MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *displ, MPI_Fint *source, MPI_Fint *dest, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cart_sub, MPI_CART_SUB)(MPI_Fint *oldcomm, MPI_Fint *remain_dims, MPI_Fint *newcomm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_cart_map, MPI_CART_MAP)(MPI_Fint *comm, MPI_Fint *ndim, MPI_Fint *dims, MPI_Fint *period, MPI_Fint *newrank, MPI_Fint *ierr);
  void F77_FUNC_(mpi_graph_map, MPI_GRAPH_MAP)(MPI_Fint *comm, MPI_Fint *nodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *newrank, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_get_processor_name, MPI_GET_PROCESSOR_NAME)(char *name, MPI_Fint *resultlen, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_get_version, MPI_GET_VERSION)(MPI_Fint *version, MPI_Fint *subversion, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_errhandler_create, MPI_ERRHANDLER_CREATE)(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_errhandler_set, MPI_ERRHANDLER_SET)(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr);
  void F77_FUNC_(mpi_errhandler_get, MPI_ERRHANDLER_GET)(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr);
  void F77_FUNC_(mpi_errhandler_free, MPI_ERRHANDLER_FREE)(MPI_Fint *errhandler, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_error_string, MPI_ERROR_STRING)(MPI_Fint *errorcode, char *errorstring, MPI_Fint *resultlen, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_error_class, MPI_ERROR_CLASS)(MPI_Fint *errorcode, MPI_Fint *errorclass, MPI_Fint *ierr);
  void F77_FUNC_(mpi_init, MPI_INIT)(MPI_Fint *ierr);
  void F77_FUNC_(mpi_finalize, MPI_FINALIZE)(MPI_Fint *ierr);
  void F77_FUNC_(mpi_initialized, MPI_INITIALIZED)(MPI_Fint *result, MPI_Fint *ierr);
  void F77_FUNC_(mpi_abort, MPI_ABORT)(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_close_port, MPI_CLOSE_PORT)(char *portname, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_comm_accept, MPI_COMM_ACCEPT)(char *portname, MPI_Fint *info, MPI_Fint *root, MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_comm_connect, MPI_COMM_CONNECT)(char *portname, MPI_Fint *info, MPI_Fint *root, MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_comm_disconnect, MPI_COMM_DISCONNECT)(MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_get_parent, MPI_COMM_GET_PARENT)(MPI_Fint *parent, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_join, MPI_COMM_JOIN)(MPI_Fint *fd, MPI_Fint *comm, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_comm_spawn, MPI_COMM_SPAWN)(char *command, char *argv, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *errcodes, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_comm_spawn_multiple, MPI_COMM_SPAWN_MULTIPLE)(MPI_Fint *count, char *commands, char *argvs, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *errcodes, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_lookup_name, MPI_LOOKUP_NAME)(char *servicename, MPI_Fint *info, char *portname, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_open_port, MPI_OPEN_PORT)(MPI_Fint *info, char *portname, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_publish_name, MPI_PUBLISH_NAME)(char *servicename, MPI_Fint *info, char *portname, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_unpublish_name, MPI_UNPUBLISH_NAME)(char *servicename, MPI_Fint *info, char *portname, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_accumulate, MPI_ACCUMULATE)(void *buf, MPI_Fint *origincount, MPI_Fint *origintype, MPI_Fint *targetrank, MPI_Aint *targetdisp, MPI_Fint *targetcount, MPI_Fint *targettype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_get, MPI_GET)(void *buf, MPI_Fint *origincount, MPI_Fint *origintype, MPI_Fint *targetrank, MPI_Aint *targetdisp, MPI_Fint *targetcount, MPI_Fint *targettype, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_put, MPI_PUT)(void *buf, MPI_Fint *origincount, MPI_Fint *origintype, MPI_Fint *targetrank, MPI_Aint *targetdisp, MPI_Fint *targetcount, MPI_Fint *targettype, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_complete, MPI_WIN_COMPLETE)(MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_create, MPI_WIN_CREATE)(void *addr, MPI_Aint *size, MPI_Fint *dispunit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_fence, MPI_WIN_FENCE)(MPI_Fint *assertion, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_free, MPI_WIN_FREE)(MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_get_group, MPI_WIN_GET_GROUP)(MPI_Fint *win, MPI_Fint *group, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_lock, MPI_WIN_LOCK)(MPI_Fint *locktype, MPI_Fint *winrank, MPI_Fint *assertion, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_post, MPI_WIN_POST)(MPI_Fint *group, MPI_Fint *assertion, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_start, MPI_WIN_START)(MPI_Fint *group, MPI_Fint *assertion, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_test, MPI_WIN_TEST)(MPI_Fint *win, MPI_Fint *flag, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_unlock, MPI_WIN_UNLOCK)(MPI_Fint *winrank, MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_wait, MPI_WIN_WAIT)(MPI_Fint *win, MPI_Fint *ierr);
  void F77_FUNC_(mpi_alltoallw, MPI_ALLTOALLW)(void *sendbuf, MPI_Fint *sendcounts, MPI_Fint *senddispls, MPI_Fint *sendtypes, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *recvdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_exscan, MPI_EXSCAN)(void *sendbuf, void *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr);
  void F77_FUNC_(mpi_add_error_class, MPI_ADD_ERROR_CLASS)(MPI_Fint *errorclass, MPI_Fint *ierr);
  void F77_FUNC_(mpi_add_error_code, MPI_ADD_ERROR_CODE)(MPI_Fint *errorclass, MPI_Fint *errorcode, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_add_error_string, MPI_ADD_ERROR_STRING)(MPI_Fint *errorcode, char *errorstring, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_comm_call_errhandler, MPI_COMM_CALL_ERRHANDLER)(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_comm_create_keyval, MPI_COMM_CREATE_KEYVAL)(MPI_Fint *copyfunc, MPI_Fint *deletefunc, MPI_Fint *keyval, void *extrastate, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_comm_delete_attr, MPI_COMM_DELETE_ATTR)(MPI_Fint *comm, MPI_Fint *keyval, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_free_keyval, MPI_COMM_FREE_KEYVAL)(MPI_Fint *keyval, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_get_attr, MPI_COMM_GET_ATTR)(MPI_Fint *comm, MPI_Fint *keyval, void *value, MPI_Fint *flag, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_comm_get_name, MPI_COMM_GET_NAME)(MPI_Fint *comm, char *name, MPI_Fint *resultlen, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_comm_set_attr, MPI_COMM_SET_ATTR)(MPI_Fint *comm, MPI_Fint *keyval, void *value, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_comm_set_name, MPI_COMM_SET_NAME)(MPI_Fint *comm, char *name, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_file_call_errhandler, MPI_FILE_CALL_ERRHANDLER)(MPI_Fint *file, MPI_Fint *errorcode, MPI_Fint *ierr);
  void F77_FUNC_(mpi_grequest_complete, MPI_GREQUEST_COMPLETE)(MPI_Fint *request, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_grequest_start, MPI_GREQUEST_START)(MPI_Fint *queryfunc, MPI_Fint *freefunc, MPI_Fint *cancelfunc, void *state, MPI_Fint *request, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_init_thread, MPI_INIT_THREAD)(MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr);
  void F77_FUNC_(mpi_is_thread_main, MPI_IS_THREAD_MAIN)(MPI_Fint *flag, MPI_Fint *ierr);
  void F77_FUNC_(mpi_query_thread, MPI_QUERY_THREAD)(MPI_Fint *supported, MPI_Fint *ierr);
  void F77_FUNC_(mpi_status_set_cancelled, MPI_STATUS_SET_CANCELLED)(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr);
  void F77_FUNC_(mpi_status_set_elements, MPI_STATUS_SET_ELEMENTS)(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_type_create_keyval, MPI_TYPE_CREATE_KEYVAL)(MPI_Fint *copyfunc, MPI_Fint *deletefunc, MPI_Fint *keyval, void *state, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_type_delete_attr, MPI_TYPE_DELETE_ATTR)(MPI_Fint *datatype, MPI_Fint *keyval, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_dup, MPI_TYPE_DUP)(MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_free_keyval, MPI_TYPE_FREE_KEYVAL)(MPI_Fint *keyval, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_get_attr, MPI_TYPE_GET_ATTR)(MPI_Fint *datatype, MPI_Fint *keyval, void *value, MPI_Fint *flag, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_get_contents, MPI_TYPE_GET_CONTENTS)(MPI_Fint *datatype, MPI_Fint *maxintegers, MPI_Fint *maxaddresses, MPI_Fint *maxdatatypes, MPI_Fint *arrintegers, MPI_Aint *arraddresses, MPI_Fint *arrdatatypes, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_get_envelope, MPI_TYPE_GET_ENVELOPE)(MPI_Fint *datatype, MPI_Fint *numintegers, MPI_Fint *numaddresses, MPI_Fint *numdatatypes, MPI_Fint *combiner, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_type_get_name, MPI_TYPE_GET_NAME)(MPI_Fint *datatype, char *name, MPI_Fint *resultlen, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_type_set_attr, MPI_TYPE_SET_ATTR)(MPI_Fint *datatype, MPI_Fint *keyval, void *state, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_type_set_name, MPI_TYPE_SET_NAME)(MPI_Fint *datatype, char *name, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_type_match_size, MPI_TYPE_MATCH_SIZE)(MPI_Fint *typeclass, MPI_Fint *size, MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_call_errhandler, MPI_WIN_CALL_ERRHANDLER)(MPI_Fint *win, MPI_Fint *errorcode, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_win_create_keyval, MPI_WIN_CREATE_KEYVAL)(MPI_Fint *copyfun, MPI_Fint *deletefun, MPI_Fint *keyval, void *state, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_win_delete_attr, MPI_WIN_DELETE_ATTR)(MPI_Fint *win, MPI_Fint *keyval, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_free_keyval, MPI_WIN_FREE_KEYVAL)(MPI_Fint *keyval, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_get_attr, MPI_WIN_GET_ATTR)(MPI_Fint *win, MPI_Fint *keyval, void *state, MPI_Fint *flag, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_win_get_name, MPI_WIN_GET_NAME)(MPI_Fint *win, char *name, 
MPI_Fint *resultlen, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_win_set_attr, MPI_WIN_SET_ATTR)(MPI_Fint *win, MPI_Fint *keyval, void *state, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_win_set_name, MPI_WIN_SET_NAME)(MPI_Fint *win, char *name, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_alloc_mem, MPI_ALLOC_MEM)(MPI_Aint *size, MPI_Fint *info, void *baseptr, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_comm_create_errhandler, MPI_COMM_CREATE_ERRHANDLER)(MPI_Fint *errfun, MPI_Fint *errhandler, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_comm_get_errhandler, MPI_COMM_GET_ERRHANDLER)(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr);
  void F77_FUNC_(mpi_comm_set_errhandler, MPI_COMM_SET_ERRHANDLER)(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_file_create_errhandler, MPI_FILE_CREATE_ERRHANDLER)(MPI_Fint *errfun, MPI_Fint *errhandler, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_file_get_errhandler, MPI_FILE_GET_ERRHANDLER)(MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_set_errhandler, MPI_FILE_SET_ERRHANDLER)(MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr);
  void F77_FUNC_(mpi_finalized, MPI_FINALIZED)(MPI_Fint *flag, MPI_Fint *ierr);
  void F77_FUNC_(mpi_free_mem, MPI_FREE_MEM)(void *buf, MPI_Fint *ierr);
  void F77_FUNC_(mpi_get_address, MPI_GET_ADDRESS)(void *buf, MPI_Aint *address, MPI_Fint *ierr);
  void F77_FUNC_(mpi_info_create, MPI_INFO_CREATE)(MPI_Fint *info, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_info_delete, MPI_INFO_DELETE)(MPI_Fint *info, char *key, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_info_dup, MPI_INFO_DUP)(MPI_Fint *oldinfo, MPI_Fint *newinfo, MPI_Fint *ierr);
  void F77_FUNC_(mpi_info_free, MPI_INFO_FREE)(MPI_Fint *info, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_info_get, MPI_INFO_GET)(MPI_Fint *info, char *key, MPI_Fint *valuelength, char *value, MPI_Fint *flag, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_info_get_nkeys, MPI_INFO_GET_NKEYS)(MPI_Fint *info, MPI_Fint *nkeys, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_info_get_nthkey, MPI_INFO_GET_NTHKEY)(MPI_Fint *info, MPI_Fint *n, char *key, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_info_get_valuelen, MPI_INFO_GET_VALUELEN)(MPI_Fint *info, char *key, MPI_Fint *valuelen, MPI_Fint *flag, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_info_set, MPI_INFO_SET)(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_pack_external, MPI_PACK_EXTERNAL)(char *datarep, void *inbuf, MPI_Fint *incount, MPI_Fint *intype, void *outbuf, MPI_Aint *outcount, MPI_Aint *position, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_pack_external_size, MPI_PACK_EXTERNAL_SIZE)(char *datarep, MPI_Fint *incount, MPI_Fint *datatype, MPI_Aint *size, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_request_get_status, MPI_REQUEST_GET_STATUS)(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_create_darray, MPI_TYPE_CREATE_DARRAY)(MPI_Fint *size, MPI_Fint *rank, MPI_Fint *ndims, MPI_Fint *gsizes, MPI_Fint *distribs, MPI_Fint *dargs, MPI_Fint *psizes, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_create_hindexed, MPI_TYPE_CREATE_HINDEXED)(MPI_Fint *count, MPI_Fint *blocklengths, MPI_Aint *displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_create_hvector, MPI_TYPE_CREATE_HVECTOR)(MPI_Fint *count, MPI_Fint *blocklength, MPI_Aint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_create_indexed_block, MPI_TYPE_CREATE_INDEXED_BLOCK)(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *displacments, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_create_resized, MPI_TYPE_CREATE_RESIZED)(MPI_Fint *oldtype, MPI_Aint *lb, MPI_Aint *extent, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_create_struct, MPI_TYPE_CREATE_STRUCT)(MPI_Fint *count, MPI_Fint *blocklengths, MPI_Aint *displacements, MPI_Fint *oldtypes, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_create_subarray, MPI_TYPE_CREATE_SUBARRAY)(MPI_Fint *ndims, MPI_Fint *sizes, MPI_Fint *subsizes, MPI_Fint *starts, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_get_extent, MPI_TYPE_GET_EXTENT)(MPI_Fint *datatype, MPI_Aint *lb, MPI_Aint *extent, MPI_Fint *ierr);
  void F77_FUNC_(mpi_type_get_true_extent, MPI_TYPE_GET_TRUE_EXTENT)(MPI_Fint *datatype, MPI_Aint *lb, MPI_Aint *extent, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_unpack_external, MPI_UNPACK_EXTERNAL)(char *datarep, void *inbuf, MPI_Aint *insize, MPI_Aint *position, void *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_win_create_errhandler, MPI_WIN_CREATE_ERRHANDLER)(MPI_Fint *errfun, MPI_Fint *errhandler, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_win_get_errhandler, MPI_WIN_GET_ERRHANDLER)(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr);
  void F77_FUNC_(mpi_win_set_errhandler, MPI_WIN_SET_ERRHANDLER)(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_open, MPI_FILE_OPEN)(MPI_Fint *comm, char *filename, MPI_Fint *amode, MPI_Fint *info, MPI_Fint *file, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_close, MPI_FILE_CLOSE)(MPI_Fint *file, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_file_delete, MPI_FILE_DELETE)(char *filename, MPI_Fint *info, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_file_set_size, MPI_FILE_SET_SIZE)(MPI_Fint *file, MPI_Offset *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_preallocate, MPI_FILE_PREALLOCATE)(MPI_Fint *file, MPI_Offset *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_size, MPI_FILE_GET_SIZE)(MPI_Fint *file, MPI_Offset *size, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_group, MPI_FILE_GET_GROUP)(MPI_Fint *file, MPI_Fint *group, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_amode, MPI_FILE_GET_AMODE)(MPI_Fint *file, MPI_Fint *amode, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_set_info, MPI_FILE_SET_INFO)(MPI_Fint *file, MPI_Fint *info, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_info, MPI_FILE_GET_INFO)(MPI_Fint *file, MPI_Fint *info, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_file_set_view, MPI_FILE_SET_VIEW)(MPI_Fint *file, MPI_Fint *offset, MPI_Fint *hosttype, MPI_Fint *filetype, char *datarep, MPI_Fint *info, MPI_Fint *ierr);
  */
  /* SKIP
  void F77_FUNC_(mpi_file_get_view, MPI_FILE_GET_VIEW)(MPI_Fint *file, MPI_Fint *offset, MPI_Fint *hosttype, MPI_Fint *filetype, char *datarep, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_file_read_at, MPI_FILE_READ_AT)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_at_all, MPI_FILE_READ_AT_ALL)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_at, MPI_FILE_WRITE_AT)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_at_all, MPI_FILE_WRITE_AT_ALL)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_iread_at, MPI_FILE_IREAD_AT)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_iwrite_at, MPI_FILE_IWRITE_AT)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read, MPI_FILE_READ)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_all, MPI_FILE_READ_ALL)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write, MPI_FILE_WRITE)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_all, MPI_FILE_WRITE_ALL)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_iread, MPI_FILE_IREAD)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_iwrite, MPI_FILE_IWRITE)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_seek, MPI_FILE_SEEK)(MPI_Fint *file, MPI_Offset *offset, MPI_Fint *whence, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_position, MPI_FILE_GET_POSITION)(MPI_Fint *file, MPI_Offset *offset, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_byte_offset, MPI_FILE_GET_BYTE_OFFSET)(MPI_Fint *file, MPI_Offset *offset, MPI_Offset *bytes, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_shared, MPI_FILE_READ_SHARED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_shared, MPI_FILE_WRITE_SHARED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_iread_shared, MPI_FILE_IREAD_SHARED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_iwrite_shared, MPI_FILE_IWRITE_SHARED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_ordered, MPI_FILE_READ_ORDERED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_ordered, MPI_FILE_WRITE_ORDERED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_seek_shared, MPI_FILE_SEEK_SHARED)(MPI_Fint *file, MPI_Offset *offset, MPI_Fint *whence, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_position_shared, MPI_FILE_GET_POSITION_SHARED)(MPI_Fint *file, MPI_Offset *offset, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_at_all_begin, MPI_FILE_READ_AT_ALL_BEGIN)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_at_all_end, MPI_FILE_READ_AT_ALL_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_at_all_begin, MPI_FILE_WRITE_AT_ALL_BEGIN)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_at_all_end, MPI_FILE_WRITE_AT_ALL_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_all_begin, MPI_FILE_READ_ALL_BEGIN)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_all_end, MPI_FILE_READ_ALL_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_all_begin, MPI_FILE_WRITE_ALL_BEGIN)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_all_end, MPI_FILE_WRITE_ALL_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_ordered_begin, MPI_FILE_READ_ORDERED_BEGIN)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_read_ordered_end, MPI_FILE_READ_ORDERED_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_ordered_begin, MPI_FILE_WRITE_ORDERED_BEGIN)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_write_ordered_end, MPI_FILE_WRITE_ORDERED_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_type_extent, MPI_FILE_GET_TYPE_EXTENT)(MPI_Fint *file, MPI_Fint *datatype, MPI_Aint *extent, MPI_Fint *ierr);
  /* SKIP
  void F77_FUNC_(mpi_register_datarep, MPI_REGISTER_DATAREP)(char *name, MPI_Fint *readfun, MPI_Fint *writefun, MPI_Fint *extentfun, void *state, MPI_Fint *ierr);
  */
  void F77_FUNC_(mpi_file_set_atomicity, MPI_FILE_SET_ATOMICITY)(MPI_Fint *file, MPI_Fint *flag, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_get_atomicity, MPI_FILE_GET_ATOMICITY)(MPI_Fint *file, MPI_Fint *flag, MPI_Fint *ierr);
  void F77_FUNC_(mpi_file_sync, MPI_FILE_SYNC)(MPI_Fint *file, MPI_Fint *ierr);

#ifdef __cplusplus
} /* end of namespace C */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBDUMPI_TOF77_H */