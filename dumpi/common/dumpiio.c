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

#include <dumpi/common/dumpiio.h>
#include <dumpi/common/iodefs.h>
#include <dumpi/common/funcs.h>

/* Added to accommodate the handling of NULL terminated arrays etc. */
static int ignore_bounds_0, ignore_bounds_1, ignore_bounds_2;

int dumpi_write_func_enter(const dumpi_func_call *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Function_enter);
  PUT_INT64_T(profile, val->fn);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_func_enter(dumpi_func_call *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Function_enter);
  val->fn = GET_INT64_T(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_func_exit(const dumpi_func_call *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Function_exit);
  PUT_INT64_T(profile, val->fn);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_func_exit(dumpi_func_call *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Function_exit);
  val->fn = GET_INT64_T(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_send(const dumpi_send *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Send);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_send(dumpi_send *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Send);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_recv(const dumpi_recv *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Recv);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_SOURCE(profile, val->source);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_recv(dumpi_recv *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Recv);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->source = GET_DUMPI_SOURCE(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_get_count(const dumpi_get_count *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Get_count);
  PUT_DUMPI_STATUS(profile, val->status);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->count);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_get_count(dumpi_get_count *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Get_count);
  val->status = GET_DUMPI_STATUS(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->count = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_bsend(const dumpi_bsend *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Bsend);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_bsend(dumpi_bsend *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Bsend);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_ssend(const dumpi_ssend *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Ssend);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_ssend(dumpi_ssend *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Ssend);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_rsend(const dumpi_rsend *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Rsend);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_rsend(dumpi_rsend *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Rsend);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_buffer_attach(const dumpi_buffer_attach *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Buffer_attach);
  PUT_INT(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_buffer_attach(dumpi_buffer_attach *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Buffer_attach);
  val->size = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_buffer_detach(const dumpi_buffer_detach *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Buffer_detach);
  PUT_INT(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_buffer_detach(dumpi_buffer_detach *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Buffer_detach);
  val->size = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_isend(const dumpi_isend *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Isend);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_INT(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_isend(dumpi_isend *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Isend);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_ibsend(const dumpi_ibsend *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Ibsend);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_ibsend(dumpi_ibsend *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Ibsend);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_issend(const dumpi_issend *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Issend);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_issend(dumpi_issend *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Issend);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_irsend(const dumpi_irsend *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Irsend);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_irsend(dumpi_irsend *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Irsend);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_irecv(const dumpi_irecv *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Irecv);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_SOURCE(profile, val->source);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_irecv(dumpi_irecv *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Irecv);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->source = GET_DUMPI_SOURCE(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_wait(const dumpi_wait *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Wait);
  PUT_DUMPI_REQUEST(profile, val->request);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_wait(dumpi_wait *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Wait);
  val->request = GET_DUMPI_REQUEST(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_test(const dumpi_test *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Test);
  PUT_DUMPI_REQUEST(profile, val->request);
  PUT_INT(profile, val->flag);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_test(dumpi_test *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Test);
  val->request = GET_DUMPI_REQUEST(profile);
  val->flag = GET_INT(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_request_free(const dumpi_request_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Request_free);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_request_free(dumpi_request_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Request_free);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_waitany(const dumpi_waitany *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Waitany);
  PUT_INT(profile, val->count);
  PUT_DUMPI_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->index);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_waitany(dumpi_waitany *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Waitany);
  val->count = GET_INT(profile);
  GET_DUMPI_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->index = GET_INT(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_testany(const dumpi_testany *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Testany);
  PUT_INT(profile, val->count);
  PUT_DUMPI_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->index);
  PUT_INT(profile, val->flag);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_testany(dumpi_testany *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Testany);
  val->count = GET_INT(profile);
  GET_DUMPI_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->index = GET_INT(profile);
  val->flag = GET_INT(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_waitall(const dumpi_waitall *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Waitall);
  PUT_INT(profile, val->count);
  PUT_DUMPI_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->count), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_waitall(dumpi_waitall *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Waitall);
  val->count = GET_INT(profile);
  GET_DUMPI_REQUEST_ARRAY_1(profile, val->count, val->requests);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->count, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_testall(const dumpi_testall *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Testall);
  PUT_INT(profile, val->count);
  PUT_DUMPI_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->flag);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->count), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_testall(dumpi_testall *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Testall);
  val->count = GET_INT(profile);
  GET_DUMPI_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->flag = GET_INT(profile);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->count, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_waitsome(const dumpi_waitsome *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Waitsome);
  PUT_INT(profile, val->count);
  PUT_DUMPI_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->outcount);
  PUT_INT_ARRAY_1(profile, (i0 < val->outcount), val->indices);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->outcount), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_waitsome(dumpi_waitsome *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Waitsome);
  val->count = GET_INT(profile);
  GET_DUMPI_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->outcount = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->outcount, val->indices);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->outcount, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_testsome(const dumpi_testsome *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Testsome);
  PUT_INT(profile, val->count);
  PUT_DUMPI_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->outcount);
  PUT_INT_ARRAY_1(profile, (i0 < val->outcount), val->indices);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->outcount), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_testsome(dumpi_testsome *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Testsome);
  val->count = GET_INT(profile);
  GET_DUMPI_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->outcount = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->outcount, val->indices);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->outcount, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_iprobe(const dumpi_iprobe *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Iprobe);
  PUT_DUMPI_SOURCE(profile, val->source);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->flag);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_iprobe(dumpi_iprobe *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Iprobe);
  val->source = GET_DUMPI_SOURCE(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->flag = GET_INT(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_probe(const dumpi_probe *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Probe);
  PUT_DUMPI_SOURCE(profile, val->source);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_probe(dumpi_probe *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Probe);
  val->source = GET_DUMPI_SOURCE(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cancel(const dumpi_cancel *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cancel);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cancel(dumpi_cancel *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cancel);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_test_cancelled(const dumpi_test_cancelled *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Test_cancelled);
  PUT_DUMPI_STATUS(profile, val->status);
  PUT_INT(profile, val->cancelled);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_test_cancelled(dumpi_test_cancelled *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Test_cancelled);
  val->status = GET_DUMPI_STATUS(profile);
  val->cancelled = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_send_init(const dumpi_send_init *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Send_init);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_send_init(dumpi_send_init *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Send_init);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_bsend_init(const dumpi_bsend_init *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Bsend_init);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_bsend_init(dumpi_bsend_init *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Bsend_init);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_ssend_init(const dumpi_ssend_init *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Ssend_init);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_ssend_init(dumpi_ssend_init *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Ssend_init);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_rsend_init(const dumpi_rsend_init *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Rsend_init);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_rsend_init(dumpi_rsend_init *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Rsend_init);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_recv_init(const dumpi_recv_init *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Recv_init);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_SOURCE(profile, val->source);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_recv_init(dumpi_recv_init *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Recv_init);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->source = GET_DUMPI_SOURCE(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_start(const dumpi_start *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Start);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_start(dumpi_start *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Start);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_startall(const dumpi_startall *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Startall);
  PUT_INT(profile, val->count);
  PUT_DUMPI_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_startall(dumpi_startall *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Startall);
  val->count = GET_INT(profile);
  GET_DUMPI_REQUEST_ARRAY_1(profile, val->count, val->requests);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_sendrecv(const dumpi_sendrecv *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Sendrecv);
  PUT_INT(profile, val->sendcount);
  PUT_DUMPI_DATATYPE(profile, val->sendtype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->sendtag);
  PUT_INT(profile, val->recvcount);
  PUT_DUMPI_DATATYPE(profile, val->recvtype);
  PUT_DUMPI_SOURCE(profile, val->source);
  PUT_DUMPI_TAG(profile, val->recvtag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_sendrecv(dumpi_sendrecv *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Sendrecv);
  val->sendcount = GET_INT(profile);
  val->sendtype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->sendtag = GET_DUMPI_TAG(profile);
  val->recvcount = GET_INT(profile);
  val->recvtype = GET_DUMPI_DATATYPE(profile);
  val->source = GET_DUMPI_SOURCE(profile);
  val->recvtag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_sendrecv_replace(const dumpi_sendrecv_replace *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Sendrecv_replace);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_DEST(profile, val->dest);
  PUT_DUMPI_TAG(profile, val->sendtag);
  PUT_DUMPI_SOURCE(profile, val->source);
  PUT_DUMPI_TAG(profile, val->recvtag);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_sendrecv_replace(dumpi_sendrecv_replace *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Sendrecv_replace);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  val->sendtag = GET_DUMPI_TAG(profile);
  val->source = GET_DUMPI_SOURCE(profile);
  val->recvtag = GET_DUMPI_TAG(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_contiguous(const dumpi_type_contiguous *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_contiguous);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_contiguous(dumpi_type_contiguous *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_contiguous);
  val->count = GET_INT(profile);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_vector(const dumpi_type_vector *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_vector);
  PUT_INT(profile, val->count);
  PUT_INT(profile, val->blocklength);
  PUT_INT(profile, val->stride);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_vector(dumpi_type_vector *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_vector);
  val->count = GET_INT(profile);
  val->blocklength = GET_INT(profile);
  val->stride = GET_INT(profile);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_hvector(const dumpi_type_hvector *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_hvector);
  PUT_INT(profile, val->count);
  PUT_INT(profile, val->blocklength);
  PUT_INT(profile, val->stride);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_hvector(dumpi_type_hvector *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_hvector);
  val->count = GET_INT(profile);
  val->blocklength = GET_INT(profile);
  val->stride = GET_INT(profile);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_indexed(const dumpi_type_indexed *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_indexed);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->lengths);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->indices);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_indexed(dumpi_type_indexed *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_indexed);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->lengths);
  GET_INT_ARRAY_1(profile, val->count, val->indices);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_hindexed(const dumpi_type_hindexed *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_hindexed);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->lengths);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->indices);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_hindexed(dumpi_type_hindexed *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_hindexed);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->lengths);
  GET_INT_ARRAY_1(profile, val->count, val->indices);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_struct(const dumpi_type_struct *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_struct);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->lengths);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->indices);
  PUT_DUMPI_DATATYPE_ARRAY_1(profile, (i0 < val->count), val->oldtypes);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_struct(dumpi_type_struct *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_struct);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->lengths);
  if(dumpi_have_version(profile->version, 0, 6, 9)) {
    GET_INT_ARRAY_1(profile, val->count, val->indices);
  }
  else {
    val->indices = (int*)calloc(val->count, sizeof(int));
    val->indices[0] = GET_INT(profile); /* Unfortunate error prior to 0.6.9 */
  }
  GET_DUMPI_DATATYPE_ARRAY_1(profile, val->count, val->oldtypes);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_address(const dumpi_address *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Address);
  PUT_INT(profile, val->address);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_address(dumpi_address *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Address);
  val->address = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_extent(const dumpi_type_extent *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_extent);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->extent);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_extent(dumpi_type_extent *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_extent);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->extent = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_size(const dumpi_type_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_size);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_size(dumpi_type_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_size);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->size = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_lb(const dumpi_type_lb *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_lb);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->lb);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_lb(dumpi_type_lb *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_lb);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->lb = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_ub(const dumpi_type_ub *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_ub);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->ub);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_ub(dumpi_type_ub *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_ub);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->ub = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_commit(const dumpi_type_commit *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_commit);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_commit(dumpi_type_commit *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_commit);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_free(const dumpi_type_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_free);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_free(dumpi_type_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_free);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_get_elements(const dumpi_get_elements *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Get_elements);
  PUT_DUMPI_STATUS(profile, val->status);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->elements);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_get_elements(dumpi_get_elements *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Get_elements);
  val->status = GET_DUMPI_STATUS(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->elements = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_pack(const dumpi_pack *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Pack);
  PUT_INT(profile, val->incount);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->outcount);
  PUT_INT(profile, val->position.in);
  PUT_INT(profile, val->position.out);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_pack(dumpi_pack *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Pack);
  val->incount = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->outcount = GET_INT(profile);
  val->position.in = GET_INT(profile);
  val->position.out = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_unpack(const dumpi_unpack *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Unpack);
  PUT_INT(profile, val->incount);
  PUT_INT(profile, val->position.in);
  PUT_INT(profile, val->position.out);
  PUT_INT(profile, val->outcount);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_unpack(dumpi_unpack *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Unpack);
  val->incount = GET_INT(profile);
  val->position.in = GET_INT(profile);
  val->position.out = GET_INT(profile);
  val->outcount = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_pack_size(const dumpi_pack_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Pack_size);
  PUT_INT(profile, val->incount);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_pack_size(dumpi_pack_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Pack_size);
  val->incount = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->size = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_barrier(const dumpi_barrier *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Barrier);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_barrier(dumpi_barrier *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Barrier);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_bcast(const dumpi_bcast *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Bcast);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_bcast(dumpi_bcast *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Bcast);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->root = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_gather(const dumpi_gather *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Gather);
  PUT_INT(profile, val->commrank);
  PUT_INT(profile, val->sendcount);
  PUT_DUMPI_DATATYPE(profile, val->sendtype);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->comm);
  if(val->commrank == val->root)   PUT_INT(profile, val->recvcount);
  if(val->commrank == val->root)   PUT_DUMPI_DATATYPE(profile, val->recvtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_gather(dumpi_gather *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Gather);
  val->commrank = GET_INT(profile);
  val->sendcount = GET_INT(profile);
  val->sendtype = GET_DUMPI_DATATYPE(profile);
  val->root = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  if(val->commrank == val->root)   val->recvcount = GET_INT(profile);
  if(val->commrank == val->root)   val->recvtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_gatherv(const dumpi_gatherv *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Gatherv);
  PUT_INT(profile, val->commrank);
  PUT_INT(profile, val->commsize);
  PUT_INT(profile, val->sendcount);
  PUT_DUMPI_DATATYPE(profile, val->sendtype);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->comm);
  if(val->commrank == val->root)   PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->recvcounts);
  if(val->commrank == val->root)   PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->displs);
  /** JJW All nodes will need to know recvtype
  if(val->commrank == val->root)   PUT_DUMPI_DATATYPE(profile, val->recvtype);
  */
  PUT_DUMPI_DATATYPE(profile, val->recvtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_gatherv(dumpi_gatherv *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Gatherv);
  val->commrank = GET_INT(profile);
  val->commsize = GET_INT(profile);
  val->sendcount = GET_INT(profile);
  val->sendtype = GET_DUMPI_DATATYPE(profile);
  val->root = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  if(val->commrank == val->root)   GET_INT_ARRAY_1(profile, val->commsize, val->recvcounts);
  if(val->commrank == val->root)   GET_INT_ARRAY_1(profile, val->commsize, val->displs);
  /** JJW All nodes will need to know recvtype
  if(val->commrank == val->root)   val->recvtype = GET_DUMPI_DATATYPE(profile);
  */
  val->recvtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_scatter(const dumpi_scatter *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Scatter);
  PUT_INT(profile, val->commrank);
  PUT_INT(profile, val->recvcount);
  PUT_DUMPI_DATATYPE(profile, val->recvtype);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->comm);
  if(val->commrank == val->root)   PUT_INT(profile, val->sendcount);
  if(val->commrank == val->root)   PUT_DUMPI_DATATYPE(profile, val->sendtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_scatter(dumpi_scatter *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Scatter);
  val->commrank = GET_INT(profile);
  val->recvcount = GET_INT(profile);
  val->recvtype = GET_DUMPI_DATATYPE(profile);
  val->root = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  if(val->commrank == val->root)   val->sendcount = GET_INT(profile);
  if(val->commrank == val->root)   val->sendtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_scatterv(const dumpi_scatterv *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Scatterv);
  PUT_INT(profile, val->commrank);
  PUT_INT(profile, val->commsize);
  PUT_DUMPI_DATATYPE(profile, val->sendtype);
  PUT_INT(profile, val->recvcount);
  PUT_DUMPI_DATATYPE(profile, val->recvtype);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->comm);
  if(val->commrank == val->root)   PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->sendcounts);
  if(val->commrank == val->root)   PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->displs);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_scatterv(dumpi_scatterv *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Scatterv);
  val->commrank = GET_INT(profile);
  val->commsize = GET_INT(profile);
  val->sendtype = GET_DUMPI_DATATYPE(profile);
  val->recvcount = GET_INT(profile);
  val->recvtype = GET_DUMPI_DATATYPE(profile);
  val->root = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  if(val->commrank == val->root)   GET_INT_ARRAY_1(profile, val->commsize, val->sendcounts);
  if(val->commrank == val->root)   GET_INT_ARRAY_1(profile, val->commsize, val->displs);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_allgather(const dumpi_allgather *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Allgather);
  PUT_INT(profile, val->sendcount);
  PUT_DUMPI_DATATYPE(profile, val->sendtype);
  PUT_INT(profile, val->recvcount);
  PUT_DUMPI_DATATYPE(profile, val->recvtype);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_allgather(dumpi_allgather *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Allgather);
  val->sendcount = GET_INT(profile);
  val->sendtype = GET_DUMPI_DATATYPE(profile);
  val->recvcount = GET_INT(profile);
  val->recvtype = GET_DUMPI_DATATYPE(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_allgatherv(const dumpi_allgatherv *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Allgatherv);
  PUT_INT(profile, val->commsize);
  PUT_INT(profile, val->sendcount);
  PUT_DUMPI_DATATYPE(profile, val->sendtype);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->recvcounts);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->displs);
  PUT_DUMPI_DATATYPE(profile, val->recvtype);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_allgatherv(dumpi_allgatherv *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Allgatherv);
  val->commsize = GET_INT(profile);
  val->sendcount = GET_INT(profile);
  val->sendtype = GET_DUMPI_DATATYPE(profile);
  GET_INT_ARRAY_1(profile, val->commsize, val->recvcounts);
  GET_INT_ARRAY_1(profile, val->commsize, val->displs);
  val->recvtype = GET_DUMPI_DATATYPE(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_alltoall(const dumpi_alltoall *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Alltoall);
  PUT_INT(profile, val->sendcount);
  PUT_DUMPI_DATATYPE(profile, val->sendtype);
  PUT_INT(profile, val->recvcount);
  PUT_DUMPI_DATATYPE(profile, val->recvtype);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_alltoall(dumpi_alltoall *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Alltoall);
  val->sendcount = GET_INT(profile);
  val->sendtype = GET_DUMPI_DATATYPE(profile);
  val->recvcount = GET_INT(profile);
  val->recvtype = GET_DUMPI_DATATYPE(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_alltoallv(const dumpi_alltoallv *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Alltoallv);
  PUT_INT(profile, val->commsize);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->sendcounts);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->senddispls);
  PUT_DUMPI_DATATYPE(profile, val->sendtype);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->recvcounts);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->recvdispls);
  PUT_DUMPI_DATATYPE(profile, val->recvtype);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_alltoallv(dumpi_alltoallv *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Alltoallv);
  val->commsize = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->commsize, val->sendcounts);
  GET_INT_ARRAY_1(profile, val->commsize, val->senddispls);
  val->sendtype = GET_DUMPI_DATATYPE(profile);
  GET_INT_ARRAY_1(profile, val->commsize, val->recvcounts);
  GET_INT_ARRAY_1(profile, val->commsize, val->recvdispls);
  val->recvtype = GET_DUMPI_DATATYPE(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_reduce(const dumpi_reduce *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Reduce);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_OP(profile, val->op);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_reduce(dumpi_reduce *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Reduce);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->op = GET_DUMPI_OP(profile);
  val->root = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_op_create(const dumpi_op_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Op_create);
  PUT_INT(profile, val->commute);
  PUT_DUMPI_OP(profile, val->op);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_op_create(dumpi_op_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Op_create);
  val->commute = GET_INT(profile);
  val->op = GET_DUMPI_OP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_op_free(const dumpi_op_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Op_free);
  PUT_DUMPI_OP(profile, val->op);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_op_free(dumpi_op_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Op_free);
  val->op = GET_DUMPI_OP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_allreduce(const dumpi_allreduce *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Allreduce);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_OP(profile, val->op);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_allreduce(dumpi_allreduce *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Allreduce);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->op = GET_DUMPI_OP(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_reduce_scatter(const dumpi_reduce_scatter *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Reduce_scatter);
  PUT_INT(profile, val->commsize);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->recvcounts);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_OP(profile, val->op);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_reduce_scatter(dumpi_reduce_scatter *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Reduce_scatter);
  val->commsize = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->commsize, val->recvcounts);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->op = GET_DUMPI_OP(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_scan(const dumpi_scan *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Scan);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_OP(profile, val->op);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_scan(dumpi_scan *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Scan);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->op = GET_DUMPI_OP(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_size(const dumpi_group_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_size);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_INT(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_size(dumpi_group_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_size);
  val->group = GET_DUMPI_GROUP(profile);
  val->size = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_rank(const dumpi_group_rank *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_rank);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_INT(profile, val->rank);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_rank(dumpi_group_rank *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_rank);
  val->group = GET_DUMPI_GROUP(profile);
  val->rank = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_translate_ranks(const dumpi_group_translate_ranks *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_translate_ranks);
  PUT_DUMPI_GROUP(profile, val->group1);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->ranks1);
  PUT_DUMPI_GROUP(profile, val->group2);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->ranks2);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_translate_ranks(dumpi_group_translate_ranks *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_translate_ranks);
  val->group1 = GET_DUMPI_GROUP(profile);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->ranks1);
  val->group2 = GET_DUMPI_GROUP(profile);
  GET_INT_ARRAY_1(profile, val->count, val->ranks2);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_compare(const dumpi_group_compare *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_compare);
  PUT_DUMPI_GROUP(profile, val->group1);
  PUT_DUMPI_GROUP(profile, val->group2);
  PUT_DUMPI_COMPARISON(profile, val->result);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_compare(dumpi_group_compare *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_compare);
  val->group1 = GET_DUMPI_GROUP(profile);
  val->group2 = GET_DUMPI_GROUP(profile);
  val->result = GET_DUMPI_COMPARISON(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_group(const dumpi_comm_group *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_group);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_GROUP(profile, val->group);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_group(dumpi_comm_group *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_group);
  val->comm = GET_DUMPI_COMM(profile);
  val->group = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_union(const dumpi_group_union *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_union);
  PUT_DUMPI_GROUP(profile, val->group1);
  PUT_DUMPI_GROUP(profile, val->group2);
  PUT_DUMPI_GROUP(profile, val->newgroup);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_union(dumpi_group_union *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_union);
  val->group1 = GET_DUMPI_GROUP(profile);
  val->group2 = GET_DUMPI_GROUP(profile);
  val->newgroup = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_intersection(const dumpi_group_intersection *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_intersection);
  PUT_DUMPI_GROUP(profile, val->group1);
  PUT_DUMPI_GROUP(profile, val->group2);
  PUT_DUMPI_GROUP(profile, val->newgroup);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_intersection(dumpi_group_intersection *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_intersection);
  val->group1 = GET_DUMPI_GROUP(profile);
  val->group2 = GET_DUMPI_GROUP(profile);
  val->newgroup = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_difference(const dumpi_group_difference *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_difference);
  PUT_DUMPI_GROUP(profile, val->group1);
  PUT_DUMPI_GROUP(profile, val->group2);
  PUT_DUMPI_GROUP(profile, val->newgroup);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_difference(dumpi_group_difference *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_difference);
  val->group1 = GET_DUMPI_GROUP(profile);
  val->group2 = GET_DUMPI_GROUP(profile);
  val->newgroup = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_incl(const dumpi_group_incl *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_incl);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->ranks);
  PUT_DUMPI_GROUP(profile, val->newgroup);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_incl(dumpi_group_incl *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_incl);
  val->group = GET_DUMPI_GROUP(profile);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->ranks);
  val->newgroup = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_excl(const dumpi_group_excl *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_excl);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->ranks);
  PUT_DUMPI_GROUP(profile, val->newgroup);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_excl(dumpi_group_excl *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_excl);
  val->group = GET_DUMPI_GROUP(profile);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->ranks);
  val->newgroup = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_range_incl(const dumpi_group_range_incl *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_range_incl);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_2(profile, (i0 < val->count), (i1 < 3), val->ranges);
  PUT_DUMPI_GROUP(profile, val->newgroup);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_range_incl(dumpi_group_range_incl *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_range_incl);
  val->group = GET_DUMPI_GROUP(profile);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_2(profile, val->count, ignore_bounds_1, val->ranges);
  val->newgroup = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_range_excl(const dumpi_group_range_excl *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_range_excl);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_2(profile, (i0 < val->count), (i1 < 3), val->ranges);
  PUT_DUMPI_GROUP(profile, val->newgroup);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_range_excl(dumpi_group_range_excl *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_range_excl);
  val->group = GET_DUMPI_GROUP(profile);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_2(profile, val->count, ignore_bounds_1, val->ranges);
  val->newgroup = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_group_free(const dumpi_group_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Group_free);
  PUT_DUMPI_GROUP(profile, val->group);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_group_free(dumpi_group_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Group_free);
  val->group = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_size(const dumpi_comm_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_size);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_size(dumpi_comm_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_size);
  val->comm = GET_DUMPI_COMM(profile);
  val->size = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_rank(const dumpi_comm_rank *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_rank);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->rank);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_rank(dumpi_comm_rank *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_rank);
  val->comm = GET_DUMPI_COMM(profile);
  val->rank = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_compare(const dumpi_comm_compare *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_compare);
  PUT_DUMPI_COMM(profile, val->comm1);
  PUT_DUMPI_COMM(profile, val->comm2);
  PUT_DUMPI_COMPARISON(profile,  val->result);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_compare(dumpi_comm_compare *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_compare);
  val->comm1 = GET_DUMPI_COMM(profile);
  val->comm2 = GET_DUMPI_COMM(profile);
  val->result = GET_DUMPI_COMPARISON(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_dup(const dumpi_comm_dup *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_dup);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_dup(dumpi_comm_dup *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_dup);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_create(const dumpi_comm_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_create);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_create(dumpi_comm_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_create);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->group = GET_DUMPI_GROUP(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_split(const dumpi_comm_split *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_split);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_INT(profile, val->color);
  PUT_INT(profile, val->key);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_split(dumpi_comm_split *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_split);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->color = GET_INT(profile);
  val->key = GET_INT(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_free(const dumpi_comm_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_free);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_free(dumpi_comm_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_free);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_test_inter(const dumpi_comm_test_inter *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_test_inter);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->inter);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_test_inter(dumpi_comm_test_inter *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_test_inter);
  val->comm = GET_DUMPI_COMM(profile);
  val->inter = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_remote_size(const dumpi_comm_remote_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_remote_size);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_remote_size(dumpi_comm_remote_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_remote_size);
  val->comm = GET_DUMPI_COMM(profile);
  val->size = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_remote_group(const dumpi_comm_remote_group *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_remote_group);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_GROUP(profile, val->group);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_remote_group(dumpi_comm_remote_group *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_remote_group);
  val->comm = GET_DUMPI_COMM(profile);
  val->group = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_intercomm_create(const dumpi_intercomm_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Intercomm_create);
  PUT_DUMPI_COMM(profile, val->localcomm);
  PUT_INT(profile, val->localleader);
  PUT_DUMPI_COMM(profile, val->remotecomm);
  PUT_INT(profile, val->remoteleader);
  PUT_DUMPI_TAG(profile, val->tag);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_intercomm_create(dumpi_intercomm_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Intercomm_create);
  val->localcomm = GET_DUMPI_COMM(profile);
  val->localleader = GET_INT(profile);
  val->remotecomm = GET_DUMPI_COMM(profile);
  val->remoteleader = GET_INT(profile);
  val->tag = GET_DUMPI_TAG(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_intercomm_merge(const dumpi_intercomm_merge *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Intercomm_merge);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->high);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_intercomm_merge(dumpi_intercomm_merge *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Intercomm_merge);
  val->comm = GET_DUMPI_COMM(profile);
  val->high = GET_INT(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_keyval_create(const dumpi_keyval_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Keyval_create);
  PUT_DUMPI_KEYVAL(profile, val->key);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_keyval_create(dumpi_keyval_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Keyval_create);
  val->key = GET_DUMPI_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_keyval_free(const dumpi_keyval_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Keyval_free);
  PUT_DUMPI_KEYVAL(profile, val->key);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_keyval_free(dumpi_keyval_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Keyval_free);
  val->key = GET_DUMPI_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_attr_put(const dumpi_attr_put *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Attr_put);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->key);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_attr_put(dumpi_attr_put *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Attr_put);
  val->comm = GET_DUMPI_COMM(profile);
  val->key = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_attr_get(const dumpi_attr_get *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Attr_get);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->key);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_attr_get(dumpi_attr_get *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Attr_get);
  val->comm = GET_DUMPI_COMM(profile);
  val->key = GET_INT(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_attr_delete(const dumpi_attr_delete *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Attr_delete);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->key);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_attr_delete(dumpi_attr_delete *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Attr_delete);
  val->comm = GET_DUMPI_COMM(profile);
  val->key = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_topo_test(const dumpi_topo_test *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Topo_test);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_TOPOLOGY(profile, val->topo);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_topo_test(dumpi_topo_test *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Topo_test);
  val->comm = GET_DUMPI_COMM(profile);
  val->topo = GET_DUMPI_TOPOLOGY(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cart_create(const dumpi_cart_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cart_create);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_INT(profile, val->ndim);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->dims);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->periods);
  PUT_INT(profile, val->reorder);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cart_create(dumpi_cart_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cart_create);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->ndim = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->ndim, val->dims);
  GET_INT_ARRAY_1(profile, val->ndim, val->periods);
  val->reorder = GET_INT(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_dims_create(const dumpi_dims_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Dims_create);
  PUT_INT(profile, val->nodes);
  PUT_INT(profile, val->ndim);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->dims.in);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->dims.out);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_dims_create(dumpi_dims_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Dims_create);
  val->nodes = GET_INT(profile);
  val->ndim = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->ndim, val->dims.in);
  GET_INT_ARRAY_1(profile, val->ndim, val->dims.out);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_graph_create(const dumpi_graph_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Graph_create);
  PUT_INT(profile, val->numedges);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_INT(profile, val->nodes);
  PUT_INT_ARRAY_1(profile, (i0 < val->nodes), val->index);
  PUT_INT_ARRAY_1(profile, (i0 < val->numedges), val->edges);
  PUT_INT(profile, val->reorder);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_graph_create(dumpi_graph_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Graph_create);
  val->numedges = GET_INT(profile);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->nodes = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->nodes, val->index);
  GET_INT_ARRAY_1(profile, val->numedges, val->edges);
  val->reorder = GET_INT(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_graphdims_get(const dumpi_graphdims_get *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Graphdims_get);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->nodes);
  PUT_INT(profile, val->edges);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_graphdims_get(dumpi_graphdims_get *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Graphdims_get);
  val->comm = GET_DUMPI_COMM(profile);
  val->nodes = GET_INT(profile);
  val->edges = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_graph_get(const dumpi_graph_get *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Graph_get);
  PUT_INT(profile, val->totedges);
  PUT_INT(profile, val->totnodes);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->maxindex);
  PUT_INT(profile, val->maxedges);
  PUT_INT_ARRAY_1(profile, (i0 < val->totnodes), val->index);
  PUT_INT_ARRAY_1(profile, (i0 < val->totedges), val->edges);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_graph_get(dumpi_graph_get *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Graph_get);
  val->totedges = GET_INT(profile);
  val->totnodes = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->maxindex = GET_INT(profile);
  val->maxedges = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->totnodes, val->index);
  GET_INT_ARRAY_1(profile, val->totedges, val->edges);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cartdim_get(const dumpi_cartdim_get *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cartdim_get);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->ndim);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cartdim_get(dumpi_cartdim_get *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cartdim_get);
  val->comm = GET_DUMPI_COMM(profile);
  val->ndim = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cart_get(const dumpi_cart_get *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cart_get);
  PUT_INT(profile, val->ndim);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->maxdims);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->dims);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->periods);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->coords);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cart_get(dumpi_cart_get *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cart_get);
  val->ndim = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->maxdims = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->ndim, val->dims);
  GET_INT_ARRAY_1(profile, val->ndim, val->periods);
  /* coords was handled wrong (buggy) prior to version 0.6.8 */
  if(dumpi_have_version(profile->version, 0, 6, 8)) {
    GET_INT_ARRAY_1(profile, val->ndim, val->coords);
  }
  else {
    val->coords = (int*)calloc(val->ndim, sizeof(int));
    val->coords[0] = GET_INT(profile);
  }
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cart_rank(const dumpi_cart_rank *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cart_rank);
  PUT_INT(profile, val->ndim);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->coords);
  PUT_INT(profile, val->rank);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cart_rank(dumpi_cart_rank *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cart_rank);
  val->ndim = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  GET_INT_ARRAY_1(profile, val->ndim, val->coords);
  val->rank = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cart_coords(const dumpi_cart_coords *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cart_coords);
  PUT_INT(profile, val->ndim);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->rank);
  PUT_INT(profile, val->maxdims);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->coords);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cart_coords(dumpi_cart_coords *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cart_coords);
  val->ndim = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->rank = GET_INT(profile);
  val->maxdims = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->ndim, val->coords);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_graph_neighbors_count(const dumpi_graph_neighbors_count *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Graph_neighbors_count);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->rank);
  PUT_INT(profile, val->nneigh);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_graph_neighbors_count(dumpi_graph_neighbors_count *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Graph_neighbors_count);
  val->comm = GET_DUMPI_COMM(profile);
  val->rank = GET_INT(profile);
  val->nneigh = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_graph_neighbors(const dumpi_graph_neighbors *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Graph_neighbors);
  PUT_INT(profile, val->nneigh);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->rank);
  PUT_INT(profile, val->maxneighbors);
  PUT_INT_ARRAY_1(profile, (i0 < val->nneigh), val->neighbors);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_graph_neighbors(dumpi_graph_neighbors *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Graph_neighbors);
  val->nneigh = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->rank = GET_INT(profile);
  val->maxneighbors = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->nneigh, val->neighbors);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cart_shift(const dumpi_cart_shift *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cart_shift);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->direction);
  PUT_INT(profile, val->displ);
  PUT_DUMPI_SOURCE(profile, val->source);
  PUT_DUMPI_DEST(profile, val->dest);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cart_shift(dumpi_cart_shift *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cart_shift);
  val->comm = GET_DUMPI_COMM(profile);
  val->direction = GET_INT(profile);
  val->displ = GET_INT(profile);
  val->source = GET_DUMPI_SOURCE(profile);
  val->dest = GET_DUMPI_DEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cart_sub(const dumpi_cart_sub *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cart_sub);
  PUT_INT(profile, val->ndim);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->remain_dims);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cart_sub(dumpi_cart_sub *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cart_sub);
  val->ndim = GET_INT(profile);
  val->oldcomm = GET_DUMPI_COMM(profile);
  GET_INT_ARRAY_1(profile, val->ndim, val->remain_dims);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_cart_map(const dumpi_cart_map *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Cart_map);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->ndim);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->dims);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndim), val->period);
  PUT_INT(profile, val->newrank);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_cart_map(dumpi_cart_map *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Cart_map);
  val->comm = GET_DUMPI_COMM(profile);
  val->ndim = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->ndim, val->dims);
  GET_INT_ARRAY_1(profile, val->ndim, val->period);
  val->newrank = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_graph_map(const dumpi_graph_map *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Graph_map);
  PUT_INT(profile, val->numedges);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->nodes);
  PUT_INT_ARRAY_1(profile, (i0 < val->nodes), val->index);
  PUT_INT_ARRAY_1(profile, (i0 < val->numedges), val->edges);
  PUT_INT(profile, val->newrank);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_graph_map(dumpi_graph_map *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Graph_map);
  val->numedges = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->nodes = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->nodes, val->index);
  GET_INT_ARRAY_1(profile, val->numedges, val->edges);
  val->newrank = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_get_processor_name(const dumpi_get_processor_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Get_processor_name);
  PUT_CHAR_ARRAY_1(profile, (i0 < val->resultlen), val->name);
  PUT_INT(profile, val->resultlen);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_get_processor_name(dumpi_get_processor_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Get_processor_name);
  GET_CHAR_ARRAY_1(profile, val->resultlen, val->name);
  val->resultlen = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_get_version(const dumpi_get_version *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Get_version);
  PUT_INT(profile, val->version);
  PUT_INT(profile, val->subversion);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_get_version(dumpi_get_version *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Get_version);
  val->version = GET_INT(profile);
  val->subversion = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_errhandler_create(const dumpi_errhandler_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Errhandler_create);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_errhandler_create(dumpi_errhandler_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Errhandler_create);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_errhandler_set(const dumpi_errhandler_set *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Errhandler_set);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_errhandler_set(dumpi_errhandler_set *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Errhandler_set);
  val->comm = GET_DUMPI_COMM(profile);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_errhandler_get(const dumpi_errhandler_get *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Errhandler_get);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_errhandler_get(dumpi_errhandler_get *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Errhandler_get);
  val->comm = GET_DUMPI_COMM(profile);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_errhandler_free(const dumpi_errhandler_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Errhandler_free);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_errhandler_free(dumpi_errhandler_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Errhandler_free);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_error_string(const dumpi_error_string *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Error_string);
  PUT_INT(profile, val->errorcode);
  PUT_CHAR_ARRAY_1(profile, (i0 < val->resultlen), val->errorstring);
  PUT_INT(profile, val->resultlen);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_error_string(dumpi_error_string *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Error_string);
  val->errorcode = GET_INT(profile);
  GET_CHAR_ARRAY_1(profile, val->resultlen, val->errorstring);
  val->resultlen = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_error_class(const dumpi_error_class *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Error_class);
  PUT_INT(profile, val->errorcode);
  PUT_INT(profile, val->errorclass);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_error_class(dumpi_error_class *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Error_class);
  val->errorcode = GET_INT(profile);
  val->errorclass = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_wtime(const dumpi_wtime *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Wtime);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_wtime(dumpi_wtime *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Wtime);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_wtick(const dumpi_wtick *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Wtick);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_wtick(dumpi_wtick *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Wtick);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_init(const dumpi_init *val, DUMPI_WRITEARGS) {
  int i;
  if(val->argc > 0) assert(val->argv != NULL);
  STARTWRITE(profile, DUMPI_Init);
  /*
  PUT_INT(profile, val->argc);
  if(val->argc > 0 && val->argv != NULL)
    PUT_CHAR_ARRAY_2(profile, (i0 < val->argc), (v1 != '\0'), val->argv);
  else
    put32(profile, 0);
  */
  put32(profile, val->argc);
  if(val->argc > 0) assert(val->argv != NULL);
  for(i = 0; i < val->argc; ++i) {
    if(val->argv[i] == NULL) {
      val->argv[i] = "";
    }
    putchararr(profile, strlen(val->argv[i]), val->argv[i]);
  }
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_init(dumpi_init *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Init);
  /*
  val->argc = GET_INT(profile);
  GET_CHAR_ARRAY_2(profile, val->argc, ignore_bounds_1, val->argv);
  */
  val->argc = get32(profile);
  /* There was a mistake in early versions of DUMPI where the
   * size of the argc array was written twice */
  if(! dumpi_have_version(profile->version, 0, 6, 2))
    get32(profile);
  
  /* OK, now we can proceed normally */
  if(val->argc > 0) {
    int i, scratch;
    val->argv = (char**)calloc(val->argc+1, sizeof(char*));
    assert(val->argv != NULL);
    for(i = 0; i < val->argc; ++i)
      getchararr(profile, &scratch, val->argv+i);
  }
  else {
    val->argc = 0;
    val->argv = NULL;
  }
  ENDREAD(profile);
  return 1;
}

int dumpi_write_finalize(const dumpi_finalize *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Finalize);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_finalize(dumpi_finalize *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Finalize);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_initialized(const dumpi_initialized *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Initialized);
  PUT_INT(profile, val->result);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_initialized(dumpi_initialized *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Initialized);
  val->result = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_abort(const dumpi_abort *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Abort);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->errorcode);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_abort(dumpi_abort *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Abort);
  val->comm = GET_DUMPI_COMM(profile);
  val->errorcode = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_close_port(const dumpi_close_port *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Close_port);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->portname);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_close_port(dumpi_close_port *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Close_port);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->portname);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_accept(const dumpi_comm_accept *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_accept);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->portname);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_accept(dumpi_comm_accept *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_accept);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->portname);
  val->info = GET_DUMPI_INFO(profile);
  val->root = GET_INT(profile);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_connect(const dumpi_comm_connect *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_connect);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->portname);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_DUMPI_COMM(profile, val->newcomm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_connect(dumpi_comm_connect *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_connect);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->portname);
  val->info = GET_DUMPI_INFO(profile);
  val->root = GET_INT(profile);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_disconnect(const dumpi_comm_disconnect *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_disconnect);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_disconnect(dumpi_comm_disconnect *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_disconnect);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_get_parent(const dumpi_comm_get_parent *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_get_parent);
  PUT_DUMPI_COMM(profile, val->parent);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_get_parent(dumpi_comm_get_parent *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_get_parent);
  val->parent = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_join(const dumpi_comm_join *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_join);
  PUT_INT(profile, val->fd);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_join(dumpi_comm_join *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_join);
  val->fd = GET_INT(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_spawn(const dumpi_comm_spawn *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_spawn);
  PUT_INT(profile, val->oldcommrank);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_DUMPI_COMM(profile, val->newcomm);
  if(val->oldcommrank == val->root)   PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->command);
  if(val->oldcommrank == val->root)   PUT_CHAR_ARRAY_2(profile, (v0 != NULL), (v1 != '\0'), val->argv);
  if(val->oldcommrank == val->root)   PUT_INT(profile, val->maxprocs);
  if(val->oldcommrank == val->root)   PUT_DUMPI_INFO(profile, val->info);
  if(val->oldcommrank == val->root)   PUT_DUMPI_ERRCODE_ARRAY_1(profile, (i0 < val->maxprocs), val->errcodes);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_spawn(dumpi_comm_spawn *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_spawn);
  val->oldcommrank = GET_INT(profile);
  val->root = GET_INT(profile);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  if(val->oldcommrank == val->root)   GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->command);
  if(val->oldcommrank == val->root)   GET_CHAR_ARRAY_2(profile, ignore_bounds_0, ignore_bounds_1, val->argv);
  if(val->oldcommrank == val->root)   val->maxprocs = GET_INT(profile);
  if(val->oldcommrank == val->root)   val->info = GET_DUMPI_INFO(profile);
  if(val->oldcommrank == val->root)   GET_DUMPI_ERRCODE_ARRAY_1(profile, val->maxprocs, val->errcodes);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_spawn_multiple(const dumpi_comm_spawn_multiple *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_spawn_multiple);
  PUT_INT(profile, val->totprocs);
  PUT_INT(profile, val->oldcommrank);
  PUT_INT(profile, val->root);
  PUT_DUMPI_COMM(profile, val->oldcomm);
  PUT_DUMPI_COMM(profile, val->newcomm);
  if(val->oldcommrank == val->root)   PUT_INT(profile, val->count);
  if(val->oldcommrank == val->root)   PUT_CHAR_ARRAY_2(profile, (i0 < val->count), (v1 != '\0'), val->commands);
  if(val->oldcommrank == val->root)   PUT_CHAR_ARRAY_3(profile, (i0 < val->count), (v1 != NULL), (v2 != '\0'), val->argvs);
  if(val->oldcommrank == val->root)   PUT_INT_ARRAY_1(profile, (i0 < val->count), val->maxprocs);
  if(val->oldcommrank == val->root)   PUT_DUMPI_INFO_ARRAY_1(profile, (i0 < val->count), val->info);
  if(val->oldcommrank == val->root)   PUT_DUMPI_ERRCODE_ARRAY_1(profile, (i0 < val->totprocs), val->errcodes);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_spawn_multiple(dumpi_comm_spawn_multiple *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_spawn_multiple);
  val->totprocs = GET_INT(profile);
  val->oldcommrank = GET_INT(profile);
  val->root = GET_INT(profile);
  val->oldcomm = GET_DUMPI_COMM(profile);
  val->newcomm = GET_DUMPI_COMM(profile);
  if(val->oldcommrank == val->root)   val->count = GET_INT(profile);
  if(val->oldcommrank == val->root)   GET_CHAR_ARRAY_2(profile, val->count, ignore_bounds_1, val->commands);
  if(val->oldcommrank == val->root)   GET_CHAR_ARRAY_3(profile, val->count, ignore_bounds_1, ignore_bounds_2, val->argvs);
  if(val->oldcommrank == val->root)   GET_INT_ARRAY_1(profile, val->count, val->maxprocs);
  if(val->oldcommrank == val->root)   GET_DUMPI_INFO_ARRAY_1(profile, val->count, val->info);
  if(val->oldcommrank == val->root)   GET_DUMPI_ERRCODE_ARRAY_1(profile, val->totprocs, val->errcodes);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_lookup_name(const dumpi_lookup_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Lookup_name);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->servicename);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->portname);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_lookup_name(dumpi_lookup_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Lookup_name);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->servicename);
  val->info = GET_DUMPI_INFO(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->portname);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_open_port(const dumpi_open_port *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Open_port);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->portname);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_open_port(dumpi_open_port *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Open_port);
  val->info = GET_DUMPI_INFO(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->portname);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_publish_name(const dumpi_publish_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Publish_name);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->servicename);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->portname);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_publish_name(dumpi_publish_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Publish_name);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->servicename);
  val->info = GET_DUMPI_INFO(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->portname);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_unpublish_name(const dumpi_unpublish_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Unpublish_name);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->servicename);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->portname);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_unpublish_name(dumpi_unpublish_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Unpublish_name);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->servicename);
  val->info = GET_DUMPI_INFO(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->portname);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_accumulate(const dumpi_accumulate *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Accumulate);
  PUT_INT(profile, val->origincount);
  PUT_DUMPI_DATATYPE(profile, val->origintype);
  PUT_INT(profile, val->targetrank);
  PUT_INT(profile, val->targetdisp);
  PUT_INT(profile, val->targetcount);
  PUT_DUMPI_DATATYPE(profile, val->targettype);
  PUT_DUMPI_OP(profile, val->op);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_accumulate(dumpi_accumulate *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Accumulate);
  val->origincount = GET_INT(profile);
  val->origintype = GET_DUMPI_DATATYPE(profile);
  val->targetrank = GET_INT(profile);
  val->targetdisp = GET_INT(profile);
  val->targetcount = GET_INT(profile);
  val->targettype = GET_DUMPI_DATATYPE(profile);
  val->op = GET_DUMPI_OP(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_get(const dumpi_get *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Get);
  PUT_INT(profile, val->origincount);
  PUT_DUMPI_DATATYPE(profile, val->origintype);
  PUT_INT(profile, val->targetrank);
  PUT_INT(profile, val->targetdisp);
  PUT_INT(profile, val->targetcount);
  PUT_DUMPI_DATATYPE(profile, val->targettype);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_get(dumpi_get *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Get);
  val->origincount = GET_INT(profile);
  val->origintype = GET_DUMPI_DATATYPE(profile);
  val->targetrank = GET_INT(profile);
  val->targetdisp = GET_INT(profile);
  val->targetcount = GET_INT(profile);
  val->targettype = GET_DUMPI_DATATYPE(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_put(const dumpi_put *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Put);
  PUT_INT(profile, val->origincount);
  PUT_DUMPI_DATATYPE(profile, val->origintype);
  PUT_INT(profile, val->targetrank);
  PUT_INT(profile, val->targetdisp);
  PUT_INT(profile, val->targetcount);
  PUT_DUMPI_DATATYPE(profile, val->targettype);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_put(dumpi_put *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Put);
  val->origincount = GET_INT(profile);
  val->origintype = GET_DUMPI_DATATYPE(profile);
  val->targetrank = GET_INT(profile);
  val->targetdisp = GET_INT(profile);
  val->targetcount = GET_INT(profile);
  val->targettype = GET_DUMPI_DATATYPE(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_complete(const dumpi_win_complete *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_complete);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_complete(dumpi_win_complete *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_complete);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_create(const dumpi_win_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_create);
  PUT_INT(profile, val->size);
  PUT_INT(profile, val->dispunit);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_create(dumpi_win_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_create);
  val->size = GET_INT(profile);
  val->dispunit = GET_INT(profile);
  val->info = GET_DUMPI_INFO(profile);
  val->comm = GET_DUMPI_COMM(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_fence(const dumpi_win_fence *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_fence);
  PUT_DUMPI_WIN_ASSERT(profile, val->assertion);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_fence(dumpi_win_fence *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_fence);
  val->assertion = GET_DUMPI_WIN_ASSERT(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_free(const dumpi_win_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_free);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_free(dumpi_win_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_free);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_get_group(const dumpi_win_get_group *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_get_group);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_DUMPI_GROUP(profile, val->group);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_get_group(dumpi_win_get_group *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_get_group);
  val->win = GET_DUMPI_WIN(profile);
  val->group = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_lock(const dumpi_win_lock *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_lock);
  PUT_DUMPI_LOCKTYPE(profile, val->locktype);
  PUT_INT(profile, val->winrank);
  PUT_DUMPI_WIN_ASSERT(profile, val->assertion);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_lock(dumpi_win_lock *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_lock);
  val->locktype = GET_DUMPI_LOCKTYPE(profile);
  val->winrank = GET_INT(profile);
  val->assertion = GET_DUMPI_WIN_ASSERT(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_post(const dumpi_win_post *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_post);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_DUMPI_WIN_ASSERT(profile, val->assertion);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_post(dumpi_win_post *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_post);
  val->group = GET_DUMPI_GROUP(profile);
  val->assertion = GET_DUMPI_WIN_ASSERT(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_start(const dumpi_win_start *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_start);
  PUT_DUMPI_GROUP(profile, val->group);
  PUT_DUMPI_WIN_ASSERT(profile, val->assertion);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_start(dumpi_win_start *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_start);
  val->group = GET_DUMPI_GROUP(profile);
  val->assertion = GET_DUMPI_WIN_ASSERT(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_test(const dumpi_win_test *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_test);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_test(dumpi_win_test *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_test);
  val->win = GET_DUMPI_WIN(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_unlock(const dumpi_win_unlock *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_unlock);
  PUT_INT(profile, val->winrank);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_unlock(dumpi_win_unlock *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_unlock);
  val->winrank = GET_INT(profile);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_wait(const dumpi_win_wait *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_wait);
  PUT_DUMPI_WIN(profile, val->win);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_wait(dumpi_win_wait *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_wait);
  val->win = GET_DUMPI_WIN(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_alltoallw(const dumpi_alltoallw *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Alltoallw);
  PUT_INT(profile, val->commsize);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->sendcounts);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->senddispls);
  PUT_DUMPI_DATATYPE_ARRAY_1(profile, (i0 < val->commsize), val->sendtypes);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->recvcounts);
  PUT_INT_ARRAY_1(profile, (i0 < val->commsize), val->recvdispls);
  PUT_DUMPI_DATATYPE_ARRAY_1(profile, (i0 < val->commsize), val->recvtypes);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_alltoallw(dumpi_alltoallw *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Alltoallw);
  val->commsize = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->commsize, val->sendcounts);
  GET_INT_ARRAY_1(profile, val->commsize, val->senddispls);
  GET_DUMPI_DATATYPE_ARRAY_1(profile, val->commsize, val->sendtypes);
  GET_INT_ARRAY_1(profile, val->commsize, val->recvcounts);
  GET_INT_ARRAY_1(profile, val->commsize, val->recvdispls);
  GET_DUMPI_DATATYPE_ARRAY_1(profile, val->commsize, val->recvtypes);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_exscan(const dumpi_exscan *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Exscan);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_OP(profile, val->op);
  PUT_DUMPI_COMM(profile, val->comm);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_exscan(dumpi_exscan *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Exscan);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->op = GET_DUMPI_OP(profile);
  val->comm = GET_DUMPI_COMM(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_add_error_class(const dumpi_add_error_class *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Add_error_class);
  PUT_INT(profile, val->errorclass);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_add_error_class(dumpi_add_error_class *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Add_error_class);
  val->errorclass = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_add_error_code(const dumpi_add_error_code *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Add_error_code);
  PUT_INT(profile, val->errorclass);
  PUT_INT(profile, val->errorcode);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_add_error_code(dumpi_add_error_code *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Add_error_code);
  val->errorclass = GET_INT(profile);
  val->errorcode = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_add_error_string(const dumpi_add_error_string *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Add_error_string);
  PUT_INT(profile, val->errorcode);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->errorstring);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_add_error_string(dumpi_add_error_string *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Add_error_string);
  val->errorcode = GET_INT(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->errorstring);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_call_errhandler(const dumpi_comm_call_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_call_errhandler);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_INT(profile, val->errorcode);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_call_errhandler(dumpi_comm_call_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_call_errhandler);
  val->comm = GET_DUMPI_COMM(profile);
  val->errorcode = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_create_keyval(const dumpi_comm_create_keyval *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_create_keyval);
  PUT_DUMPI_COMM_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_create_keyval(dumpi_comm_create_keyval *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_create_keyval);
  val->keyval = GET_DUMPI_COMM_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_delete_attr(const dumpi_comm_delete_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_delete_attr);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_COMM_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_delete_attr(dumpi_comm_delete_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_delete_attr);
  val->comm = GET_DUMPI_COMM(profile);
  val->keyval = GET_DUMPI_COMM_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_free_keyval(const dumpi_comm_free_keyval *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_free_keyval);
  PUT_DUMPI_COMM_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_free_keyval(dumpi_comm_free_keyval *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_free_keyval);
  val->keyval = GET_DUMPI_COMM_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_get_attr(const dumpi_comm_get_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_get_attr);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_COMM_KEYVAL(profile, val->keyval);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_get_attr(dumpi_comm_get_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_get_attr);
  val->comm = GET_DUMPI_COMM(profile);
  val->keyval = GET_DUMPI_COMM_KEYVAL(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_get_name(const dumpi_comm_get_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_get_name);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_CHAR_ARRAY_1(profile, (i0 < val->resultlen), val->name);
  PUT_INT(profile, val->resultlen);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_get_name(dumpi_comm_get_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_get_name);
  val->comm = GET_DUMPI_COMM(profile);
  GET_CHAR_ARRAY_1(profile, val->resultlen, val->name);
  val->resultlen = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_set_attr(const dumpi_comm_set_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_set_attr);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_COMM_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_set_attr(dumpi_comm_set_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_set_attr);
  val->comm = GET_DUMPI_COMM(profile);
  val->keyval = GET_DUMPI_COMM_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_set_name(const dumpi_comm_set_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_set_name);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->name);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_set_name(dumpi_comm_set_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_set_name);
  val->comm = GET_DUMPI_COMM(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->name);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_call_errhandler(const dumpi_file_call_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_call_errhandler);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->errorcode);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_call_errhandler(dumpi_file_call_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_call_errhandler);
  val->file = GET_DUMPI_FILE(profile);
  val->errorcode = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_grequest_complete(const dumpi_grequest_complete *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Grequest_complete);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_grequest_complete(dumpi_grequest_complete *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Grequest_complete);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_grequest_start(const dumpi_grequest_start *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Grequest_start);
  PUT_DUMPI_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_grequest_start(dumpi_grequest_start *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Grequest_start);
  val->request = GET_DUMPI_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_init_thread(const dumpi_init_thread *val, DUMPI_WRITEARGS) {
  int i;
  STARTWRITE(profile, DUMPI_Init_thread);
  /*
  PUT_INT(profile, val->argc);
  PUT_CHAR_ARRAY_2(profile, (i0 < val->argc), (v1 != '\0'), val->argv);
  */
  put32(profile, val->argc);
  if(val->argc > 0) assert(val->argv != NULL);
  for(i = 0; i < val->argc; ++i)
    putchararr(profile, strlen(val->argv[i]), val->argv[i]);
  PUT_DUMPI_THREADLEVEL(profile, val->required);
  PUT_DUMPI_THREADLEVEL(profile, val->provided);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_init_thread(dumpi_init_thread *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Init_thread);
  /*
  val->argc = GET_INT(profile);
  GET_CHAR_ARRAY_2(profile, val->argc, ignore_bounds_1, val->argv);
  */
  val->argc = get32(profile);
  if(val->argc > 0) {
    int i, scratch;
    val->argv = (char**)calloc(val->argc+1, sizeof(char*));
    assert(val->argv != NULL);
    for(i = 0; i < val->argc; ++i)
      getchararr(profile, &scratch, val->argv+i);
  }
  else {
    val->argc = 0;
    val->argv = NULL;
  }
  val->required = GET_DUMPI_THREADLEVEL(profile);
  val->provided = GET_DUMPI_THREADLEVEL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_is_thread_main(const dumpi_is_thread_main *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Is_thread_main);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_is_thread_main(dumpi_is_thread_main *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Is_thread_main);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_query_thread(const dumpi_query_thread *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Query_thread);
  PUT_DUMPI_THREADLEVEL(profile, val->supported);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_query_thread(dumpi_query_thread *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Query_thread);
  val->supported = GET_DUMPI_THREADLEVEL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_status_set_cancelled(const dumpi_status_set_cancelled *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Status_set_cancelled);
  PUT_DUMPI_STATUS(profile, val->status);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_status_set_cancelled(dumpi_status_set_cancelled *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Status_set_cancelled);
  val->status = GET_DUMPI_STATUS(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_status_set_elements(const dumpi_status_set_elements *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Status_set_elements);
  PUT_DUMPI_STATUS(profile, val->status);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->count);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_status_set_elements(dumpi_status_set_elements *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Status_set_elements);
  val->status = GET_DUMPI_STATUS(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->count = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_create_keyval(const dumpi_type_create_keyval *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_create_keyval);
  PUT_DUMPI_TYPE_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_create_keyval(dumpi_type_create_keyval *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_create_keyval);
  val->keyval = GET_DUMPI_TYPE_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_delete_attr(const dumpi_type_delete_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_delete_attr);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_TYPE_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_delete_attr(dumpi_type_delete_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_delete_attr);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->keyval = GET_DUMPI_TYPE_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_dup(const dumpi_type_dup *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_dup);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_dup(dumpi_type_dup *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_dup);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_free_keyval(const dumpi_type_free_keyval *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_free_keyval);
  PUT_DUMPI_TYPE_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_free_keyval(dumpi_type_free_keyval *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_free_keyval);
  val->keyval = GET_DUMPI_TYPE_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_get_attr(const dumpi_type_get_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_get_attr);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_TYPE_KEYVAL(profile, val->keyval);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_get_attr(dumpi_type_get_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_get_attr);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->keyval = GET_DUMPI_TYPE_KEYVAL(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_get_contents(const dumpi_type_get_contents *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_get_contents);
  PUT_INT(profile, val->numdatatypes);
  PUT_INT(profile, val->numaddresses);
  PUT_INT(profile, val->numintegers);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->maxintegers);
  PUT_INT(profile, val->maxaddresses);
  PUT_INT(profile, val->maxdatatypes);
  PUT_INT_ARRAY_1(profile, (i0 < val->numintegers), val->arrintegers);
  PUT_INT_ARRAY_1(profile, (i0 < val->numaddresses), val->arraddresses);
  PUT_DUMPI_DATATYPE_ARRAY_1(profile, (i0 < val->numdatatypes), val->arrdatatypes);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_get_contents(dumpi_type_get_contents *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_get_contents);
  val->numdatatypes = GET_INT(profile);
  val->numaddresses = GET_INT(profile);
  val->numintegers = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->maxintegers = GET_INT(profile);
  val->maxaddresses = GET_INT(profile);
  val->maxdatatypes = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->numintegers, val->arrintegers);
  GET_INT_ARRAY_1(profile, val->numaddresses, val->arraddresses);
  GET_DUMPI_DATATYPE_ARRAY_1(profile, val->numdatatypes, val->arrdatatypes);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_get_envelope(const dumpi_type_get_envelope *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_get_envelope);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->numintegers);
  PUT_INT(profile, val->numaddresses);
  PUT_INT(profile, val->numdatatypes);
  PUT_DUMPI_COMBINER(profile, val->combiner);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_get_envelope(dumpi_type_get_envelope *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_get_envelope);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->numintegers = GET_INT(profile);
  val->numaddresses = GET_INT(profile);
  val->numdatatypes = GET_INT(profile);
  val->combiner = GET_DUMPI_COMBINER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_get_name(const dumpi_type_get_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_get_name);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->name);
  PUT_INT(profile, val->resultlen);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_get_name(dumpi_type_get_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_get_name);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->name);
  val->resultlen = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_set_attr(const dumpi_type_set_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_set_attr);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_TYPE_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_set_attr(dumpi_type_set_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_set_attr);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->keyval = GET_DUMPI_TYPE_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_set_name(const dumpi_type_set_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_set_name);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->name);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_set_name(dumpi_type_set_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_set_name);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->name);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_match_size(const dumpi_type_match_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_match_size);
  PUT_DUMPI_TYPECLASS(profile, val->typeclass);
  PUT_INT(profile, val->size);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_match_size(dumpi_type_match_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_match_size);
  val->typeclass = GET_DUMPI_TYPECLASS(profile);
  val->size = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_call_errhandler(const dumpi_win_call_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_call_errhandler);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_INT(profile, val->errorcode);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_call_errhandler(dumpi_win_call_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_call_errhandler);
  val->win = GET_DUMPI_WIN(profile);
  val->errorcode = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_create_keyval(const dumpi_win_create_keyval *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_create_keyval);
  PUT_DUMPI_WIN_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_create_keyval(dumpi_win_create_keyval *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_create_keyval);
  val->keyval = GET_DUMPI_WIN_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_delete_attr(const dumpi_win_delete_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_delete_attr);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_DUMPI_WIN_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_delete_attr(dumpi_win_delete_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_delete_attr);
  val->win = GET_DUMPI_WIN(profile);
  val->keyval = GET_DUMPI_WIN_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_free_keyval(const dumpi_win_free_keyval *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_free_keyval);
  PUT_DUMPI_WIN_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_free_keyval(dumpi_win_free_keyval *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_free_keyval);
  val->keyval = GET_DUMPI_WIN_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_get_attr(const dumpi_win_get_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_get_attr);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_DUMPI_WIN_KEYVAL(profile, val->keyval);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_get_attr(dumpi_win_get_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_get_attr);
  val->win = GET_DUMPI_WIN(profile);
  val->keyval = GET_DUMPI_WIN_KEYVAL(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_get_name(const dumpi_win_get_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_get_name);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->name);
  PUT_INT(profile, val->resultlen);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_get_name(dumpi_win_get_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_get_name);
  val->win = GET_DUMPI_WIN(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->name);
  val->resultlen = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_set_attr(const dumpi_win_set_attr *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_set_attr);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_DUMPI_WIN_KEYVAL(profile, val->keyval);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_set_attr(dumpi_win_set_attr *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_set_attr);
  val->win = GET_DUMPI_WIN(profile);
  val->keyval = GET_DUMPI_WIN_KEYVAL(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_set_name(const dumpi_win_set_name *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_set_name);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->name);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_set_name(dumpi_win_set_name *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_set_name);
  val->win = GET_DUMPI_WIN(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->name);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_alloc_mem(const dumpi_alloc_mem *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Alloc_mem);
  PUT_INT(profile, val->size);
  PUT_DUMPI_INFO(profile, val->info);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_alloc_mem(dumpi_alloc_mem *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Alloc_mem);
  val->size = GET_INT(profile);
  val->info = GET_DUMPI_INFO(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_create_errhandler(const dumpi_comm_create_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_create_errhandler);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_create_errhandler(dumpi_comm_create_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_create_errhandler);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_get_errhandler(const dumpi_comm_get_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_get_errhandler);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_get_errhandler(dumpi_comm_get_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_get_errhandler);
  val->comm = GET_DUMPI_COMM(profile);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_comm_set_errhandler(const dumpi_comm_set_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Comm_set_errhandler);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_comm_set_errhandler(dumpi_comm_set_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Comm_set_errhandler);
  val->comm = GET_DUMPI_COMM(profile);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_create_errhandler(const dumpi_file_create_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_create_errhandler);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_create_errhandler(dumpi_file_create_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_create_errhandler);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_errhandler(const dumpi_file_get_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_errhandler);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_errhandler(dumpi_file_get_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_errhandler);
  val->file = GET_DUMPI_FILE(profile);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_set_errhandler(const dumpi_file_set_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_set_errhandler);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_set_errhandler(dumpi_file_set_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_set_errhandler);
  val->file = GET_DUMPI_FILE(profile);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_finalized(const dumpi_finalized *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Finalized);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_finalized(dumpi_finalized *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Finalized);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_free_mem(const dumpi_free_mem *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Free_mem);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_free_mem(dumpi_free_mem *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Free_mem);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_get_address(const dumpi_get_address *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Get_address);
  PUT_INT(profile, val->address);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_get_address(dumpi_get_address *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Get_address);
  val->address = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_create(const dumpi_info_create *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_create);
  PUT_DUMPI_INFO(profile, val->info);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_create(dumpi_info_create *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_create);
  val->info = GET_DUMPI_INFO(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_delete(const dumpi_info_delete *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_delete);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->key);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_delete(dumpi_info_delete *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_delete);
  val->info = GET_DUMPI_INFO(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->key);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_dup(const dumpi_info_dup *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_dup);
  PUT_DUMPI_INFO(profile, val->oldinfo);
  PUT_DUMPI_INFO(profile, val->newinfo);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_dup(dumpi_info_dup *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_dup);
  val->oldinfo = GET_DUMPI_INFO(profile);
  val->newinfo = GET_DUMPI_INFO(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_free(const dumpi_info_free *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_free);
  PUT_DUMPI_INFO(profile, val->info);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_free(dumpi_info_free *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_free);
  val->info = GET_DUMPI_INFO(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_get(const dumpi_info_get *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_get);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->key);
  PUT_INT(profile, val->valuelength);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->value);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_get(dumpi_info_get *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_get);
  val->info = GET_DUMPI_INFO(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->key);
  val->valuelength = GET_INT(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->value);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_get_nkeys(const dumpi_info_get_nkeys *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_get_nkeys);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_INT(profile, val->nkeys);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_get_nkeys(dumpi_info_get_nkeys *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_get_nkeys);
  val->info = GET_DUMPI_INFO(profile);
  val->nkeys = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_get_nthkey(const dumpi_info_get_nthkey *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_get_nthkey);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_INT(profile, val->n);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->key);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_get_nthkey(dumpi_info_get_nthkey *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_get_nthkey);
  val->info = GET_DUMPI_INFO(profile);
  val->n = GET_INT(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->key);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_get_valuelen(const dumpi_info_get_valuelen *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_get_valuelen);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->key);
  PUT_INT(profile, val->valuelen);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_get_valuelen(dumpi_info_get_valuelen *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_get_valuelen);
  val->info = GET_DUMPI_INFO(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->key);
  val->valuelen = GET_INT(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_info_set(const dumpi_info_set *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Info_set);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->key);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->value);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_info_set(dumpi_info_set *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Info_set);
  val->info = GET_DUMPI_INFO(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->key);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->value);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_pack_external(const dumpi_pack_external *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Pack_external);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->datarep);
  PUT_INT(profile, val->incount);
  PUT_DUMPI_DATATYPE(profile, val->intype);
  PUT_INT(profile, val->outcount);
  PUT_INT(profile, val->position.in);
  PUT_INT(profile, val->position.out);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_pack_external(dumpi_pack_external *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Pack_external);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->datarep);
  val->incount = GET_INT(profile);
  val->intype = GET_DUMPI_DATATYPE(profile);
  val->outcount = GET_INT(profile);
  val->position.in = GET_INT(profile);
  val->position.out = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_pack_external_size(const dumpi_pack_external_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Pack_external_size);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->datarep);
  PUT_INT(profile, val->incount);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_pack_external_size(dumpi_pack_external_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Pack_external_size);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->datarep);
  val->incount = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->size = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_request_get_status(const dumpi_request_get_status *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Request_get_status);
  PUT_DUMPI_REQUEST(profile, val->request);
  PUT_INT(profile, val->flag);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_request_get_status(dumpi_request_get_status *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Request_get_status);
  val->request = GET_DUMPI_REQUEST(profile);
  val->flag = GET_INT(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_create_darray(const dumpi_type_create_darray *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_create_darray);
  PUT_INT(profile, val->size);
  PUT_INT(profile, val->rank);
  PUT_INT(profile, val->ndims);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndims), val->gsizes);
  PUT_DUMPI_DISTRIBUTION_ARRAY_1(profile, (i0 < val->ndims), val->distribs);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndims), val->dargs);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndims), val->psizes);
  PUT_DUMPI_ORDERING(profile, val->order);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_create_darray(dumpi_type_create_darray *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_create_darray);
  val->size = GET_INT(profile);
  val->rank = GET_INT(profile);
  val->ndims = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->ndims, val->gsizes);
  GET_DUMPI_DISTRIBUTION_ARRAY_1(profile, val->ndims, val->distribs);
  GET_INT_ARRAY_1(profile, val->ndims, val->dargs);
  GET_INT_ARRAY_1(profile, val->ndims, val->psizes);
  val->order = GET_DUMPI_ORDERING(profile);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_create_hindexed(const dumpi_type_create_hindexed *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_create_hindexed);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->blocklengths);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->displacements);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_create_hindexed(dumpi_type_create_hindexed *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_create_hindexed);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->blocklengths);
  GET_INT_ARRAY_1(profile, val->count, val->displacements);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_create_hvector(const dumpi_type_create_hvector *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_create_hvector);
  PUT_INT(profile, val->count);
  PUT_INT(profile, val->blocklength);
  PUT_INT(profile, val->stride);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_create_hvector(dumpi_type_create_hvector *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_create_hvector);
  val->count = GET_INT(profile);
  val->blocklength = GET_INT(profile);
  val->stride = GET_INT(profile);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_create_indexed_block(const dumpi_type_create_indexed_block *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_create_indexed_block);
  PUT_INT(profile, val->count);
  PUT_INT(profile, val->blocklength);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->displacments);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_create_indexed_block(dumpi_type_create_indexed_block *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_create_indexed_block);
  val->count = GET_INT(profile);
  val->blocklength = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->displacments);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_create_resized(const dumpi_type_create_resized *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_create_resized);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_INT(profile, val->lb);
  PUT_INT(profile, val->extent);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_create_resized(dumpi_type_create_resized *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_create_resized);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->lb = GET_INT(profile);
  val->extent = GET_INT(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_create_struct(const dumpi_type_create_struct *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_create_struct);
  PUT_INT(profile, val->count);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->blocklengths);
  PUT_INT_ARRAY_1(profile, (i0 < val->count), val->displacements);
  PUT_DUMPI_DATATYPE_ARRAY_1(profile, (i0 < val->count), val->oldtypes);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_create_struct(dumpi_type_create_struct *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_create_struct);
  val->count = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->count, val->blocklengths);
  GET_INT_ARRAY_1(profile, val->count, val->displacements);
  GET_DUMPI_DATATYPE_ARRAY_1(profile, val->count, val->oldtypes);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_create_subarray(const dumpi_type_create_subarray *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_create_subarray);
  PUT_INT(profile, val->ndims);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndims), val->sizes);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndims), val->subsizes);
  PUT_INT_ARRAY_1(profile, (i0 < val->ndims), val->starts);
  PUT_DUMPI_ORDERING(profile, val->order);
  PUT_DUMPI_DATATYPE(profile, val->oldtype);
  PUT_DUMPI_DATATYPE(profile, val->newtype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_create_subarray(dumpi_type_create_subarray *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_create_subarray);
  val->ndims = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->ndims, val->sizes);
  GET_INT_ARRAY_1(profile, val->ndims, val->subsizes);
  GET_INT_ARRAY_1(profile, val->ndims, val->starts);
  val->order = GET_DUMPI_ORDERING(profile);
  val->oldtype = GET_DUMPI_DATATYPE(profile);
  val->newtype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_get_extent(const dumpi_type_get_extent *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_get_extent);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->lb);
  PUT_INT(profile, val->extent);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_get_extent(dumpi_type_get_extent *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_get_extent);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->lb = GET_INT(profile);
  val->extent = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_type_get_true_extent(const dumpi_type_get_true_extent *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Type_get_true_extent);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->lb);
  PUT_INT(profile, val->extent);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_type_get_true_extent(dumpi_type_get_true_extent *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Type_get_true_extent);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->lb = GET_INT(profile);
  val->extent = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_unpack_external(const dumpi_unpack_external *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Unpack_external);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->datarep);
  PUT_INT(profile, val->insize);
  PUT_INT(profile, val->position.in);
  PUT_INT(profile, val->position.out);
  PUT_INT(profile, val->outcount);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_unpack_external(dumpi_unpack_external *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Unpack_external);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->datarep);
  val->insize = GET_INT(profile);
  val->position.in = GET_INT(profile);
  val->position.out = GET_INT(profile);
  val->outcount = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_create_errhandler(const dumpi_win_create_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_create_errhandler);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_create_errhandler(dumpi_win_create_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_create_errhandler);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_get_errhandler(const dumpi_win_get_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_get_errhandler);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_get_errhandler(dumpi_win_get_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_get_errhandler);
  val->win = GET_DUMPI_WIN(profile);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_win_set_errhandler(const dumpi_win_set_errhandler *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Win_set_errhandler);
  PUT_DUMPI_WIN(profile, val->win);
  PUT_DUMPI_ERRHANDLER(profile, val->errhandler);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_win_set_errhandler(dumpi_win_set_errhandler *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Win_set_errhandler);
  val->win = GET_DUMPI_WIN(profile);
  val->errhandler = GET_DUMPI_ERRHANDLER(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_open(const dumpi_file_open *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_open);
  PUT_DUMPI_COMM(profile, val->comm);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->filename);
  PUT_DUMPI_FILEMODE(profile, val->amode);
  PUT_DUMPI_INFO(profile, val->info);
  PUT_DUMPI_FILE(profile, val->file);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_open(dumpi_file_open *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_open);
  val->comm = GET_DUMPI_COMM(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->filename);
  val->amode = GET_DUMPI_FILEMODE(profile);
  val->info = GET_DUMPI_INFO(profile);
  val->file = GET_DUMPI_FILE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_close(const dumpi_file_close *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_close);
  PUT_DUMPI_FILE(profile, val->file);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_close(dumpi_file_close *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_close);
  val->file = GET_DUMPI_FILE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_delete(const dumpi_file_delete *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_delete);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->filename);
  PUT_DUMPI_INFO(profile, val->info);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_delete(dumpi_file_delete *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_delete);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->filename);
  val->info = GET_DUMPI_INFO(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_set_size(const dumpi_file_set_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_set_size);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_set_size(dumpi_file_set_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_set_size);
  val->file = GET_DUMPI_FILE(profile);
  val->size = GET_INT64_T(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_preallocate(const dumpi_file_preallocate *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_preallocate);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_preallocate(dumpi_file_preallocate *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_preallocate);
  val->file = GET_DUMPI_FILE(profile);
  val->size = GET_INT64_T(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_size(const dumpi_file_get_size *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_size);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->size);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_size(dumpi_file_get_size *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_size);
  val->file = GET_DUMPI_FILE(profile);
  val->size = GET_INT64_T(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_group(const dumpi_file_get_group *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_group);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_GROUP(profile, val->group);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_group(dumpi_file_get_group *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_group);
  val->file = GET_DUMPI_FILE(profile);
  val->group = GET_DUMPI_GROUP(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_amode(const dumpi_file_get_amode *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_amode);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_FILEMODE(profile, val->amode);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_amode(dumpi_file_get_amode *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_amode);
  val->file = GET_DUMPI_FILE(profile);
  val->amode = GET_DUMPI_FILEMODE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_set_info(const dumpi_file_set_info *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_set_info);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_INFO(profile, val->info);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_set_info(dumpi_file_set_info *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_set_info);
  val->file = GET_DUMPI_FILE(profile);
  val->info = GET_DUMPI_INFO(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_info(const dumpi_file_get_info *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_info);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_INFO(profile, val->info);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_info(dumpi_file_get_info *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_info);
  val->file = GET_DUMPI_FILE(profile);
  val->info = GET_DUMPI_INFO(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_set_view(const dumpi_file_set_view *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_set_view);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_DUMPI_DATATYPE(profile, val->hosttype);
  PUT_DUMPI_DATATYPE(profile, val->filetype);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->datarep);
  PUT_DUMPI_INFO(profile, val->info);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_set_view(dumpi_file_set_view *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_set_view);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->hosttype = GET_DUMPI_DATATYPE(profile);
  val->filetype = GET_DUMPI_DATATYPE(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->datarep);
  val->info = GET_DUMPI_INFO(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_view(const dumpi_file_get_view *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_view);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_DUMPI_DATATYPE(profile, val->hosttype);
  PUT_DUMPI_DATATYPE(profile, val->filetype);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->datarep);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_view(dumpi_file_get_view *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_view);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->hosttype = GET_DUMPI_DATATYPE(profile);
  val->filetype = GET_DUMPI_DATATYPE(profile);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->datarep);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_at(const dumpi_file_read_at *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_at);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_at(dumpi_file_read_at *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_at);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_at_all(const dumpi_file_read_at_all *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_at_all);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_at_all(dumpi_file_read_at_all *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_at_all);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_at(const dumpi_file_write_at *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_at);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_at(dumpi_file_write_at *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_at);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_at_all(const dumpi_file_write_at_all *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_at_all);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_at_all(dumpi_file_write_at_all *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_at_all);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_iread_at(const dumpi_file_iread_at *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_iread_at);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPIO_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_iread_at(dumpi_file_iread_at *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_iread_at);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->request = GET_DUMPIO_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_iwrite_at(const dumpi_file_iwrite_at *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_iwrite_at);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPIO_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_iwrite_at(dumpi_file_iwrite_at *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_iwrite_at);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->request = GET_DUMPIO_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read(const dumpi_file_read *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read(dumpi_file_read *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_all(const dumpi_file_read_all *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_all);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_all(dumpi_file_read_all *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_all);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write(const dumpi_file_write *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write(dumpi_file_write *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_all(const dumpi_file_write_all *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_all);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_all(dumpi_file_write_all *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_all);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_iread(const dumpi_file_iread *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_iread);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPIO_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_iread(dumpi_file_iread *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_iread);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->request = GET_DUMPIO_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_iwrite(const dumpi_file_iwrite *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_iwrite);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPIO_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_iwrite(dumpi_file_iwrite *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_iwrite);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->request = GET_DUMPIO_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_seek(const dumpi_file_seek *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_seek);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_DUMPI_WHENCE(profile, val->whence);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_seek(dumpi_file_seek *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_seek);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->whence = GET_DUMPI_WHENCE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_position(const dumpi_file_get_position *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_position);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_position(dumpi_file_get_position *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_position);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_byte_offset(const dumpi_file_get_byte_offset *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_byte_offset);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT64_T(profile, val->bytes);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_byte_offset(dumpi_file_get_byte_offset *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_byte_offset);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->bytes = GET_INT64_T(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_shared(const dumpi_file_read_shared *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_shared);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_shared(dumpi_file_read_shared *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_shared);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_shared(const dumpi_file_write_shared *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_shared);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_shared(dumpi_file_write_shared *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_shared);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_iread_shared(const dumpi_file_iread_shared *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_iread_shared);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPIO_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_iread_shared(dumpi_file_iread_shared *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_iread_shared);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->request = GET_DUMPIO_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_iwrite_shared(const dumpi_file_iwrite_shared *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_iwrite_shared);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPIO_REQUEST(profile, val->request);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_iwrite_shared(dumpi_file_iwrite_shared *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_iwrite_shared);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->request = GET_DUMPIO_REQUEST(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_ordered(const dumpi_file_read_ordered *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_ordered);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_ordered(dumpi_file_read_ordered *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_ordered);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_ordered(const dumpi_file_write_ordered *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_ordered);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_ordered(dumpi_file_write_ordered *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_ordered);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_seek_shared(const dumpi_file_seek_shared *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_seek_shared);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_DUMPI_WHENCE(profile, val->whence);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_seek_shared(dumpi_file_seek_shared *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_seek_shared);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->whence = GET_DUMPI_WHENCE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_position_shared(const dumpi_file_get_position_shared *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_position_shared);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_position_shared(dumpi_file_get_position_shared *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_position_shared);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_at_all_begin(const dumpi_file_read_at_all_begin *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_at_all_begin);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_at_all_begin(dumpi_file_read_at_all_begin *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_at_all_begin);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_at_all_end(const dumpi_file_read_at_all_end *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_at_all_end);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_at_all_end(dumpi_file_read_at_all_end *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_at_all_end);
  val->file = GET_DUMPI_FILE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_at_all_begin(const dumpi_file_write_at_all_begin *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_at_all_begin);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT64_T(profile, val->offset);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_at_all_begin(dumpi_file_write_at_all_begin *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_at_all_begin);
  val->file = GET_DUMPI_FILE(profile);
  val->offset = GET_INT64_T(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_at_all_end(const dumpi_file_write_at_all_end *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_at_all_end);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_at_all_end(dumpi_file_write_at_all_end *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_at_all_end);
  val->file = GET_DUMPI_FILE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_all_begin(const dumpi_file_read_all_begin *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_all_begin);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_all_begin(dumpi_file_read_all_begin *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_all_begin);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_all_end(const dumpi_file_read_all_end *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_all_end);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_all_end(dumpi_file_read_all_end *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_all_end);
  val->file = GET_DUMPI_FILE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_all_begin(const dumpi_file_write_all_begin *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_all_begin);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_all_begin(dumpi_file_write_all_begin *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_all_begin);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_all_end(const dumpi_file_write_all_end *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_all_end);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_all_end(dumpi_file_write_all_end *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_all_end);
  val->file = GET_DUMPI_FILE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_ordered_begin(const dumpi_file_read_ordered_begin *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_ordered_begin);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_ordered_begin(dumpi_file_read_ordered_begin *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_ordered_begin);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_read_ordered_end(const dumpi_file_read_ordered_end *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_read_ordered_end);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_read_ordered_end(dumpi_file_read_ordered_end *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_read_ordered_end);
  val->file = GET_DUMPI_FILE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_ordered_begin(const dumpi_file_write_ordered_begin *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_ordered_begin);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->count);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_ordered_begin(dumpi_file_write_ordered_begin *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_ordered_begin);
  val->file = GET_DUMPI_FILE(profile);
  val->count = GET_INT(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_write_ordered_end(const dumpi_file_write_ordered_end *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_write_ordered_end);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_write_ordered_end(dumpi_file_write_ordered_end *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_write_ordered_end);
  val->file = GET_DUMPI_FILE(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_type_extent(const dumpi_file_get_type_extent *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_type_extent);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_DUMPI_DATATYPE(profile, val->datatype);
  PUT_INT(profile, val->extent);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_type_extent(dumpi_file_get_type_extent *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_type_extent);
  val->file = GET_DUMPI_FILE(profile);
  val->datatype = GET_DUMPI_DATATYPE(profile);
  val->extent = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_register_datarep(const dumpi_register_datarep *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_Register_datarep);
  PUT_CHAR_ARRAY_1(profile, (v0 != '\0'), val->name);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_register_datarep(dumpi_register_datarep *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_Register_datarep);
  GET_CHAR_ARRAY_1(profile, ignore_bounds_0, val->name);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_set_atomicity(const dumpi_file_set_atomicity *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_set_atomicity);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_set_atomicity(dumpi_file_set_atomicity *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_set_atomicity);
  val->file = GET_DUMPI_FILE(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_get_atomicity(const dumpi_file_get_atomicity *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_get_atomicity);
  PUT_DUMPI_FILE(profile, val->file);
  PUT_INT(profile, val->flag);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_get_atomicity(dumpi_file_get_atomicity *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_get_atomicity);
  val->file = GET_DUMPI_FILE(profile);
  val->flag = GET_INT(profile);
  ENDREAD(profile);
  return 1;
}

int dumpi_write_file_sync(const dumpi_file_sync *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPI_File_sync);
  PUT_DUMPI_FILE(profile, val->file);
  ENDWRITE(profile);
  return 1;
}

int dumpi_read_file_sync(dumpi_file_sync *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPI_File_sync);
  val->file = GET_DUMPI_FILE(profile);
  ENDREAD(profile);
  return 1;
}

int dumpio_write_test(const dumpio_test *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPIO_Test);
  PUT_DUMPIO_REQUEST(profile, val->request);
  PUT_INT(profile, val->flag);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpio_read_test(dumpio_test *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPIO_Test);
  val->request = GET_DUMPIO_REQUEST(profile);
  val->flag = GET_INT(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpio_write_wait(const dumpio_wait *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPIO_Wait);
  PUT_DUMPIO_REQUEST(profile, val->request);
  PUT_DUMPI_STATUS(profile, val->status);
  ENDWRITE(profile);
  return 1;
}

int dumpio_read_wait(dumpio_wait *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPIO_Wait);
  val->request = GET_DUMPIO_REQUEST(profile);
  val->status = GET_DUMPI_STATUS(profile);
  ENDREAD(profile);
  return 1;
}

int dumpio_write_testall(const dumpio_testall *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPIO_Testall);
  PUT_INT(profile, val->count);
  PUT_DUMPIO_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->flag);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->count), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpio_read_testall(dumpio_testall *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPIO_Testall);
  val->count = GET_INT(profile);
  GET_DUMPIO_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->flag = GET_INT(profile);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->count, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpio_write_waitall(const dumpio_waitall *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPIO_Waitall);
  PUT_INT(profile, val->count);
  PUT_DUMPIO_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->count), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpio_read_waitall(dumpio_waitall *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPIO_Waitall);
  val->count = GET_INT(profile);
  GET_DUMPIO_REQUEST_ARRAY_1(profile, val->count, val->requests);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->count, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpio_write_testany(const dumpio_testany *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPIO_Testany);
  PUT_INT(profile, val->count);
  PUT_DUMPIO_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->flag);
  PUT_INT(profile, val->index);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->count), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpio_read_testany(dumpio_testany *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPIO_Testany);
  val->count = GET_INT(profile);
  GET_DUMPIO_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->flag = GET_INT(profile);
  val->index = GET_INT(profile);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->count, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpio_write_waitany(const dumpio_waitany *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPIO_Waitany);
  PUT_INT(profile, val->count);
  PUT_DUMPIO_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->index);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->count), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpio_read_waitany(dumpio_waitany *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPIO_Waitany);
  val->count = GET_INT(profile);
  GET_DUMPIO_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->index = GET_INT(profile);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->count, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpio_write_waitsome(const dumpio_waitsome *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPIO_Waitsome);
  PUT_INT(profile, val->count);
  PUT_DUMPIO_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->outcount);
  PUT_INT_ARRAY_1(profile, (i0 < val->outcount), val->indices);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->outcount), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpio_read_waitsome(dumpio_waitsome *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPIO_Waitsome);
  val->count = GET_INT(profile);
  GET_DUMPIO_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->outcount = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->outcount, val->indices);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->outcount, val->statuses);
  ENDREAD(profile);
  return 1;
}

int dumpio_write_testsome(const dumpio_testsome *val, DUMPI_WRITEARGS) {
  STARTWRITE(profile, DUMPIO_Testsome);
  PUT_INT(profile, val->count);
  PUT_DUMPIO_REQUEST_ARRAY_1(profile, (i0 < val->count), val->requests);
  PUT_INT(profile, val->outcount);
  PUT_INT_ARRAY_1(profile, (i0 < val->outcount), val->indices);
  PUT_DUMPI_STATUS_ARRAY_1(profile, (i0 < val->outcount), val->statuses);
  ENDWRITE(profile);
  return 1;
}

int dumpio_read_testsome(dumpio_testsome *val, DUMPI_READARGS) {
  STARTREAD(profile, DUMPIO_Testsome);
  val->count = GET_INT(profile);
  GET_DUMPIO_REQUEST_ARRAY_1(profile, val->count, val->requests);
  val->outcount = GET_INT(profile);
  GET_INT_ARRAY_1(profile, val->outcount, val->indices);
  GET_DUMPI_STATUS_ARRAY_1(profile, val->outcount, val->statuses);
  ENDREAD(profile);
  return 1;
}