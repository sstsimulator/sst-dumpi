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

#include <dumpi/bin/dumpi2dumpi.h>
#include <dumpi/common/dumpiio.h>
#include <dumpi/libundumpi/libundumpi.h>
#include <dumpi/common/funcs.h>
#include <dumpi/common/io.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

/*
int handle_send(const dumpi_send *prm, uint16_t thread, const dumpi_time *cpu,
		const dumpi_time *wall, const dumpi_perfinfo *perf,
		void *userarg)
{
  const d2dopts *opts = (const d2dopts*)userarg;
  if(opts->output.function[DUMPI_Send])
    dumpi_write_send(prm, thread, cpu, wall, perf,
		     &opts->output, opts->oprofile);
  return 1;
}

int handle_recv (const dumpi_recv *prm, uint16_t thread, const dumpi_time *cpu,
             const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg)
{
  const d2dopts *opts = (const d2dopts*)userarg;
  assert(DUMPI_Recv >= 0 && DUMPI_Recv > DUMPI_END_OF_STREAM);
  if(opts->output.function[DUMPI_Recv])
    dumpi_write_recv (prm, thread, cpu, wall, perf, &opts->output, opts->oprofile);
  return 1;
}
*/

#define DUMPI_HANDLER(FUNC, GUARD)						\
  static								\
  int handle_ ## FUNC (const dumpi_ ## FUNC *prm, uint16_t thread,	\
		   const dumpi_time *cpu, const dumpi_time *wall,	\
		   const dumpi_perfinfo *perf, void *userarg)		\
  {									\
    d2dopts *opts = (d2dopts*)userarg;					\
    assert(GUARD >= 0 && GUARD < DUMPI_END_OF_STREAM);			\
    opts->footer.call_count[GUARD]++;					\
    if(opts->oprofile == NULL) {					\
      opts->oprofile =							\
	dumpi_alloc_output_profile(cpu->start.sec, wall->start.sec, 0);	\
      opts->oprofile->file = dumpi_open_output_file(opts->outname);	\
    }									\
    if(opts->output.function[GUARD]) {					\
      dumpi_write_ ## FUNC (prm, thread, cpu, wall, perf,		\
			    &opts->output, opts->oprofile);		\
    }									\
    else {								\
      opts->footer.ignored_count[GUARD]++;				\
    }									\
    return 1;								\
  }

#define DUMPI_HANDLERIO(FUNC, GUARD)						\
  static								\
  int handle_io ## FUNC (const dumpio_ ## FUNC *prm, uint16_t thread,	\
			 const dumpi_time *cpu, const dumpi_time *wall,	\
			 const dumpi_perfinfo *perf, void *userarg)	\
  {									\
    d2dopts *opts = (d2dopts*)userarg;					\
    assert(GUARD >= 0 && GUARD > DUMPI_END_OF_STREAM);			\
    opts->footer.call_count[GUARD]++;					\
    if(opts->oprofile == NULL) {					\
      opts->oprofile =							\
	dumpi_alloc_output_profile(cpu->start.sec, wall->start.sec, 0);	\
      opts->oprofile->file = dumpi_open_output_file(opts->outname);	\
    }									\
    if(opts->output.function[GUARD]) {					\
      dumpio_write_ ## FUNC (prm, thread, cpu, wall, perf,		\
			     &opts->output, opts->oprofile);		\
    }									\
    else {								\
      opts->footer.ignored_count[GUARD]++;				\
    }									\
    return 1;								\
  }

#define DUMPI_HANDLERF(LFUNC, FUNC, GUARD)					\
  static								\
  int handle_ ## LFUNC (const dumpi_func_call *prm, uint16_t thread,	\
		       const dumpi_time *cpu, const dumpi_time *wall,	\
		       const dumpi_perfinfo *perf, void *userarg)	\
  {									\
    d2dopts *opts = (d2dopts*)userarg;					\
    assert(GUARD >= 0 && GUARD > DUMPI_END_OF_STREAM);			\
    opts->footer.call_count[GUARD]++;					\
    if(opts->oprofile == NULL) {					\
      opts->oprofile =							\
	dumpi_alloc_output_profile(cpu->start.sec, wall->start.sec, 0);	\
      opts->oprofile->file = dumpi_open_output_file(opts->outname);	\
    }									\
    if(opts->output.function[GUARD]) {					\
      dumpi_write_ ## FUNC (prm, thread, cpu, wall, perf,		\
			    &opts->output, opts->oprofile);		\
    }									\
    else {								\
      opts->footer.ignored_count[GUARD]++;				\
    }									\
    return 1;								\
  }

DUMPI_HANDLER(send, DUMPI_Send)
DUMPI_HANDLER(recv, DUMPI_Recv)
DUMPI_HANDLER(get_count, DUMPI_Get_count)
DUMPI_HANDLER(bsend, DUMPI_Bsend)
DUMPI_HANDLER(ssend, DUMPI_Ssend)
DUMPI_HANDLER(rsend, DUMPI_Rsend)
DUMPI_HANDLER(buffer_attach, DUMPI_Buffer_attach)
DUMPI_HANDLER(buffer_detach, DUMPI_Buffer_detach)
DUMPI_HANDLER(isend, DUMPI_Isend)
DUMPI_HANDLER(ibsend, DUMPI_Ibsend)
DUMPI_HANDLER(issend, DUMPI_Issend)
DUMPI_HANDLER(irsend, DUMPI_Irsend)
DUMPI_HANDLER(irecv, DUMPI_Irecv)
DUMPI_HANDLER(wait, DUMPI_Wait)
DUMPI_HANDLER(test, DUMPI_Test)
DUMPI_HANDLER(request_free, DUMPI_Request_free)
DUMPI_HANDLER(waitany, DUMPI_Waitany)
DUMPI_HANDLER(testany, DUMPI_Testany)
DUMPI_HANDLER(waitall, DUMPI_Waitall)
DUMPI_HANDLER(testall, DUMPI_Testall)
DUMPI_HANDLER(waitsome, DUMPI_Waitsome)
DUMPI_HANDLER(testsome, DUMPI_Testsome)
DUMPI_HANDLER(iprobe, DUMPI_Iprobe)
DUMPI_HANDLER(probe, DUMPI_Probe)
DUMPI_HANDLER(cancel, DUMPI_Cancel)
DUMPI_HANDLER(test_cancelled, DUMPI_Test_cancelled)
DUMPI_HANDLER(send_init, DUMPI_Send_init)
DUMPI_HANDLER(bsend_init, DUMPI_Bsend_init)
DUMPI_HANDLER(ssend_init, DUMPI_Ssend_init)
DUMPI_HANDLER(rsend_init, DUMPI_Rsend_init)
DUMPI_HANDLER(recv_init, DUMPI_Recv_init)
DUMPI_HANDLER(start, DUMPI_Start)
DUMPI_HANDLER(startall, DUMPI_Startall)
DUMPI_HANDLER(sendrecv, DUMPI_Sendrecv)
DUMPI_HANDLER(sendrecv_replace, DUMPI_Sendrecv_replace)
DUMPI_HANDLER(type_contiguous, DUMPI_Type_contiguous)
DUMPI_HANDLER(type_vector, DUMPI_Type_vector)
DUMPI_HANDLER(type_hvector, DUMPI_Type_hvector)
DUMPI_HANDLER(type_indexed, DUMPI_Type_indexed)
DUMPI_HANDLER(type_hindexed, DUMPI_Type_hindexed)
DUMPI_HANDLER(type_struct, DUMPI_Type_struct)
DUMPI_HANDLER(address, DUMPI_Address)
DUMPI_HANDLER(type_extent, DUMPI_Type_extent)
DUMPI_HANDLER(type_size, DUMPI_Type_size)
DUMPI_HANDLER(type_lb, DUMPI_Type_lb)
DUMPI_HANDLER(type_ub, DUMPI_Type_ub)
DUMPI_HANDLER(type_commit, DUMPI_Type_commit)
DUMPI_HANDLER(type_free, DUMPI_Type_free)
DUMPI_HANDLER(get_elements, DUMPI_Get_elements)
DUMPI_HANDLER(pack, DUMPI_Pack)
DUMPI_HANDLER(unpack, DUMPI_Unpack)
DUMPI_HANDLER(pack_size, DUMPI_Pack_size)
DUMPI_HANDLER(barrier, DUMPI_Barrier)
DUMPI_HANDLER(bcast, DUMPI_Bcast)
DUMPI_HANDLER(gather, DUMPI_Gather)
DUMPI_HANDLER(gatherv, DUMPI_Gatherv)
DUMPI_HANDLER(scatter, DUMPI_Scatter)
DUMPI_HANDLER(scatterv, DUMPI_Scatterv)
DUMPI_HANDLER(allgather, DUMPI_Allgather)
DUMPI_HANDLER(allgatherv, DUMPI_Allgatherv)
DUMPI_HANDLER(alltoall, DUMPI_Alltoall)
DUMPI_HANDLER(alltoallv, DUMPI_Alltoallv)
DUMPI_HANDLER(reduce, DUMPI_Reduce)
DUMPI_HANDLER(op_create, DUMPI_Op_create)
DUMPI_HANDLER(op_free, DUMPI_Op_free)
DUMPI_HANDLER(allreduce, DUMPI_Allreduce)
DUMPI_HANDLER(reduce_scatter, DUMPI_Reduce_scatter)
DUMPI_HANDLER(scan, DUMPI_Scan)
DUMPI_HANDLER(group_size, DUMPI_Group_size)
DUMPI_HANDLER(group_rank, DUMPI_Group_rank)
DUMPI_HANDLER(group_translate_ranks, DUMPI_Group_translate_ranks)
DUMPI_HANDLER(group_compare, DUMPI_Group_compare)
DUMPI_HANDLER(comm_group, DUMPI_Comm_group)
DUMPI_HANDLER(group_union, DUMPI_Group_union)
DUMPI_HANDLER(group_intersection, DUMPI_Group_intersection)
DUMPI_HANDLER(group_difference, DUMPI_Group_difference)
DUMPI_HANDLER(group_incl, DUMPI_Group_incl)
DUMPI_HANDLER(group_excl, DUMPI_Group_excl)
DUMPI_HANDLER(group_range_incl, DUMPI_Group_range_incl)
DUMPI_HANDLER(group_range_excl, DUMPI_Group_range_excl)
DUMPI_HANDLER(group_free, DUMPI_Group_free)
DUMPI_HANDLER(comm_size, DUMPI_Comm_size)
DUMPI_HANDLER(comm_rank, DUMPI_Comm_rank)
DUMPI_HANDLER(comm_compare, DUMPI_Comm_compare)
DUMPI_HANDLER(comm_dup, DUMPI_Comm_dup)
DUMPI_HANDLER(comm_create, DUMPI_Comm_create)
DUMPI_HANDLER(comm_split, DUMPI_Comm_split)
DUMPI_HANDLER(comm_free, DUMPI_Comm_free)
DUMPI_HANDLER(comm_test_inter, DUMPI_Comm_test_inter)
DUMPI_HANDLER(comm_remote_size, DUMPI_Comm_remote_size)
DUMPI_HANDLER(comm_remote_group, DUMPI_Comm_remote_group)
DUMPI_HANDLER(intercomm_create, DUMPI_Intercomm_create)
DUMPI_HANDLER(intercomm_merge, DUMPI_Intercomm_merge)
DUMPI_HANDLER(keyval_create, DUMPI_Keyval_create)
DUMPI_HANDLER(keyval_free, DUMPI_Keyval_free)
DUMPI_HANDLER(attr_put, DUMPI_Attr_put)
DUMPI_HANDLER(attr_get, DUMPI_Attr_get)
DUMPI_HANDLER(attr_delete, DUMPI_Attr_delete)
DUMPI_HANDLER(topo_test, DUMPI_Topo_test)
DUMPI_HANDLER(cart_create, DUMPI_Cart_create)
DUMPI_HANDLER(dims_create, DUMPI_Dims_create)
DUMPI_HANDLER(graph_create, DUMPI_Graph_create)
DUMPI_HANDLER(graphdims_get, DUMPI_Graphdims_get)
DUMPI_HANDLER(graph_get, DUMPI_Graph_get)
DUMPI_HANDLER(cartdim_get, DUMPI_Cartdim_get)
DUMPI_HANDLER(cart_get, DUMPI_Cart_get)
DUMPI_HANDLER(cart_rank, DUMPI_Cart_rank)
DUMPI_HANDLER(cart_coords, DUMPI_Cart_coords)
DUMPI_HANDLER(graph_neighbors_count, DUMPI_Graph_neighbors_count)
DUMPI_HANDLER(graph_neighbors, DUMPI_Graph_neighbors)
DUMPI_HANDLER(cart_shift, DUMPI_Cart_shift)
DUMPI_HANDLER(cart_sub, DUMPI_Cart_sub)
DUMPI_HANDLER(cart_map, DUMPI_Cart_map)
DUMPI_HANDLER(graph_map, DUMPI_Graph_map)
DUMPI_HANDLER(get_processor_name, DUMPI_Get_processor_name)
DUMPI_HANDLER(get_version, DUMPI_Get_version)
DUMPI_HANDLER(errhandler_create, DUMPI_Errhandler_create)
DUMPI_HANDLER(errhandler_set, DUMPI_Errhandler_set)
DUMPI_HANDLER(errhandler_get, DUMPI_Errhandler_get)
DUMPI_HANDLER(errhandler_free, DUMPI_Errhandler_free)
DUMPI_HANDLER(error_string, DUMPI_Error_string)
DUMPI_HANDLER(error_class, DUMPI_Error_class)
DUMPI_HANDLER(wtime, DUMPI_Wtime)
DUMPI_HANDLER(wtick, DUMPI_Wtick)
DUMPI_HANDLER(init, DUMPI_Init)
DUMPI_HANDLER(finalize, DUMPI_Finalize)
DUMPI_HANDLER(initialized, DUMPI_Initialized)
DUMPI_HANDLER(abort, DUMPI_Abort)
DUMPI_HANDLER(close_port, DUMPI_Close_port)
DUMPI_HANDLER(comm_accept, DUMPI_Comm_accept)
DUMPI_HANDLER(comm_connect, DUMPI_Comm_connect)
DUMPI_HANDLER(comm_disconnect, DUMPI_Comm_disconnect)
DUMPI_HANDLER(comm_get_parent, DUMPI_Comm_get_parent)
DUMPI_HANDLER(comm_join, DUMPI_Comm_join)
DUMPI_HANDLER(comm_spawn, DUMPI_Comm_spawn)
DUMPI_HANDLER(comm_spawn_multiple, DUMPI_Comm_spawn_multiple)
DUMPI_HANDLER(lookup_name, DUMPI_Lookup_name)
DUMPI_HANDLER(open_port, DUMPI_Open_port)
DUMPI_HANDLER(publish_name, DUMPI_Publish_name)
DUMPI_HANDLER(unpublish_name, DUMPI_Unpublish_name)
DUMPI_HANDLER(accumulate, DUMPI_Accumulate)
DUMPI_HANDLER(get, DUMPI_Get)
DUMPI_HANDLER(put, DUMPI_Put)
DUMPI_HANDLER(win_complete, DUMPI_Win_complete)
DUMPI_HANDLER(win_create, DUMPI_Win_create)
DUMPI_HANDLER(win_fence, DUMPI_Win_fence)
DUMPI_HANDLER(win_free, DUMPI_Win_free)
DUMPI_HANDLER(win_get_group, DUMPI_Win_get_group)
DUMPI_HANDLER(win_lock, DUMPI_Win_lock)
DUMPI_HANDLER(win_post, DUMPI_Win_post)
DUMPI_HANDLER(win_start, DUMPI_Win_start)
DUMPI_HANDLER(win_test, DUMPI_Win_test)
DUMPI_HANDLER(win_unlock, DUMPI_Win_unlock)
DUMPI_HANDLER(win_wait, DUMPI_Win_wait)
DUMPI_HANDLER(alltoallw, DUMPI_Alltoallw)
DUMPI_HANDLER(exscan, DUMPI_Exscan)
DUMPI_HANDLER(add_error_class, DUMPI_Add_error_class)
DUMPI_HANDLER(add_error_code, DUMPI_Add_error_code)
DUMPI_HANDLER(add_error_string, DUMPI_Add_error_string)
DUMPI_HANDLER(comm_call_errhandler, DUMPI_Comm_call_errhandler)
DUMPI_HANDLER(comm_create_keyval, DUMPI_Comm_create_keyval)
DUMPI_HANDLER(comm_delete_attr, DUMPI_Comm_delete_attr)
DUMPI_HANDLER(comm_free_keyval, DUMPI_Comm_free_keyval)
DUMPI_HANDLER(comm_get_attr, DUMPI_Comm_get_attr)
DUMPI_HANDLER(comm_get_name, DUMPI_Comm_get_name)
DUMPI_HANDLER(comm_set_attr, DUMPI_Comm_set_attr)
DUMPI_HANDLER(comm_set_name, DUMPI_Comm_set_name)
DUMPI_HANDLER(file_call_errhandler, DUMPI_File_call_errhandler)
DUMPI_HANDLER(grequest_complete, DUMPI_Grequest_complete)
DUMPI_HANDLER(grequest_start, DUMPI_Grequest_start)
DUMPI_HANDLER(init_thread, DUMPI_Init_thread)
DUMPI_HANDLER(is_thread_main, DUMPI_Is_thread_main)
DUMPI_HANDLER(query_thread, DUMPI_Query_thread)
DUMPI_HANDLER(status_set_cancelled, DUMPI_Status_set_cancelled)
DUMPI_HANDLER(status_set_elements, DUMPI_Status_set_elements)
DUMPI_HANDLER(type_create_keyval, DUMPI_Type_create_keyval)
DUMPI_HANDLER(type_delete_attr, DUMPI_Type_delete_attr)
DUMPI_HANDLER(type_dup, DUMPI_Type_dup)
DUMPI_HANDLER(type_free_keyval, DUMPI_Type_free_keyval)
DUMPI_HANDLER(type_get_attr, DUMPI_Type_get_attr)
DUMPI_HANDLER(type_get_contents, DUMPI_Type_get_contents)
DUMPI_HANDLER(type_get_envelope, DUMPI_Type_get_envelope)
DUMPI_HANDLER(type_get_name, DUMPI_Type_get_name)
DUMPI_HANDLER(type_set_attr, DUMPI_Type_set_attr)
DUMPI_HANDLER(type_set_name, DUMPI_Type_set_name)
DUMPI_HANDLER(type_match_size, DUMPI_Type_match_size)
DUMPI_HANDLER(win_call_errhandler, DUMPI_Win_call_errhandler)
DUMPI_HANDLER(win_create_keyval, DUMPI_Win_create_keyval)
DUMPI_HANDLER(win_delete_attr, DUMPI_Win_delete_attr)
DUMPI_HANDLER(win_free_keyval, DUMPI_Win_free_keyval)
DUMPI_HANDLER(win_get_attr, DUMPI_Win_get_attr)
DUMPI_HANDLER(win_get_name, DUMPI_Win_get_name)
DUMPI_HANDLER(win_set_attr, DUMPI_Win_set_attr)
DUMPI_HANDLER(win_set_name, DUMPI_Win_set_name)
DUMPI_HANDLER(alloc_mem, DUMPI_Alloc_mem)
DUMPI_HANDLER(comm_create_errhandler, DUMPI_Comm_create_errhandler)
DUMPI_HANDLER(comm_get_errhandler, DUMPI_Comm_get_errhandler)
DUMPI_HANDLER(comm_set_errhandler, DUMPI_Comm_set_errhandler)
DUMPI_HANDLER(file_create_errhandler, DUMPI_File_create_errhandler)
DUMPI_HANDLER(file_get_errhandler, DUMPI_File_get_errhandler)
DUMPI_HANDLER(file_set_errhandler, DUMPI_File_set_errhandler)
DUMPI_HANDLER(finalized, DUMPI_Finalized)
DUMPI_HANDLER(free_mem, DUMPI_Free_mem)
DUMPI_HANDLER(get_address, DUMPI_Get_address)
DUMPI_HANDLER(info_create, DUMPI_Info_create)
DUMPI_HANDLER(info_delete, DUMPI_Info_delete)
DUMPI_HANDLER(info_dup, DUMPI_Info_dup)
DUMPI_HANDLER(info_free, DUMPI_Info_free)
DUMPI_HANDLER(info_get, DUMPI_Info_get)
DUMPI_HANDLER(info_get_nkeys, DUMPI_Info_get_nkeys)
DUMPI_HANDLER(info_get_nthkey, DUMPI_Info_get_nthkey)
DUMPI_HANDLER(info_get_valuelen, DUMPI_Info_get_valuelen)
DUMPI_HANDLER(info_set, DUMPI_Info_set)
DUMPI_HANDLER(pack_external, DUMPI_Pack_external)
DUMPI_HANDLER(pack_external_size, DUMPI_Pack_external_size)
DUMPI_HANDLER(request_get_status, DUMPI_Request_get_status)
DUMPI_HANDLER(type_create_darray, DUMPI_Type_create_darray)
DUMPI_HANDLER(type_create_hindexed, DUMPI_Type_create_hindexed)
DUMPI_HANDLER(type_create_hvector, DUMPI_Type_create_hvector)
DUMPI_HANDLER(type_create_indexed_block, DUMPI_Type_create_indexed_block)
DUMPI_HANDLER(type_create_resized, DUMPI_Type_create_resized)
DUMPI_HANDLER(type_create_struct, DUMPI_Type_create_struct)
DUMPI_HANDLER(type_create_subarray, DUMPI_Type_create_subarray)
DUMPI_HANDLER(type_get_extent, DUMPI_Type_get_extent)
DUMPI_HANDLER(type_get_true_extent, DUMPI_Type_get_true_extent)
DUMPI_HANDLER(unpack_external, DUMPI_Unpack_external)
DUMPI_HANDLER(win_create_errhandler, DUMPI_Win_create_errhandler)
DUMPI_HANDLER(win_get_errhandler, DUMPI_Win_get_errhandler)
DUMPI_HANDLER(win_set_errhandler, DUMPI_Win_set_errhandler)
DUMPI_HANDLER(file_open, DUMPI_File_open)
DUMPI_HANDLER(file_close, DUMPI_File_close)
DUMPI_HANDLER(file_delete, DUMPI_File_delete)
DUMPI_HANDLER(file_set_size, DUMPI_File_set_size)
DUMPI_HANDLER(file_preallocate, DUMPI_File_preallocate)
DUMPI_HANDLER(file_get_size, DUMPI_File_get_size)
DUMPI_HANDLER(file_get_group, DUMPI_File_get_group)
DUMPI_HANDLER(file_get_amode, DUMPI_File_get_amode)
DUMPI_HANDLER(file_set_info, DUMPI_File_set_info)
DUMPI_HANDLER(file_get_info, DUMPI_File_get_info)
DUMPI_HANDLER(file_set_view, DUMPI_File_set_view)
DUMPI_HANDLER(file_get_view, DUMPI_File_get_view)
DUMPI_HANDLER(file_read_at, DUMPI_File_read_at)
DUMPI_HANDLER(file_read_at_all, DUMPI_File_read_at_all)
DUMPI_HANDLER(file_write_at, DUMPI_File_write_at)
DUMPI_HANDLER(file_write_at_all, DUMPI_File_write_at_all)
DUMPI_HANDLER(file_iread_at, DUMPI_File_iread_at)
DUMPI_HANDLER(file_iwrite_at, DUMPI_File_iwrite_at)
DUMPI_HANDLER(file_read, DUMPI_File_read)
DUMPI_HANDLER(file_read_all, DUMPI_File_read_all)
DUMPI_HANDLER(file_write, DUMPI_File_write)
DUMPI_HANDLER(file_write_all, DUMPI_File_write_all)
DUMPI_HANDLER(file_iread, DUMPI_File_iread)
DUMPI_HANDLER(file_iwrite, DUMPI_File_iwrite)
DUMPI_HANDLER(file_seek, DUMPI_File_seek)
DUMPI_HANDLER(file_get_position, DUMPI_File_get_position)
DUMPI_HANDLER(file_get_byte_offset, DUMPI_File_get_byte_offset)
DUMPI_HANDLER(file_read_shared, DUMPI_File_read_shared)
DUMPI_HANDLER(file_write_shared, DUMPI_File_write_shared)
DUMPI_HANDLER(file_iread_shared, DUMPI_File_iread_shared)
DUMPI_HANDLER(file_iwrite_shared, DUMPI_File_iwrite_shared)
DUMPI_HANDLER(file_read_ordered, DUMPI_File_read_ordered)
DUMPI_HANDLER(file_write_ordered, DUMPI_File_write_ordered)
DUMPI_HANDLER(file_seek_shared, DUMPI_File_seek_shared)
DUMPI_HANDLER(file_get_position_shared, DUMPI_File_get_position_shared)
DUMPI_HANDLER(file_read_at_all_begin, DUMPI_File_read_at_all_begin)
DUMPI_HANDLER(file_read_at_all_end, DUMPI_File_read_at_all_end)
DUMPI_HANDLER(file_write_at_all_begin, DUMPI_File_write_at_all_begin)
DUMPI_HANDLER(file_write_at_all_end, DUMPI_File_write_at_all_end)
DUMPI_HANDLER(file_read_all_begin, DUMPI_File_read_all_begin)
DUMPI_HANDLER(file_read_all_end, DUMPI_File_read_all_end)
DUMPI_HANDLER(file_write_all_begin, DUMPI_File_write_all_begin)
DUMPI_HANDLER(file_write_all_end, DUMPI_File_write_all_end)
DUMPI_HANDLER(file_read_ordered_begin, DUMPI_File_read_ordered_begin)
DUMPI_HANDLER(file_read_ordered_end, DUMPI_File_read_ordered_end)
DUMPI_HANDLER(file_write_ordered_begin, DUMPI_File_write_ordered_begin)
DUMPI_HANDLER(file_write_ordered_end, DUMPI_File_write_ordered_end)
DUMPI_HANDLER(file_get_type_extent, DUMPI_File_get_type_extent)
DUMPI_HANDLER(register_datarep, DUMPI_Register_datarep)
DUMPI_HANDLER(file_set_atomicity, DUMPI_File_set_atomicity)
DUMPI_HANDLER(file_get_atomicity, DUMPI_File_get_atomicity)
DUMPI_HANDLER(file_sync, DUMPI_File_sync)
DUMPI_HANDLERIO(test, DUMPIO_Test)
DUMPI_HANDLERIO(wait, DUMPIO_Wait)
DUMPI_HANDLERIO(testall, DUMPIO_Testall)
DUMPI_HANDLERIO(waitall, DUMPIO_Waitall)
DUMPI_HANDLERIO(testany, DUMPIO_Testany)
DUMPI_HANDLERIO(waitany, DUMPIO_Waitany)
DUMPI_HANDLERIO(waitsome, DUMPIO_Waitsome)
DUMPI_HANDLERIO(testsome, DUMPIO_Testsome)
DUMPI_HANDLERF(function_enter, func_enter, DUMPI_Function_enter)
DUMPI_HANDLERF(function_exit, func_exit, DUMPI_Function_exit)

/* Set callbacks for MPI functions. */
int d2d_set_callbacks(d2dopts *opt) {
  assert(opt != NULL);
#define CBACK(TAG) opt->cback.on_ ## TAG = handle_ ## TAG;
  CBACK(send);
  CBACK(recv);
  CBACK(get_count);
  CBACK(bsend);
  CBACK(ssend);
  CBACK(rsend);
  CBACK(buffer_attach);
  CBACK(buffer_detach);
  CBACK(isend);
  CBACK(ibsend);
  CBACK(issend);
  CBACK(irsend);
  CBACK(irecv);
  CBACK(wait);
  CBACK(test);
  CBACK(request_free);
  CBACK(waitany);
  CBACK(testany);
  CBACK(waitall);
  CBACK(testall);
  CBACK(waitsome);
  CBACK(testsome);
  CBACK(iprobe);
  CBACK(probe);
  CBACK(cancel);
  CBACK(test_cancelled);
  CBACK(send_init);
  CBACK(bsend_init);
  CBACK(ssend_init);
  CBACK(rsend_init);
  CBACK(recv_init);
  CBACK(start);
  CBACK(startall);
  CBACK(sendrecv);
  CBACK(sendrecv_replace);
  CBACK(type_contiguous);
  CBACK(type_vector);
  CBACK(type_hvector);
  CBACK(type_indexed);
  CBACK(type_hindexed);
  CBACK(type_struct);
  CBACK(address);
  CBACK(type_extent);
  CBACK(type_size);
  CBACK(type_lb);
  CBACK(type_ub);
  CBACK(type_commit);
  CBACK(type_free);
  CBACK(get_elements);
  CBACK(pack);
  CBACK(unpack);
  CBACK(pack_size);
  CBACK(barrier);
  CBACK(bcast);
  CBACK(gather);
  CBACK(gatherv);
  CBACK(scatter);
  CBACK(scatterv);
  CBACK(allgather);
  CBACK(allgatherv);
  CBACK(alltoall);
  CBACK(alltoallv);
  CBACK(reduce);
  CBACK(op_create);
  CBACK(op_free);
  CBACK(allreduce);
  CBACK(reduce_scatter);
  CBACK(scan);
  CBACK(group_size);
  CBACK(group_rank);
  CBACK(group_translate_ranks);
  CBACK(group_compare);
  CBACK(comm_group);
  CBACK(group_union);
  CBACK(group_intersection);
  CBACK(group_difference);
  CBACK(group_incl);
  CBACK(group_excl);
  CBACK(group_range_incl);
  CBACK(group_range_excl);
  CBACK(group_free);
  CBACK(comm_size);
  CBACK(comm_rank);
  CBACK(comm_compare);
  CBACK(comm_dup);
  CBACK(comm_create);
  CBACK(comm_split);
  CBACK(comm_free);
  CBACK(comm_test_inter);
  CBACK(comm_remote_size);
  CBACK(comm_remote_group);
  CBACK(intercomm_create);
  CBACK(intercomm_merge);
  CBACK(keyval_create);
  CBACK(keyval_free);
  CBACK(attr_put);
  CBACK(attr_get);
  CBACK(attr_delete);
  CBACK(topo_test);
  CBACK(cart_create);
  CBACK(dims_create);
  CBACK(graph_create);
  CBACK(graphdims_get);
  CBACK(graph_get);
  CBACK(cartdim_get);
  CBACK(cart_get);
  CBACK(cart_rank);
  CBACK(cart_coords);
  CBACK(graph_neighbors_count);
  CBACK(graph_neighbors);
  CBACK(cart_shift);
  CBACK(cart_sub);
  CBACK(cart_map);
  CBACK(graph_map);
  CBACK(get_processor_name);
  CBACK(get_version);
  CBACK(errhandler_create);
  CBACK(errhandler_set);
  CBACK(errhandler_get);
  CBACK(errhandler_free);
  CBACK(error_string);
  CBACK(error_class);
  CBACK(wtime);
  CBACK(wtick);
  CBACK(init);
  CBACK(finalize);
  CBACK(initialized);
  CBACK(abort);
  CBACK(close_port);
  CBACK(comm_accept);
  CBACK(comm_connect);
  CBACK(comm_disconnect);
  CBACK(comm_get_parent);
  CBACK(comm_join);
  CBACK(comm_spawn);
  CBACK(comm_spawn_multiple);
  CBACK(lookup_name);
  CBACK(open_port);
  CBACK(publish_name);
  CBACK(unpublish_name);
  CBACK(accumulate);
  CBACK(get);
  CBACK(put);
  CBACK(win_complete);
  CBACK(win_create);
  CBACK(win_fence);
  CBACK(win_free);
  CBACK(win_get_group);
  CBACK(win_lock);
  CBACK(win_post);
  CBACK(win_start);
  CBACK(win_test);
  CBACK(win_unlock);
  CBACK(win_wait);
  CBACK(alltoallw);
  CBACK(exscan);
  CBACK(add_error_class);
  CBACK(add_error_code);
  CBACK(add_error_string);
  CBACK(comm_call_errhandler);
  CBACK(comm_create_keyval);
  CBACK(comm_delete_attr);
  CBACK(comm_free_keyval);
  CBACK(comm_get_attr);
  CBACK(comm_get_name);
  CBACK(comm_set_attr);
  CBACK(comm_set_name);
  CBACK(file_call_errhandler);
  CBACK(grequest_complete);
  CBACK(grequest_start);
  CBACK(init_thread);
  CBACK(is_thread_main);
  CBACK(query_thread);
  CBACK(status_set_cancelled);
  CBACK(status_set_elements);
  CBACK(type_create_keyval);
  CBACK(type_delete_attr);
  CBACK(type_dup);
  CBACK(type_free_keyval);
  CBACK(type_get_attr);
  CBACK(type_get_contents);
  CBACK(type_get_envelope);
  CBACK(type_get_name);
  CBACK(type_set_attr);
  CBACK(type_set_name);
  CBACK(type_match_size);
  CBACK(win_call_errhandler);
  CBACK(win_create_keyval);
  CBACK(win_delete_attr);
  CBACK(win_free_keyval);
  CBACK(win_get_attr);
  CBACK(win_get_name);
  CBACK(win_set_attr);
  CBACK(win_set_name);
  CBACK(alloc_mem);
  CBACK(comm_create_errhandler);
  CBACK(comm_get_errhandler);
  CBACK(comm_set_errhandler);
  CBACK(file_create_errhandler);
  CBACK(file_get_errhandler);
  CBACK(file_set_errhandler);
  CBACK(finalized);
  CBACK(free_mem);
  CBACK(get_address);
  CBACK(info_create);
  CBACK(info_delete);
  CBACK(info_dup);
  CBACK(info_free);
  CBACK(info_get);
  CBACK(info_get_nkeys);
  CBACK(info_get_nthkey);
  CBACK(info_get_valuelen);
  CBACK(info_set);
  CBACK(pack_external);
  CBACK(pack_external_size);
  CBACK(request_get_status);
  CBACK(type_create_darray);
  CBACK(type_create_hindexed);
  CBACK(type_create_hvector);
  CBACK(type_create_indexed_block);
  CBACK(type_create_resized);
  CBACK(type_create_struct);
  CBACK(type_create_subarray);
  CBACK(type_get_extent);
  CBACK(type_get_true_extent);
  CBACK(unpack_external);
  CBACK(win_create_errhandler);
  CBACK(win_get_errhandler);
  CBACK(win_set_errhandler);
  CBACK(file_open);
  CBACK(file_close);
  CBACK(file_delete);
  CBACK(file_set_size);
  CBACK(file_preallocate);
  CBACK(file_get_size);
  CBACK(file_get_group);
  CBACK(file_get_amode);
  CBACK(file_set_info);
  CBACK(file_get_info);
  CBACK(file_set_view);
  CBACK(file_get_view);
  CBACK(file_read_at);
  CBACK(file_read_at_all);
  CBACK(file_write_at);
  CBACK(file_write_at_all);
  CBACK(file_iread_at);
  CBACK(file_iwrite_at);
  CBACK(file_read);
  CBACK(file_read_all);
  CBACK(file_write);
  CBACK(file_write_all);
  CBACK(file_iread);
  CBACK(file_iwrite);
  CBACK(file_seek);
  CBACK(file_get_position);
  CBACK(file_get_byte_offset);
  CBACK(file_read_shared);
  CBACK(file_write_shared);
  CBACK(file_iread_shared);
  CBACK(file_iwrite_shared);
  CBACK(file_read_ordered);
  CBACK(file_write_ordered);
  CBACK(file_seek_shared);
  CBACK(file_get_position_shared);
  CBACK(file_read_at_all_begin);
  CBACK(file_read_at_all_end);
  CBACK(file_write_at_all_begin);
  CBACK(file_write_at_all_end);
  CBACK(file_read_all_begin);
  CBACK(file_read_all_end);
  CBACK(file_write_all_begin);
  CBACK(file_write_all_end);
  CBACK(file_read_ordered_begin);
  CBACK(file_read_ordered_end);
  CBACK(file_write_ordered_begin);
  CBACK(file_write_ordered_end);
  CBACK(file_get_type_extent);
  CBACK(register_datarep);
  CBACK(file_set_atomicity);
  CBACK(file_get_atomicity);
  CBACK(file_sync);
  opt->cback.on_iotest = handle_iotest;
  opt->cback.on_iowait = handle_iowait;
  opt->cback.on_iotestall = handle_iotestall;
  opt->cback.on_iowaitall = handle_iowaitall;
  opt->cback.on_iotestany = handle_iotestany;
  opt->cback.on_iowaitany = handle_iowaitany;
  opt->cback.on_iowaitsome = handle_iowaitsome;
  opt->cback.on_iotestsome = handle_iotestsome;
  CBACK(function_enter);
  CBACK(function_exit);

  return 0;
}

/* Parse input and write output for a single binary trace file. */
int d2d_parse_stream(const char *in, const char *out, d2dopts *opt) {
  int error = 0, id = 0;
  dumpi_profile *profile = undumpi_open(in);
  opt->oprofile = NULL;
  /* We don't open the output stream until the first MPI call,
   * otherwise we can't decide the time bias properly */
  opt->outname = out;
  /* Set up the footer -- we copy the ignored count and then add our own */
  dumpi_footer infoot;
  dumpi_read_footer(profile, &infoot);
  memset(&opt->footer, 0, sizeof(dumpi_footer));
  for(id = 0; id <= DUMPI_ALL_FUNCTIONS; ++id) {
    if(infoot.ignored_count[id]) {
      opt->footer.call_count[id] = infoot.ignored_count[id];
      opt->footer.ignored_count[id] = infoot.ignored_count[id];
    }
  }
  /* Go */
  if(! profile) {
    fprintf(stderr, "Error:  Failed to create profile \"%s\": %s\n",
	    in, strerror(errno));
    goto pieces;
  }
  /* For an unclear reason, I chose to use '1' as a good return for undumpi */
  error = (undumpi_read_stream(profile, &opt->cback, opt) == 0);
  if(error)
    fprintf(stderr, "Error:  Failed running undumpi.\n");
  /* Now we just need to handle the rest of the dumpi records */
  /* sizes, addresses, perfcounters, header, body (done), footer, and keyvals */
  {
    dumpi_header *header = dumpi_build_header();
    dumpi_read_header(profile, header);
    header->version[0] = dumpi_version;
    header->version[1] = dumpi_subversion;
    header->version[2] = dumpi_subsubversion;
    dumpi_write_header(opt->oprofile, header);
    dumpi_free_header(header);
  }
  {
    dumpi_sizeof sizes = {0, NULL};
    dumpi_read_datatype_sizes(profile, &sizes);
    dumpi_write_datatype_sizes(opt->oprofile, &sizes);
    free(sizes.size);
  }
  {
    int i, count;
    uint64_t *addresses;
    char **names;
    dumpi_read_function_addresses(profile, &count, &addresses, &names);
    if(opt->write_userfuncs == 0) count = 0;
    dumpi_write_function_addresses(opt->oprofile, count, addresses, names);
    free(addresses);
    for(i = 0; i < count; ++i) free(names[i]);
    free(names);
  }
  {
    int counters, maxlabels = 20;
    dumpi_perflabel_t *labels =
      (dumpi_perflabel_t*)malloc(maxlabels*sizeof(dumpi_perflabel_t));
    dumpi_read_perfctr_labels(profile, &counters, labels, maxlabels);
    if(opt->output.perfinfo == 0) counters = 0;
    dumpi_write_perfctr_labels(opt->oprofile, counters, labels);
    free(labels);
  }
  {
    dumpi_write_footer(opt->oprofile, &opt->footer);
  }
  {
    dumpi_keyval_record *keyval = dumpi_alloc_keyval_record();
    dumpi_read_keyval_record(profile, keyval);
    dumpi_write_keyval_record(opt->oprofile, keyval);
    dumpi_free_keyval_record(keyval);
  }
  /* This closes the oprofile and frees up the memory buffer */
  dumpi_write_index(opt->oprofile);
  fclose(profile->file);
  dumpi_free_output_profile(opt->oprofile);

 pieces:
  return error;
}