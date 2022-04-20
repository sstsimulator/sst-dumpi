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

#include <dumpi/libundumpi/callbacks.h>
#include <string.h>
#include <assert.h>

void libundumpi_clear_callbacks(libundumpi_callbacks *callbacks) {
  assert(callbacks != NULL);
  memset(callbacks, 0, sizeof(libundumpi_callbacks));
}

/*
 * Initialize all callbacks to the given function.
 */
void libundumpi_set_callbacks(libundumpi_callbacks *cb, dumpi_generic_call func)
{
  libundumpi_clear_callbacks(cb);
  cb->on_send = (dumpi_send_call)func;
  cb->on_recv = (dumpi_recv_call)func;
  cb->on_get_count = (dumpi_get_count_call)func;
  cb->on_bsend = (dumpi_bsend_call)func;
  cb->on_ssend = (dumpi_ssend_call)func;
  cb->on_rsend = (dumpi_rsend_call)func;
  cb->on_buffer_attach = (dumpi_buffer_attach_call)func;
  cb->on_buffer_detach = (dumpi_buffer_detach_call)func;
  cb->on_isend = (dumpi_isend_call)func;
  cb->on_ibsend = (dumpi_ibsend_call)func;
  cb->on_issend = (dumpi_issend_call)func;
  cb->on_irsend = (dumpi_irsend_call)func;
  cb->on_irecv = (dumpi_irecv_call)func;
  cb->on_wait = (dumpi_wait_call)func;
  cb->on_test = (dumpi_test_call)func;
  cb->on_request_free = (dumpi_request_free_call)func;
  cb->on_waitany = (dumpi_waitany_call)func;
  cb->on_testany = (dumpi_testany_call)func;
  cb->on_waitall = (dumpi_waitall_call)func;
  cb->on_testall = (dumpi_testall_call)func;
  cb->on_waitsome = (dumpi_waitsome_call)func;
  cb->on_testsome = (dumpi_testsome_call)func;
  cb->on_iprobe = (dumpi_iprobe_call)func;
  cb->on_probe = (dumpi_probe_call)func;
  cb->on_cancel = (dumpi_cancel_call)func;
  cb->on_test_cancelled = (dumpi_test_cancelled_call)func;
  cb->on_send_init = (dumpi_send_init_call)func;
  cb->on_bsend_init = (dumpi_bsend_init_call)func;
  cb->on_ssend_init = (dumpi_ssend_init_call)func;
  cb->on_rsend_init = (dumpi_rsend_init_call)func;
  cb->on_recv_init = (dumpi_recv_init_call)func;
  cb->on_start = (dumpi_start_call)func;
  cb->on_startall = (dumpi_startall_call)func;
  cb->on_sendrecv = (dumpi_sendrecv_call)func;
  cb->on_sendrecv_replace = (dumpi_sendrecv_replace_call)func;
  cb->on_type_contiguous = (dumpi_type_contiguous_call)func;
  cb->on_type_vector = (dumpi_type_vector_call)func;
  cb->on_type_hvector = (dumpi_type_hvector_call)func;
  cb->on_type_indexed = (dumpi_type_indexed_call)func;
  cb->on_type_hindexed = (dumpi_type_hindexed_call)func;
  cb->on_type_struct = (dumpi_type_struct_call)func;
  cb->on_address = (dumpi_address_call)func;
  cb->on_type_extent = (dumpi_type_extent_call)func;
  cb->on_type_size = (dumpi_type_size_call)func;
  cb->on_type_lb = (dumpi_type_lb_call)func;
  cb->on_type_ub = (dumpi_type_ub_call)func;
  cb->on_type_commit = (dumpi_type_commit_call)func;
  cb->on_type_free = (dumpi_type_free_call)func;
  cb->on_get_elements = (dumpi_get_elements_call)func;
  cb->on_pack = (dumpi_pack_call)func;
  cb->on_unpack = (dumpi_unpack_call)func;
  cb->on_pack_size = (dumpi_pack_size_call)func;
  cb->on_barrier = (dumpi_barrier_call)func;
  cb->on_bcast = (dumpi_bcast_call)func;
  cb->on_gather = (dumpi_gather_call)func;
  cb->on_gatherv = (dumpi_gatherv_call)func;
  cb->on_scatter = (dumpi_scatter_call)func;
  cb->on_scatterv = (dumpi_scatterv_call)func;
  cb->on_allgather = (dumpi_allgather_call)func;
  cb->on_allgatherv = (dumpi_allgatherv_call)func;
  cb->on_alltoall = (dumpi_alltoall_call)func;
  cb->on_alltoallv = (dumpi_alltoallv_call)func;
  cb->on_reduce = (dumpi_reduce_call)func;
  cb->on_op_create = (dumpi_op_create_call)func;
  cb->on_op_free = (dumpi_op_free_call)func;
  cb->on_allreduce = (dumpi_allreduce_call)func;
  cb->on_reduce_scatter = (dumpi_reduce_scatter_call)func;
  cb->on_scan = (dumpi_scan_call)func;
  cb->on_group_size = (dumpi_group_size_call)func;
  cb->on_group_rank = (dumpi_group_rank_call)func;
  cb->on_group_translate_ranks = (dumpi_group_translate_ranks_call)func;
  cb->on_group_compare = (dumpi_group_compare_call)func;
  cb->on_comm_group = (dumpi_comm_group_call)func;
  cb->on_group_union = (dumpi_group_union_call)func;
  cb->on_group_intersection = (dumpi_group_intersection_call)func;
  cb->on_group_difference = (dumpi_group_difference_call)func;
  cb->on_group_incl = (dumpi_group_incl_call)func;
  cb->on_group_excl = (dumpi_group_excl_call)func;
  cb->on_group_range_incl = (dumpi_group_range_incl_call)func;
  cb->on_group_range_excl = (dumpi_group_range_excl_call)func;
  cb->on_group_free = (dumpi_group_free_call)func;
  cb->on_comm_size = (dumpi_comm_size_call)func;
  cb->on_comm_rank = (dumpi_comm_rank_call)func;
  cb->on_comm_compare = (dumpi_comm_compare_call)func;
  cb->on_comm_dup = (dumpi_comm_dup_call)func;
  cb->on_comm_create = (dumpi_comm_create_call)func;
  cb->on_comm_split = (dumpi_comm_split_call)func;
  cb->on_comm_free = (dumpi_comm_free_call)func;
  cb->on_comm_test_inter = (dumpi_comm_test_inter_call)func;
  cb->on_comm_remote_size = (dumpi_comm_remote_size_call)func;
  cb->on_comm_remote_group = (dumpi_comm_remote_group_call)func;
  cb->on_intercomm_create = (dumpi_intercomm_create_call)func;
  cb->on_intercomm_merge = (dumpi_intercomm_merge_call)func;
  cb->on_keyval_create = (dumpi_keyval_create_call)func;
  cb->on_keyval_free = (dumpi_keyval_free_call)func;
  cb->on_attr_put = (dumpi_attr_put_call)func;
  cb->on_attr_get = (dumpi_attr_get_call)func;
  cb->on_attr_delete = (dumpi_attr_delete_call)func;
  cb->on_topo_test = (dumpi_topo_test_call)func;
  cb->on_cart_create = (dumpi_cart_create_call)func;
  cb->on_dims_create = (dumpi_dims_create_call)func;
  cb->on_graph_create = (dumpi_graph_create_call)func;
  cb->on_graphdims_get = (dumpi_graphdims_get_call)func;
  cb->on_graph_get = (dumpi_graph_get_call)func;
  cb->on_cartdim_get = (dumpi_cartdim_get_call)func;
  cb->on_cart_get = (dumpi_cart_get_call)func;
  cb->on_cart_rank = (dumpi_cart_rank_call)func;
  cb->on_cart_coords = (dumpi_cart_coords_call)func;
  cb->on_graph_neighbors_count = (dumpi_graph_neighbors_count_call)func;
  cb->on_graph_neighbors = (dumpi_graph_neighbors_call)func;
  cb->on_cart_shift = (dumpi_cart_shift_call)func;
  cb->on_cart_sub = (dumpi_cart_sub_call)func;
  cb->on_cart_map = (dumpi_cart_map_call)func;
  cb->on_graph_map = (dumpi_graph_map_call)func;
  cb->on_get_processor_name = (dumpi_get_processor_name_call)func;
  cb->on_get_version = (dumpi_get_version_call)func;
  cb->on_errhandler_create = (dumpi_errhandler_create_call)func;
  cb->on_errhandler_set = (dumpi_errhandler_set_call)func;
  cb->on_errhandler_get = (dumpi_errhandler_get_call)func;
  cb->on_errhandler_free = (dumpi_errhandler_free_call)func;
  cb->on_error_string = (dumpi_error_string_call)func;
  cb->on_error_class = (dumpi_error_class_call)func;
  cb->on_wtime = (dumpi_wtime_call)func;
  cb->on_wtick = (dumpi_wtick_call)func;
  cb->on_init = (dumpi_init_call)func;
  cb->on_finalize = (dumpi_finalize_call)func;
  cb->on_initialized = (dumpi_initialized_call)func;
  cb->on_abort = (dumpi_abort_call)func;
  cb->on_close_port = (dumpi_close_port_call)func;
  cb->on_comm_accept = (dumpi_comm_accept_call)func;
  cb->on_comm_connect = (dumpi_comm_connect_call)func;
  cb->on_comm_disconnect = (dumpi_comm_disconnect_call)func;
  cb->on_comm_get_parent = (dumpi_comm_get_parent_call)func;
  cb->on_comm_join = (dumpi_comm_join_call)func;
  cb->on_comm_spawn = (dumpi_comm_spawn_call)func;
  cb->on_comm_spawn_multiple = (dumpi_comm_spawn_multiple_call)func;
  cb->on_lookup_name = (dumpi_lookup_name_call)func;
  cb->on_open_port = (dumpi_open_port_call)func;
  cb->on_publish_name = (dumpi_publish_name_call)func;
  cb->on_unpublish_name = (dumpi_unpublish_name_call)func;
  cb->on_accumulate = (dumpi_accumulate_call)func;
  cb->on_get = (dumpi_get_call)func;
  cb->on_put = (dumpi_put_call)func;
  cb->on_win_complete = (dumpi_win_complete_call)func;
  cb->on_win_create = (dumpi_win_create_call)func;
  cb->on_win_fence = (dumpi_win_fence_call)func;
  cb->on_win_free = (dumpi_win_free_call)func;
  cb->on_win_get_group = (dumpi_win_get_group_call)func;
  cb->on_win_lock = (dumpi_win_lock_call)func;
  cb->on_win_post = (dumpi_win_post_call)func;
  cb->on_win_start = (dumpi_win_start_call)func;
  cb->on_win_test = (dumpi_win_test_call)func;
  cb->on_win_unlock = (dumpi_win_unlock_call)func;
  cb->on_win_wait = (dumpi_win_wait_call)func;
  cb->on_alltoallw = (dumpi_alltoallw_call)func;
  cb->on_exscan = (dumpi_exscan_call)func;
  cb->on_add_error_class = (dumpi_add_error_class_call)func;
  cb->on_add_error_code = (dumpi_add_error_code_call)func;
  cb->on_add_error_string = (dumpi_add_error_string_call)func;
  cb->on_comm_call_errhandler = (dumpi_comm_call_errhandler_call)func;
  cb->on_comm_create_keyval = (dumpi_comm_create_keyval_call)func;
  cb->on_comm_delete_attr = (dumpi_comm_delete_attr_call)func;
  cb->on_comm_free_keyval = (dumpi_comm_free_keyval_call)func;
  cb->on_comm_get_attr = (dumpi_comm_get_attr_call)func;
  cb->on_comm_get_name = (dumpi_comm_get_name_call)func;
  cb->on_comm_set_attr = (dumpi_comm_set_attr_call)func;
  cb->on_comm_set_name = (dumpi_comm_set_name_call)func;
  cb->on_file_call_errhandler = (dumpi_file_call_errhandler_call)func;
  cb->on_grequest_complete = (dumpi_grequest_complete_call)func;
  cb->on_grequest_start = (dumpi_grequest_start_call)func;
  cb->on_init_thread = (dumpi_init_thread_call)func;
  cb->on_is_thread_main = (dumpi_is_thread_main_call)func;
  cb->on_query_thread = (dumpi_query_thread_call)func;
  cb->on_status_set_cancelled = (dumpi_status_set_cancelled_call)func;
  cb->on_status_set_elements = (dumpi_status_set_elements_call)func;
  cb->on_type_create_keyval = (dumpi_type_create_keyval_call)func;
  cb->on_type_delete_attr = (dumpi_type_delete_attr_call)func;
  cb->on_type_dup = (dumpi_type_dup_call)func;
  cb->on_type_free_keyval = (dumpi_type_free_keyval_call)func;
  cb->on_type_get_attr = (dumpi_type_get_attr_call)func;
  cb->on_type_get_contents = (dumpi_type_get_contents_call)func;
  cb->on_type_get_envelope = (dumpi_type_get_envelope_call)func;
  cb->on_type_get_name = (dumpi_type_get_name_call)func;
  cb->on_type_set_attr = (dumpi_type_set_attr_call)func;
  cb->on_type_set_name = (dumpi_type_set_name_call)func;
  cb->on_type_match_size = (dumpi_type_match_size_call)func;
  cb->on_win_call_errhandler = (dumpi_win_call_errhandler_call)func;
  cb->on_win_create_keyval = (dumpi_win_create_keyval_call)func;
  cb->on_win_delete_attr = (dumpi_win_delete_attr_call)func;
  cb->on_win_free_keyval = (dumpi_win_free_keyval_call)func;
  cb->on_win_get_attr = (dumpi_win_get_attr_call)func;
  cb->on_win_get_name = (dumpi_win_get_name_call)func;
  cb->on_win_set_attr = (dumpi_win_set_attr_call)func;
  cb->on_win_set_name = (dumpi_win_set_name_call)func;
  cb->on_alloc_mem = (dumpi_alloc_mem_call)func;
  cb->on_comm_create_errhandler = (dumpi_comm_create_errhandler_call)func;
  cb->on_comm_get_errhandler = (dumpi_comm_get_errhandler_call)func;
  cb->on_comm_set_errhandler = (dumpi_comm_set_errhandler_call)func;
  cb->on_file_create_errhandler = (dumpi_file_create_errhandler_call)func;
  cb->on_file_get_errhandler = (dumpi_file_get_errhandler_call)func;
  cb->on_file_set_errhandler = (dumpi_file_set_errhandler_call)func;
  cb->on_finalized = (dumpi_finalized_call)func;
  cb->on_free_mem = (dumpi_free_mem_call)func;
  cb->on_get_address = (dumpi_get_address_call)func;
  cb->on_info_create = (dumpi_info_create_call)func;
  cb->on_info_delete = (dumpi_info_delete_call)func;
  cb->on_info_dup = (dumpi_info_dup_call)func;
  cb->on_info_free = (dumpi_info_free_call)func;
  cb->on_info_get = (dumpi_info_get_call)func;
  cb->on_info_get_nkeys = (dumpi_info_get_nkeys_call)func;
  cb->on_info_get_nthkey = (dumpi_info_get_nthkey_call)func;
  cb->on_info_get_valuelen = (dumpi_info_get_valuelen_call)func;
  cb->on_info_set = (dumpi_info_set_call)func;
  cb->on_pack_external = (dumpi_pack_external_call)func;
  cb->on_pack_external_size = (dumpi_pack_external_size_call)func;
  cb->on_request_get_status = (dumpi_request_get_status_call)func;
  cb->on_type_create_darray = (dumpi_type_create_darray_call)func;
  cb->on_type_create_hindexed = (dumpi_type_create_hindexed_call)func;
  cb->on_type_create_hvector = (dumpi_type_create_hvector_call)func;
  cb->on_type_create_indexed_block = (dumpi_type_create_indexed_block_call)func;
  cb->on_type_create_resized = (dumpi_type_create_resized_call)func;
  cb->on_type_create_struct = (dumpi_type_create_struct_call)func;
  cb->on_type_create_subarray = (dumpi_type_create_subarray_call)func;
  cb->on_type_get_extent = (dumpi_type_get_extent_call)func;
  cb->on_type_get_true_extent = (dumpi_type_get_true_extent_call)func;
  cb->on_unpack_external = (dumpi_unpack_external_call)func;
  cb->on_win_create_errhandler = (dumpi_win_create_errhandler_call)func;
  cb->on_win_get_errhandler = (dumpi_win_get_errhandler_call)func;
  cb->on_win_set_errhandler = (dumpi_win_set_errhandler_call)func;
  cb->on_file_open = (dumpi_file_open_call)func;
  cb->on_file_close = (dumpi_file_close_call)func;
  cb->on_file_delete = (dumpi_file_delete_call)func;
  cb->on_file_set_size = (dumpi_file_set_size_call)func;
  cb->on_file_preallocate = (dumpi_file_preallocate_call)func;
  cb->on_file_get_size = (dumpi_file_get_size_call)func;
  cb->on_file_get_group = (dumpi_file_get_group_call)func;
  cb->on_file_get_amode = (dumpi_file_get_amode_call)func;
  cb->on_file_set_info = (dumpi_file_set_info_call)func;
  cb->on_file_get_info = (dumpi_file_get_info_call)func;
  cb->on_file_set_view = (dumpi_file_set_view_call)func;
  cb->on_file_get_view = (dumpi_file_get_view_call)func;
  cb->on_file_read_at = (dumpi_file_read_at_call)func;
  cb->on_file_read_at_all = (dumpi_file_read_at_all_call)func;
  cb->on_file_write_at = (dumpi_file_write_at_call)func;
  cb->on_file_write_at_all = (dumpi_file_write_at_all_call)func;
  cb->on_file_iread_at = (dumpi_file_iread_at_call)func;
  cb->on_file_iwrite_at = (dumpi_file_iwrite_at_call)func;
  cb->on_file_read = (dumpi_file_read_call)func;
  cb->on_file_read_all = (dumpi_file_read_all_call)func;
  cb->on_file_write = (dumpi_file_write_call)func;
  cb->on_file_write_all = (dumpi_file_write_all_call)func;
  cb->on_file_iread = (dumpi_file_iread_call)func;
  cb->on_file_iwrite = (dumpi_file_iwrite_call)func;
  cb->on_file_seek = (dumpi_file_seek_call)func;
  cb->on_file_get_position = (dumpi_file_get_position_call)func;
  cb->on_file_get_byte_offset = (dumpi_file_get_byte_offset_call)func;
  cb->on_file_read_shared = (dumpi_file_read_shared_call)func;
  cb->on_file_write_shared = (dumpi_file_write_shared_call)func;
  cb->on_file_iread_shared = (dumpi_file_iread_shared_call)func;
  cb->on_file_iwrite_shared = (dumpi_file_iwrite_shared_call)func;
  cb->on_file_read_ordered = (dumpi_file_read_ordered_call)func;
  cb->on_file_write_ordered = (dumpi_file_write_ordered_call)func;
  cb->on_file_seek_shared = (dumpi_file_seek_shared_call)func;
  cb->on_file_get_position_shared = (dumpi_file_get_position_shared_call)func;
  cb->on_file_read_at_all_begin = (dumpi_file_read_at_all_begin_call)func;
  cb->on_file_read_at_all_end = (dumpi_file_read_at_all_end_call)func;
  cb->on_file_write_at_all_begin = (dumpi_file_write_at_all_begin_call)func;
  cb->on_file_write_at_all_end = (dumpi_file_write_at_all_end_call)func;
  cb->on_file_read_all_begin = (dumpi_file_read_all_begin_call)func;
  cb->on_file_read_all_end = (dumpi_file_read_all_end_call)func;
  cb->on_file_write_all_begin = (dumpi_file_write_all_begin_call)func;
  cb->on_file_write_all_end = (dumpi_file_write_all_end_call)func;
  cb->on_file_read_ordered_begin = (dumpi_file_read_ordered_begin_call)func;
  cb->on_file_read_ordered_end = (dumpi_file_read_ordered_end_call)func;
  cb->on_file_write_ordered_begin = (dumpi_file_write_ordered_begin_call)func;
  cb->on_file_write_ordered_end = (dumpi_file_write_ordered_end_call)func;
  cb->on_file_get_type_extent = (dumpi_file_get_type_extent_call)func;
  cb->on_register_datarep = (dumpi_register_datarep_call)func;
  cb->on_file_set_atomicity = (dumpi_file_set_atomicity_call)func;
  cb->on_file_get_atomicity = (dumpi_file_get_atomicity_call)func;
  cb->on_file_sync = (dumpi_file_sync_call)func;
  cb->on_iotest = (dumpio_test_call)func;
  cb->on_iowait = (dumpio_wait_call)func;
  cb->on_iotestall = (dumpio_testall_call)func;
  cb->on_iowaitall = (dumpio_waitall_call)func;
  cb->on_iotestany = (dumpio_testany_call)func;
  cb->on_iowaitany = (dumpio_waitany_call)func;
  cb->on_iowaitsome = (dumpio_waitsome_call)func;
  cb->on_iotestsome = (dumpio_testsome_call)func;
  cb->on_function_enter = (dumpi_function_enter_call)func;
  cb-> on_function_exit = (dumpi_function_exit_call)func;
}