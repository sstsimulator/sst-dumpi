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
#include <dumpi/bin/dumpi2otf2-defs.h>
#include <dumpi/libotf2dump/otf2writer.h>
#include <assert.h>

// Generates a captureless lambda function signature for an undumpi callback
#define CBACK_INIT(struct_name) \
  cbacks->on_ ## struct_name = [](const dumpi_ ## struct_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *userarg)

// Boilerplate initialization
#define UNPACK_ARGS() \
  dumpi::OTF2_Writer& otf2_writer = *(dumpi::OTF2_Writer*)userarg; \
  auto& p = *prm;

// A wrapper for calls that do not record parameters in OTF2
#define GENERIC_CALL(name) \
  UNPACK_ARGS(); \
  otf2_writer.generic_call(convert_time(wall->start), convert_time(wall->stop), name); \
  return 1;

// Turns a dumpi_clock struct into an integer of total nanoseconds
static inline uint64_t convert_time(dumpi_clock c) {
  return ((((uint64_t)c.sec) * 1E9) + (uint64_t)c.nsec);
}

// Partially generated using set_callbacks() in dumpi2ascii-callbacks.cc and this vim substitution:
// :%s/^.*->on_\(.\{-}\)\(\s*\)= report_\(.\{-}\)\s*;$/  CBACK_INIT(\1)\2{ GENERIC_CALL("\3"); };/g
void set_callbacks(libundumpi_callbacks *cbacks) {
  assert(cbacks != NULL);
  CBACK_INIT(send)                      { UNPACK_ARGS(); otf2_writer.mpi_send(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.dest, p.comm, p.tag); return 1; };
  CBACK_INIT(recv)                      { UNPACK_ARGS(); otf2_writer.mpi_recv(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.source, p.comm, p.tag); return 1; };
  CBACK_INIT(get_count)                 { GENERIC_CALL("MPI_Get_count"); };
  CBACK_INIT(bsend)                     { UNPACK_ARGS(); otf2_writer.mpi_bsend(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.dest, p.comm, p.tag); return 1; };
  CBACK_INIT(ssend)                     { UNPACK_ARGS(); otf2_writer.mpi_ssend(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.dest, p.comm, p.tag); return 1; };
  CBACK_INIT(rsend)                     { UNPACK_ARGS(); otf2_writer.mpi_rsend(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.dest, p.comm, p.tag); return 1; };
  CBACK_INIT(buffer_attach)             { GENERIC_CALL("MPI_Buffer_attach"); };
  CBACK_INIT(buffer_detach)             { GENERIC_CALL("MPI_Buffer_detach"); };
  CBACK_INIT(comm_free)                 { UNPACK_ARGS(); otf2_writer.mpi_comm_free(convert_time(wall->start), convert_time(wall->stop), p.comm); return 1; };
  CBACK_INIT(isend)                     { UNPACK_ARGS(); otf2_writer.mpi_isend(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.dest, p.comm, p.tag, p.request); return 1; };
  CBACK_INIT(ibsend)                    { UNPACK_ARGS(); otf2_writer.mpi_ibsend(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.dest, p.comm, p.tag, p.request); return 1; };
  CBACK_INIT(issend)                    { UNPACK_ARGS(); otf2_writer.mpi_issend(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.dest, p.comm, p.tag, p.request); return 1; };
  CBACK_INIT(irsend)                    { UNPACK_ARGS(); otf2_writer.mpi_irsend(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.dest, p.comm, p.tag, p.request); return 1; };
  CBACK_INIT(irecv)                     { UNPACK_ARGS(); otf2_writer.mpi_irecv(convert_time(wall->start), convert_time(wall->stop), p.datatype, p.count, p.source, p.comm, p.tag, p.request); return 1; };
  CBACK_INIT(wait)                      { UNPACK_ARGS(); otf2_writer.mpi_wait(convert_time(wall->start), convert_time(wall->stop), p.request, p.status); return 1; };
  CBACK_INIT(test)                      { UNPACK_ARGS(); otf2_writer.mpi_test(convert_time(wall->start), convert_time(wall->stop), p.request, p.flag, p.status); return 1; };
  CBACK_INIT(request_free)              { GENERIC_CALL("MPI_Request_free"); };
  CBACK_INIT(waitany)                   { UNPACK_ARGS(); otf2_writer.mpi_waitany(convert_time(wall->start), convert_time(wall->stop), p.requests[p.index], p.status); return 1; };
  CBACK_INIT(testany)                   { UNPACK_ARGS(); otf2_writer.mpi_testany(convert_time(wall->start), convert_time(wall->stop), p.requests, p.index, p.flag, p.status); return 1; };
  CBACK_INIT(waitall)                   { UNPACK_ARGS(); otf2_writer.mpi_waitall(convert_time(wall->start), convert_time(wall->stop), p.count, p.requests, p.statuses); return 1; };
  CBACK_INIT(testall)                   { UNPACK_ARGS(); otf2_writer.mpi_testall(convert_time(wall->start), convert_time(wall->stop), p.count, p.requests, p.flag, p.statuses); return 1; };
  CBACK_INIT(waitsome)                  { UNPACK_ARGS(); otf2_writer.mpi_waitsome(convert_time(wall->start), convert_time(wall->stop), p.requests, p.outcount, p.indices, p.statuses); return 1; };
  CBACK_INIT(testsome)                  { UNPACK_ARGS(); otf2_writer.mpi_testsome(convert_time(wall->start), convert_time(wall->stop), p.requests, p.outcount, p.indices, p.statuses); return 1; };
  CBACK_INIT(iprobe)                    { GENERIC_CALL("MPI_Iprobe"); };
  CBACK_INIT(probe)                     { GENERIC_CALL("MPI_Probe"); };
  CBACK_INIT(cancel)                    { GENERIC_CALL("MPI_Cancel"); };
  CBACK_INIT(test_cancelled)            { GENERIC_CALL("MPI_Test_cancelled"); };
  CBACK_INIT(send_init)                 { GENERIC_CALL("MPI_Send_init"); };
  CBACK_INIT(bsend_init)                { GENERIC_CALL("MPI_Bsend_init"); };
  CBACK_INIT(ssend_init)                { GENERIC_CALL("MPI_Ssend_init"); };
  CBACK_INIT(rsend_init)                { GENERIC_CALL("MPI_Rsend_init"); };
  CBACK_INIT(recv_init)                 { GENERIC_CALL("MPI_Recv_init"); };
  CBACK_INIT(start)                     { GENERIC_CALL("MPI_Start"); };
  CBACK_INIT(startall)                  { GENERIC_CALL("MPI_Startall"); };
  CBACK_INIT(sendrecv)                  { GENERIC_CALL("MPI_Sendrecv"); };
  CBACK_INIT(sendrecv_replace)          { GENERIC_CALL("MPI_Sendrecv_replace"); };
  CBACK_INIT(type_contiguous)           { UNPACK_ARGS(); otf2_writer.mpi_type_contiguous(convert_time(wall->start), convert_time(wall->stop), p.count, p.oldtype, p.newtype); return 1; };
  CBACK_INIT(type_vector)               { UNPACK_ARGS(); otf2_writer.mpi_type_vector(convert_time(wall->start), convert_time(wall->stop), p.count, p.blocklength, p.oldtype, p.newtype); return 1; };
  CBACK_INIT(type_hvector)              { GENERIC_CALL("MPI_Type_hvector"); };
  CBACK_INIT(type_indexed)              { UNPACK_ARGS(); otf2_writer.mpi_type_indexed(convert_time(wall->start), convert_time(wall->stop), p.count, p.lengths, p.oldtype, p.newtype); return 1; };
  CBACK_INIT(type_hindexed)             { UNPACK_ARGS(); otf2_writer.mpi_type_indexed(convert_time(wall->start), convert_time(wall->stop), p.count, p.lengths, p.oldtype, p.newtype); return 1; };
  CBACK_INIT(type_struct)               { UNPACK_ARGS(); otf2_writer.mpi_type_struct(convert_time(wall->start), convert_time(wall->stop), p.count, p.lengths, p.oldtypes, p.newtype); return 1; };
  CBACK_INIT(address)                   { GENERIC_CALL("MPI_Address"); };
  CBACK_INIT(type_extent)               { GENERIC_CALL("MPI_Type_extent"); };
  CBACK_INIT(type_size)                 { GENERIC_CALL("MPI_Type_size"); };
  CBACK_INIT(type_lb)                   { GENERIC_CALL("MPI_Type_lb"); };
  CBACK_INIT(type_ub)                   { GENERIC_CALL("MPI_Type_ub"); };
  CBACK_INIT(type_commit)               { GENERIC_CALL("MPI_Type_commit"); };
  CBACK_INIT(type_free)                 { GENERIC_CALL("MPI_Type_free"); };
  CBACK_INIT(get_elements)              { GENERIC_CALL("MPI_Get_elements"); };
  CBACK_INIT(pack)                      { GENERIC_CALL("MPI_Pack"); };
  CBACK_INIT(unpack)                    { GENERIC_CALL("MPI_Unpack"); };
  CBACK_INIT(pack_size)                 { GENERIC_CALL("MPI_Pack_size"); };
  CBACK_INIT(barrier)                   { UNPACK_ARGS(); otf2_writer.mpi_barrier(convert_time(wall->start), convert_time(wall->stop), p.comm); return 1; };
  CBACK_INIT(bcast)                     { UNPACK_ARGS(); otf2_writer.mpi_bcast(convert_time(wall->start), convert_time(wall->stop), p.count, p.datatype, p.root, p.comm); return 1; };
  CBACK_INIT(gather)                    { UNPACK_ARGS(); otf2_writer.mpi_gather(convert_time(wall->start), convert_time(wall->stop), p.sendcount, p.sendtype, p.recvcount, p.recvtype, p.root, p.comm); return 1; };
  CBACK_INIT(gatherv)                   { UNPACK_ARGS(); otf2_writer.mpi_gatherv(convert_time(wall->start), convert_time(wall->stop), p.commsize, p.sendcount, p.sendtype, p.recvcounts, p.recvtype, p.root, p.comm); return 1; };
  CBACK_INIT(scatter)                   { UNPACK_ARGS(); otf2_writer.mpi_scatter(convert_time(wall->start), convert_time(wall->stop), p.sendcount, p.sendtype, p.recvcount, p.recvtype, p.root, p.comm); return 1; };
  CBACK_INIT(scatterv)                  { UNPACK_ARGS(); otf2_writer.mpi_scatterv(convert_time(wall->start), convert_time(wall->stop), p.commsize, p.sendcounts, p.sendtype, p.recvcount, p.recvtype, p.root, p.comm); return 1; };
  CBACK_INIT(allgather)                 { UNPACK_ARGS(); otf2_writer.mpi_allgather(convert_time(wall->start), convert_time(wall->stop), p.sendcount, p.sendtype, p.recvcount, p.recvtype, p.comm); return 1; };
  CBACK_INIT(allgatherv)                { UNPACK_ARGS(); otf2_writer.mpi_allgatherv(convert_time(wall->start), convert_time(wall->stop), p.commsize, p.sendcount, p.sendtype, p.recvcounts, p.recvtype, p.comm); return 1; };
  CBACK_INIT(alltoall)                  { UNPACK_ARGS(); otf2_writer.mpi_alltoall(convert_time(wall->start), convert_time(wall->stop), p.sendcount, p.sendtype, p.recvcount, p.recvtype, p.comm); return 1; };
  CBACK_INIT(alltoallv)                 { UNPACK_ARGS(); otf2_writer.mpi_alltoallv(convert_time(wall->start), convert_time(wall->stop), p.commsize, p.sendcounts, p.sendtype, p.recvcounts, p.recvtype, p.comm); return 1; };
  CBACK_INIT(reduce)                    { UNPACK_ARGS(); otf2_writer.mpi_reduce(convert_time(wall->start), convert_time(wall->stop), p.count, p.datatype, p.root, p.comm); return 1; };
  CBACK_INIT(op_create)                 { GENERIC_CALL("MPI_Op_create"); };
  CBACK_INIT(op_free)                   { GENERIC_CALL("MPI_Op_free"); };
  CBACK_INIT(allreduce)                 { UNPACK_ARGS(); otf2_writer.mpi_allreduce(convert_time(wall->start), convert_time(wall->stop), p.count, p.datatype, p.comm); return 1; };
  CBACK_INIT(reduce_scatter)            { UNPACK_ARGS(); otf2_writer.mpi_reduce_scatter(convert_time(wall->start), convert_time(wall->stop), p.commsize, p.recvcounts, p.datatype, p.comm); return 1; };
  CBACK_INIT(scan)                      { UNPACK_ARGS(); otf2_writer.mpi_scan(convert_time(wall->start), convert_time(wall->stop), p.count, p.datatype, p.comm); return 1; };
  CBACK_INIT(group_size)                { GENERIC_CALL("MPI_Group_size"); };
  CBACK_INIT(group_rank)                { GENERIC_CALL("MPI_Group_rank"); };
  CBACK_INIT(group_translate_ranks)     { GENERIC_CALL("MPI_Group_translate_ranks"); };
  CBACK_INIT(group_compare)             { GENERIC_CALL("MPI_Group_compare"); };
  CBACK_INIT(comm_group)                { UNPACK_ARGS(); otf2_writer.mpi_comm_group(convert_time(wall->start), convert_time(wall->stop), p.comm, p.group); return 1; };
  CBACK_INIT(group_union)               { UNPACK_ARGS(); otf2_writer.mpi_group_union(convert_time(wall->start), convert_time(wall->stop), p.group1, p.group2, p.newgroup); return 1; };
  CBACK_INIT(group_intersection)        { UNPACK_ARGS(); otf2_writer.mpi_group_intersection(convert_time(wall->start), convert_time(wall->stop), p.group1, p.group2, p.newgroup); return 1; };
  CBACK_INIT(group_difference)          { UNPACK_ARGS(); otf2_writer.mpi_group_difference(convert_time(wall->start), convert_time(wall->stop), p.group1, p.group2, p.newgroup); return 1; };
  CBACK_INIT(group_incl)                { UNPACK_ARGS(); otf2_writer.mpi_group_incl(convert_time(wall->start), convert_time(wall->stop), p.group, p.count, p.ranks, p.newgroup); return 1; };
  CBACK_INIT(group_excl)                { UNPACK_ARGS(); otf2_writer.mpi_group_excl(convert_time(wall->start), convert_time(wall->stop), p.group, p.count, p.ranks, p.newgroup); return 1; };
  CBACK_INIT(group_range_incl)          { UNPACK_ARGS(); otf2_writer.mpi_group_range_incl(convert_time(wall->start), convert_time(wall->stop), p.group, p.count, p.ranges, p.newgroup); return 1; };
  CBACK_INIT(group_range_excl)          { GENERIC_CALL("MPI_Group_range_excl"); };
  CBACK_INIT(group_free)                { GENERIC_CALL("MPI_Group_free"); };
  CBACK_INIT(comm_size)                 { GENERIC_CALL("MPI_Comm_size"); };
  CBACK_INIT(comm_rank)                 { GENERIC_CALL("MPI_Comm_rank"); };
  CBACK_INIT(comm_compare)              { GENERIC_CALL("MPI_Comm_compare"); };
  CBACK_INIT(comm_dup)                  { UNPACK_ARGS(); otf2_writer.mpi_comm_dup(convert_time(wall->start), convert_time(wall->stop), p.oldcomm, p.newcomm); return 1; };
  CBACK_INIT(comm_create)               { UNPACK_ARGS(); otf2_writer.mpi_comm_create(convert_time(wall->start), convert_time(wall->stop), p.oldcomm, p.group, p.newcomm); return 1; };
  CBACK_INIT(comm_split)                { UNPACK_ARGS(); otf2_writer.mpi_comm_split(convert_time(wall->start), convert_time(wall->stop), p.oldcomm, p.color, p.key, p.newcomm); return 1; };
  CBACK_INIT(comm_test_inter)           { GENERIC_CALL("MPI_Comm_test_inter"); };
  CBACK_INIT(comm_remote_size)          { GENERIC_CALL("MPI_Comm_remote_size"); };
  CBACK_INIT(comm_remote_group)         { GENERIC_CALL("MPI_Comm_remote_group"); };
  CBACK_INIT(intercomm_create)          { GENERIC_CALL("MPI_Intercomm_create"); };
  CBACK_INIT(intercomm_merge)           { GENERIC_CALL("MPI_Intercomm_merge"); };
  CBACK_INIT(keyval_create)             { GENERIC_CALL("MPI_Keyval_create"); };
  CBACK_INIT(keyval_free)               { GENERIC_CALL("MPI_Keyval_free"); };
  CBACK_INIT(attr_put)                  { GENERIC_CALL("MPI_Attr_put"); };
  CBACK_INIT(attr_get)                  { GENERIC_CALL("MPI_Attr_get"); };
  CBACK_INIT(attr_delete)               { GENERIC_CALL("MPI_Attr_delete"); };
  CBACK_INIT(topo_test)                 { GENERIC_CALL("MPI_Topo_test"); };
  CBACK_INIT(cart_create)               { GENERIC_CALL("MPI_Cart_create"); };
  CBACK_INIT(dims_create)               { GENERIC_CALL("MPI_Dims_create"); };
  CBACK_INIT(graph_create)              { GENERIC_CALL("MPI_Graph_create"); };
  CBACK_INIT(graphdims_get)             { GENERIC_CALL("MPI_Graphdims_get"); };
  CBACK_INIT(graph_get)                 { GENERIC_CALL("MPI_Graph_get"); };
  CBACK_INIT(cartdim_get)               { GENERIC_CALL("MPI_Cartdim_get"); };
  CBACK_INIT(cart_get)                  { GENERIC_CALL("MPI_Cart_get"); };
  CBACK_INIT(cart_rank)                 { GENERIC_CALL("MPI_Cart_rank"); };
  CBACK_INIT(cart_coords)               { GENERIC_CALL("MPI_Cart_coords"); };
  CBACK_INIT(graph_neighbors_count)     { GENERIC_CALL("MPI_Graph_neighbors_count"); };
  CBACK_INIT(graph_neighbors)           { GENERIC_CALL("MPI_Graph_neighbors"); };
  CBACK_INIT(cart_shift)                { GENERIC_CALL("MPI_Cart_shift"); };
  CBACK_INIT(cart_sub)                  { GENERIC_CALL("MPI_Cart_sub"); };
  CBACK_INIT(cart_map)                  { GENERIC_CALL("MPI_Cart_map"); };
  CBACK_INIT(graph_map)                 { GENERIC_CALL("MPI_Graph_map"); };
  CBACK_INIT(get_processor_name)        { GENERIC_CALL("MPI_Get_processor_name"); };
  CBACK_INIT(get_version)               { GENERIC_CALL("MPI_Get_version"); };
  CBACK_INIT(errhandler_create)         { GENERIC_CALL("MPI_Errhandler_create"); };
  CBACK_INIT(errhandler_set)            { GENERIC_CALL("MPI_Errhandler_set"); };
  CBACK_INIT(errhandler_get)            { GENERIC_CALL("MPI_Errhandler_get"); };
  CBACK_INIT(errhandler_free)           { GENERIC_CALL("MPI_Errhandler_free"); };
  CBACK_INIT(error_string)              { GENERIC_CALL("MPI_Error_string"); };
  CBACK_INIT(error_class)               { GENERIC_CALL("MPI_Error_class"); };
  CBACK_INIT(wtime)                     { GENERIC_CALL("MPI_Wtime"); };
  CBACK_INIT(wtick)                     { GENERIC_CALL("MPI_Wtick"); };
  CBACK_INIT(init)                      { GENERIC_CALL("MPI_Init"); };
  CBACK_INIT(finalize)                  { GENERIC_CALL("MPI_Finalize"); };
  CBACK_INIT(initialized)               { GENERIC_CALL("MPI_Initialized"); };
  CBACK_INIT(abort)                     { GENERIC_CALL("MPI_Abort"); };
  CBACK_INIT(close_port)                { GENERIC_CALL("MPI_Close_port"); };
  CBACK_INIT(comm_accept)               { GENERIC_CALL("MPI_Comm_accept"); };
  CBACK_INIT(comm_connect)              { GENERIC_CALL("MPI_Comm_connect"); };
  CBACK_INIT(comm_disconnect)           { GENERIC_CALL("MPI_Comm_disconnect"); };
  CBACK_INIT(comm_get_parent)           { GENERIC_CALL("MPI_Comm_get_parent"); };
  CBACK_INIT(comm_join)                 { GENERIC_CALL("MPI_Comm_join"); };
  CBACK_INIT(comm_spawn)                { GENERIC_CALL("MPI_Comm_spawn"); };
  CBACK_INIT(comm_spawn_multiple)       { GENERIC_CALL("MPI_Comm_spawn_multiple"); };
  CBACK_INIT(lookup_name)               { GENERIC_CALL("MPI_Lookup_name"); };
  CBACK_INIT(open_port)                 { GENERIC_CALL("MPI_Open_port"); };
  CBACK_INIT(publish_name)              { GENERIC_CALL("MPI_Publish_name"); };
  CBACK_INIT(unpublish_name)            { GENERIC_CALL("MPI_Unpublish_name"); };
  CBACK_INIT(accumulate)                { GENERIC_CALL("MPI_Accumulate"); };
  CBACK_INIT(get)                       { GENERIC_CALL("MPI_Get"); };
  CBACK_INIT(put)                       { GENERIC_CALL("MPI_Put"); };
  CBACK_INIT(win_complete)              { GENERIC_CALL("MPI_Win_complete"); };
  CBACK_INIT(win_create)                { GENERIC_CALL("MPI_Win_create"); };
  CBACK_INIT(win_fence)                 { GENERIC_CALL("MPI_Win_fence"); };
  CBACK_INIT(win_free)                  { GENERIC_CALL("MPI_Win_free"); };
  CBACK_INIT(win_get_group)             { GENERIC_CALL("MPI_Win_get_group"); };
  CBACK_INIT(win_lock)                  { GENERIC_CALL("MPI_Win_lock"); };
  CBACK_INIT(win_post)                  { GENERIC_CALL("MPI_Win_post"); };
  CBACK_INIT(win_start)                 { GENERIC_CALL("MPI_Win_start"); };
  CBACK_INIT(win_test)                  { GENERIC_CALL("MPI_Win_test"); };
  CBACK_INIT(win_unlock)                { GENERIC_CALL("MPI_Win_unlock"); };
  CBACK_INIT(win_wait)                  { GENERIC_CALL("MPI_Win_wait"); };
  CBACK_INIT(alltoallw)                 { GENERIC_CALL("MPI_Alltoallw"); };
  CBACK_INIT(exscan)                    { GENERIC_CALL("MPI_Exscan"); };
  CBACK_INIT(add_error_class)           { GENERIC_CALL("MPI_Add_error_class"); };
  CBACK_INIT(add_error_code)            { GENERIC_CALL("MPI_Add_error_code"); };
  CBACK_INIT(add_error_string)          { GENERIC_CALL("MPI_Add_error_string"); };
  CBACK_INIT(comm_call_errhandler)      { GENERIC_CALL("MPI_Comm_call_errhandler"); };
  CBACK_INIT(comm_create_keyval)        { GENERIC_CALL("MPI_Comm_create_keyval"); };
  CBACK_INIT(comm_delete_attr)          { GENERIC_CALL("MPI_Comm_delete_attr"); };
  CBACK_INIT(comm_free_keyval)          { GENERIC_CALL("MPI_Comm_free_keyval"); };
  CBACK_INIT(comm_get_attr)             { GENERIC_CALL("MPI_Comm_get_attr"); };
  CBACK_INIT(comm_get_name)             { GENERIC_CALL("MPI_Comm_get_name"); };
  CBACK_INIT(comm_set_attr)             { GENERIC_CALL("MPI_Comm_set_attr"); };
  CBACK_INIT(comm_set_name)             { GENERIC_CALL("MPI_Comm_set_name"); };
  CBACK_INIT(file_call_errhandler)      { GENERIC_CALL("MPI_File_call_errhandler"); };
  CBACK_INIT(grequest_complete)         { GENERIC_CALL("MPI_Grequest_complete"); };
  CBACK_INIT(grequest_start)            { GENERIC_CALL("MPI_Grequest_start"); };
  CBACK_INIT(init_thread)               { GENERIC_CALL("MPI_Init_thread"); };
  CBACK_INIT(is_thread_main)            { GENERIC_CALL("MPI_Is_thread_main"); };
  CBACK_INIT(query_thread)              { GENERIC_CALL("MPI_Query_thread"); };
  CBACK_INIT(status_set_cancelled)      { GENERIC_CALL("MPI_Status_set_cancelled"); };
  CBACK_INIT(status_set_elements)       { GENERIC_CALL("MPI_Status_set_elements"); };
  CBACK_INIT(type_create_keyval)        { GENERIC_CALL("MPI_Type_create_keyval"); };
  CBACK_INIT(type_delete_attr)          { GENERIC_CALL("MPI_Type_delete_attr"); };
  CBACK_INIT(type_dup)                  { GENERIC_CALL("MPI_Type_dup"); };
  CBACK_INIT(type_free_keyval)          { GENERIC_CALL("MPI_Type_free_keyval"); };
  CBACK_INIT(type_get_attr)             { GENERIC_CALL("MPI_Type_get_attr"); };
  CBACK_INIT(type_get_contents)         { GENERIC_CALL("MPI_Type_get_contents"); };
  CBACK_INIT(type_get_envelope)         { GENERIC_CALL("MPI_Type_get_envelope"); };
  CBACK_INIT(type_get_name)             { GENERIC_CALL("MPI_Type_get_name"); };
  CBACK_INIT(type_set_attr)             { GENERIC_CALL("MPI_Type_set_attr"); };
  CBACK_INIT(type_set_name)             { GENERIC_CALL("MPI_Type_set_name"); };
  CBACK_INIT(type_match_size)           { GENERIC_CALL("MPI_Type_match_size"); };
  CBACK_INIT(win_call_errhandler)       { GENERIC_CALL("MPI_Win_call_errhandler"); };
  CBACK_INIT(win_create_keyval)         { GENERIC_CALL("MPI_Win_create_keyval"); };
  CBACK_INIT(win_delete_attr)           { GENERIC_CALL("MPI_Win_delete_attr"); };
  CBACK_INIT(win_free_keyval)           { GENERIC_CALL("MPI_Win_free_keyval"); };
  CBACK_INIT(win_get_attr)              { GENERIC_CALL("MPI_Win_get_attr"); };
  CBACK_INIT(win_get_name)              { GENERIC_CALL("MPI_Win_get_name"); };
  CBACK_INIT(win_set_attr)              { GENERIC_CALL("MPI_Win_set_attr"); };
  CBACK_INIT(win_set_name)              { GENERIC_CALL("MPI_Win_set_name"); };
  CBACK_INIT(alloc_mem)                 { GENERIC_CALL("MPI_Alloc_mem"); };
  CBACK_INIT(comm_create_errhandler)    { GENERIC_CALL("MPI_Comm_create_errhandler"); };
  CBACK_INIT(comm_get_errhandler)       { GENERIC_CALL("MPI_Comm_get_errhandler"); };
  CBACK_INIT(comm_set_errhandler)       { GENERIC_CALL("MPI_Comm_set_errhandler"); };
  CBACK_INIT(file_create_errhandler)    { GENERIC_CALL("MPI_File_create_errhandler"); };
  CBACK_INIT(file_get_errhandler)       { GENERIC_CALL("MPI_File_get_errhandler"); };
  CBACK_INIT(file_set_errhandler)       { GENERIC_CALL("MPI_File_set_errhandler"); };
  CBACK_INIT(finalized)                 { GENERIC_CALL("MPI_Finalized"); };
  CBACK_INIT(free_mem)                  { GENERIC_CALL("MPI_Free_mem"); };
  CBACK_INIT(get_address)               { GENERIC_CALL("MPI_Get_address"); };
  CBACK_INIT(info_create)               { GENERIC_CALL("MPI_Info_create"); };
  CBACK_INIT(info_delete)               { GENERIC_CALL("MPI_Info_delete"); };
  CBACK_INIT(info_dup)                  { GENERIC_CALL("MPI_Info_dup"); };
  CBACK_INIT(info_free)                 { GENERIC_CALL("MPI_Info_free"); };
  CBACK_INIT(info_get)                  { GENERIC_CALL("MPI_Info_get"); };
  CBACK_INIT(info_get_nkeys)            { GENERIC_CALL("MPI_Info_get_nkeys"); };
  CBACK_INIT(info_get_nthkey)           { GENERIC_CALL("MPI_Info_get_nthkey"); };
  CBACK_INIT(info_get_valuelen)         { GENERIC_CALL("MPI_Info_get_valuelen"); };
  CBACK_INIT(info_set)                  { GENERIC_CALL("MPI_Info_set"); };
  CBACK_INIT(pack_external)             { GENERIC_CALL("MPI_Pack_external"); };
  CBACK_INIT(pack_external_size)        { GENERIC_CALL("MPI_Pack_external_size"); };
  CBACK_INIT(request_get_status)        { GENERIC_CALL("MPI_Request_get_status"); };
  CBACK_INIT(type_create_darray)        { GENERIC_CALL("MPI_Type_create_darray"); };
  CBACK_INIT(type_create_hindexed)      { UNPACK_ARGS(); otf2_writer.mpi_type_create_hindexed(convert_time(wall->start), convert_time(wall->stop), p.count, p.blocklengths, p.oldtype, p.newtype); return 1; };
  CBACK_INIT(type_create_hvector)       { UNPACK_ARGS(); otf2_writer.mpi_type_create_hvector(convert_time(wall->start), convert_time(wall->stop), p.count, p.blocklength, p.oldtype, p.newtype); return 1; };
  CBACK_INIT(type_create_indexed_block) { GENERIC_CALL("MPI_Type_create_indexed_block"); };
  CBACK_INIT(type_create_resized)       { GENERIC_CALL("MPI_Type_create_resized"); };
  CBACK_INIT(type_create_struct)        { UNPACK_ARGS(); otf2_writer.mpi_type_create_struct(convert_time(wall->start), convert_time(wall->stop), p.count, p.blocklengths, p.oldtypes, p.newtype); return 1; };
  CBACK_INIT(type_create_subarray)      { UNPACK_ARGS(); otf2_writer.mpi_type_create_subarray(convert_time(wall->start), convert_time(wall->stop), p.ndims, p.subsizes, p.oldtype, p.newtype); return 1; };
  CBACK_INIT(type_get_extent)           { GENERIC_CALL("MPI_Type_get_extent"); };
  CBACK_INIT(type_get_true_extent)      { GENERIC_CALL("MPI_Type_get_true_extent"); };
  CBACK_INIT(unpack_external)           { GENERIC_CALL("MPI_Unpack_external"); };
  CBACK_INIT(win_create_errhandler)     { GENERIC_CALL("MPI_Win_create_errhandler"); };
  CBACK_INIT(win_get_errhandler)        { GENERIC_CALL("MPI_Win_get_errhandler"); };
  CBACK_INIT(win_set_errhandler)        { GENERIC_CALL("MPI_Win_set_errhandler"); };
  CBACK_INIT(file_open)                 { GENERIC_CALL("MPI_File_open"); };
  CBACK_INIT(file_close)                { GENERIC_CALL("MPI_File_close"); };
  CBACK_INIT(file_delete)               { GENERIC_CALL("MPI_File_delete"); };
  CBACK_INIT(file_set_size)             { GENERIC_CALL("MPI_File_set_size"); };
  CBACK_INIT(file_preallocate)          { GENERIC_CALL("MPI_File_preallocate"); };
  CBACK_INIT(file_get_size)             { GENERIC_CALL("MPI_File_get_size"); };
  CBACK_INIT(file_get_group)            { GENERIC_CALL("MPI_File_get_group"); };
  CBACK_INIT(file_get_amode)            { GENERIC_CALL("MPI_File_get_amode"); };
  CBACK_INIT(file_set_info)             { GENERIC_CALL("MPI_File_set_info"); };
  CBACK_INIT(file_get_info)             { GENERIC_CALL("MPI_File_get_info"); };
  CBACK_INIT(file_set_view)             { GENERIC_CALL("MPI_File_set_view"); };
  CBACK_INIT(file_get_view)             { GENERIC_CALL("MPI_File_get_view"); };
  CBACK_INIT(file_read_at)              { GENERIC_CALL("MPI_File_read_at"); };
  CBACK_INIT(file_read_at_all)          { GENERIC_CALL("MPI_File_read_at_all"); };
  CBACK_INIT(file_write_at)             { GENERIC_CALL("MPI_File_write_at"); };
  CBACK_INIT(file_write_at_all)         { GENERIC_CALL("MPI_File_write_at_all"); };
  CBACK_INIT(file_iread_at)             { GENERIC_CALL("MPI_File_iread_at"); };
  CBACK_INIT(file_iwrite_at)            { GENERIC_CALL("MPI_File_iwrite_at"); };
  CBACK_INIT(file_read)                 { GENERIC_CALL("MPI_File_read"); };
  CBACK_INIT(file_read_all)             { GENERIC_CALL("MPI_File_read_all"); };
  CBACK_INIT(file_write)                { GENERIC_CALL("MPI_File_write"); };
  CBACK_INIT(file_write_all)            { GENERIC_CALL("MPI_File_write_all"); };
  CBACK_INIT(file_iread)                { GENERIC_CALL("MPI_File_iread"); };
  CBACK_INIT(file_iwrite)               { GENERIC_CALL("MPI_File_iwrite"); };
  CBACK_INIT(file_seek)                 { GENERIC_CALL("MPI_File_seek"); };
  CBACK_INIT(file_get_position)         { GENERIC_CALL("MPI_File_get_position"); };
  CBACK_INIT(file_get_byte_offset)      { GENERIC_CALL("MPI_File_get_byte_offset"); };
  CBACK_INIT(file_read_shared)          { GENERIC_CALL("MPI_File_read_shared"); };
  CBACK_INIT(file_write_shared)         { GENERIC_CALL("MPI_File_write_shared"); };
  CBACK_INIT(file_iread_shared)         { GENERIC_CALL("MPI_File_iread_shared"); };
  CBACK_INIT(file_iwrite_shared)        { GENERIC_CALL("MPI_File_iwrite_shared"); };
  CBACK_INIT(file_read_ordered)         { GENERIC_CALL("MPI_File_read_ordered"); };
  CBACK_INIT(file_write_ordered)        { GENERIC_CALL("MPI_File_write_ordered"); };
  CBACK_INIT(file_seek_shared)          { GENERIC_CALL("MPI_File_seek_shared"); };
  CBACK_INIT(file_get_position_shared)  { GENERIC_CALL("MPI_File_get_position_shared"); };
  CBACK_INIT(file_read_at_all_begin)    { GENERIC_CALL("MPI_File_read_at_all_begin"); };
  CBACK_INIT(file_read_at_all_end)      { GENERIC_CALL("MPI_File_read_at_all_end"); };
  CBACK_INIT(file_write_at_all_begin)   { GENERIC_CALL("MPI_File_write_at_all_begin"); };
  CBACK_INIT(file_write_at_all_end)     { GENERIC_CALL("MPI_File_write_at_all_end"); };
  CBACK_INIT(file_read_all_begin)       { GENERIC_CALL("MPI_File_read_all_begin"); };
  CBACK_INIT(file_read_all_end)         { GENERIC_CALL("MPI_File_read_all_end"); };
  CBACK_INIT(file_write_all_begin)      { GENERIC_CALL("MPI_File_write_all_begin"); };
  CBACK_INIT(file_write_all_end)        { GENERIC_CALL("MPI_File_write_all_end"); };
  CBACK_INIT(file_read_ordered_begin)   { GENERIC_CALL("MPI_File_read_ordered_begin"); };
  CBACK_INIT(file_read_ordered_end)     { GENERIC_CALL("MPI_File_read_ordered_end"); };
  CBACK_INIT(file_write_ordered_begin)  { GENERIC_CALL("MPI_File_write_ordered_begin"); };
  CBACK_INIT(file_write_ordered_end)    { GENERIC_CALL("MPI_File_write_ordered_end"); };
  CBACK_INIT(file_get_type_extent)      { GENERIC_CALL("MPI_File_get_type_extent"); };
  CBACK_INIT(register_datarep)          { GENERIC_CALL("MPI_Register_datarep"); };
  CBACK_INIT(file_set_atomicity)        { GENERIC_CALL("MPI_File_set_atomicity"); };
  CBACK_INIT(file_get_atomicity)        { GENERIC_CALL("MPI_File_get_atomicity"); };
  CBACK_INIT(file_sync)                 { GENERIC_CALL("MPI_File_sync"); };
}

void set_first_pass_callbacks(libundumpi_callbacks *cbacks) {
  assert(cbacks != NULL);
  CBACK_INIT(comm_group)                { UNPACK_ARGS(); otf2_writer.mpi_comm_group_first_pass(p.comm, p.group); return 1; };
  CBACK_INIT(group_union)               { UNPACK_ARGS(); otf2_writer.mpi_group_union_first_pass(p.group1, p.group2, p.newgroup); return 1; };
  CBACK_INIT(group_intersection)        { UNPACK_ARGS(); otf2_writer.mpi_group_intersection_first_pass(p.group1, p.group2, p.newgroup); return 1; };
  CBACK_INIT(group_difference)          { UNPACK_ARGS(); otf2_writer.mpi_group_difference_first_pass(p.group1, p.group2, p.newgroup); return 1; };
  CBACK_INIT(group_incl)                { UNPACK_ARGS(); otf2_writer.mpi_group_incl_first_pass(p.group, p.count, p.ranks, p.newgroup); return 1; };
  CBACK_INIT(group_excl)                { UNPACK_ARGS(); otf2_writer.mpi_group_excl_first_pass(p.group, p.count, p.ranks, p.newgroup); return 1; };
  CBACK_INIT(group_range_incl)          { UNPACK_ARGS(); otf2_writer.mpi_group_range_incl_first_pass(p.group, p.count, p.ranges, p.newgroup); return 1; };
  CBACK_INIT(group_range_excl)          { GENERIC_CALL("MPI_Group_range_excl"); };
  CBACK_INIT(comm_dup)                  { UNPACK_ARGS(); otf2_writer.mpi_comm_dup_first_pass(p.oldcomm, p.newcomm); return 1; };
  CBACK_INIT(comm_create)               { UNPACK_ARGS(); otf2_writer.mpi_comm_create_first_pass(p.oldcomm, p.group, p.newcomm); return 1; };
  CBACK_INIT(comm_split)                { UNPACK_ARGS(); otf2_writer.mpi_comm_split_first_pass(p.oldcomm, p.color, p.key, p.newcomm); return 1; };
}
