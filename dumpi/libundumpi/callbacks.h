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

#ifndef DUMPI_LIBUNDUMPI_CALLBACKS_H
#define DUMPI_LIBUNDUMPI_CALLBACKS_H

#include <dumpi/common/argtypes.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup libundumpi
   * \defgroup libundumpi_callbacks libundumpi/callbacks: \
   *   User callbacks for parsing DUMPI files.
   */
  /*@{*/ /* Place subsequent doxygen comments in the callbacks group */

  /* Callbacks for profiling (enter and exit functions) */
  /* OBSOLETE:  To be erased */
  /*
  typedef int (*dumpi_func_enter_call)(const dumpi_func_call *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpi_func_exit_call)(const dumpi_func_call *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  */

  /* Register callbacks for MPI function types */

  /** Signature for user-defined callback to handle MPI send calls. */
  typedef int (*dumpi_send_call)(const dumpi_send *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI recv calls. */
  typedef int (*dumpi_recv_call)(const dumpi_recv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI get_count calls. */
  typedef int (*dumpi_get_count_call)(const dumpi_get_count *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI bsend calls. */
  typedef int (*dumpi_bsend_call)(const dumpi_bsend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI ssend calls. */
  typedef int (*dumpi_ssend_call)(const dumpi_ssend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI rsend calls. */
  typedef int (*dumpi_rsend_call)(const dumpi_rsend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI buffer_attach calls. */
  typedef int (*dumpi_buffer_attach_call)(const dumpi_buffer_attach *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI buffer_detach calls. */
  typedef int (*dumpi_buffer_detach_call)(const dumpi_buffer_detach *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI isend calls. */
  typedef int (*dumpi_isend_call)(const dumpi_isend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI ibsend calls. */
  typedef int (*dumpi_ibsend_call)(const dumpi_ibsend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI issend calls. */
  typedef int (*dumpi_issend_call)(const dumpi_issend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI irsend calls. */
  typedef int (*dumpi_irsend_call)(const dumpi_irsend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI irecv calls. */
  typedef int (*dumpi_irecv_call)(const dumpi_irecv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI wait calls. */
  typedef int (*dumpi_wait_call)(const dumpi_wait *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI test calls. */
  typedef int (*dumpi_test_call)(const dumpi_test *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI request_free calls. */
  typedef int (*dumpi_request_free_call)(const dumpi_request_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI waitany calls. */
  typedef int (*dumpi_waitany_call)(const dumpi_waitany *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI testany calls. */
  typedef int (*dumpi_testany_call)(const dumpi_testany *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI waitall calls. */
  typedef int (*dumpi_waitall_call)(const dumpi_waitall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI testall calls. */
  typedef int (*dumpi_testall_call)(const dumpi_testall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI waitsome calls. */
  typedef int (*dumpi_waitsome_call)(const dumpi_waitsome *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI testsome calls. */
  typedef int (*dumpi_testsome_call)(const dumpi_testsome *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI iprobe calls. */
  typedef int (*dumpi_iprobe_call)(const dumpi_iprobe *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI probe calls. */
  typedef int (*dumpi_probe_call)(const dumpi_probe *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cancel calls. */
  typedef int (*dumpi_cancel_call)(const dumpi_cancel *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI test_cancelled calls. */
  typedef int (*dumpi_test_cancelled_call)(const dumpi_test_cancelled *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI send_init calls. */
  typedef int (*dumpi_send_init_call)(const dumpi_send_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI bsend_init calls. */
  typedef int (*dumpi_bsend_init_call)(const dumpi_bsend_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI ssend_init calls. */
  typedef int (*dumpi_ssend_init_call)(const dumpi_ssend_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI rsend_init calls. */
  typedef int (*dumpi_rsend_init_call)(const dumpi_rsend_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI recv_init calls. */
  typedef int (*dumpi_recv_init_call)(const dumpi_recv_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI start calls. */
  typedef int (*dumpi_start_call)(const dumpi_start *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI startall calls. */
  typedef int (*dumpi_startall_call)(const dumpi_startall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI sendrecv calls. */
  typedef int (*dumpi_sendrecv_call)(const dumpi_sendrecv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI sendrecv_replace calls. */
  typedef int (*dumpi_sendrecv_replace_call)(const dumpi_sendrecv_replace *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_contiguous calls. */
  typedef int (*dumpi_type_contiguous_call)(const dumpi_type_contiguous *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_vector calls. */
  typedef int (*dumpi_type_vector_call)(const dumpi_type_vector *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_hvector calls. */
  typedef int (*dumpi_type_hvector_call)(const dumpi_type_hvector *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_indexed calls. */
  typedef int (*dumpi_type_indexed_call)(const dumpi_type_indexed *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_hindexed calls. */
  typedef int (*dumpi_type_hindexed_call)(const dumpi_type_hindexed *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_struct calls. */
  typedef int (*dumpi_type_struct_call)(const dumpi_type_struct *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI address calls. */
  typedef int (*dumpi_address_call)(const dumpi_address *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_extent calls. */
  typedef int (*dumpi_type_extent_call)(const dumpi_type_extent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_size calls. */
  typedef int (*dumpi_type_size_call)(const dumpi_type_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_lb calls. */
  typedef int (*dumpi_type_lb_call)(const dumpi_type_lb *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_ub calls. */
  typedef int (*dumpi_type_ub_call)(const dumpi_type_ub *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_commit calls. */
  typedef int (*dumpi_type_commit_call)(const dumpi_type_commit *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_free calls. */
  typedef int (*dumpi_type_free_call)(const dumpi_type_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI get_elements calls. */
  typedef int (*dumpi_get_elements_call)(const dumpi_get_elements *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI pack calls. */
  typedef int (*dumpi_pack_call)(const dumpi_pack *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI unpack calls. */
  typedef int (*dumpi_unpack_call)(const dumpi_unpack *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI pack_size calls. */
  typedef int (*dumpi_pack_size_call)(const dumpi_pack_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI barrier calls. */
  typedef int (*dumpi_barrier_call)(const dumpi_barrier *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI bcast calls. */
  typedef int (*dumpi_bcast_call)(const dumpi_bcast *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI gather calls. */
  typedef int (*dumpi_gather_call)(const dumpi_gather *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI gatherv calls. */
  typedef int (*dumpi_gatherv_call)(const dumpi_gatherv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI scatter calls. */
  typedef int (*dumpi_scatter_call)(const dumpi_scatter *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI scatterv calls. */
  typedef int (*dumpi_scatterv_call)(const dumpi_scatterv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI allgather calls. */
  typedef int (*dumpi_allgather_call)(const dumpi_allgather *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI allgatherv calls. */
  typedef int (*dumpi_allgatherv_call)(const dumpi_allgatherv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI alltoall calls. */
  typedef int (*dumpi_alltoall_call)(const dumpi_alltoall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI alltoallv calls. */
  typedef int (*dumpi_alltoallv_call)(const dumpi_alltoallv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI reduce calls. */
  typedef int (*dumpi_reduce_call)(const dumpi_reduce *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI op_create calls. */
  typedef int (*dumpi_op_create_call)(const dumpi_op_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI op_free calls. */
  typedef int (*dumpi_op_free_call)(const dumpi_op_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI allreduce calls. */
  typedef int (*dumpi_allreduce_call)(const dumpi_allreduce *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI reduce_scatter calls. */
  typedef int (*dumpi_reduce_scatter_call)(const dumpi_reduce_scatter *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI scan calls. */
  typedef int (*dumpi_scan_call)(const dumpi_scan *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_size calls. */
  typedef int (*dumpi_group_size_call)(const dumpi_group_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_rank calls. */
  typedef int (*dumpi_group_rank_call)(const dumpi_group_rank *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_translate_ranks calls. */
  typedef int (*dumpi_group_translate_ranks_call)(const dumpi_group_translate_ranks *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_compare calls. */
  typedef int (*dumpi_group_compare_call)(const dumpi_group_compare *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_group calls. */
  typedef int (*dumpi_comm_group_call)(const dumpi_comm_group *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_union calls. */
  typedef int (*dumpi_group_union_call)(const dumpi_group_union *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_intersection calls. */
  typedef int (*dumpi_group_intersection_call)(const dumpi_group_intersection *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_difference calls. */
  typedef int (*dumpi_group_difference_call)(const dumpi_group_difference *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_incl calls. */
  typedef int (*dumpi_group_incl_call)(const dumpi_group_incl *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_excl calls. */
  typedef int (*dumpi_group_excl_call)(const dumpi_group_excl *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_range_incl calls. */
  typedef int (*dumpi_group_range_incl_call)(const dumpi_group_range_incl *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_range_excl calls. */
  typedef int (*dumpi_group_range_excl_call)(const dumpi_group_range_excl *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI group_free calls. */
  typedef int (*dumpi_group_free_call)(const dumpi_group_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_size calls. */
  typedef int (*dumpi_comm_size_call)(const dumpi_comm_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_rank calls. */
  typedef int (*dumpi_comm_rank_call)(const dumpi_comm_rank *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_compare calls. */
  typedef int (*dumpi_comm_compare_call)(const dumpi_comm_compare *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_dup calls. */
  typedef int (*dumpi_comm_dup_call)(const dumpi_comm_dup *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_create calls. */
  typedef int (*dumpi_comm_create_call)(const dumpi_comm_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_split calls. */
  typedef int (*dumpi_comm_split_call)(const dumpi_comm_split *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_free calls. */
  typedef int (*dumpi_comm_free_call)(const dumpi_comm_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_test_inter calls. */
  typedef int (*dumpi_comm_test_inter_call)(const dumpi_comm_test_inter *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_remote_size calls. */
  typedef int (*dumpi_comm_remote_size_call)(const dumpi_comm_remote_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_remote_group calls. */
  typedef int (*dumpi_comm_remote_group_call)(const dumpi_comm_remote_group *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI intercomm_create calls. */
  typedef int (*dumpi_intercomm_create_call)(const dumpi_intercomm_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI intercomm_merge calls. */
  typedef int (*dumpi_intercomm_merge_call)(const dumpi_intercomm_merge *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI keyval_create calls. */
  typedef int (*dumpi_keyval_create_call)(const dumpi_keyval_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI keyval_free calls. */
  typedef int (*dumpi_keyval_free_call)(const dumpi_keyval_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI attr_put calls. */
  typedef int (*dumpi_attr_put_call)(const dumpi_attr_put *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI attr_get calls. */
  typedef int (*dumpi_attr_get_call)(const dumpi_attr_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI attr_delete calls. */
  typedef int (*dumpi_attr_delete_call)(const dumpi_attr_delete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI topo_test calls. */
  typedef int (*dumpi_topo_test_call)(const dumpi_topo_test *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cart_create calls. */
  typedef int (*dumpi_cart_create_call)(const dumpi_cart_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI dims_create calls. */
  typedef int (*dumpi_dims_create_call)(const dumpi_dims_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI graph_create calls. */
  typedef int (*dumpi_graph_create_call)(const dumpi_graph_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI graphdims_get calls. */
  typedef int (*dumpi_graphdims_get_call)(const dumpi_graphdims_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI graph_get calls. */
  typedef int (*dumpi_graph_get_call)(const dumpi_graph_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cartdim_get calls. */
  typedef int (*dumpi_cartdim_get_call)(const dumpi_cartdim_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cart_get calls. */
  typedef int (*dumpi_cart_get_call)(const dumpi_cart_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cart_rank calls. */
  typedef int (*dumpi_cart_rank_call)(const dumpi_cart_rank *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cart_coords calls. */
  typedef int (*dumpi_cart_coords_call)(const dumpi_cart_coords *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI graph_neighbors_count calls. */
  typedef int (*dumpi_graph_neighbors_count_call)(const dumpi_graph_neighbors_count *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI graph_neighbors calls. */
  typedef int (*dumpi_graph_neighbors_call)(const dumpi_graph_neighbors *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cart_shift calls. */
  typedef int (*dumpi_cart_shift_call)(const dumpi_cart_shift *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cart_sub calls. */
  typedef int (*dumpi_cart_sub_call)(const dumpi_cart_sub *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI cart_map calls. */
  typedef int (*dumpi_cart_map_call)(const dumpi_cart_map *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI graph_map calls. */
  typedef int (*dumpi_graph_map_call)(const dumpi_graph_map *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI get_processor_name calls. */
  typedef int (*dumpi_get_processor_name_call)(const dumpi_get_processor_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI get_version calls. */
  typedef int (*dumpi_get_version_call)(const dumpi_get_version *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI errhandler_create calls. */
  typedef int (*dumpi_errhandler_create_call)(const dumpi_errhandler_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI errhandler_set calls. */
  typedef int (*dumpi_errhandler_set_call)(const dumpi_errhandler_set *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI errhandler_get calls. */
  typedef int (*dumpi_errhandler_get_call)(const dumpi_errhandler_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI errhandler_free calls. */
  typedef int (*dumpi_errhandler_free_call)(const dumpi_errhandler_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI error_string calls. */
  typedef int (*dumpi_error_string_call)(const dumpi_error_string *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI error_class calls. */
  typedef int (*dumpi_error_class_call)(const dumpi_error_class *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI wtime calls. */
  typedef int (*dumpi_wtime_call)(const dumpi_wtime *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI wtick calls. */
  typedef int (*dumpi_wtick_call)(const dumpi_wtick *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI init calls. */
  typedef int (*dumpi_init_call)(const dumpi_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI finalize calls. */
  typedef int (*dumpi_finalize_call)(const dumpi_finalize *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI initialized calls. */
  typedef int (*dumpi_initialized_call)(const dumpi_initialized *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI abort calls. */
  typedef int (*dumpi_abort_call)(const dumpi_abort *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI close_port calls. */
  typedef int (*dumpi_close_port_call)(const dumpi_close_port *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_accept calls. */
  typedef int (*dumpi_comm_accept_call)(const dumpi_comm_accept *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_connect calls. */
  typedef int (*dumpi_comm_connect_call)(const dumpi_comm_connect *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_disconnect calls. */
  typedef int (*dumpi_comm_disconnect_call)(const dumpi_comm_disconnect *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_get_parent calls. */
  typedef int (*dumpi_comm_get_parent_call)(const dumpi_comm_get_parent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_join calls. */
  typedef int (*dumpi_comm_join_call)(const dumpi_comm_join *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_spawn calls. */
  typedef int (*dumpi_comm_spawn_call)(const dumpi_comm_spawn *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_spawn_multiple calls. */
  typedef int (*dumpi_comm_spawn_multiple_call)(const dumpi_comm_spawn_multiple *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI lookup_name calls. */
  typedef int (*dumpi_lookup_name_call)(const dumpi_lookup_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI open_port calls. */
  typedef int (*dumpi_open_port_call)(const dumpi_open_port *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI publish_name calls. */
  typedef int (*dumpi_publish_name_call)(const dumpi_publish_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI unpublish_name calls. */
  typedef int (*dumpi_unpublish_name_call)(const dumpi_unpublish_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI accumulate calls. */
  typedef int (*dumpi_accumulate_call)(const dumpi_accumulate *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI get calls. */
  typedef int (*dumpi_get_call)(const dumpi_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI put calls. */
  typedef int (*dumpi_put_call)(const dumpi_put *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_complete calls. */
  typedef int (*dumpi_win_complete_call)(const dumpi_win_complete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_create calls. */
  typedef int (*dumpi_win_create_call)(const dumpi_win_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_fence calls. */
  typedef int (*dumpi_win_fence_call)(const dumpi_win_fence *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_free calls. */
  typedef int (*dumpi_win_free_call)(const dumpi_win_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_get_group calls. */
  typedef int (*dumpi_win_get_group_call)(const dumpi_win_get_group *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_lock calls. */
  typedef int (*dumpi_win_lock_call)(const dumpi_win_lock *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_post calls. */
  typedef int (*dumpi_win_post_call)(const dumpi_win_post *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_start calls. */
  typedef int (*dumpi_win_start_call)(const dumpi_win_start *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_test calls. */
  typedef int (*dumpi_win_test_call)(const dumpi_win_test *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_unlock calls. */
  typedef int (*dumpi_win_unlock_call)(const dumpi_win_unlock *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_wait calls. */
  typedef int (*dumpi_win_wait_call)(const dumpi_win_wait *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI alltoallw calls. */
  typedef int (*dumpi_alltoallw_call)(const dumpi_alltoallw *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI exscan calls. */
  typedef int (*dumpi_exscan_call)(const dumpi_exscan *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI add_error_class calls. */
  typedef int (*dumpi_add_error_class_call)(const dumpi_add_error_class *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI add_error_code calls. */
  typedef int (*dumpi_add_error_code_call)(const dumpi_add_error_code *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI add_error_string calls. */
  typedef int (*dumpi_add_error_string_call)(const dumpi_add_error_string *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm calls. */
  typedef int (*dumpi_comm_call_errhandler_call)(const dumpi_comm_call_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_create_keyval calls. */
  typedef int (*dumpi_comm_create_keyval_call)(const dumpi_comm_create_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_delete_attr calls. */
  typedef int (*dumpi_comm_delete_attr_call)(const dumpi_comm_delete_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_free_keyval calls. */
  typedef int (*dumpi_comm_free_keyval_call)(const dumpi_comm_free_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_get_attr calls. */
  typedef int (*dumpi_comm_get_attr_call)(const dumpi_comm_get_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_get_name calls. */
  typedef int (*dumpi_comm_get_name_call)(const dumpi_comm_get_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_set_attr calls. */
  typedef int (*dumpi_comm_set_attr_call)(const dumpi_comm_set_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_set_name calls. */
  typedef int (*dumpi_comm_set_name_call)(const dumpi_comm_set_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file calls. */
  typedef int (*dumpi_file_call_errhandler_call)(const dumpi_file_call_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI grequest_complete calls. */
  typedef int (*dumpi_grequest_complete_call)(const dumpi_grequest_complete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI grequest_start calls. */
  typedef int (*dumpi_grequest_start_call)(const dumpi_grequest_start *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI init_thread calls. */
  typedef int (*dumpi_init_thread_call)(const dumpi_init_thread *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI is_thread_main calls. */
  typedef int (*dumpi_is_thread_main_call)(const dumpi_is_thread_main *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI query_thread calls. */
  typedef int (*dumpi_query_thread_call)(const dumpi_query_thread *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI status_set_cancelled calls. */
  typedef int (*dumpi_status_set_cancelled_call)(const dumpi_status_set_cancelled *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI status_set_elements calls. */
  typedef int (*dumpi_status_set_elements_call)(const dumpi_status_set_elements *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_create_keyval calls. */
  typedef int (*dumpi_type_create_keyval_call)(const dumpi_type_create_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_delete_attr calls. */
  typedef int (*dumpi_type_delete_attr_call)(const dumpi_type_delete_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_dup calls. */
  typedef int (*dumpi_type_dup_call)(const dumpi_type_dup *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_free_keyval calls. */
  typedef int (*dumpi_type_free_keyval_call)(const dumpi_type_free_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_get_attr calls. */
  typedef int (*dumpi_type_get_attr_call)(const dumpi_type_get_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_get_contents calls. */
  typedef int (*dumpi_type_get_contents_call)(const dumpi_type_get_contents *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_get_envelope calls. */
  typedef int (*dumpi_type_get_envelope_call)(const dumpi_type_get_envelope *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_get_name calls. */
  typedef int (*dumpi_type_get_name_call)(const dumpi_type_get_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_set_attr calls. */
  typedef int (*dumpi_type_set_attr_call)(const dumpi_type_set_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_set_name calls. */
  typedef int (*dumpi_type_set_name_call)(const dumpi_type_set_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_match_size calls. */
  typedef int (*dumpi_type_match_size_call)(const dumpi_type_match_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win calls. */
  typedef int (*dumpi_win_call_errhandler_call)(const dumpi_win_call_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_create_keyval calls. */
  typedef int (*dumpi_win_create_keyval_call)(const dumpi_win_create_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_delete_attr calls. */
  typedef int (*dumpi_win_delete_attr_call)(const dumpi_win_delete_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_free_keyval calls. */
  typedef int (*dumpi_win_free_keyval_call)(const dumpi_win_free_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_get_attr calls. */
  typedef int (*dumpi_win_get_attr_call)(const dumpi_win_get_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_get_name calls. */
  typedef int (*dumpi_win_get_name_call)(const dumpi_win_get_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_set_attr calls. */
  typedef int (*dumpi_win_set_attr_call)(const dumpi_win_set_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_set_name calls. */
  typedef int (*dumpi_win_set_name_call)(const dumpi_win_set_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI alloc_mem calls. */
  typedef int (*dumpi_alloc_mem_call)(const dumpi_alloc_mem *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_create_errhandler calls. */
  typedef int (*dumpi_comm_create_errhandler_call)(const dumpi_comm_create_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_get_errhandler calls. */
  typedef int (*dumpi_comm_get_errhandler_call)(const dumpi_comm_get_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI comm_set_errhandler calls. */
  typedef int (*dumpi_comm_set_errhandler_call)(const dumpi_comm_set_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_create_errhandler calls. */
  typedef int (*dumpi_file_create_errhandler_call)(const dumpi_file_create_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_errhandler calls. */
  typedef int (*dumpi_file_get_errhandler_call)(const dumpi_file_get_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_set_errhandler calls. */
  typedef int (*dumpi_file_set_errhandler_call)(const dumpi_file_set_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI finalized calls. */
  typedef int (*dumpi_finalized_call)(const dumpi_finalized *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI free_mem calls. */
  typedef int (*dumpi_free_mem_call)(const dumpi_free_mem *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI get_address calls. */
  typedef int (*dumpi_get_address_call)(const dumpi_get_address *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_create calls. */
  typedef int (*dumpi_info_create_call)(const dumpi_info_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_delete calls. */
  typedef int (*dumpi_info_delete_call)(const dumpi_info_delete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_dup calls. */
  typedef int (*dumpi_info_dup_call)(const dumpi_info_dup *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_free calls. */
  typedef int (*dumpi_info_free_call)(const dumpi_info_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_get calls. */
  typedef int (*dumpi_info_get_call)(const dumpi_info_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_get_nkeys calls. */
  typedef int (*dumpi_info_get_nkeys_call)(const dumpi_info_get_nkeys *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_get_nthkey calls. */
  typedef int (*dumpi_info_get_nthkey_call)(const dumpi_info_get_nthkey *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_get_valuelen calls. */
  typedef int (*dumpi_info_get_valuelen_call)(const dumpi_info_get_valuelen *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI info_set calls. */
  typedef int (*dumpi_info_set_call)(const dumpi_info_set *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI pack_external calls. */
  typedef int (*dumpi_pack_external_call)(const dumpi_pack_external *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI pack_external_size calls. */
  typedef int (*dumpi_pack_external_size_call)(const dumpi_pack_external_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI request_get_status calls. */
  typedef int (*dumpi_request_get_status_call)(const dumpi_request_get_status *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_create_darray calls. */
  typedef int (*dumpi_type_create_darray_call)(const dumpi_type_create_darray *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_create_hindexed calls. */
  typedef int (*dumpi_type_create_hindexed_call)(const dumpi_type_create_hindexed *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_create_hvector calls. */
  typedef int (*dumpi_type_create_hvector_call)(const dumpi_type_create_hvector *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_create_indexed_block calls. */
  typedef int (*dumpi_type_create_indexed_block_call)(const dumpi_type_create_indexed_block *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_create_resized calls. */
  typedef int (*dumpi_type_create_resized_call)(const dumpi_type_create_resized *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_create_struct calls. */
  typedef int (*dumpi_type_create_struct_call)(const dumpi_type_create_struct *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_create_subarray calls. */
  typedef int (*dumpi_type_create_subarray_call)(const dumpi_type_create_subarray *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_get_extent calls. */
  typedef int (*dumpi_type_get_extent_call)(const dumpi_type_get_extent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI type_get_true_extent calls. */
  typedef int (*dumpi_type_get_true_extent_call)(const dumpi_type_get_true_extent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI unpack_external calls. */
  typedef int (*dumpi_unpack_external_call)(const dumpi_unpack_external *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_create_errhandler calls. */
  typedef int (*dumpi_win_create_errhandler_call)(const dumpi_win_create_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_get_errhandler calls. */
  typedef int (*dumpi_win_get_errhandler_call)(const dumpi_win_get_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI win_set_errhandler calls. */
  typedef int (*dumpi_win_set_errhandler_call)(const dumpi_win_set_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_open calls. */
  typedef int (*dumpi_file_open_call)(const dumpi_file_open *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_close calls. */
  typedef int (*dumpi_file_close_call)(const dumpi_file_close *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_delete calls. */
  typedef int (*dumpi_file_delete_call)(const dumpi_file_delete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_set_size calls. */
  typedef int (*dumpi_file_set_size_call)(const dumpi_file_set_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_preallocate calls. */
  typedef int (*dumpi_file_preallocate_call)(const dumpi_file_preallocate *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_size calls. */
  typedef int (*dumpi_file_get_size_call)(const dumpi_file_get_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_group calls. */
  typedef int (*dumpi_file_get_group_call)(const dumpi_file_get_group *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_amode calls. */
  typedef int (*dumpi_file_get_amode_call)(const dumpi_file_get_amode *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_set_info calls. */
  typedef int (*dumpi_file_set_info_call)(const dumpi_file_set_info *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_info calls. */
  typedef int (*dumpi_file_get_info_call)(const dumpi_file_get_info *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_set_view calls. */
  typedef int (*dumpi_file_set_view_call)(const dumpi_file_set_view *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_view calls. */
  typedef int (*dumpi_file_get_view_call)(const dumpi_file_get_view *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_at calls. */
  typedef int (*dumpi_file_read_at_call)(const dumpi_file_read_at *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_at_all calls. */
  typedef int (*dumpi_file_read_at_all_call)(const dumpi_file_read_at_all *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_at calls. */
  typedef int (*dumpi_file_write_at_call)(const dumpi_file_write_at *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_at_all calls. */
  typedef int (*dumpi_file_write_at_all_call)(const dumpi_file_write_at_all *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_iread_at calls. */
  typedef int (*dumpi_file_iread_at_call)(const dumpi_file_iread_at *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_iwrite_at calls. */
  typedef int (*dumpi_file_iwrite_at_call)(const dumpi_file_iwrite_at *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read calls. */
  typedef int (*dumpi_file_read_call)(const dumpi_file_read *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_all calls. */
  typedef int (*dumpi_file_read_all_call)(const dumpi_file_read_all *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write calls. */
  typedef int (*dumpi_file_write_call)(const dumpi_file_write *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_all calls. */
  typedef int (*dumpi_file_write_all_call)(const dumpi_file_write_all *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_iread calls. */
  typedef int (*dumpi_file_iread_call)(const dumpi_file_iread *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_iwrite calls. */
  typedef int (*dumpi_file_iwrite_call)(const dumpi_file_iwrite *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_seek calls. */
  typedef int (*dumpi_file_seek_call)(const dumpi_file_seek *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_position calls. */
  typedef int (*dumpi_file_get_position_call)(const dumpi_file_get_position *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_byte_offset calls. */
  typedef int (*dumpi_file_get_byte_offset_call)(const dumpi_file_get_byte_offset *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_shared calls. */
  typedef int (*dumpi_file_read_shared_call)(const dumpi_file_read_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_shared calls. */
  typedef int (*dumpi_file_write_shared_call)(const dumpi_file_write_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_iread_shared calls. */
  typedef int (*dumpi_file_iread_shared_call)(const dumpi_file_iread_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_iwrite_shared calls. */
  typedef int (*dumpi_file_iwrite_shared_call)(const dumpi_file_iwrite_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_ordered calls. */
  typedef int (*dumpi_file_read_ordered_call)(const dumpi_file_read_ordered *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_ordered calls. */
  typedef int (*dumpi_file_write_ordered_call)(const dumpi_file_write_ordered *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_seek_shared calls. */
  typedef int (*dumpi_file_seek_shared_call)(const dumpi_file_seek_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_position_shared calls. */
  typedef int (*dumpi_file_get_position_shared_call)(const dumpi_file_get_position_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_at_all_begin calls. */
  typedef int (*dumpi_file_read_at_all_begin_call)(const dumpi_file_read_at_all_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_at_all_end calls. */
  typedef int (*dumpi_file_read_at_all_end_call)(const dumpi_file_read_at_all_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_at_all_begin calls. */
  typedef int (*dumpi_file_write_at_all_begin_call)(const dumpi_file_write_at_all_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_at_all_end calls. */
  typedef int (*dumpi_file_write_at_all_end_call)(const dumpi_file_write_at_all_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_all_begin calls. */
  typedef int (*dumpi_file_read_all_begin_call)(const dumpi_file_read_all_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_all_end calls. */
  typedef int (*dumpi_file_read_all_end_call)(const dumpi_file_read_all_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_all_begin calls. */
  typedef int (*dumpi_file_write_all_begin_call)(const dumpi_file_write_all_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_all_end calls. */
  typedef int (*dumpi_file_write_all_end_call)(const dumpi_file_write_all_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_ordered_begin calls. */
  typedef int (*dumpi_file_read_ordered_begin_call)(const dumpi_file_read_ordered_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_read_ordered_end calls. */
  typedef int (*dumpi_file_read_ordered_end_call)(const dumpi_file_read_ordered_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_ordered_begin calls. */
  typedef int (*dumpi_file_write_ordered_begin_call)(const dumpi_file_write_ordered_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_write_ordered_end calls. */
  typedef int (*dumpi_file_write_ordered_end_call)(const dumpi_file_write_ordered_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_type_extent calls. */
  typedef int (*dumpi_file_get_type_extent_call)(const dumpi_file_get_type_extent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI register_datarep calls. */
  typedef int (*dumpi_register_datarep_call)(const dumpi_register_datarep *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_set_atomicity calls. */
  typedef int (*dumpi_file_set_atomicity_call)(const dumpi_file_set_atomicity *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_get_atomicity calls. */
  typedef int (*dumpi_file_get_atomicity_call)(const dumpi_file_get_atomicity *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI file_sync calls. */
  typedef int (*dumpi_file_sync_call)(const dumpi_file_sync *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpio_test_call)(const dumpio_test *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpio_wait_call)(const dumpio_wait *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpio_testall_call)(const dumpio_testall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpio_waitall_call)(const dumpio_waitall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpio_testany_call)(const dumpio_testany *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpio_waitany_call)(const dumpio_waitany *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpio_waitsome_call)(const dumpio_waitsome *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  typedef int (*dumpio_testsome_call)(const dumpio_testsome *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI function_enter calls. */
  typedef int (*dumpi_function_enter_call)(const dumpi_func_call *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined callback to handle MPI function_exit calls. */
  typedef int (*dumpi_function_exit_call)(const dumpi_func_call *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);
  /** Signature for user-defined generic callback for MPI function calls. */
  typedef int (*dumpi_generic_call)(const void *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg);

  /**
   * Data structure used to specify callback routines for calls profiled with
   * DUMPI.  Any callback left undefined (as NULL) will be passed over during 
   * parsing.
   *
   * All the callback routines have similar function signatures, differing
   * only by the nature of the first argument.
   */
  typedef struct libundumpi_callbacks {
    /* handling function entry and exit */
    /*
    dumpi_func_enter_call on_func_enter;
    dumpi_func_exit_call  on_func_exit;
    */
    /* handling individual MPI function calls */
    dumpi_send_call on_send;
    dumpi_recv_call on_recv;
    dumpi_get_count_call on_get_count;
    dumpi_bsend_call on_bsend;
    dumpi_ssend_call on_ssend;
    dumpi_rsend_call on_rsend;
    dumpi_buffer_attach_call on_buffer_attach;
    dumpi_buffer_detach_call on_buffer_detach;
    dumpi_isend_call on_isend;
    dumpi_ibsend_call on_ibsend;
    dumpi_issend_call on_issend;
    dumpi_irsend_call on_irsend;
    dumpi_irecv_call on_irecv;
    dumpi_wait_call on_wait;
    dumpi_test_call on_test;
    dumpi_request_free_call on_request_free;
    dumpi_waitany_call on_waitany;
    dumpi_testany_call on_testany;
    dumpi_waitall_call on_waitall;
    dumpi_testall_call on_testall;
    dumpi_waitsome_call on_waitsome;
    dumpi_testsome_call on_testsome;
    dumpi_iprobe_call on_iprobe;
    dumpi_probe_call on_probe;
    dumpi_cancel_call on_cancel;
    dumpi_test_cancelled_call on_test_cancelled;
    dumpi_send_init_call on_send_init;
    dumpi_bsend_init_call on_bsend_init;
    dumpi_ssend_init_call on_ssend_init;
    dumpi_rsend_init_call on_rsend_init;
    dumpi_recv_init_call on_recv_init;
    dumpi_start_call on_start;
    dumpi_startall_call on_startall;
    dumpi_sendrecv_call on_sendrecv;
    dumpi_sendrecv_replace_call on_sendrecv_replace;
    dumpi_type_contiguous_call on_type_contiguous;
    dumpi_type_vector_call on_type_vector;
    dumpi_type_hvector_call on_type_hvector;
    dumpi_type_indexed_call on_type_indexed;
    dumpi_type_hindexed_call on_type_hindexed;
    dumpi_type_struct_call on_type_struct;
    dumpi_address_call on_address;
    dumpi_type_extent_call on_type_extent;
    dumpi_type_size_call on_type_size;
    dumpi_type_lb_call on_type_lb;
    dumpi_type_ub_call on_type_ub;
    dumpi_type_commit_call on_type_commit;
    dumpi_type_free_call on_type_free;
    dumpi_get_elements_call on_get_elements;
    dumpi_pack_call on_pack;
    dumpi_unpack_call on_unpack;
    dumpi_pack_size_call on_pack_size;
    dumpi_barrier_call on_barrier;
    dumpi_bcast_call on_bcast;
    dumpi_gather_call on_gather;
    dumpi_gatherv_call on_gatherv;
    dumpi_scatter_call on_scatter;
    dumpi_scatterv_call on_scatterv;
    dumpi_allgather_call on_allgather;
    dumpi_allgatherv_call on_allgatherv;
    dumpi_alltoall_call on_alltoall;
    dumpi_alltoallv_call on_alltoallv;
    dumpi_reduce_call on_reduce;
    dumpi_op_create_call on_op_create;
    dumpi_op_free_call on_op_free;
    dumpi_allreduce_call on_allreduce;
    dumpi_reduce_scatter_call on_reduce_scatter;
    dumpi_scan_call on_scan;
    dumpi_group_size_call on_group_size;
    dumpi_group_rank_call on_group_rank;
    dumpi_group_translate_ranks_call on_group_translate_ranks;
    dumpi_group_compare_call on_group_compare;
    dumpi_comm_group_call on_comm_group;
    dumpi_group_union_call on_group_union;
    dumpi_group_intersection_call on_group_intersection;
    dumpi_group_difference_call on_group_difference;
    dumpi_group_incl_call on_group_incl;
    dumpi_group_excl_call on_group_excl;
    dumpi_group_range_incl_call on_group_range_incl;
    dumpi_group_range_excl_call on_group_range_excl;
    dumpi_group_free_call on_group_free;
    dumpi_comm_size_call on_comm_size;
    dumpi_comm_rank_call on_comm_rank;
    dumpi_comm_compare_call on_comm_compare;
    dumpi_comm_dup_call on_comm_dup;
    dumpi_comm_create_call on_comm_create;
    dumpi_comm_split_call on_comm_split;
    dumpi_comm_free_call on_comm_free;
    dumpi_comm_test_inter_call on_comm_test_inter;
    dumpi_comm_remote_size_call on_comm_remote_size;
    dumpi_comm_remote_group_call on_comm_remote_group;
    dumpi_intercomm_create_call on_intercomm_create;
    dumpi_intercomm_merge_call on_intercomm_merge;
    dumpi_keyval_create_call on_keyval_create;
    dumpi_keyval_free_call on_keyval_free;
    dumpi_attr_put_call on_attr_put;
    dumpi_attr_get_call on_attr_get;
    dumpi_attr_delete_call on_attr_delete;
    dumpi_topo_test_call on_topo_test;
    dumpi_cart_create_call on_cart_create;
    dumpi_dims_create_call on_dims_create;
    dumpi_graph_create_call on_graph_create;
    dumpi_graphdims_get_call on_graphdims_get;
    dumpi_graph_get_call on_graph_get;
    dumpi_cartdim_get_call on_cartdim_get;
    dumpi_cart_get_call on_cart_get;
    dumpi_cart_rank_call on_cart_rank;
    dumpi_cart_coords_call on_cart_coords;
    dumpi_graph_neighbors_count_call on_graph_neighbors_count;
    dumpi_graph_neighbors_call on_graph_neighbors;
    dumpi_cart_shift_call on_cart_shift;
    dumpi_cart_sub_call on_cart_sub;
    dumpi_cart_map_call on_cart_map;
    dumpi_graph_map_call on_graph_map;
    dumpi_get_processor_name_call on_get_processor_name;
    dumpi_get_version_call on_get_version;
    dumpi_errhandler_create_call on_errhandler_create;
    dumpi_errhandler_set_call on_errhandler_set;
    dumpi_errhandler_get_call on_errhandler_get;
    dumpi_errhandler_free_call on_errhandler_free;
    dumpi_error_string_call on_error_string;
    dumpi_error_class_call on_error_class;
    dumpi_wtime_call on_wtime;
    dumpi_wtick_call on_wtick;
    dumpi_init_call on_init;
    dumpi_finalize_call on_finalize;
    dumpi_initialized_call on_initialized;
    dumpi_abort_call on_abort;
    dumpi_close_port_call on_close_port;
    dumpi_comm_accept_call on_comm_accept;
    dumpi_comm_connect_call on_comm_connect;
    dumpi_comm_disconnect_call on_comm_disconnect;
    dumpi_comm_get_parent_call on_comm_get_parent;
    dumpi_comm_join_call on_comm_join;
    dumpi_comm_spawn_call on_comm_spawn;
    dumpi_comm_spawn_multiple_call on_comm_spawn_multiple;
    dumpi_lookup_name_call on_lookup_name;
    dumpi_open_port_call on_open_port;
    dumpi_publish_name_call on_publish_name;
    dumpi_unpublish_name_call on_unpublish_name;
    dumpi_accumulate_call on_accumulate;
    dumpi_get_call on_get;
    dumpi_put_call on_put;
    dumpi_win_complete_call on_win_complete;
    dumpi_win_create_call on_win_create;
    dumpi_win_fence_call on_win_fence;
    dumpi_win_free_call on_win_free;
    dumpi_win_get_group_call on_win_get_group;
    dumpi_win_lock_call on_win_lock;
    dumpi_win_post_call on_win_post;
    dumpi_win_start_call on_win_start;
    dumpi_win_test_call on_win_test;
    dumpi_win_unlock_call on_win_unlock;
    dumpi_win_wait_call on_win_wait;
    dumpi_alltoallw_call on_alltoallw;
    dumpi_exscan_call on_exscan;
    dumpi_add_error_class_call on_add_error_class;
    dumpi_add_error_code_call on_add_error_code;
    dumpi_add_error_string_call on_add_error_string;
    dumpi_comm_call_errhandler_call on_comm_call_errhandler;
    dumpi_comm_create_keyval_call on_comm_create_keyval;
    dumpi_comm_delete_attr_call on_comm_delete_attr;
    dumpi_comm_free_keyval_call on_comm_free_keyval;
    dumpi_comm_get_attr_call on_comm_get_attr;
    dumpi_comm_get_name_call on_comm_get_name;
    dumpi_comm_set_attr_call on_comm_set_attr;
    dumpi_comm_set_name_call on_comm_set_name;
    dumpi_file_call_errhandler_call on_file_call_errhandler;
    dumpi_grequest_complete_call on_grequest_complete;
    dumpi_grequest_start_call on_grequest_start;
    dumpi_init_thread_call on_init_thread;
    dumpi_is_thread_main_call on_is_thread_main;
    dumpi_query_thread_call on_query_thread;
    dumpi_status_set_cancelled_call on_status_set_cancelled;
    dumpi_status_set_elements_call on_status_set_elements;
    dumpi_type_create_keyval_call on_type_create_keyval;
    dumpi_type_delete_attr_call on_type_delete_attr;
    dumpi_type_dup_call on_type_dup;
    dumpi_type_free_keyval_call on_type_free_keyval;
    dumpi_type_get_attr_call on_type_get_attr;
    dumpi_type_get_contents_call on_type_get_contents;
    dumpi_type_get_envelope_call on_type_get_envelope;
    dumpi_type_get_name_call on_type_get_name;
    dumpi_type_set_attr_call on_type_set_attr;
    dumpi_type_set_name_call on_type_set_name;
    dumpi_type_match_size_call on_type_match_size;
    dumpi_win_call_errhandler_call on_win_call_errhandler;
    dumpi_win_create_keyval_call on_win_create_keyval;
    dumpi_win_delete_attr_call on_win_delete_attr;
    dumpi_win_free_keyval_call on_win_free_keyval;
    dumpi_win_get_attr_call on_win_get_attr;
    dumpi_win_get_name_call on_win_get_name;
    dumpi_win_set_attr_call on_win_set_attr;
    dumpi_win_set_name_call on_win_set_name;
    dumpi_alloc_mem_call on_alloc_mem;
    dumpi_comm_create_errhandler_call on_comm_create_errhandler;
    dumpi_comm_get_errhandler_call on_comm_get_errhandler;
    dumpi_comm_set_errhandler_call on_comm_set_errhandler;
    dumpi_file_create_errhandler_call on_file_create_errhandler;
    dumpi_file_get_errhandler_call on_file_get_errhandler;
    dumpi_file_set_errhandler_call on_file_set_errhandler;
    dumpi_finalized_call on_finalized;
    dumpi_free_mem_call on_free_mem;
    dumpi_get_address_call on_get_address;
    dumpi_info_create_call on_info_create;
    dumpi_info_delete_call on_info_delete;
    dumpi_info_dup_call on_info_dup;
    dumpi_info_free_call on_info_free;
    dumpi_info_get_call on_info_get;
    dumpi_info_get_nkeys_call on_info_get_nkeys;
    dumpi_info_get_nthkey_call on_info_get_nthkey;
    dumpi_info_get_valuelen_call on_info_get_valuelen;
    dumpi_info_set_call on_info_set;
    dumpi_pack_external_call on_pack_external;
    dumpi_pack_external_size_call on_pack_external_size;
    dumpi_request_get_status_call on_request_get_status;
    dumpi_type_create_darray_call on_type_create_darray;
    dumpi_type_create_hindexed_call on_type_create_hindexed;
    dumpi_type_create_hvector_call on_type_create_hvector;
    dumpi_type_create_indexed_block_call on_type_create_indexed_block;
    dumpi_type_create_resized_call on_type_create_resized;
    dumpi_type_create_struct_call on_type_create_struct;
    dumpi_type_create_subarray_call on_type_create_subarray;
    dumpi_type_get_extent_call on_type_get_extent;
    dumpi_type_get_true_extent_call on_type_get_true_extent;
    dumpi_unpack_external_call on_unpack_external;
    dumpi_win_create_errhandler_call on_win_create_errhandler;
    dumpi_win_get_errhandler_call on_win_get_errhandler;
    dumpi_win_set_errhandler_call on_win_set_errhandler;
    dumpi_file_open_call on_file_open;
    dumpi_file_close_call on_file_close;
    dumpi_file_delete_call on_file_delete;
    dumpi_file_set_size_call on_file_set_size;
    dumpi_file_preallocate_call on_file_preallocate;
    dumpi_file_get_size_call on_file_get_size;
    dumpi_file_get_group_call on_file_get_group;
    dumpi_file_get_amode_call on_file_get_amode;
    dumpi_file_set_info_call on_file_set_info;
    dumpi_file_get_info_call on_file_get_info;
    dumpi_file_set_view_call on_file_set_view;
    dumpi_file_get_view_call on_file_get_view;
    dumpi_file_read_at_call on_file_read_at;
    dumpi_file_read_at_all_call on_file_read_at_all;
    dumpi_file_write_at_call on_file_write_at;
    dumpi_file_write_at_all_call on_file_write_at_all;
    dumpi_file_iread_at_call on_file_iread_at;
    dumpi_file_iwrite_at_call on_file_iwrite_at;
    dumpi_file_read_call on_file_read;
    dumpi_file_read_all_call on_file_read_all;
    dumpi_file_write_call on_file_write;
    dumpi_file_write_all_call on_file_write_all;
    dumpi_file_iread_call on_file_iread;
    dumpi_file_iwrite_call on_file_iwrite;
    dumpi_file_seek_call on_file_seek;
    dumpi_file_get_position_call on_file_get_position;
    dumpi_file_get_byte_offset_call on_file_get_byte_offset;
    dumpi_file_read_shared_call on_file_read_shared;
    dumpi_file_write_shared_call on_file_write_shared;
    dumpi_file_iread_shared_call on_file_iread_shared;
    dumpi_file_iwrite_shared_call on_file_iwrite_shared;
    dumpi_file_read_ordered_call on_file_read_ordered;
    dumpi_file_write_ordered_call on_file_write_ordered;
    dumpi_file_seek_shared_call on_file_seek_shared;
    dumpi_file_get_position_shared_call on_file_get_position_shared;
    dumpi_file_read_at_all_begin_call on_file_read_at_all_begin;
    dumpi_file_read_at_all_end_call on_file_read_at_all_end;
    dumpi_file_write_at_all_begin_call on_file_write_at_all_begin;
    dumpi_file_write_at_all_end_call on_file_write_at_all_end;
    dumpi_file_read_all_begin_call on_file_read_all_begin;
    dumpi_file_read_all_end_call on_file_read_all_end;
    dumpi_file_write_all_begin_call on_file_write_all_begin;
    dumpi_file_write_all_end_call on_file_write_all_end;
    dumpi_file_read_ordered_begin_call on_file_read_ordered_begin;
    dumpi_file_read_ordered_end_call on_file_read_ordered_end;
    dumpi_file_write_ordered_begin_call on_file_write_ordered_begin;
    dumpi_file_write_ordered_end_call on_file_write_ordered_end;
    dumpi_file_get_type_extent_call on_file_get_type_extent;
    dumpi_register_datarep_call on_register_datarep;
    dumpi_file_set_atomicity_call on_file_set_atomicity;
    dumpi_file_get_atomicity_call on_file_get_atomicity;
    dumpi_file_sync_call on_file_sync;
    dumpio_test_call on_iotest;
    dumpio_wait_call on_iowait;
    dumpio_testall_call on_iotestall;
    dumpio_waitall_call on_iowaitall;
    dumpio_testany_call on_iotestany;
    dumpio_waitany_call on_iowaitany;
    dumpio_waitsome_call on_iowaitsome;
    dumpio_testsome_call on_iotestsome;
    dumpi_function_enter_call on_function_enter;
    dumpi_function_exit_call  on_function_exit;
  } libundumpi_callbacks;

  /** Clear a callback struct (set all callbacks to NULL) */
  void libundumpi_clear_callbacks(libundumpi_callbacks *callbacks);
  
  /**
   * Initialize all callbacks to the given function.
   * Note that this mechanism does not give any indication which MPI function
   * was actually called, so it is best suited for simple statistics gathering
   * (timing or performance counters) or if unhandled callbacks are to be
   * handled as errors.
   */
  void libundumpi_set_callbacks(libundumpi_callbacks *callbacks,
                                dumpi_generic_call handler);

  /*@}*/ /* Close the scope of the doxygen comment attribution */

#ifdef __cplusplus
} /* End of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBUNDUMPI_CALLBACKS_H */