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

#include <dumpi/common/funcs.h>
#include <dumpi/dumpiconfig.h>

const int8_t dumpi_version = (DUMPI_VERSION);
const int8_t dumpi_subversion = (DUMPI_SUBVERSION);
const int8_t dumpi_subsubversion = (DUMPI_SUBSUBVERSION);

const char *dumpi_function_names[DUMPI_END_OF_STREAM+1] = {
  "MPI_Send"                     , "MPI_Recv"                     , 
  "MPI_Get_count"                , "MPI_Bsend"                    , 
  "MPI_Ssend"                    , "MPI_Rsend"                    , 
  "MPI_Buffer_attach"            , "MPI_Buffer_detach"            , 
  "MPI_Isend"                    , "MPI_Ibsend"                   , 
  "MPI_Issend"                   , "MPI_Irsend"                   , 
  "MPI_Irecv"                    , "MPI_Wait"                     , 
  "MPI_Test"                     , "MPI_Request_free"             , 
  "MPI_Waitany"                  , "MPI_Testany"                  , 
  "MPI_Waitall"                  , "MPI_Testall"                  , 
  "MPI_Waitsome"                 , "MPI_Testsome"                 , 
  "MPI_Iprobe"                   , "MPI_Probe"                    , 
  "MPI_Cancel"                   , "MPI_Test_cancelled"           , 
  "MPI_Send_init"                , "MPI_Bsend_init"               , 
  "MPI_Ssend_init"               , "MPI_Rsend_init"               , 
  "MPI_Recv_init"                , "MPI_Start"                    , 
  "MPI_Startall"                 , "MPI_Sendrecv"                 , 
  "MPI_Sendrecv_replace"         , "MPI_Type_contiguous"          , 
  "MPI_Type_vector"              , "MPI_Type_hvector"             , 
  "MPI_Type_indexed"             , "MPI_Type_hindexed"            , 
  "MPI_Type_struct"              , "MPI_Address"                  , 
  "MPI_Type_extent"              , "MPI_Type_size"                , 
  "MPI_Type_lb"                  , "MPI_Type_ub"                  , 
  "MPI_Type_commit"              , "MPI_Type_free"                , 
  "MPI_Get_elements"             , "MPI_Pack"                     , 
  "MPI_Unpack"                   , "MPI_Pack_size"                , 
  "MPI_Barrier"                  , "MPI_Bcast"                    , 
  "MPI_Gather"                   , "MPI_Gatherv"                  , 
  "MPI_Scatter"                  , "MPI_Scatterv"                 , 
  "MPI_Allgather"                , "MPI_Allgatherv"               , 
  "MPI_Alltoall"                 , "MPI_Alltoallv"                , 
  "MPI_Reduce"                   , "MPI_Op_create"                , 
  "MPI_Op_free"                  , "MPI_Allreduce"                , 
  "MPI_Reduce_scatter"           , "MPI_Scan"                     , 
  "MPI_Group_size"               , "MPI_Group_rank"               , 
  "MPI_Group_translate_ranks"    , "MPI_Group_compare"            , 
  "MPI_Comm_group"               , "MPI_Group_union"              , 
  "MPI_Group_intersection"       , "MPI_Group_difference"         , 
  "MPI_Group_incl"               , "MPI_Group_excl"               , 
  "MPI_Group_range_incl"         , "MPI_Group_range_excl"         , 
  "MPI_Group_free"               , "MPI_Comm_size"                , 
  "MPI_Comm_rank"                , "MPI_Comm_compare"             , 
  "MPI_Comm_dup"                 , "MPI_Comm_create"              , 
  "MPI_Comm_split"               , "MPI_Comm_free"                , 
  "MPI_Comm_test_inter"          , "MPI_Comm_remote_size"         , 
  "MPI_Comm_remote_group"        , "MPI_Intercomm_create"         , 
  "MPI_Intercomm_merge"          , "MPI_Keyval_create"            , 
  "MPI_Keyval_free"              , "MPI_Attr_put"                 , 
  "MPI_Attr_get"                 , "MPI_Attr_delete"              , 
  "MPI_Topo_test"                , "MPI_Cart_create"              , 
  "MPI_Dims_create"              , "MPI_Graph_create"             , 
  "MPI_Graphdims_get"            , "MPI_Graph_get"                , 
  "MPI_Cartdim_get"              , "MPI_Cart_get"                 , 
  "MPI_Cart_rank"                , "MPI_Cart_coords"              , 
  "MPI_Graph_neighbors_count"    , "MPI_Graph_neighbors"          , 
  "MPI_Cart_shift"               , "MPI_Cart_sub"                 , 
  "MPI_Cart_map"                 , "MPI_Graph_map"                , 
  "MPI_Get_processor_name"       , "MPI_Get_version"              , 
  "MPI_Errhandler_create"        , "MPI_Errhandler_set"           , 
  "MPI_Errhandler_get"           , "MPI_Errhandler_free"          , 
  "MPI_Error_string"             , "MPI_Error_class"              , 
  "MPI_Wtime"                    , "MPI_Wtick"                    , 
  "MPI_Init"                     , "MPI_Finalize"                 , 
  "MPI_Initialized"              , "MPI_Abort"                    , 
  "MPI_Pcontrol"                 , "MPI_Close_port"               , 
  "MPI_Comm_accept"              , "MPI_Comm_connect"             , 
  "MPI_Comm_disconnect"          , "MPI_Comm_get_parent"          , 
  "MPI_Comm_join"                , "MPI_Comm_spawn"               , 
  "MPI_Comm_spawn_multiple"      , "MPI_Lookup_name"              , 
  "MPI_Open_port"                , "MPI_Publish_name"             , 
  "MPI_Unpublish_name"           , "MPI_Accumulate"               , 
  "MPI_Get"                      , "MPI_Put"                      , 
  "MPI_Win_complete"             , "MPI_Win_create"               , 
  "MPI_Win_fence"                , "MPI_Win_free"                 , 
  "MPI_Win_get_group"            , "MPI_Win_lock"                 , 
  "MPI_Win_post"                 , "MPI_Win_start"                , 
  "MPI_Win_test"                 , "MPI_Win_unlock"               , 
  "MPI_Win_wait"                 , "MPI_Alltoallw"                , 
  "MPI_Exscan"                   , "MPI_Add_error_class"          , 
  "MPI_Add_error_code"           , "MPI_Add_error_string"         , 
  "MPI_Comm_call_errhandler"     , "MPI_Comm_create_keyval"       , 
  "MPI_Comm_delete_attr"         , "MPI_Comm_free_keyval"         , 
  "MPI_Comm_get_attr"            , "MPI_Comm_get_name"            , 
  "MPI_Comm_set_attr"            , "MPI_Comm_set_name"            , 
  "MPI_File_call_errhandler"     , "MPI_Grequest_complete"        , 
  "MPI_Grequest_start"           , "MPI_Init_thread"              , 
  "MPI_Is_thread_main"           , "MPI_Query_thread"             , 
  "MPI_Status_set_cancelled"     , "MPI_Status_set_elements"      , 
  "MPI_Type_create_keyval"       , "MPI_Type_delete_attr"         , 
  "MPI_Type_dup"                 , "MPI_Type_free_keyval"         , 
  "MPI_Type_get_attr"            , "MPI_Type_get_contents"        , 
  "MPI_Type_get_envelope"        , "MPI_Type_get_name"            , 
  "MPI_Type_set_attr"            , "MPI_Type_set_name"            , 
  "MPI_Type_match_size"          , "MPI_Win_call_errhandler"      , 
  "MPI_Win_create_keyval"        , "MPI_Win_delete_attr"          , 
  "MPI_Win_free_keyval"          , "MPI_Win_get_attr"             , 
  "MPI_Win_get_name"             , "MPI_Win_set_attr"             , 
  "MPI_Win_set_name"             , "MPI_Alloc_mem"                , 
  "MPI_Comm_create_errhandler"   , "MPI_Comm_get_errhandler"      , 
  "MPI_Comm_set_errhandler"      , "MPI_File_create_errhandler"   , 
  "MPI_File_get_errhandler"      , "MPI_File_set_errhandler"      , 
  "MPI_Finalized"                , "MPI_Free_mem"                 , 
  "MPI_Get_address"              , "MPI_Info_create"              , 
  "MPI_Info_delete"              , "MPI_Info_dup"                 , 
  "MPI_Info_free"                , "MPI_Info_get"                 , 
  "MPI_Info_get_nkeys"           , "MPI_Info_get_nthkey"          , 
  "MPI_Info_get_valuelen"        , "MPI_Info_set"                 , 
  "MPI_Pack_external"            , "MPI_Pack_external_size"       , 
  "MPI_Request_get_status"       , "MPI_Type_create_darray"       , 
  "MPI_Type_create_hindexed"     , "MPI_Type_create_hvector"      , 
  "MPI_Type_create_indexed_block", "MPI_Type_create_resized"      , 
  "MPI_Type_create_struct"       , "MPI_Type_create_subarray"     , 
  "MPI_Type_get_extent"          , "MPI_Type_get_true_extent"     , 
  "MPI_Unpack_external"          , "MPI_Win_create_errhandler"    , 
  "MPI_Win_get_errhandler"       , "MPI_Win_set_errhandler"       , 
  "MPI_File_open"                , "MPI_File_close"               , 
  "MPI_File_delete"              , "MPI_File_set_size"            , 
  "MPI_File_preallocate"         , "MPI_File_get_size"            , 
  "MPI_File_get_group"           , "MPI_File_get_amode"           , 
  "MPI_File_set_info"            , "MPI_File_get_info"            , 
  "MPI_File_set_view"            , "MPI_File_get_view"            , 
  "MPI_File_read_at"             , "MPI_File_read_at_all"         , 
  "MPI_File_write_at"            , "MPI_File_write_at_all"        , 
  "MPI_File_iread_at"            , "MPI_File_iwrite_at"           , 
  "MPI_File_read"                , "MPI_File_read_all"            , 
  "MPI_File_write"               , "MPI_File_write_all"           , 
  "MPI_File_iread"               , "MPI_File_iwrite"              , 
  "MPI_File_seek"                , "MPI_File_get_position"        , 
  "MPI_File_get_byte_offset"     , "MPI_File_read_shared"         , 
  "MPI_File_write_shared"        , "MPI_File_iread_shared"        , 
  "MPI_File_iwrite_shared"       , "MPI_File_read_ordered"        , 
  "MPI_File_write_ordered"       , "MPI_File_seek_shared"         , 
  "MPI_File_get_position_shared" , "MPI_File_read_at_all_begin"   , 
  "MPI_File_read_at_all_end"     , "MPI_File_write_at_all_begin"  , 
  "MPI_File_write_at_all_end"    , "MPI_File_read_all_begin"      , 
  "MPI_File_read_all_end"        , "MPI_File_write_all_begin"     , 
  "MPI_File_write_all_end"       , "MPI_File_read_ordered_begin"  , 
  "MPI_File_read_ordered_end"    , "MPI_File_write_ordered_begin" , 
  "MPI_File_write_ordered_end"   , "MPI_File_get_type_extent"     , 
  "MPI_Register_datarep"         , "MPI_File_set_atomicity"       , 
  "MPI_File_get_atomicity"       , "MPI_File_sync"                , 
  "MPIO_Test"                    , "MPIO_Wait"                    , 
  "MPIO_Testall"                 , "MPIO_Waitall"                 , 
  "MPIO_Testany"                 , "MPIO_Waitany"                 , 
  "MPIO_Waitsome"                , "MPIO_Testsome"                , 
  "MPI_ALL_FUNCTIONS"            , 
  "Function_enter"               , "Function_exit"                ,
  NULL  /* sentinel */
};

const char* dumpi_get_function_name(int funcid) {
  if(funcid >= 0 && funcid <= DUMPI_ALL_FUNCTIONS)
    return dumpi_function_names[funcid];
  else
    return "DUMPI_NO_SUCH_FUNCTION";
}