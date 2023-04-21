/**
Copyright 2009-2023 National Technology and Engineering Solutions of Sandia,
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S. Government
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly
owned subsidiary of Honeywell International, Inc., for the U.S. Department of
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2023, NTESS

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

#ifndef DUMPI_COMMON_FUNCLABELS_H
#define DUMPI_COMMON_FUNCLABELS_H

#ifdef __cplusplus
extern "C" {
#endif /* !__cplusplus */
  
  /**
   * \ingroup common
   * \defgroup common_labels common/labels:  Function labels
   */
  /*@{*/

  /**
   * Function labels for every profiled function type.
   * These labels are used to determine what callback routines should be
   * invoked, to keep track of call counts and profiling settings,
   * and a host of other locations in the code
   */
  typedef enum dumpi_function {
    DUMPI_Send=0,                    DUMPI_Recv,                      
    DUMPI_Get_count,                 DUMPI_Bsend,                     
    DUMPI_Ssend,                     DUMPI_Rsend,                     
    DUMPI_Buffer_attach,             DUMPI_Buffer_detach,             
    DUMPI_Isend,                     DUMPI_Ibsend,                    
    DUMPI_Issend,                    DUMPI_Irsend,                    
    DUMPI_Irecv,                     DUMPI_Wait,                      
    DUMPI_Test,                      DUMPI_Request_free,              
    DUMPI_Waitany,                   DUMPI_Testany,                   
    DUMPI_Waitall,                   DUMPI_Testall,                   
    DUMPI_Waitsome,                  DUMPI_Testsome,                  
    DUMPI_Iprobe,                    DUMPI_Probe,                     
    DUMPI_Cancel,                    DUMPI_Test_cancelled,            
    DUMPI_Send_init,                 DUMPI_Bsend_init,                
    DUMPI_Ssend_init,                DUMPI_Rsend_init,                
    DUMPI_Recv_init,                 DUMPI_Start,                     
    DUMPI_Startall,                  DUMPI_Sendrecv,                  
    DUMPI_Sendrecv_replace,          DUMPI_Type_contiguous,           
    DUMPI_Type_vector,               DUMPI_Type_hvector,              
    DUMPI_Type_indexed,              DUMPI_Type_hindexed,             
    DUMPI_Type_struct,               DUMPI_Address,                   
    DUMPI_Type_extent,               DUMPI_Type_size,                 
    DUMPI_Type_lb,                   DUMPI_Type_ub,                   
    DUMPI_Type_commit,               DUMPI_Type_free,                 
    DUMPI_Get_elements,              DUMPI_Pack,                      
    DUMPI_Unpack,                    DUMPI_Pack_size,                 
    DUMPI_Barrier,                   DUMPI_Bcast,                     
    DUMPI_Gather,                    DUMPI_Gatherv,                   
    DUMPI_Scatter,                   DUMPI_Scatterv,                  
    DUMPI_Allgather,                 DUMPI_Allgatherv,                
    DUMPI_Alltoall,                  DUMPI_Alltoallv,                 
    DUMPI_Reduce,                    DUMPI_Op_create,                 
    DUMPI_Op_free,                   DUMPI_Allreduce,                 
    DUMPI_Reduce_scatter,            DUMPI_Scan,                      
    DUMPI_Group_size,                DUMPI_Group_rank,                
    DUMPI_Group_translate_ranks,     DUMPI_Group_compare,             
    DUMPI_Comm_group,                DUMPI_Group_union,               
    DUMPI_Group_intersection,        DUMPI_Group_difference,          
    DUMPI_Group_incl,                DUMPI_Group_excl,                
    DUMPI_Group_range_incl,          DUMPI_Group_range_excl,          
    DUMPI_Group_free,                DUMPI_Comm_size,                 
    DUMPI_Comm_rank,                 DUMPI_Comm_compare,              
    DUMPI_Comm_dup,                  DUMPI_Comm_create,               
    DUMPI_Comm_split,                DUMPI_Comm_free,                 
    DUMPI_Comm_test_inter,           DUMPI_Comm_remote_size,          
    DUMPI_Comm_remote_group,         DUMPI_Intercomm_create,          
    DUMPI_Intercomm_merge,           DUMPI_Keyval_create,             
    DUMPI_Keyval_free,               DUMPI_Attr_put,                  
    DUMPI_Attr_get,                  DUMPI_Attr_delete,               
    DUMPI_Topo_test,                 DUMPI_Cart_create,               
    DUMPI_Dims_create,               DUMPI_Graph_create,              
    DUMPI_Graphdims_get,             DUMPI_Graph_get,                 
    DUMPI_Cartdim_get,               DUMPI_Cart_get,                  
    DUMPI_Cart_rank,                 DUMPI_Cart_coords,               
    DUMPI_Graph_neighbors_count,     DUMPI_Graph_neighbors,           
    DUMPI_Cart_shift,                DUMPI_Cart_sub,                  
    DUMPI_Cart_map,                  DUMPI_Graph_map,                 
    DUMPI_Get_processor_name,        DUMPI_Get_version,               
    DUMPI_Errhandler_create,         DUMPI_Errhandler_set,            
    DUMPI_Errhandler_get,            DUMPI_Errhandler_free,           
    DUMPI_Error_string,              DUMPI_Error_class,               
    DUMPI_Wtime,                     DUMPI_Wtick,                     
    DUMPI_Init,                      DUMPI_Finalize,                  
    DUMPI_Initialized,               DUMPI_Abort,                     
    DUMPI_Pcontrol,                  DUMPI_Close_port,                
    DUMPI_Comm_accept,               DUMPI_Comm_connect,              
    DUMPI_Comm_disconnect,           DUMPI_Comm_get_parent,           
    DUMPI_Comm_join,                 DUMPI_Comm_spawn,                
    DUMPI_Comm_spawn_multiple,       DUMPI_Lookup_name,               
    DUMPI_Open_port,                 DUMPI_Publish_name,              
    DUMPI_Unpublish_name,            DUMPI_Accumulate,                
    DUMPI_Get,                       DUMPI_Put,                       
    DUMPI_Win_complete,              DUMPI_Win_create,                
    DUMPI_Win_fence,                 DUMPI_Win_free,                  
    DUMPI_Win_get_group,             DUMPI_Win_lock,                  
    DUMPI_Win_post,                  DUMPI_Win_start,                 
    DUMPI_Win_test,                  DUMPI_Win_unlock,                
    DUMPI_Win_wait,                  DUMPI_Alltoallw,                 
    DUMPI_Exscan,                    DUMPI_Add_error_class,           
    DUMPI_Add_error_code,            DUMPI_Add_error_string,          
    DUMPI_Comm_call_errhandler,      DUMPI_Comm_create_keyval,        
    DUMPI_Comm_delete_attr,          DUMPI_Comm_free_keyval,          
    DUMPI_Comm_get_attr,             DUMPI_Comm_get_name,             
    DUMPI_Comm_set_attr,             DUMPI_Comm_set_name,             
    DUMPI_File_call_errhandler,      DUMPI_Grequest_complete,         
    DUMPI_Grequest_start,            DUMPI_Init_thread,               
    DUMPI_Is_thread_main,            DUMPI_Query_thread,              
    DUMPI_Status_set_cancelled,      DUMPI_Status_set_elements,       
    DUMPI_Type_create_keyval,        DUMPI_Type_delete_attr,          
    DUMPI_Type_dup,                  DUMPI_Type_free_keyval,          
    DUMPI_Type_get_attr,             DUMPI_Type_get_contents,         
    DUMPI_Type_get_envelope,         DUMPI_Type_get_name,             
    DUMPI_Type_set_attr,             DUMPI_Type_set_name,             
    DUMPI_Type_match_size,           DUMPI_Win_call_errhandler,       
    DUMPI_Win_create_keyval,         DUMPI_Win_delete_attr,           
    DUMPI_Win_free_keyval,           DUMPI_Win_get_attr,              
    DUMPI_Win_get_name,              DUMPI_Win_set_attr,              
    DUMPI_Win_set_name,              DUMPI_Alloc_mem,                 
    DUMPI_Comm_create_errhandler,    DUMPI_Comm_get_errhandler,       
    DUMPI_Comm_set_errhandler,       DUMPI_File_create_errhandler,    
    DUMPI_File_get_errhandler,       DUMPI_File_set_errhandler,       
    DUMPI_Finalized,                 DUMPI_Free_mem,                  
    DUMPI_Get_address,               DUMPI_Info_create,               
    DUMPI_Info_delete,               DUMPI_Info_dup,                  
    DUMPI_Info_free,                 DUMPI_Info_get,                  
    DUMPI_Info_get_nkeys,            DUMPI_Info_get_nthkey,           
    DUMPI_Info_get_valuelen,         DUMPI_Info_set,                  
    DUMPI_Pack_external,             DUMPI_Pack_external_size,        
    DUMPI_Request_get_status,        DUMPI_Type_create_darray,        
    DUMPI_Type_create_hindexed,      DUMPI_Type_create_hvector,       
    DUMPI_Type_create_indexed_block, DUMPI_Type_create_resized,       
    DUMPI_Type_create_struct,        DUMPI_Type_create_subarray,      
    DUMPI_Type_get_extent,           DUMPI_Type_get_true_extent,      
    DUMPI_Unpack_external,           DUMPI_Win_create_errhandler,     
    DUMPI_Win_get_errhandler,        DUMPI_Win_set_errhandler,        
    DUMPI_File_open,                 DUMPI_File_close,                
    DUMPI_File_delete,               DUMPI_File_set_size,             
    DUMPI_File_preallocate,          DUMPI_File_get_size,             
    DUMPI_File_get_group,            DUMPI_File_get_amode,            
    DUMPI_File_set_info,             DUMPI_File_get_info,             
    DUMPI_File_set_view,             DUMPI_File_get_view,             
    DUMPI_File_read_at,              DUMPI_File_read_at_all,          
    DUMPI_File_write_at,             DUMPI_File_write_at_all,         
    DUMPI_File_iread_at,             DUMPI_File_iwrite_at,            
    DUMPI_File_read,                 DUMPI_File_read_all,             
    DUMPI_File_write,                DUMPI_File_write_all,            
    DUMPI_File_iread,                DUMPI_File_iwrite,               
    DUMPI_File_seek,                 DUMPI_File_get_position,         
    DUMPI_File_get_byte_offset,      DUMPI_File_read_shared,          
    DUMPI_File_write_shared,         DUMPI_File_iread_shared,         
    DUMPI_File_iwrite_shared,        DUMPI_File_read_ordered,         
    DUMPI_File_write_ordered,        DUMPI_File_seek_shared,          
    DUMPI_File_get_position_shared,  DUMPI_File_read_at_all_begin,    
    DUMPI_File_read_at_all_end,      DUMPI_File_write_at_all_begin,   
    DUMPI_File_write_at_all_end,     DUMPI_File_read_all_begin,       
    DUMPI_File_read_all_end,         DUMPI_File_write_all_begin,      
    DUMPI_File_write_all_end,        DUMPI_File_read_ordered_begin,   
    DUMPI_File_read_ordered_end,     DUMPI_File_write_ordered_begin,  
    DUMPI_File_write_ordered_end,    DUMPI_File_get_type_extent,      
    DUMPI_Register_datarep,          DUMPI_File_set_atomicity,        
    DUMPI_File_get_atomicity,        DUMPI_File_sync,                 
    DUMPIO_Test,                     DUMPIO_Wait,                     
    DUMPIO_Testall,                  DUMPIO_Waitall,                  
    DUMPIO_Testany,                  DUMPIO_Waitany,                  
    DUMPIO_Waitsome,                 DUMPIO_Testsome,                 
    DUMPI_ALL_FUNCTIONS,  /* Sentinel to mark last MPI function */
    /* Special lables to indicate profiled functions. */
    DUMPI_Function_enter,            DUMPI_Function_exit,
    DUMPI_END_OF_STREAM  /* Sentinel to mark end of trace stream */
  } dumpi_function;

  /** Get the std::string name of a dumpi_function enum. */
  const char *dumpi_function_label(dumpi_function func);

  /*@}*/

#ifdef __cplusplus
} /* end of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_FUNCLABELS_H */
