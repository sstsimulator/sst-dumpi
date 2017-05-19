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

#ifndef DUMPI_COMMON_ARGTYPES_H
#define DUMPI_COMMON_ARGTYPES_H

#include <dumpi/common/types.h>
#include <dumpi/dumpiconfig-generated.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common
   * \defgroup common_argtypes common/argtypes:  Function call arguments \
   *     for all MPI calls.
   */
  /*@{*/

  /**
   * Hold the argument values for a function call
   */
  typedef struct dumpi_func_call {
    /** Function address as reported by the instrumentation function */
    uint64_t fn;
  } dumpi_func_call;

  /**
   * Hold the argument values for a send operation.
   */
  typedef struct dumpi_send {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_send;

  /**
   * Hold the argument values for a recv operation.
   */
  typedef struct dumpi_recv {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_source  source;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_recv;

  /**
   * Hold the argument values for a get_count operation.
   */
  typedef struct dumpi_get_count {
    /** Argument value before PMPI call */
    dumpi_status * status;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  count;
  } dumpi_get_count;

  /**
   * Hold the argument values for a bsend operation.
   */
  typedef struct dumpi_bsend {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_bsend;

  /**
   * Hold the argument values for a ssend operation.
   */
  typedef struct dumpi_ssend {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_ssend;

  /**
   * Hold the argument values for a rsend operation.
   */
  typedef struct dumpi_rsend {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_rsend;

  /**
   * Hold the argument values for a buffer_attach operation.
   */
  typedef struct dumpi_buffer_attach {
    /** Argument value before PMPI call */
    int  size;
  } dumpi_buffer_attach;

  /**
   * Hold the argument values for a buffer_detach operation.
   */
  typedef struct dumpi_buffer_detach {
    /** Argument value after PMPI call */
    int  size;
  } dumpi_buffer_detach;

  /**
   * Hold the argument values for a isend operation.
   */
  typedef struct dumpi_isend {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    int  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_isend;

  /**
   * Hold the argument values for a ibsend operation.
   */
  typedef struct dumpi_ibsend {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_ibsend;

  /**
   * Hold the argument values for a issend operation.
   */
  typedef struct dumpi_issend {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_issend;

  /**
   * Hold the argument values for a irsend operation.
   */
  typedef struct dumpi_irsend {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_irsend;

  /**
   * Hold the argument values for a irecv operation.
   */
  typedef struct dumpi_irecv {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_source  source;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_irecv;

  /**
   * Hold the argument values for a wait operation.
   */
  typedef struct dumpi_wait {
    /** Argument value before PMPI call */
    dumpi_request  request;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_wait;

  /**
   * Hold the argument values for a test operation.
   */
  typedef struct dumpi_test {
    /** Argument value before PMPI call */
    dumpi_request  request;
    /** Argument value after PMPI call */
    int  flag;
    /** Argument value before PMPI call.  Only stored if(flag != 0) */
    dumpi_status * status;
  } dumpi_test;

  /**
   * Hold the argument values for a request_free operation.
   */
  typedef struct dumpi_request_free {
    /** Argument value before PMPI call */
    dumpi_request  request;
  } dumpi_request_free;

  /**
   * Hold the argument values for a waitany operation.
   */
  typedef struct dumpi_waitany {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpi_request * requests;
    /** Argument value after PMPI call */
    int  index;
    /** Argument value after PMPI call.  Only stored if(index != MPI_UNDEFINED) */
    dumpi_status * status;
  } dumpi_waitany;

  /**
   * Hold the argument values for a testany operation.
   */
  typedef struct dumpi_testany {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpi_request * requests;
    /** Argument value after PMPI call */
    int  index;
    /** Argument value after PMPI call */
    int  flag;
    /** Argument value after PMPI call.  Only stored if(flag != 0) */
    dumpi_status * status;
  } dumpi_testany;

  /**
   * Hold the argument values for a waitall operation.
   */
  typedef struct dumpi_waitall {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpi_request * requests;
    /** Argument value after PMPI call.  Array of length [count] */
    dumpi_status * statuses;
  } dumpi_waitall;

  /**
   * Hold the argument values for a testall operation.
   */
  typedef struct dumpi_testall {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpi_request * requests;
    /** Argument value after PMPI call */
    int  flag;
    /**
     * Argument value after PMPI call.  Array of length [count].
     * Only stored if(flag != 0)
     */
    dumpi_status * statuses;
  } dumpi_testall;

  /**
   * Hold the argument values for a waitsome operation.
   */
  typedef struct dumpi_waitsome {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpi_request * requests;
    /** Argument value after PMPI call */
    int  outcount;
    /** Argument value after PMPI call.  Array of length [*outcount] */
    int * indices;
    /** Argument value after PMPI call.  Array of length [*outcount] */
    dumpi_status * statuses;
  } dumpi_waitsome;

  /**
   * Hold the argument values for a testsome operation.
   */
  typedef struct dumpi_testsome {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpi_request * requests;
    /** Argument value after PMPI call */
    int  outcount;
    /** Argument value after PMPI call.  Array of length [*outcount] */
    int * indices;
    /** Argument value after PMPI call.  Array of length [*outcount] */
    dumpi_status * statuses;
  } dumpi_testsome;

  /**
   * Hold the argument values for a iprobe operation.
   */
  typedef struct dumpi_iprobe {
    /** Argument value before PMPI call */
    dumpi_source  source;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    int  flag;
    /** Argument value after PMPI call.  Only stored if(flag != 0) */
    dumpi_status * status;
  } dumpi_iprobe;

  /**
   * Hold the argument values for a probe operation.
   */
  typedef struct dumpi_probe {
    /** Argument value before PMPI call */
    dumpi_source  source;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_probe;

  /**
   * Hold the argument values for a cancel operation.
   */
  typedef struct dumpi_cancel {
    /** Argument value before PMPI call */
    dumpi_request  request;
  } dumpi_cancel;

  /**
   * Hold the argument values for a test_cancelled operation.
   */
  typedef struct dumpi_test_cancelled {
    /** Argument value before PMPI call */
    dumpi_status * status;
    /** Argument value after PMPI call */
    int  cancelled;
  } dumpi_test_cancelled;

  /**
   * Hold the argument values for a send_init operation.
   */
  typedef struct dumpi_send_init {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_send_init;

  /**
   * Hold the argument values for a bsend_init operation.
   */
  typedef struct dumpi_bsend_init {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_bsend_init;

  /**
   * Hold the argument values for a ssend_init operation.
   */
  typedef struct dumpi_ssend_init {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_ssend_init;

  /**
   * Hold the argument values for a rsend_init operation.
   */
  typedef struct dumpi_rsend_init {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_rsend_init;

  /**
   * Hold the argument values for a recv_init operation.
   */
  typedef struct dumpi_recv_init {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_source  source;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_recv_init;

  /**
   * Hold the argument values for a start operation.
   */
  typedef struct dumpi_start {
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_start;

  /**
   * Hold the argument values for a startall operation.
   */
  typedef struct dumpi_startall {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value after PMPI call.  Array of length [count] */
    dumpi_request * requests;
  } dumpi_startall;

  /**
   * Hold the argument values for a sendrecv operation.
   */
  typedef struct dumpi_sendrecv {
    /** Argument value before PMPI call */
    int  sendcount;
    /** Argument value before PMPI call */
    dumpi_datatype  sendtype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  sendtag;
    /** Argument value before PMPI call */
    int  recvcount;
    /** Argument value before PMPI call */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    dumpi_source  source;
    /** Argument value before PMPI call */
    dumpi_tag  recvtag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_sendrecv;

  /**
   * Hold the argument values for a sendrecv_replace operation.
   */
  typedef struct dumpi_sendrecv_replace {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_dest  dest;
    /** Argument value before PMPI call */
    dumpi_tag  sendtag;
    /** Argument value before PMPI call */
    dumpi_source  source;
    /** Argument value before PMPI call */
    dumpi_tag  recvtag;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_sendrecv_replace;

  /**
   * Hold the argument values for a type_contiguous operation.
   */
  typedef struct dumpi_type_contiguous {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_contiguous;

  /**
   * Hold the argument values for a type_vector operation.
   */
  typedef struct dumpi_type_vector {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    int  blocklength;
    /** Argument value before PMPI call */
    int  stride;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_vector;

  /**
   * Hold the argument values for a type_hvector operation.
   */
  typedef struct dumpi_type_hvector {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    int  blocklength;
    /** Argument value before PMPI call */
    int  stride;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_hvector;

  /**
   * Hold the argument values for a type_indexed operation.
   */
  typedef struct dumpi_type_indexed {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    int * lengths;
    /** Argument value before PMPI call.  Array of length [count] */
    int * indices;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_indexed;

  /**
   * Hold the argument values for a type_hindexed operation.
   */
  typedef struct dumpi_type_hindexed {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    int * lengths;
    /** Argument value before PMPI call.  Array of length [count] */
    int * indices;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_hindexed;

  /**
   * Hold the argument values for a type_struct operation.
   */
  typedef struct dumpi_type_struct {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    int * lengths;
    /** Argument value before PMPI call */
    int  * indices;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpi_datatype * oldtypes;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_struct;

  /**
   * Hold the argument values for a address operation.
   */
  typedef struct dumpi_address {
    /** Argument value after PMPI call */
    int  address;
  } dumpi_address;

  /**
   * Hold the argument values for a type_extent operation.
   */
  typedef struct dumpi_type_extent {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  extent;
  } dumpi_type_extent;

  /**
   * Hold the argument values for a type_size operation.
   */
  typedef struct dumpi_type_size {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  size;
  } dumpi_type_size;

  /**
   * Hold the argument values for a type_lb operation.
   */
  typedef struct dumpi_type_lb {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  lb;
  } dumpi_type_lb;

  /**
   * Hold the argument values for a type_ub operation.
   */
  typedef struct dumpi_type_ub {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  ub;
  } dumpi_type_ub;

  /**
   * Hold the argument values for a type_commit operation.
   */
  typedef struct dumpi_type_commit {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_type_commit;

  /**
   * Hold the argument values for a type_free operation.
   */
  typedef struct dumpi_type_free {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_type_free;

  /**
   * Hold the argument values for a get_elements operation.
   */
  typedef struct dumpi_get_elements {
    /** Argument value before PMPI call */
    dumpi_status * status;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  elements;
  } dumpi_get_elements;

  /**
   * Hold the argument values for a pack operation.
   */
  typedef struct dumpi_pack {
    /** Argument value before PMPI call */
    int  incount;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    int  outcount;
    /** Argument value before and after PMPI call. */
    struct { int in, out; }  position;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_pack;

  /**
   * Hold the argument values for a unpack operation.
   */
  typedef struct dumpi_unpack {
    /** Argument value before PMPI call */
    int  incount;
    /** Argument value before and after PMPI call. */
    struct { int in, out; }  position;
    /** Argument value before PMPI call */
    int  outcount;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_unpack;

  /**
   * Hold the argument values for a pack_size operation.
   */
  typedef struct dumpi_pack_size {
    /** Argument value before PMPI call */
    int  incount;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    int  size;
  } dumpi_pack_size;

  /**
   * Hold the argument values for a barrier operation.
   */
  typedef struct dumpi_barrier {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_barrier;

  /**
   * Hold the argument values for a bcast operation.
   */
  typedef struct dumpi_bcast {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_bcast;

  /**
   * Hold the argument values for a gather operation.
   */
  typedef struct dumpi_gather {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commrank;
    /** Argument value before PMPI call */
    int  sendcount;
    /** Argument value before PMPI call */
    dumpi_datatype  sendtype;
    /** Argument value before PMPI call.  Only stored if(commrank==root) */
    int  recvcount;
    /** Argument value before PMPI call.  Only stored if(commrank==root) */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_gather;

  /**
   * Hold the argument values for a gatherv operation.
   */
  typedef struct dumpi_gatherv {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commrank;
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commsize;
    /** Argument value before PMPI call */
    int  sendcount;
    /** Argument value before PMPI call */
    dumpi_datatype  sendtype;
    /**
     * Argument value before PMPI call.  Array of length [commsize].
     * Only stored if(commrank==root)
     */
    int * recvcounts;
    /**
     * Argument value before PMPI call.  Array of length [commsize].
     * Only stored if(commrank==root)
     */
    int * displs;
    /** Argument value before PMPI call.  Only stored if(commrank==root) */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_gatherv;

  /**
   * Hold the argument values for a scatter operation.
   */
  typedef struct dumpi_scatter {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commrank;
    /** Argument value before PMPI call.  Only stored if(commrank==root) */
    int  sendcount;
    /** Argument value before PMPI call.  Only stored if(commrank==root) */
    dumpi_datatype  sendtype;
    /** Argument value before PMPI call */
    int  recvcount;
    /** Argument value before PMPI call */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_scatter;

  /**
   * Hold the argument values for a scatterv operation.
   */
  typedef struct dumpi_scatterv {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commrank;
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commsize;
    /**
     * Argument value before PMPI call.  Array of length [commsize].
     * Only stored if(commrank==root)
     */
    int * sendcounts;
    /**
     * Argument value before PMPI call.  Array of length [commsize].
     * Only stored if(commrank==root)
     */
    int * displs;
    /** Argument value before PMPI call */
    dumpi_datatype  sendtype;
    /** Argument value before PMPI call */
    int  recvcount;
    /** Argument value before PMPI call */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_scatterv;

  /**
   * Hold the argument values for a allgather operation.
   */
  typedef struct dumpi_allgather {
    /** Argument value before PMPI call */
    int  sendcount;
    /** Argument value before PMPI call */
    dumpi_datatype  sendtype;
    /** Argument value before PMPI call */
    int  recvcount;
    /** Argument value before PMPI call */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_allgather;

  /**
   * Hold the argument values for a allgatherv operation.
   */
  typedef struct dumpi_allgatherv {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commsize;
    /** Argument value before PMPI call */
    int  sendcount;
    /** Argument value before PMPI call */
    dumpi_datatype  sendtype;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * recvcounts;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * displs;
    /** Argument value before PMPI call */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_allgatherv;

  /**
   * Hold the argument values for a alltoall operation.
   */
  typedef struct dumpi_alltoall {
    /** Argument value before PMPI call */
    int  sendcount;
    /** Argument value before PMPI call */
    dumpi_datatype  sendtype;
    /** Argument value before PMPI call */
    int  recvcount;
    /** Argument value before PMPI call */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_alltoall;

  /**
   * Hold the argument values for a alltoallv operation.
   */
  typedef struct dumpi_alltoallv {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commsize;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * sendcounts;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * senddispls;
    /** Argument value before PMPI call */
    dumpi_datatype  sendtype;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * recvcounts;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * recvdispls;
    /** Argument value before PMPI call */
    dumpi_datatype  recvtype;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_alltoallv;

  /**
   * Hold the argument values for a reduce operation.
   */
  typedef struct dumpi_reduce {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_op  op;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_reduce;

  /**
   * Hold the argument values for a op_create operation.
   */
  typedef struct dumpi_op_create {
    /** Argument value before PMPI call */
    int  commute;
    /** Argument value after PMPI call */
    dumpi_op  op;
  } dumpi_op_create;

  /**
   * Hold the argument values for a op_free operation.
   */
  typedef struct dumpi_op_free {
    /** Argument value before PMPI call */
    dumpi_op  op;
  } dumpi_op_free;

  /**
   * Hold the argument values for a allreduce operation.
   */
  typedef struct dumpi_allreduce {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_op  op;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_allreduce;

  /**
   * Hold the argument values for a reduce_scatter operation.
   */
  typedef struct dumpi_reduce_scatter {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commsize;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * recvcounts;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_op  op;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_reduce_scatter;

  /**
   * Hold the argument values for a scan operation.
   */
  typedef struct dumpi_scan {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_op  op;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_scan;

  /**
   * Hold the argument values for a group_size operation.
   */
  typedef struct dumpi_group_size {
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value after PMPI call */
    int  size;
  } dumpi_group_size;

  /**
   * Hold the argument values for a group_rank operation.
   */
  typedef struct dumpi_group_rank {
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value after PMPI call */
    int  rank;
  } dumpi_group_rank;

  /**
   * Hold the argument values for a group_translate_ranks operation.
   */
  typedef struct dumpi_group_translate_ranks {
    /** Argument value before PMPI call */
    dumpi_group  group1;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    int * ranks1;
    /** Argument value before PMPI call */
    dumpi_group  group2;
    /** Argument value after PMPI call.  Array of length [count] */
    int * ranks2;
  } dumpi_group_translate_ranks;

  /**
   * Hold the argument values for a group_compare operation.
   */
  typedef struct dumpi_group_compare {
    /** Argument value before PMPI call */
    dumpi_group  group1;
    /** Argument value before PMPI call */
    dumpi_group  group2;
    /** Argument value after PMPI call */
    dumpi_comparison  result;
  } dumpi_group_compare;

  /**
   * Hold the argument values for a comm_group operation.
   */
  typedef struct dumpi_comm_group {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_group  group;
  } dumpi_comm_group;

  /**
   * Hold the argument values for a group_union operation.
   */
  typedef struct dumpi_group_union {
    /** Argument value before PMPI call */
    dumpi_group  group1;
    /** Argument value before PMPI call */
    dumpi_group  group2;
    /** Argument value after PMPI call */
    dumpi_group  newgroup;
  } dumpi_group_union;

  /**
   * Hold the argument values for a group_intersection operation.
   */
  typedef struct dumpi_group_intersection {
    /** Argument value before PMPI call */
    dumpi_group  group1;
    /** Argument value before PMPI call */
    dumpi_group  group2;
    /** Argument value after PMPI call */
    dumpi_group  newgroup;
  } dumpi_group_intersection;

  /**
   * Hold the argument values for a group_difference operation.
   */
  typedef struct dumpi_group_difference {
    /** Argument value before PMPI call */
    dumpi_group  group1;
    /** Argument value before PMPI call */
    dumpi_group  group2;
    /** Argument value after PMPI call */
    dumpi_group  newgroup;
  } dumpi_group_difference;

  /**
   * Hold the argument values for a group_incl operation.
   */
  typedef struct dumpi_group_incl {
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value before PMPI call */
    int count;
    /** Argument value before PMPI call.  Array of length [count] */
    int * ranks;
    /** Argument value after PMPI call */
    dumpi_group  newgroup;
  } dumpi_group_incl;

  /**
   * Hold the argument values for a group_excl operation.
   */
  typedef struct dumpi_group_excl {
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    int * ranks;
    /** Argument value after PMPI call */
    dumpi_group  newgroup;
  } dumpi_group_excl;

  /**
   * Hold the argument values for a group_range_incl operation.
   */
  typedef struct dumpi_group_range_incl {
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  2D-aray of length [count][3] */
    int ** ranges;
    /** Argument value after PMPI call */
    dumpi_group  newgroup;
  } dumpi_group_range_incl;

  /**
   * Hold the argument values for a group_range_excl operation.
   */
  typedef struct dumpi_group_range_excl {
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  2D-array of length [count][3] */
    int ** ranges;
    /** Argument value after PMPI call */
    dumpi_group  newgroup;
  } dumpi_group_range_excl;

  /**
   * Hold the argument values for a group_free operation.
   */
  typedef struct dumpi_group_free {
    /** Argument value before PMPI call */
    dumpi_group  group;
  } dumpi_group_free;

  /**
   * Hold the argument values for a comm_size operation.
   */
  typedef struct dumpi_comm_size {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    int  size;
  } dumpi_comm_size;

  /**
   * Hold the argument values for a comm_rank operation.
   */
  typedef struct dumpi_comm_rank {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    int  rank;
  } dumpi_comm_rank;

  /**
   * Hold the argument values for a comm_compare operation.
   */
  typedef struct dumpi_comm_compare {
    /** Argument value before PMPI call */
    dumpi_comm  comm1;
    /** Argument value before PMPI call */
    dumpi_comm  comm2;
    /** Argument value after PMPI call */
    dumpi_comparison  result;
  } dumpi_comm_compare;

  /**
   * Hold the argument values for a comm_dup operation.
   */
  typedef struct dumpi_comm_dup {
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_comm_dup;

  /**
   * Hold the argument values for a comm_create operation.
   */
  typedef struct dumpi_comm_create {
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_comm_create;

  /**
   * Hold the argument values for a comm_split operation.
   */
  typedef struct dumpi_comm_split {
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value before PMPI call */
    int  color;
    /** Argument value before PMPI call */
    int  key;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_comm_split;

  /**
   * Hold the argument values for a comm_free operation.
   */
  typedef struct dumpi_comm_free {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_comm_free;

  /**
   * Hold the argument values for a comm_test_inter operation.
   */
  typedef struct dumpi_comm_test_inter {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    int  inter;
  } dumpi_comm_test_inter;

  /**
   * Hold the argument values for a comm_remote_size operation.
   */
  typedef struct dumpi_comm_remote_size {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    int  size;
  } dumpi_comm_remote_size;

  /**
   * Hold the argument values for a comm_remote_group operation.
   */
  typedef struct dumpi_comm_remote_group {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_group  group;
  } dumpi_comm_remote_group;

  /**
   * Hold the argument values for a intercomm_create operation.
   */
  typedef struct dumpi_intercomm_create {
    /** Argument value before PMPI call */
    dumpi_comm  localcomm;
    /** Argument value before PMPI call */
    int  localleader;
    /** Argument value before PMPI call */
    dumpi_comm  remotecomm;
    /** Argument value before PMPI call */
    int  remoteleader;
    /** Argument value before PMPI call */
    dumpi_tag  tag;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_intercomm_create;

  /**
   * Hold the argument values for a intercomm_merge operation.
   */
  typedef struct dumpi_intercomm_merge {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  high;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_intercomm_merge;

  /**
   * Hold the argument values for a keyval_create operation.
   */
  typedef struct dumpi_keyval_create {
    /** Argument value after PMPI call */
    dumpi_keyval  key;
  } dumpi_keyval_create;

  /**
   * Hold the argument values for a keyval_free operation.
   */
  typedef struct dumpi_keyval_free {
    /** Argument value before PMPI call */
    dumpi_keyval  key;
  } dumpi_keyval_free;

  /**
   * Hold the argument values for a attr_put operation.
   */
  typedef struct dumpi_attr_put {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  key;
  } dumpi_attr_put;

  /**
   * Hold the argument values for a attr_get operation.
   */
  typedef struct dumpi_attr_get {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  key;
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_attr_get;

  /**
   * Hold the argument values for a attr_delete operation.
   */
  typedef struct dumpi_attr_delete {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  key;
  } dumpi_attr_delete;

  /**
   * Hold the argument values for a topo_test operation.
   */
  typedef struct dumpi_topo_test {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_topology  topo;
  } dumpi_topo_test;

  /**
   * Hold the argument values for a cart_create operation.
   */
  typedef struct dumpi_cart_create {
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value before PMPI call */
    int  ndim;
    /** Argument value before PMPI call.  Array of length [ndim] */
    int * dims;
    /** Argument value before PMPI call.  Array of length [ndim] */
    int * periods;
    /** Argument value before PMPI call */
    int  reorder;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_cart_create;

  /**
   * Hold the argument values for a dims_create operation.
   */
  typedef struct dumpi_dims_create {
    /** Argument value before PMPI call */
    int  nodes;
    /** Argument value before PMPI call */
    int  ndim;
    /** Argument value before and after PMPI call. Array of lenght [ndim] */
    struct { int *in, *out; }  dims;
  } dumpi_dims_create;

  /**
   * Hold the argument values for a graph_create operation.
   */
  typedef struct dumpi_graph_create {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  numedges;
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value before PMPI call */
    int  nodes;
    /** Argument value before PMPI call.  Array of length [nodes] */
    int * index;
    /** Argument value before PMPI call.  Array of length [numedges] */
    int * edges;
    /** Argument value before PMPI call */
    int  reorder;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_graph_create;

  /**
   * Hold the argument values for a graphdims_get operation.
   */
  typedef struct dumpi_graphdims_get {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    int  nodes;
    /** Argument value after PMPI call */
    int  edges;
  } dumpi_graphdims_get;

  /**
   * Hold the argument values for a graph_get operation.
   */
  typedef struct dumpi_graph_get {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  totedges;
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  totnodes;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  maxindex;
    /** Argument value before PMPI call */
    int  maxedges;
    /** Argument value after PMPI call.  Array of length [totnodes] */
    int * index;
    /** Argument value after PMPI call.  Array of length [totedges] */
    int * edges;
  } dumpi_graph_get;

  /**
   * Hold the argument values for a cartdim_get operation.
   */
  typedef struct dumpi_cartdim_get {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    int  ndim;
  } dumpi_cartdim_get;

  /**
   * Hold the argument values for a cart_get operation.
   */
  typedef struct dumpi_cart_get {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  ndim;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  maxdims;
    /** Argument value after PMPI call.  Array of length [ndim] */
    int * dims;
    /** Argument value after PMPI call.  Array of length [ndim] */
    int * periods;
    /** Argument value after PMPI call */
    int * coords;
  } dumpi_cart_get;

  /**
   * Hold the argument values for a cart_rank operation.
   */
  typedef struct dumpi_cart_rank {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  ndim;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call.  Array of length [ndim] */
    int * coords;
    /** Argument value after PMPI call */
    int  rank;
  } dumpi_cart_rank;

  /**
   * Hold the argument values for a cart_coords operation.
   */
  typedef struct dumpi_cart_coords {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  ndim;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  rank;
    /** Argument value before PMPI call */
    int  maxdims;
    /** Argument value after PMPI call.  Array of length [ndim] */
    int * coords;
  } dumpi_cart_coords;

  /**
   * Hold the argument values for a graph_neighbors_count operation.
   */
  typedef struct dumpi_graph_neighbors_count {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  rank;
    /** Argument value after PMPI call */
    int  nneigh;
  } dumpi_graph_neighbors_count;

  /**
   * Hold the argument values for a graph_neighbors operation.
   */
  typedef struct dumpi_graph_neighbors {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  nneigh;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  rank;
    /** Argument value before PMPI call */
    int  maxneighbors;
    /** Argument value after PMPI call.  Array of length [nneigh] */
    int * neighbors;
  } dumpi_graph_neighbors;

  /**
   * Hold the argument values for a cart_shift operation.
   */
  typedef struct dumpi_cart_shift {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  direction;
    /** Argument value before PMPI call */
    int  displ;
    /** Argument value after PMPI call */
    dumpi_source  source;
    /** Argument value after PMPI call */
    dumpi_dest  dest;
  } dumpi_cart_shift;

  /**
   * Hold the argument values for a cart_sub operation.
   */
  typedef struct dumpi_cart_sub {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  ndim;
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value before PMPI call.  Array of length [ndim] */
    int * remain_dims;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_cart_sub;

  /**
   * Hold the argument values for a cart_map operation.
   */
  typedef struct dumpi_cart_map {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  ndim;
    /** Argument value before PMPI call.  Array of length [ndim] */
    int * dims;
    /** Argument value before PMPI call.  Array of length [ndim] */
    int * period;
    /** Argument value after PMPI call */
    int  newrank;
  } dumpi_cart_map;

  /**
   * Hold the argument values for a graph_map operation.
   */
  typedef struct dumpi_graph_map {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  numedges;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  nodes;
    /** Argument value before PMPI call.  Array of length [nodes] */
    int * index;
    /** Argument value before PMPI call.  Array of length [numedges] */
    int * edges;
    /** Argument value after PMPI call */
    int  newrank;
  } dumpi_graph_map;

  /**
   * Hold the argument values for a get_processor_name operation.
   */
  typedef struct dumpi_get_processor_name {
    /** Argument value after PMPI call.  Array of length [*resultlen] */
    char * name;
    /** Argument value after PMPI call */
    int  resultlen;
  } dumpi_get_processor_name;

  /**
   * Hold the argument values for a get_version operation.
   */
  typedef struct dumpi_get_version {
    /** Argument value after PMPI call */
    int  version;
    /** Argument value after PMPI call */
    int  subversion;
  } dumpi_get_version;

  /**
   * Hold the argument values for a errhandler_create operation.
   */
  typedef struct dumpi_errhandler_create {
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_errhandler_create;

  /**
   * Hold the argument values for a errhandler_set operation.
   */
  typedef struct dumpi_errhandler_set {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_errhandler_set;

  /**
   * Hold the argument values for a errhandler_get operation.
   */
  typedef struct dumpi_errhandler_get {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_errhandler_get;

  /**
   * Hold the argument values for a errhandler_free operation.
   */
  typedef struct dumpi_errhandler_free {
    /** Argument value before PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_errhandler_free;

  /**
   * Hold the argument values for a error_string operation.
   */
  typedef struct dumpi_error_string {
    /** Argument value before PMPI call */
    int  errorcode;
    /** Argument value after PMPI call.  Array of length [*resultlen] */
    char * errorstring;
    /** Argument value after PMPI call */
    int  resultlen;
  } dumpi_error_string;

  /**
   * Hold the argument values for a error_class operation.
   */
  typedef struct dumpi_error_class {
    /** Argument value before PMPI call */
    int  errorcode;
    /** Argument value after PMPI call */
    int  errorclass;
  } dumpi_error_class;

  /**
   * Hold the argument values for a wtime operation.
   * This was actually messed up prior to version 0.6.7.
   */
  typedef struct dumpi_wtime {
    uint64_t psec;
  } dumpi_wtime;

  /**
   * Hold the argument values for a wtick operation.
   * This was actually messed up prior to version 0.6.7.
   */
  typedef struct dumpi_wtick {
    uint64_t psec;
  } dumpi_wtick;

  /**
   * Hold the argument values for a init operation.
   */
  typedef struct dumpi_init {
    /** Argument value before PMPI call */
    int  argc;
    /** 
     * Argument value before PMPI call.  
     * Array of length [argc] containing NUL-terminated std::strings.
     */
    char ** argv;
  } dumpi_init;

  /**
   * Hold the argument values for a finalize operation.
   */
  typedef struct dumpi_finalize {
    int dummy;
  } dumpi_finalize;

  /**
   * Hold the argument values for a initialized operation.
   */
  typedef struct dumpi_initialized {
    /** Argument value after PMPI call */
    int  result;
  } dumpi_initialized;

  /**
   * Hold the argument values for a abort operation.
   */
  typedef struct dumpi_abort {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  errorcode;
  } dumpi_abort;

  /**
   * Hold the argument values for a close_port operation.
   */
  typedef struct dumpi_close_port {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * portname;
  } dumpi_close_port;

  /**
   * Hold the argument values for a comm_accept operation.
   */
  typedef struct dumpi_comm_accept {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * portname;
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_comm_accept;

  /**
   * Hold the argument values for a comm_connect operation.
   */
  typedef struct dumpi_comm_connect {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * portname;
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
  } dumpi_comm_connect;

  /**
   * Hold the argument values for a comm_disconnect operation.
   */
  typedef struct dumpi_comm_disconnect {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_comm_disconnect;

  /**
   * Hold the argument values for a comm_get_parent operation.
   */
  typedef struct dumpi_comm_get_parent {
    /** Argument value after PMPI call */
    dumpi_comm  parent;
  } dumpi_comm_get_parent;

  /**
   * Hold the argument values for a comm_join operation.
   */
  typedef struct dumpi_comm_join {
    /** Argument value before PMPI call */
    int  fd;
    /** Argument value after PMPI call */
    dumpi_comm  comm;
  } dumpi_comm_join;

  /**
   * Hold the argument values for a comm_spawn operation.
   */
  typedef struct dumpi_comm_spawn {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  oldcommrank;
    /**
     * Argument value before PMPI call.  NUL-terminated std::string.
     * Only stored if(oldcommrank==root)
     */
    char * command;
    /**
     * Argument value before PMPI call.  
     * Null-terminated array of containing NUL-terminated std::strings.
     * Only stored if(oldcommrank==root)
     */
    char ** argv;
    /** Argument value before PMPI call.  Only stored if(oldcommrank==root) */
    int  maxprocs;
    /** Argument value before PMPI call.  Only stored if(oldcommrank==root) */
    dumpi_info  info;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
    /**
     * Argument value after PMPI call.  Array of length [maxprocs].
     * Only stored if(oldcommrank==root)
     */
    dumpi_errcode * errcodes;
  } dumpi_comm_spawn;

  /**
   * Hold the argument values for a comm_spawn_multiple operation.
   */
  typedef struct dumpi_comm_spawn_multiple {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  totprocs;
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  oldcommrank;
    /** Argument value before PMPI call.  Only stored if(oldcommrank==root) */
    int  count;
    /**
     * Argument value before PMPI call.  
     * Array of length [count] containing NUL-terminated std::strings.
     * Only stored if(oldcommrank==root)
     */
    char ** commands;
    /**
     * Argument value before PMPI call.
     * 3D-array of length [count], with second-dimension NULL-terminated,
     * and third dimension NUL-terminated std::strings (MPI_Comm_spawn_multiple
     * is completely insane).
     * Only stored if(oldcommrank==root)
     */
    char *** argvs;
    /**
     * Argument value before PMPI call.  Array of length [count].
     * Only stored if(oldcommrank==root)
     */
    int * maxprocs;
    /**
     * Argument value before PMPI call.  Array of length [count].
     * Only stored if(oldcommrank==root)
     */
    dumpi_info * info;
    /** Argument value before PMPI call */
    int  root;
    /** Argument value before PMPI call */
    dumpi_comm  oldcomm;
    /** Argument value after PMPI call */
    dumpi_comm  newcomm;
    /**
     * Argument value after PMPI call.  Array of length [totprocs].
     * Only stored if(oldcommrank==root)
     */
    dumpi_errcode * errcodes;
  } dumpi_comm_spawn_multiple;

  /**
   * Hold the argument values for a lookup_name operation.
   */
  typedef struct dumpi_lookup_name {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * servicename;
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value after PMPI call.  NUL-terminated std::string. */
    char * portname;
  } dumpi_lookup_name;

  /**
   * Hold the argument values for a open_port operation.
   */
  typedef struct dumpi_open_port {
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value after PMPI call.  NUL-terminated std::string */
    char * portname;
  } dumpi_open_port;

  /**
   * Hold the argument values for a publish_name operation.
   */
  typedef struct dumpi_publish_name {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * servicename;
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * portname;
  } dumpi_publish_name;

  /**
   * Hold the argument values for a unpublish_name operation.
   */
  typedef struct dumpi_unpublish_name {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * servicename;
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * portname;
  } dumpi_unpublish_name;

  /**
   * Hold the argument values for a accumulate operation.
   */
  typedef struct dumpi_accumulate {
    /** Argument value before PMPI call */
    int  origincount;
    /** Argument value before PMPI call */
    dumpi_datatype  origintype;
    /** Argument value before PMPI call */
    int  targetrank;
    /** Argument value before PMPI call */
    int  targetdisp;
    /** Argument value before PMPI call */
    int  targetcount;
    /** Argument value before PMPI call */
    dumpi_datatype  targettype;
    /** Argument value before PMPI call */
    dumpi_op  op;
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_accumulate;

  /**
   * Hold the argument values for a get operation.
   */
  typedef struct dumpi_get {
    /** Argument value before PMPI call */
    int  origincount;
    /** Argument value before PMPI call */
    dumpi_datatype  origintype;
    /** Argument value before PMPI call */
    int  targetrank;
    /** Argument value before PMPI call */
    int  targetdisp;
    /** Argument value before PMPI call */
    int  targetcount;
    /** Argument value before PMPI call */
    dumpi_datatype  targettype;
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_get;

  /**
   * Hold the argument values for a put operation.
   */
  typedef struct dumpi_put {
    /** Argument value before PMPI call */
    int  origincount;
    /** Argument value before PMPI call */
    dumpi_datatype  origintype;
    /** Argument value before PMPI call */
    int  targetrank;
    /** Argument value before PMPI call */
    int  targetdisp;
    /** Argument value before PMPI call */
    int  targetcount;
    /** Argument value before PMPI call */
    dumpi_datatype  targettype;
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_put;

  /**
   * Hold the argument values for a win_complete operation.
   */
  typedef struct dumpi_win_complete {
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_win_complete;

  /**
   * Hold the argument values for a win_create operation.
   */
  typedef struct dumpi_win_create {
    /** Argument value before PMPI call */
    int  size;
    /** Argument value before PMPI call */
    int  dispunit;
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_win  win;
  } dumpi_win_create;

  /**
   * Hold the argument values for a win_fence operation.
   */
  typedef struct dumpi_win_fence {
    /** Argument value before PMPI call */
    dumpi_win_assert  assertion;
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_win_fence;

  /**
   * Hold the argument values for a win_free operation.
   */
  typedef struct dumpi_win_free {
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_win_free;

  /**
   * Hold the argument values for a win_get_group operation.
   */
  typedef struct dumpi_win_get_group {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value after PMPI call */
    dumpi_group  group;
  } dumpi_win_get_group;

  /**
   * Hold the argument values for a win_lock operation.
   */
  typedef struct dumpi_win_lock {
    /** Argument value before PMPI call */
    dumpi_locktype  locktype;
    /** Argument value before PMPI call */
    int  winrank;
    /** Argument value before PMPI call */
    dumpi_win_assert  assertion;
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_win_lock;

  /**
   * Hold the argument values for a win_post operation.
   */
  typedef struct dumpi_win_post {
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value before PMPI call */
    dumpi_win_assert  assertion;
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_win_post;

  /**
   * Hold the argument values for a win_start operation.
   */
  typedef struct dumpi_win_start {
    /** Argument value before PMPI call */
    dumpi_group  group;
    /** Argument value before PMPI call */
    dumpi_win_assert  assertion;
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_win_start;

  /**
   * Hold the argument values for a win_test operation.
   */
  typedef struct dumpi_win_test {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_win_test;

  /**
   * Hold the argument values for a win_unlock operation.
   */
  typedef struct dumpi_win_unlock {
    /** Argument value before PMPI call */
    int  winrank;
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_win_unlock;

  /**
   * Hold the argument values for a win_wait operation.
   */
  typedef struct dumpi_win_wait {
    /** Argument value before PMPI call */
    dumpi_win  win;
  } dumpi_win_wait;

  /**
   * Hold the argument values for a alltoallw operation.
   */
  typedef struct dumpi_alltoallw {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  commsize;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * sendcounts;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * senddispls;
    /** Argument value before PMPI call.  Array of length [commsize] */
    dumpi_datatype * sendtypes;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * recvcounts;
    /** Argument value before PMPI call.  Array of length [commsize] */
    int * recvdispls;
    /** Argument value before PMPI call.  Array of length [commsize] */
    dumpi_datatype * recvtypes;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_alltoallw;

  /**
   * Hold the argument values for a exscan operation.
   */
  typedef struct dumpi_exscan {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_op  op;
    /** Argument value before PMPI call */
    dumpi_comm  comm;
  } dumpi_exscan;

  /**
   * Hold the argument values for a add_error_class operation.
   */
  typedef struct dumpi_add_error_class {
    /** Argument value after PMPI call */
    int  errorclass;
  } dumpi_add_error_class;

  /**
   * Hold the argument values for a add_error_code operation.
   */
  typedef struct dumpi_add_error_code {
    /** Argument value before PMPI call */
    int  errorclass;
    /** Argument value after PMPI call */
    int  errorcode;
  } dumpi_add_error_code;

  /**
   * Hold the argument values for a add_error_string operation.
   */
  typedef struct dumpi_add_error_string {
    /** Argument value before PMPI call */
    int  errorcode;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * errorstring;
  } dumpi_add_error_string;

  /**
   * Hold the argument values for a comm_call_errhandler operation.
   */
  typedef struct dumpi_comm_call_errhandler {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    int  errorcode;
  } dumpi_comm_call_errhandler;

  /**
   * Hold the argument values for a comm_create_keyval operation.
   */
  typedef struct dumpi_comm_create_keyval {
    /** Argument value after PMPI call */
    dumpi_comm_keyval  keyval;
  } dumpi_comm_create_keyval;

  /**
   * Hold the argument values for a comm_delete_attr operation.
   */
  typedef struct dumpi_comm_delete_attr {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    dumpi_comm_keyval  keyval;
  } dumpi_comm_delete_attr;

  /**
   * Hold the argument values for a comm_free_keyval operation.
   */
  typedef struct dumpi_comm_free_keyval {
    /** Argument value before PMPI call */
    dumpi_comm_keyval  keyval;
  } dumpi_comm_free_keyval;

  /**
   * Hold the argument values for a comm_get_attr operation.
   */
  typedef struct dumpi_comm_get_attr {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    dumpi_comm_keyval  keyval;
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_comm_get_attr;

  /**
   * Hold the argument values for a comm_get_name operation.
   */
  typedef struct dumpi_comm_get_name {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call.  Array of length [*resultlen] */
    char * name;
    /** Argument value after PMPI call */
    int  resultlen;
  } dumpi_comm_get_name;

  /**
   * Hold the argument values for a comm_set_attr operation.
   */
  typedef struct dumpi_comm_set_attr {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    dumpi_comm_keyval  keyval;
  } dumpi_comm_set_attr;

  /**
   * Hold the argument values for a comm_set_name operation.
   */
  typedef struct dumpi_comm_set_name {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * name;
  } dumpi_comm_set_name;

  /**
   * Hold the argument values for a file_call_errhandler operation.
   */
  typedef struct dumpi_file_call_errhandler {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  errorcode;
  } dumpi_file_call_errhandler;

  /**
   * Hold the argument values for a grequest_complete operation.
   */
  typedef struct dumpi_grequest_complete {
    /** Argument value before PMPI call */
    dumpi_request  request;
  } dumpi_grequest_complete;

  /**
   * Hold the argument values for a grequest_start operation.
   */
  typedef struct dumpi_grequest_start {
    /** Argument value after PMPI call */
    dumpi_request  request;
  } dumpi_grequest_start;

  /**
   * Hold the argument values for a init_thread operation.
   */
  typedef struct dumpi_init_thread {
    /** Argument value before PMPI call */
    int  argc;
    /** 
     * Argument value before PMPI call.  
     * Array of length [argc] containing NUL-terminated std::strings.
     */
    char ** argv;
    /** Argument value before PMPI call */
    dumpi_threadlevel  required;
    /** Argument value after PMPI call */
    dumpi_threadlevel  provided;
  } dumpi_init_thread;

  /**
   * Hold the argument values for a is_thread_main operation.
   */
  typedef struct dumpi_is_thread_main {
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_is_thread_main;

  /**
   * Hold the argument values for a query_thread operation.
   */
  typedef struct dumpi_query_thread {
    /** Argument value after PMPI call */
    dumpi_threadlevel  supported;
  } dumpi_query_thread;

  /**
   * Hold the argument values for a status_set_cancelled operation.
   */
  typedef struct dumpi_status_set_cancelled {
    /** Argument value before PMPI call */
    dumpi_status * status;
    /** Argument value before PMPI call */
    int  flag;
  } dumpi_status_set_cancelled;

  /**
   * Hold the argument values for a status_set_elements operation.
   */
  typedef struct dumpi_status_set_elements {
    /** Argument value before PMPI call */
    dumpi_status * status;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    int  count;
  } dumpi_status_set_elements;

  /**
   * Hold the argument values for a type_create_keyval operation.
   */
  typedef struct dumpi_type_create_keyval {
    /** Argument value after PMPI call */
    dumpi_type_keyval  keyval;
  } dumpi_type_create_keyval;

  /**
   * Hold the argument values for a type_delete_attr operation.
   */
  typedef struct dumpi_type_delete_attr {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_type_keyval  keyval;
  } dumpi_type_delete_attr;

  /**
   * Hold the argument values for a type_dup operation.
   */
  typedef struct dumpi_type_dup {
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_dup;

  /**
   * Hold the argument values for a type_free_keyval operation.
   */
  typedef struct dumpi_type_free_keyval {
    /** Argument value before PMPI call */
    dumpi_type_keyval  keyval;
  } dumpi_type_free_keyval;

  /**
   * Hold the argument values for a type_get_attr operation.
   */
  typedef struct dumpi_type_get_attr {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_type_keyval  keyval;
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_type_get_attr;

  /**
   * Hold the argument values for a type_get_contents operation.
   */
  typedef struct dumpi_type_get_contents {
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  numdatatypes;
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  numaddresses;
    /** Not an MPI argument.  Added to index relevant data in the struct. */
    int  numintegers;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    int  maxintegers;
    /** Argument value before PMPI call */
    int  maxaddresses;
    /** Argument value before PMPI call */
    int  maxdatatypes;
    /** Argument value after PMPI call.  Array of length [numintegers] */
    int * arrintegers;
    /** Argument value after PMPI call.  Array of length [numaddresses] */
    int * arraddresses;
    /** Argument value after PMPI call.  Array of length [numdatatypes] */
    dumpi_datatype * arrdatatypes;
  } dumpi_type_get_contents;

  /**
   * Hold the argument values for a type_get_envelope operation.
   */
  typedef struct dumpi_type_get_envelope {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  numintegers;
    /** Argument value after PMPI call */
    int  numaddresses;
    /** Argument value after PMPI call */
    int  numdatatypes;
    /** Argument value after PMPI call */
    dumpi_combiner  combiner;
  } dumpi_type_get_envelope;

  /**
   * Hold the argument values for a type_get_name operation.
   */
  typedef struct dumpi_type_get_name {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call.  NUL-terminated std::string. */
    char * name;
    /** Argument value after PMPI call */
    int  resultlen;
  } dumpi_type_get_name;

  /**
   * Hold the argument values for a type_set_attr operation.
   */
  typedef struct dumpi_type_set_attr {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call */
    dumpi_type_keyval  keyval;
  } dumpi_type_set_attr;

  /**
   * Hold the argument values for a type_set_name operation.
   */
  typedef struct dumpi_type_set_name {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * name;
  } dumpi_type_set_name;

  /**
   * Hold the argument values for a type_match_size operation.
   */
  typedef struct dumpi_type_match_size {
    /** Argument value before PMPI call */
    dumpi_typeclass  typeclass;
    /** Argument value before PMPI call */
    int  size;
    /** Argument value after PMPI call */
    dumpi_datatype  datatype;
  } dumpi_type_match_size;

  /**
   * Hold the argument values for a win_call_errhandler operation.
   */
  typedef struct dumpi_win_call_errhandler {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value before PMPI call */
    int  errorcode;
  } dumpi_win_call_errhandler;

  /**
   * Hold the argument values for a win_create_keyval operation.
   */
  typedef struct dumpi_win_create_keyval {
    /** Argument value after PMPI call */
    dumpi_win_keyval  keyval;
  } dumpi_win_create_keyval;

  /**
   * Hold the argument values for a win_delete_attr operation.
   */
  typedef struct dumpi_win_delete_attr {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value before PMPI call */
    dumpi_win_keyval  keyval;
  } dumpi_win_delete_attr;

  /**
   * Hold the argument values for a win_free_keyval operation.
   */
  typedef struct dumpi_win_free_keyval {
    /** Argument value before PMPI call */
    dumpi_win_keyval  keyval;
  } dumpi_win_free_keyval;

  /**
   * Hold the argument values for a win_get_attr operation.
   */
  typedef struct dumpi_win_get_attr {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value before PMPI call */
    dumpi_win_keyval  keyval;
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_win_get_attr;

  /**
   * Hold the argument values for a win_get_name operation.
   */
  typedef struct dumpi_win_get_name {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value after PMPI call.  NUL-terminated std::string. */
    char * name;
    /** Argument value after PMPI call */
    int  resultlen;
  } dumpi_win_get_name;

  /**
   * Hold the argument values for a win_set_attr operation.
   */
  typedef struct dumpi_win_set_attr {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value before PMPI call */
    dumpi_win_keyval  keyval;
  } dumpi_win_set_attr;

  /**
   * Hold the argument values for a win_set_name operation.
   */
  typedef struct dumpi_win_set_name {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * name;
  } dumpi_win_set_name;

  /**
   * Hold the argument values for a alloc_mem operation.
   */
  typedef struct dumpi_alloc_mem {
    /** Argument value before PMPI call */
    int  size;
    /** Argument value before PMPI call */
    dumpi_info  info;
  } dumpi_alloc_mem;

  /**
   * Hold the argument values for a comm_create_errhandler operation.
   */
  typedef struct dumpi_comm_create_errhandler {
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_comm_create_errhandler;

  /**
   * Hold the argument values for a comm_get_errhandler operation.
   */
  typedef struct dumpi_comm_get_errhandler {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_comm_get_errhandler;

  /**
   * Hold the argument values for a comm_set_errhandler operation.
   */
  typedef struct dumpi_comm_set_errhandler {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_comm_set_errhandler;

  /**
   * Hold the argument values for a file_create_errhandler operation.
   */
  typedef struct dumpi_file_create_errhandler {
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_file_create_errhandler;

  /**
   * Hold the argument values for a file_get_errhandler operation.
   */
  typedef struct dumpi_file_get_errhandler {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_file_get_errhandler;

  /**
   * Hold the argument values for a file_set_errhandler operation.
   */
  typedef struct dumpi_file_set_errhandler {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_file_set_errhandler;

  /**
   * Hold the argument values for a finalized operation.
   */
  typedef struct dumpi_finalized {
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_finalized;

  /**
   * Hold the argument values for a free_mem operation.
   */
  typedef struct dumpi_free_mem {
    int dummy;
  } dumpi_free_mem;

  /**
   * Hold the argument values for a get_address operation.
   */
  typedef struct dumpi_get_address {
    /** Argument value after PMPI call */
    int  address;
  } dumpi_get_address;

  /**
   * Hold the argument values for a info_create operation.
   */
  typedef struct dumpi_info_create {
    /** Argument value after PMPI call */
    dumpi_info  info;
  } dumpi_info_create;

  /**
   * Hold the argument values for a info_delete operation.
   */
  typedef struct dumpi_info_delete {
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * key;
  } dumpi_info_delete;

  /**
   * Hold the argument values for a info_dup operation.
   */
  typedef struct dumpi_info_dup {
    /** Argument value before PMPI call */
    dumpi_info  oldinfo;
    /** Argument value after PMPI call */
    dumpi_info  newinfo;
  } dumpi_info_dup;

  /**
   * Hold the argument values for a info_free operation.
   */
  typedef struct dumpi_info_free {
    /** Argument value before PMPI call */
    dumpi_info  info;
  } dumpi_info_free;

  /**
   * Hold the argument values for a info_get operation.
   */
  typedef struct dumpi_info_get {
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * key;
    /** Argument value before PMPI call */
    int  valuelength;
    /** Argument value after PMPI call.  NUL-terminated std::string. */
    char * value;
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_info_get;

  /**
   * Hold the argument values for a info_get_nkeys operation.
   */
  typedef struct dumpi_info_get_nkeys {
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value after PMPI call */
    int  nkeys;
  } dumpi_info_get_nkeys;

  /**
   * Hold the argument values for a info_get_nthkey operation.
   */
  typedef struct dumpi_info_get_nthkey {
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call */
    int  n;
    /** Argument value after PMPI call.  NUL-terminated std::string */
    char * key;
  } dumpi_info_get_nthkey;

  /**
   * Hold the argument values for a info_get_valuelen operation.
   */
  typedef struct dumpi_info_get_valuelen {
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * key;
    /** Argument value after PMPI call */
    int  valuelen;
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_info_get_valuelen;

  /**
   * Hold the argument values for a info_set operation.
   */
  typedef struct dumpi_info_set {
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * key;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * value;
  } dumpi_info_set;

  /**
   * Hold the argument values for a pack_external operation.
   */
  typedef struct dumpi_pack_external {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * datarep;
    /** Argument value before PMPI call */
    int  incount;
    /** Argument value before PMPI call */
    dumpi_datatype  intype;
    /** Argument value before PMPI call */
    int  outcount;
    /** Argument value before and after PMPI call. */
    struct { int in, out; }  position;
  } dumpi_pack_external;

  /**
   * Hold the argument values for a pack_external_size operation.
   */
  typedef struct dumpi_pack_external_size {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * datarep;
    /** Argument value before PMPI call */
    int  incount;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  size;
  } dumpi_pack_external_size;

  /**
   * Hold the argument values for a request_get_status operation.
   */
  typedef struct dumpi_request_get_status {
    /** Argument value before PMPI call */
    dumpi_request  request;
    /** Argument value after PMPI call */
    int  flag;
    /** Argument value before PMPI call.  Only stored if(flag!=0) */
    dumpi_status * status;
  } dumpi_request_get_status;

  /**
   * Hold the argument values for a type_create_darray operation.
   */
  typedef struct dumpi_type_create_darray {
    /** Argument value before PMPI call */
    int  size;
    /** Argument value before PMPI call */
    int  rank;
    /** Argument value before PMPI call */
    int  ndims;
    /** Argument value before PMPI call.  Array of length [ndims] */
    int * gsizes;
    /** Argument value before PMPI call.  Array of length [ndims] */
    dumpi_distribution * distribs;
    /** Argument value before PMPI call.  Array of length [ndims] */
    int * dargs;
    /** Argument value before PMPI call.  Array of length [ndims] */
    int * psizes;
    /** Argument value before PMPI call */
    dumpi_ordering  order;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_create_darray;

  /**
   * Hold the argument values for a type_create_hindexed operation.
   */
  typedef struct dumpi_type_create_hindexed {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    int * blocklengths;
    /** Argument value before PMPI call.  Array of length [count] */
    int * displacements;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_create_hindexed;

  /**
   * Hold the argument values for a type_create_hvector operation.
   */
  typedef struct dumpi_type_create_hvector {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    int  blocklength;
    /** Argument value before PMPI call */
    int  stride;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_create_hvector;

  /**
   * Hold the argument values for a type_create_indexed_block operation.
   */
  typedef struct dumpi_type_create_indexed_block {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    int  blocklength;
    /** Argument value before PMPI call.  Array of length [count] */
    int * displacments;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_create_indexed_block;

  /**
   * Hold the argument values for a type_create_resized operation.
   */
  typedef struct dumpi_type_create_resized {
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value before PMPI call */
    int  lb;
    /** Argument value before PMPI call */
    int  extent;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_create_resized;

  /**
   * Hold the argument values for a type_create_struct operation.
   */
  typedef struct dumpi_type_create_struct {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    int * blocklengths;
    /** Argument value before PMPI call.  Array of length [count] */
    int * displacements;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpi_datatype * oldtypes;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_create_struct;

  /**
   * Hold the argument values for a type_create_subarray operation.
   */
  typedef struct dumpi_type_create_subarray {
    /** Argument value before PMPI call */
    int  ndims;
    /** Argument value before PMPI call.  Array of length [ndims] */
    int * sizes;
    /** Argument value before PMPI call.  Array of length [ndims] */
    int * subsizes;
    /** Argument value before PMPI call.  Array of length [ndims] */
    int * starts;
    /** Argument value before PMPI call */
    dumpi_ordering  order;
    /** Argument value before PMPI call */
    dumpi_datatype  oldtype;
    /** Argument value after PMPI call */
    dumpi_datatype  newtype;
  } dumpi_type_create_subarray;

  /**
   * Hold the argument values for a type_get_extent operation.
   */
  typedef struct dumpi_type_get_extent {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  lb;
    /** Argument value after PMPI call */
    int  extent;
  } dumpi_type_get_extent;

  /**
   * Hold the argument values for a type_get_true_extent operation.
   */
  typedef struct dumpi_type_get_true_extent {
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  lb;
    /** Argument value after PMPI call */
    int  extent;
  } dumpi_type_get_true_extent;

  /**
   * Hold the argument values for a unpack_external operation.
   */
  typedef struct dumpi_unpack_external {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * datarep;
    /** Argument value before PMPI call */
    int  insize;
    /** Argument value before and after PMPI call. */
    struct { int in, out; }  position;
    /** Argument value before PMPI call */
    int  outcount;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_unpack_external;

  /**
   * Hold the argument values for a win_create_errhandler operation.
   */
  typedef struct dumpi_win_create_errhandler {
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_win_create_errhandler;

  /**
   * Hold the argument values for a win_get_errhandler operation.
   */
  typedef struct dumpi_win_get_errhandler {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_win_get_errhandler;

  /**
   * Hold the argument values for a win_set_errhandler operation.
   */
  typedef struct dumpi_win_set_errhandler {
    /** Argument value before PMPI call */
    dumpi_win  win;
    /** Argument value after PMPI call */
    dumpi_errhandler  errhandler;
  } dumpi_win_set_errhandler;

  /**
   * Hold the argument values for a file_open operation.
   */
  typedef struct dumpi_file_open {
    /** Argument value before PMPI call */
    dumpi_comm  comm;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * filename;
    /** Argument value before PMPI call */
    dumpi_filemode  amode;
    /** Argument value before PMPI call */
    dumpi_info  info;
    /** Argument value after PMPI call */
    dumpi_file  file;
  } dumpi_file_open;

  /**
   * Hold the argument values for a file_close operation.
   */
  typedef struct dumpi_file_close {
    /** Argument value before PMPI call */
    dumpi_file  file;
  } dumpi_file_close;

  /**
   * Hold the argument values for a file_delete operation.
   */
  typedef struct dumpi_file_delete {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * filename;
    /** Argument value before PMPI call */
    dumpi_info  info;
  } dumpi_file_delete;

  /**
   * Hold the argument values for a file_set_size operation.
   */
  typedef struct dumpi_file_set_size {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  size;
  } dumpi_file_set_size;

  /**
   * Hold the argument values for a file_preallocate operation.
   */
  typedef struct dumpi_file_preallocate {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  size;
  } dumpi_file_preallocate;

  /**
   * Hold the argument values for a file_get_size operation.
   */
  typedef struct dumpi_file_get_size {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    int64_t  size;
  } dumpi_file_get_size;

  /**
   * Hold the argument values for a file_get_group operation.
   */
  typedef struct dumpi_file_get_group {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_group  group;
  } dumpi_file_get_group;

  /**
   * Hold the argument values for a file_get_amode operation.
   */
  typedef struct dumpi_file_get_amode {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_filemode  amode;
  } dumpi_file_get_amode;

  /**
   * Hold the argument values for a file_set_info operation.
   */
  typedef struct dumpi_file_set_info {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    dumpi_info  info;
  } dumpi_file_set_info;

  /**
   * Hold the argument values for a file_get_info operation.
   */
  typedef struct dumpi_file_get_info {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_info  info;
  } dumpi_file_get_info;

  /**
   * Hold the argument values for a file_set_view operation.
   */
  typedef struct dumpi_file_set_view {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    dumpi_datatype  hosttype;
    /** Argument value before PMPI call */
    dumpi_datatype  filetype;
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * datarep;
    /** Argument value before PMPI call */
    dumpi_info  info;
  } dumpi_file_set_view;

  /**
   * Hold the argument values for a file_get_view operation.
   */
  typedef struct dumpi_file_get_view {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    int64_t  offset;
    /** Argument value after PMPI call */
    dumpi_datatype  hosttype;
    /** Argument value after PMPI call */
    dumpi_datatype  filetype;
    /** Argument value after PMPI call.  NUL-terminated std::string */
    char * datarep;
  } dumpi_file_get_view;

  /**
   * Hold the argument values for a file_read_at operation.
   */
  typedef struct dumpi_file_read_at {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read_at;

  /**
   * Hold the argument values for a file_read_at_all operation.
   */
  typedef struct dumpi_file_read_at_all {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read_at_all;

  /**
   * Hold the argument values for a file_write_at operation.
   */
  typedef struct dumpi_file_write_at {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write_at;

  /**
   * Hold the argument values for a file_write_at_all operation.
   */
  typedef struct dumpi_file_write_at_all {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write_at_all;

  /**
   * Hold the argument values for a file_iread_at operation.
   */
  typedef struct dumpi_file_iread_at {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpio_request  request;
  } dumpi_file_iread_at;

  /**
   * Hold the argument values for a file_iwrite_at operation.
   */
  typedef struct dumpi_file_iwrite_at {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpio_request  request;
  } dumpi_file_iwrite_at;

  /**
   * Hold the argument values for a file_read operation.
   */
  typedef struct dumpi_file_read {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read;

  /**
   * Hold the argument values for a file_read_all operation.
   */
  typedef struct dumpi_file_read_all {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read_all;

  /**
   * Hold the argument values for a file_write operation.
   */
  typedef struct dumpi_file_write {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write;

  /**
   * Hold the argument values for a file_write_all operation.
   */
  typedef struct dumpi_file_write_all {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write_all;

  /**
   * Hold the argument values for a file_iread operation.
   */
  typedef struct dumpi_file_iread {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpio_request  request;
  } dumpi_file_iread;

  /**
   * Hold the argument values for a file_iwrite operation.
   */
  typedef struct dumpi_file_iwrite {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpio_request  request;
  } dumpi_file_iwrite;

  /**
   * Hold the argument values for a file_seek operation.
   */
  typedef struct dumpi_file_seek {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    dumpi_whence  whence;
  } dumpi_file_seek;

  /**
   * Hold the argument values for a file_get_position operation.
   */
  typedef struct dumpi_file_get_position {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    int64_t  offset;
  } dumpi_file_get_position;

  /**
   * Hold the argument values for a file_get_byte_offset operation.
   */
  typedef struct dumpi_file_get_byte_offset {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value after PMPI call */
    int64_t  bytes;
  } dumpi_file_get_byte_offset;

  /**
   * Hold the argument values for a file_read_shared operation.
   */
  typedef struct dumpi_file_read_shared {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read_shared;

  /**
   * Hold the argument values for a file_write_shared operation.
   */
  typedef struct dumpi_file_write_shared {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write_shared;

  /**
   * Hold the argument values for a file_iread_shared operation.
   */
  typedef struct dumpi_file_iread_shared {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpio_request  request;
  } dumpi_file_iread_shared;

  /**
   * Hold the argument values for a file_iwrite_shared operation.
   */
  typedef struct dumpi_file_iwrite_shared {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpio_request  request;
  } dumpi_file_iwrite_shared;

  /**
   * Hold the argument values for a file_read_ordered operation.
   */
  typedef struct dumpi_file_read_ordered {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read_ordered;

  /**
   * Hold the argument values for a file_write_ordered operation.
   */
  typedef struct dumpi_file_write_ordered {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write_ordered;

  /**
   * Hold the argument values for a file_seek_shared operation.
   */
  typedef struct dumpi_file_seek_shared {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    dumpi_whence  whence;
  } dumpi_file_seek_shared;

  /**
   * Hold the argument values for a file_get_position_shared operation.
   */
  typedef struct dumpi_file_get_position_shared {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    int64_t  offset;
  } dumpi_file_get_position_shared;

  /**
   * Hold the argument values for a file_read_at_all_begin operation.
   */
  typedef struct dumpi_file_read_at_all_begin {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_file_read_at_all_begin;

  /**
   * Hold the argument values for a file_read_at_all_end operation.
   */
  typedef struct dumpi_file_read_at_all_end {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read_at_all_end;

  /**
   * Hold the argument values for a file_write_at_all_begin operation.
   */
  typedef struct dumpi_file_write_at_all_begin {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int64_t  offset;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_file_write_at_all_begin;

  /**
   * Hold the argument values for a file_write_at_all_end operation.
   */
  typedef struct dumpi_file_write_at_all_end {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write_at_all_end;

  /**
   * Hold the argument values for a file_read_all_begin operation.
   */
  typedef struct dumpi_file_read_all_begin {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_file_read_all_begin;

  /**
   * Hold the argument values for a file_read_all_end operation.
   */
  typedef struct dumpi_file_read_all_end {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read_all_end;

  /**
   * Hold the argument values for a file_write_all_begin operation.
   */
  typedef struct dumpi_file_write_all_begin {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_file_write_all_begin;

  /**
   * Hold the argument values for a file_write_all_end operation.
   */
  typedef struct dumpi_file_write_all_end {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write_all_end;

  /**
   * Hold the argument values for a file_read_ordered_begin operation.
   */
  typedef struct dumpi_file_read_ordered_begin {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_file_read_ordered_begin;

  /**
   * Hold the argument values for a file_read_ordered_end operation.
   */
  typedef struct dumpi_file_read_ordered_end {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_read_ordered_end;

  /**
   * Hold the argument values for a file_write_ordered_begin operation.
   */
  typedef struct dumpi_file_write_ordered_begin {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
  } dumpi_file_write_ordered_begin;

  /**
   * Hold the argument values for a file_write_ordered_end operation.
   */
  typedef struct dumpi_file_write_ordered_end {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpi_file_write_ordered_end;

  /**
   * Hold the argument values for a file_get_type_extent operation.
   */
  typedef struct dumpi_file_get_type_extent {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    dumpi_datatype  datatype;
    /** Argument value after PMPI call */
    int  extent;
  } dumpi_file_get_type_extent;

  /**
   * Hold the argument values for a register_datarep operation.
   */
  typedef struct dumpi_register_datarep {
    /** Argument value before PMPI call.  NUL-terminated std::string */
    char * name;
  } dumpi_register_datarep;

  /**
   * Hold the argument values for a file_set_atomicity operation.
   */
  typedef struct dumpi_file_set_atomicity {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value before PMPI call */
    int  flag;
  } dumpi_file_set_atomicity;

  /**
   * Hold the argument values for a file_get_atomicity operation.
   */
  typedef struct dumpi_file_get_atomicity {
    /** Argument value before PMPI call */
    dumpi_file  file;
    /** Argument value after PMPI call */
    int  flag;
  } dumpi_file_get_atomicity;

  /**
   * Hold the argument values for a file_sync operation.
   */
  typedef struct dumpi_file_sync {
    /** Argument value before PMPI call */
    dumpi_file  file;
  } dumpi_file_sync;

  /**
   * Hold the argument values for an IO test operation.
   */
  typedef struct dumpio_test {
    /** Argument value before PMPI call */
    dumpio_request  request;
    /** Argument value after PMPI call */
    int  flag;
    /** Argument value before PMPI call.  Only stored if(flag!=0) */
    dumpi_status * status;
  } dumpio_test;

  /**
   * Hold the argument values for an IO wait operation.
   */
  typedef struct dumpio_wait {
    /** Argument value before PMPI call */
    dumpio_request  request;
    /** Argument value after PMPI call */
    dumpi_status * status;
  } dumpio_wait;

  /**
   * Hold the argument values for an IO testall operation.
   */
  typedef struct dumpio_testall {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpio_request * requests;
    /** Argument value after PMPI call */
    int  flag;
    /**
     * Argument value after PMPI call.  Array of length [count].
     * Only stored if(flag!=0)
     */
    dumpi_status * statuses;
  } dumpio_testall;

  /**
   * Hold the argument values for an IO waitall operation.
   */
  typedef struct dumpio_waitall {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpio_request * requests;
    /** Argument value after PMPI call.  Array of length [count] */
    dumpi_status * statuses;
  } dumpio_waitall;

  /**
   * Hold the argument values for an IO testany operation.
   */
  typedef struct dumpio_testany {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpio_request * requests;
    /** Argument value before PMPI call.  Only stored if(flag!=0) */
    int  index;
    /** Argument value after PMPI call */
    int  flag;
    /**
     * Argument value after PMPI call.  Array of length [count].
     * Only stored if(flag!=0)
     */
    dumpi_status * statuses;
  } dumpio_testany;

  /**
   * Hold the argument values for an IO waitany operation.
   */
  typedef struct dumpio_waitany {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpio_request * requests;
    /** Argument value after PMPI call */
    int  index;
    /**
     * Argument value after PMPI call.  Array of length [count].
     * Only stored if(index != MPI_UNDEFINED)
     */
    dumpi_status * statuses;
  } dumpio_waitany;

  /**
   * Hold the argument values for an IO waitsome operation.
   */
  typedef struct dumpio_waitsome {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpio_request * requests;
    /** Argument value after PMPI call */
    int  outcount;
    /** Argument value after PMPI call.  Array of length [*outcount] */
    int * indices;
    /** Argument value after PMPI call.  Array of length [*outcount] */
    dumpi_status * statuses;
  } dumpio_waitsome;

  /**
   * Hold the argument values for an IO testsome operation.
   */
  typedef struct dumpio_testsome {
    /** Argument value before PMPI call */
    int  count;
    /** Argument value before PMPI call.  Array of length [count] */
    dumpio_request * requests;
    /** Argument value after PMPI call */
    int  outcount;
    /** Argument value after PMPI call.  Array of length [*outcount] */
    int * indices;
    /** Argument value after PMPI call.  Array of length [*outcount] */
    dumpi_status * statuses;
  } dumpio_testsome;

  /*@}*/

#ifdef __cplusplus
} /* end of extern C block */
#endif /* ! __cplusplus */

#endif /* ! COMMON_ARGTYPES_H */