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

#ifndef DUMPI_COMMON_DUMPIIO_H
#define DUMPI_COMMON_DUMPIIO_H

#include <dumpi/common/io.h>
#include <dumpi/common/argtypes.h>
#include <dumpi/common/types.h>
#include <dumpi/dumpiconfig.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \addtogroup common_io
   */
  /*@{*/

#define DUMPI_WRITEARGS uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf,  const dumpi_outputs *output, dumpi_profile *profile
#define DUMPI_READARGS  uint16_t *thread, dumpi_time *cpu, dumpi_time *wall, dumpi_perfinfo *perf, dumpi_profile *profile

  /** Write a func_enter record (including function id) at current position */
  int DUMPI_SKIP_INSTRUMENTING
  dumpi_write_func_enter(const dumpi_func_call *val, DUMPI_WRITEARGS);
  /** Read a func_enter record (excluding function id) at current position */
  int dumpi_read_func_enter(dumpi_func_call *val, DUMPI_READARGS);

  /** Write a func_exit record at the current stream position */
  int DUMPI_SKIP_INSTRUMENTING
  dumpi_write_func_exit(const dumpi_func_call *val, DUMPI_WRITEARGS);
  /** Read a func_exit record (excluding function id) at current position */
  int dumpi_read_func_exit(dumpi_func_call *val, DUMPI_READARGS);

  /** Write a send record (including function id) at current position */
  int dumpi_write_send(const dumpi_send *val, DUMPI_WRITEARGS);
  /** Read a send record (excluding function id) at current position */
  int dumpi_read_send(dumpi_send *val, DUMPI_READARGS);

  /** Write a recv record (including function id) at current position */
  int dumpi_write_recv(const dumpi_recv *val, DUMPI_WRITEARGS);
  /** Read a recv record (excluding function id) at current position */
  int dumpi_read_recv(dumpi_recv *val, DUMPI_READARGS);

  /** Write a get_count record (including function id) at current position */
  int dumpi_write_get_count(const dumpi_get_count *val, DUMPI_WRITEARGS);
  /** Read a get_count record (excluding function id) at current position */
  int dumpi_read_get_count(dumpi_get_count *val, DUMPI_READARGS);

  /** Write a bsend record (including function id) at current position */
  int dumpi_write_bsend(const dumpi_bsend *val, DUMPI_WRITEARGS);
  /** Read a bsend record (excluding function id) at current position */
  int dumpi_read_bsend(dumpi_bsend *val, DUMPI_READARGS);

  /** Write a ssend record (including function id) at current position */
  int dumpi_write_ssend(const dumpi_ssend *val, DUMPI_WRITEARGS);
  /** Read a ssend record (excluding function id) at current position */
  int dumpi_read_ssend(dumpi_ssend *val, DUMPI_READARGS);

  /** Write a rsend record (including function id) at current position */
  int dumpi_write_rsend(const dumpi_rsend *val, DUMPI_WRITEARGS);
  /** Read a rsend record (excluding function id) at current position */
  int dumpi_read_rsend(dumpi_rsend *val, DUMPI_READARGS);

  /** Write a buffer_attach record (including function id) at current position */
  int dumpi_write_buffer_attach(const dumpi_buffer_attach *val, DUMPI_WRITEARGS);
  /** Read a buffer_attach record (excluding function id) at current position */
  int dumpi_read_buffer_attach(dumpi_buffer_attach *val, DUMPI_READARGS);

  /** Write a buffer_detach record (including function id) at current position */
  int dumpi_write_buffer_detach(const dumpi_buffer_detach *val, DUMPI_WRITEARGS);
  /** Read a buffer_detach record (excluding function id) at current position */
  int dumpi_read_buffer_detach(dumpi_buffer_detach *val, DUMPI_READARGS);

  /** Write a isend record (including function id) at current position */
  int dumpi_write_isend(const dumpi_isend *val, DUMPI_WRITEARGS);
  /** Read a isend record (excluding function id) at current position */
  int dumpi_read_isend(dumpi_isend *val, DUMPI_READARGS);

  /** Write a ibsend record (including function id) at current position */
  int dumpi_write_ibsend(const dumpi_ibsend *val, DUMPI_WRITEARGS);
  /** Read a ibsend record (excluding function id) at current position */
  int dumpi_read_ibsend(dumpi_ibsend *val, DUMPI_READARGS);

  /** Write a issend record (including function id) at current position */
  int dumpi_write_issend(const dumpi_issend *val, DUMPI_WRITEARGS);
  /** Read a issend record (excluding function id) at current position */
  int dumpi_read_issend(dumpi_issend *val, DUMPI_READARGS);

  /** Write a irsend record (including function id) at current position */
  int dumpi_write_irsend(const dumpi_irsend *val, DUMPI_WRITEARGS);
  /** Read a irsend record (excluding function id) at current position */
  int dumpi_read_irsend(dumpi_irsend *val, DUMPI_READARGS);

  /** Write a irecv record (including function id) at current position */
  int dumpi_write_irecv(const dumpi_irecv *val, DUMPI_WRITEARGS);
  /** Read a irecv record (excluding function id) at current position */
  int dumpi_read_irecv(dumpi_irecv *val, DUMPI_READARGS);

  /** Write a wait record (including function id) at current position */
  int dumpi_write_wait(const dumpi_wait *val, DUMPI_WRITEARGS);
  /** Read a wait record (excluding function id) at current position */
  int dumpi_read_wait(dumpi_wait *val, DUMPI_READARGS);

  /** Write a test record (including function id) at current position */
  int dumpi_write_test(const dumpi_test *val, DUMPI_WRITEARGS);
  /** Read a test record (excluding function id) at current position */
  int dumpi_read_test(dumpi_test *val, DUMPI_READARGS);

  /** Write a request_free record (including function id) at current position */
  int dumpi_write_request_free(const dumpi_request_free *val, DUMPI_WRITEARGS);
  /** Read a request_free record (excluding function id) at current position */
  int dumpi_read_request_free(dumpi_request_free *val, DUMPI_READARGS);

  /** Write a waitany record (including function id) at current position */
  int dumpi_write_waitany(const dumpi_waitany *val, DUMPI_WRITEARGS);
  /** Read a waitany record (excluding function id) at current position */
  int dumpi_read_waitany(dumpi_waitany *val, DUMPI_READARGS);

  /** Write a testany record (including function id) at current position */
  int dumpi_write_testany(const dumpi_testany *val, DUMPI_WRITEARGS);
  /** Read a testany record (excluding function id) at current position */
  int dumpi_read_testany(dumpi_testany *val, DUMPI_READARGS);

  /** Write a waitall record (including function id) at current position */
  int dumpi_write_waitall(const dumpi_waitall *val, DUMPI_WRITEARGS);
  /** Read a waitall record (excluding function id) at current position */
  int dumpi_read_waitall(dumpi_waitall *val, DUMPI_READARGS);

  /** Write a testall record (including function id) at current position */
  int dumpi_write_testall(const dumpi_testall *val, DUMPI_WRITEARGS);
  /** Read a testall record (excluding function id) at current position */
  int dumpi_read_testall(dumpi_testall *val, DUMPI_READARGS);

  /** Write a waitsome record (including function id) at current position */
  int dumpi_write_waitsome(const dumpi_waitsome *val, DUMPI_WRITEARGS);
  /** Read a waitsome record (excluding function id) at current position */
  int dumpi_read_waitsome(dumpi_waitsome *val, DUMPI_READARGS);

  /** Write a testsome record (including function id) at current position */
  int dumpi_write_testsome(const dumpi_testsome *val, DUMPI_WRITEARGS);
  /** Read a testsome record (excluding function id) at current position */
  int dumpi_read_testsome(dumpi_testsome *val, DUMPI_READARGS);

  /** Write a iprobe record (including function id) at current position */
  int dumpi_write_iprobe(const dumpi_iprobe *val, DUMPI_WRITEARGS);
  /** Read a iprobe record (excluding function id) at current position */
  int dumpi_read_iprobe(dumpi_iprobe *val, DUMPI_READARGS);

  /** Write a probe record (including function id) at current position */
  int dumpi_write_probe(const dumpi_probe *val, DUMPI_WRITEARGS);
  /** Read a probe record (excluding function id) at current position */
  int dumpi_read_probe(dumpi_probe *val, DUMPI_READARGS);

  /** Write a cancel record (including function id) at current position */
  int dumpi_write_cancel(const dumpi_cancel *val, DUMPI_WRITEARGS);
  /** Read a cancel record (excluding function id) at current position */
  int dumpi_read_cancel(dumpi_cancel *val, DUMPI_READARGS);

  /** Write a test_cancelled record (including function id) at current position */
  int dumpi_write_test_cancelled(const dumpi_test_cancelled *val, DUMPI_WRITEARGS);
  /** Read a test_cancelled record (excluding function id) at current position */
  int dumpi_read_test_cancelled(dumpi_test_cancelled *val, DUMPI_READARGS);

  /** Write a send_init record (including function id) at current position */
  int dumpi_write_send_init(const dumpi_send_init *val, DUMPI_WRITEARGS);
  /** Read a send_init record (excluding function id) at current position */
  int dumpi_read_send_init(dumpi_send_init *val, DUMPI_READARGS);

  /** Write a bsend_init record (including function id) at current position */
  int dumpi_write_bsend_init(const dumpi_bsend_init *val, DUMPI_WRITEARGS);
  /** Read a bsend_init record (excluding function id) at current position */
  int dumpi_read_bsend_init(dumpi_bsend_init *val, DUMPI_READARGS);

  /** Write a ssend_init record (including function id) at current position */
  int dumpi_write_ssend_init(const dumpi_ssend_init *val, DUMPI_WRITEARGS);
  /** Read a ssend_init record (excluding function id) at current position */
  int dumpi_read_ssend_init(dumpi_ssend_init *val, DUMPI_READARGS);

  /** Write a rsend_init record (including function id) at current position */
  int dumpi_write_rsend_init(const dumpi_rsend_init *val, DUMPI_WRITEARGS);
  /** Read a rsend_init record (excluding function id) at current position */
  int dumpi_read_rsend_init(dumpi_rsend_init *val, DUMPI_READARGS);

  /** Write a recv_init record (including function id) at current position */
  int dumpi_write_recv_init(const dumpi_recv_init *val, DUMPI_WRITEARGS);
  /** Read a recv_init record (excluding function id) at current position */
  int dumpi_read_recv_init(dumpi_recv_init *val, DUMPI_READARGS);

  /** Write a start record (including function id) at current position */
  int dumpi_write_start(const dumpi_start *val, DUMPI_WRITEARGS);
  /** Read a start record (excluding function id) at current position */
  int dumpi_read_start(dumpi_start *val, DUMPI_READARGS);

  /** Write a startall record (including function id) at current position */
  int dumpi_write_startall(const dumpi_startall *val, DUMPI_WRITEARGS);
  /** Read a startall record (excluding function id) at current position */
  int dumpi_read_startall(dumpi_startall *val, DUMPI_READARGS);

  /** Write a sendrecv record (including function id) at current position */
  int dumpi_write_sendrecv(const dumpi_sendrecv *val, DUMPI_WRITEARGS);
  /** Read a sendrecv record (excluding function id) at current position */
  int dumpi_read_sendrecv(dumpi_sendrecv *val, DUMPI_READARGS);

  /** Write a sendrecv_replace record (including function id) at current position */
  int dumpi_write_sendrecv_replace(const dumpi_sendrecv_replace *val, DUMPI_WRITEARGS);
  /** Read a sendrecv_replace record (excluding function id) at current position */
  int dumpi_read_sendrecv_replace(dumpi_sendrecv_replace *val, DUMPI_READARGS);

  /** Write a type_contiguous record (including function id) at current position */
  int dumpi_write_type_contiguous(const dumpi_type_contiguous *val, DUMPI_WRITEARGS);
  /** Read a type_contiguous record (excluding function id) at current position */
  int dumpi_read_type_contiguous(dumpi_type_contiguous *val, DUMPI_READARGS);

  /** Write a type_vector record (including function id) at current position */
  int dumpi_write_type_vector(const dumpi_type_vector *val, DUMPI_WRITEARGS);
  /** Read a type_vector record (excluding function id) at current position */
  int dumpi_read_type_vector(dumpi_type_vector *val, DUMPI_READARGS);

  /** Write a type_hvector record (including function id) at current position */
  int dumpi_write_type_hvector(const dumpi_type_hvector *val, DUMPI_WRITEARGS);
  /** Read a type_hvector record (excluding function id) at current position */
  int dumpi_read_type_hvector(dumpi_type_hvector *val, DUMPI_READARGS);

  /** Write a type_indexed record (including function id) at current position */
  int dumpi_write_type_indexed(const dumpi_type_indexed *val, DUMPI_WRITEARGS);
  /** Read a type_indexed record (excluding function id) at current position */
  int dumpi_read_type_indexed(dumpi_type_indexed *val, DUMPI_READARGS);

  /** Write a type_hindexed record (including function id) at current position */
  int dumpi_write_type_hindexed(const dumpi_type_hindexed *val, DUMPI_WRITEARGS);
  /** Read a type_hindexed record (excluding function id) at current position */
  int dumpi_read_type_hindexed(dumpi_type_hindexed *val, DUMPI_READARGS);

  /** Write a type_struct record (including function id) at current position */
  int dumpi_write_type_struct(const dumpi_type_struct *val, DUMPI_WRITEARGS);
  /** Read a type_struct record (excluding function id) at current position */
  int dumpi_read_type_struct(dumpi_type_struct *val, DUMPI_READARGS);

  /** Write a address record (including function id) at current position */
  int dumpi_write_address(const dumpi_address *val, DUMPI_WRITEARGS);
  /** Read a address record (excluding function id) at current position */
  int dumpi_read_address(dumpi_address *val, DUMPI_READARGS);

  /** Write a type_extent record (including function id) at current position */
  int dumpi_write_type_extent(const dumpi_type_extent *val, DUMPI_WRITEARGS);
  /** Read a type_extent record (excluding function id) at current position */
  int dumpi_read_type_extent(dumpi_type_extent *val, DUMPI_READARGS);

  /** Write a type_size record (including function id) at current position */
  int dumpi_write_type_size(const dumpi_type_size *val, DUMPI_WRITEARGS);
  /** Read a type_size record (excluding function id) at current position */
  int dumpi_read_type_size(dumpi_type_size *val, DUMPI_READARGS);

  /** Write a type_lb record (including function id) at current position */
  int dumpi_write_type_lb(const dumpi_type_lb *val, DUMPI_WRITEARGS);
  /** Read a type_lb record (excluding function id) at current position */
  int dumpi_read_type_lb(dumpi_type_lb *val, DUMPI_READARGS);

  /** Write a type_ub record (including function id) at current position */
  int dumpi_write_type_ub(const dumpi_type_ub *val, DUMPI_WRITEARGS);
  /** Read a type_ub record (excluding function id) at current position */
  int dumpi_read_type_ub(dumpi_type_ub *val, DUMPI_READARGS);

  /** Write a type_commit record (including function id) at current position */
  int dumpi_write_type_commit(const dumpi_type_commit *val, DUMPI_WRITEARGS);
  /** Read a type_commit record (excluding function id) at current position */
  int dumpi_read_type_commit(dumpi_type_commit *val, DUMPI_READARGS);

  /** Write a type_free record (including function id) at current position */
  int dumpi_write_type_free(const dumpi_type_free *val, DUMPI_WRITEARGS);
  /** Read a type_free record (excluding function id) at current position */
  int dumpi_read_type_free(dumpi_type_free *val, DUMPI_READARGS);

  /** Write a get_elements record (including function id) at current position */
  int dumpi_write_get_elements(const dumpi_get_elements *val, DUMPI_WRITEARGS);
  /** Read a get_elements record (excluding function id) at current position */
  int dumpi_read_get_elements(dumpi_get_elements *val, DUMPI_READARGS);

  /** Write a pack record (including function id) at current position */
  int dumpi_write_pack(const dumpi_pack *val, DUMPI_WRITEARGS);
  /** Read a pack record (excluding function id) at current position */
  int dumpi_read_pack(dumpi_pack *val, DUMPI_READARGS);

  /** Write a unpack record (including function id) at current position */
  int dumpi_write_unpack(const dumpi_unpack *val, DUMPI_WRITEARGS);
  /** Read a unpack record (excluding function id) at current position */
  int dumpi_read_unpack(dumpi_unpack *val, DUMPI_READARGS);

  /** Write a pack_size record (including function id) at current position */
  int dumpi_write_pack_size(const dumpi_pack_size *val, DUMPI_WRITEARGS);
  /** Read a pack_size record (excluding function id) at current position */
  int dumpi_read_pack_size(dumpi_pack_size *val, DUMPI_READARGS);

  /** Write a barrier record (including function id) at current position */
  int dumpi_write_barrier(const dumpi_barrier *val, DUMPI_WRITEARGS);
  /** Read a barrier record (excluding function id) at current position */
  int dumpi_read_barrier(dumpi_barrier *val, DUMPI_READARGS);

  /** Write a bcast record (including function id) at current position */
  int dumpi_write_bcast(const dumpi_bcast *val, DUMPI_WRITEARGS);
  /** Read a bcast record (excluding function id) at current position */
  int dumpi_read_bcast(dumpi_bcast *val, DUMPI_READARGS);

  /** Write a gather record (including function id) at current position */
  int dumpi_write_gather(const dumpi_gather *val, DUMPI_WRITEARGS);
  /** Read a gather record (excluding function id) at current position */
  int dumpi_read_gather(dumpi_gather *val, DUMPI_READARGS);

  /** Write a gatherv record (including function id) at current position */
  int dumpi_write_gatherv(const dumpi_gatherv *val, DUMPI_WRITEARGS);
  /** Read a gatherv record (excluding function id) at current position */
  int dumpi_read_gatherv(dumpi_gatherv *val, DUMPI_READARGS);

  /** Write a scatter record (including function id) at current position */
  int dumpi_write_scatter(const dumpi_scatter *val, DUMPI_WRITEARGS);
  /** Read a scatter record (excluding function id) at current position */
  int dumpi_read_scatter(dumpi_scatter *val, DUMPI_READARGS);

  /** Write a scatterv record (including function id) at current position */
  int dumpi_write_scatterv(const dumpi_scatterv *val, DUMPI_WRITEARGS);
  /** Read a scatterv record (excluding function id) at current position */
  int dumpi_read_scatterv(dumpi_scatterv *val, DUMPI_READARGS);

  /** Write a allgather record (including function id) at current position */
  int dumpi_write_allgather(const dumpi_allgather *val, DUMPI_WRITEARGS);
  /** Read a allgather record (excluding function id) at current position */
  int dumpi_read_allgather(dumpi_allgather *val, DUMPI_READARGS);

  /** Write a allgatherv record (including function id) at current position */
  int dumpi_write_allgatherv(const dumpi_allgatherv *val, DUMPI_WRITEARGS);
  /** Read a allgatherv record (excluding function id) at current position */
  int dumpi_read_allgatherv(dumpi_allgatherv *val, DUMPI_READARGS);

  /** Write a alltoall record (including function id) at current position */
  int dumpi_write_alltoall(const dumpi_alltoall *val, DUMPI_WRITEARGS);
  /** Read a alltoall record (excluding function id) at current position */
  int dumpi_read_alltoall(dumpi_alltoall *val, DUMPI_READARGS);

  /** Write a alltoallv record (including function id) at current position */
  int dumpi_write_alltoallv(const dumpi_alltoallv *val, DUMPI_WRITEARGS);
  /** Read a alltoallv record (excluding function id) at current position */
  int dumpi_read_alltoallv(dumpi_alltoallv *val, DUMPI_READARGS);

  /** Write a reduce record (including function id) at current position */
  int dumpi_write_reduce(const dumpi_reduce *val, DUMPI_WRITEARGS);
  /** Read a reduce record (excluding function id) at current position */
  int dumpi_read_reduce(dumpi_reduce *val, DUMPI_READARGS);

  /** Write a op_create record (including function id) at current position */
  int dumpi_write_op_create(const dumpi_op_create *val, DUMPI_WRITEARGS);
  /** Read a op_create record (excluding function id) at current position */
  int dumpi_read_op_create(dumpi_op_create *val, DUMPI_READARGS);

  /** Write a op_free record (including function id) at current position */
  int dumpi_write_op_free(const dumpi_op_free *val, DUMPI_WRITEARGS);
  /** Read a op_free record (excluding function id) at current position */
  int dumpi_read_op_free(dumpi_op_free *val, DUMPI_READARGS);

  /** Write a allreduce record (including function id) at current position */
  int dumpi_write_allreduce(const dumpi_allreduce *val, DUMPI_WRITEARGS);
  /** Read a allreduce record (excluding function id) at current position */
  int dumpi_read_allreduce(dumpi_allreduce *val, DUMPI_READARGS);

  /** Write a reduce_scatter record (including function id) at current position */
  int dumpi_write_reduce_scatter(const dumpi_reduce_scatter *val, DUMPI_WRITEARGS);
  /** Read a reduce_scatter record (excluding function id) at current position */
  int dumpi_read_reduce_scatter(dumpi_reduce_scatter *val, DUMPI_READARGS);

  /** Write a scan record (including function id) at current position */
  int dumpi_write_scan(const dumpi_scan *val, DUMPI_WRITEARGS);
  /** Read a scan record (excluding function id) at current position */
  int dumpi_read_scan(dumpi_scan *val, DUMPI_READARGS);

  /** Write a group_size record (including function id) at current position */
  int dumpi_write_group_size(const dumpi_group_size *val, DUMPI_WRITEARGS);
  /** Read a group_size record (excluding function id) at current position */
  int dumpi_read_group_size(dumpi_group_size *val, DUMPI_READARGS);

  /** Write a group_rank record (including function id) at current position */
  int dumpi_write_group_rank(const dumpi_group_rank *val, DUMPI_WRITEARGS);
  /** Read a group_rank record (excluding function id) at current position */
  int dumpi_read_group_rank(dumpi_group_rank *val, DUMPI_READARGS);

  /** Write a group_translate_ranks record (including function id) at current position */
  int dumpi_write_group_translate_ranks(const dumpi_group_translate_ranks *val, DUMPI_WRITEARGS);
  /** Read a group_translate_ranks record (excluding function id) at current position */
  int dumpi_read_group_translate_ranks(dumpi_group_translate_ranks *val, DUMPI_READARGS);

  /** Write a group_compare record (including function id) at current position */
  int dumpi_write_group_compare(const dumpi_group_compare *val, DUMPI_WRITEARGS);
  /** Read a group_compare record (excluding function id) at current position */
  int dumpi_read_group_compare(dumpi_group_compare *val, DUMPI_READARGS);

  /** Write a comm_group record (including function id) at current position */
  int dumpi_write_comm_group(const dumpi_comm_group *val, DUMPI_WRITEARGS);
  /** Read a comm_group record (excluding function id) at current position */
  int dumpi_read_comm_group(dumpi_comm_group *val, DUMPI_READARGS);

  /** Write a group_union record (including function id) at current position */
  int dumpi_write_group_union(const dumpi_group_union *val, DUMPI_WRITEARGS);
  /** Read a group_union record (excluding function id) at current position */
  int dumpi_read_group_union(dumpi_group_union *val, DUMPI_READARGS);

  /** Write a group_intersection record (including function id) at current position */
  int dumpi_write_group_intersection(const dumpi_group_intersection *val, DUMPI_WRITEARGS);
  /** Read a group_intersection record (excluding function id) at current position */
  int dumpi_read_group_intersection(dumpi_group_intersection *val, DUMPI_READARGS);

  /** Write a group_difference record (including function id) at current position */
  int dumpi_write_group_difference(const dumpi_group_difference *val, DUMPI_WRITEARGS);
  /** Read a group_difference record (excluding function id) at current position */
  int dumpi_read_group_difference(dumpi_group_difference *val, DUMPI_READARGS);

  /** Write a group_incl record (including function id) at current position */
  int dumpi_write_group_incl(const dumpi_group_incl *val, DUMPI_WRITEARGS);
  /** Read a group_incl record (excluding function id) at current position */
  int dumpi_read_group_incl(dumpi_group_incl *val, DUMPI_READARGS);

  /** Write a group_excl record (including function id) at current position */
  int dumpi_write_group_excl(const dumpi_group_excl *val, DUMPI_WRITEARGS);
  /** Read a group_excl record (excluding function id) at current position */
  int dumpi_read_group_excl(dumpi_group_excl *val, DUMPI_READARGS);

  /** Write a group_range_incl record (including function id) at current position */
  int dumpi_write_group_range_incl(const dumpi_group_range_incl *val, DUMPI_WRITEARGS);
  /** Read a group_range_incl record (excluding function id) at current position */
  int dumpi_read_group_range_incl(dumpi_group_range_incl *val, DUMPI_READARGS);

  /** Write a group_range_excl record (including function id) at current position */
  int dumpi_write_group_range_excl(const dumpi_group_range_excl *val, DUMPI_WRITEARGS);
  /** Read a group_range_excl record (excluding function id) at current position */
  int dumpi_read_group_range_excl(dumpi_group_range_excl *val, DUMPI_READARGS);

  /** Write a group_free record (including function id) at current position */
  int dumpi_write_group_free(const dumpi_group_free *val, DUMPI_WRITEARGS);
  /** Read a group_free record (excluding function id) at current position */
  int dumpi_read_group_free(dumpi_group_free *val, DUMPI_READARGS);

  /** Write a comm_size record (including function id) at current position */
  int dumpi_write_comm_size(const dumpi_comm_size *val, DUMPI_WRITEARGS);
  /** Read a comm_size record (excluding function id) at current position */
  int dumpi_read_comm_size(dumpi_comm_size *val, DUMPI_READARGS);

  /** Write a comm_rank record (including function id) at current position */
  int dumpi_write_comm_rank(const dumpi_comm_rank *val, DUMPI_WRITEARGS);
  /** Read a comm_rank record (excluding function id) at current position */
  int dumpi_read_comm_rank(dumpi_comm_rank *val, DUMPI_READARGS);

  /** Write a comm_compare record (including function id) at current position */
  int dumpi_write_comm_compare(const dumpi_comm_compare *val, DUMPI_WRITEARGS);
  /** Read a comm_compare record (excluding function id) at current position */
  int dumpi_read_comm_compare(dumpi_comm_compare *val, DUMPI_READARGS);

  /** Write a comm_dup record (including function id) at current position */
  int dumpi_write_comm_dup(const dumpi_comm_dup *val, DUMPI_WRITEARGS);
  /** Read a comm_dup record (excluding function id) at current position */
  int dumpi_read_comm_dup(dumpi_comm_dup *val, DUMPI_READARGS);

  /** Write a comm_create record (including function id) at current position */
  int dumpi_write_comm_create(const dumpi_comm_create *val, DUMPI_WRITEARGS);
  /** Read a comm_create record (excluding function id) at current position */
  int dumpi_read_comm_create(dumpi_comm_create *val, DUMPI_READARGS);

  /** Write a comm_split record (including function id) at current position */
  int dumpi_write_comm_split(const dumpi_comm_split *val, DUMPI_WRITEARGS);
  /** Read a comm_split record (excluding function id) at current position */
  int dumpi_read_comm_split(dumpi_comm_split *val, DUMPI_READARGS);

  /** Write a comm_free record (including function id) at current position */
  int dumpi_write_comm_free(const dumpi_comm_free *val, DUMPI_WRITEARGS);
  /** Read a comm_free record (excluding function id) at current position */
  int dumpi_read_comm_free(dumpi_comm_free *val, DUMPI_READARGS);

  /** Write a comm_test_inter record (including function id) at current position */
  int dumpi_write_comm_test_inter(const dumpi_comm_test_inter *val, DUMPI_WRITEARGS);
  /** Read a comm_test_inter record (excluding function id) at current position */
  int dumpi_read_comm_test_inter(dumpi_comm_test_inter *val, DUMPI_READARGS);

  /** Write a comm_remote_size record (including function id) at current position */
  int dumpi_write_comm_remote_size(const dumpi_comm_remote_size *val, DUMPI_WRITEARGS);
  /** Read a comm_remote_size record (excluding function id) at current position */
  int dumpi_read_comm_remote_size(dumpi_comm_remote_size *val, DUMPI_READARGS);

  /** Write a comm_remote_group record (including function id) at current position */
  int dumpi_write_comm_remote_group(const dumpi_comm_remote_group *val, DUMPI_WRITEARGS);
  /** Read a comm_remote_group record (excluding function id) at current position */
  int dumpi_read_comm_remote_group(dumpi_comm_remote_group *val, DUMPI_READARGS);

  /** Write a intercomm_create record (including function id) at current position */
  int dumpi_write_intercomm_create(const dumpi_intercomm_create *val, DUMPI_WRITEARGS);
  /** Read a intercomm_create record (excluding function id) at current position */
  int dumpi_read_intercomm_create(dumpi_intercomm_create *val, DUMPI_READARGS);

  /** Write a intercomm_merge record (including function id) at current position */
  int dumpi_write_intercomm_merge(const dumpi_intercomm_merge *val, DUMPI_WRITEARGS);
  /** Read a intercomm_merge record (excluding function id) at current position */
  int dumpi_read_intercomm_merge(dumpi_intercomm_merge *val, DUMPI_READARGS);

  /** Write a keyval_create record (including function id) at current position */
  int dumpi_write_keyval_create(const dumpi_keyval_create *val, DUMPI_WRITEARGS);
  /** Read a keyval_create record (excluding function id) at current position */
  int dumpi_read_keyval_create(dumpi_keyval_create *val, DUMPI_READARGS);

  /** Write a keyval_free record (including function id) at current position */
  int dumpi_write_keyval_free(const dumpi_keyval_free *val, DUMPI_WRITEARGS);
  /** Read a keyval_free record (excluding function id) at current position */
  int dumpi_read_keyval_free(dumpi_keyval_free *val, DUMPI_READARGS);

  /** Write a attr_put record (including function id) at current position */
  int dumpi_write_attr_put(const dumpi_attr_put *val, DUMPI_WRITEARGS);
  /** Read a attr_put record (excluding function id) at current position */
  int dumpi_read_attr_put(dumpi_attr_put *val, DUMPI_READARGS);

  /** Write a attr_get record (including function id) at current position */
  int dumpi_write_attr_get(const dumpi_attr_get *val, DUMPI_WRITEARGS);
  /** Read a attr_get record (excluding function id) at current position */
  int dumpi_read_attr_get(dumpi_attr_get *val, DUMPI_READARGS);

  /** Write a attr_delete record (including function id) at current position */
  int dumpi_write_attr_delete(const dumpi_attr_delete *val, DUMPI_WRITEARGS);
  /** Read a attr_delete record (excluding function id) at current position */
  int dumpi_read_attr_delete(dumpi_attr_delete *val, DUMPI_READARGS);

  /** Write a topo_test record (including function id) at current position */
  int dumpi_write_topo_test(const dumpi_topo_test *val, DUMPI_WRITEARGS);
  /** Read a topo_test record (excluding function id) at current position */
  int dumpi_read_topo_test(dumpi_topo_test *val, DUMPI_READARGS);

  /** Write a cart_create record (including function id) at current position */
  int dumpi_write_cart_create(const dumpi_cart_create *val, DUMPI_WRITEARGS);
  /** Read a cart_create record (excluding function id) at current position */
  int dumpi_read_cart_create(dumpi_cart_create *val, DUMPI_READARGS);

  /** Write a dims_create record (including function id) at current position */
  int dumpi_write_dims_create(const dumpi_dims_create *val, DUMPI_WRITEARGS);
  /** Read a dims_create record (excluding function id) at current position */
  int dumpi_read_dims_create(dumpi_dims_create *val, DUMPI_READARGS);

  /** Write a graph_create record (including function id) at current position */
  int dumpi_write_graph_create(const dumpi_graph_create *val, DUMPI_WRITEARGS);
  /** Read a graph_create record (excluding function id) at current position */
  int dumpi_read_graph_create(dumpi_graph_create *val, DUMPI_READARGS);

  /** Write a graphdims_get record (including function id) at current position */
  int dumpi_write_graphdims_get(const dumpi_graphdims_get *val, DUMPI_WRITEARGS);
  /** Read a graphdims_get record (excluding function id) at current position */
  int dumpi_read_graphdims_get(dumpi_graphdims_get *val, DUMPI_READARGS);

  /** Write a graph_get record (including function id) at current position */
  int dumpi_write_graph_get(const dumpi_graph_get *val, DUMPI_WRITEARGS);
  /** Read a graph_get record (excluding function id) at current position */
  int dumpi_read_graph_get(dumpi_graph_get *val, DUMPI_READARGS);

  /** Write a cartdim_get record (including function id) at current position */
  int dumpi_write_cartdim_get(const dumpi_cartdim_get *val, DUMPI_WRITEARGS);
  /** Read a cartdim_get record (excluding function id) at current position */
  int dumpi_read_cartdim_get(dumpi_cartdim_get *val, DUMPI_READARGS);

  /** Write a cart_get record (including function id) at current position */
  int dumpi_write_cart_get(const dumpi_cart_get *val, DUMPI_WRITEARGS);
  /** Read a cart_get record (excluding function id) at current position */
  int dumpi_read_cart_get(dumpi_cart_get *val, DUMPI_READARGS);

  /** Write a cart_rank record (including function id) at current position */
  int dumpi_write_cart_rank(const dumpi_cart_rank *val, DUMPI_WRITEARGS);
  /** Read a cart_rank record (excluding function id) at current position */
  int dumpi_read_cart_rank(dumpi_cart_rank *val, DUMPI_READARGS);

  /** Write a cart_coords record (including function id) at current position */
  int dumpi_write_cart_coords(const dumpi_cart_coords *val, DUMPI_WRITEARGS);
  /** Read a cart_coords record (excluding function id) at current position */
  int dumpi_read_cart_coords(dumpi_cart_coords *val, DUMPI_READARGS);

  /** Write a graph_neighbors_count record (including function id) at current position */
  int dumpi_write_graph_neighbors_count(const dumpi_graph_neighbors_count *val, DUMPI_WRITEARGS);
  /** Read a graph_neighbors_count record (excluding function id) at current position */
  int dumpi_read_graph_neighbors_count(dumpi_graph_neighbors_count *val, DUMPI_READARGS);

  /** Write a graph_neighbors record (including function id) at current position */
  int dumpi_write_graph_neighbors(const dumpi_graph_neighbors *val, DUMPI_WRITEARGS);
  /** Read a graph_neighbors record (excluding function id) at current position */
  int dumpi_read_graph_neighbors(dumpi_graph_neighbors *val, DUMPI_READARGS);

  /** Write a cart_shift record (including function id) at current position */
  int dumpi_write_cart_shift(const dumpi_cart_shift *val, DUMPI_WRITEARGS);
  /** Read a cart_shift record (excluding function id) at current position */
  int dumpi_read_cart_shift(dumpi_cart_shift *val, DUMPI_READARGS);

  /** Write a cart_sub record (including function id) at current position */
  int dumpi_write_cart_sub(const dumpi_cart_sub *val, DUMPI_WRITEARGS);
  /** Read a cart_sub record (excluding function id) at current position */
  int dumpi_read_cart_sub(dumpi_cart_sub *val, DUMPI_READARGS);

  /** Write a cart_map record (including function id) at current position */
  int dumpi_write_cart_map(const dumpi_cart_map *val, DUMPI_WRITEARGS);
  /** Read a cart_map record (excluding function id) at current position */
  int dumpi_read_cart_map(dumpi_cart_map *val, DUMPI_READARGS);

  /** Write a graph_map record (including function id) at current position */
  int dumpi_write_graph_map(const dumpi_graph_map *val, DUMPI_WRITEARGS);
  /** Read a graph_map record (excluding function id) at current position */
  int dumpi_read_graph_map(dumpi_graph_map *val, DUMPI_READARGS);

  /** Write a get_processor_name record (including function id) at current position */
  int dumpi_write_get_processor_name(const dumpi_get_processor_name *val, DUMPI_WRITEARGS);
  /** Read a get_processor_name record (excluding function id) at current position */
  int dumpi_read_get_processor_name(dumpi_get_processor_name *val, DUMPI_READARGS);

  /** Write a get_version record (including function id) at current position */
  int dumpi_write_get_version(const dumpi_get_version *val, DUMPI_WRITEARGS);
  /** Read a get_version record (excluding function id) at current position */
  int dumpi_read_get_version(dumpi_get_version *val, DUMPI_READARGS);

  /** Write a errhandler_create record (including function id) at current position */
  int dumpi_write_errhandler_create(const dumpi_errhandler_create *val, DUMPI_WRITEARGS);
  /** Read a errhandler_create record (excluding function id) at current position */
  int dumpi_read_errhandler_create(dumpi_errhandler_create *val, DUMPI_READARGS);

  /** Write a errhandler_set record (including function id) at current position */
  int dumpi_write_errhandler_set(const dumpi_errhandler_set *val, DUMPI_WRITEARGS);
  /** Read a errhandler_set record (excluding function id) at current position */
  int dumpi_read_errhandler_set(dumpi_errhandler_set *val, DUMPI_READARGS);

  /** Write a errhandler_get record (including function id) at current position */
  int dumpi_write_errhandler_get(const dumpi_errhandler_get *val, DUMPI_WRITEARGS);
  /** Read a errhandler_get record (excluding function id) at current position */
  int dumpi_read_errhandler_get(dumpi_errhandler_get *val, DUMPI_READARGS);

  /** Write a errhandler_free record (including function id) at current position */
  int dumpi_write_errhandler_free(const dumpi_errhandler_free *val, DUMPI_WRITEARGS);
  /** Read a errhandler_free record (excluding function id) at current position */
  int dumpi_read_errhandler_free(dumpi_errhandler_free *val, DUMPI_READARGS);

  /** Write a error_string record (including function id) at current position */
  int dumpi_write_error_string(const dumpi_error_string *val, DUMPI_WRITEARGS);
  /** Read a error_string record (excluding function id) at current position */
  int dumpi_read_error_string(dumpi_error_string *val, DUMPI_READARGS);

  /** Write a error_class record (including function id) at current position */
  int dumpi_write_error_class(const dumpi_error_class *val, DUMPI_WRITEARGS);
  /** Read a error_class record (excluding function id) at current position */
  int dumpi_read_error_class(dumpi_error_class *val, DUMPI_READARGS);

  /** Write a wtime record (including function id) at current position */
  int dumpi_write_wtime(const dumpi_wtime *val, DUMPI_WRITEARGS);
  /** Read a wtime record (excluding function id) at current position */
  int dumpi_read_wtime(dumpi_wtime *val, DUMPI_READARGS);

  /** Write a wtick record (including function id) at current position */
  int dumpi_write_wtick(const dumpi_wtick *val, DUMPI_WRITEARGS);
  /** Read a wtick record (excluding function id) at current position */
  int dumpi_read_wtick(dumpi_wtick *val, DUMPI_READARGS);

  /** Write a init record (including function id) at current position */
  int dumpi_write_init(const dumpi_init *val, DUMPI_WRITEARGS);
  /** Read a init record (excluding function id) at current position */
  int dumpi_read_init(dumpi_init *val, DUMPI_READARGS);

  /** Write a finalize record (including function id) at current position */
  int dumpi_write_finalize(const dumpi_finalize *val, DUMPI_WRITEARGS);
  /** Read a finalize record (excluding function id) at current position */
  int dumpi_read_finalize(dumpi_finalize *val, DUMPI_READARGS);

  /** Write a initialized record (including function id) at current position */
  int dumpi_write_initialized(const dumpi_initialized *val, DUMPI_WRITEARGS);
  /** Read a initialized record (excluding function id) at current position */
  int dumpi_read_initialized(dumpi_initialized *val, DUMPI_READARGS);

  /** Write a abort record (including function id) at current position */
  int dumpi_write_abort(const dumpi_abort *val, DUMPI_WRITEARGS);
  /** Read a abort record (excluding function id) at current position */
  int dumpi_read_abort(dumpi_abort *val, DUMPI_READARGS);

  /** Write a close_port record (including function id) at current position */
  int dumpi_write_close_port(const dumpi_close_port *val, DUMPI_WRITEARGS);
  /** Read a close_port record (excluding function id) at current position */
  int dumpi_read_close_port(dumpi_close_port *val, DUMPI_READARGS);

  /** Write a comm_accept record (including function id) at current position */
  int dumpi_write_comm_accept(const dumpi_comm_accept *val, DUMPI_WRITEARGS);
  /** Read a comm_accept record (excluding function id) at current position */
  int dumpi_read_comm_accept(dumpi_comm_accept *val, DUMPI_READARGS);

  /** Write a comm_connect record (including function id) at current position */
  int dumpi_write_comm_connect(const dumpi_comm_connect *val, DUMPI_WRITEARGS);
  /** Read a comm_connect record (excluding function id) at current position */
  int dumpi_read_comm_connect(dumpi_comm_connect *val, DUMPI_READARGS);

  /** Write a comm_disconnect record (including function id) at current position */
  int dumpi_write_comm_disconnect(const dumpi_comm_disconnect *val, DUMPI_WRITEARGS);
  /** Read a comm_disconnect record (excluding function id) at current position */
  int dumpi_read_comm_disconnect(dumpi_comm_disconnect *val, DUMPI_READARGS);

  /** Write a comm_get_parent record (including function id) at current position */
  int dumpi_write_comm_get_parent(const dumpi_comm_get_parent *val, DUMPI_WRITEARGS);
  /** Read a comm_get_parent record (excluding function id) at current position */
  int dumpi_read_comm_get_parent(dumpi_comm_get_parent *val, DUMPI_READARGS);

  /** Write a comm_join record (including function id) at current position */
  int dumpi_write_comm_join(const dumpi_comm_join *val, DUMPI_WRITEARGS);
  /** Read a comm_join record (excluding function id) at current position */
  int dumpi_read_comm_join(dumpi_comm_join *val, DUMPI_READARGS);

  /** Write a comm_spawn record (including function id) at current position */
  int dumpi_write_comm_spawn(const dumpi_comm_spawn *val, DUMPI_WRITEARGS);
  /** Read a comm_spawn record (excluding function id) at current position */
  int dumpi_read_comm_spawn(dumpi_comm_spawn *val, DUMPI_READARGS);

  /** Write a comm_spawn_multiple record (including function id) at current position */
  int dumpi_write_comm_spawn_multiple(const dumpi_comm_spawn_multiple *val, DUMPI_WRITEARGS);
  /** Read a comm_spawn_multiple record (excluding function id) at current position */
  int dumpi_read_comm_spawn_multiple(dumpi_comm_spawn_multiple *val, DUMPI_READARGS);

  /** Write a lookup_name record (including function id) at current position */
  int dumpi_write_lookup_name(const dumpi_lookup_name *val, DUMPI_WRITEARGS);
  /** Read a lookup_name record (excluding function id) at current position */
  int dumpi_read_lookup_name(dumpi_lookup_name *val, DUMPI_READARGS);

  /** Write a open_port record (including function id) at current position */
  int dumpi_write_open_port(const dumpi_open_port *val, DUMPI_WRITEARGS);
  /** Read a open_port record (excluding function id) at current position */
  int dumpi_read_open_port(dumpi_open_port *val, DUMPI_READARGS);

  /** Write a publish_name record (including function id) at current position */
  int dumpi_write_publish_name(const dumpi_publish_name *val, DUMPI_WRITEARGS);
  /** Read a publish_name record (excluding function id) at current position */
  int dumpi_read_publish_name(dumpi_publish_name *val, DUMPI_READARGS);

  /** Write a unpublish_name record (including function id) at current position */
  int dumpi_write_unpublish_name(const dumpi_unpublish_name *val, DUMPI_WRITEARGS);
  /** Read a unpublish_name record (excluding function id) at current position */
  int dumpi_read_unpublish_name(dumpi_unpublish_name *val, DUMPI_READARGS);

  /** Write a accumulate record (including function id) at current position */
  int dumpi_write_accumulate(const dumpi_accumulate *val, DUMPI_WRITEARGS);
  /** Read a accumulate record (excluding function id) at current position */
  int dumpi_read_accumulate(dumpi_accumulate *val, DUMPI_READARGS);

  /** Write a get record (including function id) at current position */
  int dumpi_write_get(const dumpi_get *val, DUMPI_WRITEARGS);
  /** Read a get record (excluding function id) at current position */
  int dumpi_read_get(dumpi_get *val, DUMPI_READARGS);

  /** Write a put record (including function id) at current position */
  int dumpi_write_put(const dumpi_put *val, DUMPI_WRITEARGS);
  /** Read a put record (excluding function id) at current position */
  int dumpi_read_put(dumpi_put *val, DUMPI_READARGS);

  /** Write a win_complete record (including function id) at current position */
  int dumpi_write_win_complete(const dumpi_win_complete *val, DUMPI_WRITEARGS);
  /** Read a win_complete record (excluding function id) at current position */
  int dumpi_read_win_complete(dumpi_win_complete *val, DUMPI_READARGS);

  /** Write a win_create record (including function id) at current position */
  int dumpi_write_win_create(const dumpi_win_create *val, DUMPI_WRITEARGS);
  /** Read a win_create record (excluding function id) at current position */
  int dumpi_read_win_create(dumpi_win_create *val, DUMPI_READARGS);

  /** Write a win_fence record (including function id) at current position */
  int dumpi_write_win_fence(const dumpi_win_fence *val, DUMPI_WRITEARGS);
  /** Read a win_fence record (excluding function id) at current position */
  int dumpi_read_win_fence(dumpi_win_fence *val, DUMPI_READARGS);

  /** Write a win_free record (including function id) at current position */
  int dumpi_write_win_free(const dumpi_win_free *val, DUMPI_WRITEARGS);
  /** Read a win_free record (excluding function id) at current position */
  int dumpi_read_win_free(dumpi_win_free *val, DUMPI_READARGS);

  /** Write a win_get_group record (including function id) at current position */
  int dumpi_write_win_get_group(const dumpi_win_get_group *val, DUMPI_WRITEARGS);
  /** Read a win_get_group record (excluding function id) at current position */
  int dumpi_read_win_get_group(dumpi_win_get_group *val, DUMPI_READARGS);

  /** Write a win_lock record (including function id) at current position */
  int dumpi_write_win_lock(const dumpi_win_lock *val, DUMPI_WRITEARGS);
  /** Read a win_lock record (excluding function id) at current position */
  int dumpi_read_win_lock(dumpi_win_lock *val, DUMPI_READARGS);

  /** Write a win_post record (including function id) at current position */
  int dumpi_write_win_post(const dumpi_win_post *val, DUMPI_WRITEARGS);
  /** Read a win_post record (excluding function id) at current position */
  int dumpi_read_win_post(dumpi_win_post *val, DUMPI_READARGS);

  /** Write a win_start record (including function id) at current position */
  int dumpi_write_win_start(const dumpi_win_start *val, DUMPI_WRITEARGS);
  /** Read a win_start record (excluding function id) at current position */
  int dumpi_read_win_start(dumpi_win_start *val, DUMPI_READARGS);

  /** Write a win_test record (including function id) at current position */
  int dumpi_write_win_test(const dumpi_win_test *val, DUMPI_WRITEARGS);
  /** Read a win_test record (excluding function id) at current position */
  int dumpi_read_win_test(dumpi_win_test *val, DUMPI_READARGS);

  /** Write a win_unlock record (including function id) at current position */
  int dumpi_write_win_unlock(const dumpi_win_unlock *val, DUMPI_WRITEARGS);
  /** Read a win_unlock record (excluding function id) at current position */
  int dumpi_read_win_unlock(dumpi_win_unlock *val, DUMPI_READARGS);

  /** Write a win_wait record (including function id) at current position */
  int dumpi_write_win_wait(const dumpi_win_wait *val, DUMPI_WRITEARGS);
  /** Read a win_wait record (excluding function id) at current position */
  int dumpi_read_win_wait(dumpi_win_wait *val, DUMPI_READARGS);

  /** Write a alltoallw record (including function id) at current position */
  int dumpi_write_alltoallw(const dumpi_alltoallw *val, DUMPI_WRITEARGS);
  /** Read a alltoallw record (excluding function id) at current position */
  int dumpi_read_alltoallw(dumpi_alltoallw *val, DUMPI_READARGS);

  /** Write a exscan record (including function id) at current position */
  int dumpi_write_exscan(const dumpi_exscan *val, DUMPI_WRITEARGS);
  /** Read a exscan record (excluding function id) at current position */
  int dumpi_read_exscan(dumpi_exscan *val, DUMPI_READARGS);

  /** Write a add_error_class record (including function id) at current position */
  int dumpi_write_add_error_class(const dumpi_add_error_class *val, DUMPI_WRITEARGS);
  /** Read a add_error_class record (excluding function id) at current position */
  int dumpi_read_add_error_class(dumpi_add_error_class *val, DUMPI_READARGS);

  /** Write a add_error_code record (including function id) at current position */
  int dumpi_write_add_error_code(const dumpi_add_error_code *val, DUMPI_WRITEARGS);
  /** Read a add_error_code record (excluding function id) at current position */
  int dumpi_read_add_error_code(dumpi_add_error_code *val, DUMPI_READARGS);

  /** Write a add_error_string record (including function id) at current position */
  int dumpi_write_add_error_string(const dumpi_add_error_string *val, DUMPI_WRITEARGS);
  /** Read a add_error_string record (excluding function id) at current position */
  int dumpi_read_add_error_string(dumpi_add_error_string *val, DUMPI_READARGS);

  /** Write a comm_call_errhandler record (including function id) at current position */
  int dumpi_write_comm_call_errhandler(const dumpi_comm_call_errhandler *val, DUMPI_WRITEARGS);
  /** Read a comm_call_errhandler record (excluding function id) at current position */
  int dumpi_read_comm_call_errhandler(dumpi_comm_call_errhandler *val, DUMPI_READARGS);

  /** Write a comm_create_keyval record (including function id) at current position */
  int dumpi_write_comm_create_keyval(const dumpi_comm_create_keyval *val, DUMPI_WRITEARGS);
  /** Read a comm_create_keyval record (excluding function id) at current position */
  int dumpi_read_comm_create_keyval(dumpi_comm_create_keyval *val, DUMPI_READARGS);

  /** Write a comm_delete_attr record (including function id) at current position */
  int dumpi_write_comm_delete_attr(const dumpi_comm_delete_attr *val, DUMPI_WRITEARGS);
  /** Read a comm_delete_attr record (excluding function id) at current position */
  int dumpi_read_comm_delete_attr(dumpi_comm_delete_attr *val, DUMPI_READARGS);

  /** Write a comm_free_keyval record (including function id) at current position */
  int dumpi_write_comm_free_keyval(const dumpi_comm_free_keyval *val, DUMPI_WRITEARGS);
  /** Read a comm_free_keyval record (excluding function id) at current position */
  int dumpi_read_comm_free_keyval(dumpi_comm_free_keyval *val, DUMPI_READARGS);

  /** Write a comm_get_attr record (including function id) at current position */
  int dumpi_write_comm_get_attr(const dumpi_comm_get_attr *val, DUMPI_WRITEARGS);
  /** Read a comm_get_attr record (excluding function id) at current position */
  int dumpi_read_comm_get_attr(dumpi_comm_get_attr *val, DUMPI_READARGS);

  /** Write a comm_get_name record (including function id) at current position */
  int dumpi_write_comm_get_name(const dumpi_comm_get_name *val, DUMPI_WRITEARGS);
  /** Read a comm_get_name record (excluding function id) at current position */
  int dumpi_read_comm_get_name(dumpi_comm_get_name *val, DUMPI_READARGS);

  /** Write a comm_set_attr record (including function id) at current position */
  int dumpi_write_comm_set_attr(const dumpi_comm_set_attr *val, DUMPI_WRITEARGS);
  /** Read a comm_set_attr record (excluding function id) at current position */
  int dumpi_read_comm_set_attr(dumpi_comm_set_attr *val, DUMPI_READARGS);

  /** Write a comm_set_name record (including function id) at current position */
  int dumpi_write_comm_set_name(const dumpi_comm_set_name *val, DUMPI_WRITEARGS);
  /** Read a comm_set_name record (excluding function id) at current position */
  int dumpi_read_comm_set_name(dumpi_comm_set_name *val, DUMPI_READARGS);

  /** Write a file_call_errhandler record (including function id) at current position */
  int dumpi_write_file_call_errhandler(const dumpi_file_call_errhandler *val, DUMPI_WRITEARGS);
  /** Read a file_call_errhandler record (excluding function id) at current position */
  int dumpi_read_file_call_errhandler(dumpi_file_call_errhandler *val, DUMPI_READARGS);

  /** Write a grequest_complete record (including function id) at current position */
  int dumpi_write_grequest_complete(const dumpi_grequest_complete *val, DUMPI_WRITEARGS);
  /** Read a grequest_complete record (excluding function id) at current position */
  int dumpi_read_grequest_complete(dumpi_grequest_complete *val, DUMPI_READARGS);

  /** Write a grequest_start record (including function id) at current position */
  int dumpi_write_grequest_start(const dumpi_grequest_start *val, DUMPI_WRITEARGS);
  /** Read a grequest_start record (excluding function id) at current position */
  int dumpi_read_grequest_start(dumpi_grequest_start *val, DUMPI_READARGS);

  /** Write a init_thread record (including function id) at current position */
  int dumpi_write_init_thread(const dumpi_init_thread *val, DUMPI_WRITEARGS);
  /** Read a init_thread record (excluding function id) at current position */
  int dumpi_read_init_thread(dumpi_init_thread *val, DUMPI_READARGS);

  /** Write a is_thread_main record (including function id) at current position */
  int dumpi_write_is_thread_main(const dumpi_is_thread_main *val, DUMPI_WRITEARGS);
  /** Read a is_thread_main record (excluding function id) at current position */
  int dumpi_read_is_thread_main(dumpi_is_thread_main *val, DUMPI_READARGS);

  /** Write a query_thread record (including function id) at current position */
  int dumpi_write_query_thread(const dumpi_query_thread *val, DUMPI_WRITEARGS);
  /** Read a query_thread record (excluding function id) at current position */
  int dumpi_read_query_thread(dumpi_query_thread *val, DUMPI_READARGS);

  /** Write a status_set_cancelled record (including function id) at current position */
  int dumpi_write_status_set_cancelled(const dumpi_status_set_cancelled *val, DUMPI_WRITEARGS);
  /** Read a status_set_cancelled record (excluding function id) at current position */
  int dumpi_read_status_set_cancelled(dumpi_status_set_cancelled *val, DUMPI_READARGS);

  /** Write a status_set_elements record (including function id) at current position */
  int dumpi_write_status_set_elements(const dumpi_status_set_elements *val, DUMPI_WRITEARGS);
  /** Read a status_set_elements record (excluding function id) at current position */
  int dumpi_read_status_set_elements(dumpi_status_set_elements *val, DUMPI_READARGS);

  /** Write a type_create_keyval record (including function id) at current position */
  int dumpi_write_type_create_keyval(const dumpi_type_create_keyval *val, DUMPI_WRITEARGS);
  /** Read a type_create_keyval record (excluding function id) at current position */
  int dumpi_read_type_create_keyval(dumpi_type_create_keyval *val, DUMPI_READARGS);

  /** Write a type_delete_attr record (including function id) at current position */
  int dumpi_write_type_delete_attr(const dumpi_type_delete_attr *val, DUMPI_WRITEARGS);
  /** Read a type_delete_attr record (excluding function id) at current position */
  int dumpi_read_type_delete_attr(dumpi_type_delete_attr *val, DUMPI_READARGS);

  /** Write a type_dup record (including function id) at current position */
  int dumpi_write_type_dup(const dumpi_type_dup *val, DUMPI_WRITEARGS);
  /** Read a type_dup record (excluding function id) at current position */
  int dumpi_read_type_dup(dumpi_type_dup *val, DUMPI_READARGS);

  /** Write a type_free_keyval record (including function id) at current position */
  int dumpi_write_type_free_keyval(const dumpi_type_free_keyval *val, DUMPI_WRITEARGS);
  /** Read a type_free_keyval record (excluding function id) at current position */
  int dumpi_read_type_free_keyval(dumpi_type_free_keyval *val, DUMPI_READARGS);

  /** Write a type_get_attr record (including function id) at current position */
  int dumpi_write_type_get_attr(const dumpi_type_get_attr *val, DUMPI_WRITEARGS);
  /** Read a type_get_attr record (excluding function id) at current position */
  int dumpi_read_type_get_attr(dumpi_type_get_attr *val, DUMPI_READARGS);

  /** Write a type_get_contents record (including function id) at current position */
  int dumpi_write_type_get_contents(const dumpi_type_get_contents *val, DUMPI_WRITEARGS);
  /** Read a type_get_contents record (excluding function id) at current position */
  int dumpi_read_type_get_contents(dumpi_type_get_contents *val, DUMPI_READARGS);

  /** Write a type_get_envelope record (including function id) at current position */
  int dumpi_write_type_get_envelope(const dumpi_type_get_envelope *val, DUMPI_WRITEARGS);
  /** Read a type_get_envelope record (excluding function id) at current position */
  int dumpi_read_type_get_envelope(dumpi_type_get_envelope *val, DUMPI_READARGS);

  /** Write a type_get_name record (including function id) at current position */
  int dumpi_write_type_get_name(const dumpi_type_get_name *val, DUMPI_WRITEARGS);
  /** Read a type_get_name record (excluding function id) at current position */
  int dumpi_read_type_get_name(dumpi_type_get_name *val, DUMPI_READARGS);

  /** Write a type_set_attr record (including function id) at current position */
  int dumpi_write_type_set_attr(const dumpi_type_set_attr *val, DUMPI_WRITEARGS);
  /** Read a type_set_attr record (excluding function id) at current position */
  int dumpi_read_type_set_attr(dumpi_type_set_attr *val, DUMPI_READARGS);

  /** Write a type_set_name record (including function id) at current position */
  int dumpi_write_type_set_name(const dumpi_type_set_name *val, DUMPI_WRITEARGS);
  /** Read a type_set_name record (excluding function id) at current position */
  int dumpi_read_type_set_name(dumpi_type_set_name *val, DUMPI_READARGS);

  /** Write a type_match_size record (including function id) at current position */
  int dumpi_write_type_match_size(const dumpi_type_match_size *val, DUMPI_WRITEARGS);
  /** Read a type_match_size record (excluding function id) at current position */
  int dumpi_read_type_match_size(dumpi_type_match_size *val, DUMPI_READARGS);

  /** Write a win_call_errhandler record (including function id) at current position */
  int dumpi_write_win_call_errhandler(const dumpi_win_call_errhandler *val, DUMPI_WRITEARGS);
  /** Read a win_call_errhandler record (excluding function id) at current position */
  int dumpi_read_win_call_errhandler(dumpi_win_call_errhandler *val, DUMPI_READARGS);

  /** Write a win_create_keyval record (including function id) at current position */
  int dumpi_write_win_create_keyval(const dumpi_win_create_keyval *val, DUMPI_WRITEARGS);
  /** Read a win_create_keyval record (excluding function id) at current position */
  int dumpi_read_win_create_keyval(dumpi_win_create_keyval *val, DUMPI_READARGS);

  /** Write a win_delete_attr record (including function id) at current position */
  int dumpi_write_win_delete_attr(const dumpi_win_delete_attr *val, DUMPI_WRITEARGS);
  /** Read a win_delete_attr record (excluding function id) at current position */
  int dumpi_read_win_delete_attr(dumpi_win_delete_attr *val, DUMPI_READARGS);

  /** Write a win_free_keyval record (including function id) at current position */
  int dumpi_write_win_free_keyval(const dumpi_win_free_keyval *val, DUMPI_WRITEARGS);
  /** Read a win_free_keyval record (excluding function id) at current position */
  int dumpi_read_win_free_keyval(dumpi_win_free_keyval *val, DUMPI_READARGS);

  /** Write a win_get_attr record (including function id) at current position */
  int dumpi_write_win_get_attr(const dumpi_win_get_attr *val, DUMPI_WRITEARGS);
  /** Read a win_get_attr record (excluding function id) at current position */
  int dumpi_read_win_get_attr(dumpi_win_get_attr *val, DUMPI_READARGS);

  /** Write a win_get_name record (including function id) at current position */
  int dumpi_write_win_get_name(const dumpi_win_get_name *val, DUMPI_WRITEARGS);
  /** Read a win_get_name record (excluding function id) at current position */
  int dumpi_read_win_get_name(dumpi_win_get_name *val, DUMPI_READARGS);

  /** Write a win_set_attr record (including function id) at current position */
  int dumpi_write_win_set_attr(const dumpi_win_set_attr *val, DUMPI_WRITEARGS);
  /** Read a win_set_attr record (excluding function id) at current position */
  int dumpi_read_win_set_attr(dumpi_win_set_attr *val, DUMPI_READARGS);

  /** Write a win_set_name record (including function id) at current position */
  int dumpi_write_win_set_name(const dumpi_win_set_name *val, DUMPI_WRITEARGS);
  /** Read a win_set_name record (excluding function id) at current position */
  int dumpi_read_win_set_name(dumpi_win_set_name *val, DUMPI_READARGS);

  /** Write a alloc_mem record (including function id) at current position */
  int dumpi_write_alloc_mem(const dumpi_alloc_mem *val, DUMPI_WRITEARGS);
  /** Read a alloc_mem record (excluding function id) at current position */
  int dumpi_read_alloc_mem(dumpi_alloc_mem *val, DUMPI_READARGS);

  /** Write a comm_create_errhandler record (including function id) at current position */
  int dumpi_write_comm_create_errhandler(const dumpi_comm_create_errhandler *val, DUMPI_WRITEARGS);
  /** Read a comm_create_errhandler record (excluding function id) at current position */
  int dumpi_read_comm_create_errhandler(dumpi_comm_create_errhandler *val, DUMPI_READARGS);

  /** Write a comm_get_errhandler record (including function id) at current position */
  int dumpi_write_comm_get_errhandler(const dumpi_comm_get_errhandler *val, DUMPI_WRITEARGS);
  /** Read a comm_get_errhandler record (excluding function id) at current position */
  int dumpi_read_comm_get_errhandler(dumpi_comm_get_errhandler *val, DUMPI_READARGS);

  /** Write a comm_set_errhandler record (including function id) at current position */
  int dumpi_write_comm_set_errhandler(const dumpi_comm_set_errhandler *val, DUMPI_WRITEARGS);
  /** Read a comm_set_errhandler record (excluding function id) at current position */
  int dumpi_read_comm_set_errhandler(dumpi_comm_set_errhandler *val, DUMPI_READARGS);

  /** Write a file_create_errhandler record (including function id) at current position */
  int dumpi_write_file_create_errhandler(const dumpi_file_create_errhandler *val, DUMPI_WRITEARGS);
  /** Read a file_create_errhandler record (excluding function id) at current position */
  int dumpi_read_file_create_errhandler(dumpi_file_create_errhandler *val, DUMPI_READARGS);

  /** Write a file_get_errhandler record (including function id) at current position */
  int dumpi_write_file_get_errhandler(const dumpi_file_get_errhandler *val, DUMPI_WRITEARGS);
  /** Read a file_get_errhandler record (excluding function id) at current position */
  int dumpi_read_file_get_errhandler(dumpi_file_get_errhandler *val, DUMPI_READARGS);

  /** Write a file_set_errhandler record (including function id) at current position */
  int dumpi_write_file_set_errhandler(const dumpi_file_set_errhandler *val, DUMPI_WRITEARGS);
  /** Read a file_set_errhandler record (excluding function id) at current position */
  int dumpi_read_file_set_errhandler(dumpi_file_set_errhandler *val, DUMPI_READARGS);

  /** Write a finalized record (including function id) at current position */
  int dumpi_write_finalized(const dumpi_finalized *val, DUMPI_WRITEARGS);
  /** Read a finalized record (excluding function id) at current position */
  int dumpi_read_finalized(dumpi_finalized *val, DUMPI_READARGS);

  /** Write a free_mem record (including function id) at current position */
  int dumpi_write_free_mem(const dumpi_free_mem *val, DUMPI_WRITEARGS);
  /** Read a free_mem record (excluding function id) at current position */
  int dumpi_read_free_mem(dumpi_free_mem *val, DUMPI_READARGS);

  /** Write a get_address record (including function id) at current position */
  int dumpi_write_get_address(const dumpi_get_address *val, DUMPI_WRITEARGS);
  /** Read a get_address record (excluding function id) at current position */
  int dumpi_read_get_address(dumpi_get_address *val, DUMPI_READARGS);

  /** Write a info_create record (including function id) at current position */
  int dumpi_write_info_create(const dumpi_info_create *val, DUMPI_WRITEARGS);
  /** Read a info_create record (excluding function id) at current position */
  int dumpi_read_info_create(dumpi_info_create *val, DUMPI_READARGS);

  /** Write a info_delete record (including function id) at current position */
  int dumpi_write_info_delete(const dumpi_info_delete *val, DUMPI_WRITEARGS);
  /** Read a info_delete record (excluding function id) at current position */
  int dumpi_read_info_delete(dumpi_info_delete *val, DUMPI_READARGS);

  /** Write a info_dup record (including function id) at current position */
  int dumpi_write_info_dup(const dumpi_info_dup *val, DUMPI_WRITEARGS);
  /** Read a info_dup record (excluding function id) at current position */
  int dumpi_read_info_dup(dumpi_info_dup *val, DUMPI_READARGS);

  /** Write a info_free record (including function id) at current position */
  int dumpi_write_info_free(const dumpi_info_free *val, DUMPI_WRITEARGS);
  /** Read a info_free record (excluding function id) at current position */
  int dumpi_read_info_free(dumpi_info_free *val, DUMPI_READARGS);

  /** Write a info_get record (including function id) at current position */
  int dumpi_write_info_get(const dumpi_info_get *val, DUMPI_WRITEARGS);
  /** Read a info_get record (excluding function id) at current position */
  int dumpi_read_info_get(dumpi_info_get *val, DUMPI_READARGS);

  /** Write a info_get_nkeys record (including function id) at current position */
  int dumpi_write_info_get_nkeys(const dumpi_info_get_nkeys *val, DUMPI_WRITEARGS);
  /** Read a info_get_nkeys record (excluding function id) at current position */
  int dumpi_read_info_get_nkeys(dumpi_info_get_nkeys *val, DUMPI_READARGS);

  /** Write a info_get_nthkey record (including function id) at current position */
  int dumpi_write_info_get_nthkey(const dumpi_info_get_nthkey *val, DUMPI_WRITEARGS);
  /** Read a info_get_nthkey record (excluding function id) at current position */
  int dumpi_read_info_get_nthkey(dumpi_info_get_nthkey *val, DUMPI_READARGS);

  /** Write a info_get_valuelen record (including function id) at current position */
  int dumpi_write_info_get_valuelen(const dumpi_info_get_valuelen *val, DUMPI_WRITEARGS);
  /** Read a info_get_valuelen record (excluding function id) at current position */
  int dumpi_read_info_get_valuelen(dumpi_info_get_valuelen *val, DUMPI_READARGS);

  /** Write a info_set record (including function id) at current position */
  int dumpi_write_info_set(const dumpi_info_set *val, DUMPI_WRITEARGS);
  /** Read a info_set record (excluding function id) at current position */
  int dumpi_read_info_set(dumpi_info_set *val, DUMPI_READARGS);

  /** Write a pack_external record (including function id) at current position */
  int dumpi_write_pack_external(const dumpi_pack_external *val, DUMPI_WRITEARGS);
  /** Read a pack_external record (excluding function id) at current position */
  int dumpi_read_pack_external(dumpi_pack_external *val, DUMPI_READARGS);

  /** Write a pack_external_size record (including function id) at current position */
  int dumpi_write_pack_external_size(const dumpi_pack_external_size *val, DUMPI_WRITEARGS);
  /** Read a pack_external_size record (excluding function id) at current position */
  int dumpi_read_pack_external_size(dumpi_pack_external_size *val, DUMPI_READARGS);

  /** Write a request_get_status record (including function id) at current position */
  int dumpi_write_request_get_status(const dumpi_request_get_status *val, DUMPI_WRITEARGS);
  /** Read a request_get_status record (excluding function id) at current position */
  int dumpi_read_request_get_status(dumpi_request_get_status *val, DUMPI_READARGS);

  /** Write a type_create_darray record (including function id) at current position */
  int dumpi_write_type_create_darray(const dumpi_type_create_darray *val, DUMPI_WRITEARGS);
  /** Read a type_create_darray record (excluding function id) at current position */
  int dumpi_read_type_create_darray(dumpi_type_create_darray *val, DUMPI_READARGS);

  /** Write a type_create_hindexed record (including function id) at current position */
  int dumpi_write_type_create_hindexed(const dumpi_type_create_hindexed *val, DUMPI_WRITEARGS);
  /** Read a type_create_hindexed record (excluding function id) at current position */
  int dumpi_read_type_create_hindexed(dumpi_type_create_hindexed *val, DUMPI_READARGS);

  /** Write a type_create_hvector record (including function id) at current position */
  int dumpi_write_type_create_hvector(const dumpi_type_create_hvector *val, DUMPI_WRITEARGS);
  /** Read a type_create_hvector record (excluding function id) at current position */
  int dumpi_read_type_create_hvector(dumpi_type_create_hvector *val, DUMPI_READARGS);

  /** Write a type_create_indexed_block record (including function id) at current position */
  int dumpi_write_type_create_indexed_block(const dumpi_type_create_indexed_block *val, DUMPI_WRITEARGS);
  /** Read a type_create_indexed_block record (excluding function id) at current position */
  int dumpi_read_type_create_indexed_block(dumpi_type_create_indexed_block *val, DUMPI_READARGS);

  /** Write a type_create_resized record (including function id) at current position */
  int dumpi_write_type_create_resized(const dumpi_type_create_resized *val, DUMPI_WRITEARGS);
  /** Read a type_create_resized record (excluding function id) at current position */
  int dumpi_read_type_create_resized(dumpi_type_create_resized *val, DUMPI_READARGS);

  /** Write a type_create_struct record (including function id) at current position */
  int dumpi_write_type_create_struct(const dumpi_type_create_struct *val, DUMPI_WRITEARGS);
  /** Read a type_create_struct record (excluding function id) at current position */
  int dumpi_read_type_create_struct(dumpi_type_create_struct *val, DUMPI_READARGS);

  /** Write a type_create_subarray record (including function id) at current position */
  int dumpi_write_type_create_subarray(const dumpi_type_create_subarray *val, DUMPI_WRITEARGS);
  /** Read a type_create_subarray record (excluding function id) at current position */
  int dumpi_read_type_create_subarray(dumpi_type_create_subarray *val, DUMPI_READARGS);

  /** Write a type_get_extent record (including function id) at current position */
  int dumpi_write_type_get_extent(const dumpi_type_get_extent *val, DUMPI_WRITEARGS);
  /** Read a type_get_extent record (excluding function id) at current position */
  int dumpi_read_type_get_extent(dumpi_type_get_extent *val, DUMPI_READARGS);

  /** Write a type_get_true_extent record (including function id) at current position */
  int dumpi_write_type_get_true_extent(const dumpi_type_get_true_extent *val, DUMPI_WRITEARGS);
  /** Read a type_get_true_extent record (excluding function id) at current position */
  int dumpi_read_type_get_true_extent(dumpi_type_get_true_extent *val, DUMPI_READARGS);

  /** Write a unpack_external record (including function id) at current position */
  int dumpi_write_unpack_external(const dumpi_unpack_external *val, DUMPI_WRITEARGS);
  /** Read a unpack_external record (excluding function id) at current position */
  int dumpi_read_unpack_external(dumpi_unpack_external *val, DUMPI_READARGS);

  /** Write a win_create_errhandler record (including function id) at current position */
  int dumpi_write_win_create_errhandler(const dumpi_win_create_errhandler *val, DUMPI_WRITEARGS);
  /** Read a win_create_errhandler record (excluding function id) at current position */
  int dumpi_read_win_create_errhandler(dumpi_win_create_errhandler *val, DUMPI_READARGS);

  /** Write a win_get_errhandler record (including function id) at current position */
  int dumpi_write_win_get_errhandler(const dumpi_win_get_errhandler *val, DUMPI_WRITEARGS);
  /** Read a win_get_errhandler record (excluding function id) at current position */
  int dumpi_read_win_get_errhandler(dumpi_win_get_errhandler *val, DUMPI_READARGS);

  /** Write a win_set_errhandler record (including function id) at current position */
  int dumpi_write_win_set_errhandler(const dumpi_win_set_errhandler *val, DUMPI_WRITEARGS);
  /** Read a win_set_errhandler record (excluding function id) at current position */
  int dumpi_read_win_set_errhandler(dumpi_win_set_errhandler *val, DUMPI_READARGS);

  /** Write a file_open record (including function id) at current position */
  int dumpi_write_file_open(const dumpi_file_open *val, DUMPI_WRITEARGS);
  /** Read a file_open record (excluding function id) at current position */
  int dumpi_read_file_open(dumpi_file_open *val, DUMPI_READARGS);

  /** Write a file_close record (including function id) at current position */
  int dumpi_write_file_close(const dumpi_file_close *val, DUMPI_WRITEARGS);
  /** Read a file_close record (excluding function id) at current position */
  int dumpi_read_file_close(dumpi_file_close *val, DUMPI_READARGS);

  /** Write a file_delete record (including function id) at current position */
  int dumpi_write_file_delete(const dumpi_file_delete *val, DUMPI_WRITEARGS);
  /** Read a file_delete record (excluding function id) at current position */
  int dumpi_read_file_delete(dumpi_file_delete *val, DUMPI_READARGS);

  /** Write a file_set_size record (including function id) at current position */
  int dumpi_write_file_set_size(const dumpi_file_set_size *val, DUMPI_WRITEARGS);
  /** Read a file_set_size record (excluding function id) at current position */
  int dumpi_read_file_set_size(dumpi_file_set_size *val, DUMPI_READARGS);

  /** Write a file_preallocate record (including function id) at current position */
  int dumpi_write_file_preallocate(const dumpi_file_preallocate *val, DUMPI_WRITEARGS);
  /** Read a file_preallocate record (excluding function id) at current position */
  int dumpi_read_file_preallocate(dumpi_file_preallocate *val, DUMPI_READARGS);

  /** Write a file_get_size record (including function id) at current position */
  int dumpi_write_file_get_size(const dumpi_file_get_size *val, DUMPI_WRITEARGS);
  /** Read a file_get_size record (excluding function id) at current position */
  int dumpi_read_file_get_size(dumpi_file_get_size *val, DUMPI_READARGS);

  /** Write a file_get_group record (including function id) at current position */
  int dumpi_write_file_get_group(const dumpi_file_get_group *val, DUMPI_WRITEARGS);
  /** Read a file_get_group record (excluding function id) at current position */
  int dumpi_read_file_get_group(dumpi_file_get_group *val, DUMPI_READARGS);

  /** Write a file_get_amode record (including function id) at current position */
  int dumpi_write_file_get_amode(const dumpi_file_get_amode *val, DUMPI_WRITEARGS);
  /** Read a file_get_amode record (excluding function id) at current position */
  int dumpi_read_file_get_amode(dumpi_file_get_amode *val, DUMPI_READARGS);

  /** Write a file_set_info record (including function id) at current position */
  int dumpi_write_file_set_info(const dumpi_file_set_info *val, DUMPI_WRITEARGS);
  /** Read a file_set_info record (excluding function id) at current position */
  int dumpi_read_file_set_info(dumpi_file_set_info *val, DUMPI_READARGS);

  /** Write a file_get_info record (including function id) at current position */
  int dumpi_write_file_get_info(const dumpi_file_get_info *val, DUMPI_WRITEARGS);
  /** Read a file_get_info record (excluding function id) at current position */
  int dumpi_read_file_get_info(dumpi_file_get_info *val, DUMPI_READARGS);

  /** Write a file_set_view record (including function id) at current position */
  int dumpi_write_file_set_view(const dumpi_file_set_view *val, DUMPI_WRITEARGS);
  /** Read a file_set_view record (excluding function id) at current position */
  int dumpi_read_file_set_view(dumpi_file_set_view *val, DUMPI_READARGS);

  /** Write a file_get_view record (including function id) at current position */
  int dumpi_write_file_get_view(const dumpi_file_get_view *val, DUMPI_WRITEARGS);
  /** Read a file_get_view record (excluding function id) at current position */
  int dumpi_read_file_get_view(dumpi_file_get_view *val, DUMPI_READARGS);

  /** Write a file_read_at record (including function id) at current position */
  int dumpi_write_file_read_at(const dumpi_file_read_at *val, DUMPI_WRITEARGS);
  /** Read a file_read_at record (excluding function id) at current position */
  int dumpi_read_file_read_at(dumpi_file_read_at *val, DUMPI_READARGS);

  /** Write a file_read_at_all record (including function id) at current position */
  int dumpi_write_file_read_at_all(const dumpi_file_read_at_all *val, DUMPI_WRITEARGS);
  /** Read a file_read_at_all record (excluding function id) at current position */
  int dumpi_read_file_read_at_all(dumpi_file_read_at_all *val, DUMPI_READARGS);

  /** Write a file_write_at record (including function id) at current position */
  int dumpi_write_file_write_at(const dumpi_file_write_at *val, DUMPI_WRITEARGS);
  /** Read a file_write_at record (excluding function id) at current position */
  int dumpi_read_file_write_at(dumpi_file_write_at *val, DUMPI_READARGS);

  /** Write a file_write_at_all record (including function id) at current position */
  int dumpi_write_file_write_at_all(const dumpi_file_write_at_all *val, DUMPI_WRITEARGS);
  /** Read a file_write_at_all record (excluding function id) at current position */
  int dumpi_read_file_write_at_all(dumpi_file_write_at_all *val, DUMPI_READARGS);

  /** Write a file_iread_at record (including function id) at current position */
  int dumpi_write_file_iread_at(const dumpi_file_iread_at *val, DUMPI_WRITEARGS);
  /** Read a file_iread_at record (excluding function id) at current position */
  int dumpi_read_file_iread_at(dumpi_file_iread_at *val, DUMPI_READARGS);

  /** Write a file_iwrite_at record (including function id) at current position */
  int dumpi_write_file_iwrite_at(const dumpi_file_iwrite_at *val, DUMPI_WRITEARGS);
  /** Read a file_iwrite_at record (excluding function id) at current position */
  int dumpi_read_file_iwrite_at(dumpi_file_iwrite_at *val, DUMPI_READARGS);

  /** Write a file_read record (including function id) at current position */
  int dumpi_write_file_read(const dumpi_file_read *val, DUMPI_WRITEARGS);
  /** Read a file_read record (excluding function id) at current position */
  int dumpi_read_file_read(dumpi_file_read *val, DUMPI_READARGS);

  /** Write a file_read_all record (including function id) at current position */
  int dumpi_write_file_read_all(const dumpi_file_read_all *val, DUMPI_WRITEARGS);
  /** Read a file_read_all record (excluding function id) at current position */
  int dumpi_read_file_read_all(dumpi_file_read_all *val, DUMPI_READARGS);

  /** Write a file_write record (including function id) at current position */
  int dumpi_write_file_write(const dumpi_file_write *val, DUMPI_WRITEARGS);
  /** Read a file_write record (excluding function id) at current position */
  int dumpi_read_file_write(dumpi_file_write *val, DUMPI_READARGS);

  /** Write a file_write_all record (including function id) at current position */
  int dumpi_write_file_write_all(const dumpi_file_write_all *val, DUMPI_WRITEARGS);
  /** Read a file_write_all record (excluding function id) at current position */
  int dumpi_read_file_write_all(dumpi_file_write_all *val, DUMPI_READARGS);

  /** Write a file_iread record (including function id) at current position */
  int dumpi_write_file_iread(const dumpi_file_iread *val, DUMPI_WRITEARGS);
  /** Read a file_iread record (excluding function id) at current position */
  int dumpi_read_file_iread(dumpi_file_iread *val, DUMPI_READARGS);

  /** Write a file_iwrite record (including function id) at current position */
  int dumpi_write_file_iwrite(const dumpi_file_iwrite *val, DUMPI_WRITEARGS);
  /** Read a file_iwrite record (excluding function id) at current position */
  int dumpi_read_file_iwrite(dumpi_file_iwrite *val, DUMPI_READARGS);

  /** Write a file_seek record (including function id) at current position */
  int dumpi_write_file_seek(const dumpi_file_seek *val, DUMPI_WRITEARGS);
  /** Read a file_seek record (excluding function id) at current position */
  int dumpi_read_file_seek(dumpi_file_seek *val, DUMPI_READARGS);

  /** Write a file_get_position record (including function id) at current position */
  int dumpi_write_file_get_position(const dumpi_file_get_position *val, DUMPI_WRITEARGS);
  /** Read a file_get_position record (excluding function id) at current position */
  int dumpi_read_file_get_position(dumpi_file_get_position *val, DUMPI_READARGS);

  /** Write a file_get_byte_offset record (including function id) at current position */
  int dumpi_write_file_get_byte_offset(const dumpi_file_get_byte_offset *val, DUMPI_WRITEARGS);
  /** Read a file_get_byte_offset record (excluding function id) at current position */
  int dumpi_read_file_get_byte_offset(dumpi_file_get_byte_offset *val, DUMPI_READARGS);

  /** Write a file_read_shared record (including function id) at current position */
  int dumpi_write_file_read_shared(const dumpi_file_read_shared *val, DUMPI_WRITEARGS);
  /** Read a file_read_shared record (excluding function id) at current position */
  int dumpi_read_file_read_shared(dumpi_file_read_shared *val, DUMPI_READARGS);

  /** Write a file_write_shared record (including function id) at current position */
  int dumpi_write_file_write_shared(const dumpi_file_write_shared *val, DUMPI_WRITEARGS);
  /** Read a file_write_shared record (excluding function id) at current position */
  int dumpi_read_file_write_shared(dumpi_file_write_shared *val, DUMPI_READARGS);

  /** Write a file_iread_shared record (including function id) at current position */
  int dumpi_write_file_iread_shared(const dumpi_file_iread_shared *val, DUMPI_WRITEARGS);
  /** Read a file_iread_shared record (excluding function id) at current position */
  int dumpi_read_file_iread_shared(dumpi_file_iread_shared *val, DUMPI_READARGS);

  /** Write a file_iwrite_shared record (including function id) at current position */
  int dumpi_write_file_iwrite_shared(const dumpi_file_iwrite_shared *val, DUMPI_WRITEARGS);
  /** Read a file_iwrite_shared record (excluding function id) at current position */
  int dumpi_read_file_iwrite_shared(dumpi_file_iwrite_shared *val, DUMPI_READARGS);

  /** Write a file_read_ordered record (including function id) at current position */
  int dumpi_write_file_read_ordered(const dumpi_file_read_ordered *val, DUMPI_WRITEARGS);
  /** Read a file_read_ordered record (excluding function id) at current position */
  int dumpi_read_file_read_ordered(dumpi_file_read_ordered *val, DUMPI_READARGS);

  /** Write a file_write_ordered record (including function id) at current position */
  int dumpi_write_file_write_ordered(const dumpi_file_write_ordered *val, DUMPI_WRITEARGS);
  /** Read a file_write_ordered record (excluding function id) at current position */
  int dumpi_read_file_write_ordered(dumpi_file_write_ordered *val, DUMPI_READARGS);

  /** Write a file_seek_shared record (including function id) at current position */
  int dumpi_write_file_seek_shared(const dumpi_file_seek_shared *val, DUMPI_WRITEARGS);
  /** Read a file_seek_shared record (excluding function id) at current position */
  int dumpi_read_file_seek_shared(dumpi_file_seek_shared *val, DUMPI_READARGS);

  /** Write a file_get_position_shared record (including function id) at current position */
  int dumpi_write_file_get_position_shared(const dumpi_file_get_position_shared *val, DUMPI_WRITEARGS);
  /** Read a file_get_position_shared record (excluding function id) at current position */
  int dumpi_read_file_get_position_shared(dumpi_file_get_position_shared *val, DUMPI_READARGS);

  /** Write a file_read_at_all_begin record (including function id) at current position */
  int dumpi_write_file_read_at_all_begin(const dumpi_file_read_at_all_begin *val, DUMPI_WRITEARGS);
  /** Read a file_read_at_all_begin record (excluding function id) at current position */
  int dumpi_read_file_read_at_all_begin(dumpi_file_read_at_all_begin *val, DUMPI_READARGS);

  /** Write a file_read_at_all_end record (including function id) at current position */
  int dumpi_write_file_read_at_all_end(const dumpi_file_read_at_all_end *val, DUMPI_WRITEARGS);
  /** Read a file_read_at_all_end record (excluding function id) at current position */
  int dumpi_read_file_read_at_all_end(dumpi_file_read_at_all_end *val, DUMPI_READARGS);

  /** Write a file_write_at_all_begin record (including function id) at current position */
  int dumpi_write_file_write_at_all_begin(const dumpi_file_write_at_all_begin *val, DUMPI_WRITEARGS);
  /** Read a file_write_at_all_begin record (excluding function id) at current position */
  int dumpi_read_file_write_at_all_begin(dumpi_file_write_at_all_begin *val, DUMPI_READARGS);

  /** Write a file_write_at_all_end record (including function id) at current position */
  int dumpi_write_file_write_at_all_end(const dumpi_file_write_at_all_end *val, DUMPI_WRITEARGS);
  /** Read a file_write_at_all_end record (excluding function id) at current position */
  int dumpi_read_file_write_at_all_end(dumpi_file_write_at_all_end *val, DUMPI_READARGS);

  /** Write a file_read_all_begin record (including function id) at current position */
  int dumpi_write_file_read_all_begin(const dumpi_file_read_all_begin *val, DUMPI_WRITEARGS);
  /** Read a file_read_all_begin record (excluding function id) at current position */
  int dumpi_read_file_read_all_begin(dumpi_file_read_all_begin *val, DUMPI_READARGS);

  /** Write a file_read_all_end record (including function id) at current position */
  int dumpi_write_file_read_all_end(const dumpi_file_read_all_end *val, DUMPI_WRITEARGS);
  /** Read a file_read_all_end record (excluding function id) at current position */
  int dumpi_read_file_read_all_end(dumpi_file_read_all_end *val, DUMPI_READARGS);

  /** Write a file_write_all_begin record (including function id) at current position */
  int dumpi_write_file_write_all_begin(const dumpi_file_write_all_begin *val, DUMPI_WRITEARGS);
  /** Read a file_write_all_begin record (excluding function id) at current position */
  int dumpi_read_file_write_all_begin(dumpi_file_write_all_begin *val, DUMPI_READARGS);

  /** Write a file_write_all_end record (including function id) at current position */
  int dumpi_write_file_write_all_end(const dumpi_file_write_all_end *val, DUMPI_WRITEARGS);
  /** Read a file_write_all_end record (excluding function id) at current position */
  int dumpi_read_file_write_all_end(dumpi_file_write_all_end *val, DUMPI_READARGS);

  /** Write a file_read_ordered_begin record (including function id) at current position */
  int dumpi_write_file_read_ordered_begin(const dumpi_file_read_ordered_begin *val, DUMPI_WRITEARGS);
  /** Read a file_read_ordered_begin record (excluding function id) at current position */
  int dumpi_read_file_read_ordered_begin(dumpi_file_read_ordered_begin *val, DUMPI_READARGS);

  /** Write a file_read_ordered_end record (including function id) at current position */
  int dumpi_write_file_read_ordered_end(const dumpi_file_read_ordered_end *val, DUMPI_WRITEARGS);
  /** Read a file_read_ordered_end record (excluding function id) at current position */
  int dumpi_read_file_read_ordered_end(dumpi_file_read_ordered_end *val, DUMPI_READARGS);

  /** Write a file_write_ordered_begin record (including function id) at current position */
  int dumpi_write_file_write_ordered_begin(const dumpi_file_write_ordered_begin *val, DUMPI_WRITEARGS);
  /** Read a file_write_ordered_begin record (excluding function id) at current position */
  int dumpi_read_file_write_ordered_begin(dumpi_file_write_ordered_begin *val, DUMPI_READARGS);

  /** Write a file_write_ordered_end record (including function id) at current position */
  int dumpi_write_file_write_ordered_end(const dumpi_file_write_ordered_end *val, DUMPI_WRITEARGS);
  /** Read a file_write_ordered_end record (excluding function id) at current position */
  int dumpi_read_file_write_ordered_end(dumpi_file_write_ordered_end *val, DUMPI_READARGS);

  /** Write a file_get_type_extent record (including function id) at current position */
  int dumpi_write_file_get_type_extent(const dumpi_file_get_type_extent *val, DUMPI_WRITEARGS);
  /** Read a file_get_type_extent record (excluding function id) at current position */
  int dumpi_read_file_get_type_extent(dumpi_file_get_type_extent *val, DUMPI_READARGS);

  /** Write a register_datarep record (including function id) at current position */
  int dumpi_write_register_datarep(const dumpi_register_datarep *val, DUMPI_WRITEARGS);
  /** Read a register_datarep record (excluding function id) at current position */
  int dumpi_read_register_datarep(dumpi_register_datarep *val, DUMPI_READARGS);

  /** Write a file_set_atomicity record (including function id) at current position */
  int dumpi_write_file_set_atomicity(const dumpi_file_set_atomicity *val, DUMPI_WRITEARGS);
  /** Read a file_set_atomicity record (excluding function id) at current position */
  int dumpi_read_file_set_atomicity(dumpi_file_set_atomicity *val, DUMPI_READARGS);

  /** Write a file_get_atomicity record (including function id) at current position */
  int dumpi_write_file_get_atomicity(const dumpi_file_get_atomicity *val, DUMPI_WRITEARGS);
  /** Read a file_get_atomicity record (excluding function id) at current position */
  int dumpi_read_file_get_atomicity(dumpi_file_get_atomicity *val, DUMPI_READARGS);

  /** Write a file_sync record (including function id) at current position */
  int dumpi_write_file_sync(const dumpi_file_sync *val, DUMPI_WRITEARGS);
  /** Read a file_sync record (excluding function id) at current position */
  int dumpi_read_file_sync(dumpi_file_sync *val, DUMPI_READARGS);

  /** Write an IO test record (including function id) at current position */
  int dumpio_write_test(const dumpio_test *val, DUMPI_WRITEARGS);
  /** Read an IO test record (excluding function id) at current position */
  int dumpio_read_test(dumpio_test *val, DUMPI_READARGS);

  /** Write an IO wait record (including function id) at current position */
  int dumpio_write_wait(const dumpio_wait *val, DUMPI_WRITEARGS);
  /** Read an IO wait record (excluding function id) at current position */
  int dumpio_read_wait(dumpio_wait *val, DUMPI_READARGS);

  /** Write an IO testall record (including function id) at current position */
  int dumpio_write_testall(const dumpio_testall *val, DUMPI_WRITEARGS);
  /** Read an IO testall record (excluding function id) at current position */
  int dumpio_read_testall(dumpio_testall *val, DUMPI_READARGS);

  /** Write an IO waitall record (including function id) at current position */
  int dumpio_write_waitall(const dumpio_waitall *val, DUMPI_WRITEARGS);
  /** Read an IO waitall record (excluding function id) at current position */
  int dumpio_read_waitall(dumpio_waitall *val, DUMPI_READARGS);

  /** Write an IO testany record (including function id) at current position */
  int dumpio_write_testany(const dumpio_testany *val, DUMPI_WRITEARGS);
  /** Read an IO testany record (excluding function id) at current position */
  int dumpio_read_testany(dumpio_testany *val, DUMPI_READARGS);

  /** Write an IO waitany record (including function id) at current position */
  int dumpio_write_waitany(const dumpio_waitany *val, DUMPI_WRITEARGS);
  /** Read an IO waitany record (excluding function id) at current position */
  int dumpio_read_waitany(dumpio_waitany *val, DUMPI_READARGS);

  /** Write an IO waitsome record (including function id) at current position */
  int dumpio_write_waitsome(const dumpio_waitsome *val, DUMPI_WRITEARGS);
  /** Read an IO waitsome record (excluding function id) at current position */
  int dumpio_read_waitsome(dumpio_waitsome *val, DUMPI_READARGS);

  /** Write an IO testsome record (including function id) at current position */
  int dumpio_write_testsome(const dumpio_testsome *val, DUMPI_WRITEARGS);
  /** Read an IO testsome record (excluding function id) at current position */
  int dumpio_read_testsome(dumpio_testsome *val, DUMPI_READARGS);
  
  /*@}*/

#ifdef __cplusplus
} /* End of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_DUMPIIO_H */