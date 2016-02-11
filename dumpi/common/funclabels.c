/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
 */

#include <dumpi/common/funclabels.h>
#include <assert.h>
#include <stdio.h>

#define DUMPI_CR(LABEL) case DU ## LABEL: return #LABEL; break

const char *dumpi_function_label(dumpi_function func) {
  switch(func) {
    DUMPI_CR(MPI_Send);                      DUMPI_CR(MPI_Recv);                      
    DUMPI_CR(MPI_Get_count);                 DUMPI_CR(MPI_Bsend);                     
    DUMPI_CR(MPI_Ssend);                     DUMPI_CR(MPI_Rsend);                     
    DUMPI_CR(MPI_Buffer_attach);             DUMPI_CR(MPI_Buffer_detach);             
    DUMPI_CR(MPI_Isend);                     DUMPI_CR(MPI_Ibsend);                    
    DUMPI_CR(MPI_Issend);                    DUMPI_CR(MPI_Irsend);                    
    DUMPI_CR(MPI_Irecv);                     DUMPI_CR(MPI_Wait);                      
    DUMPI_CR(MPI_Test);                      DUMPI_CR(MPI_Request_free);              
    DUMPI_CR(MPI_Waitany);                   DUMPI_CR(MPI_Testany);                   
    DUMPI_CR(MPI_Waitall);                   DUMPI_CR(MPI_Testall);                   
    DUMPI_CR(MPI_Waitsome);                  DUMPI_CR(MPI_Testsome);                  
    DUMPI_CR(MPI_Iprobe);                    DUMPI_CR(MPI_Probe);                     
    DUMPI_CR(MPI_Cancel);                    DUMPI_CR(MPI_Test_cancelled);            
    DUMPI_CR(MPI_Send_init);                 DUMPI_CR(MPI_Bsend_init);                
    DUMPI_CR(MPI_Ssend_init);                DUMPI_CR(MPI_Rsend_init);                
    DUMPI_CR(MPI_Recv_init);                 DUMPI_CR(MPI_Start);                     
    DUMPI_CR(MPI_Startall);                  DUMPI_CR(MPI_Sendrecv);                  
    DUMPI_CR(MPI_Sendrecv_replace);          DUMPI_CR(MPI_Type_contiguous);           
    DUMPI_CR(MPI_Type_vector);               DUMPI_CR(MPI_Type_hvector);              
    DUMPI_CR(MPI_Type_indexed);              DUMPI_CR(MPI_Type_hindexed);             
    DUMPI_CR(MPI_Type_struct);               DUMPI_CR(MPI_Address);                   
    DUMPI_CR(MPI_Type_extent);               DUMPI_CR(MPI_Type_size);                 
    DUMPI_CR(MPI_Type_lb);                   DUMPI_CR(MPI_Type_ub);                   
    DUMPI_CR(MPI_Type_commit);               DUMPI_CR(MPI_Type_free);                 
    DUMPI_CR(MPI_Get_elements);              DUMPI_CR(MPI_Pack);                      
    DUMPI_CR(MPI_Unpack);                    DUMPI_CR(MPI_Pack_size);                 
    DUMPI_CR(MPI_Barrier);                   DUMPI_CR(MPI_Bcast);                     
    DUMPI_CR(MPI_Gather);                    DUMPI_CR(MPI_Gatherv);                   
    DUMPI_CR(MPI_Scatter);                   DUMPI_CR(MPI_Scatterv);                  
    DUMPI_CR(MPI_Allgather);                 DUMPI_CR(MPI_Allgatherv);                
    DUMPI_CR(MPI_Alltoall);                  DUMPI_CR(MPI_Alltoallv);                 
    DUMPI_CR(MPI_Reduce);                    DUMPI_CR(MPI_Op_create);                 
    DUMPI_CR(MPI_Op_free);                   DUMPI_CR(MPI_Allreduce);                 
    DUMPI_CR(MPI_Reduce_scatter);            DUMPI_CR(MPI_Scan);                      
    DUMPI_CR(MPI_Group_size);                DUMPI_CR(MPI_Group_rank);                
    DUMPI_CR(MPI_Group_translate_ranks);     DUMPI_CR(MPI_Group_compare);             
    DUMPI_CR(MPI_Comm_group);                DUMPI_CR(MPI_Group_union);               
    DUMPI_CR(MPI_Group_intersection);        DUMPI_CR(MPI_Group_difference);          
    DUMPI_CR(MPI_Group_incl);                DUMPI_CR(MPI_Group_excl);                
    DUMPI_CR(MPI_Group_range_incl);          DUMPI_CR(MPI_Group_range_excl);          
    DUMPI_CR(MPI_Group_free);                DUMPI_CR(MPI_Comm_size);                 
    DUMPI_CR(MPI_Comm_rank);                 DUMPI_CR(MPI_Comm_compare);              
    DUMPI_CR(MPI_Comm_dup);                  DUMPI_CR(MPI_Comm_create);               
    DUMPI_CR(MPI_Comm_split);                DUMPI_CR(MPI_Comm_free);                 
    DUMPI_CR(MPI_Comm_test_inter);           DUMPI_CR(MPI_Comm_remote_size);          
    DUMPI_CR(MPI_Comm_remote_group);         DUMPI_CR(MPI_Intercomm_create);          
    DUMPI_CR(MPI_Intercomm_merge);           DUMPI_CR(MPI_Keyval_create);             
    DUMPI_CR(MPI_Keyval_free);               DUMPI_CR(MPI_Attr_put);                  
    DUMPI_CR(MPI_Attr_get);                  DUMPI_CR(MPI_Attr_delete);               
    DUMPI_CR(MPI_Topo_test);                 DUMPI_CR(MPI_Cart_create);               
    DUMPI_CR(MPI_Dims_create);               DUMPI_CR(MPI_Graph_create);              
    DUMPI_CR(MPI_Graphdims_get);             DUMPI_CR(MPI_Graph_get);                 
    DUMPI_CR(MPI_Cartdim_get);               DUMPI_CR(MPI_Cart_get);                  
    DUMPI_CR(MPI_Cart_rank);                 DUMPI_CR(MPI_Cart_coords);               
    DUMPI_CR(MPI_Graph_neighbors_count);     DUMPI_CR(MPI_Graph_neighbors);           
    DUMPI_CR(MPI_Cart_shift);                DUMPI_CR(MPI_Cart_sub);                  
    DUMPI_CR(MPI_Cart_map);                  DUMPI_CR(MPI_Graph_map);                 
    DUMPI_CR(MPI_Get_processor_name);        DUMPI_CR(MPI_Get_version);               
    DUMPI_CR(MPI_Errhandler_create);         DUMPI_CR(MPI_Errhandler_set);            
    DUMPI_CR(MPI_Errhandler_get);            DUMPI_CR(MPI_Errhandler_free);           
    DUMPI_CR(MPI_Error_string);              DUMPI_CR(MPI_Error_class);               
    DUMPI_CR(MPI_Wtime);                     DUMPI_CR(MPI_Wtick);                     
    DUMPI_CR(MPI_Init);                      DUMPI_CR(MPI_Finalize);                  
    DUMPI_CR(MPI_Initialized);               DUMPI_CR(MPI_Abort);                     
    DUMPI_CR(MPI_Pcontrol);                  DUMPI_CR(MPI_Close_port);                
    DUMPI_CR(MPI_Comm_accept);               DUMPI_CR(MPI_Comm_connect);              
    DUMPI_CR(MPI_Comm_disconnect);           DUMPI_CR(MPI_Comm_get_parent);           
    DUMPI_CR(MPI_Comm_join);                 DUMPI_CR(MPI_Comm_spawn);                
    DUMPI_CR(MPI_Comm_spawn_multiple);       DUMPI_CR(MPI_Lookup_name);               
    DUMPI_CR(MPI_Open_port);                 DUMPI_CR(MPI_Publish_name);              
    DUMPI_CR(MPI_Unpublish_name);            DUMPI_CR(MPI_Accumulate);                
    DUMPI_CR(MPI_Get);                       DUMPI_CR(MPI_Put);                       
    DUMPI_CR(MPI_Win_complete);              DUMPI_CR(MPI_Win_create);                
    DUMPI_CR(MPI_Win_fence);                 DUMPI_CR(MPI_Win_free);                  
    DUMPI_CR(MPI_Win_get_group);             DUMPI_CR(MPI_Win_lock);                  
    DUMPI_CR(MPI_Win_post);                  DUMPI_CR(MPI_Win_start);                 
    DUMPI_CR(MPI_Win_test);                  DUMPI_CR(MPI_Win_unlock);                
    DUMPI_CR(MPI_Win_wait);                  DUMPI_CR(MPI_Alltoallw);                 
    DUMPI_CR(MPI_Exscan);                    DUMPI_CR(MPI_Add_error_class);           
    DUMPI_CR(MPI_Add_error_code);            DUMPI_CR(MPI_Add_error_string);          
    DUMPI_CR(MPI_Comm_call_errhandler);      DUMPI_CR(MPI_Comm_create_keyval);        
    DUMPI_CR(MPI_Comm_delete_attr);          DUMPI_CR(MPI_Comm_free_keyval);          
    DUMPI_CR(MPI_Comm_get_attr);             DUMPI_CR(MPI_Comm_get_name);             
    DUMPI_CR(MPI_Comm_set_attr);             DUMPI_CR(MPI_Comm_set_name);             
    DUMPI_CR(MPI_File_call_errhandler);      DUMPI_CR(MPI_Grequest_complete);         
    DUMPI_CR(MPI_Grequest_start);            DUMPI_CR(MPI_Init_thread);               
    DUMPI_CR(MPI_Is_thread_main);            DUMPI_CR(MPI_Query_thread);              
    DUMPI_CR(MPI_Status_set_cancelled);      DUMPI_CR(MPI_Status_set_elements);       
    DUMPI_CR(MPI_Type_create_keyval);        DUMPI_CR(MPI_Type_delete_attr);          
    DUMPI_CR(MPI_Type_dup);                  DUMPI_CR(MPI_Type_free_keyval);          
    DUMPI_CR(MPI_Type_get_attr);             DUMPI_CR(MPI_Type_get_contents);         
    DUMPI_CR(MPI_Type_get_envelope);         DUMPI_CR(MPI_Type_get_name);             
    DUMPI_CR(MPI_Type_set_attr);             DUMPI_CR(MPI_Type_set_name);             
    DUMPI_CR(MPI_Type_match_size);           DUMPI_CR(MPI_Win_call_errhandler);       
    DUMPI_CR(MPI_Win_create_keyval);         DUMPI_CR(MPI_Win_delete_attr);           
    DUMPI_CR(MPI_Win_free_keyval);           DUMPI_CR(MPI_Win_get_attr);              
    DUMPI_CR(MPI_Win_get_name);              DUMPI_CR(MPI_Win_set_attr);              
    DUMPI_CR(MPI_Win_set_name);              DUMPI_CR(MPI_Alloc_mem);                 
    DUMPI_CR(MPI_Comm_create_errhandler);    DUMPI_CR(MPI_Comm_get_errhandler);       
    DUMPI_CR(MPI_Comm_set_errhandler);       DUMPI_CR(MPI_File_create_errhandler);    
    DUMPI_CR(MPI_File_get_errhandler);       DUMPI_CR(MPI_File_set_errhandler);       
    DUMPI_CR(MPI_Finalized);                 DUMPI_CR(MPI_Free_mem);                  
    DUMPI_CR(MPI_Get_address);               DUMPI_CR(MPI_Info_create);               
    DUMPI_CR(MPI_Info_delete);               DUMPI_CR(MPI_Info_dup);                  
    DUMPI_CR(MPI_Info_free);                 DUMPI_CR(MPI_Info_get);                  
    DUMPI_CR(MPI_Info_get_nkeys);            DUMPI_CR(MPI_Info_get_nthkey);           
    DUMPI_CR(MPI_Info_get_valuelen);         DUMPI_CR(MPI_Info_set);                  
    DUMPI_CR(MPI_Pack_external);             DUMPI_CR(MPI_Pack_external_size);        
    DUMPI_CR(MPI_Request_get_status);        DUMPI_CR(MPI_Type_create_darray);        
    DUMPI_CR(MPI_Type_create_hindexed);      DUMPI_CR(MPI_Type_create_hvector);       
    DUMPI_CR(MPI_Type_create_indexed_block); DUMPI_CR(MPI_Type_create_resized);       
    DUMPI_CR(MPI_Type_create_struct);        DUMPI_CR(MPI_Type_create_subarray);      
    DUMPI_CR(MPI_Type_get_extent);           DUMPI_CR(MPI_Type_get_true_extent);      
    DUMPI_CR(MPI_Unpack_external);           DUMPI_CR(MPI_Win_create_errhandler);     
    DUMPI_CR(MPI_Win_get_errhandler);        DUMPI_CR(MPI_Win_set_errhandler);        
    DUMPI_CR(MPI_File_open);                 DUMPI_CR(MPI_File_close);                
    DUMPI_CR(MPI_File_delete);               DUMPI_CR(MPI_File_set_size);             
    DUMPI_CR(MPI_File_preallocate);          DUMPI_CR(MPI_File_get_size);             
    DUMPI_CR(MPI_File_get_group);            DUMPI_CR(MPI_File_get_amode);            
    DUMPI_CR(MPI_File_set_info);             DUMPI_CR(MPI_File_get_info);             
    DUMPI_CR(MPI_File_set_view);             DUMPI_CR(MPI_File_get_view);             
    DUMPI_CR(MPI_File_read_at);              DUMPI_CR(MPI_File_read_at_all);          
    DUMPI_CR(MPI_File_write_at);             DUMPI_CR(MPI_File_write_at_all);         
    DUMPI_CR(MPI_File_iread_at);             DUMPI_CR(MPI_File_iwrite_at);            
    DUMPI_CR(MPI_File_read);                 DUMPI_CR(MPI_File_read_all);             
    DUMPI_CR(MPI_File_write);                DUMPI_CR(MPI_File_write_all);            
    DUMPI_CR(MPI_File_iread);                DUMPI_CR(MPI_File_iwrite);               
    DUMPI_CR(MPI_File_seek);                 DUMPI_CR(MPI_File_get_position);         
    DUMPI_CR(MPI_File_get_byte_offset);      DUMPI_CR(MPI_File_read_shared);          
    DUMPI_CR(MPI_File_write_shared);         DUMPI_CR(MPI_File_iread_shared);         
    DUMPI_CR(MPI_File_iwrite_shared);        DUMPI_CR(MPI_File_read_ordered);         
    DUMPI_CR(MPI_File_write_ordered);        DUMPI_CR(MPI_File_seek_shared);          
    DUMPI_CR(MPI_File_get_position_shared);  DUMPI_CR(MPI_File_read_at_all_begin);    
    DUMPI_CR(MPI_File_read_at_all_end);      DUMPI_CR(MPI_File_write_at_all_begin);   
    DUMPI_CR(MPI_File_write_at_all_end);     DUMPI_CR(MPI_File_read_all_begin);       
    DUMPI_CR(MPI_File_read_all_end);         DUMPI_CR(MPI_File_write_all_begin);      
    DUMPI_CR(MPI_File_write_all_end);        DUMPI_CR(MPI_File_read_ordered_begin);   
    DUMPI_CR(MPI_File_read_ordered_end);     DUMPI_CR(MPI_File_write_ordered_begin);  
    DUMPI_CR(MPI_File_write_ordered_end);    DUMPI_CR(MPI_File_get_type_extent);      
    DUMPI_CR(MPI_Register_datarep);          DUMPI_CR(MPI_File_set_atomicity);        
    DUMPI_CR(MPI_File_get_atomicity);        DUMPI_CR(MPI_File_sync);                 
    DUMPI_CR(MPIO_Test);                     DUMPI_CR(MPIO_Wait);                     
    DUMPI_CR(MPIO_Testall);                  DUMPI_CR(MPIO_Waitall);                  
    DUMPI_CR(MPIO_Testany);                  DUMPI_CR(MPIO_Waitany);                  
    DUMPI_CR(MPIO_Waitsome);                 DUMPI_CR(MPIO_Testsome);                 
    DUMPI_CR(MPI_ALL_FUNCTIONS);  /* Sentinel to mark last MPI function */
    /* Special lables to indicate profiled functions. */
    DUMPI_CR(MPI_Function_enter);            DUMPI_CR(MPI_Function_exit);
    DUMPI_CR(MPI_END_OF_STREAM);  /* Sentinel to mark end of trace stream */
  default:
    fprintf(stderr, "dumpi_function_label:  error:  unknown function label\n");
    assert(0);
  }
  return NULL; /* This is unreachable, but some compilers complain anyway */
}
