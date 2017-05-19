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

int MPI_Send(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm);
int MPI_Recv(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN SOURCE source, IN TAG tag, IN MPI_Comm comm, OUT MPI_Status *status);
int MPI_Get_count(IN MPI_Status *status, IN MPI_Datatype datatype, OUT int *count);
int MPI_Bsend(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm);
int MPI_Ssend(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm);
int MPI_Rsend(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm);
int MPI_Buffer_attach(SKIP void *buf, IN int size);
int MPI_Buffer_detach(SKIP void *buf, OUT int *size);
int MPI_Isend(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN int tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Ibsend(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Issend(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Irsend(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Irecv(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN SOURCE source, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Wait(IN MPI_Request *request, OUT MPI_Status *status);
int MPI_Test(IN MPI_Request *request, OUT int *flag, OUT MPI_Status *status {COND(*flag != 0)});
int MPI_Request_free(IN MPI_Request *request);
int MPI_Waitany(IN int count, IN MPI_Request *requests[count], OUT int *index, OUT MPI_Status *status {COND(*index != MPI_UNDEFINED)});
int MPI_Testany(IN int count, IN MPI_Request *requests[count], OUT int *index, OUT int *flag, OUT MPI_Status *status {COND(*flag != 0)});
int MPI_Waitall(IN int count, IN MPI_Request *requests[count], OUT MPI_Status *statuses[count]);
int MPI_Testall(IN int count, IN MPI_Request *requests[count], OUT int *flag, OUT MPI_Status *statuses[count] {COND(*flag != 0)});
int MPI_Waitsome(IN int count, IN MPI_Request *requests[count], OUT int *outcount, OUT int *indices[*outcount], OUT MPI_Status *statuses[*outcount]);
int MPI_Testsome(IN int count, IN MPI_Request *requests[count], OUT int *outcount, OUT int *indices[*outcount], OUT MPI_Status *statuses[*outcount]);
int MPI_Iprobe(IN SOURCE source, IN TAG tag, IN MPI_Comm comm, OUT int *flag, OUT MPI_Status *status {COND(*flag != 0)});
int MPI_Probe(IN SOURCE source, IN TAG tag, IN MPI_Comm comm, OUT MPI_Status *status);
int MPI_Cancel(IN MPI_Request *request);
int MPI_Test_cancelled(IN MPI_Status *status, OUT int *cancelled);
int MPI_Send_init(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Bsend_init(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Ssend_init(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Rsend_init(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Recv_init(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN SOURCE source, IN TAG tag, IN MPI_Comm comm, OUT MPI_Request *request);
int MPI_Start(OUT MPI_Request *request);
int MPI_Startall(IN int count, OUT MPI_Request *requests[count]);
int MPI_Sendrecv(SKIP void *sendbuf, IN int sendcount, IN MPI_Datatype sendtype, IN DEST dest, IN TAG sendtag, SKIP void *recvbuf, IN int recvcount, IN MPI_Datatype recvtype, IN SOURCE source, IN TAG recvtag, IN MPI_Comm comm, OUT MPI_Status *status);
int MPI_Sendrecv_replace(SKIP void *buf, IN int count, IN MPI_Datatype datatype, IN DEST dest, IN TAG sendtag, IN SOURCE source, IN TAG recvtag, IN MPI_Comm comm, OUT MPI_Status *status);
int MPI_Type_contiguous(IN int count, IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_vector(IN int count, IN int blocklength, IN int stride, IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_hvector(IN int count, IN int blocklength, IN MPI_Aint stride, IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_indexed(IN int count, IN int *lengths[count], IN int *indices[count], IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_hindexed(IN int count, IN int *lengths[count], IN MPI_Aint *indices[count], IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_struct(IN int count, IN int *lengths[count], IN MPI_Aint *indices, IN MPI_Datatype *oldtypes[count], OUT MPI_Datatype *newtype);
int MPI_Address(SKIP void *buf, OUT MPI_Aint *address);
int MPI_Type_extent(IN MPI_Datatype datatype, OUT MPI_Aint *extent);
int MPI_Type_size(IN MPI_Datatype datatype, OUT int *size);
int MPI_Type_lb(IN MPI_Datatype datatype, OUT MPI_Aint *lb);
int MPI_Type_ub(IN MPI_Datatype datatype, OUT MPI_Aint *ub);
int MPI_Type_commit(IN MPI_Datatype *datatype);
int MPI_Type_free(IN MPI_Datatype *datatype);
int MPI_Get_elements(IN MPI_Status *status, IN MPI_Datatype datatype, OUT int *elements);
int MPI_Pack(SKIP void *inbuf, IN int incount, IN MPI_Datatype datatype, SKIP void *outbuf, IN int outcount, INOUT int *position,  IN MPI_Comm comm);
int MPI_Unpack(SKIP void *inbuf, IN int incount, INOUT int *position, SKIP void *outbuf, IN int outcount, IN MPI_Datatype datatype, IN MPI_Comm comm);
int MPI_Pack_size(IN int incount, IN MPI_Datatype datatype, IN MPI_Comm comm, OUT int *size);
int MPI_Barrier(IN MPI_Comm comm);
int MPI_Bcast(SKIP void *sendbuf, IN int count, IN MPI_Datatype datatype, IN int root, IN MPI_Comm comm);
int MPI_Gather(SKIP void *inbuf, IN int sendcount, IN MPI_Datatype sendtype, SKIP void *recvbuf, IN int recvcount {COND(commrank==root)}, IN MPI_Datatype recvtype {COND(commrank==root)}, IN int root, IN MPI_Comm comm); EXTRA({DECLARE ADD int commrank;}{PREAMBLE PMPI_Comm_rank(comm, &commrank);});
int MPI_Gatherv(SKIP void *sendbuf, IN int sendcount, IN MPI_Datatype sendtype, SKIP void *recvbuf, IN int *recvcounts[commsize] {COND(commrank==root)}, IN int *displs[commsize] {COND(commrank==root)}, IN MPI_Datatype recvtype {COND(commrank==root)}, IN int root, IN MPI_Comm comm); EXTRA({DECLARE ADD int commsize; ADD int commrank;}{PREAMBLE PMPI_Comm_size(comm, &commsize); PMPI_Comm_rank(comm, &commrank);});
int MPI_Scatter(SKIP void *sendbuf, IN int sendcount {COND(commrank==root)}, IN MPI_Datatype sendtype {COND(commrank==root)}, SKIP void *recvbuf, IN int recvcount, IN MPI_Datatype recvtype, IN int root, IN MPI_Comm comm); EXTRA({DECLARE ADD int commrank;}{PREAMBLE PMPI_Comm_rank(comm, &commrank);});
int MPI_Scatterv(SKIP void *sendbuf , IN int *sendcounts[commsize] {COND(commrank==root)}, IN int *displs[commsize] {COND(commrank==root)},  IN MPI_Datatype sendtype, SKIP void *recvbuf, IN int recvcount, IN MPI_Datatype recvtype, IN int root, IN MPI_Comm comm); EXTRA({DECLARE ADD int commsize; ADD int commrank;}{PREAMBLE PMPI_Comm_size(comm, &commsize); PMPI_Comm_rank(comm, &commrank);});
int MPI_Allgather(SKIP void *sendbuf , IN int sendcount, IN MPI_Datatype sendtype, SKIP void *recvbuf, IN int recvcount, IN MPI_Datatype recvtype, IN MPI_Comm comm);
int MPI_Allgatherv(SKIP void *sendbuf , IN int sendcount, IN MPI_Datatype sendtype, SKIP void *recvbuf, IN int *recvcounts[commsize], IN int *displs[commsize], IN MPI_Datatype recvtype, IN MPI_Comm comm); EXTRA({DECLARE ADD int commsize;}{PREAMBLE PMPI_Comm_size(comm, &commsize);});
int MPI_Alltoall(SKIP void *sendbuf , IN int sendcount, IN MPI_Datatype sendtype, SKIP void *recvbuf, IN int recvcount, IN MPI_Datatype recvtype, IN MPI_Comm comm);
int MPI_Alltoallv(SKIP void *sendbuf, IN int *sendcounts[commsize], IN int *senddispls[commsize], IN MPI_Datatype sendtype, SKIP void *recvbuf, IN int *recvcounts[commsize], IN int *recvdispls[commsize], IN MPI_Datatype recvtype, IN MPI_Comm comm); EXTRA({DECLARE ADD int commsize;}{PREAMBLE PMPI_Comm_size(comm, &commsize);});
int MPI_Reduce(SKIP void *sendbuf, SKIP void *recvbuf, IN int count, IN MPI_Datatype datatype, IN MPI_Op op, IN int root, IN MPI_Comm comm);
int MPI_Op_create(SKIP MPI_User_function *fptr, IN int commute, OUT MPI_Op *op);
int MPI_Op_free(IN MPI_Op *op);
int MPI_Allreduce(SKIP void *sendbuf, SKIP void *recvbuf, IN int count, IN MPI_Datatype datatype, IN MPI_Op op, IN MPI_Comm comm);
int MPI_Reduce_scatter(SKIP void *sendbuf, SKIP void *recvbuf, IN int *recvcounts[commsize], IN MPI_Datatype datatype, IN MPI_Op op, IN MPI_Comm comm); EXTRA({DECLARE ADD int commsize;}{PREAMBLE PMPI_Comm_size(comm, &commsize);});
int MPI_Scan(SKIP void *sendbuf, SKIP void *recvbuf, IN int count, IN MPI_Datatype datatype, IN MPI_Op op, IN MPI_Comm comm);
int MPI_Group_size(IN MPI_Group group, OUT int *size);
int MPI_Group_rank(IN MPI_Group group, OUT int *rank);
int MPI_Group_translate_ranks(IN MPI_Group group1, IN int count, IN int *ranks1[count], IN MPI_Group group2, OUT int *ranks2[count]);
int MPI_Group_compare(IN MPI_Group group1, IN MPI_Group group2, OUT COMPARISON *result);
int MPI_Comm_group(IN MPI_Comm comm, OUT MPI_Group *group);
int MPI_Group_union(IN MPI_Group group1, IN MPI_Group group2, OUT MPI_Group *newgroup);
int MPI_Group_intersection(IN MPI_Group group1, IN MPI_Group group2, OUT MPI_Group *newgroup);
int MPI_Group_difference(IN MPI_Group group1, IN MPI_Group group2, OUT MPI_Group *newgroup);
int MPI_Group_incl(IN MPI_Group group, IN int count, IN int *ranks[count], OUT MPI_Group *newgroup);
int MPI_Group_excl(IN MPI_Group group, IN int count, IN int *ranks[count], OUT MPI_Group *newgroup);
int MPI_Group_range_incl(IN MPI_Group group, IN int count, IN int **ranges[count][3], OUT MPI_Group *newgroup);
int MPI_Group_range_excl(IN MPI_Group group, IN int count, IN int **ranges[count][3], OUT MPI_Group *newgroup);
int MPI_Group_free(IN MPI_Group *group);
int MPI_Comm_size(IN MPI_Comm comm, OUT int *size);
int MPI_Comm_rank(IN MPI_Comm comm, OUT int *rank);
int MPI_Comm_compare(IN MPI_Comm comm1, IN MPI_Comm comm2, OUT COMPARISON *result);
int MPI_Comm_dup(IN MPI_Comm oldcomm, OUT MPI_Comm *newcomm);
int MPI_Comm_create(IN MPI_Comm oldcomm, IN MPI_Group group, OUT MPI_Comm *newcomm);
int MPI_Comm_split(IN MPI_Comm oldcomm, IN int color, IN int key, OUT MPI_Comm *newcomm);
int MPI_Comm_free(IN MPI_Comm *comm);
int MPI_Comm_test_inter(IN MPI_Comm comm, OUT int *inter);
int MPI_Comm_remote_size(IN MPI_Comm comm, OUT int *size);
int MPI_Comm_remote_group(IN MPI_Comm comm, OUT MPI_Group *group);
int MPI_Intercomm_create(IN MPI_Comm localcomm, IN int localleader, IN MPI_Comm remotecomm, IN int remoteleader, IN TAG tag, OUT MPI_Comm *newcomm);
int MPI_Intercomm_merge(IN MPI_Comm comm, IN int high, OUT MPI_Comm *newcomm);
int MPI_Keyval_create(SKIP MPI_Copy_function *copyfunc, SKIP MPI_Delete_function *delfunc, OUT KEYVAL *key, SKIP void *state);
int MPI_Keyval_free(IN KEYVAL *key);
int MPI_Attr_put(IN MPI_Comm comm, IN int key, SKIP void *attr);
int MPI_Attr_get(IN MPI_Comm comm, IN int key, SKIP void *attr, OUT int *flag);
int MPI_Attr_delete(IN MPI_Comm comm, IN int key);
int MPI_Topo_test(IN MPI_Comm comm, OUT TOPOLOGY *topo);
int MPI_Cart_create(IN MPI_Comm oldcomm, IN int ndim, IN int *dims[ndim], IN int *periods[ndim], IN int reorder, OUT MPI_Comm *newcomm);
int MPI_Dims_create(IN int nodes, IN int ndim, INOUT int *dims[ndim]);
int MPI_Graph_create(IN MPI_Comm oldcomm, IN int nodes, IN int *index[nodes], IN int *edges[numedges], IN int reorder, OUT MPI_Comm *newcomm); EXTRA({DECLARE ADD int numedges;}{numedges=index[nodes-1];});
int MPI_Graphdims_get(IN MPI_Comm comm, OUT int *nodes, OUT int *edges);
int MPI_Graph_get(IN MPI_Comm comm, IN int maxindex, IN int maxedges, OUT int *index[totnodes], OUT int *edges[totedges]); EXTRA({DECLARE ADD int totnodes; ADD int totedges;} {PREAMBLE PMPI_Graphdims_get(comm, &totnodes, &totedges);});
int MPI_Cartdim_get(IN MPI_Comm comm, OUT int *ndim);
int MPI_Cart_get(IN MPI_Comm comm, IN int maxdims, OUT int *dims[ndim], OUT int *periods[ndim], OUT int *coords); EXTRA({DECLARE ADD int ndim;} {PREAMBLE PMPI_Cartdim_get(comm, &ndim);});
int MPI_Cart_rank(IN MPI_Comm comm, IN int *coords[ndim], OUT int *rank); EXTRA({DECLARE ADD int ndim;} {PREAMBLE PMPI_Cartdim_get(comm, &ndim);});
int MPI_Cart_coords(IN MPI_Comm comm, IN int rank, IN int maxdims, OUT int *coords[ndim]); EXTRA({DECLARE ADD int ndim;} {PREAMBLE PMPI_Cartdim_get(comm, &ndim);});
int MPI_Graph_neighbors_count(IN MPI_Comm comm, IN int rank, OUT int *nneigh);
int MPI_Graph_neighbors(IN MPI_Comm comm, IN int rank, IN int maxneighbors, OUT int *neighbors[nneigh]); EXTRA({DECLARE ADD int nneigh;} {PREAMBLE PMPI_Graph_neighbors_count(comm, rank, &nneigh);});
int MPI_Cart_shift(IN MPI_Comm comm, IN int direction, IN int displ, OUT SOURCE *source, OUT DEST *dest);
int MPI_Cart_sub(IN MPI_Comm oldcomm, IN int *remain_dims[ndim], OUT MPI_Comm *newcomm); EXTRA({DECLARE ADD int ndim;} {PREAMBLE PMPI_Cartdim_get(oldcomm, &ndim);});
int MPI_Cart_map(IN MPI_Comm comm, IN int ndim, IN int *dims[ndim], IN int *period[ndim], OUT int *newrank);
int MPI_Graph_map(IN MPI_Comm comm, IN int nodes, IN int *index[nodes], IN int *edges[numedges], OUT int *newrank); EXTRA({DECLARE ADD int numedges;}{numedges=index[nodes-1];});
int MPI_Get_processor_name(OUT char *name[*resultlen], OUT int *resultlen);
int MPI_Get_version(OUT int *version, OUT int *subversion);
int MPI_Errhandler_create(SKIP MPI_Handler_function *function, OUT MPI_Errhandler *errhandler);
int MPI_Errhandler_set(IN MPI_Comm comm, IN MPI_Errhandler errhandler);
int MPI_Errhandler_get(IN MPI_Comm comm, OUT MPI_Errhandler *errhandler);
int MPI_Errhandler_free(IN MPI_Errhandler *errhandler);
int MPI_Error_string(IN int errorcode, OUT char *errorstring[*resultlen], OUT int *resultlen);
int MPI_Error_class(IN int errorcode, OUT int *errorclass);
double MPI_Wtime(SKIP void);
double MPI_Wtick(SKIP void);
int MPI_Init(IN int *argc, IN char ***argv[1][argc][CSTRING]); INITIAL();
int MPI_Finalize(SKIP void); FINAL();
int MPI_Initialized(OUT int *result); ANYTIME();
int MPI_Abort(IN MPI_Comm comm, IN int errorcode); NORETURN();
int MPI_Pcontrol(IN const int, SKIP ...); SKIP();
int MPI_Close_port(IN char *portname[CSTRING]);
int MPI_Comm_accept(IN char *portname[CSTRING], IN MPI_Info info, IN int root, IN MPI_Comm oldcomm, OUT MPI_Comm *newcomm);
int MPI_Comm_connect(IN char *portname[CSTRING], IN MPI_Info info, IN int root, IN MPI_Comm oldcomm, OUT MPI_Comm *newcomm);
int MPI_Comm_disconnect(IN MPI_Comm *comm);
int MPI_Comm_get_parent(OUT MPI_Comm *parent);
int MPI_Comm_join(IN int fd, OUT MPI_Comm *comm);
int MPI_Comm_spawn(IN char *command[CSTRING] {COND(oldcommrank==root)}, IN char **argv[NULLTERM][CSTRING] {COND(oldcommrank==root)}, IN int maxprocs {COND(oldcommrank==root)}, IN MPI_Info info {COND(oldcommrank==root)}, IN int root, IN MPI_Comm oldcomm, OUT MPI_Comm *newcomm, OUT ERRCODE *errcodes[maxprocs]{COND(oldcommrank==root)}); EXTRA({DECLARE ADD int oldcommrank;}{PREAMBLE PMPI_Comm_rank(oldcomm, &oldcommrank);});
int MPI_Comm_spawn_multiple(IN int count {COND(oldcommrank==root)}, IN char **commands[count][CSTRING] {COND(oldcommrank==root)}, IN char ***argvs[count][NULLTERM][CSTRING] {COND(oldcommrank==root)}, IN int *maxprocs[count] {COND(oldcommrank==root)}, IN MPI_Info *info[count] {COND(oldcommrank==root)}, IN int root, IN MPI_Comm oldcomm, OUT MPI_Comm *newcomm, OUT ERRCODE *errcodes[totprocs] {COND(oldcommrank==root)}); EXTRA({DECLARE ADD int oldcommrank; ADD int totprocs; int i;}{PREAMBLE PMPI_Comm_rank(oldcomm, &oldcommrank); totprocs=0; if(oldcommrank==root) for(i=0; i<count;++i) totprocs+=maxprocs[i];});
int MPI_Lookup_name(IN char *servicename[CSTRING], IN MPI_Info info, OUT char *portname[CSTRING]);
int MPI_Open_port(IN MPI_Info info, OUT char *portname[CSTRING]);
int MPI_Publish_name(IN char *servicename[CSTRING], IN MPI_Info info, IN char *portname[CSTRING]);
int MPI_Unpublish_name(IN char *servicename[CSTRING], IN MPI_Info info, IN char *portname[CSTRING]);
int MPI_Accumulate(SKIP void *buf, IN int origincount, IN MPI_Datatype origintype, IN int targetrank, IN MPI_Aint targetdisp, IN int targetcount, IN MPI_Datatype targettype,  IN MPI_Op op, IN MPI_Win win);
int MPI_Get(SKIP void *buf, IN int origincount, IN MPI_Datatype origintype, IN int targetrank, IN MPI_Aint targetdisp, IN int targetcount, IN MPI_Datatype targettype, IN MPI_Win win);
int MPI_Put(SKIP void *buf, IN int origincount, IN MPI_Datatype origintype, IN int targetrank, IN MPI_Aint targetdisp, IN int targetcount, IN MPI_Datatype targettype, IN MPI_Win win);
int MPI_Win_complete(IN MPI_Win win);
int MPI_Win_create(SKIP void *addr, IN MPI_Aint size, IN int dispunit, IN MPI_Info info, IN MPI_Comm comm, OUT MPI_Win *win);
int MPI_Win_fence(IN WINASSERT assertion, IN MPI_Win win);
int MPI_Win_free(IN MPI_Win *win);
int MPI_Win_get_group(IN MPI_Win win, OUT MPI_Group *group);
int MPI_Win_lock(IN LOCKTYPE locktype, IN int winrank, IN WINASSERT assertion, IN MPI_Win win);
int MPI_Win_post(IN MPI_Group group, IN WINASSERT assertion, IN MPI_Win win);
int MPI_Win_start(IN MPI_Group group, IN WINASSERT assertion, IN MPI_Win win);
int MPI_Win_test(IN MPI_Win win, OUT int *flag);
int MPI_Win_unlock(IN int winrank, IN MPI_Win win);
int MPI_Win_wait(IN MPI_Win win);
int MPI_Alltoallw(SKIP void *sendbuf, IN int *sendcounts[commsize], IN int *senddispls[commsize], IN MPI_Datatype *sendtypes[commsize], SKIP void *recvbuf, IN int *recvcounts[commsize], IN int *recvdispls[commsize], IN MPI_Datatype *recvtypes[commsize], IN MPI_Comm comm); EXTRA({DECLARE ADD int commsize;}{PREAMBLE PMPI_Comm_size(comm, &commsize);});
int MPI_Exscan(SKIP void *sendbuf, SKIP void *recvbuf, IN int count, IN MPI_Datatype datatype, IN MPI_Op op, IN MPI_Comm comm);
int MPI_Add_error_class(OUT int *errorclass);
int MPI_Add_error_code(IN int errorclass, OUT int *errorcode);
int MPI_Add_error_string(IN int errorcode, IN char *errorstring[CSTRING]);
int MPI_Comm_call_errhandler(IN MPI_Comm comm, IN int errorcode);
int MPI_Comm_create_keyval(SKIP MPI_Comm_copy_attr_function *copyfunc, SKIP MPI_Comm_delete_attr_function *deletefunc, OUT COMMKEYVAL *keyval, SKIP void *extrastate);
int MPI_Comm_delete_attr(IN MPI_Comm comm, IN COMMKEYVAL keyval);
int MPI_Comm_free_keyval(IN COMMKEYVAL *keyval);
int MPI_Comm_get_attr(IN MPI_Comm comm, IN COMMKEYVAL keyval, SKIP void *value, OUT int *flag);
int MPI_Comm_get_name(IN MPI_Comm comm, OUT char *name[*resultlen], OUT int *resultlen);
int MPI_Comm_set_attr(IN MPI_Comm comm, IN COMMKEYVAL keyval, SKIP void *value);
int MPI_Comm_set_name(IN MPI_Comm comm, IN char *name[CSTRING]);
int MPI_File_call_errhandler(IN MPI_File file, IN int errorcode);
int MPI_Grequest_complete(IN MPI_Request request);
int MPI_Grequest_start(SKIP MPI_Grequest_query_function *queryfunc, SKIP MPI_Grequest_free_function *freefunc, SKIP MPI_Grequest_cancel_function *cancelfunc, SKIP void *state, OUT MPI_Request *request);
int MPI_Init_thread(IN int *argc, IN char ***argv[1][argc][CSTRING], IN THREADLEVEL required, OUT THREADLEVEL *provided); INITIAL();
int MPI_Is_thread_main(OUT int *flag); ANYTIME();
int MPI_Query_thread(OUT THREADLEVEL *supported); ANYTIME();
int MPI_Status_set_cancelled(IN MPI_Status *status, IN int flag);
int MPI_Status_set_elements(IN MPI_Status *status, IN MPI_Datatype datatype, IN int count);
int MPI_Type_create_keyval(SKIP MPI_Type_copy_attr_function *copyfunc, SKIP MPI_Type_delete_attr_function *deletefunc, OUT TYPEKEYVAL *keyval, SKIP void *state);
int MPI_Type_delete_attr(IN MPI_Datatype datatype, IN TYPEKEYVAL keyval);
int MPI_Type_dup(IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_free_keyval(IN TYPEKEYVAL *keyval);
int MPI_Type_get_attr(IN MPI_Datatype datatype, IN TYPEKEYVAL keyval, SKIP void *value, OUT int *flag);
int MPI_Type_get_contents(IN MPI_Datatype datatype, IN int maxintegers, IN int maxaddresses, IN int maxdatatypes, OUT int *arrintegers[numintegers], OUT MPI_Aint *arraddresses[numaddresses], OUT MPI_Datatype *arrdatatypes[numdatatypes]); EXTRA({DECLARE ADD int numintegers; ADD int numaddresses; ADD int numdatatypes; int combiner} {PREAMBLE PMPI_Type_get_envelope(datatype, &numintegers, &numaddresses, &numdatatypes, &combiner);});
int MPI_Type_get_envelope(IN MPI_Datatype datatype, OUT int *numintegers, OUT int *numaddresses, OUT int *numdatatypes, OUT COMBINER *combiner);
int MPI_Type_get_name(IN MPI_Datatype datatype, OUT char *name[CSTRING], OUT int *resultlen);
int MPI_Type_set_attr(IN MPI_Datatype datatype, IN TYPEKEYVAL keyval, SKIP void *state);
int MPI_Type_set_name(IN MPI_Datatype datatype, IN char *name[CSTRING]);
int MPI_Type_match_size(IN TYPECLASS typeclass, IN int size, OUT MPI_Datatype *datatype);
int MPI_Win_call_errhandler(IN MPI_Win win, IN int errorcode);
int MPI_Win_create_keyval(SKIP MPI_Win_copy_attr_function *copyfun, SKIP MPI_Win_delete_attr_function *deletefun, OUT WINKEYVAL *keyval, SKIP void *state);
int MPI_Win_delete_attr(IN MPI_Win win, IN WINKEYVAL keyval);
int MPI_Win_free_keyval(IN WINKEYVAL *keyval);
int MPI_Win_get_attr(IN MPI_Win win, IN WINKEYVAL keyval, SKIP void *state, OUT int *flag);
int MPI_Win_get_name(IN MPI_Win win, OUT char *name[CSTRING], OUT int *resultlen);
int MPI_Win_set_attr(IN MPI_Win win, IN WINKEYVAL keyval, SKIP void *state);
int MPI_Win_set_name(IN MPI_Win win, IN char *name[CSTRING]);
int MPI_Alloc_mem(IN MPI_Aint size, IN MPI_Info info, SKIP void *baseptr);
int MPI_Comm_create_errhandler(SKIP MPI_Comm_errhandler_fn *errfun, OUT MPI_Errhandler *errhandler);
int MPI_Comm_get_errhandler(IN MPI_Comm comm, OUT MPI_Errhandler *errhandler);
int MPI_Comm_set_errhandler(IN MPI_Comm comm, IN MPI_Errhandler errhandler);
int MPI_File_create_errhandler(SKIP MPI_File_errhandler_fn *errfun, OUT MPI_Errhandler *errhandler);
int MPI_File_get_errhandler(IN MPI_File file, OUT MPI_Errhandler *errhandler);
int MPI_File_set_errhandler(IN MPI_File file, IN MPI_Errhandler errhandler);
int MPI_Finalized(OUT int *flag); ANYTIME();
int MPI_Free_mem(SKIP void *buf);
int MPI_Get_address(SKIP void *buf, OUT MPI_Aint *address);
int MPI_Info_create(OUT MPI_Info *info);
int MPI_Info_delete(IN MPI_Info info, IN char *key[CSTRING]);
int MPI_Info_dup(IN MPI_Info oldinfo, OUT MPI_Info *newinfo);
int MPI_Info_free(IN MPI_Info *info);
int MPI_Info_get(IN MPI_Info info, IN char *key[CSTRING], IN int valuelength, OUT char *value[CSTRING], OUT int *flag);
int MPI_Info_get_nkeys(IN MPI_Info info, OUT int *nkeys);
int MPI_Info_get_nthkey(IN MPI_Info info, IN int n, OUT char *key[CSTRING]);
int MPI_Info_get_valuelen(IN MPI_Info info, IN char *key[CSTRING], OUT int *valuelen, OUT int *flag);
int MPI_Info_set(IN MPI_Info info, IN char *key[CSTRING], IN char *value[CSTRING]);
int MPI_Pack_external(IN char *datarep[CSTRING], SKIP void *inbuf, IN int  incount, IN MPI_Datatype intype, SKIP void *outbuf, IN MPI_Aint outcount, INOUT MPI_Aint *position); 
int MPI_Pack_external_size(IN char *datarep[CSTRING], IN int incount, IN MPI_Datatype datatype, OUT MPI_Aint *size); 
int MPI_Request_get_status(IN MPI_Request request, OUT int *flag, OUT MPI_Status *status {COND(*flag!=0)});
int MPI_Type_create_darray(IN int size, IN int rank, IN int ndims, IN int *gsizes[ndims], IN DISTRIBUTION *distribs[ndims], IN int *dargs[ndims], IN int *psizes[ndims], IN ORDERING order, IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_create_hindexed(IN int count, IN int *blocklengths[count], IN MPI_Aint *displacements[count], IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_create_hvector(IN int count, IN int blocklength, IN MPI_Aint stride, IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_create_indexed_block(IN int count, IN int blocklength, IN int *displacments[count], IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_create_resized(IN MPI_Datatype oldtype, IN MPI_Aint lb, IN MPI_Aint extent, OUT MPI_Datatype *newtype);
int MPI_Type_create_struct(IN int count, IN int *blocklengths[count], IN MPI_Aint *displacements[count], IN MPI_Datatype *oldtypes[count], OUT MPI_Datatype *newtype);
int MPI_Type_create_subarray(IN int ndims, IN int *sizes[ndims], IN int *subsizes[ndims], IN int *starts[ndims], IN ORDERING order, IN MPI_Datatype oldtype, OUT MPI_Datatype *newtype);
int MPI_Type_get_extent(IN MPI_Datatype datatype, OUT MPI_Aint *lb, OUT MPI_Aint *extent);
int MPI_Type_get_true_extent(IN MPI_Datatype datatype, OUT MPI_Aint *lb, OUT MPI_Aint *extent);
int MPI_Unpack_external(IN char *datarep[CSTRING], SKIP void *inbuf, IN MPI_Aint insize, INOUT MPI_Aint *position, SKIP void *outbuf, IN int outcount, IN MPI_Datatype datatype);
int MPI_Win_create_errhandler(SKIP MPI_Win_errhandler_fn *errfun, OUT MPI_Errhandler *errhandler);
int MPI_Win_get_errhandler(IN MPI_Win win, OUT MPI_Errhandler *errhandler);
int MPI_Win_set_errhandler(IN MPI_Win win, OUT MPI_Errhandler errhandler);
int MPI_File_open(IN MPI_Comm comm, IN char *filename[CSTRING], IN FILEMODE amode, IN MPI_Info info, OUT MPI_File *file);
int MPI_File_close(IN MPI_File *file);
int MPI_File_delete(IN char *filename[CSTRING], IN MPI_Info info);
int MPI_File_set_size(IN MPI_File file, IN MPI_Offset size);
int MPI_File_preallocate(IN MPI_File file, IN MPI_Offset size);
int MPI_File_get_size(IN MPI_File file, OUT MPI_Offset *size);
int MPI_File_get_group(IN MPI_File file, OUT MPI_Group *group);
int MPI_File_get_amode(IN MPI_File file, OUT FILEMODE *amode);
int MPI_File_set_info(IN MPI_File file, IN MPI_Info info);
int MPI_File_get_info(IN MPI_File file, OUT MPI_Info *info);
int MPI_File_set_view(IN MPI_File file, IN MPI_Offset offset, IN MPI_Datatype hosttype, IN MPI_Datatype filetype, IN char *datarep[CSTRING], IN MPI_Info info);
int MPI_File_get_view(IN MPI_File file, OUT MPI_Offset *offset, OUT MPI_Datatype *hosttype, OUT MPI_Datatype *filetype, OUT char *datarep[CSTRING]);
int MPI_File_read_at(IN MPI_File file, IN MPI_Offset offset, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_read_at_all(IN MPI_File file, IN MPI_Offset offset, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_write_at(IN MPI_File file, IN MPI_Offset offset, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_write_at_all(IN MPI_File file, IN MPI_Offset offset, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_iread_at(IN MPI_File file, IN MPI_Offset offset, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPIO_Request *request);
int MPI_File_iwrite_at(IN MPI_File file, IN MPI_Offset offset, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPIO_Request *request);
int MPI_File_read(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status); 
int MPI_File_read_all(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status); 
int MPI_File_write(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_write_all(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_iread(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPIO_Request *request); 
int MPI_File_iwrite(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPIO_Request *request);
int MPI_File_seek(IN MPI_File file, IN MPI_Offset offset, IN WHENCE whence);
int MPI_File_get_position(IN MPI_File file, OUT MPI_Offset *offset);
int MPI_File_get_byte_offset(IN MPI_File file, IN MPI_Offset offset, OUT MPI_Offset *bytes);
int MPI_File_read_shared(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_write_shared(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_iread_shared(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPIO_Request *request);
int MPI_File_iwrite_shared(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPIO_Request *request);
int MPI_File_read_ordered(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_write_ordered(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype, OUT MPI_Status *status);
int MPI_File_seek_shared(IN MPI_File file, IN MPI_Offset offset, IN WHENCE whence);
int MPI_File_get_position_shared(IN MPI_File file, OUT MPI_Offset *offset);
int MPI_File_read_at_all_begin(IN MPI_File file, IN MPI_Offset offset, SKIP void *buf, IN int count, IN MPI_Datatype datatype);
int MPI_File_read_at_all_end(IN MPI_File file, SKIP void *buf, OUT MPI_Status *status);
int MPI_File_write_at_all_begin(IN MPI_File file, IN MPI_Offset offset, SKIP void *buf, IN int count, IN MPI_Datatype datatype);
int MPI_File_write_at_all_end(IN MPI_File file, SKIP void *buf, OUT MPI_Status *status);
int MPI_File_read_all_begin(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype);
int MPI_File_read_all_end(IN MPI_File file, SKIP void *buf, OUT MPI_Status *status);
int MPI_File_write_all_begin(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype);
int MPI_File_write_all_end(IN MPI_File file, SKIP void *buf, OUT MPI_Status *status);
int MPI_File_read_ordered_begin(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype);
int MPI_File_read_ordered_end(IN MPI_File file, SKIP void *buf, OUT MPI_Status *status);
int MPI_File_write_ordered_begin(IN MPI_File file, SKIP void *buf, IN int count, IN MPI_Datatype datatype);
int MPI_File_write_ordered_end(IN MPI_File file, SKIP void *buf, OUT MPI_Status *status);
int MPI_File_get_type_extent(IN MPI_File file, IN MPI_Datatype datatype, OUT MPI_Aint *extent);
int MPI_Register_datarep(IN char *name[CSTRING], SKIP MPI_Datarep_conversion_function *readfun, SKIP MPI_Datarep_conversion_function *writefun, SKIP MPI_Datarep_extent_function *extentfun, SKIP void *state);
int MPI_File_set_atomicity(IN MPI_File file, IN int flag);
int MPI_File_get_atomicity(IN MPI_File file, OUT int *flag);
int MPI_File_sync(IN MPI_File file);
int MPIO_Test(IN MPIO_Request *request, OUT int *flag, OUT MPI_Status *status {COND(*flag!=0)});
int MPIO_Wait(IN MPIO_Request *request, OUT MPI_Status *status);
int MPIO_Testall(IN int count, IN MPIO_Request *requests[count], OUT int *flag, OUT MPI_Status *statuses[count] {COND(*flag!=0)});
int MPIO_Waitall(IN int count, IN MPIO_Request *requests[count], OUT MPI_Status *statuses[count]);
int MPIO_Testany(IN int count, IN MPIO_Request *requests[count], OUT int *index {COND(*flag!=0)}, OUT int *flag, OUT MPI_Status *statuses[count] {COND(*flag!=0)});
int MPIO_Waitany(IN int count, IN MPIO_Request *requests[count], OUT int *index, OUT MPI_Status *statuses[count] {COND(*index != MPI_UNDEFINED)}); 
int MPIO_Waitsome(IN int count, IN MPIO_Request *requests[count], OUT int *outcount, OUT int *indices[*outcount], OUT MPI_Status *statuses[*outcount]);
int MPIO_Testsome(IN int count, IN MPIO_Request *requests[count], OUT int *outcount, OUT int *indices[*outcount], OUT MPI_Status *statuses[*outcount]);