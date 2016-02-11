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

#include <dumpi/libundumpi/bindings.h>
#include <dumpi/libundumpi/freedefs.h>
#include <dumpi/common/dumpiio.h>
#include <string.h>
#include <stdlib.h>

#define DUMPI_RELEASE_PERFTAGS(PERF) do {		\
} while(0)

int libundumpi_grab_send(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_send val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_send));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_send(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_send_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_recv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_recv val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_recv));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_recv(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_recv_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_get_count(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_get_count val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_get_count));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_get_count(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_get_count_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_bsend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_bsend val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_bsend));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_bsend(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_bsend_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_ssend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_ssend val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_ssend));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_ssend(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_ssend_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_rsend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_rsend val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_rsend));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_rsend(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_rsend_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_buffer_attach(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_buffer_attach val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_buffer_attach));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_buffer_attach(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_buffer_attach_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_buffer_detach(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_buffer_detach val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_buffer_detach));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_buffer_detach(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_buffer_detach_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_isend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_isend val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_isend));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_isend(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_isend_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_ibsend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_ibsend val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_ibsend));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_ibsend(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_ibsend_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_issend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_issend val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_issend));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_issend(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_issend_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_irsend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_irsend val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_irsend));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_irsend(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_irsend_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_irecv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_irecv val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_irecv));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_irecv(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_irecv_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_wait(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_wait val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_wait));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_wait(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_wait_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_test(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_test val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_test));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_test(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_test_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_request_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_request_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_request_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_request_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_request_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_waitany(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_waitany val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_waitany));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_waitany(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_waitany_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPI_FREE_REQUEST(val.requests);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_testany(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_testany val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_testany));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_testany(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_testany_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPI_FREE_REQUEST(val.requests);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_waitall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_waitall val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_waitall));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_waitall(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_waitall_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPI_FREE_REQUEST(val.requests);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_testall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_testall val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_testall));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_testall(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_testall_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPI_FREE_REQUEST(val.requests);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_waitsome(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_waitsome val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_waitsome));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_waitsome(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_waitsome_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPI_FREE_REQUEST(val.requests);
  if(val.indices != NULL) DUMPI_FREE_INT(val.indices);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_testsome(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_testsome val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_testsome));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_testsome(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_testsome_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPI_FREE_REQUEST(val.requests);
  if(val.indices != NULL) DUMPI_FREE_INT(val.indices);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_iprobe(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_iprobe val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_iprobe));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_iprobe(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_iprobe_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_probe(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_probe val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_probe));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_probe(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_probe_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cancel(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cancel val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cancel));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cancel(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cancel_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_test_cancelled(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_test_cancelled val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_test_cancelled));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_test_cancelled(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_test_cancelled_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_send_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_send_init val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_send_init));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_send_init(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_send_init_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_bsend_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_bsend_init val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_bsend_init));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_bsend_init(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_bsend_init_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_ssend_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_ssend_init val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_ssend_init));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_ssend_init(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_ssend_init_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_rsend_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_rsend_init val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_rsend_init));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_rsend_init(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_rsend_init_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_recv_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_recv_init val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_recv_init));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_recv_init(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_recv_init_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_start(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_start val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_start));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_start(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_start_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_startall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_startall val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_startall));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_startall(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_startall_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_sendrecv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_sendrecv val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_sendrecv));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_sendrecv(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_sendrecv_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_sendrecv_replace(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_sendrecv_replace val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_sendrecv_replace));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_sendrecv_replace(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_sendrecv_replace_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_contiguous(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_contiguous val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_contiguous));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_contiguous(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_contiguous_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_vector(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_vector val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_vector));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_vector(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_vector_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_hvector(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_hvector val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_hvector));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_hvector(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_hvector_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_indexed(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_indexed val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_indexed));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_indexed(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_indexed_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.lengths != NULL) DUMPI_FREE_INT(val.lengths);
  if(val.indices != NULL) DUMPI_FREE_INT(val.indices);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_hindexed(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_hindexed val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_hindexed));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_hindexed(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_hindexed_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.lengths != NULL) DUMPI_FREE_INT(val.lengths);
  if(val.indices != NULL) DUMPI_FREE_INT(val.indices);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_struct(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_struct val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_struct));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_struct(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_struct_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.lengths != NULL) DUMPI_FREE_INT(val.lengths);
  if(val.oldtypes != NULL) DUMPI_FREE_DATATYPE(val.oldtypes);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_address(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_address val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_address));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_address(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_address_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_extent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_extent val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_extent));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_extent(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_extent_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_lb(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_lb val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_lb));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_lb(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_lb_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_ub(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_ub val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_ub));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_ub(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_ub_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_commit(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_commit val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_commit));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_commit(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_commit_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_get_elements(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_get_elements val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_get_elements));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_get_elements(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_get_elements_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_pack(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_pack val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_pack));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_pack(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_pack_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_unpack(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_unpack val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_unpack));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_unpack(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_unpack_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_pack_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_pack_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_pack_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_pack_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_pack_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_barrier(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_barrier val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_barrier));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_barrier(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_barrier_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_bcast(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_bcast val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_bcast));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_bcast(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_bcast_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_gather(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_gather val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_gather));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_gather(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_gather_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_gatherv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_gatherv val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_gatherv));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_gatherv(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_gatherv_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.recvcounts != NULL) DUMPI_FREE_INT(val.recvcounts);
  if(val.displs != NULL) DUMPI_FREE_INT(val.displs);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_scatter(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_scatter val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_scatter));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_scatter(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_scatter_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_scatterv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_scatterv val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_scatterv));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_scatterv(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_scatterv_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.sendcounts != NULL) DUMPI_FREE_INT(val.sendcounts);
  if(val.displs != NULL) DUMPI_FREE_INT(val.displs);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_allgather(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_allgather val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_allgather));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_allgather(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_allgather_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_allgatherv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_allgatherv val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_allgatherv));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_allgatherv(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_allgatherv_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.recvcounts != NULL) DUMPI_FREE_INT(val.recvcounts);
  if(val.displs != NULL) DUMPI_FREE_INT(val.displs);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_alltoall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_alltoall val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_alltoall));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_alltoall(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_alltoall_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_alltoallv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_alltoallv val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_alltoallv));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_alltoallv(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_alltoallv_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.sendcounts != NULL) DUMPI_FREE_INT(val.sendcounts);
  if(val.senddispls != NULL) DUMPI_FREE_INT(val.senddispls);
  if(val.recvcounts != NULL) DUMPI_FREE_INT(val.recvcounts);
  if(val.recvdispls != NULL) DUMPI_FREE_INT(val.recvdispls);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_reduce(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_reduce val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_reduce));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_reduce(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_reduce_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_op_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_op_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_op_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_op_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_op_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_op_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_op_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_op_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_op_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_op_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_allreduce(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_allreduce val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_allreduce));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_allreduce(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_allreduce_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_reduce_scatter(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_reduce_scatter val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_reduce_scatter));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_reduce_scatter(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_reduce_scatter_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.recvcounts != NULL) DUMPI_FREE_INT(val.recvcounts);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_scan(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_scan val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_scan));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_scan(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_scan_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_rank(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_rank val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_rank));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_rank(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_rank_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_translate_ranks(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_translate_ranks val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_translate_ranks));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_translate_ranks(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_translate_ranks_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.ranks1 != NULL) DUMPI_FREE_INT(val.ranks1);
  if(val.ranks2 != NULL) DUMPI_FREE_INT(val.ranks2);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_compare(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_compare val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_compare));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_compare(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_compare_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_group(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_group val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_group));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_group(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_group_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_union(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_union val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_union));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_union(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_union_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_intersection(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_intersection val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_intersection));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_intersection(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_intersection_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_difference(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_difference val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_difference));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_difference(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_difference_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_incl(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_incl val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_incl));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_incl(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_incl_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.ranks != NULL) DUMPI_FREE_INT(val.ranks);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_excl(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_excl val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_excl));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_excl(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_excl_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.ranks != NULL) DUMPI_FREE_INT(val.ranks);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_range_incl(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_range_incl val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_range_incl));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_range_incl(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_range_incl_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.ranges != NULL)
    DUMPI_FREE_INT_ARRAY_2(val.count, val.ranges);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_range_excl(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_range_excl val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_range_excl));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_range_excl(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_range_excl_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.ranges != NULL)
    DUMPI_FREE_INT_ARRAY_2(val.count, val.ranges);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_group_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_group_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_group_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_group_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_group_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_rank(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_rank val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_rank));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_rank(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_rank_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_compare(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_compare val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_compare));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_compare(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_compare_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_dup(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_dup val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_dup));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_dup(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_dup_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_split(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_split val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_split));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_split(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_split_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_test_inter(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_test_inter val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_test_inter));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_test_inter(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_test_inter_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_remote_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_remote_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_remote_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_remote_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_remote_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_remote_group(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_remote_group val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_remote_group));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_remote_group(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_remote_group_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_intercomm_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_intercomm_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_intercomm_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_intercomm_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_intercomm_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_intercomm_merge(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_intercomm_merge val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_intercomm_merge));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_intercomm_merge(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_intercomm_merge_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_keyval_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_keyval_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_keyval_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_keyval_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_keyval_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_keyval_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_keyval_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_keyval_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_keyval_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_keyval_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_attr_put(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_attr_put val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_attr_put));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_attr_put(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_attr_put_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_attr_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_attr_get val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_attr_get));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_attr_get(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_attr_get_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_attr_delete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_attr_delete val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_attr_delete));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_attr_delete(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_attr_delete_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_topo_test(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_topo_test val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_topo_test));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_topo_test(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_topo_test_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cart_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cart_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cart_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cart_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cart_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.dims != NULL) DUMPI_FREE_INT(val.dims);
  if(val.periods != NULL) DUMPI_FREE_INT(val.periods);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_dims_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_dims_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_dims_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_dims_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_dims_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.dims.in != NULL) DUMPI_FREE_INT(val.dims.in);
  if(val.dims.out != NULL) DUMPI_FREE_INT(val.dims.out);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_graph_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_graph_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_graph_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_graph_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_graph_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.index != NULL) DUMPI_FREE_INT(val.index);
  if(val.edges != NULL) DUMPI_FREE_INT(val.edges);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_graphdims_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_graphdims_get val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_graphdims_get));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_graphdims_get(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_graphdims_get_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_graph_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_graph_get val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_graph_get));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_graph_get(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_graph_get_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.index != NULL) DUMPI_FREE_INT(val.index);
  if(val.edges != NULL) DUMPI_FREE_INT(val.edges);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cartdim_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cartdim_get val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cartdim_get));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cartdim_get(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cartdim_get_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cart_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cart_get val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cart_get));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cart_get(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cart_get_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.dims != NULL) DUMPI_FREE_INT(val.dims);
  if(val.periods != NULL) DUMPI_FREE_INT(val.periods);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cart_rank(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cart_rank val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cart_rank));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cart_rank(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cart_rank_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.coords != NULL) DUMPI_FREE_INT(val.coords);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cart_coords(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cart_coords val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cart_coords));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cart_coords(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cart_coords_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.coords != NULL) DUMPI_FREE_INT(val.coords);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_graph_neighbors_count(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_graph_neighbors_count val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_graph_neighbors_count));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_graph_neighbors_count(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_graph_neighbors_count_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_graph_neighbors(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_graph_neighbors val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_graph_neighbors));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_graph_neighbors(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_graph_neighbors_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.neighbors != NULL) DUMPI_FREE_INT(val.neighbors);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cart_shift(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cart_shift val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cart_shift));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cart_shift(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cart_shift_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cart_sub(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cart_sub val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cart_sub));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cart_sub(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cart_sub_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.remain_dims != NULL) DUMPI_FREE_INT(val.remain_dims);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_cart_map(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_cart_map val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_cart_map));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_cart_map(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_cart_map_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.dims != NULL) DUMPI_FREE_INT(val.dims);
  if(val.period != NULL) DUMPI_FREE_INT(val.period);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_graph_map(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_graph_map val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_graph_map));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_graph_map(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_graph_map_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.index != NULL) DUMPI_FREE_INT(val.index);
  if(val.edges != NULL) DUMPI_FREE_INT(val.edges);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_get_processor_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_get_processor_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_get_processor_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_get_processor_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_get_processor_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.name != NULL) DUMPI_FREE_CHAR(val.name);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_get_version(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_get_version val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_get_version));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_get_version(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_get_version_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_errhandler_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_errhandler_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_errhandler_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_errhandler_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_errhandler_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_errhandler_set(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_errhandler_set val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_errhandler_set));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_errhandler_set(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_errhandler_set_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_errhandler_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_errhandler_get val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_errhandler_get));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_errhandler_get(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_errhandler_get_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_errhandler_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_errhandler_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_errhandler_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_errhandler_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_errhandler_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_error_string(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_error_string val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_error_string));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_error_string(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_error_string_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.errorstring != NULL) DUMPI_FREE_CHAR(val.errorstring);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_error_class(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_error_class val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_error_class));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_error_class(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_error_class_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_wtime(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_wtime val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_wtime));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_wtime(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_wtime_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_wtick(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_wtick val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_wtick));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_wtick(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_wtick_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_init val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_init));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_init(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_init_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.argv != NULL)
    DUMPI_FREE_CHAR_ARRAY_2(val.argc, val.argv);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_finalize(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_finalize val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_finalize));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_finalize(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_finalize_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_initialized(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_initialized val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_initialized));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_initialized(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_initialized_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_abort(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_abort val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_abort));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_abort(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_abort_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_close_port(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_close_port val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_close_port));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_close_port(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_close_port_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.portname != NULL) DUMPI_FREE_CHAR(val.portname);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_accept(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_accept val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_accept));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_accept(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_accept_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.portname != NULL) DUMPI_FREE_CHAR(val.portname);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_connect(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_connect val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_connect));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_connect(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_connect_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.portname != NULL) DUMPI_FREE_CHAR(val.portname);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_disconnect(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_disconnect val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_disconnect));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_disconnect(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_disconnect_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_get_parent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_get_parent val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_get_parent));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_get_parent(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_get_parent_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_join(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_join val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_join));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_join(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_join_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_spawn(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_spawn val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_spawn));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_spawn(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_spawn_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.command != NULL) DUMPI_FREE_CHAR(val.command);
  if(val.argv != NULL)
    DUMPI_FREE_CHAR_ARRAY_2(DUMPI_NULLTERM, val.argv);
  if(val.errcodes != NULL) DUMPI_FREE_ERRCODE(val.errcodes);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_spawn_multiple(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_spawn_multiple val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_spawn_multiple));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_spawn_multiple(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_spawn_multiple_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.commands != NULL)
    DUMPI_FREE_CHAR_ARRAY_2(val.count, val.commands);
  if(val.argvs != NULL)
    DUMPI_FREE_CHAR_ARRAY_3(val.count, DUMPI_NULLTERM, val.argvs);
  if(val.maxprocs != NULL) DUMPI_FREE_INT(val.maxprocs);
  if(val.info != NULL) DUMPI_FREE_INFO(val.info);
  if(val.errcodes != NULL) DUMPI_FREE_ERRCODE(val.errcodes);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_lookup_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_lookup_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_lookup_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_lookup_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_lookup_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.servicename != NULL) DUMPI_FREE_CHAR(val.servicename);
  if(val.portname != NULL) DUMPI_FREE_CHAR(val.portname);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_open_port(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_open_port val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_open_port));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_open_port(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_open_port_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.portname != NULL) DUMPI_FREE_CHAR(val.portname);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_publish_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_publish_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_publish_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_publish_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_publish_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.servicename != NULL) DUMPI_FREE_CHAR(val.servicename);
  if(val.portname != NULL) DUMPI_FREE_CHAR(val.portname);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_unpublish_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_unpublish_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_unpublish_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_unpublish_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_unpublish_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.servicename != NULL) DUMPI_FREE_CHAR(val.servicename);
  if(val.portname != NULL) DUMPI_FREE_CHAR(val.portname);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_accumulate(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_accumulate val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_accumulate));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_accumulate(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_accumulate_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_get val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_get));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_get(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_get_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_put(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_put val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_put));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_put(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_put_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_complete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_complete val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_complete));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_complete(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_complete_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_fence(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_fence val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_fence));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_fence(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_fence_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_get_group(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_get_group val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_get_group));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_get_group(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_get_group_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_lock(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_lock val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_lock));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_lock(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_lock_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_post(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_post val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_post));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_post(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_post_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_start(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_start val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_start));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_start(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_start_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_test(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_test val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_test));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_test(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_test_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_unlock(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_unlock val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_unlock));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_unlock(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_unlock_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_wait(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_wait val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_wait));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_wait(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_wait_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_alltoallw(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_alltoallw val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_alltoallw));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_alltoallw(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_alltoallw_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.sendcounts != NULL) DUMPI_FREE_INT(val.sendcounts);
  if(val.senddispls != NULL) DUMPI_FREE_INT(val.senddispls);
  if(val.sendtypes != NULL) DUMPI_FREE_DATATYPE(val.sendtypes);
  if(val.recvcounts != NULL) DUMPI_FREE_INT(val.recvcounts);
  if(val.recvdispls != NULL) DUMPI_FREE_INT(val.recvdispls);
  if(val.recvtypes != NULL) DUMPI_FREE_DATATYPE(val.recvtypes);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_exscan(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_exscan val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_exscan));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_exscan(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_exscan_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_add_error_class(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_add_error_class val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_add_error_class));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_add_error_class(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_add_error_class_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_add_error_code(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_add_error_code val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_add_error_code));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_add_error_code(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_add_error_code_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_add_error_string(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_add_error_string val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_add_error_string));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_add_error_string(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_add_error_string_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.errorstring != NULL) DUMPI_FREE_CHAR(val.errorstring);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_call_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_call_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_call_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_call_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_call_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_create_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_create_keyval val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_create_keyval));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_create_keyval(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_create_keyval_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_delete_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_delete_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_delete_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_delete_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_delete_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_free_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_free_keyval val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_free_keyval));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_free_keyval(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_free_keyval_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_get_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_get_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_get_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_get_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_get_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_get_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_get_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_get_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_get_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_get_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.name != NULL) DUMPI_FREE_CHAR(val.name);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_set_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_set_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_set_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_set_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_set_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_set_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_set_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_set_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_set_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_set_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.name != NULL) DUMPI_FREE_CHAR(val.name);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_call_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_call_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_call_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_call_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_call_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_grequest_complete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_grequest_complete val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_grequest_complete));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_grequest_complete(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_grequest_complete_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_grequest_start(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_grequest_start val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_grequest_start));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_grequest_start(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_grequest_start_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_init_thread(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_init_thread val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_init_thread));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_init_thread(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_init_thread_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.argv != NULL)
    DUMPI_FREE_CHAR_ARRAY_2(val.argc, val.argv);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_is_thread_main(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_is_thread_main val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_is_thread_main));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_is_thread_main(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_is_thread_main_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_query_thread(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_query_thread val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_query_thread));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_query_thread(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_query_thread_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_status_set_cancelled(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_status_set_cancelled val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_status_set_cancelled));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_status_set_cancelled(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_status_set_cancelled_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_status_set_elements(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_status_set_elements val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_status_set_elements));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_status_set_elements(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_status_set_elements_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_create_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_create_keyval val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_create_keyval));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_create_keyval(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_create_keyval_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_delete_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_delete_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_delete_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_delete_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_delete_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_dup(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_dup val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_dup));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_dup(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_dup_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_free_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_free_keyval val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_free_keyval));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_free_keyval(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_free_keyval_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_get_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_get_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_get_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_get_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_get_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_get_contents(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_get_contents val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_get_contents));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_get_contents(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_get_contents_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.arrintegers != NULL) DUMPI_FREE_INT(val.arrintegers);
  if(val.arraddresses != NULL) DUMPI_FREE_INT(val.arraddresses);
  if(val.arrdatatypes != NULL) DUMPI_FREE_DATATYPE(val.arrdatatypes);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_get_envelope(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_get_envelope val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_get_envelope));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_get_envelope(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_get_envelope_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_get_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_get_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_get_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_get_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_get_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.name != NULL) DUMPI_FREE_CHAR(val.name);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_set_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_set_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_set_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_set_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_set_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_set_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_set_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_set_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_set_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_set_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.name != NULL) DUMPI_FREE_CHAR(val.name);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_match_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_match_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_match_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_match_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_match_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_call_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_call_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_call_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_call_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_call_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_create_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_create_keyval val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_create_keyval));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_create_keyval(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_create_keyval_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_delete_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_delete_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_delete_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_delete_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_delete_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_free_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_free_keyval val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_free_keyval));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_free_keyval(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_free_keyval_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_get_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_get_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_get_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_get_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_get_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_get_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_get_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_get_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_get_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_get_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.name != NULL) DUMPI_FREE_CHAR(val.name);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_set_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_set_attr val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_set_attr));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_set_attr(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_set_attr_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_set_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_set_name val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_set_name));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_set_name(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_set_name_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.name != NULL) DUMPI_FREE_CHAR(val.name);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_alloc_mem(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_alloc_mem val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_alloc_mem));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_alloc_mem(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_alloc_mem_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_create_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_create_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_create_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_create_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_create_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_get_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_get_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_get_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_get_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_get_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_comm_set_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_comm_set_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_comm_set_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_comm_set_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_comm_set_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_create_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_create_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_create_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_create_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_create_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_set_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_set_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_set_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_set_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_set_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_finalized(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_finalized val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_finalized));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_finalized(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_finalized_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_free_mem(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_free_mem val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_free_mem));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_free_mem(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_free_mem_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_get_address(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_get_address val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_get_address));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_get_address(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_get_address_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_create val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_create));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_create(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_create_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_delete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_delete val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_delete));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_delete(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_delete_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.key != NULL) DUMPI_FREE_CHAR(val.key);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_dup(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_dup val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_dup));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_dup(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_dup_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_free val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_free));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_free(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_free_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_get val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_get));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_get(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_get_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.key != NULL) DUMPI_FREE_CHAR(val.key);
  if(val.value != NULL) DUMPI_FREE_CHAR(val.value);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_get_nkeys(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_get_nkeys val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_get_nkeys));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_get_nkeys(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_get_nkeys_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_get_nthkey(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_get_nthkey val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_get_nthkey));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_get_nthkey(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_get_nthkey_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.key != NULL) DUMPI_FREE_CHAR(val.key);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_get_valuelen(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_get_valuelen val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_get_valuelen));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_get_valuelen(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_get_valuelen_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.key != NULL) DUMPI_FREE_CHAR(val.key);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_info_set(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_info_set val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_info_set));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_info_set(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_info_set_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.key != NULL) DUMPI_FREE_CHAR(val.key);
  if(val.value != NULL) DUMPI_FREE_CHAR(val.value);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_pack_external(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_pack_external val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_pack_external));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_pack_external(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_pack_external_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.datarep != NULL) DUMPI_FREE_CHAR(val.datarep);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_pack_external_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_pack_external_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_pack_external_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_pack_external_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_pack_external_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.datarep != NULL) DUMPI_FREE_CHAR(val.datarep);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_request_get_status(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_request_get_status val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_request_get_status));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_request_get_status(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_request_get_status_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_create_darray(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_create_darray val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_create_darray));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_create_darray(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_create_darray_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.gsizes != NULL) DUMPI_FREE_INT(val.gsizes);
  if(val.distribs != NULL) DUMPI_FREE_DISTRIBUTION(val.distribs);
  if(val.dargs != NULL) DUMPI_FREE_INT(val.dargs);
  if(val.psizes != NULL) DUMPI_FREE_INT(val.psizes);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_create_hindexed(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_create_hindexed val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_create_hindexed));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_create_hindexed(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_create_hindexed_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.blocklengths != NULL) DUMPI_FREE_INT(val.blocklengths);
  if(val.displacements != NULL) DUMPI_FREE_INT(val.displacements);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_create_hvector(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_create_hvector val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_create_hvector));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_create_hvector(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_create_hvector_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_create_indexed_block(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_create_indexed_block val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_create_indexed_block));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_create_indexed_block(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_create_indexed_block_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.displacments != NULL) DUMPI_FREE_INT(val.displacments);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_create_resized(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_create_resized val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_create_resized));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_create_resized(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_create_resized_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_create_struct(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_create_struct val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_create_struct));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_create_struct(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_create_struct_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.blocklengths != NULL) DUMPI_FREE_INT(val.blocklengths);
  if(val.displacements != NULL) DUMPI_FREE_INT(val.displacements);
  if(val.oldtypes != NULL) DUMPI_FREE_DATATYPE(val.oldtypes);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_create_subarray(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_create_subarray val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_create_subarray));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_create_subarray(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_create_subarray_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.sizes != NULL) DUMPI_FREE_INT(val.sizes);
  if(val.subsizes != NULL) DUMPI_FREE_INT(val.subsizes);
  if(val.starts != NULL) DUMPI_FREE_INT(val.starts);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_get_extent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_get_extent val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_get_extent));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_get_extent(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_get_extent_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_type_get_true_extent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_type_get_true_extent val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_type_get_true_extent));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_type_get_true_extent(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_type_get_true_extent_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_unpack_external(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_unpack_external val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_unpack_external));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_unpack_external(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_unpack_external_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.datarep != NULL) DUMPI_FREE_CHAR(val.datarep);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_create_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_create_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_create_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_create_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_create_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_get_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_get_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_get_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_get_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_get_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_win_set_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_win_set_errhandler val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_win_set_errhandler));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_win_set_errhandler(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_win_set_errhandler_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_open(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_open val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_open));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_open(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_open_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.filename != NULL) DUMPI_FREE_CHAR(val.filename);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_close(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_close val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_close));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_close(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_close_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_delete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_delete val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_delete));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_delete(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_delete_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.filename != NULL) DUMPI_FREE_CHAR(val.filename);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_set_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_set_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_set_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_set_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_set_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_preallocate(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_preallocate val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_preallocate));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_preallocate(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_preallocate_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_size val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_size));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_size(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_size_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_group(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_group val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_group));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_group(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_group_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_amode(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_amode val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_amode));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_amode(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_amode_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_set_info(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_set_info val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_set_info));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_set_info(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_set_info_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_info(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_info val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_info));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_info(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_info_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_set_view(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_set_view val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_set_view));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_set_view(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_set_view_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.datarep != NULL) DUMPI_FREE_CHAR(val.datarep);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_view(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_view val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_view));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_view(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_view_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.datarep != NULL) DUMPI_FREE_CHAR(val.datarep);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_at(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_at val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_at));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_at(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_at_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_at_all(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_at_all val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_at_all));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_at_all(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_at_all_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_at(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_at val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_at));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_at(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_at_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_at_all(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_at_all val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_at_all));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_at_all(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_at_all_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_iread_at(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_iread_at val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_iread_at));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_iread_at(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_iread_at_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_iwrite_at(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_iwrite_at val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_iwrite_at));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_iwrite_at(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_iwrite_at_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_all(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_all val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_all));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_all(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_all_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_all(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_all val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_all));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_all(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_all_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_iread(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_iread val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_iread));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_iread(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_iread_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_iwrite(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_iwrite val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_iwrite));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_iwrite(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_iwrite_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_seek(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_seek val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_seek));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_seek(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_seek_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_position(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_position val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_position));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_position(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_position_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_byte_offset(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_byte_offset val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_byte_offset));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_byte_offset(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_byte_offset_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_shared val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_shared));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_shared(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_shared_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_shared val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_shared));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_shared(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_shared_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_iread_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_iread_shared val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_iread_shared));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_iread_shared(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_iread_shared_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_iwrite_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_iwrite_shared val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_iwrite_shared));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_iwrite_shared(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_iwrite_shared_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_ordered(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_ordered val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_ordered));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_ordered(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_ordered_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_ordered(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_ordered val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_ordered));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_ordered(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_ordered_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_seek_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_seek_shared val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_seek_shared));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_seek_shared(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_seek_shared_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_position_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_position_shared val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_position_shared));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_position_shared(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_position_shared_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_at_all_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_at_all_begin val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_at_all_begin));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_at_all_begin(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_at_all_begin_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_at_all_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_at_all_end val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_at_all_end));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_at_all_end(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_at_all_end_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_at_all_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_at_all_begin val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_at_all_begin));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_at_all_begin(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_at_all_begin_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_at_all_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_at_all_end val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_at_all_end));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_at_all_end(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_at_all_end_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_all_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_all_begin val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_all_begin));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_all_begin(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_all_begin_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_all_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_all_end val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_all_end));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_all_end(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_all_end_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_all_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_all_begin val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_all_begin));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_all_begin(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_all_begin_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_all_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_all_end val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_all_end));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_all_end(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_all_end_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_ordered_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_ordered_begin val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_ordered_begin));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_ordered_begin(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_ordered_begin_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_read_ordered_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_read_ordered_end val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_read_ordered_end));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_read_ordered_end(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_read_ordered_end_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_ordered_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_ordered_begin val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_ordered_begin));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_ordered_begin(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_ordered_begin_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_write_ordered_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_write_ordered_end val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_write_ordered_end));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_write_ordered_end(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_write_ordered_end_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_type_extent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_type_extent val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_type_extent));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_type_extent(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_type_extent_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_register_datarep(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_register_datarep val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_register_datarep));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_register_datarep(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_register_datarep_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.name != NULL) DUMPI_FREE_CHAR(val.name);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_set_atomicity(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_set_atomicity val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_set_atomicity));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_set_atomicity(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_set_atomicity_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_get_atomicity(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_get_atomicity val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_get_atomicity));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_get_atomicity(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_get_atomicity_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_file_sync(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_file_sync val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_file_sync));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_file_sync(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_file_sync_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libdumpi_grab_io_test(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpio_test val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpio_test));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpio_read_test(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpio_test_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libdumpi_grab_io_wait(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpio_wait val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpio_wait));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpio_read_wait(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpio_wait_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.status != NULL) DUMPI_FREE_STATUS(val.status);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libdumpi_grab_io_testall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpio_testall val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpio_testall));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpio_read_testall(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpio_testall_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPIO_FREE_REQUEST(val.requests);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libdumpi_grab_io_waitall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpio_waitall val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpio_waitall));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpio_read_waitall(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpio_waitall_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPIO_FREE_REQUEST(val.requests);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libdumpi_grab_io_testany(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpio_testany val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpio_testany));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpio_read_testany(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpio_testany_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPIO_FREE_REQUEST(val.requests);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libdumpi_grab_io_waitany(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpio_waitany val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpio_waitany));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpio_read_waitany(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpio_waitany_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPIO_FREE_REQUEST(val.requests);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libdumpi_grab_io_waitsome(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpio_waitsome val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpio_waitsome));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpio_read_waitsome(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpio_waitsome_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPIO_FREE_REQUEST(val.requests);
  if(val.indices != NULL) DUMPI_FREE_INT(val.indices);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libdumpi_grab_io_testsome(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpio_testsome val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpio_testsome));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpio_read_testsome(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpio_testsome_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  if(val.requests != NULL) DUMPIO_FREE_REQUEST(val.requests);
  if(val.indices != NULL) DUMPI_FREE_INT(val.indices);
  if(val.statuses != NULL) DUMPI_FREE_STATUS(val.statuses);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_all_functions(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  /* This one is a no-op */
  (void)profile;
  (void)callout;
  (void)uarg;
  return 1;
}

int libundumpi_grab_function_enter(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_func_call val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_func_call));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_func_enter(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_function_enter_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

int libundumpi_grab_function_exit(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg) {
  dumpi_func_call val;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  memset(&val, 0, sizeof(dumpi_func_call));
  thread = 0;
  memset(&cpu, 0, sizeof(dumpi_time));
  memset(&wall, 0, sizeof(dumpi_time));
  memset(&perf, 0, sizeof(dumpi_perfinfo));
  dumpi_read_func_exit(&val, &thread, &cpu, &wall, &perf, profile);
  if(callout != NULL)
    ((dumpi_function_exit_call)callout)(&val, thread, &cpu, &wall, &perf, uarg);
  DUMPI_RELEASE_PERFTAGS(perf);
  return 1;
}

void libundumpi_populate_callbacks(const libundumpi_callbacks *callback,
           libundumpi_cbpair *callarr)
{
  libundumpi_populate_callouts(callback,callarr);
  libundumpi_populate_handlers(callback,callarr);
}

void libundumpi_populate_callouts(const libundumpi_callbacks *callback,
           libundumpi_cbpair *callarr)
{
  callarr[DUMPI_Send].callout = (libundumpi_unsafe_fun)callback->on_send;
  callarr[DUMPI_Recv].callout = (libundumpi_unsafe_fun)callback->on_recv;
  callarr[DUMPI_Get_count].callout = (libundumpi_unsafe_fun)callback->on_get_count;
  callarr[DUMPI_Bsend].callout = (libundumpi_unsafe_fun)callback->on_bsend;
  callarr[DUMPI_Ssend].callout = (libundumpi_unsafe_fun)callback->on_ssend;
  callarr[DUMPI_Rsend].callout = (libundumpi_unsafe_fun)callback->on_rsend;
  callarr[DUMPI_Buffer_attach].callout = (libundumpi_unsafe_fun)callback->on_buffer_attach;
  callarr[DUMPI_Buffer_detach].callout = (libundumpi_unsafe_fun)callback->on_buffer_detach;
  callarr[DUMPI_Isend].callout = (libundumpi_unsafe_fun)callback->on_isend;
  callarr[DUMPI_Ibsend].callout = (libundumpi_unsafe_fun)callback->on_ibsend;
  callarr[DUMPI_Issend].callout = (libundumpi_unsafe_fun)callback->on_issend;
  callarr[DUMPI_Irsend].callout = (libundumpi_unsafe_fun)callback->on_irsend;
  callarr[DUMPI_Irecv].callout = (libundumpi_unsafe_fun)callback->on_irecv;
  callarr[DUMPI_Wait].callout = (libundumpi_unsafe_fun)callback->on_wait;
  callarr[DUMPI_Test].callout = (libundumpi_unsafe_fun)callback->on_test;
  callarr[DUMPI_Request_free].callout = (libundumpi_unsafe_fun)callback->on_request_free;
  callarr[DUMPI_Waitany].callout = (libundumpi_unsafe_fun)callback->on_waitany;
  callarr[DUMPI_Testany].callout = (libundumpi_unsafe_fun)callback->on_testany;
  callarr[DUMPI_Waitall].callout = (libundumpi_unsafe_fun)callback->on_waitall;
  callarr[DUMPI_Testall].callout = (libundumpi_unsafe_fun)callback->on_testall;
  callarr[DUMPI_Waitsome].callout = (libundumpi_unsafe_fun)callback->on_waitsome;
  callarr[DUMPI_Testsome].callout = (libundumpi_unsafe_fun)callback->on_testsome;
  callarr[DUMPI_Iprobe].callout = (libundumpi_unsafe_fun)callback->on_iprobe;
  callarr[DUMPI_Probe].callout = (libundumpi_unsafe_fun)callback->on_probe;
  callarr[DUMPI_Cancel].callout = (libundumpi_unsafe_fun)callback->on_cancel;
  callarr[DUMPI_Test_cancelled].callout = (libundumpi_unsafe_fun)callback->on_test_cancelled;
  callarr[DUMPI_Send_init].callout = (libundumpi_unsafe_fun)callback->on_send_init;
  callarr[DUMPI_Bsend_init].callout = (libundumpi_unsafe_fun)callback->on_bsend_init;
  callarr[DUMPI_Ssend_init].callout = (libundumpi_unsafe_fun)callback->on_ssend_init;
  callarr[DUMPI_Rsend_init].callout = (libundumpi_unsafe_fun)callback->on_rsend_init;
  callarr[DUMPI_Recv_init].callout = (libundumpi_unsafe_fun)callback->on_recv_init;
  callarr[DUMPI_Start].callout = (libundumpi_unsafe_fun)callback->on_start;
  callarr[DUMPI_Startall].callout = (libundumpi_unsafe_fun)callback->on_startall;
  callarr[DUMPI_Sendrecv].callout = (libundumpi_unsafe_fun)callback->on_sendrecv;
  callarr[DUMPI_Sendrecv_replace].callout = (libundumpi_unsafe_fun)callback->on_sendrecv_replace;
  callarr[DUMPI_Type_contiguous].callout = (libundumpi_unsafe_fun)callback->on_type_contiguous;
  callarr[DUMPI_Type_vector].callout = (libundumpi_unsafe_fun)callback->on_type_vector;
  callarr[DUMPI_Type_hvector].callout = (libundumpi_unsafe_fun)callback->on_type_hvector;
  callarr[DUMPI_Type_indexed].callout = (libundumpi_unsafe_fun)callback->on_type_indexed;
  callarr[DUMPI_Type_hindexed].callout = (libundumpi_unsafe_fun)callback->on_type_hindexed;
  callarr[DUMPI_Type_struct].callout = (libundumpi_unsafe_fun)callback->on_type_struct;
  callarr[DUMPI_Address].callout = (libundumpi_unsafe_fun)callback->on_address;
  callarr[DUMPI_Type_extent].callout = (libundumpi_unsafe_fun)callback->on_type_extent;
  callarr[DUMPI_Type_size].callout = (libundumpi_unsafe_fun)callback->on_type_size;
  callarr[DUMPI_Type_lb].callout = (libundumpi_unsafe_fun)callback->on_type_lb;
  callarr[DUMPI_Type_ub].callout = (libundumpi_unsafe_fun)callback->on_type_ub;
  callarr[DUMPI_Type_commit].callout = (libundumpi_unsafe_fun)callback->on_type_commit;
  callarr[DUMPI_Type_free].callout = (libundumpi_unsafe_fun)callback->on_type_free;
  callarr[DUMPI_Get_elements].callout = (libundumpi_unsafe_fun)callback->on_get_elements;
  callarr[DUMPI_Pack].callout = (libundumpi_unsafe_fun)callback->on_pack;
  callarr[DUMPI_Unpack].callout = (libundumpi_unsafe_fun)callback->on_unpack;
  callarr[DUMPI_Pack_size].callout = (libundumpi_unsafe_fun)callback->on_pack_size;
  callarr[DUMPI_Barrier].callout = (libundumpi_unsafe_fun)callback->on_barrier;
  callarr[DUMPI_Bcast].callout = (libundumpi_unsafe_fun)callback->on_bcast;
  callarr[DUMPI_Gather].callout = (libundumpi_unsafe_fun)callback->on_gather;
  callarr[DUMPI_Gatherv].callout = (libundumpi_unsafe_fun)callback->on_gatherv;
  callarr[DUMPI_Scatter].callout = (libundumpi_unsafe_fun)callback->on_scatter;
  callarr[DUMPI_Scatterv].callout = (libundumpi_unsafe_fun)callback->on_scatterv;
  callarr[DUMPI_Allgather].callout = (libundumpi_unsafe_fun)callback->on_allgather;
  callarr[DUMPI_Allgatherv].callout = (libundumpi_unsafe_fun)callback->on_allgatherv;
  callarr[DUMPI_Alltoall].callout = (libundumpi_unsafe_fun)callback->on_alltoall;
  callarr[DUMPI_Alltoallv].callout = (libundumpi_unsafe_fun)callback->on_alltoallv;
  callarr[DUMPI_Reduce].callout = (libundumpi_unsafe_fun)callback->on_reduce;
  callarr[DUMPI_Op_create].callout = (libundumpi_unsafe_fun)callback->on_op_create;
  callarr[DUMPI_Op_free].callout = (libundumpi_unsafe_fun)callback->on_op_free;
  callarr[DUMPI_Allreduce].callout = (libundumpi_unsafe_fun)callback->on_allreduce;
  callarr[DUMPI_Reduce_scatter].callout = (libundumpi_unsafe_fun)callback->on_reduce_scatter;
  callarr[DUMPI_Scan].callout = (libundumpi_unsafe_fun)callback->on_scan;
  callarr[DUMPI_Group_size].callout = (libundumpi_unsafe_fun)callback->on_group_size;
  callarr[DUMPI_Group_rank].callout = (libundumpi_unsafe_fun)callback->on_group_rank;
  callarr[DUMPI_Group_translate_ranks].callout = (libundumpi_unsafe_fun)callback->on_group_translate_ranks;
  callarr[DUMPI_Group_compare].callout = (libundumpi_unsafe_fun)callback->on_group_compare;
  callarr[DUMPI_Comm_group].callout = (libundumpi_unsafe_fun)callback->on_comm_group;
  callarr[DUMPI_Group_union].callout = (libundumpi_unsafe_fun)callback->on_group_union;
  callarr[DUMPI_Group_intersection].callout = (libundumpi_unsafe_fun)callback->on_group_intersection;
  callarr[DUMPI_Group_difference].callout = (libundumpi_unsafe_fun)callback->on_group_difference;
  callarr[DUMPI_Group_incl].callout = (libundumpi_unsafe_fun)callback->on_group_incl;
  callarr[DUMPI_Group_excl].callout = (libundumpi_unsafe_fun)callback->on_group_excl;
  callarr[DUMPI_Group_range_incl].callout = (libundumpi_unsafe_fun)callback->on_group_range_incl;
  callarr[DUMPI_Group_range_excl].callout = (libundumpi_unsafe_fun)callback->on_group_range_excl;
  callarr[DUMPI_Group_free].callout = (libundumpi_unsafe_fun)callback->on_group_free;
  callarr[DUMPI_Comm_size].callout = (libundumpi_unsafe_fun)callback->on_comm_size;
  callarr[DUMPI_Comm_rank].callout = (libundumpi_unsafe_fun)callback->on_comm_rank;
  callarr[DUMPI_Comm_compare].callout = (libundumpi_unsafe_fun)callback->on_comm_compare;
  callarr[DUMPI_Comm_dup].callout = (libundumpi_unsafe_fun)callback->on_comm_dup;
  callarr[DUMPI_Comm_create].callout = (libundumpi_unsafe_fun)callback->on_comm_create;
  callarr[DUMPI_Comm_split].callout = (libundumpi_unsafe_fun)callback->on_comm_split;
  callarr[DUMPI_Comm_free].callout = (libundumpi_unsafe_fun)callback->on_comm_free;
  callarr[DUMPI_Comm_test_inter].callout = (libundumpi_unsafe_fun)callback->on_comm_test_inter;
  callarr[DUMPI_Comm_remote_size].callout = (libundumpi_unsafe_fun)callback->on_comm_remote_size;
  callarr[DUMPI_Comm_remote_group].callout = (libundumpi_unsafe_fun)callback->on_comm_remote_group;
  callarr[DUMPI_Intercomm_create].callout = (libundumpi_unsafe_fun)callback->on_intercomm_create;
  callarr[DUMPI_Intercomm_merge].callout = (libundumpi_unsafe_fun)callback->on_intercomm_merge;
  callarr[DUMPI_Keyval_create].callout = (libundumpi_unsafe_fun)callback->on_keyval_create;
  callarr[DUMPI_Keyval_free].callout = (libundumpi_unsafe_fun)callback->on_keyval_free;
  callarr[DUMPI_Attr_put].callout = (libundumpi_unsafe_fun)callback->on_attr_put;
  callarr[DUMPI_Attr_get].callout = (libundumpi_unsafe_fun)callback->on_attr_get;
  callarr[DUMPI_Attr_delete].callout = (libundumpi_unsafe_fun)callback->on_attr_delete;
  callarr[DUMPI_Topo_test].callout = (libundumpi_unsafe_fun)callback->on_topo_test;
  callarr[DUMPI_Cart_create].callout = (libundumpi_unsafe_fun)callback->on_cart_create;
  callarr[DUMPI_Dims_create].callout = (libundumpi_unsafe_fun)callback->on_dims_create;
  callarr[DUMPI_Graph_create].callout = (libundumpi_unsafe_fun)callback->on_graph_create;
  callarr[DUMPI_Graphdims_get].callout = (libundumpi_unsafe_fun)callback->on_graphdims_get;
  callarr[DUMPI_Graph_get].callout = (libundumpi_unsafe_fun)callback->on_graph_get;
  callarr[DUMPI_Cartdim_get].callout = (libundumpi_unsafe_fun)callback->on_cartdim_get;
  callarr[DUMPI_Cart_get].callout = (libundumpi_unsafe_fun)callback->on_cart_get;
  callarr[DUMPI_Cart_rank].callout = (libundumpi_unsafe_fun)callback->on_cart_rank;
  callarr[DUMPI_Cart_coords].callout = (libundumpi_unsafe_fun)callback->on_cart_coords;
  callarr[DUMPI_Graph_neighbors_count].callout = (libundumpi_unsafe_fun)callback->on_graph_neighbors_count;
  callarr[DUMPI_Graph_neighbors].callout = (libundumpi_unsafe_fun)callback->on_graph_neighbors;
  callarr[DUMPI_Cart_shift].callout = (libundumpi_unsafe_fun)callback->on_cart_shift;
  callarr[DUMPI_Cart_sub].callout = (libundumpi_unsafe_fun)callback->on_cart_sub;
  callarr[DUMPI_Cart_map].callout = (libundumpi_unsafe_fun)callback->on_cart_map;
  callarr[DUMPI_Graph_map].callout = (libundumpi_unsafe_fun)callback->on_graph_map;
  callarr[DUMPI_Get_processor_name].callout = (libundumpi_unsafe_fun)callback->on_get_processor_name;
  callarr[DUMPI_Get_version].callout = (libundumpi_unsafe_fun)callback->on_get_version;
  callarr[DUMPI_Errhandler_create].callout = (libundumpi_unsafe_fun)callback->on_errhandler_create;
  callarr[DUMPI_Errhandler_set].callout = (libundumpi_unsafe_fun)callback->on_errhandler_set;
  callarr[DUMPI_Errhandler_get].callout = (libundumpi_unsafe_fun)callback->on_errhandler_get;
  callarr[DUMPI_Errhandler_free].callout = (libundumpi_unsafe_fun)callback->on_errhandler_free;
  callarr[DUMPI_Error_string].callout = (libundumpi_unsafe_fun)callback->on_error_string;
  callarr[DUMPI_Error_class].callout = (libundumpi_unsafe_fun)callback->on_error_class;
  callarr[DUMPI_Wtime].callout = (libundumpi_unsafe_fun)callback->on_wtime;
  callarr[DUMPI_Wtick].callout = (libundumpi_unsafe_fun)callback->on_wtick;
  callarr[DUMPI_Init].callout = (libundumpi_unsafe_fun)callback->on_init;
  callarr[DUMPI_Finalize].callout = (libundumpi_unsafe_fun)callback->on_finalize;
  callarr[DUMPI_Initialized].callout = (libundumpi_unsafe_fun)callback->on_initialized;
  callarr[DUMPI_Abort].callout = (libundumpi_unsafe_fun)callback->on_abort;
  callarr[DUMPI_Close_port].callout = (libundumpi_unsafe_fun)callback->on_close_port;
  callarr[DUMPI_Comm_accept].callout = (libundumpi_unsafe_fun)callback->on_comm_accept;
  callarr[DUMPI_Comm_connect].callout = (libundumpi_unsafe_fun)callback->on_comm_connect;
  callarr[DUMPI_Comm_disconnect].callout = (libundumpi_unsafe_fun)callback->on_comm_disconnect;
  callarr[DUMPI_Comm_get_parent].callout = (libundumpi_unsafe_fun)callback->on_comm_get_parent;
  callarr[DUMPI_Comm_join].callout = (libundumpi_unsafe_fun)callback->on_comm_join;
  callarr[DUMPI_Comm_spawn].callout = (libundumpi_unsafe_fun)callback->on_comm_spawn;
  callarr[DUMPI_Comm_spawn_multiple].callout = (libundumpi_unsafe_fun)callback->on_comm_spawn_multiple;
  callarr[DUMPI_Lookup_name].callout = (libundumpi_unsafe_fun)callback->on_lookup_name;
  callarr[DUMPI_Open_port].callout = (libundumpi_unsafe_fun)callback->on_open_port;
  callarr[DUMPI_Publish_name].callout = (libundumpi_unsafe_fun)callback->on_publish_name;
  callarr[DUMPI_Unpublish_name].callout = (libundumpi_unsafe_fun)callback->on_unpublish_name;
  callarr[DUMPI_Accumulate].callout = (libundumpi_unsafe_fun)callback->on_accumulate;
  callarr[DUMPI_Get].callout = (libundumpi_unsafe_fun)callback->on_get;
  callarr[DUMPI_Put].callout = (libundumpi_unsafe_fun)callback->on_put;
  callarr[DUMPI_Win_complete].callout = (libundumpi_unsafe_fun)callback->on_win_complete;
  callarr[DUMPI_Win_create].callout = (libundumpi_unsafe_fun)callback->on_win_create;
  callarr[DUMPI_Win_fence].callout = (libundumpi_unsafe_fun)callback->on_win_fence;
  callarr[DUMPI_Win_free].callout = (libundumpi_unsafe_fun)callback->on_win_free;
  callarr[DUMPI_Win_get_group].callout = (libundumpi_unsafe_fun)callback->on_win_get_group;
  callarr[DUMPI_Win_lock].callout = (libundumpi_unsafe_fun)callback->on_win_lock;
  callarr[DUMPI_Win_post].callout = (libundumpi_unsafe_fun)callback->on_win_post;
  callarr[DUMPI_Win_start].callout = (libundumpi_unsafe_fun)callback->on_win_start;
  callarr[DUMPI_Win_test].callout = (libundumpi_unsafe_fun)callback->on_win_test;
  callarr[DUMPI_Win_unlock].callout = (libundumpi_unsafe_fun)callback->on_win_unlock;
  callarr[DUMPI_Win_wait].callout = (libundumpi_unsafe_fun)callback->on_win_wait;
  callarr[DUMPI_Alltoallw].callout = (libundumpi_unsafe_fun)callback->on_alltoallw;
  callarr[DUMPI_Exscan].callout = (libundumpi_unsafe_fun)callback->on_exscan;
  callarr[DUMPI_Add_error_class].callout = (libundumpi_unsafe_fun)callback->on_add_error_class;
  callarr[DUMPI_Add_error_code].callout = (libundumpi_unsafe_fun)callback->on_add_error_code;
  callarr[DUMPI_Add_error_string].callout = (libundumpi_unsafe_fun)callback->on_add_error_string;
  callarr[DUMPI_Comm_call_errhandler].callout = (libundumpi_unsafe_fun)callback->on_comm_call_errhandler;
  callarr[DUMPI_Comm_create_keyval].callout = (libundumpi_unsafe_fun)callback->on_comm_create_keyval;
  callarr[DUMPI_Comm_delete_attr].callout = (libundumpi_unsafe_fun)callback->on_comm_delete_attr;
  callarr[DUMPI_Comm_free_keyval].callout = (libundumpi_unsafe_fun)callback->on_comm_free_keyval;
  callarr[DUMPI_Comm_get_attr].callout = (libundumpi_unsafe_fun)callback->on_comm_get_attr;
  callarr[DUMPI_Comm_get_name].callout = (libundumpi_unsafe_fun)callback->on_comm_get_name;
  callarr[DUMPI_Comm_set_attr].callout = (libundumpi_unsafe_fun)callback->on_comm_set_attr;
  callarr[DUMPI_Comm_set_name].callout = (libundumpi_unsafe_fun)callback->on_comm_set_name;
  callarr[DUMPI_File_call_errhandler].callout = (libundumpi_unsafe_fun)callback->on_file_call_errhandler;
  callarr[DUMPI_Grequest_complete].callout = (libundumpi_unsafe_fun)callback->on_grequest_complete;
  callarr[DUMPI_Grequest_start].callout = (libundumpi_unsafe_fun)callback->on_grequest_start;
  callarr[DUMPI_Init_thread].callout = (libundumpi_unsafe_fun)callback->on_init_thread;
  callarr[DUMPI_Is_thread_main].callout = (libundumpi_unsafe_fun)callback->on_is_thread_main;
  callarr[DUMPI_Query_thread].callout = (libundumpi_unsafe_fun)callback->on_query_thread;
  callarr[DUMPI_Status_set_cancelled].callout = (libundumpi_unsafe_fun)callback->on_status_set_cancelled;
  callarr[DUMPI_Status_set_elements].callout = (libundumpi_unsafe_fun)callback->on_status_set_elements;
  callarr[DUMPI_Type_create_keyval].callout = (libundumpi_unsafe_fun)callback->on_type_create_keyval;
  callarr[DUMPI_Type_delete_attr].callout = (libundumpi_unsafe_fun)callback->on_type_delete_attr;
  callarr[DUMPI_Type_dup].callout = (libundumpi_unsafe_fun)callback->on_type_dup;
  callarr[DUMPI_Type_free_keyval].callout = (libundumpi_unsafe_fun)callback->on_type_free_keyval;
  callarr[DUMPI_Type_get_attr].callout = (libundumpi_unsafe_fun)callback->on_type_get_attr;
  callarr[DUMPI_Type_get_contents].callout = (libundumpi_unsafe_fun)callback->on_type_get_contents;
  callarr[DUMPI_Type_get_envelope].callout = (libundumpi_unsafe_fun)callback->on_type_get_envelope;
  callarr[DUMPI_Type_get_name].callout = (libundumpi_unsafe_fun)callback->on_type_get_name;
  callarr[DUMPI_Type_set_attr].callout = (libundumpi_unsafe_fun)callback->on_type_set_attr;
  callarr[DUMPI_Type_set_name].callout = (libundumpi_unsafe_fun)callback->on_type_set_name;
  callarr[DUMPI_Type_match_size].callout = (libundumpi_unsafe_fun)callback->on_type_match_size;
  callarr[DUMPI_Win_call_errhandler].callout = (libundumpi_unsafe_fun)callback->on_win_call_errhandler;
  callarr[DUMPI_Win_create_keyval].callout = (libundumpi_unsafe_fun)callback->on_win_create_keyval;
  callarr[DUMPI_Win_delete_attr].callout = (libundumpi_unsafe_fun)callback->on_win_delete_attr;
  callarr[DUMPI_Win_free_keyval].callout = (libundumpi_unsafe_fun)callback->on_win_free_keyval;
  callarr[DUMPI_Win_get_attr].callout = (libundumpi_unsafe_fun)callback->on_win_get_attr;
  callarr[DUMPI_Win_get_name].callout = (libundumpi_unsafe_fun)callback->on_win_get_name;
  callarr[DUMPI_Win_set_attr].callout = (libundumpi_unsafe_fun)callback->on_win_set_attr;
  callarr[DUMPI_Win_set_name].callout = (libundumpi_unsafe_fun)callback->on_win_set_name;
  callarr[DUMPI_Alloc_mem].callout = (libundumpi_unsafe_fun)callback->on_alloc_mem;
  callarr[DUMPI_Comm_create_errhandler].callout = (libundumpi_unsafe_fun)callback->on_comm_create_errhandler;
  callarr[DUMPI_Comm_get_errhandler].callout = (libundumpi_unsafe_fun)callback->on_comm_get_errhandler;
  callarr[DUMPI_Comm_set_errhandler].callout = (libundumpi_unsafe_fun)callback->on_comm_set_errhandler;
  callarr[DUMPI_File_create_errhandler].callout = (libundumpi_unsafe_fun)callback->on_file_create_errhandler;
  callarr[DUMPI_File_get_errhandler].callout = (libundumpi_unsafe_fun)callback->on_file_get_errhandler;
  callarr[DUMPI_File_set_errhandler].callout = (libundumpi_unsafe_fun)callback->on_file_set_errhandler;
  callarr[DUMPI_Finalized].callout = (libundumpi_unsafe_fun)callback->on_finalized;
  callarr[DUMPI_Free_mem].callout = (libundumpi_unsafe_fun)callback->on_free_mem;
  callarr[DUMPI_Get_address].callout = (libundumpi_unsafe_fun)callback->on_get_address;
  callarr[DUMPI_Info_create].callout = (libundumpi_unsafe_fun)callback->on_info_create;
  callarr[DUMPI_Info_delete].callout = (libundumpi_unsafe_fun)callback->on_info_delete;
  callarr[DUMPI_Info_dup].callout = (libundumpi_unsafe_fun)callback->on_info_dup;
  callarr[DUMPI_Info_free].callout = (libundumpi_unsafe_fun)callback->on_info_free;
  callarr[DUMPI_Info_get].callout = (libundumpi_unsafe_fun)callback->on_info_get;
  callarr[DUMPI_Info_get_nkeys].callout = (libundumpi_unsafe_fun)callback->on_info_get_nkeys;
  callarr[DUMPI_Info_get_nthkey].callout = (libundumpi_unsafe_fun)callback->on_info_get_nthkey;
  callarr[DUMPI_Info_get_valuelen].callout = (libundumpi_unsafe_fun)callback->on_info_get_valuelen;
  callarr[DUMPI_Info_set].callout = (libundumpi_unsafe_fun)callback->on_info_set;
  callarr[DUMPI_Pack_external].callout = (libundumpi_unsafe_fun)callback->on_pack_external;
  callarr[DUMPI_Pack_external_size].callout = (libundumpi_unsafe_fun)callback->on_pack_external_size;
  callarr[DUMPI_Request_get_status].callout = (libundumpi_unsafe_fun)callback->on_request_get_status;
  callarr[DUMPI_Type_create_darray].callout = (libundumpi_unsafe_fun)callback->on_type_create_darray;
  callarr[DUMPI_Type_create_hindexed].callout = (libundumpi_unsafe_fun)callback->on_type_create_hindexed;
  callarr[DUMPI_Type_create_hvector].callout = (libundumpi_unsafe_fun)callback->on_type_create_hvector;
  callarr[DUMPI_Type_create_indexed_block].callout = (libundumpi_unsafe_fun)callback->on_type_create_indexed_block;
  callarr[DUMPI_Type_create_resized].callout = (libundumpi_unsafe_fun)callback->on_type_create_resized;
  callarr[DUMPI_Type_create_struct].callout = (libundumpi_unsafe_fun)callback->on_type_create_struct;
  callarr[DUMPI_Type_create_subarray].callout = (libundumpi_unsafe_fun)callback->on_type_create_subarray;
  callarr[DUMPI_Type_get_extent].callout = (libundumpi_unsafe_fun)callback->on_type_get_extent;
  callarr[DUMPI_Type_get_true_extent].callout = (libundumpi_unsafe_fun)callback->on_type_get_true_extent;
  callarr[DUMPI_Unpack_external].callout = (libundumpi_unsafe_fun)callback->on_unpack_external;
  callarr[DUMPI_Win_create_errhandler].callout = (libundumpi_unsafe_fun)callback->on_win_create_errhandler;
  callarr[DUMPI_Win_get_errhandler].callout = (libundumpi_unsafe_fun)callback->on_win_get_errhandler;
  callarr[DUMPI_Win_set_errhandler].callout = (libundumpi_unsafe_fun)callback->on_win_set_errhandler;
  callarr[DUMPI_File_open].callout = (libundumpi_unsafe_fun)callback->on_file_open;
  callarr[DUMPI_File_close].callout = (libundumpi_unsafe_fun)callback->on_file_close;
  callarr[DUMPI_File_delete].callout = (libundumpi_unsafe_fun)callback->on_file_delete;
  callarr[DUMPI_File_set_size].callout = (libundumpi_unsafe_fun)callback->on_file_set_size;
  callarr[DUMPI_File_preallocate].callout = (libundumpi_unsafe_fun)callback->on_file_preallocate;
  callarr[DUMPI_File_get_size].callout = (libundumpi_unsafe_fun)callback->on_file_get_size;
  callarr[DUMPI_File_get_group].callout = (libundumpi_unsafe_fun)callback->on_file_get_group;
  callarr[DUMPI_File_get_amode].callout = (libundumpi_unsafe_fun)callback->on_file_get_amode;
  callarr[DUMPI_File_set_info].callout = (libundumpi_unsafe_fun)callback->on_file_set_info;
  callarr[DUMPI_File_get_info].callout = (libundumpi_unsafe_fun)callback->on_file_get_info;
  callarr[DUMPI_File_set_view].callout = (libundumpi_unsafe_fun)callback->on_file_set_view;
  callarr[DUMPI_File_get_view].callout = (libundumpi_unsafe_fun)callback->on_file_get_view;
  callarr[DUMPI_File_read_at].callout = (libundumpi_unsafe_fun)callback->on_file_read_at;
  callarr[DUMPI_File_read_at_all].callout = (libundumpi_unsafe_fun)callback->on_file_read_at_all;
  callarr[DUMPI_File_write_at].callout = (libundumpi_unsafe_fun)callback->on_file_write_at;
  callarr[DUMPI_File_write_at_all].callout = (libundumpi_unsafe_fun)callback->on_file_write_at_all;
  callarr[DUMPI_File_iread_at].callout = (libundumpi_unsafe_fun)callback->on_file_iread_at;
  callarr[DUMPI_File_iwrite_at].callout = (libundumpi_unsafe_fun)callback->on_file_iwrite_at;
  callarr[DUMPI_File_read].callout = (libundumpi_unsafe_fun)callback->on_file_read;
  callarr[DUMPI_File_read_all].callout = (libundumpi_unsafe_fun)callback->on_file_read_all;
  callarr[DUMPI_File_write].callout = (libundumpi_unsafe_fun)callback->on_file_write;
  callarr[DUMPI_File_write_all].callout = (libundumpi_unsafe_fun)callback->on_file_write_all;
  callarr[DUMPI_File_iread].callout = (libundumpi_unsafe_fun)callback->on_file_iread;
  callarr[DUMPI_File_iwrite].callout = (libundumpi_unsafe_fun)callback->on_file_iwrite;
  callarr[DUMPI_File_seek].callout = (libundumpi_unsafe_fun)callback->on_file_seek;
  callarr[DUMPI_File_get_position].callout = (libundumpi_unsafe_fun)callback->on_file_get_position;
  callarr[DUMPI_File_get_byte_offset].callout = (libundumpi_unsafe_fun)callback->on_file_get_byte_offset;
  callarr[DUMPI_File_read_shared].callout = (libundumpi_unsafe_fun)callback->on_file_read_shared;
  callarr[DUMPI_File_write_shared].callout = (libundumpi_unsafe_fun)callback->on_file_write_shared;
  callarr[DUMPI_File_iread_shared].callout = (libundumpi_unsafe_fun)callback->on_file_iread_shared;
  callarr[DUMPI_File_iwrite_shared].callout = (libundumpi_unsafe_fun)callback->on_file_iwrite_shared;
  callarr[DUMPI_File_read_ordered].callout = (libundumpi_unsafe_fun)callback->on_file_read_ordered;
  callarr[DUMPI_File_write_ordered].callout = (libundumpi_unsafe_fun)callback->on_file_write_ordered;
  callarr[DUMPI_File_seek_shared].callout = (libundumpi_unsafe_fun)callback->on_file_seek_shared;
  callarr[DUMPI_File_get_position_shared].callout = (libundumpi_unsafe_fun)callback->on_file_get_position_shared;
  callarr[DUMPI_File_read_at_all_begin].callout = (libundumpi_unsafe_fun)callback->on_file_read_at_all_begin;
  callarr[DUMPI_File_read_at_all_end].callout = (libundumpi_unsafe_fun)callback->on_file_read_at_all_end;
  callarr[DUMPI_File_write_at_all_begin].callout = (libundumpi_unsafe_fun)callback->on_file_write_at_all_begin;
  callarr[DUMPI_File_write_at_all_end].callout = (libundumpi_unsafe_fun)callback->on_file_write_at_all_end;
  callarr[DUMPI_File_read_all_begin].callout = (libundumpi_unsafe_fun)callback->on_file_read_all_begin;
  callarr[DUMPI_File_read_all_end].callout = (libundumpi_unsafe_fun)callback->on_file_read_all_end;
  callarr[DUMPI_File_write_all_begin].callout = (libundumpi_unsafe_fun)callback->on_file_write_all_begin;
  callarr[DUMPI_File_write_all_end].callout = (libundumpi_unsafe_fun)callback->on_file_write_all_end;
  callarr[DUMPI_File_read_ordered_begin].callout = (libundumpi_unsafe_fun)callback->on_file_read_ordered_begin;
  callarr[DUMPI_File_read_ordered_end].callout = (libundumpi_unsafe_fun)callback->on_file_read_ordered_end;
  callarr[DUMPI_File_write_ordered_begin].callout = (libundumpi_unsafe_fun)callback->on_file_write_ordered_begin;
  callarr[DUMPI_File_write_ordered_end].callout = (libundumpi_unsafe_fun)callback->on_file_write_ordered_end;
  callarr[DUMPI_File_get_type_extent].callout = (libundumpi_unsafe_fun)callback->on_file_get_type_extent;
  callarr[DUMPI_Register_datarep].callout = (libundumpi_unsafe_fun)callback->on_register_datarep;
  callarr[DUMPI_File_set_atomicity].callout = (libundumpi_unsafe_fun)callback->on_file_set_atomicity;
  callarr[DUMPI_File_get_atomicity].callout = (libundumpi_unsafe_fun)callback->on_file_get_atomicity;
  callarr[DUMPI_File_sync].callout = (libundumpi_unsafe_fun)callback->on_file_sync;
  callarr[DUMPIO_Test].callout = (libundumpi_unsafe_fun)callback->on_iotest;
  callarr[DUMPIO_Wait].callout = (libundumpi_unsafe_fun)callback->on_iowait;
  callarr[DUMPIO_Testall].callout = (libundumpi_unsafe_fun)callback->on_iotestall;
  callarr[DUMPIO_Waitall].callout = (libundumpi_unsafe_fun)callback->on_iowaitall;
  callarr[DUMPIO_Testany].callout = (libundumpi_unsafe_fun)callback->on_iotestany;
  callarr[DUMPIO_Waitany].callout = (libundumpi_unsafe_fun)callback->on_iowaitany;
  callarr[DUMPIO_Waitsome].callout = (libundumpi_unsafe_fun)callback->on_iowaitsome;
  callarr[DUMPIO_Testsome].callout = (libundumpi_unsafe_fun)callback->on_iotestsome;
  callarr[DUMPI_ALL_FUNCTIONS].callout = NULL;
  callarr[DUMPI_Function_enter].callout = (libundumpi_unsafe_fun)callback->on_function_enter;
  callarr[DUMPI_Function_exit].callout = (libundumpi_unsafe_fun)callback->on_function_exit;

}

void libundumpi_populate_handlers(const libundumpi_callbacks *callback,
           libundumpi_cbpair *callarr)
{
  callarr[DUMPI_Send].handler = libundumpi_grab_send;
  callarr[DUMPI_Recv].handler = libundumpi_grab_recv;
  callarr[DUMPI_Get_count].handler = libundumpi_grab_get_count;
  callarr[DUMPI_Bsend].handler = libundumpi_grab_bsend;
  callarr[DUMPI_Ssend].handler = libundumpi_grab_ssend;
  callarr[DUMPI_Rsend].handler = libundumpi_grab_rsend;
  callarr[DUMPI_Buffer_attach].handler = libundumpi_grab_buffer_attach;
  callarr[DUMPI_Buffer_detach].handler = libundumpi_grab_buffer_detach;
  callarr[DUMPI_Isend].handler = libundumpi_grab_isend;
  callarr[DUMPI_Ibsend].handler = libundumpi_grab_ibsend;
  callarr[DUMPI_Issend].handler = libundumpi_grab_issend;
  callarr[DUMPI_Irsend].handler = libundumpi_grab_irsend;
  callarr[DUMPI_Irecv].handler = libundumpi_grab_irecv;
  callarr[DUMPI_Wait].handler = libundumpi_grab_wait;
  callarr[DUMPI_Test].handler = libundumpi_grab_test;
  callarr[DUMPI_Request_free].handler = libundumpi_grab_request_free;
  callarr[DUMPI_Waitany].handler = libundumpi_grab_waitany;
  callarr[DUMPI_Testany].handler = libundumpi_grab_testany;
  callarr[DUMPI_Waitall].handler = libundumpi_grab_waitall;
  callarr[DUMPI_Testall].handler = libundumpi_grab_testall;
  callarr[DUMPI_Waitsome].handler = libundumpi_grab_waitsome;
  callarr[DUMPI_Testsome].handler = libundumpi_grab_testsome;
  callarr[DUMPI_Iprobe].handler = libundumpi_grab_iprobe;
  callarr[DUMPI_Probe].handler = libundumpi_grab_probe;
  callarr[DUMPI_Cancel].handler = libundumpi_grab_cancel;
  callarr[DUMPI_Test_cancelled].handler = libundumpi_grab_test_cancelled;
  callarr[DUMPI_Send_init].handler = libundumpi_grab_send_init;
  callarr[DUMPI_Bsend_init].handler = libundumpi_grab_bsend_init;
  callarr[DUMPI_Ssend_init].handler = libundumpi_grab_ssend_init;
  callarr[DUMPI_Rsend_init].handler = libundumpi_grab_rsend_init;
  callarr[DUMPI_Recv_init].handler = libundumpi_grab_recv_init;
  callarr[DUMPI_Start].handler = libundumpi_grab_start;
  callarr[DUMPI_Startall].handler = libundumpi_grab_startall;
  callarr[DUMPI_Sendrecv].handler = libundumpi_grab_sendrecv;
  callarr[DUMPI_Sendrecv_replace].handler = libundumpi_grab_sendrecv_replace;
  callarr[DUMPI_Type_contiguous].handler = libundumpi_grab_type_contiguous;
  callarr[DUMPI_Type_vector].handler = libundumpi_grab_type_vector;
  callarr[DUMPI_Type_hvector].handler = libundumpi_grab_type_hvector;
  callarr[DUMPI_Type_indexed].handler = libundumpi_grab_type_indexed;
  callarr[DUMPI_Type_hindexed].handler = libundumpi_grab_type_hindexed;
  callarr[DUMPI_Type_struct].handler = libundumpi_grab_type_struct;
  callarr[DUMPI_Address].handler = libundumpi_grab_address;
  callarr[DUMPI_Type_extent].handler = libundumpi_grab_type_extent;
  callarr[DUMPI_Type_size].handler = libundumpi_grab_type_size;
  callarr[DUMPI_Type_lb].handler = libundumpi_grab_type_lb;
  callarr[DUMPI_Type_ub].handler = libundumpi_grab_type_ub;
  callarr[DUMPI_Type_commit].handler = libundumpi_grab_type_commit;
  callarr[DUMPI_Type_free].handler = libundumpi_grab_type_free;
  callarr[DUMPI_Get_elements].handler = libundumpi_grab_get_elements;
  callarr[DUMPI_Pack].handler = libundumpi_grab_pack;
  callarr[DUMPI_Unpack].handler = libundumpi_grab_unpack;
  callarr[DUMPI_Pack_size].handler = libundumpi_grab_pack_size;
  callarr[DUMPI_Barrier].handler = libundumpi_grab_barrier;
  callarr[DUMPI_Bcast].handler = libundumpi_grab_bcast;
  callarr[DUMPI_Gather].handler = libundumpi_grab_gather;
  callarr[DUMPI_Gatherv].handler = libundumpi_grab_gatherv;
  callarr[DUMPI_Scatter].handler = libundumpi_grab_scatter;
  callarr[DUMPI_Scatterv].handler = libundumpi_grab_scatterv;
  callarr[DUMPI_Allgather].handler = libundumpi_grab_allgather;
  callarr[DUMPI_Allgatherv].handler = libundumpi_grab_allgatherv;
  callarr[DUMPI_Alltoall].handler = libundumpi_grab_alltoall;
  callarr[DUMPI_Alltoallv].handler = libundumpi_grab_alltoallv;
  callarr[DUMPI_Reduce].handler = libundumpi_grab_reduce;
  callarr[DUMPI_Op_create].handler = libundumpi_grab_op_create;
  callarr[DUMPI_Op_free].handler = libundumpi_grab_op_free;
  callarr[DUMPI_Allreduce].handler = libundumpi_grab_allreduce;
  callarr[DUMPI_Reduce_scatter].handler = libundumpi_grab_reduce_scatter;
  callarr[DUMPI_Scan].handler = libundumpi_grab_scan;
  callarr[DUMPI_Group_size].handler = libundumpi_grab_group_size;
  callarr[DUMPI_Group_rank].handler = libundumpi_grab_group_rank;
  callarr[DUMPI_Group_translate_ranks].handler = libundumpi_grab_group_translate_ranks;
  callarr[DUMPI_Group_compare].handler = libundumpi_grab_group_compare;
  callarr[DUMPI_Comm_group].handler = libundumpi_grab_comm_group;
  callarr[DUMPI_Group_union].handler = libundumpi_grab_group_union;
  callarr[DUMPI_Group_intersection].handler = libundumpi_grab_group_intersection;
  callarr[DUMPI_Group_difference].handler = libundumpi_grab_group_difference;
  callarr[DUMPI_Group_incl].handler = libundumpi_grab_group_incl;
  callarr[DUMPI_Group_excl].handler = libundumpi_grab_group_excl;
  callarr[DUMPI_Group_range_incl].handler = libundumpi_grab_group_range_incl;
  callarr[DUMPI_Group_range_excl].handler = libundumpi_grab_group_range_excl;
  callarr[DUMPI_Group_free].handler = libundumpi_grab_group_free;
  callarr[DUMPI_Comm_size].handler = libundumpi_grab_comm_size;
  callarr[DUMPI_Comm_rank].handler = libundumpi_grab_comm_rank;
  callarr[DUMPI_Comm_compare].handler = libundumpi_grab_comm_compare;
  callarr[DUMPI_Comm_dup].handler = libundumpi_grab_comm_dup;
  callarr[DUMPI_Comm_create].handler = libundumpi_grab_comm_create;
  callarr[DUMPI_Comm_split].handler = libundumpi_grab_comm_split;
  callarr[DUMPI_Comm_free].handler = libundumpi_grab_comm_free;
  callarr[DUMPI_Comm_test_inter].handler = libundumpi_grab_comm_test_inter;
  callarr[DUMPI_Comm_remote_size].handler = libundumpi_grab_comm_remote_size;
  callarr[DUMPI_Comm_remote_group].handler = libundumpi_grab_comm_remote_group;
  callarr[DUMPI_Intercomm_create].handler = libundumpi_grab_intercomm_create;
  callarr[DUMPI_Intercomm_merge].handler = libundumpi_grab_intercomm_merge;
  callarr[DUMPI_Keyval_create].handler = libundumpi_grab_keyval_create;
  callarr[DUMPI_Keyval_free].handler = libundumpi_grab_keyval_free;
  callarr[DUMPI_Attr_put].handler = libundumpi_grab_attr_put;
  callarr[DUMPI_Attr_get].handler = libundumpi_grab_attr_get;
  callarr[DUMPI_Attr_delete].handler = libundumpi_grab_attr_delete;
  callarr[DUMPI_Topo_test].handler = libundumpi_grab_topo_test;
  callarr[DUMPI_Cart_create].handler = libundumpi_grab_cart_create;
  callarr[DUMPI_Dims_create].handler = libundumpi_grab_dims_create;
  callarr[DUMPI_Graph_create].handler = libundumpi_grab_graph_create;
  callarr[DUMPI_Graphdims_get].handler = libundumpi_grab_graphdims_get;
  callarr[DUMPI_Graph_get].handler = libundumpi_grab_graph_get;
  callarr[DUMPI_Cartdim_get].handler = libundumpi_grab_cartdim_get;
  callarr[DUMPI_Cart_get].handler = libundumpi_grab_cart_get;
  callarr[DUMPI_Cart_rank].handler = libundumpi_grab_cart_rank;
  callarr[DUMPI_Cart_coords].handler = libundumpi_grab_cart_coords;
  callarr[DUMPI_Graph_neighbors_count].handler = libundumpi_grab_graph_neighbors_count;
  callarr[DUMPI_Graph_neighbors].handler = libundumpi_grab_graph_neighbors;
  callarr[DUMPI_Cart_shift].handler = libundumpi_grab_cart_shift;
  callarr[DUMPI_Cart_sub].handler = libundumpi_grab_cart_sub;
  callarr[DUMPI_Cart_map].handler = libundumpi_grab_cart_map;
  callarr[DUMPI_Graph_map].handler = libundumpi_grab_graph_map;
  callarr[DUMPI_Get_processor_name].handler = libundumpi_grab_get_processor_name;
  callarr[DUMPI_Get_version].handler = libundumpi_grab_get_version;
  callarr[DUMPI_Errhandler_create].handler = libundumpi_grab_errhandler_create;
  callarr[DUMPI_Errhandler_create].callout = (libundumpi_unsafe_fun)callback->on_errhandler_create;
  callarr[DUMPI_Errhandler_set].handler = libundumpi_grab_errhandler_set;
  callarr[DUMPI_Errhandler_set].callout = (libundumpi_unsafe_fun)callback->on_errhandler_set;
  callarr[DUMPI_Errhandler_get].handler = libundumpi_grab_errhandler_get;
  callarr[DUMPI_Errhandler_get].callout = (libundumpi_unsafe_fun)callback->on_errhandler_get;
  callarr[DUMPI_Errhandler_free].handler = libundumpi_grab_errhandler_free;
  callarr[DUMPI_Errhandler_free].callout = (libundumpi_unsafe_fun)callback->on_errhandler_free;
  callarr[DUMPI_Error_string].handler = libundumpi_grab_error_string;
  callarr[DUMPI_Error_class].handler = libundumpi_grab_error_class;
  callarr[DUMPI_Wtime].handler = libundumpi_grab_wtime;
  callarr[DUMPI_Wtick].handler = libundumpi_grab_wtick;
  callarr[DUMPI_Init].handler = libundumpi_grab_init;
  callarr[DUMPI_Finalize].handler = libundumpi_grab_finalize;
  callarr[DUMPI_Initialized].handler = libundumpi_grab_initialized;
  callarr[DUMPI_Abort].handler = libundumpi_grab_abort;
  callarr[DUMPI_Close_port].handler = libundumpi_grab_close_port;
  callarr[DUMPI_Comm_accept].handler = libundumpi_grab_comm_accept;
  callarr[DUMPI_Comm_connect].handler = libundumpi_grab_comm_connect;
  callarr[DUMPI_Comm_disconnect].handler = libundumpi_grab_comm_disconnect;
  callarr[DUMPI_Comm_get_parent].handler = libundumpi_grab_comm_get_parent;
  callarr[DUMPI_Comm_join].handler = libundumpi_grab_comm_join;
  callarr[DUMPI_Comm_spawn].handler = libundumpi_grab_comm_spawn;
  callarr[DUMPI_Comm_spawn_multiple].handler = libundumpi_grab_comm_spawn_multiple;
  callarr[DUMPI_Lookup_name].handler = libundumpi_grab_lookup_name;
  callarr[DUMPI_Open_port].handler = libundumpi_grab_open_port;
  callarr[DUMPI_Publish_name].handler = libundumpi_grab_publish_name;
  callarr[DUMPI_Unpublish_name].handler = libundumpi_grab_unpublish_name;
  callarr[DUMPI_Accumulate].handler = libundumpi_grab_accumulate;
  callarr[DUMPI_Get].handler = libundumpi_grab_get;
  callarr[DUMPI_Put].handler = libundumpi_grab_put;
  callarr[DUMPI_Win_complete].handler = libundumpi_grab_win_complete;
  callarr[DUMPI_Win_create].handler = libundumpi_grab_win_create;
  callarr[DUMPI_Win_fence].handler = libundumpi_grab_win_fence;
  callarr[DUMPI_Win_free].handler = libundumpi_grab_win_free;
  callarr[DUMPI_Win_get_group].handler = libundumpi_grab_win_get_group;
  callarr[DUMPI_Win_lock].handler = libundumpi_grab_win_lock;
  callarr[DUMPI_Win_post].handler = libundumpi_grab_win_post;
  callarr[DUMPI_Win_start].handler = libundumpi_grab_win_start;
  callarr[DUMPI_Win_test].handler = libundumpi_grab_win_test;
  callarr[DUMPI_Win_unlock].handler = libundumpi_grab_win_unlock;
  callarr[DUMPI_Win_wait].handler = libundumpi_grab_win_wait;
  callarr[DUMPI_Alltoallw].handler = libundumpi_grab_alltoallw;
  callarr[DUMPI_Exscan].handler = libundumpi_grab_exscan;
  callarr[DUMPI_Add_error_class].handler = libundumpi_grab_add_error_class;
  callarr[DUMPI_Add_error_code].handler = libundumpi_grab_add_error_code;
  callarr[DUMPI_Add_error_string].handler = libundumpi_grab_add_error_string;
  callarr[DUMPI_Comm_call_errhandler].handler = libundumpi_grab_comm_call_errhandler;
  callarr[DUMPI_Comm_call_errhandler].callout = (libundumpi_unsafe_fun)callback->on_comm_call_errhandler;
  callarr[DUMPI_Comm_create_keyval].handler = libundumpi_grab_comm_create_keyval;
  callarr[DUMPI_Comm_delete_attr].handler = libundumpi_grab_comm_delete_attr;
  callarr[DUMPI_Comm_free_keyval].handler = libundumpi_grab_comm_free_keyval;
  callarr[DUMPI_Comm_get_attr].handler = libundumpi_grab_comm_get_attr;
  callarr[DUMPI_Comm_get_name].handler = libundumpi_grab_comm_get_name;
  callarr[DUMPI_Comm_set_attr].handler = libundumpi_grab_comm_set_attr;
  callarr[DUMPI_Comm_set_name].handler = libundumpi_grab_comm_set_name;
  callarr[DUMPI_File_call_errhandler].handler = libundumpi_grab_file_call_errhandler;
  callarr[DUMPI_File_call_errhandler].callout = (libundumpi_unsafe_fun)callback->on_file_call_errhandler;
  callarr[DUMPI_Grequest_complete].handler = libundumpi_grab_grequest_complete;
  callarr[DUMPI_Grequest_start].handler = libundumpi_grab_grequest_start;
  callarr[DUMPI_Init_thread].handler = libundumpi_grab_init_thread;
  callarr[DUMPI_Is_thread_main].handler = libundumpi_grab_is_thread_main;
  callarr[DUMPI_Query_thread].handler = libundumpi_grab_query_thread;
  callarr[DUMPI_Status_set_cancelled].handler = libundumpi_grab_status_set_cancelled;
  callarr[DUMPI_Status_set_elements].handler = libundumpi_grab_status_set_elements;
  callarr[DUMPI_Type_create_keyval].handler = libundumpi_grab_type_create_keyval;
  callarr[DUMPI_Type_delete_attr].handler = libundumpi_grab_type_delete_attr;
  callarr[DUMPI_Type_dup].handler = libundumpi_grab_type_dup;
  callarr[DUMPI_Type_free_keyval].handler = libundumpi_grab_type_free_keyval;
  callarr[DUMPI_Type_get_attr].handler = libundumpi_grab_type_get_attr;
  callarr[DUMPI_Type_get_contents].handler = libundumpi_grab_type_get_contents;
  callarr[DUMPI_Type_get_envelope].handler = libundumpi_grab_type_get_envelope;
  callarr[DUMPI_Type_get_name].handler = libundumpi_grab_type_get_name;
  callarr[DUMPI_Type_set_attr].handler = libundumpi_grab_type_set_attr;
  callarr[DUMPI_Type_set_name].handler = libundumpi_grab_type_set_name;
  callarr[DUMPI_Type_match_size].handler = libundumpi_grab_type_match_size;
  callarr[DUMPI_Win_call_errhandler].handler = libundumpi_grab_win_call_errhandler;
  callarr[DUMPI_Win_call_errhandler].callout = (libundumpi_unsafe_fun)callback->on_win_call_errhandler;
  callarr[DUMPI_Win_create_keyval].handler = libundumpi_grab_win_create_keyval;
  callarr[DUMPI_Win_delete_attr].handler = libundumpi_grab_win_delete_attr;
  callarr[DUMPI_Win_free_keyval].handler = libundumpi_grab_win_free_keyval;
  callarr[DUMPI_Win_get_attr].handler = libundumpi_grab_win_get_attr;
  callarr[DUMPI_Win_get_name].handler = libundumpi_grab_win_get_name;
  callarr[DUMPI_Win_set_attr].handler = libundumpi_grab_win_set_attr;
  callarr[DUMPI_Win_set_name].handler = libundumpi_grab_win_set_name;
  callarr[DUMPI_Alloc_mem].handler = libundumpi_grab_alloc_mem;
  callarr[DUMPI_Comm_create_errhandler].handler = libundumpi_grab_comm_create_errhandler;
  callarr[DUMPI_Comm_create_errhandler].callout = (libundumpi_unsafe_fun)callback->on_comm_create_errhandler;
  callarr[DUMPI_Comm_get_errhandler].handler = libundumpi_grab_comm_get_errhandler;
  callarr[DUMPI_Comm_get_errhandler].callout = (libundumpi_unsafe_fun)callback->on_comm_get_errhandler;
  callarr[DUMPI_Comm_set_errhandler].handler = libundumpi_grab_comm_set_errhandler;
  callarr[DUMPI_Comm_set_errhandler].callout = (libundumpi_unsafe_fun)callback->on_comm_set_errhandler;
  callarr[DUMPI_File_create_errhandler].handler = libundumpi_grab_file_create_errhandler;
  callarr[DUMPI_File_create_errhandler].callout = (libundumpi_unsafe_fun)callback->on_file_create_errhandler;
  callarr[DUMPI_File_get_errhandler].handler = libundumpi_grab_file_get_errhandler;
  callarr[DUMPI_File_get_errhandler].callout = (libundumpi_unsafe_fun)callback->on_file_get_errhandler;
  callarr[DUMPI_File_set_errhandler].handler = libundumpi_grab_file_set_errhandler;
  callarr[DUMPI_File_set_errhandler].callout = (libundumpi_unsafe_fun)callback->on_file_set_errhandler;
  callarr[DUMPI_Finalized].handler = libundumpi_grab_finalized;
  callarr[DUMPI_Free_mem].handler = libundumpi_grab_free_mem;
  callarr[DUMPI_Get_address].handler = libundumpi_grab_get_address;
  callarr[DUMPI_Info_create].handler = libundumpi_grab_info_create;
  callarr[DUMPI_Info_delete].handler = libundumpi_grab_info_delete;
  callarr[DUMPI_Info_dup].handler = libundumpi_grab_info_dup;
  callarr[DUMPI_Info_free].handler = libundumpi_grab_info_free;
  callarr[DUMPI_Info_get].handler = libundumpi_grab_info_get;
  callarr[DUMPI_Info_get_nkeys].handler = libundumpi_grab_info_get_nkeys;
  callarr[DUMPI_Info_get_nthkey].handler = libundumpi_grab_info_get_nthkey;
  callarr[DUMPI_Info_get_valuelen].handler = libundumpi_grab_info_get_valuelen;
  callarr[DUMPI_Info_set].handler = libundumpi_grab_info_set;
  callarr[DUMPI_Pack_external].handler = libundumpi_grab_pack_external;
  callarr[DUMPI_Pack_external_size].handler = libundumpi_grab_pack_external_size;
  callarr[DUMPI_Request_get_status].handler = libundumpi_grab_request_get_status;
  callarr[DUMPI_Type_create_darray].handler = libundumpi_grab_type_create_darray;
  callarr[DUMPI_Type_create_hindexed].handler = libundumpi_grab_type_create_hindexed;
  callarr[DUMPI_Type_create_hvector].handler = libundumpi_grab_type_create_hvector;
  callarr[DUMPI_Type_create_indexed_block].handler = libundumpi_grab_type_create_indexed_block;
  callarr[DUMPI_Type_create_resized].handler = libundumpi_grab_type_create_resized;
  callarr[DUMPI_Type_create_struct].handler = libundumpi_grab_type_create_struct;
  callarr[DUMPI_Type_create_subarray].handler = libundumpi_grab_type_create_subarray;
  callarr[DUMPI_Type_get_extent].handler = libundumpi_grab_type_get_extent;
  callarr[DUMPI_Type_get_true_extent].handler = libundumpi_grab_type_get_true_extent;
  callarr[DUMPI_Unpack_external].handler = libundumpi_grab_unpack_external;
  callarr[DUMPI_Win_create_errhandler].handler = libundumpi_grab_win_create_errhandler;
  callarr[DUMPI_Win_create_errhandler].callout = (libundumpi_unsafe_fun)callback->on_win_create_errhandler;
  callarr[DUMPI_Win_get_errhandler].handler = libundumpi_grab_win_get_errhandler;
  callarr[DUMPI_Win_get_errhandler].callout = (libundumpi_unsafe_fun)callback->on_win_get_errhandler;
  callarr[DUMPI_Win_set_errhandler].handler = libundumpi_grab_win_set_errhandler;
  callarr[DUMPI_Win_set_errhandler].callout = (libundumpi_unsafe_fun)callback->on_win_set_errhandler;
  callarr[DUMPI_File_open].handler = libundumpi_grab_file_open;
  callarr[DUMPI_File_close].handler = libundumpi_grab_file_close;
  callarr[DUMPI_File_delete].handler = libundumpi_grab_file_delete;
  callarr[DUMPI_File_set_size].handler = libundumpi_grab_file_set_size;
  callarr[DUMPI_File_preallocate].handler = libundumpi_grab_file_preallocate;
  callarr[DUMPI_File_get_size].handler = libundumpi_grab_file_get_size;
  callarr[DUMPI_File_get_group].handler = libundumpi_grab_file_get_group;
  callarr[DUMPI_File_get_amode].handler = libundumpi_grab_file_get_amode;
  callarr[DUMPI_File_set_info].handler = libundumpi_grab_file_set_info;
  callarr[DUMPI_File_get_info].handler = libundumpi_grab_file_get_info;
  callarr[DUMPI_File_set_view].handler = libundumpi_grab_file_set_view;
  callarr[DUMPI_File_get_view].handler = libundumpi_grab_file_get_view;
  callarr[DUMPI_File_read_at].handler = libundumpi_grab_file_read_at;
  callarr[DUMPI_File_read_at_all].handler = libundumpi_grab_file_read_at_all;
  callarr[DUMPI_File_write_at].handler = libundumpi_grab_file_write_at;
  callarr[DUMPI_File_write_at_all].handler = libundumpi_grab_file_write_at_all;
  callarr[DUMPI_File_iread_at].handler = libundumpi_grab_file_iread_at;
  callarr[DUMPI_File_iwrite_at].handler = libundumpi_grab_file_iwrite_at;
  callarr[DUMPI_File_read].handler = libundumpi_grab_file_read;
  callarr[DUMPI_File_read_all].handler = libundumpi_grab_file_read_all;
  callarr[DUMPI_File_write].handler = libundumpi_grab_file_write;
  callarr[DUMPI_File_write_all].handler = libundumpi_grab_file_write_all;
  callarr[DUMPI_File_iread].handler = libundumpi_grab_file_iread;
  callarr[DUMPI_File_iwrite].handler = libundumpi_grab_file_iwrite;
  callarr[DUMPI_File_seek].handler = libundumpi_grab_file_seek;
  callarr[DUMPI_File_get_position].handler = libundumpi_grab_file_get_position;
  callarr[DUMPI_File_get_byte_offset].handler = libundumpi_grab_file_get_byte_offset;
  callarr[DUMPI_File_read_shared].handler = libundumpi_grab_file_read_shared;
  callarr[DUMPI_File_write_shared].handler = libundumpi_grab_file_write_shared;
  callarr[DUMPI_File_iread_shared].handler = libundumpi_grab_file_iread_shared;
  callarr[DUMPI_File_iwrite_shared].handler = libundumpi_grab_file_iwrite_shared;
  callarr[DUMPI_File_read_ordered].handler = libundumpi_grab_file_read_ordered;
  callarr[DUMPI_File_write_ordered].handler = libundumpi_grab_file_write_ordered;
  callarr[DUMPI_File_seek_shared].handler = libundumpi_grab_file_seek_shared;
  callarr[DUMPI_File_get_position_shared].handler = libundumpi_grab_file_get_position_shared;
  callarr[DUMPI_File_read_at_all_begin].handler = libundumpi_grab_file_read_at_all_begin;
  callarr[DUMPI_File_read_at_all_end].handler = libundumpi_grab_file_read_at_all_end;
  callarr[DUMPI_File_write_at_all_begin].handler = libundumpi_grab_file_write_at_all_begin;
  callarr[DUMPI_File_write_at_all_end].handler = libundumpi_grab_file_write_at_all_end;
  callarr[DUMPI_File_read_all_begin].handler = libundumpi_grab_file_read_all_begin;
  callarr[DUMPI_File_read_all_end].handler = libundumpi_grab_file_read_all_end;
  callarr[DUMPI_File_write_all_begin].handler = libundumpi_grab_file_write_all_begin;
  callarr[DUMPI_File_write_all_end].handler = libundumpi_grab_file_write_all_end;
  callarr[DUMPI_File_read_ordered_begin].handler = libundumpi_grab_file_read_ordered_begin;
  callarr[DUMPI_File_read_ordered_end].handler = libundumpi_grab_file_read_ordered_end;
  callarr[DUMPI_File_write_ordered_begin].handler = libundumpi_grab_file_write_ordered_begin;
  callarr[DUMPI_File_write_ordered_end].handler = libundumpi_grab_file_write_ordered_end;
  callarr[DUMPI_File_get_type_extent].handler = libundumpi_grab_file_get_type_extent;
  callarr[DUMPI_Register_datarep].handler = libundumpi_grab_register_datarep;
  callarr[DUMPI_File_set_atomicity].handler = libundumpi_grab_file_set_atomicity;
  callarr[DUMPI_File_get_atomicity].handler = libundumpi_grab_file_get_atomicity;
  callarr[DUMPI_File_sync].handler = libundumpi_grab_file_sync;
  callarr[DUMPIO_Test].handler = libdumpi_grab_io_test;
  callarr[DUMPIO_Wait].handler = libdumpi_grab_io_wait;
  callarr[DUMPIO_Testall].handler = libdumpi_grab_io_testall;
  callarr[DUMPIO_Waitall].handler = libdumpi_grab_io_waitall;
  callarr[DUMPIO_Testany].handler = libdumpi_grab_io_testany;
  callarr[DUMPIO_Waitany].handler = libdumpi_grab_io_waitany;
  callarr[DUMPIO_Waitsome].handler = libdumpi_grab_io_waitsome;
  callarr[DUMPIO_Testsome].handler = libdumpi_grab_io_testsome;
  callarr[DUMPI_ALL_FUNCTIONS].handler = libundumpi_grab_all_functions;
  callarr[DUMPI_Function_enter].handler = libundumpi_grab_function_enter;
  callarr[DUMPI_Function_exit].handler = libundumpi_grab_function_exit;
}

