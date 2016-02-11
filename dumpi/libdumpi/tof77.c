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

#include <dumpi/libdumpi/tof77.h>
#include <mpi.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/* OpenMPI does some really screwy stuff with MPI_STATUSES_IGNORE
 * (defining it as a double instead of integer(mpi_status_size, 1),
 * so there is no portable way to do status tests in C */

static int f77_constants_initialized = 0;
static MPI_Fint f77_status_size, f77_compare_ident, f77_compare_similar, f77_compare_unequal;
static MPI_Fint *f77_status_ignore = NULL, *f77_statuses_ignore = NULL;
static MPI_Status *static_status = NULL;

#define GET_FCONSTANTS F77_FUNC_(libdumpi_get_f77_constants, LIBDUMPI_GET_F77_CONSTANTS)
#define GET_FSTATUSES F77_FUNC_(libdumpi_get_f77_statuses, LIBDUMPI_GET_F77_STATUSES)

void F77_FUNC_(libdumpi_f77_put_status_info, LIBDUMPI_F77_PUT_STATUS_INFO)
     (MPI_Fint *status_ignore, MPI_Fint *statuses_ignore)
{
  f77_status_ignore = status_ignore;
  f77_statuses_ignore = statuses_ignore;
}

static inline void init_constants() {
  if(! f77_constants_initialized) {
    f77_constants_initialized = 1;
    GET_FCONSTANTS(&f77_status_size, &f77_compare_ident, &f77_compare_similar, &f77_compare_unequal);
    /* By now, the Fortran bindings better have set status_ignore and statuses_ignore */
    assert((f77_status_ignore != NULL) && (f77_statuses_ignore != NULL));
  }
}

static inline MPI_Request* clone_requests(MPI_Fint *count, MPI_Fint *frequests) {
  int i;
  MPI_Request *crequests;
  assert((crequests = (MPI_Request*)calloc(*count, sizeof(MPI_Request))) != NULL);
  for(i = 0; i < *count; ++i)
    crequests[i] = MPI_Request_f2c(frequests[i]);
  return crequests;
}

static inline void update_requests(MPI_Fint *count, MPI_Request *crequests, MPI_Fint *frequests) {
  int i;
  for(i = 0; i < *count; ++i)
    frequests[i] = MPI_Request_c2f(crequests[i]);
  free(crequests);
}

static inline MPI_Status* start_inbound_status(MPI_Fint *status) {
  init_constants();
  if(! static_status)
    static_status = (MPI_Status*)calloc(1, sizeof(MPI_Status));
  assert((status != f77_status_ignore) && (status != f77_statuses_ignore));
  MPI_Status_f2c(status, static_status);
  return static_status;
}

static inline MPI_Status* start_outbound_status(MPI_Fint *status) {
  init_constants();
  if(! static_status)
    static_status = (MPI_Status*)calloc(1, sizeof(MPI_Status));
  if((status == f77_status_ignore) || (status == f77_statuses_ignore))
    return MPI_STATUS_IGNORE;
  else
    return static_status;
}

static inline void finish_status(MPI_Status *cstatus, MPI_Fint *fstatus) {
  init_constants();
  if(cstatus != MPI_STATUS_IGNORE)
    MPI_Status_c2f(cstatus, fstatus);
}

static inline MPI_Status* start_outbound_statuses(MPI_Fint *count, MPI_Fint *statuses) {
  init_constants();
  if((*count <= 0) || (statuses == f77_status_ignore) || (statuses == f77_statuses_ignore))
    return MPI_STATUSES_IGNORE;
  else {
    MPI_Status *retval = NULL;
    assert((retval = (MPI_Status*)calloc(*count, sizeof(MPI_Status))) != NULL);
    return retval;
  }
}

static inline MPI_Status* start_inbound_statuses(MPI_Fint *count, MPI_Fint *statuses) {
  init_constants();
  MPI_Status *cstatus = start_outbound_statuses(count, statuses);
  if(cstatus != MPI_STATUS_IGNORE) {
    int i;
    for(i = 0; i < *count; ++i)
      MPI_Status_f2c(statuses+i, cstatus+i);
  }
  return cstatus;
}

static inline void finish_statuses(MPI_Fint *count, MPI_Status *cstatuses, MPI_Fint *fstatuses) {
  init_constants();
  if(cstatuses != MPI_STATUSES_IGNORE) {
    int i;
    for(i = 0; i < *count; ++i)
      MPI_Status_c2f(cstatuses+i, fstatuses+i);
    free(cstatuses);
  }
}

static inline MPI_Fint c_compare_to_fint(int ccomp) {
  init_constants();
  MPI_Fint fcomp;
  switch(ccomp) {
  case MPI_IDENT:   fcomp = f77_compare_ident;   break;
  case MPI_SIMILAR: fcomp = f77_compare_similar; break;
  default: fcomp = f77_compare_unequal;
  }
  return fcomp;
}

/*****************************************************************/

void F77_FUNC_(mpi_pcontrol2, MPI_PCONTROL2)(char *fstring, MPI_Fint *ierr, int length) {
  int i;
  char *cstring = NULL;
  assert((cstring = (char*)calloc(length + 1, sizeof(char))) != NULL);
  memcpy(cstring, fstring, length);
  *ierr = MPI_Pcontrol(2, cstring);
  free(cstring);
}

void F77_FUNC_(mpi_pcontrol3, MPI_PCONTROL3)(MPI_Fint *tag, char *fstring, MPI_Fint *ierr, int length) {
  char *cstring = NULL;
  assert((cstring = (char*)malloc(length + 1)) != NULL);
  memcpy(cstring, fstring, length);
  *ierr = MPI_Pcontrol(3, (uint64_t)(*tag), cstring);
  free(cstring);
}

void F77_FUNC_(mpi_pcontrol, MPI_PCONTROL)(MPI_Fint *key, MPI_Fint *ierr) {
  *ierr = MPI_Pcontrol(*key);
}

void F77_FUNC_(mpi_send, MPI_SEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Send(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_recv, MPI_RECV)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *stat = start_outbound_status(status);
  *ierr = MPI_Recv(buf, *count, MPI_Type_f2c(*datatype), *source, *tag, MPI_Comm_f2c(*comm), stat);
  finish_status(stat, status);
}

void F77_FUNC_(mpi_get_count, MPI_GET_COUNT)(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
  MPI_Status stat;
  MPI_Status_f2c(status, &stat);
  *ierr = MPI_Get_count(&stat, MPI_Type_f2c(*datatype), count);
  MPI_Status_c2f(&stat, status);
}

void F77_FUNC_(mpi_bsend, MPI_BSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Bsend(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_ssend, MPI_SSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Ssend(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_rsend, MPI_RSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Rsend(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_buffer_attach, MPI_BUFFER_ATTACH)(void *buf, MPI_Fint *size, MPI_Fint *ierr)
{
  *ierr = MPI_Buffer_attach(buf, *size);
}

void F77_FUNC_(mpi_buffer_detach, MPI_BUFFER_DETACH)(void *buf, MPI_Fint *size, MPI_Fint *ierr)
{
  int csize;
  *ierr = MPI_Buffer_detach(buf, &csize);
  *size = csize;
}

void F77_FUNC_(mpi_isend, MPI_ISEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request req;
  *ierr = MPI_Isend(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm), &req);
  *request = MPI_Request_c2f(req);
}

void F77_FUNC_(mpi_ibsend, MPI_IBSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request req;
  *ierr = MPI_Ibsend(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm), &req);
  *request = MPI_Request_c2f(req);
}

void F77_FUNC_(mpi_issend, MPI_ISSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request req;
  *ierr = MPI_Issend(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm), &req);
  *request = MPI_Request_c2f(req);
}

void F77_FUNC_(mpi_irsend, MPI_IRSEND)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request req;
  *ierr = MPI_Irsend(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm), &req);
  *request = MPI_Request_c2f(req);
}

void F77_FUNC_(mpi_irecv, MPI_IRECV)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request req;
  *ierr = MPI_Irecv(buf, *count, MPI_Type_f2c(*datatype), *source, *tag, MPI_Comm_f2c(*comm), &req);
  *request = MPI_Request_c2f(req);
}

void F77_FUNC_(mpi_wait, MPI_WAIT)(MPI_Fint *request, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *stat;
  MPI_Request req = MPI_Request_f2c(*request);
  stat = start_inbound_status(status);
  *ierr = MPI_Wait(&req, stat);
  *request = MPI_Request_c2f(req);
  finish_status(stat, status);
}

void F77_FUNC_(mpi_test, MPI_TEST)(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
  int cflag;
  MPI_Request req;
  MPI_Status *stat = start_outbound_status(status);
  req = MPI_Request_f2c(*request);
  *ierr = MPI_Test(&req, &cflag, stat);
  *flag = cflag;
  *request = MPI_Request_c2f(req);
  finish_status(stat, status);
}

void F77_FUNC_(mpi_request_free, MPI_REQUEST_FREE)(MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request req = MPI_Request_f2c(*request);
  *ierr = MPI_Request_free(&req);
}

void F77_FUNC_(mpi_waitany, MPI_WAITANY)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *index, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Request *crequests;
  int id;
  MPI_Status *stat;
  crequests = clone_requests(count, requests);
  stat = start_outbound_status(status);
  *ierr = MPI_Waitany(*count, crequests, &id, stat);
  update_requests(count, crequests, requests);
  *index = id;
  finish_status(stat, status);
}

void F77_FUNC_(mpi_testany, MPI_TESTANY)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *index, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Request *crequests;
  int id, cflag;
  MPI_Status *stat;
  crequests = clone_requests(count, requests);
  stat = start_outbound_status(status);
  *ierr = MPI_Testany(*count, crequests, &id, &cflag, stat);
  update_requests(count, crequests, requests);
  *index = id;
  *flag = cflag;
  finish_status(stat, status);
}

void F77_FUNC_(mpi_waitall, MPI_WAITALL)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *statuses, MPI_Fint *ierr)
{
  MPI_Request *crequests;
  MPI_Status *cstatuses;
  crequests = clone_requests(count, requests);
  cstatuses = start_outbound_statuses(count, statuses);
  *ierr = MPI_Waitall(*count, crequests, cstatuses);
  update_requests(count, crequests, requests);
  finish_statuses(count, cstatuses, statuses);
}

void F77_FUNC_(mpi_testall, MPI_TESTALL)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *flag, MPI_Fint *statuses, MPI_Fint *ierr)
{
  MPI_Request *crequests;
  MPI_Status *cstatuses;
  int cflag;
  crequests = clone_requests(count, requests);
  cstatuses = start_outbound_statuses(count, statuses);
  *ierr = MPI_Testall(*count, crequests, &cflag, cstatuses);
  update_requests(count, crequests, requests);
  *flag = cflag;
  finish_statuses(count, cstatuses, statuses);
}

void F77_FUNC_(mpi_waitsome, MPI_WAITSOME)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *outcount, MPI_Fint *indices, MPI_Fint *statuses, MPI_Fint *ierr)
{
  MPI_Request *crequests;
  MPI_Status *cstatuses;
  int *cindices;
  int coutcount;
  crequests = clone_requests(count, requests);
  assert((cindices = (int*)calloc(*count, sizeof(int))) != NULL);
  cstatuses = start_outbound_statuses(count, statuses);
  *ierr = MPI_Waitsome(*count, crequests, &coutcount, cindices, cstatuses);
  update_requests(count, crequests, requests);
  *outcount = coutcount;
  if(coutcount != MPI_UNDEFINED) {
    int i;
    for(i = 0; i < coutcount; ++i) indices[i] = cindices[i];
  }
  finish_statuses(count, cstatuses, statuses);
}

void F77_FUNC_(mpi_testsome, MPI_TESTSOME)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *outcount, MPI_Fint *indices, MPI_Fint *statuses, MPI_Fint *ierr)
{
  MPI_Request *crequests;
  MPI_Status *cstatuses;
  int *cindices;
  int coutcount;
  crequests = clone_requests(count, requests);
  assert((cindices = (int*)calloc(*count, sizeof(int))) != NULL);
  cstatuses = start_outbound_statuses(count, statuses);
  *ierr = MPI_Testsome(*count, crequests, &coutcount, cindices, cstatuses);
  update_requests(count, crequests, requests);
  *outcount = coutcount;
  if(coutcount != MPI_UNDEFINED) {
    int i;
    for(i = 0; i < coutcount; ++i) indices[i] = cindices[i];
  }
  finish_statuses(count, cstatuses, statuses);
}

void F77_FUNC_(mpi_iprobe, MPI_IPROBE)(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
  int cflag;
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_Iprobe(*source, *tag, MPI_Comm_f2c(*comm), &cflag, cstatus);
  *flag = cflag;
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_probe, MPI_PROBE)(MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_Probe(*source, *tag, MPI_Comm_f2c(*comm), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_cancel, MPI_CANCEL)(MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request creq = MPI_Request_f2c(*request);
  *ierr = MPI_Cancel(&creq);
  *request = MPI_Request_c2f(creq);
}

void F77_FUNC_(mpi_test_cancelled, MPI_TEST_CANCELLED)(MPI_Fint *status, MPI_Fint *cancelled, MPI_Fint *ierr)
{
  int ccancelled;
  MPI_Status *cstatus = start_inbound_status(status);
  *ierr = MPI_Test_cancelled(cstatus, &ccancelled);
  *cancelled = ccancelled;
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_send_init, MPI_SEND_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_Send_init(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_bsend_init, MPI_BSEND_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_Bsend_init(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_ssend_init, MPI_SSEND_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_Ssend_init(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_rsend_init, MPI_RSEND_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_Rsend_init(buf, *count, MPI_Type_f2c(*datatype), *dest, *tag, MPI_Comm_f2c(*comm), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_recv_init, MPI_RECV_INIT)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *source, MPI_Fint *tag, MPI_Fint *comm, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_Recv_init(buf, *count, MPI_Type_f2c(*datatype), *source, *tag, MPI_Comm_f2c(*comm), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_start, MPI_START)(MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest = MPI_Request_f2c(*request);
  *ierr = MPI_Start(&crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_startall, MPI_STARTALL)(MPI_Fint *count, MPI_Fint *requests, MPI_Fint *ierr)
{
  MPI_Request *crequests = clone_requests(count, requests);
  *ierr = MPI_Startall(*count, crequests);
  update_requests(count, crequests, requests);
}

void F77_FUNC_(mpi_sendrecv, MPI_SENDRECV)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, MPI_Fint *dest, MPI_Fint *sendtag, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_Sendrecv(sendbuf, *sendcount, MPI_Type_f2c(*sendtype), *dest, *sendtag, recvbuf, *recvcount, MPI_Type_f2c(*recvtype), *source, *recvtag, MPI_Comm_f2c(*comm), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_sendrecv_replace, MPI_SENDRECV_REPLACE)(void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *dest, MPI_Fint *sendtag, MPI_Fint *source, MPI_Fint *recvtag, MPI_Fint *comm, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);;
  *ierr = MPI_Sendrecv_replace(buf, *count, MPI_Type_f2c(*datatype), *dest, *sendtag, *source, *recvtag, MPI_Comm_f2c(*comm), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_type_contiguous, MPI_TYPE_CONTIGUOUS)(MPI_Fint *count, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  MPI_Datatype cnewtype;
  *ierr = MPI_Type_contiguous(*count, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
}

void F77_FUNC_(mpi_type_vector, MPI_TYPE_VECTOR)(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  MPI_Datatype cnewtype;
  *ierr = MPI_Type_vector(*count, *blocklength, *stride, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
}

void F77_FUNC_(mpi_type_hvector, MPI_TYPE_HVECTOR)(MPI_Fint *count, MPI_Fint *blocklength, MPI_Aint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  MPI_Datatype cnewtype;
  *ierr = MPI_Type_hvector(*count, *blocklength, *stride, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
}

void F77_FUNC_(mpi_type_indexed, MPI_TYPE_INDEXED)(MPI_Fint *count, MPI_Fint *lengths, MPI_Fint *indices, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  int i;
  int *clengths, *cindices;
  MPI_Datatype cnewtype;
  assert((clengths = (int*)calloc(*count, sizeof(int))) != NULL);
  assert((cindices = (int*)calloc(*count, sizeof(int))) != NULL);
  for(i = 0; i < *count; ++i) {
    clengths[i] = lengths[i];
    cindices[i] = indices[i];
  }
  *ierr = MPI_Type_indexed(*count, clengths, cindices, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
  free(clengths);
  free(cindices);
}

void F77_FUNC_(mpi_type_hindexed, MPI_TYPE_HINDEXED)(MPI_Fint *count, MPI_Fint *lengths, MPI_Aint *indices, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  int i;
  int *clengths;
  MPI_Datatype cnewtype;
  assert((clengths = (int*)calloc(*count, sizeof(int))) != NULL);
  for(i = 0; i < *count; ++i) {
    clengths[i] = lengths[i];
  }
  *ierr = MPI_Type_hindexed(*count, clengths, indices, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
  free(clengths);
}

void F77_FUNC_(mpi_type_struct, MPI_TYPE_STRUCT)(MPI_Fint *count, MPI_Fint *lengths, MPI_Aint *indices, MPI_Fint *oldtypes, MPI_Fint *newtype, MPI_Fint *ierr)
{
  int i;
  int *clengths;
  MPI_Datatype *coldtypes;
  MPI_Datatype cnewtype;
  assert((clengths = (int*)calloc(*count, sizeof(int))) != NULL);
  assert((coldtypes = (MPI_Datatype*)calloc(*count, sizeof(MPI_Datatype))) != NULL);
  for(i = 0; i < *count; ++i) {
    clengths[i] = lengths[i];
    coldtypes[i] = MPI_Type_f2c(oldtypes[i]);
  }
  *ierr = MPI_Type_struct(*count, lengths, indices, coldtypes, &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
  free(coldtypes);
  free(clengths);
}

void F77_FUNC_(mpi_address, MPI_ADDRESS)(void *buf, MPI_Aint *address, MPI_Fint *ierr)
{
  *ierr = MPI_Address(buf, address);
}

void F77_FUNC_(mpi_type_extent, MPI_TYPE_EXTENT)(MPI_Fint *datatype, MPI_Aint *extent, MPI_Fint *ierr)
{
  *ierr = MPI_Type_extent(MPI_Type_f2c(*datatype), extent);
}

void F77_FUNC_(mpi_type_size, MPI_TYPE_SIZE)(MPI_Fint *datatype, MPI_Fint *size, MPI_Fint *ierr)
{
  int csize;
  *ierr = MPI_Type_size(MPI_Type_f2c(*datatype), &csize);
  *size = csize;
}

void F77_FUNC_(mpi_type_lb, MPI_TYPE_LB)(MPI_Fint *datatype, MPI_Aint *lb, MPI_Fint *ierr)
{
  *ierr = MPI_Type_lb(MPI_Type_f2c(*datatype), lb);
}

void F77_FUNC_(mpi_type_ub, MPI_TYPE_UB)(MPI_Fint *datatype, MPI_Aint *ub, MPI_Fint *ierr)
{
  *ierr = MPI_Type_ub(MPI_Type_f2c(*datatype), ub);
}

void F77_FUNC_(mpi_type_commit, MPI_TYPE_COMMIT)(MPI_Fint *datatype, MPI_Fint *ierr)
{
  MPI_Datatype ctype = MPI_Type_f2c(*datatype);
  *ierr = MPI_Type_commit(&ctype);
  *datatype = MPI_Type_c2f(ctype);
}

void F77_FUNC_(mpi_type_free, MPI_TYPE_FREE)(MPI_Fint *datatype, MPI_Fint *ierr)
{
  MPI_Datatype ctype = MPI_Type_f2c(*datatype);
  *ierr = MPI_Type_free(&ctype);
}

void F77_FUNC_(mpi_get_elements, MPI_GET_ELEMENTS)(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *elements, MPI_Fint *ierr)
{
  int celements;
  MPI_Status *cstatus = start_inbound_status(status);
  *ierr = MPI_Get_elements(cstatus, MPI_Type_f2c(*datatype), &celements);
  *elements = celements;
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_pack, MPI_PACK)(void *inbuf, MPI_Fint *incount, MPI_Fint *datatype, void *outbuf, MPI_Fint *outcount, MPI_Fint *position, MPI_Fint *comm, MPI_Fint *ierr)
{
  int cposition;
  *ierr = MPI_Pack(inbuf, *incount, MPI_Type_f2c(*datatype), outbuf, *outcount, &cposition, MPI_Comm_f2c(*comm));
  *position = cposition;
}

void F77_FUNC_(mpi_unpack, MPI_UNPACK)(void *inbuf, MPI_Fint *incount, MPI_Fint *position, void *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *ierr)
{
  int cposition;
  *ierr = MPI_Unpack(inbuf, *incount, &cposition, outbuf, *outcount, MPI_Type_f2c(*datatype), MPI_Comm_f2c(*comm));
  *position = cposition;
}

void F77_FUNC_(mpi_pack_size, MPI_PACK_SIZE)(MPI_Fint *incount, MPI_Fint *datatype, MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
  int csize;
  *ierr = MPI_Pack_size(*incount, MPI_Type_f2c(*datatype), MPI_Comm_f2c(*comm), &csize);
  *size = csize;
}

void F77_FUNC_(mpi_barrier, MPI_BARRIER)(MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Barrier(MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_bcast, MPI_BCAST)(void *sendbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Bcast(sendbuf, *count, MPI_Type_f2c(*datatype), *root, MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_gather, MPI_GATHER)(void *inbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Gather(inbuf, *sendcount, MPI_Type_f2c(*sendtype), recvbuf, *recvcount, MPI_Type_f2c(*recvtype), *root, MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_gatherv, MPI_GATHERV)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
  int i, commsize, commrank;
  int *crecvcounts, *cdispls;
  PMPI_Comm_size(MPI_Comm_f2c(*comm), &commsize);
  PMPI_Comm_rank(MPI_Comm_f2c(*comm), &commrank);
  assert((crecvcounts = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((cdispls = (int*)calloc(commsize, sizeof(int))) != NULL);
  if(commrank == *root) {
    for(i = 0; i < commsize; ++i) {
      crecvcounts[i] = recvcounts[i];
      cdispls[i] = displs[i];
    }
  }
  *ierr = MPI_Gatherv(sendbuf, *sendcount, MPI_Type_f2c(*sendtype), recvbuf, crecvcounts, cdispls, MPI_Type_f2c(*recvtype), *root, MPI_Comm_f2c(*comm));
  free(crecvcounts);
  free(cdispls);
}

void F77_FUNC_(mpi_scatter, MPI_SCATTER)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Scatter(sendbuf, *sendcount, MPI_Type_f2c(*sendtype), recvbuf, *recvcount, MPI_Type_f2c(*recvtype), *root, MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_scatterv, MPI_SCATTERV)(void *sendbuf, MPI_Fint *sendcounts, MPI_Fint *displs, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
  int i, commsize, commrank;
  int *csendcounts, *cdispls;
  PMPI_Comm_size(MPI_Comm_f2c(*comm), &commsize);
  PMPI_Comm_rank(MPI_Comm_f2c(*comm), &commrank);
  assert((csendcounts = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((cdispls = (int*)calloc(commsize, sizeof(int))) != NULL);
  if(commrank == *root) {
    for(i = 0; i < commsize; ++i) {
      csendcounts[i] = sendcounts[i];
      cdispls[i] = displs[i];
    }
  }
  *ierr = MPI_Scatterv(sendbuf, csendcounts, cdispls, MPI_Type_f2c(*sendtype), recvbuf, *recvcount, MPI_Type_f2c(*recvtype), *root, MPI_Comm_f2c(*comm));
  free(csendcounts);
  free(cdispls);
}

void F77_FUNC_(mpi_allgather, MPI_ALLGATHER)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Allgather(sendbuf, *sendcount, MPI_Type_f2c(*sendtype), recvbuf, *recvcount, MPI_Type_f2c(*recvtype), MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_allgatherv, MPI_ALLGATHERV)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *displs, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
  int i, commsize;
  int *crecvcounts, *cdispls;
  PMPI_Comm_size(MPI_Comm_f2c(*comm), &commsize);
  assert((crecvcounts = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((cdispls = (int*)calloc(commsize, sizeof(int))) != NULL);
  for(i = 0; i < commsize; ++i) {
    crecvcounts[i] = recvcounts[i];
    cdispls[i] = displs[i];
  }
  *ierr = MPI_Allgatherv(sendbuf, *sendcount, MPI_Type_f2c(*sendtype), recvbuf, crecvcounts, cdispls, MPI_Type_f2c(*recvtype), MPI_Comm_f2c(*comm));
  free(crecvcounts);
  free(cdispls);
}

void F77_FUNC_(mpi_alltoall, MPI_ALLTOALL)(void *sendbuf, MPI_Fint *sendcount, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcount, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Alltoall(sendbuf, *sendcount, MPI_Type_f2c(*sendtype), recvbuf, *recvcount, MPI_Type_f2c(*recvtype), MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_alltoallv, MPI_ALLTOALLV)(void *sendbuf, MPI_Fint *sendcounts, MPI_Fint *senddispls, MPI_Fint *sendtype, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *recvdispls, MPI_Fint *recvtype, MPI_Fint *comm, MPI_Fint *ierr)
{
  int i, commsize;
  int *csendcounts, *csenddispls, *crecvcounts, *crecvdispls;
  PMPI_Comm_size(MPI_Comm_f2c(*comm), &commsize);
  assert((csendcounts = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((csenddispls = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((crecvcounts = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((crecvdispls = (int*)calloc(commsize, sizeof(int))) != NULL);
  for(i = 0; i < commsize; ++i) {
    csendcounts[i] = sendcounts[i];
    csenddispls[i] = senddispls[i];
    crecvcounts[i] = recvcounts[i];
    crecvdispls[i] = recvdispls[i];
  }
  *ierr = MPI_Alltoallv(sendbuf, csendcounts, csenddispls, MPI_Type_f2c(*sendtype), recvbuf, crecvcounts, crecvdispls, MPI_Type_f2c(*recvtype), MPI_Comm_f2c(*comm));
  free(csendcounts);
  free(csenddispls);
  free(crecvcounts);
  free(crecvdispls);
}

void F77_FUNC_(mpi_reduce, MPI_REDUCE)(void *sendbuf, void *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *root, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Reduce(sendbuf, recvbuf, *count, MPI_Type_f2c(*datatype), MPI_Op_f2c(*op), *root, MPI_Comm_f2c(*comm));
}

/*
 * SKIP:  It's not clear that I can safely perform this mapping without
 * providing extra facilities for function mapping (too messy, too much).
void F77_FUNC_(mpi_op_create, MPI_OP_CREATE)(dumpi_f77_mpi_op *fptr, MPI_Fint *commute, MPI_Fint *op, MPI_Fint *ierr)
{
  MPI_Op cop;
  *ierr = MPI_Op_create((MPI_User_function*)fptr, *commute, &cop);
  *op = MPI_Op_c2f(cop);
}
*/

void F77_FUNC_(mpi_op_free, MPI_OP_FREE)(MPI_Fint *op, MPI_Fint *ierr)
{
  MPI_Op cop = MPI_Op_f2c(*op);
  *ierr = MPI_Op_free(&cop);
}

void F77_FUNC_(mpi_allreduce, MPI_ALLREDUCE)(void *sendbuf, void *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Allreduce(sendbuf, recvbuf, *count, MPI_Type_f2c(*datatype), MPI_Op_f2c(*op), MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_reduce_scatter, MPI_REDUCE_SCATTER)(void *sendbuf, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
  int i, commsize;
  int *crecvcounts;
  assert((crecvcounts = (int*)calloc(commsize, sizeof(int))) != NULL);
  for(i = 0; i < commsize; ++i)
    crecvcounts[i] = recvcounts[i];
  PMPI_Comm_size(MPI_Comm_f2c(*comm), &commsize);
  *ierr = MPI_Reduce_scatter(sendbuf, recvbuf, crecvcounts, MPI_Type_f2c(*datatype), MPI_Op_f2c(*op), MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_scan, MPI_SCAN)(void *sendbuf, void *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Scan(sendbuf, recvbuf, *count, MPI_Type_f2c(*datatype), MPI_Op_f2c(*op), MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_group_size, MPI_GROUP_SIZE)(MPI_Fint *group, MPI_Fint *size, MPI_Fint *ierr)
{
  int csize;
  *ierr = MPI_Group_size(MPI_Group_f2c(*group), &csize);
  *size = csize;
}

void F77_FUNC_(mpi_group_rank, MPI_GROUP_RANK)(MPI_Fint *group, MPI_Fint *rank, MPI_Fint *ierr)
{
  int crank;
  *ierr = MPI_Group_rank(MPI_Group_f2c(*group), &crank);
  *rank = crank;
}

void F77_FUNC_(mpi_group_translate_ranks, MPI_GROUP_TRANSLATE_RANKS)(MPI_Fint *group1, MPI_Fint *count, MPI_Fint *ranks1, MPI_Fint *group2, MPI_Fint *ranks2, MPI_Fint *ierr)
{
  int i;
  int *cranks1, *cranks2;
  assert((cranks1 = (int*)calloc(*count, sizeof(int))) != NULL);
  assert((cranks2 = (int*)calloc(*count, sizeof(int))) != NULL);
  for(i = 0; i < *count; ++i) {
    cranks1[i] = ranks1[i];
    cranks2[i] = ranks2[i];
  }
  *ierr = MPI_Group_translate_ranks(MPI_Group_f2c(*group1), *count, cranks1, MPI_Group_f2c(*group2), cranks2);
  free(cranks1);
  free(cranks2);
}

void F77_FUNC_(mpi_group_compare, MPI_GROUP_COMPARE)(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *result, MPI_Fint *ierr)
{
  int cresult;
  *ierr = MPI_Group_compare(MPI_Group_f2c(*group1), MPI_Group_f2c(*group2), &cresult);
  *result = c_compare_to_fint(cresult);
}

void F77_FUNC_(mpi_comm_group, MPI_COMM_GROUP)(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
  MPI_Group cgroup;
  *ierr = MPI_Comm_group(MPI_Comm_f2c(*comm), &cgroup);
  *group = MPI_Group_c2f(cgroup);
}

void F77_FUNC_(mpi_group_union, MPI_GROUP_UNION)(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
  MPI_Group cnewgroup;
  *ierr = MPI_Group_union(MPI_Group_f2c(*group1), MPI_Group_f2c(*group2), &cnewgroup);
  *newgroup = MPI_Group_c2f(cnewgroup);
}

void F77_FUNC_(mpi_group_intersection, MPI_GROUP_INTERSECTION)(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
  MPI_Group cnewgroup;
  *ierr = MPI_Group_intersection(MPI_Group_f2c(*group1), MPI_Group_f2c(*group2), &cnewgroup);
  *newgroup = MPI_Group_c2f(cnewgroup);
}

void F77_FUNC_(mpi_group_difference, MPI_GROUP_DIFFERENCE)(MPI_Fint *group1, MPI_Fint *group2, MPI_Fint *newgroup, MPI_Fint *ierr)
{
  MPI_Group cnewgroup;
  *ierr = MPI_Group_difference(MPI_Group_f2c(*group1), MPI_Group_f2c(*group2), &cnewgroup);
  *newgroup = MPI_Group_c2f(cnewgroup);
}

void F77_FUNC_(mpi_group_incl, MPI_GROUP_INCL)(MPI_Fint *group, MPI_Fint *count, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
  int i;
  int *cranks;
  MPI_Group cnewgroup;
  assert((cranks = (int*)calloc(*count, sizeof(int))) != NULL);
  for(i = 0; i < *count; ++i) cranks[i] = ranks[i];
  *ierr = MPI_Group_incl(MPI_Group_f2c(*group), *count, cranks, &cnewgroup);
  *newgroup = MPI_Group_c2f(cnewgroup);
  free(cranks);
}
   
void F77_FUNC_(mpi_group_excl, MPI_GROUP_EXCL)(MPI_Fint *group, MPI_Fint *count, MPI_Fint *ranks, MPI_Fint *newgroup, MPI_Fint *ierr)
{
  int i;
  int *cranks;
  MPI_Group cnewgroup;
  assert((cranks = (int*)calloc(*count, sizeof(int))) != NULL);
  for(i = 0; i < *count; ++i) cranks[i] = ranks[i];
  *ierr = MPI_Group_excl(MPI_Group_f2c(*group), *count, cranks, &cnewgroup);
  *newgroup = MPI_Group_c2f(cnewgroup);
  free(cranks);
}


/* Note that the fortran flavorrs of group_range_{incl,excl} have range definitions RANKS(3,*), while C uses ranks[][3] 
 * -- making a direct mapping possible between the array types. */
typedef int grange[3];
void F77_FUNC_(mpi_group_range_incl, MPI_GROUP_RANGE_INCL)(MPI_Fint *group, MPI_Fint *count, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
  int i, j;
  grange *cranges;
  MPI_Group cnewgroup;
  assert((cranges = (grange*)calloc(*count, sizeof(grange))) != NULL);
  for(i = 0; i < *count; ++i)
    for(j = 0; j < 3; ++j)
      cranges[i][j] = ranges[i][j];
  *ierr = MPI_Group_range_incl(MPI_Group_f2c(*group), *count, cranges, &cnewgroup);
  *newgroup = MPI_Group_c2f(cnewgroup);
  free(cranges);
}

void F77_FUNC_(mpi_group_range_excl, MPI_GROUP_RANGE_EXCL)(MPI_Fint *group, MPI_Fint *count, MPI_Fint ranges[][3], MPI_Fint *newgroup, MPI_Fint *ierr)
{
  int i, j;
  grange *cranges;
  MPI_Group cnewgroup;
  assert((cranges = (grange*)calloc(*count, sizeof(grange))) != NULL);
  for(i = 0; i < *count; ++i)
    for(j = 0; j < 3; ++j)
      cranges[i][j] = ranges[i][j];
  *ierr = MPI_Group_range_excl(MPI_Group_f2c(*group), *count, cranges, &cnewgroup);
  *newgroup = MPI_Group_c2f(cnewgroup);
  free(cranges);
}

void F77_FUNC_(mpi_group_free, MPI_GROUP_FREE)(MPI_Fint *group, MPI_Fint *ierr)
{
  MPI_Group cgroup = MPI_Group_f2c(*group);
  *ierr = MPI_Group_free(&cgroup);
}

void F77_FUNC_(mpi_comm_size, MPI_COMM_SIZE)(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
  int csize;
  *ierr = MPI_Comm_size(MPI_Comm_f2c(*comm), &csize);
  *size = csize;
}

void F77_FUNC_(mpi_comm_rank, MPI_COMM_RANK)(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *ierr)
{
  int crank;
  *ierr = MPI_Comm_rank(MPI_Comm_f2c(*comm), &crank);
  *rank = crank;
}

void F77_FUNC_(mpi_comm_compare, MPI_COMM_COMPARE)(MPI_Fint *comm1, MPI_Fint *comm2, MPI_Fint *result, MPI_Fint *ierr)
{
  int cresult;
  *ierr = MPI_Comm_compare(MPI_Comm_f2c(*comm1), MPI_Comm_f2c(*comm2), &cresult);
  *result = cresult;
}

void F77_FUNC_(mpi_comm_dup, MPI_COMM_DUP)(MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  MPI_Comm cnewcomm;
  *ierr = MPI_Comm_dup(MPI_Comm_f2c(*oldcomm), &cnewcomm);
  *newcomm = MPI_Comm_c2f(cnewcomm);
}

void F77_FUNC_(mpi_comm_create, MPI_COMM_CREATE)(MPI_Fint *oldcomm, MPI_Fint *group, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  MPI_Comm cnewcomm;
  *ierr = MPI_Comm_create(MPI_Comm_f2c(*oldcomm), MPI_Group_f2c(*group), &cnewcomm);
  *newcomm = MPI_Comm_c2f(cnewcomm);
}

void F77_FUNC_(mpi_comm_split, MPI_COMM_SPLIT)(MPI_Fint *oldcomm, MPI_Fint *color, MPI_Fint *key, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  MPI_Comm cnewcomm;
  *ierr = MPI_Comm_split(MPI_Comm_f2c(*oldcomm), *color, *key, &cnewcomm);
  *newcomm = MPI_Comm_c2f(cnewcomm);
}

void F77_FUNC_(mpi_comm_free, MPI_COMM_FREE)(MPI_Fint *comm, MPI_Fint *ierr)
{
  MPI_Comm ccomm = MPI_Comm_f2c(*comm);
  *ierr = MPI_Comm_free(&ccomm);
  *comm = MPI_Comm_c2f(ccomm);
}

void F77_FUNC_(mpi_comm_test_inter, MPI_COMM_TEST_INTER)(MPI_Fint *comm, MPI_Fint *inter, MPI_Fint *ierr)
{
  int cinter;
  *ierr = MPI_Comm_test_inter(MPI_Comm_f2c(*comm), &cinter);
  *inter = cinter;
}

void F77_FUNC_(mpi_comm_remote_size, MPI_COMM_REMOTE_SIZE)(MPI_Fint *comm, MPI_Fint *size, MPI_Fint *ierr)
{
  int csize;
  *ierr = MPI_Comm_remote_size(MPI_Comm_f2c(*comm), &csize);
  *size = csize;
}

void F77_FUNC_(mpi_comm_remote_group, MPI_COMM_REMOTE_GROUP)(MPI_Fint *comm, MPI_Fint *group, MPI_Fint *ierr)
{
  MPI_Group cgroup;
  *ierr = MPI_Comm_remote_group(MPI_Comm_f2c(*comm), &cgroup);
  *group = MPI_Group_c2f(cgroup);
}

void F77_FUNC_(mpi_intercomm_create, MPI_INTERCOMM_CREATE)(MPI_Fint *localcomm, MPI_Fint *localleader, MPI_Fint *remotecomm, MPI_Fint *remoteleader, MPI_Fint *tag, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  MPI_Comm cnewcomm;
  *ierr = MPI_Intercomm_create(MPI_Comm_f2c(*localcomm), *localleader, MPI_Comm_f2c(*remotecomm), *remoteleader, *tag, &cnewcomm);
  *newcomm = MPI_Comm_c2f(cnewcomm);
}

void F77_FUNC_(mpi_intercomm_merge, MPI_INTERCOMM_MERGE)(MPI_Fint *comm, MPI_Fint *high, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  MPI_Comm cnewcomm;
  *ierr = MPI_Intercomm_merge(MPI_Comm_f2c(*comm), *high, &cnewcomm);
  *newcomm = MPI_Comm_c2f(cnewcomm);
}

/* SKIP: It is not clear that this can be supported without extra
 * wrapper functions
void F77_FUNC_(mpi_keyval_create, MPI_KEYVAL_CREATE)(dumpi_f77_copy_fn *copyfunc, dumpi_f77_del_fn *delfunc, MPI_Fint *key, void *state, MPI_Fint *ierr)
{
  *ierr = MPI_Keyval_create(*copyfunc, *delfunc, *key, state);
}
*/

void F77_FUNC_(mpi_keyval_free, MPI_KEYVAL_FREE)(MPI_Fint *key, MPI_Fint *ierr)
{
  int ckey = *key;
  *ierr = MPI_Keyval_free(&ckey);
  *key = ckey;
}

void F77_FUNC_(mpi_attr_put, MPI_ATTR_PUT)(MPI_Fint *comm, MPI_Fint *key, void *attr, MPI_Fint *ierr)
{
  *ierr = MPI_Attr_put(MPI_Comm_f2c(*comm), *key, attr);
}

void F77_FUNC_(mpi_attr_get, MPI_ATTR_GET)(MPI_Fint *comm, MPI_Fint *key, void *attr, MPI_Fint *flag, MPI_Fint *ierr)
{
  int cflag;
  *ierr = MPI_Attr_get(MPI_Comm_f2c(*comm), *key, attr, &cflag);
  *flag = cflag;
}

void F77_FUNC_(mpi_attr_delete, MPI_ATTR_DELETE)(MPI_Fint *comm, MPI_Fint *key, MPI_Fint *ierr)
{
  *ierr = MPI_Attr_delete(MPI_Comm_f2c(*comm), *key);
}

void F77_FUNC_(mpi_topo_test, MPI_TOPO_TEST)(MPI_Fint *comm, MPI_Fint *topo, MPI_Fint *ierr)
{
  int ctopo;
  *ierr = MPI_Topo_test(MPI_Comm_f2c(*comm), &ctopo);
  *topo = ctopo;
}

void F77_FUNC_(mpi_cart_create, MPI_CART_CREATE)(MPI_Fint *oldcomm, MPI_Fint *ndim, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *reorder, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  int i;
  int *cdims, *cperiods;
  MPI_Comm cnewcomm;
  assert((cdims = (int*)calloc(*ndim, sizeof(int))) != NULL);
  assert((cperiods = (int*)calloc(*ndim, sizeof(int))) != NULL);
  for(i = 0; i < *ndim; ++i) {
    cdims[i] = dims[i];
    cperiods[i] = periods[i];
  }
  *ierr = MPI_Cart_create(MPI_Comm_f2c(*oldcomm), *ndim, cdims, cperiods, *reorder, &cnewcomm);
  *newcomm = MPI_Comm_c2f(cnewcomm);
  free(cdims);
  free(cperiods);
}

void F77_FUNC_(mpi_dims_create, MPI_DIMS_CREATE)(MPI_Fint *nodes, MPI_Fint *ndim, MPI_Fint *dims, MPI_Fint *ierr)
{
  int i;
  int *cdims;
  assert((cdims = (int*)calloc(*ndim, sizeof(int))) != NULL);
  for(i = 0; i < *ndim; ++i) cdims[i] = dims[i];
  *ierr = MPI_Dims_create(*nodes, *ndim, cdims);
  for(i = 0; i < *ndim; ++i) dims[i] = cdims[i];
  free(cdims);
}

void F77_FUNC_(mpi_graph_create, MPI_GRAPH_CREATE)(MPI_Fint *oldcomm, MPI_Fint *nodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *reorder, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  int i, numedges;
  int *cindex, *cedges;
  MPI_Comm cnewcomm;
  numedges = edges[*nodes];
  assert((cindex = (int*)calloc(*nodes, sizeof(int))) != NULL);
  assert((cedges = (int*)calloc(numedges, sizeof(int))) != NULL);
  for(i = 0; i < *nodes; ++i) cindex[i] = index[i];
  for(i = 0; i < numedges; ++i) cedges[i] = edges[i];
  *ierr = MPI_Graph_create(MPI_Comm_f2c(*oldcomm), *nodes, cindex, cedges, *reorder, &cnewcomm);
  *newcomm = MPI_Comm_c2f(cnewcomm);
  free(cindex);
  free(cedges);
}

void F77_FUNC_(mpi_graphdims_get, MPI_GRAPHDIMS_GET)(MPI_Fint *comm, MPI_Fint *nodes, MPI_Fint *edges, MPI_Fint *ierr)
{
  int cnodes, cedges;
  *ierr = MPI_Graphdims_get(MPI_Comm_f2c(*comm), &cnodes, &cedges);
  *nodes = cnodes;
  *edges = cedges;
}

void F77_FUNC_(mpi_graph_get, MPI_GRAPH_GET)(MPI_Fint *comm, MPI_Fint *maxindex, MPI_Fint *maxedges, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *ierr)
{
  int i;
  int *cindex, *cedges;
  assert((cindex = (int*)calloc(*maxindex, sizeof(int))) != NULL);
  assert((cedges = (int*)calloc(*maxedges, sizeof(int))) != NULL);
  *ierr = MPI_Graph_get(MPI_Comm_f2c(*comm), *maxindex, *maxedges, cindex, cedges);
  for(i = 0; i < *maxindex; ++i) index[i] = cindex[i];
  for(i = 0; i < *maxedges; ++i) edges[i] = cedges[i];
  free(cindex);
  free(cedges);
}

void F77_FUNC_(mpi_cartdim_get, MPI_CARTDIM_GET)(MPI_Fint *comm, MPI_Fint *ndim, MPI_Fint *ierr)
{
  int cndim;
  *ierr = MPI_Cartdim_get(MPI_Comm_f2c(*comm), &cndim);
  *ndim = cndim;
}

void F77_FUNC_(mpi_cart_get, MPI_CART_GET)(MPI_Fint *comm, MPI_Fint *maxdims, MPI_Fint *dims, MPI_Fint *periods, MPI_Fint *coords, MPI_Fint *ierr)
{
  int i;
  int *cdims, *cperiods, *ccoords;
  assert((cdims = (int*)calloc(*maxdims, sizeof(int))) != NULL);
  assert((cperiods = (int*)calloc(*maxdims, sizeof(int))) != NULL);
  assert((ccoords = (int*)calloc(*maxdims, sizeof(int))) != NULL);
  *ierr = MPI_Cart_get(MPI_Comm_f2c(*comm), *maxdims, cdims, cperiods, ccoords);
  for(i = 0; i < *maxdims; ++i) {
    dims[i] = cdims[i];
    periods[i] = cperiods[i];
    coords[i] = ccoords[i];
  }
  free(cdims);
  free(cperiods);
  free(ccoords);
}

void F77_FUNC_(mpi_cart_rank, MPI_CART_RANK)(MPI_Fint *comm, MPI_Fint *coords, MPI_Fint *rank, MPI_Fint *ierr)
{
  int i, ndim, crank;
  int *ccoords;
  PMPI_Cartdim_get(MPI_Comm_f2c(*comm), &ndim);
  assert((ccoords = (int*)calloc(ndim, sizeof(int))) != NULL);
  for(i = 0; i < ndim; ++i) ccoords[i] = coords[i];
  *ierr = MPI_Cart_rank(MPI_Comm_f2c(*comm), ccoords, &crank);
  *rank = crank;
  free(ccoords);
}

void F77_FUNC_(mpi_cart_coords, MPI_CART_COORDS)(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxdims, MPI_Fint *coords, MPI_Fint *ierr)
{
  int i;
  int *ccoords;
  assert((ccoords = (int*)calloc(*maxdims, sizeof(int))) != NULL);
  *ierr = MPI_Cart_coords(MPI_Comm_f2c(*comm), *rank, *maxdims, ccoords);
  for(i = 0; i < *maxdims; ++i) coords[i] = ccoords[i];
  free(ccoords);
}

void F77_FUNC_(mpi_graph_neighbors_count, MPI_GRAPH_NEIGHBORS_COUNT)(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *nneigh, MPI_Fint *ierr)
{
  int cnneigh;
  *ierr = MPI_Graph_neighbors_count(MPI_Comm_f2c(*comm), *rank, &cnneigh);
  *nneigh = cnneigh;
}

void F77_FUNC_(mpi_graph_neighbors, MPI_GRAPH_NEIGHBORS)(MPI_Fint *comm, MPI_Fint *rank, MPI_Fint *maxneighbors, MPI_Fint *neighbors, MPI_Fint *ierr)
{
  int i, *cneighbors;
  assert((cneighbors = (int*)calloc(*maxneighbors, sizeof(int))) != NULL);
  *ierr = MPI_Graph_neighbors(MPI_Comm_f2c(*comm), *rank, *maxneighbors, cneighbors);
  for(i = 0; i < *maxneighbors; ++i) neighbors[i] = cneighbors[i];
}

void F77_FUNC_(mpi_cart_shift, MPI_CART_SHIFT)(MPI_Fint *comm, MPI_Fint *direction, MPI_Fint *displ, MPI_Fint *source, MPI_Fint *dest, MPI_Fint *ierr)
{
  int csource, cdest;
  *ierr = MPI_Cart_shift(MPI_Comm_f2c(*comm), *direction, *displ, &csource, &cdest);
  *source = csource;
  *dest = cdest;
}

void F77_FUNC_(mpi_cart_sub, MPI_CART_SUB)(MPI_Fint *oldcomm, MPI_Fint *remain_dims, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  int i, ndim;
  int *cremain_dims;
  MPI_Comm coldcomm, cnewcomm;
  coldcomm = MPI_Comm_f2c(*oldcomm);
  PMPI_Cartdim_get(MPI_Comm_f2c(*oldcomm), &ndim);
  assert((cremain_dims = (int*)calloc(ndim, sizeof(int))) != NULL);
  for(i = 0; i < ndim; ++i) cremain_dims[i] = remain_dims[i];
  *ierr = MPI_Cart_sub(coldcomm, cremain_dims, &cnewcomm);
  *newcomm = MPI_Comm_c2f(cnewcomm);
  free(cremain_dims);
}

void F77_FUNC_(mpi_cart_map, MPI_CART_MAP)(MPI_Fint *comm, MPI_Fint *ndim, MPI_Fint *dims, MPI_Fint *period, MPI_Fint *newrank, MPI_Fint *ierr)
{
  int i, *cdims, *cperiod, cnewrank;
  assert((cdims = (int*)calloc(*ndim, sizeof(int))) != NULL);
  assert((cperiod = (int*)calloc(*ndim, sizeof(int))) != NULL);
  for(i = 0; i < *ndim; ++i) {
    cdims[i] = dims[i];
    cperiod[i] = period[i];
  }
  *ierr = MPI_Cart_map(MPI_Comm_f2c(*comm), *ndim, cdims, cperiod, &cnewrank);
  *newrank = cnewrank;
}

void F77_FUNC_(mpi_graph_map, MPI_GRAPH_MAP)(MPI_Fint *comm, MPI_Fint *nodes, MPI_Fint *index, MPI_Fint *edges, MPI_Fint *newrank, MPI_Fint *ierr)
{
  int i, numedges, *cindex, *cedges, cnewrank;
  MPI_Comm ccomm;
  ccomm = MPI_Comm_f2c(*comm);
  numedges = edges[*nodes - 1];
  assert((cindex = (int*)calloc(*nodes, sizeof(int))) != NULL);
  assert((cedges = (int*)calloc(numedges, sizeof(int))) != NULL);
  for(i = 0; i < *nodes; ++i) cindex[i] = index[i];
  for(i = 0; i < numedges; ++i) cedges[i] = edges[i];
  *ierr = MPI_Graph_map(ccomm, *nodes, cindex, cedges, &cnewrank);
  *newrank = cnewrank;
  free(cindex);
  free(cedges);
}

/* SKIP:  I hate Fortran string handling
void F77_FUNC_(mpi_get_processor_name, MPI_GET_PROCESSOR_NAME)(char *name, MPI_Fint *resultlen, MPI_Fint *ierr)
{
  *ierr = MPI_Get_processor_name(*name, *resultlen);
}
*/

void F77_FUNC_(mpi_get_version, MPI_GET_VERSION)(MPI_Fint *version, MPI_Fint *subversion, MPI_Fint *ierr)
{
  int cversion, csubversion;
  *ierr = MPI_Get_version(&cversion, &csubversion);
  *version = cversion;
  *subversion = csubversion;
}

/* SKIP:  Not sure about portability of wrapping Fortran functions
void F77_FUNC_(mpi_errhandler_create, MPI_ERRHANDLER_CREATE)(MPI_Fint *function, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  *ierr = MPI_Errhandler_create(*function, MPI_Errhandler_f2c(*errhandler));
}
*/

void F77_FUNC_(mpi_errhandler_set, MPI_ERRHANDLER_SET)(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  MPI_Errhandler cerrhandler = MPI_Errhandler_f2c(*errhandler);
  *ierr = MPI_Errhandler_set(MPI_Comm_f2c(*comm), cerrhandler);
}

void F77_FUNC_(mpi_errhandler_get, MPI_ERRHANDLER_GET)(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  MPI_Errhandler cerrhandler;
  *ierr = MPI_Errhandler_get(MPI_Comm_f2c(*comm), &cerrhandler);
  *errhandler = MPI_Errhandler_c2f(cerrhandler);
}

void F77_FUNC_(mpi_errhandler_free, MPI_ERRHANDLER_FREE)(MPI_Fint *errhandler, MPI_Fint *ierr)
{
  MPI_Errhandler cerrhandler = MPI_Errhandler_f2c(*errhandler);
  *ierr = MPI_Errhandler_free(&cerrhandler);
}

/* SKIP:  Don't like Fortran string handling.
void F77_FUNC_(mpi_error_string, MPI_ERROR_STRING)(MPI_Fint *errorcode, char *errorstring, MPI_Fint *resultlen, MPI_Fint *ierr)
{
  *ierr = MPI_Error_string(*errorcode, *errorstring, *resultlen);
}
*/

void F77_FUNC_(mpi_error_class, MPI_ERROR_CLASS)(MPI_Fint *errorcode, MPI_Fint *errorclass, MPI_Fint *ierr)
{
  int cerrorclass;
  *ierr = MPI_Error_class(*errorcode, &cerrorclass);
  *errorclass = cerrorclass;
}
 
void F77_FUNC_(mpi_init, MPI_INIT)(MPI_Fint *ierr)
{
  *ierr = MPI_Init(0, NULL);
}

void F77_FUNC_(mpi_finalize, MPI_FINALIZE)(MPI_Fint *ierr)
{
  *ierr = MPI_Finalize();
}

void F77_FUNC_(mpi_initialized, MPI_INITIALIZED)(MPI_Fint *result, MPI_Fint *ierr)
{
  int cresult;
  *ierr = MPI_Initialized(&cresult);
  *result = cresult;
}

void F77_FUNC_(mpi_abort, MPI_ABORT)(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
  *ierr = MPI_Abort(MPI_Comm_f2c(*comm), *errorcode);
}

/* SKIP: String handling
void F77_FUNC_(mpi_close_port, MPI_CLOSE_PORT)(char *portname, MPI_Fint *ierr)
{
  *ierr = MPI_Close_port(*portname);
}
*/

/* SKIP: String handling
void F77_FUNC_(mpi_comm_accept, MPI_COMM_ACCEPT)(char *portname, MPI_Fint *info, MPI_Fint *root, MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_accept(*portname, MPI_Info_f2c(*info), *root, MPI_Comm_f2c(*oldcomm), MPI_Comm_f2c(*newcomm));
}
*/

/* SKIP:  Don't want to deal with Fortran string handling right now.
void F77_FUNC_(mpi_comm_connect, MPI_COMM_CONNECT)(char *portname, MPI_Fint *info, MPI_Fint *root, MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_connect(*portname, MPI_Info_f2c(*info), *root, MPI_Comm_f2c(*oldcomm), MPI_Comm_f2c(*newcomm));
}
*/

void F77_FUNC_(mpi_comm_disconnect, MPI_COMM_DISCONNECT)(MPI_Fint *comm, MPI_Fint *ierr)
{
  MPI_Comm ccomm = MPI_Comm_f2c(*comm);
  *ierr = MPI_Comm_disconnect(&ccomm);
}

void F77_FUNC_(mpi_comm_get_parent, MPI_COMM_GET_PARENT)(MPI_Fint *parent, MPI_Fint *ierr)
{
  MPI_Comm cparent;
  *ierr = MPI_Comm_get_parent(&cparent);
  *parent = MPI_Comm_c2f(cparent);
}

void F77_FUNC_(mpi_comm_join, MPI_COMM_JOIN)(MPI_Fint *fd, MPI_Fint *comm, MPI_Fint *ierr)
{
  MPI_Comm ccomm;
  *ierr = MPI_Comm_join(*fd, &ccomm);
  *comm = MPI_Comm_c2f(ccomm);
}

/* SKIP
void F77_FUNC_(mpi_comm_spawn, MPI_COMM_SPAWN)(char *command, char *argv, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *errcodes, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_spawn(*command, *argv, *maxprocs, MPI_Info_f2c(*info), *root, MPI_Comm_f2c(*oldcomm), MPI_Comm_f2c(*newcomm), *errcodes);
}
*/

/* SKIP
void F77_FUNC_(mpi_comm_spawn_multiple, MPI_COMM_SPAWN_MULTIPLE)(MPI_Fint *count, char *commands, char *argvs, MPI_Fint *maxprocs, MPI_Fint *info, MPI_Fint *root, MPI_Fint *oldcomm, MPI_Fint *newcomm, MPI_Fint *errcodes, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_spawn_multiple(*count, *commands, *argvs, *maxprocs, MPI_Info_f2c(*info), *root, MPI_Comm_f2c(*oldcomm), MPI_Comm_f2c(*newcomm), *errcodes);
}
*/

/* SKIP
void F77_FUNC_(mpi_lookup_name, MPI_LOOKUP_NAME)(char *servicename, MPI_Fint *info, char *portname, MPI_Fint *ierr)
{
  *ierr = MPI_Lookup_name(*servicename, MPI_Info_f2c(*info), *portname);
}
*/

/* SKIP
void F77_FUNC_(mpi_open_port, MPI_OPEN_PORT)(MPI_Fint *info, char *portname, MPI_Fint *ierr)
{
  *ierr = MPI_Open_port(MPI_Info_f2c(*info), *portname);
}
*/

/* SKIP
void F77_FUNC_(mpi_publish_name, MPI_PUBLISH_NAME)(char *servicename, MPI_Fint *info, char *portname, MPI_Fint *ierr)
{
  *ierr = MPI_Publish_name(*servicename, MPI_Info_f2c(*info), *portname);
}
*/

 /* SKIP
void F77_FUNC_(mpi_unpublish_name, MPI_UNPUBLISH_NAME)(char *servicename, MPI_Fint *info, char *portname, MPI_Fint *ierr)
{
  *ierr = MPI_Unpublish_name(*servicename, MPI_Info_f2c(*info), *portname);
}
 */

void F77_FUNC_(mpi_accumulate, MPI_ACCUMULATE)(void *buf, MPI_Fint *origincount, MPI_Fint *origintype, MPI_Fint *targetrank, MPI_Aint *targetdisp, MPI_Fint *targetcount, MPI_Fint *targettype, MPI_Fint *op, MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Accumulate(buf, *origincount, MPI_Type_f2c(*origintype), *targetrank, *targetdisp, *targetcount, MPI_Type_f2c(*targettype), MPI_Op_f2c(*op), MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_get, MPI_GET)(void *buf, MPI_Fint *origincount, MPI_Fint *origintype, MPI_Fint *targetrank, MPI_Aint *targetdisp, MPI_Fint *targetcount, MPI_Fint *targettype, MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Get(buf, *origincount, MPI_Type_f2c(*origintype), *targetrank, *targetdisp, *targetcount, MPI_Type_f2c(*targettype), MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_put, MPI_PUT)(void *buf, MPI_Fint *origincount, MPI_Fint *origintype, MPI_Fint *targetrank, MPI_Aint *targetdisp, MPI_Fint *targetcount, MPI_Fint *targettype, MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Put(buf, *origincount, MPI_Type_f2c(*origintype), *targetrank, *targetdisp, *targetcount, MPI_Type_f2c(*targettype), MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_win_complete, MPI_WIN_COMPLETE)(MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Win_complete(MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_win_create, MPI_WIN_CREATE)(void *addr, MPI_Aint *size, MPI_Fint *dispunit, MPI_Fint *info, MPI_Fint *comm, MPI_Fint *win, MPI_Fint *ierr)
{
  MPI_Win cwin;
  *ierr = MPI_Win_create(addr, *size, *dispunit, MPI_Info_f2c(*info), MPI_Comm_f2c(*comm), &cwin);
  *win = MPI_Win_c2f(cwin);
}

void F77_FUNC_(mpi_win_fence, MPI_WIN_FENCE)(MPI_Fint *assertion, MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Win_fence(*assertion, MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_win_free, MPI_WIN_FREE)(MPI_Fint *win, MPI_Fint *ierr)
{
  MPI_Win cwin = MPI_Win_f2c(*win);
  *ierr = MPI_Win_free(&cwin);
}

void F77_FUNC_(mpi_win_get_group, MPI_WIN_GET_GROUP)(MPI_Fint *win, MPI_Fint *group, MPI_Fint *ierr)
{
  MPI_Group cgroup;
  *ierr = MPI_Win_get_group(MPI_Win_f2c(*win), &cgroup);
  *group = MPI_Group_c2f(cgroup);
}

void F77_FUNC_(mpi_win_lock, MPI_WIN_LOCK)(MPI_Fint *locktype, MPI_Fint *winrank, MPI_Fint *assertion, MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Win_lock(*locktype, *winrank, *assertion, MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_win_post, MPI_WIN_POST)(MPI_Fint *group, MPI_Fint *assertion, MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Win_post(MPI_Group_f2c(*group), *assertion, MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_win_start, MPI_WIN_START)(MPI_Fint *group, MPI_Fint *assertion, MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Win_start(MPI_Group_f2c(*group), *assertion, MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_win_test, MPI_WIN_TEST)(MPI_Fint *win, MPI_Fint *flag, MPI_Fint *ierr)
{
  int cflag;
  *ierr = MPI_Win_test(MPI_Win_f2c(*win), &cflag);
  *flag = cflag;
}

void F77_FUNC_(mpi_win_unlock, MPI_WIN_UNLOCK)(MPI_Fint *winrank, MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Win_unlock(*winrank, MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_win_wait, MPI_WIN_WAIT)(MPI_Fint *win, MPI_Fint *ierr)
{
  *ierr = MPI_Win_wait(MPI_Win_f2c(*win));
}

void F77_FUNC_(mpi_alltoallw, MPI_ALLTOALLW)(void *sendbuf, MPI_Fint *sendcounts, MPI_Fint *senddispls, MPI_Fint *sendtypes, void *recvbuf, MPI_Fint *recvcounts, MPI_Fint *recvdispls, MPI_Fint *recvtypes, MPI_Fint *comm, MPI_Fint *ierr)
{
  int i, commsize;
  int *csendcounts, *csenddispls, *crecvcounts, *crecvdispls;
  MPI_Datatype *csendtypes, *crecvtypes;
  PMPI_Comm_size(MPI_Comm_f2c(*comm), &commsize);
  assert((csendcounts = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((csenddispls = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((crecvcounts = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((crecvdispls = (int*)calloc(commsize, sizeof(int))) != NULL);
  assert((csendtypes = (MPI_Datatype*)calloc(commsize, sizeof(MPI_Datatype))) != NULL);
  assert((crecvtypes = (MPI_Datatype*)calloc(commsize, sizeof(MPI_Datatype))) != NULL);
  for(i = 0; i < commsize; ++i) {
    csendcounts[i] = sendcounts[i];
    csenddispls[i] = senddispls[i];
    crecvcounts[i] = recvcounts[i];
    crecvdispls[i] = recvdispls[i];
    csendtypes[i] = MPI_Type_f2c(sendtypes[i]);
    crecvtypes[i] = MPI_Type_f2c(recvtypes[i]);
  }
  *ierr = MPI_Alltoallw(sendbuf, csendcounts, csenddispls, csendtypes, recvbuf, crecvcounts, crecvdispls, crecvtypes, MPI_Comm_f2c(*comm));
  free(csendcounts);
  free(csenddispls);
  free(crecvcounts);
  free(crecvdispls);
  free(csendtypes);
  free(crecvtypes);
}

void F77_FUNC_(mpi_exscan, MPI_EXSCAN)(void *sendbuf, void *recvbuf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *op, MPI_Fint *comm, MPI_Fint *ierr)
{
  *ierr = MPI_Exscan(sendbuf, recvbuf, *count, MPI_Type_f2c(*datatype), MPI_Op_f2c(*op), MPI_Comm_f2c(*comm));
}

void F77_FUNC_(mpi_add_error_class, MPI_ADD_ERROR_CLASS)(MPI_Fint *errorclass, MPI_Fint *ierr)
{
  int cerrorclass = *errorclass;
  *ierr = MPI_Add_error_class(&cerrorclass);
  *errorclass = cerrorclass;
}

void F77_FUNC_(mpi_add_error_code, MPI_ADD_ERROR_CODE)(MPI_Fint *errorclass, MPI_Fint *errorcode, MPI_Fint *ierr)
{
  int cerrorcode = *errorcode;
  *ierr = MPI_Add_error_code(*errorclass, &cerrorcode);
  *errorcode = cerrorcode;
}

/* SKIP
void F77_FUNC_(mpi_add_error_string, MPI_ADD_ERROR_STRING)(MPI_Fint *errorcode, char *errorstring, MPI_Fint *ierr)
{
  *ierr = MPI_Add_error_string(*errorcode, *errorstring);
}
*/

void F77_FUNC_(mpi_comm_call_errhandler, MPI_COMM_CALL_ERRHANDLER)(MPI_Fint *comm, MPI_Fint *errorcode, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_call_errhandler(MPI_Comm_f2c(*comm), *errorcode);
}

/* SKIP
void F77_FUNC_(mpi_comm_create_keyval, MPI_COMM_CREATE_KEYVAL)(MPI_Fint *copyfunc, MPI_Fint *deletefunc, MPI_Fint *keyval, void *extrastate, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_create_keyval(*copyfunc, *deletefunc, *keyval, extrastate);
}
*/

void F77_FUNC_(mpi_comm_delete_attr, MPI_COMM_DELETE_ATTR)(MPI_Fint *comm, MPI_Fint *keyval, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_delete_attr(MPI_Comm_f2c(*comm), *keyval);
}

void F77_FUNC_(mpi_comm_free_keyval, MPI_COMM_FREE_KEYVAL)(MPI_Fint *keyval, MPI_Fint *ierr)
{
  int ckeyval = *keyval;
  *ierr = MPI_Comm_free_keyval(&ckeyval);
  *keyval = ckeyval;
}

void F77_FUNC_(mpi_comm_get_attr, MPI_COMM_GET_ATTR)(MPI_Fint *comm, MPI_Fint *keyval, void *value, MPI_Fint *flag, MPI_Fint *ierr)
{
  int cflag;
  *ierr = MPI_Comm_get_attr(MPI_Comm_f2c(*comm), *keyval, value, &cflag);
  *flag = cflag;
}

/* SKIP
void F77_FUNC_(mpi_comm_get_name, MPI_COMM_GET_NAME)(MPI_Fint *comm, char *name, MPI_Fint *resultlen, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_get_name(MPI_Comm_f2c(*comm), *name, *resultlen);
}
*/

void F77_FUNC_(mpi_comm_set_attr, MPI_COMM_SET_ATTR)(MPI_Fint *comm, MPI_Fint *keyval, void *value, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_set_attr(MPI_Comm_f2c(*comm), *keyval, value);
}

/* SKIP
void F77_FUNC_(mpi_comm_set_name, MPI_COMM_SET_NAME)(MPI_Fint *comm, char *name, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_set_name(MPI_Comm_f2c(*comm), *name);
}
*/

void F77_FUNC_(mpi_file_call_errhandler, MPI_FILE_CALL_ERRHANDLER)(MPI_Fint *file, MPI_Fint *errorcode, MPI_Fint *ierr)
{
  *ierr = MPI_File_call_errhandler(MPI_File_f2c(*file), *errorcode);
}

void F77_FUNC_(mpi_grequest_complete, MPI_GREQUEST_COMPLETE)(MPI_Fint *request, MPI_Fint *ierr)
{
  *ierr = MPI_Grequest_complete(MPI_Request_f2c(*request));
}

/* SKIP
void F77_FUNC_(mpi_grequest_start, MPI_GREQUEST_START)(MPI_Fint *queryfunc, MPI_Fint *freefunc, MPI_Fint *cancelfunc, void *state, MPI_Fint *request, MPI_Fint *ierr)
{
  *ierr = MPI_Grequest_start(*queryfunc, *freefunc, *cancelfunc, state, MPI_Request_f2c(*request));
}
*/

void F77_FUNC_(mpi_init_thread, MPI_INIT_THREAD)(MPI_Fint *required, MPI_Fint *provided, MPI_Fint *ierr)
{
  int cprovided;
  *ierr = MPI_Init_thread(0, NULL, *required, &cprovided);
  *provided = cprovided;
}

void F77_FUNC_(mpi_is_thread_main, MPI_IS_THREAD_MAIN)(MPI_Fint *flag, MPI_Fint *ierr)
{
  int cflag;
  *ierr = MPI_Is_thread_main(&cflag);
  *flag = cflag;
}

void F77_FUNC_(mpi_query_thread, MPI_QUERY_THREAD)(MPI_Fint *supported, MPI_Fint *ierr)
{
  int csupported;
  *ierr = MPI_Query_thread(&csupported);
  *supported = csupported;
}

void F77_FUNC_(mpi_status_set_cancelled, MPI_STATUS_SET_CANCELLED)(MPI_Fint *status, MPI_Fint *flag, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_inbound_status(status);
  *ierr = MPI_Status_set_cancelled(cstatus, *flag);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_status_set_elements, MPI_STATUS_SET_ELEMENTS)(MPI_Fint *status, MPI_Fint *datatype, MPI_Fint *count, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_inbound_status(status);
  *ierr = MPI_Status_set_elements(cstatus, MPI_Type_f2c(*datatype), *count);
  finish_status(cstatus, status);
}

/* SKIP
void F77_FUNC_(mpi_type_create_keyval, MPI_TYPE_CREATE_KEYVAL)(MPI_Fint *copyfunc, MPI_Fint *deletefunc, MPI_Fint *keyval, void *state, MPI_Fint *ierr)
{
  *ierr = MPI_Type_create_keyval(*copyfunc, *deletefunc, *keyval, state);
}
*/

void F77_FUNC_(mpi_type_delete_attr, MPI_TYPE_DELETE_ATTR)(MPI_Fint *datatype, MPI_Fint *keyval, MPI_Fint *ierr)
{
  *ierr = MPI_Type_delete_attr(MPI_Type_f2c(*datatype), *keyval);
}

void F77_FUNC_(mpi_type_dup, MPI_TYPE_DUP)(MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  MPI_Datatype cnewtype;
  *ierr = MPI_Type_dup(MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
}

void F77_FUNC_(mpi_type_free_keyval, MPI_TYPE_FREE_KEYVAL)(MPI_Fint *keyval, MPI_Fint *ierr)
{
  int ckeyval = *keyval;
  *ierr = MPI_Type_free_keyval(&ckeyval);
  *keyval = ckeyval;
}

void F77_FUNC_(mpi_type_get_attr, MPI_TYPE_GET_ATTR)(MPI_Fint *datatype, MPI_Fint *keyval, void *value, MPI_Fint *flag, MPI_Fint *ierr)
{
  int cflag;
  *ierr = MPI_Type_get_attr(MPI_Type_f2c(*datatype), *keyval, value, &cflag);
  *flag = cflag;
}

void F77_FUNC_(mpi_type_get_contents, MPI_TYPE_GET_CONTENTS)(MPI_Fint *datatype, MPI_Fint *maxintegers, MPI_Fint *maxaddresses, MPI_Fint *maxdatatypes, MPI_Fint *arrintegers, MPI_Aint *arraddresses, MPI_Fint *arrdatatypes, MPI_Fint *ierr)
{
  int i;
  int *carrintegers;
  MPI_Datatype *carrdatatypes;
  assert((carrintegers = (int*)calloc(*maxintegers, sizeof(int))) != NULL);
  assert((carrdatatypes = (MPI_Datatype*)calloc(*maxdatatypes, sizeof(MPI_Datatype))) != NULL);
  *ierr = MPI_Type_get_contents(MPI_Type_f2c(*datatype), *maxintegers, *maxaddresses, *maxdatatypes, carrintegers, arraddresses, carrdatatypes);
  for(i = 0; i < *maxintegers; ++i) arrintegers[i] = carrintegers[i];
  for(i = 0; i < *maxdatatypes; ++i)
    arrdatatypes[i] = MPI_Type_c2f(carrdatatypes[i]);
  free(carrintegers);
  free(carrdatatypes);
}

void F77_FUNC_(mpi_type_get_envelope, MPI_TYPE_GET_ENVELOPE)(MPI_Fint *datatype, MPI_Fint *numintegers, MPI_Fint *numaddresses, MPI_Fint *numdatatypes, MPI_Fint *combiner, MPI_Fint *ierr)
{
  int cnumintegers, cnumaddresses, cnumdatatypes, ccombiner;
  *ierr = MPI_Type_get_envelope(MPI_Type_f2c(*datatype), &cnumintegers, &cnumaddresses, &cnumdatatypes, &ccombiner);
  *numintegers = cnumintegers;
  *numaddresses = cnumaddresses;
  *numdatatypes = cnumdatatypes;
  *combiner = ccombiner;
}

/* SKIP
void F77_FUNC_(mpi_type_get_name, MPI_TYPE_GET_NAME)(MPI_Fint *datatype, char *name, MPI_Fint *resultlen, MPI_Fint *ierr)
{
  *ierr = MPI_Type_get_name(MPI_Type_f2c(*datatype), *name, *resultlen);
}
*/

void F77_FUNC_(mpi_type_set_attr, MPI_TYPE_SET_ATTR)(MPI_Fint *datatype, MPI_Fint *keyval, void *state, MPI_Fint *ierr)
{
  *ierr = MPI_Type_set_attr(MPI_Type_f2c(*datatype), *keyval, state);
}

/* SKIP
void F77_FUNC_(mpi_type_set_name, MPI_TYPE_SET_NAME)(MPI_Fint *datatype, char *name, MPI_Fint *ierr)
{
  *ierr = MPI_Type_set_name(MPI_Type_f2c(*datatype), *name);
}
*/

void F77_FUNC_(mpi_type_match_size, MPI_TYPE_MATCH_SIZE)(MPI_Fint *typeclass, MPI_Fint *size, MPI_Fint *datatype, MPI_Fint *ierr)
{
  MPI_Datatype cdatatype;
  *ierr = MPI_Type_match_size(*typeclass, *size, &cdatatype);
  *datatype = MPI_Type_c2f(cdatatype);
}

void F77_FUNC_(mpi_win_call_errhandler, MPI_WIN_CALL_ERRHANDLER)(MPI_Fint *win, MPI_Fint *errorcode, MPI_Fint *ierr)
{
  *ierr = MPI_Win_call_errhandler(MPI_Win_f2c(*win), *errorcode);
}

/* SKIP
void F77_FUNC_(mpi_win_create_keyval, MPI_WIN_CREATE_KEYVAL)(MPI_Fint *copyfun, MPI_Fint *deletefun, MPI_Fint *keyval, void *state, MPI_Fint *ierr)
{
  *ierr = MPI_Win_create_keyval(*copyfun, *deletefun, *keyval, state);
}
*/

void F77_FUNC_(mpi_win_delete_attr, MPI_WIN_DELETE_ATTR)(MPI_Fint *win, MPI_Fint *keyval, MPI_Fint *ierr)
{
  *ierr = MPI_Win_delete_attr(MPI_Win_f2c(*win), *keyval);
}

void F77_FUNC_(mpi_win_free_keyval, MPI_WIN_FREE_KEYVAL)(MPI_Fint *keyval, MPI_Fint *ierr)
{
  int ckeyval = *keyval;
  *ierr = MPI_Win_free_keyval(&ckeyval);
  *keyval = ckeyval;
}

void F77_FUNC_(mpi_win_get_attr, MPI_WIN_GET_ATTR)(MPI_Fint *win, MPI_Fint *keyval, void *state, MPI_Fint *flag, MPI_Fint *ierr)
{
  int cflag;
  *ierr = MPI_Win_get_attr(MPI_Win_f2c(*win), *keyval, state, &cflag);
  *flag = cflag;
}

/* SKIP
void F77_FUNC_(mpi_win_get_name, MPI_WIN_GET_NAME)(MPI_Fint *win, char *name, MPI_Fint *resultlen, MPI_Fint *ierr)
{
  *ierr = MPI_Win_get_name(MPI_Win_f2c(*win), *name, *resultlen);
}
*/

void F77_FUNC_(mpi_win_set_attr, MPI_WIN_SET_ATTR)(MPI_Fint *win, MPI_Fint *keyval, void *state, MPI_Fint *ierr)
{
  *ierr = MPI_Win_set_attr(MPI_Win_f2c(*win), *keyval, state);
}

/*
void F77_FUNC_(mpi_win_set_name, MPI_WIN_SET_NAME)(MPI_Fint *win, char *name, MPI_Fint *ierr)
{
  *ierr = MPI_Win_set_name(MPI_Win_f2c(*win), *name);
}
*/

void F77_FUNC_(mpi_alloc_mem, MPI_ALLOC_MEM)(MPI_Aint *size, MPI_Fint *info, void *baseptr, MPI_Fint *ierr)
{
  *ierr = MPI_Alloc_mem(*size, MPI_Info_f2c(*info), baseptr);
}

/* SKIP
void F77_FUNC_(mpi_comm_create_errhandler, MPI_COMM_CREATE_ERRHANDLER)(MPI_Fint *errfun, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_create_errhandler(*errfun, MPI_Errhandler_f2c(*errhandler));
}
*/

void F77_FUNC_(mpi_comm_get_errhandler, MPI_COMM_GET_ERRHANDLER)(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  MPI_Errhandler cerrhandler;
  *ierr = MPI_Comm_get_errhandler(MPI_Comm_f2c(*comm), &cerrhandler);
  *errhandler = MPI_Errhandler_c2f(cerrhandler);
}

void F77_FUNC_(mpi_comm_set_errhandler, MPI_COMM_SET_ERRHANDLER)(MPI_Fint *comm, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  *ierr = MPI_Comm_set_errhandler(MPI_Comm_f2c(*comm), MPI_Errhandler_f2c(*errhandler));
}

/* SKIP
void F77_FUNC_(mpi_file_create_errhandler, MPI_FILE_CREATE_ERRHANDLER)(MPI_Fint *errfun, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  MPI_Errhandler cerrhandler;
  *ierr = MPI_File_create_errhandler(*errfun, &cerrhandler);
  *errhandler = MPI_Errhandler_c2f(cerrhandler);
}
*/

void F77_FUNC_(mpi_file_get_errhandler, MPI_FILE_GET_ERRHANDLER)(MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  MPI_Errhandler cerrhandler;
  *ierr = MPI_File_get_errhandler(MPI_File_f2c(*file), &cerrhandler);
  *errhandler = MPI_Errhandler_c2f(cerrhandler);
}

void F77_FUNC_(mpi_file_set_errhandler, MPI_FILE_SET_ERRHANDLER)(MPI_Fint *file, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  *ierr = MPI_File_set_errhandler(MPI_File_f2c(*file), MPI_Errhandler_f2c(*errhandler));
}

void F77_FUNC_(mpi_finalized, MPI_FINALIZED)(MPI_Fint *flag, MPI_Fint *ierr)
{
  int cflag;
  *ierr = MPI_Finalized(&cflag);
  *flag = cflag;
}

void F77_FUNC_(mpi_free_mem, MPI_FREE_MEM)(void *buf, MPI_Fint *ierr)
{
  *ierr = MPI_Free_mem(buf);
}

void F77_FUNC_(mpi_get_address, MPI_GET_ADDRESS)(void *buf, MPI_Aint *address, MPI_Fint *ierr)
{
  *ierr = MPI_Get_address(buf, address);
}

void F77_FUNC_(mpi_info_create, MPI_INFO_CREATE)(MPI_Fint *info, MPI_Fint *ierr)
{
  MPI_Info cinfo;
  *ierr = MPI_Info_create(&cinfo);
  *info = MPI_Info_c2f(cinfo);
}

/* SKIP
void F77_FUNC_(mpi_info_delete, MPI_INFO_DELETE)(MPI_Fint *info, char *key, MPI_Fint *ierr)
{
  *ierr = MPI_Info_delete(MPI_Info_f2c(*info), *key);
}
*/

void F77_FUNC_(mpi_info_dup, MPI_INFO_DUP)(MPI_Fint *oldinfo, MPI_Fint *newinfo, MPI_Fint *ierr)
{
  MPI_Info cnewinfo;
  *ierr = MPI_Info_dup(MPI_Info_f2c(*oldinfo), &cnewinfo);
  *newinfo = MPI_Info_c2f(cnewinfo);
}

void F77_FUNC_(mpi_info_free, MPI_INFO_FREE)(MPI_Fint *info, MPI_Fint *ierr)
{
  MPI_Info cinfo = MPI_Info_f2c(*info);
  *ierr = MPI_Info_free(&cinfo);
}

/* SKIP
void F77_FUNC_(mpi_info_get, MPI_INFO_GET)(MPI_Fint *info, char *key, MPI_Fint *valuelength, char *value, MPI_Fint *flag, MPI_Fint *ierr)
{
  *ierr = MPI_Info_get(MPI_Info_f2c(*info), *key, *valuelength, *value, *flag);
}
*/

void F77_FUNC_(mpi_info_get_nkeys, MPI_INFO_GET_NKEYS)(MPI_Fint *info, MPI_Fint *nkeys, MPI_Fint *ierr)
{
  int cnkeys;
  *ierr = MPI_Info_get_nkeys(MPI_Info_f2c(*info), &cnkeys);
  *nkeys = cnkeys;
}

/* SKIP
void F77_FUNC_(mpi_info_get_nthkey, MPI_INFO_GET_NTHKEY)(MPI_Fint *info, MPI_Fint *n, char *key, MPI_Fint *ierr)
{
  *ierr = MPI_Info_get_nthkey(MPI_Info_f2c(*info), *n, *key);
}
*/

/* SKIP
void F77_FUNC_(mpi_info_get_valuelen, MPI_INFO_GET_VALUELEN)(MPI_Fint *info, char *key, MPI_Fint *valuelen, MPI_Fint *flag, MPI_Fint *ierr)
{
  *ierr = MPI_Info_get_valuelen(MPI_Info_f2c(*info), *key, *valuelen, *flag);
}
*/

/* SKIP
void F77_FUNC_(mpi_info_set, MPI_INFO_SET)(MPI_Fint *info, char *key, char *value, MPI_Fint *ierr)
{
  *ierr = MPI_Info_set(MPI_Info_f2c(*info), *key, *value);
}
*/

/* SKIP
void F77_FUNC_(mpi_pack_external, MPI_PACK_EXTERNAL)(char *datarep, void *inbuf, MPI_Fint *incount, MPI_Fint *intype, void *outbuf, MPI_Aint *outcount, MPI_Aint *position, MPI_Fint *ierr)
{
  *ierr = MPI_Pack_external(*datarep, inbuf, *incount, MPI_Type_f2c(*intype), outbuf, *outcount, *position);
}
 */

/* SKIP
void F77_FUNC_(mpi_pack_external_size, MPI_PACK_EXTERNAL_SIZE)(char *datarep, MPI_Fint *incount, MPI_Fint *datatype, MPI_Aint *size, MPI_Fint *ierr)
{
  *ierr = MPI_Pack_external_size(*datarep, *incount, MPI_Type_f2c(*datatype), *size);
}
*/

void F77_FUNC_(mpi_request_get_status, MPI_REQUEST_GET_STATUS)(MPI_Fint *request, MPI_Fint *flag, MPI_Fint *status, MPI_Fint *ierr)
{
  int cflag;
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_Request_get_status(MPI_Request_f2c(*request), &cflag, cstatus);
  *flag = cflag;
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_type_create_darray, MPI_TYPE_CREATE_DARRAY)(MPI_Fint *size, MPI_Fint *rank, MPI_Fint *ndims, MPI_Fint *gsizes, MPI_Fint *distribs, MPI_Fint *dargs, MPI_Fint *psizes, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  int i, *cgsizes, *cdistribs, *cdargs, *cpsizes;
  MPI_Datatype cnewtype;
  assert((cgsizes = (int*)calloc(*ndims, sizeof(int))) != NULL);
  assert((cdistribs = (int*)calloc(*ndims, sizeof(int))) != NULL);
  assert((cdargs = (int*)calloc(*ndims, sizeof(int))) != NULL);
  assert((cpsizes = (int*)calloc(*ndims, sizeof(int))) != NULL);
  for(i = 0; i < *ndims; ++i) {
    cgsizes[i] = gsizes[i];
    cdistribs[i] = distribs[i];
    cdargs[i] = dargs[i];
    cpsizes[i] = psizes[i];
  }
  *ierr = MPI_Type_create_darray(*size, *rank, *ndims, cgsizes, cdistribs, cdargs, cpsizes, *order, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
  free(cgsizes);
  free(cdistribs);
  free(cdargs);
  free(cpsizes);
}

void F77_FUNC_(mpi_type_create_hindexed, MPI_TYPE_CREATE_HINDEXED)(MPI_Fint *count, MPI_Fint *blocklengths, MPI_Aint *displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  int i, *cblocklengths;
  MPI_Datatype cnewtype;
  assert((cblocklengths = (int*)calloc(*count, sizeof(int))) != NULL);
  for(i = 0; i < *count; ++i) cblocklengths[i] = blocklengths[i];
  *ierr = MPI_Type_create_hindexed(*count, cblocklengths, displacements, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
  free(cblocklengths);
}

void F77_FUNC_(mpi_type_create_hvector, MPI_TYPE_CREATE_HVECTOR)(MPI_Fint *count, MPI_Fint *blocklength, MPI_Aint *stride, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  MPI_Datatype cnewtype;
  *ierr = MPI_Type_create_hvector(*count, *blocklength, *stride, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
}

void F77_FUNC_(mpi_type_create_indexed_block, MPI_TYPE_CREATE_INDEXED_BLOCK)(MPI_Fint *count, MPI_Fint *blocklength, MPI_Fint *displacements, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  int i, *cdisplacements;
  MPI_Datatype cnewtype;
  assert((cdisplacements =  (int*)calloc(*count, sizeof(int))) != NULL);
  for(i = 0; i < *count; ++i) cdisplacements[i] = displacements[i];
  *ierr = MPI_Type_create_indexed_block(*count, *blocklength, cdisplacements, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
  free(cdisplacements);
}

void F77_FUNC_(mpi_type_create_resized, MPI_TYPE_CREATE_RESIZED)(MPI_Fint *oldtype, MPI_Aint *lb, MPI_Aint *extent, MPI_Fint *newtype, MPI_Fint *ierr)
{
  MPI_Datatype cnewtype;
  *ierr = MPI_Type_create_resized(MPI_Type_f2c(*oldtype), *lb, *extent, &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
}

void F77_FUNC_(mpi_type_create_struct, MPI_TYPE_CREATE_STRUCT)(MPI_Fint *count, MPI_Fint *blocklengths, MPI_Aint *displacements, MPI_Fint *oldtypes, MPI_Fint *newtype, MPI_Fint *ierr)
{
  int i, *cblocklengths;
  MPI_Datatype *coldtypes, cnewtype;
  assert((cblocklengths = (int*)calloc(*count, sizeof(int))) != NULL);
  assert((coldtypes = (MPI_Datatype*)calloc(*count, sizeof(MPI_Datatype))) != NULL);
  for(i = 0; i < *count; ++i) cblocklengths[i] = blocklengths[i];
  for(i = 0; i < *count; ++i) coldtypes[i] = MPI_Type_f2c(oldtypes[i]);
  *ierr = MPI_Type_create_struct(*count, cblocklengths, displacements, coldtypes, &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
  free(cblocklengths);
  free(coldtypes);
}

void F77_FUNC_(mpi_type_create_subarray, MPI_TYPE_CREATE_SUBARRAY)(MPI_Fint *ndims, MPI_Fint *sizes, MPI_Fint *subsizes, MPI_Fint *starts, MPI_Fint *order, MPI_Fint *oldtype, MPI_Fint *newtype, MPI_Fint *ierr)
{
  int i, *csizes, *csubsizes, *cstarts;
  MPI_Datatype cnewtype;
  assert((csizes = (int*)calloc(*ndims, sizeof(int))) != NULL);
  assert((csubsizes = (int*)calloc(*ndims, sizeof(int))) != NULL);
  assert((cstarts = (int*)calloc(*ndims, sizeof(int))) != NULL);
  for(i = 0; i < *ndims; ++i) csizes[i] = sizes[i];
  for(i = 0; i < *ndims; ++i) csubsizes[i] = subsizes[i];
  for(i = 0; i < *ndims; ++i) cstarts[i] = starts[i];
  *ierr = MPI_Type_create_subarray(*ndims, csizes, csubsizes, cstarts, *order, MPI_Type_f2c(*oldtype), &cnewtype);
  *newtype = MPI_Type_c2f(cnewtype);
  free(csizes);
  free(csubsizes);
  free(cstarts);
}

void F77_FUNC_(mpi_type_get_extent, MPI_TYPE_GET_EXTENT)(MPI_Fint *datatype, MPI_Aint *lb, MPI_Aint *extent, MPI_Fint *ierr)
{
  *ierr = MPI_Type_get_extent(MPI_Type_f2c(*datatype), lb, extent);
}

void F77_FUNC_(mpi_type_get_true_extent, MPI_TYPE_GET_TRUE_EXTENT)(MPI_Fint *datatype, MPI_Aint *lb, MPI_Aint *extent, MPI_Fint *ierr)
{
  *ierr = MPI_Type_get_true_extent(MPI_Type_f2c(*datatype), lb, extent);
}

/* SKIP
void F77_FUNC_(mpi_unpack_external, MPI_UNPACK_EXTERNAL)(char *datarep, void *inbuf, MPI_Aint *insize, MPI_Aint *position, void *outbuf, MPI_Fint *outcount, MPI_Fint *datatype, MPI_Fint *ierr)
{
  *ierr = MPI_Unpack_external(*datarep, inbuf, *insize, *position, outbuf, *outcount, MPI_Type_f2c(*datatype));
}
*/

/* SKIP
void F77_FUNC_(mpi_win_create_errhandler, MPI_WIN_CREATE_ERRHANDLER)(MPI_Fint *errfun, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  *ierr = MPI_Win_create_errhandler(*errfun, MPI_Errhandler_f2c(*errhandler));
}
*/

void F77_FUNC_(mpi_win_get_errhandler, MPI_WIN_GET_ERRHANDLER)(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  MPI_Errhandler cerrhandler;
  *ierr = MPI_Win_get_errhandler(MPI_Win_f2c(*win), &cerrhandler);
  *errhandler = MPI_Errhandler_c2f(cerrhandler);
}

void F77_FUNC_(mpi_win_set_errhandler, MPI_WIN_SET_ERRHANDLER)(MPI_Fint *win, MPI_Fint *errhandler, MPI_Fint *ierr)
{
  *ierr = MPI_Win_set_errhandler(MPI_Win_f2c(*win), MPI_Errhandler_f2c(*errhandler));
}

/* SKIP
void F77_FUNC_(mpi_file_open, MPI_FILE_OPEN)(MPI_Fint *comm, char *filename, MPI_Fint *amode, MPI_Fint *info, MPI_Fint *file, MPI_Fint *ierr)
{
  *ierr = MPI_File_open(MPI_Comm_f2c(*comm), *filename, *amode, MPI_Info_f2c(*info), *file);
}
*/

void F77_FUNC_(mpi_file_close, MPI_FILE_CLOSE)(MPI_Fint *file, MPI_Fint *ierr)
{
  MPI_File cfile = MPI_File_f2c(*file);
  *ierr = MPI_File_close(&cfile);
}

/* SKIP
void F77_FUNC_(mpi_file_delete, MPI_FILE_DELETE)(char *filename, MPI_Fint *info, MPI_Fint *ierr)
{
  *ierr = MPI_File_delete(*filename, MPI_Info_f2c(*info));
}
*/

void F77_FUNC_(mpi_file_set_size, MPI_FILE_SET_SIZE)(MPI_Fint *file, MPI_Offset *size, MPI_Fint *ierr)
{
  *ierr = MPI_File_set_size(MPI_File_f2c(*file), *size);
}

void F77_FUNC_(mpi_file_preallocate, MPI_FILE_PREALLOCATE)(MPI_Fint *file, MPI_Offset *size, MPI_Fint *ierr)
{
  *ierr = MPI_File_preallocate(MPI_File_f2c(*file), *size);
}

void F77_FUNC_(mpi_file_get_size, MPI_FILE_GET_SIZE)(MPI_Fint *file, MPI_Offset *size, MPI_Fint *ierr)
{
  *ierr = MPI_File_get_size(MPI_File_f2c(*file), size);
}

void F77_FUNC_(mpi_file_get_group, MPI_FILE_GET_GROUP)(MPI_Fint *file, MPI_Fint *group, MPI_Fint *ierr)
{
  MPI_Group cgroup;
  *ierr = MPI_File_get_group(MPI_File_f2c(*file), &cgroup);
  *group = MPI_Group_c2f(cgroup);
}

void F77_FUNC_(mpi_file_get_amode, MPI_FILE_GET_AMODE)(MPI_Fint *file, MPI_Fint *amode, MPI_Fint *ierr)
{
  int camode;
  *ierr = MPI_File_get_amode(MPI_File_f2c(*file), &camode);
  *amode = camode;
}

void F77_FUNC_(mpi_file_set_info, MPI_FILE_SET_INFO)(MPI_Fint *file, MPI_Fint *info, MPI_Fint *ierr)
{
  *ierr = MPI_File_set_info(MPI_File_f2c(*file), MPI_Info_f2c(*info));
}

void F77_FUNC_(mpi_file_get_info, MPI_FILE_GET_INFO)(MPI_Fint *file, MPI_Fint *info, MPI_Fint *ierr)
{
  MPI_Info cinfo;
  *ierr = MPI_File_get_info(MPI_File_f2c(*file), &cinfo);
  *info = MPI_Info_c2f(cinfo);
}

/* SKIP
void F77_FUNC_(mpi_file_set_view, MPI_FILE_SET_VIEW)(MPI_Fint *file, MPI_Fint *offset, MPI_Fint *hosttype, MPI_Fint *filetype, char *datarep, MPI_Fint *info, MPI_Fint *ierr)
{
  *ierr = MPI_File_set_view(*file, *offset, MPI_Type_f2c(*hosttype), MPI_Type_f2c(*filetype), *datarep, MPI_Info_f2c(*info));
}
*/

/* SKIP
void F77_FUNC_(mpi_file_get_view, MPI_FILE_GET_VIEW)(MPI_Fint *file, MPI_Fint *offset, MPI_Fint *hosttype, MPI_Fint *filetype, char *datarep, MPI_Fint *ierr)
{
  *ierr = MPI_File_get_view(*file, *offset, MPI_Type_f2c(*hosttype), MPI_Type_f2c(*filetype), *datarep);
}
*/

void F77_FUNC_(mpi_file_read_at, MPI_FILE_READ_AT)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read_at(MPI_File_f2c(*file), *offset, buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_read_at_all, MPI_FILE_READ_AT_ALL)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read_at_all(MPI_File_f2c(*file), *offset, buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write_at, MPI_FILE_WRITE_AT)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write_at(MPI_File_f2c(*file), *offset, buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write_at_all, MPI_FILE_WRITE_AT_ALL)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write_at_all(MPI_File_f2c(*file), *offset, buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_iread_at, MPI_FILE_IREAD_AT)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_File_iread_at(MPI_File_f2c(*file), *offset, buf, *count, MPI_Type_f2c(*datatype), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_file_iwrite_at, MPI_FILE_IWRITE_AT)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_File_iwrite_at(MPI_File_f2c(*file), *offset, buf, *count, MPI_Type_f2c(*datatype), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_file_read, MPI_FILE_READ)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_read_all, MPI_FILE_READ_ALL)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read_all(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write, MPI_FILE_WRITE)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write_all, MPI_FILE_WRITE_ALL)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write_all(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_iread, MPI_FILE_IREAD)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_File_iread(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_file_iwrite, MPI_FILE_IWRITE)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_File_iwrite(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_file_seek, MPI_FILE_SEEK)(MPI_Fint *file, MPI_Offset *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
  *ierr = MPI_File_seek(MPI_File_f2c(*file), *offset, *whence);
}

void F77_FUNC_(mpi_file_get_position, MPI_FILE_GET_POSITION)(MPI_Fint *file, MPI_Offset *offset, MPI_Fint *ierr)
{
  *ierr = MPI_File_get_position(MPI_File_f2c(*file), offset);
}

void F77_FUNC_(mpi_file_get_byte_offset, MPI_FILE_GET_BYTE_OFFSET)(MPI_Fint *file, MPI_Offset *offset, MPI_Offset *bytes, MPI_Fint *ierr)
{
  *ierr = MPI_File_get_byte_offset(MPI_File_f2c(*file), *offset, bytes);
}

void F77_FUNC_(mpi_file_read_shared, MPI_FILE_READ_SHARED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read_shared(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write_shared, MPI_FILE_WRITE_SHARED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write_shared(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_iread_shared, MPI_FILE_IREAD_SHARED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_File_iread_shared(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_file_iwrite_shared, MPI_FILE_IWRITE_SHARED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *request, MPI_Fint *ierr)
{
  MPI_Request crequest;
  *ierr = MPI_File_iwrite_shared(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), &crequest);
  *request = MPI_Request_c2f(crequest);
}

void F77_FUNC_(mpi_file_read_ordered, MPI_FILE_READ_ORDERED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read_ordered(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write_ordered, MPI_FILE_WRITE_ORDERED)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write_ordered(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype), cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_seek_shared, MPI_FILE_SEEK_SHARED)(MPI_Fint *file, MPI_Offset *offset, MPI_Fint *whence, MPI_Fint *ierr)
{
  *ierr = MPI_File_seek_shared(MPI_File_f2c(*file), *offset, *whence);
}

void F77_FUNC_(mpi_file_get_position_shared, MPI_FILE_GET_POSITION_SHARED)(MPI_Fint *file, MPI_Offset *offset, MPI_Fint *ierr)
{
  *ierr = MPI_File_get_position_shared(MPI_File_f2c(*file), offset);
}

void F77_FUNC_(mpi_file_read_at_all_begin, MPI_FILE_READ_AT_ALL_BEGIN)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
  *ierr = MPI_File_read_at_all_begin(MPI_File_f2c(*file), *offset, buf, *count, MPI_Type_f2c(*datatype));
}

void F77_FUNC_(mpi_file_read_at_all_end, MPI_FILE_READ_AT_ALL_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read_at_all_end(MPI_File_f2c(*file), buf, cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write_at_all_begin, MPI_FILE_WRITE_AT_ALL_BEGIN)(MPI_Fint *file, MPI_Offset *offset, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
  *ierr = MPI_File_write_at_all_begin(MPI_File_f2c(*file), *offset, buf, *count, MPI_Type_f2c(*datatype));
}

void F77_FUNC_(mpi_file_write_at_all_end, MPI_FILE_WRITE_AT_ALL_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write_at_all_end(MPI_File_f2c(*file), buf, cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_read_all_begin, MPI_FILE_READ_ALL_BEGIN)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
  *ierr = MPI_File_read_all_begin(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype));
}

void F77_FUNC_(mpi_file_read_all_end, MPI_FILE_READ_ALL_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read_all_end(MPI_File_f2c(*file), buf, cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write_all_begin, MPI_FILE_WRITE_ALL_BEGIN)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
  *ierr = MPI_File_write_all_begin(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype));
}

void F77_FUNC_(mpi_file_write_all_end, MPI_FILE_WRITE_ALL_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write_all_end(MPI_File_f2c(*file), buf, cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_read_ordered_begin, MPI_FILE_READ_ORDERED_BEGIN)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
  *ierr = MPI_File_read_ordered_begin(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype));
}

void F77_FUNC_(mpi_file_read_ordered_end, MPI_FILE_READ_ORDERED_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_read_ordered_end(MPI_File_f2c(*file), buf, cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_write_ordered_begin, MPI_FILE_WRITE_ORDERED_BEGIN)(MPI_Fint *file, void *buf, MPI_Fint *count, MPI_Fint *datatype, MPI_Fint *ierr)
{
  *ierr = MPI_File_write_ordered_begin(MPI_File_f2c(*file), buf, *count, MPI_Type_f2c(*datatype));
}

void F77_FUNC_(mpi_file_write_ordered_end, MPI_FILE_WRITE_ORDERED_END)(MPI_Fint *file, void *buf, MPI_Fint *status, MPI_Fint *ierr)
{
  MPI_Status *cstatus = start_outbound_status(status);
  *ierr = MPI_File_write_ordered_end(MPI_File_f2c(*file), buf, cstatus);
  finish_status(cstatus, status);
}

void F77_FUNC_(mpi_file_get_type_extent, MPI_FILE_GET_TYPE_EXTENT)(MPI_Fint *file, MPI_Fint *datatype, MPI_Aint *extent, MPI_Fint *ierr)
{
  *ierr = MPI_File_get_type_extent(MPI_File_f2c(*file), MPI_Type_f2c(*datatype), extent);
}

/* SKIP
void F77_FUNC_(mpi_register_datarep, MPI_REGISTER_DATAREP)(char *name, MPI_Fint *readfun, MPI_Fint *writefun, MPI_Fint *extentfun, void *state, MPI_Fint *ierr)
{
  *ierr = MPI_Register_datarep(*name, *readfun, *writefun, *extentfun, state);
}
*/

void F77_FUNC_(mpi_file_set_atomicity, MPI_FILE_SET_ATOMICITY)(MPI_Fint *file, MPI_Fint *flag, MPI_Fint *ierr)
{
  *ierr = MPI_File_set_atomicity(MPI_File_f2c(*file), *flag);
}

void F77_FUNC_(mpi_file_get_atomicity, MPI_FILE_GET_ATOMICITY)(MPI_Fint *file, MPI_Fint *flag, MPI_Fint *ierr)
{
  int cflag;
  *ierr = MPI_File_get_atomicity(MPI_File_f2c(*file), &cflag);
  *flag = cflag;
}

void F77_FUNC_(mpi_file_sync, MPI_FILE_SYNC)(MPI_Fint *file, MPI_Fint *ierr)
{
  *ierr = MPI_File_sync(MPI_File_f2c(*file));
}

/* THIS IS WHERE I AM AT

*/
