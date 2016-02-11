/*
s *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
 */

/*
 * Bindings for all MPI routines.
 * Auto-generated on Thu Aug 20 23:13:24 PDT 2009
 */

#include <dumpi/dumpiconfig.h>
#include <dumpi/libdumpi/mpibindings.h>
#include <dumpi/libdumpi/callprofile.h>
#include <dumpi/libdumpi/fused-bindings.h>
#include <dumpi/libdumpi/libdumpi.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#ifdef DUMPI_USE_PTHREADS
#include <pthread.h>
#endif /* ! DUMPI_USE_PTHREADS */

/* Unfortunately, MPIO_Test is often defined to be MPI_Test.*/
#undef MPIO_Test
#undef MPIO_Testany
#undef MPIO_Testsome
#undef MPIO_Testall
#undef MPIO_Wait
#undef MPIO_Waitany
#undef MPIO_Waitsome
#undef MPIO_Waitany

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))

#define MIN_USER_KEY 1ULL
#define MAX_USER_KEY 4096ULL

static uint64_t next_comment_index_ = MAX_USER_KEY;
static size_t comment_buffer_size_ = 0;
static size_t comment_buffer_offset_ = 0;
static char *comment_buffer_ = NULL;
#define comment_header_ "ANNOTATION: "

/*
 * Utility function to print to the comment buffer.
 */
static int print_to_comment_buffer(va_list arglist) {
  size_t strsize;
  const char *fmt;
  va_list argcpy;
  fmt = va_arg(arglist, const char*);
  va_copy(argcpy, arglist);
  strsize = vsnprintf(comment_buffer_+comment_buffer_offset_,
                      comment_buffer_size_-comment_buffer_offset_,
                      fmt, arglist);
  if(strsize >= (comment_buffer_size_-comment_buffer_offset_)) {
    /* Ran out of buffer space -- create some more */
    strsize += comment_buffer_offset_;
    assert((comment_buffer_ = realloc(comment_buffer_, strsize + 1)) != NULL);
    comment_buffer_size_ = strsize + 1;
    snprintf(comment_buffer_, comment_buffer_offset_+1, comment_header_);
    vsnprintf(comment_buffer_+comment_buffer_offset_,
              comment_buffer_size_+comment_buffer_offset_, fmt, argcpy);
  }  
  va_end(argcpy);
  return 1;
}

/*
 * subvert the function entry/function exit mechanism to allow custom
 * annotation.
 *  MPI_Pcontrol(2, const char *fmt, ...) creates a string annotation
 *  MPI_Pcontrol(3, uint64_t key, const char *fmt, ...) is a keyed annotation
 */
int MPI_Pcontrol(const int level, ...) {
  va_list arglist;
  dumpi_func_call stat;
  uint16_t thread;
  dumpi_time cpu_start, wall_start, cpu_stop, wall_stop;
  if(dumpi_global == NULL)
    libdumpi_init();
  DUMPI_START_TIME(cpu_start, wall_start);
  DUMPI_STOP_TIME(cpu_start, wall_start);
  thread = libdumpi_get_thread_id();
  if(! comment_buffer_) {
    comment_buffer_offset_ = strlen(comment_header_);
    comment_buffer_size_ = comment_buffer_offset_ + 10;;
    assert((comment_buffer_ = (char*)malloc(comment_buffer_size_)) != NULL);
    snprintf(comment_buffer_, comment_buffer_offset_+1, comment_header_);
  }
  switch(level) {
  case 0:
    libdumpi_disable_profiling();
    break;
  case 1:
    libdumpi_enable_profiling();
    break;
  case 2:
    /* Automated keys count starting at 4096.
     * On 64-bit Linux systems, we get quite a few entries before 
     * bumping into the text segment of the program. */
    if(dumpi_global->output->function[DUMPI_ALL_FUNCTIONS]) {
      va_start(arglist, level);
      print_to_comment_buffer(arglist);
      stat.fn = next_comment_index_++;
      libdumpi_insert_data(stat.fn, comment_buffer_);
      DUMPI_START_TIME(cpu_stop, wall_stop);
      DUMPI_STOP_TIME(cpu_stop, wall_stop);
      libdumpi_lock_io();
      dumpi_write_func_enter(&stat, thread, &cpu_start, &wall_start,
                             dumpi_global->perf, dumpi_global->output,
                             dumpi_global->profile);
      dumpi_write_func_exit(&stat, thread, &cpu_stop, &wall_stop,
                            dumpi_global->perf, dumpi_global->output,
                            dumpi_global->profile);
      libdumpi_unlock_io();
      va_end(arglist);
    }
    break;
  case 3:
    /* We permit user-keys in the range 1..4095.
     * (reserve '0' for NULL pointer). 
     * This should irmly put us in the first (blocked) page on most systems */
    if(dumpi_global->output->function[DUMPI_ALL_FUNCTIONS]) {
      va_start(arglist, level);
      stat.fn = va_arg(arglist, uint64_t);
      if((stat.fn < MIN_USER_KEY) || (stat.fn >= MAX_USER_KEY)) {
        fprintf(stderr, "WARNING:  DUMPI:  MPI_Pcontrol(3, ...):  "
                "User key %llu is outside the recommended range "
                "%llu through %llu\n", stat.fn, MIN_USER_KEY, MAX_USER_KEY-1);
      }
      print_to_comment_buffer(arglist);
      libdumpi_insert_data(stat.fn, comment_buffer_);    
      DUMPI_START_TIME(cpu_stop, wall_stop);
      DUMPI_STOP_TIME(cpu_stop, wall_stop);
      libdumpi_lock_io();
      dumpi_write_func_enter(&stat, thread, &cpu_start, &wall_start,
                             dumpi_global->perf, dumpi_global->output,
                             dumpi_global->profile);
      dumpi_write_func_exit(&stat, thread, &cpu_stop, &wall_stop,
                            dumpi_global->perf, dumpi_global->output,
                            dumpi_global->profile);
      libdumpi_unlock_io();
      va_end(arglist);
    }
  }
  return MPI_SUCCESS;
}

#ifndef DUMPI_SKIP_MPI_SEND
int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_send stat;
  memset(&stat, 0, sizeof(dumpi_send));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Send);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Send);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Send);
  }
  retval = PMPI_Send(buf, count, datatype, dest, tag, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Send);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_send(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Send);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SEND */

#ifndef DUMPI_SKIP_MPI_RECV
int MPI_Recv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_recv stat;
  memset(&stat, 0, sizeof(dumpi_recv));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Recv);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Recv);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_SOURCE_FROM_INT(stat.source, source);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Recv);
  }
  retval = PMPI_Recv(buf, count, datatype, source, tag, comm, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Recv);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_recv(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Recv);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_RECV */

#ifndef DUMPI_SKIP_MPI_GET_COUNT
int MPI_Get_count(const MPI_Status *status, MPI_Datatype datatype, int *count) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_get_count stat;
  memset(&stat, 0, sizeof(dumpi_get_count));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Get_count);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_count);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Get_count);
  }
  retval = PMPI_Get_count(status, datatype, count);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_count);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.count, *count);
    libdumpi_lock_io();
    dumpi_write_get_count(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Get_count);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GET_COUNT */

#ifndef DUMPI_SKIP_MPI_BSEND
int MPI_Bsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_bsend stat;
  memset(&stat, 0, sizeof(dumpi_bsend));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Bsend);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Bsend);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Bsend);
  }
  retval = PMPI_Bsend(buf, count, datatype, dest, tag, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Bsend);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_bsend(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Bsend);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_BSEND */

#ifndef DUMPI_SKIP_MPI_SSEND
int MPI_Ssend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_ssend stat;
  memset(&stat, 0, sizeof(dumpi_ssend));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Ssend);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Ssend);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Ssend);
  }
  retval = PMPI_Ssend(buf, count, datatype, dest, tag, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Ssend);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_ssend(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Ssend);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SSEND */

#ifndef DUMPI_SKIP_MPI_RSEND
int MPI_Rsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_rsend stat;
  memset(&stat, 0, sizeof(dumpi_rsend));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Rsend);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Rsend);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Rsend);
  }
  retval = PMPI_Rsend(buf, count, datatype, dest, tag, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Rsend);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_rsend(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Rsend);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_RSEND */

#ifndef DUMPI_SKIP_MPI_BUFFER_ATTACH
int MPI_Buffer_attach(void *buf, int size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_buffer_attach stat;
  memset(&stat, 0, sizeof(dumpi_buffer_attach));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Buffer_attach);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Buffer_attach);
    DUMPI_INT_FROM_INT(stat.size, size);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Buffer_attach);
  }
  retval = PMPI_Buffer_attach(buf, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Buffer_attach);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_buffer_attach(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Buffer_attach);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_BUFFER_ATTACH */

#ifndef DUMPI_SKIP_MPI_BUFFER_DETACH
int MPI_Buffer_detach(void *buf, int *size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_buffer_detach stat;
  memset(&stat, 0, sizeof(dumpi_buffer_detach));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Buffer_detach);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Buffer_detach);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Buffer_detach);
  }
  retval = PMPI_Buffer_detach(buf, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Buffer_detach);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.size, *size);
    libdumpi_lock_io();
    dumpi_write_buffer_detach(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Buffer_detach);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_BUFFER_DETACH */

#ifndef DUMPI_SKIP_MPI_ISEND
int MPI_Isend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_isend stat;
  memset(&stat, 0, sizeof(dumpi_isend));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Isend);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Isend);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_INT_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Isend);
  }
  retval = PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Isend);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_isend(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Isend);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ISEND */

#ifndef DUMPI_SKIP_MPI_IBSEND
int MPI_Ibsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_ibsend stat;
  memset(&stat, 0, sizeof(dumpi_ibsend));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Ibsend);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Ibsend);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Ibsend);
  }
  retval = PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Ibsend);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_ibsend(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Ibsend);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_IBSEND */

#ifndef DUMPI_SKIP_MPI_ISSEND
int MPI_Issend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_issend stat;
  memset(&stat, 0, sizeof(dumpi_issend));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Issend);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Issend);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Issend);
  }
  retval = PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Issend);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_issend(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Issend);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ISSEND */

#ifndef DUMPI_SKIP_MPI_IRSEND
int MPI_Irsend(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_irsend stat;
  memset(&stat, 0, sizeof(dumpi_irsend));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Irsend);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Irsend);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Irsend);
  }
  retval = PMPI_Irsend(buf, count, datatype, dest, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Irsend);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_irsend(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Irsend);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_IRSEND */

#ifndef DUMPI_SKIP_MPI_IRECV
int MPI_Irecv(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_irecv stat;
  memset(&stat, 0, sizeof(dumpi_irecv));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Irecv);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Irecv);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_SOURCE_FROM_INT(stat.source, source);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Irecv);
  }
  retval = PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Irecv);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_irecv(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Irecv);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_IRECV */

#ifndef DUMPI_SKIP_MPI_WAIT
int MPI_Wait(MPI_Request *request, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_wait stat;
  memset(&stat, 0, sizeof(dumpi_wait));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Wait);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Wait);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Wait);
  }
  retval = PMPI_Wait(request, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Wait);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_wait(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Wait);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WAIT */

#ifndef DUMPI_SKIP_MPI_TEST
int MPI_Test(MPI_Request *request, int *flag, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_test stat;
  memset(&stat, 0, sizeof(dumpi_test));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Test);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Test);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Test);
  }
  retval = PMPI_Test(request, flag, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Test);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    if(*flag != 0)    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_test(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Test);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TEST */

#ifndef DUMPI_SKIP_MPI_REQUEST_FREE
int MPI_Request_free(MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_request_free stat;
  memset(&stat, 0, sizeof(dumpi_request_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Request_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Request_free);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Request_free);
  }
  retval = PMPI_Request_free(request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Request_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_request_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Request_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_REQUEST_FREE */

#ifndef DUMPI_SKIP_MPI_WAITANY
int MPI_Waitany(int count, MPI_Request *requests, int *index, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_waitany stat;
  memset(&stat, 0, sizeof(dumpi_waitany));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Waitany);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Waitany);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Waitany);
  }
  retval = PMPI_Waitany(count, requests, index, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Waitany);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.index, *index);
    if(*index != MPI_UNDEFINED)    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_waitany(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(requests != NULL) DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(stat.requests);
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Waitany);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WAITANY */

#ifndef DUMPI_SKIP_MPI_TESTANY
int MPI_Testany(int count, MPI_Request *requests, int *index, int *flag, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_testany stat;
  memset(&stat, 0, sizeof(dumpi_testany));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Testany);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Testany);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Testany);
  }
  retval = PMPI_Testany(count, requests, index, flag, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Testany);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.index, *index);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    if(*flag != 0)    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_testany(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(requests != NULL) DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(stat.requests);
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Testany);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TESTANY */

#ifndef DUMPI_SKIP_MPI_WAITALL
int MPI_Waitall(int count, MPI_Request *requests, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_waitall stat;
  memset(&stat, 0, sizeof(dumpi_waitall));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Waitall);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Waitall);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Waitall);
  }
  retval = PMPI_Waitall(count, requests, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Waitall);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(count, stat.statuses, statuses);
    libdumpi_lock_io();
    dumpi_write_waitall(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(requests != NULL) DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(stat.requests);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPI_Waitall);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WAITALL */

#ifndef DUMPI_SKIP_MPI_TESTALL
int MPI_Testall(int count, MPI_Request *requests, int *flag, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_testall stat;
  memset(&stat, 0, sizeof(dumpi_testall));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Testall);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Testall);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Testall);
  }
  retval = PMPI_Testall(count, requests, flag, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Testall);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    if(*flag != 0)    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(count, stat.statuses, statuses);
    libdumpi_lock_io();
    dumpi_write_testall(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(requests != NULL) DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(stat.requests);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPI_Testall);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TESTALL */

#ifndef DUMPI_SKIP_MPI_WAITSOME
int MPI_Waitsome(int count, MPI_Request *requests, int *outcount, int *indices, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_waitsome stat;
  memset(&stat, 0, sizeof(dumpi_waitsome));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Waitsome);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Waitsome);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Waitsome);
  }
  retval = PMPI_Waitsome(count, requests, outcount, indices, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Waitsome);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.outcount, *outcount);
    DUMPI_INT_FROM_INT_ARRAY_1(*outcount, stat.indices, indices);
    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(*outcount, stat.statuses, statuses);
    libdumpi_lock_io();
    dumpi_write_waitsome(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(requests != NULL) DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(stat.requests);
    if(indices != NULL) DUMPI_FREE_INT_FROM_INT(stat.indices);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPI_Waitsome);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WAITSOME */

#ifndef DUMPI_SKIP_MPI_TESTSOME
int MPI_Testsome(int count, MPI_Request *requests, int *outcount, int *indices, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_testsome stat;
  memset(&stat, 0, sizeof(dumpi_testsome));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Testsome);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Testsome);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Testsome);
  }
  retval = PMPI_Testsome(count, requests, outcount, indices, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Testsome);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.outcount, *outcount);
    DUMPI_INT_FROM_INT_ARRAY_1(*outcount, stat.indices, indices);
    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(*outcount, stat.statuses, statuses);
    libdumpi_lock_io();
    dumpi_write_testsome(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(requests != NULL) DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(stat.requests);
    if(indices != NULL) DUMPI_FREE_INT_FROM_INT(stat.indices);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPI_Testsome);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TESTSOME */

#ifndef DUMPI_SKIP_MPI_IPROBE
int MPI_Iprobe(int source, int tag, MPI_Comm comm, int *flag, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_iprobe stat;
  memset(&stat, 0, sizeof(dumpi_iprobe));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Iprobe);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Iprobe);
    DUMPI_SOURCE_FROM_INT(stat.source, source);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Iprobe);
  }
  retval = PMPI_Iprobe(source, tag, comm, flag, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Iprobe);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    if(*flag != 0)    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_iprobe(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Iprobe);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_IPROBE */

#ifndef DUMPI_SKIP_MPI_PROBE
int MPI_Probe(int source, int tag, MPI_Comm comm, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_probe stat;
  memset(&stat, 0, sizeof(dumpi_probe));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Probe);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Probe);
    DUMPI_SOURCE_FROM_INT(stat.source, source);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Probe);
  }
  retval = PMPI_Probe(source, tag, comm, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Probe);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_probe(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Probe);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_PROBE */

#ifndef DUMPI_SKIP_MPI_CANCEL
int MPI_Cancel(MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cancel stat;
  memset(&stat, 0, sizeof(dumpi_cancel));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cancel);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cancel);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cancel);
  }
  retval = PMPI_Cancel(request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cancel);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_cancel(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Cancel);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CANCEL */

#ifndef DUMPI_SKIP_MPI_TEST_CANCELLED
int MPI_Test_cancelled(const MPI_Status *status, int *cancelled) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_test_cancelled stat;
  memset(&stat, 0, sizeof(dumpi_test_cancelled));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Test_cancelled);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Test_cancelled);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Test_cancelled);
  }
  retval = PMPI_Test_cancelled(status, cancelled);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Test_cancelled);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.cancelled, *cancelled);
    libdumpi_lock_io();
    dumpi_write_test_cancelled(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Test_cancelled);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TEST_CANCELLED */

#ifndef DUMPI_SKIP_MPI_SEND_INIT
int MPI_Send_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_send_init stat;
  memset(&stat, 0, sizeof(dumpi_send_init));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Send_init);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Send_init);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Send_init);
  }
  retval = PMPI_Send_init(buf, count, datatype, dest, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Send_init);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_send_init(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Send_init);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SEND_INIT */

#ifndef DUMPI_SKIP_MPI_BSEND_INIT
int MPI_Bsend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_bsend_init stat;
  memset(&stat, 0, sizeof(dumpi_bsend_init));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Bsend_init);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Bsend_init);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Bsend_init);
  }
  retval = PMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Bsend_init);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_bsend_init(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Bsend_init);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_BSEND_INIT */

#ifndef DUMPI_SKIP_MPI_SSEND_INIT
int MPI_Ssend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_ssend_init stat;
  memset(&stat, 0, sizeof(dumpi_ssend_init));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Ssend_init);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Ssend_init);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Ssend_init);
  }
  retval = PMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Ssend_init);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_ssend_init(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Ssend_init);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SSEND_INIT */

#ifndef DUMPI_SKIP_MPI_RSEND_INIT
int MPI_Rsend_init(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_rsend_init stat;
  memset(&stat, 0, sizeof(dumpi_rsend_init));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Rsend_init);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Rsend_init);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Rsend_init);
  }
  retval = PMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Rsend_init);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_rsend_init(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Rsend_init);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_RSEND_INIT */

#ifndef DUMPI_SKIP_MPI_RECV_INIT
int MPI_Recv_init(void *buf, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm comm, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_recv_init stat;
  memset(&stat, 0, sizeof(dumpi_recv_init));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Recv_init);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Recv_init);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_SOURCE_FROM_INT(stat.source, source);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Recv_init);
  }
  retval = PMPI_Recv_init(buf, count, datatype, source, tag, comm, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Recv_init);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_recv_init(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Recv_init);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_RECV_INIT */

#ifndef DUMPI_SKIP_MPI_START
int MPI_Start(MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_start stat;
  memset(&stat, 0, sizeof(dumpi_start));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Start);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Start);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Start);
  }
  retval = PMPI_Start(request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Start);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_start(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Start);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_START */

#ifndef DUMPI_SKIP_MPI_STARTALL
int MPI_Startall(int count, MPI_Request *requests) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_startall stat;
  memset(&stat, 0, sizeof(dumpi_startall));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Startall);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Startall);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Startall);
  }
  retval = PMPI_Startall(count, requests);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Startall);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(count, stat.requests, requests);
    libdumpi_lock_io();
    dumpi_write_startall(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(requests != NULL) DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(stat.requests);
    DUMPI_STOP_OVERHEAD(DUMPI_Startall);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_STARTALL */

#ifndef DUMPI_SKIP_MPI_SENDRECV
int MPI_Sendrecv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, int dest, int sendtag, void *recvbuf, int recvcount, MPI_Datatype recvtype, int source, int recvtag, MPI_Comm comm, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_sendrecv stat;
  memset(&stat, 0, sizeof(dumpi_sendrecv));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Sendrecv);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Sendrecv);
    DUMPI_INT_FROM_INT(stat.sendcount, sendcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.sendtag, sendtag);
    DUMPI_INT_FROM_INT(stat.recvcount, recvcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_SOURCE_FROM_INT(stat.source, source);
    DUMPI_TAG_FROM_INT(stat.recvtag, recvtag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Sendrecv);
  }
  retval = PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Sendrecv);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_sendrecv(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Sendrecv);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SENDRECV */

#ifndef DUMPI_SKIP_MPI_SENDRECV_REPLACE
int MPI_Sendrecv_replace(void *buf, int count, MPI_Datatype datatype, int dest, int sendtag, int source, int recvtag, MPI_Comm comm, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_sendrecv_replace stat;
  memset(&stat, 0, sizeof(dumpi_sendrecv_replace));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Sendrecv_replace);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Sendrecv_replace);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_DEST_FROM_INT(stat.dest, dest);
    DUMPI_TAG_FROM_INT(stat.sendtag, sendtag);
    DUMPI_SOURCE_FROM_INT(stat.source, source);
    DUMPI_TAG_FROM_INT(stat.recvtag, recvtag);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Sendrecv_replace);
  }
  retval = PMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag, source, recvtag, comm, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Sendrecv_replace);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_sendrecv_replace(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Sendrecv_replace);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SENDRECV_REPLACE */

#ifndef DUMPI_SKIP_MPI_TYPE_CONTIGUOUS
int MPI_Type_contiguous(int count, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_contiguous stat;
  memset(&stat, 0, sizeof(dumpi_type_contiguous));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_contiguous);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_contiguous);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_contiguous);
  }
  retval = PMPI_Type_contiguous(count, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_contiguous);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_contiguous(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_contiguous);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CONTIGUOUS */

#ifndef DUMPI_SKIP_MPI_TYPE_VECTOR
int MPI_Type_vector(int count, int blocklength, int stride, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_vector stat;
  memset(&stat, 0, sizeof(dumpi_type_vector));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_vector);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_vector);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT(stat.blocklength, blocklength);
    DUMPI_INT_FROM_INT(stat.stride, stride);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_vector);
  }
  retval = PMPI_Type_vector(count, blocklength, stride, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_vector);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_vector(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_vector);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_VECTOR */

#ifndef DUMPI_SKIP_MPI_TYPE_HVECTOR
int MPI_Type_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_hvector stat;
  memset(&stat, 0, sizeof(dumpi_type_hvector));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_hvector);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_hvector);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT(stat.blocklength, blocklength);
    DUMPI_INT_FROM_MPI_AINT(stat.stride, stride);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_hvector);
  }
  retval = PMPI_Type_hvector(count, blocklength, stride, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_hvector);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_hvector(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_hvector);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_HVECTOR */

#ifndef DUMPI_SKIP_MPI_TYPE_INDEXED
int MPI_Type_indexed(int count, const int *lengths, const int *indices, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_indexed stat;
  memset(&stat, 0, sizeof(dumpi_type_indexed));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_indexed);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_indexed);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.lengths, lengths);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.indices, indices);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_indexed);
  }
  retval = PMPI_Type_indexed(count, lengths, indices, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_indexed);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_indexed(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(lengths != NULL) DUMPI_FREE_INT_FROM_INT(stat.lengths);
    if(indices != NULL) DUMPI_FREE_INT_FROM_INT(stat.indices);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_indexed);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_INDEXED */

#ifndef DUMPI_SKIP_MPI_TYPE_HINDEXED
int MPI_Type_hindexed(int count, const int *lengths, const MPI_Aint *indices, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_hindexed stat;
  memset(&stat, 0, sizeof(dumpi_type_hindexed));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_hindexed);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_hindexed);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.lengths, lengths);
    DUMPI_INT_FROM_MPI_AINT_ARRAY_1(count, stat.indices, indices);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_hindexed);
  }
  retval = PMPI_Type_hindexed(count, lengths, indices, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_hindexed);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_hindexed(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(lengths != NULL) DUMPI_FREE_INT_FROM_INT(stat.lengths);
    if(indices != NULL) DUMPI_FREE_INT_FROM_MPI_AINT(stat.indices);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_hindexed);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_HINDEXED */

#ifndef DUMPI_SKIP_MPI_TYPE_STRUCT
int MPI_Type_struct(int count, const int *lengths, const MPI_Aint *indices, const MPI_Datatype *oldtypes, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_struct stat;
  memset(&stat, 0, sizeof(dumpi_type_struct));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_struct);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_struct);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.lengths, lengths);
    DUMPI_INT_FROM_MPI_AINT_ARRAY_1(count, stat.indices, indices);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_ARRAY_1(count, stat.oldtypes, oldtypes);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_struct);
  }
  retval = PMPI_Type_struct(count, lengths, indices, oldtypes, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_struct);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_struct(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(lengths != NULL) DUMPI_FREE_INT_FROM_INT(stat.lengths);
    if(oldtypes != NULL) DUMPI_FREE_DATATYPE_FROM_MPI_DATATYPE(stat.oldtypes);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_struct);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_STRUCT */

#ifndef DUMPI_SKIP_MPI_ADDRESS
int MPI_Address(const void *buf, MPI_Aint *address) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_address stat;
  memset(&stat, 0, sizeof(dumpi_address));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Address);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Address);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Address);
  }
  retval = PMPI_Address(buf, address);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Address);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.address, *address);
    libdumpi_lock_io();
    dumpi_write_address(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Address);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ADDRESS */

#ifndef DUMPI_SKIP_MPI_TYPE_EXTENT
int MPI_Type_extent(MPI_Datatype datatype, MPI_Aint *extent) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_extent stat;
  memset(&stat, 0, sizeof(dumpi_type_extent));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_extent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_extent);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_extent);
  }
  retval = PMPI_Type_extent(datatype, extent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_extent);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.extent, *extent);
    libdumpi_lock_io();
    dumpi_write_type_extent(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_extent);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_EXTENT */

#ifndef DUMPI_SKIP_MPI_TYPE_SIZE
int MPI_Type_size(MPI_Datatype datatype, int *size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_size stat;
  memset(&stat, 0, sizeof(dumpi_type_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_size);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_size);
  }
  retval = PMPI_Type_size(datatype, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_size);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.size, *size);
    libdumpi_lock_io();
    dumpi_write_type_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_SIZE */

#ifndef DUMPI_SKIP_MPI_TYPE_LB
int MPI_Type_lb(MPI_Datatype datatype, MPI_Aint *lb) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_lb stat;
  memset(&stat, 0, sizeof(dumpi_type_lb));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_lb);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_lb);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_lb);
  }
  retval = PMPI_Type_lb(datatype, lb);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_lb);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.lb, *lb);
    libdumpi_lock_io();
    dumpi_write_type_lb(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_lb);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_LB */

#ifndef DUMPI_SKIP_MPI_TYPE_UB
int MPI_Type_ub(MPI_Datatype datatype, MPI_Aint *ub) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_ub stat;
  memset(&stat, 0, sizeof(dumpi_type_ub));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_ub);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_ub);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_ub);
  }
  retval = PMPI_Type_ub(datatype, ub);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_ub);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.ub, *ub);
    libdumpi_lock_io();
    dumpi_write_type_ub(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_ub);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_UB */

#ifndef DUMPI_SKIP_MPI_TYPE_COMMIT
int MPI_Type_commit(MPI_Datatype *datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_commit stat;
  memset(&stat, 0, sizeof(dumpi_type_commit));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_commit);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_commit);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, *datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_commit);
  }
  retval = PMPI_Type_commit(datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_commit);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_type_commit(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_commit);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_COMMIT */

#ifndef DUMPI_SKIP_MPI_TYPE_FREE
int MPI_Type_free(MPI_Datatype *datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_free stat;
  memset(&stat, 0, sizeof(dumpi_type_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_free);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.datatype, *datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_free);
  }
  retval = PMPI_Type_free(datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_type_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_FREE */

#ifndef DUMPI_SKIP_MPI_GET_ELEMENTS
int MPI_Get_elements(const MPI_Status *status, MPI_Datatype datatype, int *elements) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_get_elements stat;
  memset(&stat, 0, sizeof(dumpi_get_elements));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Get_elements);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_elements);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Get_elements);
  }
  retval = PMPI_Get_elements(status, datatype, elements);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_elements);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.elements, *elements);
    libdumpi_lock_io();
    dumpi_write_get_elements(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Get_elements);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GET_ELEMENTS */

#ifndef DUMPI_SKIP_MPI_PACK
int MPI_Pack(const void *inbuf, int incount, MPI_Datatype datatype, void *outbuf, int outcount, int *position, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_pack stat;
  memset(&stat, 0, sizeof(dumpi_pack));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Pack);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Pack);
    DUMPI_INT_FROM_INT(stat.incount, incount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_INT_FROM_INT(stat.outcount, outcount);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.position.in, *position);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Pack);
  }
  retval = PMPI_Pack(inbuf, incount, datatype, outbuf, outcount, position, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Pack);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.position.out, *position);
    libdumpi_lock_io();
    dumpi_write_pack(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Pack);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_PACK */

#ifndef DUMPI_SKIP_MPI_UNPACK
int MPI_Unpack(const void *inbuf, int incount, int *position, void *outbuf, int outcount, MPI_Datatype datatype, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_unpack stat;
  memset(&stat, 0, sizeof(dumpi_unpack));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Unpack);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Unpack);
    DUMPI_INT_FROM_INT(stat.incount, incount);
    DUMPI_INT_FROM_INT(stat.outcount, outcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.position.in, *position);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Unpack);
  }
  retval = PMPI_Unpack(inbuf, incount, position, outbuf, outcount, datatype, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Unpack);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.position.out, *position);
    libdumpi_lock_io();
    dumpi_write_unpack(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Unpack);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_UNPACK */

#ifndef DUMPI_SKIP_MPI_PACK_SIZE
int MPI_Pack_size(int incount, MPI_Datatype datatype, MPI_Comm comm, int *size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_pack_size stat;
  memset(&stat, 0, sizeof(dumpi_pack_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Pack_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Pack_size);
    DUMPI_INT_FROM_INT(stat.incount, incount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Pack_size);
  }
  retval = PMPI_Pack_size(incount, datatype, comm, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Pack_size);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.size, *size);
    libdumpi_lock_io();
    dumpi_write_pack_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Pack_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_PACK_SIZE */

#ifndef DUMPI_SKIP_MPI_BARRIER
int MPI_Barrier(MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_barrier stat;
  memset(&stat, 0, sizeof(dumpi_barrier));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Barrier);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Barrier);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Barrier);
  }
  retval = PMPI_Barrier(comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Barrier);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_barrier(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Barrier);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_BARRIER */

#ifndef DUMPI_SKIP_MPI_BCAST
int MPI_Bcast(void *sendbuf, int count, MPI_Datatype datatype, int root, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_bcast stat;
  memset(&stat, 0, sizeof(dumpi_bcast));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Bcast);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Bcast);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Bcast);
  }
  retval = PMPI_Bcast(sendbuf, count, datatype, root, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Bcast);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_bcast(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Bcast);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_BCAST */

#ifndef DUMPI_SKIP_MPI_GATHER
int MPI_Gather(const void *inbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_gather stat;
  memset(&stat, 0, sizeof(dumpi_gather));
  /* Extra declarations */
  int commrank;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Gather);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Gather);
    PMPI_Comm_rank(comm, &commrank);
    DUMPI_INT_FROM_INT(stat.commrank, commrank);
    DUMPI_INT_FROM_INT(stat.sendcount, sendcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    if(commrank==root)    DUMPI_INT_FROM_INT(stat.recvcount, recvcount);
    if(commrank==root)    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Gather);
  }
  retval = PMPI_Gather(inbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Gather);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_gather(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Gather);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GATHER */

#ifndef DUMPI_SKIP_MPI_GATHERV
int MPI_Gatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *displs, MPI_Datatype recvtype, int root, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_gatherv stat;
  memset(&stat, 0, sizeof(dumpi_gatherv));
  /* Extra declarations */
  int commsize;
  int commrank;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Gatherv);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Gatherv);
    PMPI_Comm_size(comm, &commsize); PMPI_Comm_rank(comm, &commrank);
    DUMPI_INT_FROM_INT(stat.commrank, commrank);
    DUMPI_INT_FROM_INT(stat.commsize, commsize);
    DUMPI_INT_FROM_INT(stat.sendcount, sendcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    if(commrank==root)    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.recvcounts, recvcounts);
    if(commrank==root)    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.displs, displs);
    if(commrank==root)    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Gatherv);
  }
  retval = PMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Gatherv);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_gatherv(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(recvcounts != NULL) DUMPI_FREE_INT_FROM_INT(stat.recvcounts);
    if(displs != NULL) DUMPI_FREE_INT_FROM_INT(stat.displs);
    DUMPI_STOP_OVERHEAD(DUMPI_Gatherv);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GATHERV */

#ifndef DUMPI_SKIP_MPI_SCATTER
int MPI_Scatter(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_scatter stat;
  memset(&stat, 0, sizeof(dumpi_scatter));
  /* Extra declarations */
  int commrank;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Scatter);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Scatter);
    PMPI_Comm_rank(comm, &commrank);
    DUMPI_INT_FROM_INT(stat.commrank, commrank);
    if(commrank==root)    DUMPI_INT_FROM_INT(stat.sendcount, sendcount);
    if(commrank==root)    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    DUMPI_INT_FROM_INT(stat.recvcount, recvcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Scatter);
  }
  retval = PMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Scatter);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_scatter(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Scatter);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SCATTER */

#ifndef DUMPI_SKIP_MPI_SCATTERV
int MPI_Scatterv(const void *sendbuf, const int *sendcounts, const int *displs, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, int root, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_scatterv stat;
  memset(&stat, 0, sizeof(dumpi_scatterv));
  /* Extra declarations */
  int commsize;
  int commrank;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Scatterv);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Scatterv);
    PMPI_Comm_size(comm, &commsize); PMPI_Comm_rank(comm, &commrank);
    DUMPI_INT_FROM_INT(stat.commrank, commrank);
    DUMPI_INT_FROM_INT(stat.commsize, commsize);
    if(commrank==root)    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.sendcounts, sendcounts);
    if(commrank==root)    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.displs, displs);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    DUMPI_INT_FROM_INT(stat.recvcount, recvcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Scatterv);
  }
  retval = PMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Scatterv);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_scatterv(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(sendcounts != NULL) DUMPI_FREE_INT_FROM_INT(stat.sendcounts);
    if(displs != NULL) DUMPI_FREE_INT_FROM_INT(stat.displs);
    DUMPI_STOP_OVERHEAD(DUMPI_Scatterv);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SCATTERV */

#ifndef DUMPI_SKIP_MPI_ALLGATHER
int MPI_Allgather(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_allgather stat;
  memset(&stat, 0, sizeof(dumpi_allgather));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Allgather);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Allgather);
    DUMPI_INT_FROM_INT(stat.sendcount, sendcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    DUMPI_INT_FROM_INT(stat.recvcount, recvcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Allgather);
  }
  retval = PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Allgather);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_allgather(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Allgather);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ALLGATHER */

#ifndef DUMPI_SKIP_MPI_ALLGATHERV
int MPI_Allgatherv(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *displs, MPI_Datatype recvtype, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_allgatherv stat;
  memset(&stat, 0, sizeof(dumpi_allgatherv));
  /* Extra declarations */
  int commsize;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Allgatherv);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Allgatherv);
    PMPI_Comm_size(comm, &commsize);
    DUMPI_INT_FROM_INT(stat.commsize, commsize);
    DUMPI_INT_FROM_INT(stat.sendcount, sendcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.recvcounts, recvcounts);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.displs, displs);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Allgatherv);
  }
  retval = PMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Allgatherv);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_allgatherv(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(recvcounts != NULL) DUMPI_FREE_INT_FROM_INT(stat.recvcounts);
    if(displs != NULL) DUMPI_FREE_INT_FROM_INT(stat.displs);
    DUMPI_STOP_OVERHEAD(DUMPI_Allgatherv);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ALLGATHERV */

#ifndef DUMPI_SKIP_MPI_ALLTOALL
int MPI_Alltoall(const void *sendbuf, int sendcount, MPI_Datatype sendtype, void *recvbuf, int recvcount, MPI_Datatype recvtype, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_alltoall stat;
  memset(&stat, 0, sizeof(dumpi_alltoall));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Alltoall);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Alltoall);
    DUMPI_INT_FROM_INT(stat.sendcount, sendcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    DUMPI_INT_FROM_INT(stat.recvcount, recvcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Alltoall);
  }
  retval = PMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Alltoall);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_alltoall(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Alltoall);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ALLTOALL */

#ifndef DUMPI_SKIP_MPI_ALLTOALLV
int MPI_Alltoallv(const void *sendbuf, const int *sendcounts, const int *senddispls, MPI_Datatype sendtype, void *recvbuf, const int *recvcounts, const int *recvdispls, MPI_Datatype recvtype, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_alltoallv stat;
  memset(&stat, 0, sizeof(dumpi_alltoallv));
  /* Extra declarations */
  int commsize;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Alltoallv);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Alltoallv);
    PMPI_Comm_size(comm, &commsize);
    DUMPI_INT_FROM_INT(stat.commsize, commsize);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.sendcounts, sendcounts);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.senddispls, senddispls);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.sendtype, sendtype);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.recvcounts, recvcounts);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.recvdispls, recvdispls);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.recvtype, recvtype);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Alltoallv);
  }
  retval = PMPI_Alltoallv(sendbuf, sendcounts, senddispls, sendtype, recvbuf, recvcounts, recvdispls, recvtype, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Alltoallv);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_alltoallv(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(sendcounts != NULL) DUMPI_FREE_INT_FROM_INT(stat.sendcounts);
    if(senddispls != NULL) DUMPI_FREE_INT_FROM_INT(stat.senddispls);
    if(recvcounts != NULL) DUMPI_FREE_INT_FROM_INT(stat.recvcounts);
    if(recvdispls != NULL) DUMPI_FREE_INT_FROM_INT(stat.recvdispls);
    DUMPI_STOP_OVERHEAD(DUMPI_Alltoallv);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ALLTOALLV */

#ifndef DUMPI_SKIP_MPI_REDUCE
int MPI_Reduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_reduce stat;
  memset(&stat, 0, sizeof(dumpi_reduce));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Reduce);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Reduce);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_OP_FROM_MPI_OP(stat.op, op);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Reduce);
  }
  retval = PMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Reduce);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_reduce(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Reduce);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_REDUCE */

#ifndef DUMPI_SKIP_MPI_OP_CREATE
int MPI_Op_create(MPI_User_function *fptr, int commute, MPI_Op *op) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_op_create stat;
  memset(&stat, 0, sizeof(dumpi_op_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Op_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Op_create);
    DUMPI_INT_FROM_INT(stat.commute, commute);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Op_create);
  }
  retval = PMPI_Op_create(fptr, commute, op);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Op_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_OP_FROM_MPI_OP(stat.op, *op);
    libdumpi_lock_io();
    dumpi_write_op_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Op_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_OP_CREATE */

#ifndef DUMPI_SKIP_MPI_OP_FREE
int MPI_Op_free(MPI_Op *op) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_op_free stat;
  memset(&stat, 0, sizeof(dumpi_op_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Op_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Op_free);
    DUMPI_OP_FROM_MPI_OP(stat.op, *op);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Op_free);
  }
  retval = PMPI_Op_free(op);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Op_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_op_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Op_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_OP_FREE */

#ifndef DUMPI_SKIP_MPI_ALLREDUCE
int MPI_Allreduce(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_allreduce stat;
  memset(&stat, 0, sizeof(dumpi_allreduce));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Allreduce);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Allreduce);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_OP_FROM_MPI_OP(stat.op, op);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Allreduce);
  }
  retval = PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Allreduce);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_allreduce(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Allreduce);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ALLREDUCE */

#ifndef DUMPI_SKIP_MPI_REDUCE_SCATTER
int MPI_Reduce_scatter(const void *sendbuf, void *recvbuf, const int *recvcounts, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_reduce_scatter stat;
  memset(&stat, 0, sizeof(dumpi_reduce_scatter));
  /* Extra declarations */
  int commsize;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Reduce_scatter);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Reduce_scatter);
    PMPI_Comm_size(comm, &commsize);
    DUMPI_INT_FROM_INT(stat.commsize, commsize);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.recvcounts, recvcounts);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_OP_FROM_MPI_OP(stat.op, op);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Reduce_scatter);
  }
  retval = PMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Reduce_scatter);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_reduce_scatter(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(recvcounts != NULL) DUMPI_FREE_INT_FROM_INT(stat.recvcounts);
    DUMPI_STOP_OVERHEAD(DUMPI_Reduce_scatter);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_REDUCE_SCATTER */

#ifndef DUMPI_SKIP_MPI_SCAN
int MPI_Scan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_scan stat;
  memset(&stat, 0, sizeof(dumpi_scan));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Scan);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Scan);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_OP_FROM_MPI_OP(stat.op, op);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Scan);
  }
  retval = PMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Scan);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_scan(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Scan);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_SCAN */

#ifndef DUMPI_SKIP_MPI_GROUP_SIZE
int MPI_Group_size(MPI_Group group, int *size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_size stat;
  memset(&stat, 0, sizeof(dumpi_group_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_size);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_size);
  }
  retval = PMPI_Group_size(group, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_size);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.size, *size);
    libdumpi_lock_io();
    dumpi_write_group_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Group_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_SIZE */

#ifndef DUMPI_SKIP_MPI_GROUP_RANK
int MPI_Group_rank(MPI_Group group, int *rank) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_rank stat;
  memset(&stat, 0, sizeof(dumpi_group_rank));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_rank);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_rank);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_rank);
  }
  retval = PMPI_Group_rank(group, rank);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_rank);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.rank, *rank);
    libdumpi_lock_io();
    dumpi_write_group_rank(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Group_rank);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_RANK */

#ifndef DUMPI_SKIP_MPI_GROUP_TRANSLATE_RANKS
int MPI_Group_translate_ranks(MPI_Group group1, int count, const int *ranks1, MPI_Group group2, int *ranks2) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_translate_ranks stat;
  memset(&stat, 0, sizeof(dumpi_group_translate_ranks));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_translate_ranks);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_translate_ranks);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group1, group1);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.ranks1, ranks1);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group2, group2);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_translate_ranks);
  }
  retval = PMPI_Group_translate_ranks(group1, count, ranks1, group2, ranks2);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_translate_ranks);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.ranks2, ranks2);
    libdumpi_lock_io();
    dumpi_write_group_translate_ranks(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(ranks1 != NULL) DUMPI_FREE_INT_FROM_INT(stat.ranks1);
    if(ranks2 != NULL) DUMPI_FREE_INT_FROM_INT(stat.ranks2);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_translate_ranks);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_TRANSLATE_RANKS */

#ifndef DUMPI_SKIP_MPI_GROUP_COMPARE
int MPI_Group_compare(MPI_Group group1, MPI_Group group2, int *result) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_compare stat;
  memset(&stat, 0, sizeof(dumpi_group_compare));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_compare);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_compare);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group1, group1);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group2, group2);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_compare);
  }
  retval = PMPI_Group_compare(group1, group2, result);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_compare);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMPARISON_FROM_INT(stat.result, *result);
    libdumpi_lock_io();
    dumpi_write_group_compare(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Group_compare);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_COMPARE */

#ifndef DUMPI_SKIP_MPI_COMM_GROUP
int MPI_Comm_group(MPI_Comm comm, MPI_Group *group) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_group stat;
  memset(&stat, 0, sizeof(dumpi_comm_group));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_group);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_group);
  }
  retval = PMPI_Comm_group(comm, group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_group);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, *group);
    libdumpi_lock_io();
    dumpi_write_comm_group(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_group);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_GROUP */

#ifndef DUMPI_SKIP_MPI_GROUP_UNION
int MPI_Group_union(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_union stat;
  memset(&stat, 0, sizeof(dumpi_group_union));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_union);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_union);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group1, group1);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group2, group2);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_union);
  }
  retval = PMPI_Group_union(group1, group2, newgroup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_union);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.newgroup, *newgroup);
    libdumpi_lock_io();
    dumpi_write_group_union(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Group_union);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_UNION */

#ifndef DUMPI_SKIP_MPI_GROUP_INTERSECTION
int MPI_Group_intersection(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_intersection stat;
  memset(&stat, 0, sizeof(dumpi_group_intersection));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_intersection);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_intersection);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group1, group1);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group2, group2);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_intersection);
  }
  retval = PMPI_Group_intersection(group1, group2, newgroup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_intersection);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.newgroup, *newgroup);
    libdumpi_lock_io();
    dumpi_write_group_intersection(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Group_intersection);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_INTERSECTION */

#ifndef DUMPI_SKIP_MPI_GROUP_DIFFERENCE
int MPI_Group_difference(MPI_Group group1, MPI_Group group2, MPI_Group *newgroup) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_difference stat;
  memset(&stat, 0, sizeof(dumpi_group_difference));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_difference);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_difference);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group1, group1);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group2, group2);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_difference);
  }
  retval = PMPI_Group_difference(group1, group2, newgroup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_difference);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.newgroup, *newgroup);
    libdumpi_lock_io();
    dumpi_write_group_difference(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Group_difference);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_DIFFERENCE */

#ifndef DUMPI_SKIP_MPI_GROUP_INCL
int MPI_Group_incl(MPI_Group group, int count, const int *ranks, MPI_Group *newgroup) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_incl stat;
  memset(&stat, 0, sizeof(dumpi_group_incl));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_incl);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_incl);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.ranks, ranks);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_incl);
  }
  retval = PMPI_Group_incl(group, count, ranks, newgroup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_incl);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.newgroup, *newgroup);
    libdumpi_lock_io();
    dumpi_write_group_incl(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(ranks != NULL) DUMPI_FREE_INT_FROM_INT(stat.ranks);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_incl);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_INCL */

#ifndef DUMPI_SKIP_MPI_GROUP_EXCL
int MPI_Group_excl(MPI_Group group, int count, const int *ranks, MPI_Group *newgroup) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_excl stat;
  memset(&stat, 0, sizeof(dumpi_group_excl));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_excl);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_excl);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.ranks, ranks);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_excl);
  }
  retval = PMPI_Group_excl(group, count, ranks, newgroup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_excl);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.newgroup, *newgroup);
    libdumpi_lock_io();
    dumpi_write_group_excl(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(ranks != NULL) DUMPI_FREE_INT_FROM_INT(stat.ranks);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_excl);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_EXCL */

#ifndef DUMPI_SKIP_MPI_GROUP_RANGE_INCL
int MPI_Group_range_incl(MPI_Group group, int count, int ranges[][3], MPI_Group *newgroup) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_range_incl stat;
  memset(&stat, 0, sizeof(dumpi_group_range_incl));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_range_incl);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_range_incl);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_2_FIXBOUND(count, 3, stat.ranges, ranges);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_range_incl);
  }
  retval = PMPI_Group_range_incl(group, count, ranges, newgroup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_range_incl);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.newgroup, *newgroup);
    libdumpi_lock_io();
    dumpi_write_group_range_incl(&stat, thread, &cpu, &wall, dumpi_global->perf,
				 dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(ranges != NULL)
      DUMPI_FREE_INT_FROM_INT_ARRAY_2_FIXBOUND(count, stat.ranges);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_range_incl);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_RANGE_INCL */

#ifndef DUMPI_SKIP_MPI_GROUP_RANGE_EXCL
int MPI_Group_range_excl(MPI_Group group, int count, int ranges[][3], MPI_Group *newgroup) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_range_excl stat;
  memset(&stat, 0, sizeof(dumpi_group_range_excl));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_range_excl);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_range_excl);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_2_FIXBOUND(count, 3, stat.ranges, ranges);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_range_excl);
  }
  retval = PMPI_Group_range_excl(group, count, ranges, newgroup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_range_excl);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.newgroup, *newgroup);
    libdumpi_lock_io();
    dumpi_write_group_range_excl(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(ranges != NULL)
      DUMPI_FREE_INT_FROM_INT_ARRAY_2_FIXBOUND(count, stat.ranges);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_range_excl);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_RANGE_EXCL */

#ifndef DUMPI_SKIP_MPI_GROUP_FREE
int MPI_Group_free(MPI_Group *group) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_group_free stat;
  memset(&stat, 0, sizeof(dumpi_group_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Group_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_free);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, *group);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Group_free);
  }
  retval = PMPI_Group_free(group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Group_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_group_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Group_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GROUP_FREE */

#ifndef DUMPI_SKIP_MPI_COMM_SIZE
int MPI_Comm_size(MPI_Comm comm, int *size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_size stat;
  memset(&stat, 0, sizeof(dumpi_comm_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_size);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_size);
  }
  retval = PMPI_Comm_size(comm, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_size);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.size, *size);
    libdumpi_lock_io();
    dumpi_write_comm_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_SIZE */

#ifndef DUMPI_SKIP_MPI_COMM_RANK
int MPI_Comm_rank(MPI_Comm comm, int *rank) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_rank stat;
  memset(&stat, 0, sizeof(dumpi_comm_rank));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_rank);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_rank);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_rank);
  }
  retval = PMPI_Comm_rank(comm, rank);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_rank);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.rank, *rank);
    libdumpi_lock_io();
    dumpi_write_comm_rank(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_rank);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_RANK */

#ifndef DUMPI_SKIP_MPI_COMM_COMPARE
int MPI_Comm_compare(MPI_Comm comm1, MPI_Comm comm2, int *result) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_compare stat;
  memset(&stat, 0, sizeof(dumpi_comm_compare));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_compare);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_compare);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm1, comm1);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm2, comm2);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_compare);
  }
  retval = PMPI_Comm_compare(comm1, comm2, result);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_compare);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMPARISON_FROM_INT(stat.result, *result);
    libdumpi_lock_io();
    dumpi_write_comm_compare(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_compare);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_COMPARE */

#ifndef DUMPI_SKIP_MPI_COMM_DUP
int MPI_Comm_dup(MPI_Comm oldcomm, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_dup stat;
  memset(&stat, 0, sizeof(dumpi_comm_dup));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_dup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_dup);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_dup);
  }
  retval = PMPI_Comm_dup(oldcomm, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_dup);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_comm_dup(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_dup);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_DUP */

#ifndef DUMPI_SKIP_MPI_COMM_CREATE
int MPI_Comm_create(MPI_Comm oldcomm, MPI_Group group, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_create stat;
  memset(&stat, 0, sizeof(dumpi_comm_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_create);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_create);
  }
  retval = PMPI_Comm_create(oldcomm, group, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_comm_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_CREATE */

#ifndef DUMPI_SKIP_MPI_COMM_SPLIT
int MPI_Comm_split(MPI_Comm oldcomm, int color, int key, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_split stat;
  memset(&stat, 0, sizeof(dumpi_comm_split));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_split);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_split);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_INT_FROM_INT(stat.color, color);
    DUMPI_INT_FROM_INT(stat.key, key);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_split);
  }
  retval = PMPI_Comm_split(oldcomm, color, key, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_split);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_comm_split(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_split);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_SPLIT */

#ifndef DUMPI_SKIP_MPI_COMM_FREE
int MPI_Comm_free(MPI_Comm *comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_free stat;
  memset(&stat, 0, sizeof(dumpi_comm_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_free);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, *comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_free);
  }
  retval = PMPI_Comm_free(comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_comm_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_FREE */

#ifndef DUMPI_SKIP_MPI_COMM_TEST_INTER
int MPI_Comm_test_inter(MPI_Comm comm, int *inter) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_test_inter stat;
  memset(&stat, 0, sizeof(dumpi_comm_test_inter));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_test_inter);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_test_inter);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_test_inter);
  }
  retval = PMPI_Comm_test_inter(comm, inter);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_test_inter);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.inter, *inter);
    libdumpi_lock_io();
    dumpi_write_comm_test_inter(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_test_inter);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_TEST_INTER */

#ifndef DUMPI_SKIP_MPI_COMM_REMOTE_SIZE
int MPI_Comm_remote_size(MPI_Comm comm, int *size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_remote_size stat;
  memset(&stat, 0, sizeof(dumpi_comm_remote_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_remote_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_remote_size);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_remote_size);
  }
  retval = PMPI_Comm_remote_size(comm, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_remote_size);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.size, *size);
    libdumpi_lock_io();
    dumpi_write_comm_remote_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_remote_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_REMOTE_SIZE */

#ifndef DUMPI_SKIP_MPI_COMM_REMOTE_GROUP
int MPI_Comm_remote_group(MPI_Comm comm, MPI_Group *group) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_remote_group stat;
  memset(&stat, 0, sizeof(dumpi_comm_remote_group));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_remote_group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_remote_group);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_remote_group);
  }
  retval = PMPI_Comm_remote_group(comm, group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_remote_group);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, *group);
    libdumpi_lock_io();
    dumpi_write_comm_remote_group(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_remote_group);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_REMOTE_GROUP */

#ifndef DUMPI_SKIP_MPI_INTERCOMM_CREATE
int MPI_Intercomm_create(MPI_Comm localcomm, int localleader, MPI_Comm remotecomm, int remoteleader, int tag, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_intercomm_create stat;
  memset(&stat, 0, sizeof(dumpi_intercomm_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Intercomm_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Intercomm_create);
    DUMPI_COMM_FROM_MPI_COMM(stat.localcomm, localcomm);
    DUMPI_INT_FROM_INT(stat.localleader, localleader);
    DUMPI_COMM_FROM_MPI_COMM(stat.remotecomm, remotecomm);
    DUMPI_INT_FROM_INT(stat.remoteleader, remoteleader);
    DUMPI_TAG_FROM_INT(stat.tag, tag);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Intercomm_create);
  }
  retval = PMPI_Intercomm_create(localcomm, localleader, remotecomm, remoteleader, tag, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Intercomm_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_intercomm_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Intercomm_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INTERCOMM_CREATE */

#ifndef DUMPI_SKIP_MPI_INTERCOMM_MERGE
int MPI_Intercomm_merge(MPI_Comm comm, int high, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_intercomm_merge stat;
  memset(&stat, 0, sizeof(dumpi_intercomm_merge));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Intercomm_merge);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Intercomm_merge);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.high, high);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Intercomm_merge);
  }
  retval = PMPI_Intercomm_merge(comm, high, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Intercomm_merge);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_intercomm_merge(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Intercomm_merge);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INTERCOMM_MERGE */

#ifndef DUMPI_SKIP_MPI_KEYVAL_CREATE
int MPI_Keyval_create(MPI_Copy_function *copyfunc, MPI_Delete_function *delfunc, int *key, void *state) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_keyval_create stat;
  memset(&stat, 0, sizeof(dumpi_keyval_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Keyval_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Keyval_create);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Keyval_create);
  }
  retval = PMPI_Keyval_create(copyfunc, delfunc, key, state);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Keyval_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_KEYVAL_FROM_INT(stat.key, *key);
    libdumpi_lock_io();
    dumpi_write_keyval_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Keyval_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_KEYVAL_CREATE */

#ifndef DUMPI_SKIP_MPI_KEYVAL_FREE
int MPI_Keyval_free(int *key) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_keyval_free stat;
  memset(&stat, 0, sizeof(dumpi_keyval_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Keyval_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Keyval_free);
    DUMPI_KEYVAL_FROM_INT(stat.key, *key);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Keyval_free);
  }
  retval = PMPI_Keyval_free(key);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Keyval_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_keyval_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Keyval_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_KEYVAL_FREE */

#ifndef DUMPI_SKIP_MPI_ATTR_PUT
int MPI_Attr_put(MPI_Comm comm, int key, void *attr) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_attr_put stat;
  memset(&stat, 0, sizeof(dumpi_attr_put));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Attr_put);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Attr_put);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.key, key);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Attr_put);
  }
  retval = PMPI_Attr_put(comm, key, attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Attr_put);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_attr_put(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Attr_put);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ATTR_PUT */

#ifndef DUMPI_SKIP_MPI_ATTR_GET
int MPI_Attr_get(MPI_Comm comm, int key, void *attr, int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_attr_get stat;
  memset(&stat, 0, sizeof(dumpi_attr_get));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Attr_get);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Attr_get);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.key, key);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Attr_get);
  }
  retval = PMPI_Attr_get(comm, key, attr, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Attr_get);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_attr_get(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Attr_get);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ATTR_GET */

#ifndef DUMPI_SKIP_MPI_ATTR_DELETE
int MPI_Attr_delete(MPI_Comm comm, int key) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_attr_delete stat;
  memset(&stat, 0, sizeof(dumpi_attr_delete));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Attr_delete);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Attr_delete);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.key, key);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Attr_delete);
  }
  retval = PMPI_Attr_delete(comm, key);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Attr_delete);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_attr_delete(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Attr_delete);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ATTR_DELETE */

#ifndef DUMPI_SKIP_MPI_TOPO_TEST
int MPI_Topo_test(MPI_Comm comm, int *topo) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_topo_test stat;
  memset(&stat, 0, sizeof(dumpi_topo_test));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Topo_test);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Topo_test);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Topo_test);
  }
  retval = PMPI_Topo_test(comm, topo);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Topo_test);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_TOPOLOGY_FROM_INT(stat.topo, *topo);
    libdumpi_lock_io();
    dumpi_write_topo_test(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Topo_test);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TOPO_TEST */

#ifndef DUMPI_SKIP_MPI_CART_CREATE
int MPI_Cart_create(MPI_Comm oldcomm, int ndim, const int *dims, const int *periods, int reorder, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cart_create stat;
  memset(&stat, 0, sizeof(dumpi_cart_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cart_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_create);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_INT_FROM_INT(stat.ndim, ndim);
    DUMPI_INT_FROM_INT_ARRAY_1(ndim, stat.dims, dims);
    DUMPI_INT_FROM_INT_ARRAY_1(ndim, stat.periods, periods);
    DUMPI_INT_FROM_INT(stat.reorder, reorder);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_create);
  }
  retval = PMPI_Cart_create(oldcomm, ndim, dims, periods, reorder, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_cart_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(dims != NULL) DUMPI_FREE_INT_FROM_INT(stat.dims);
    if(periods != NULL) DUMPI_FREE_INT_FROM_INT(stat.periods);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CART_CREATE */

#ifndef DUMPI_SKIP_MPI_DIMS_CREATE
int MPI_Dims_create(int nodes, int ndim, int *dims) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_dims_create stat;
  memset(&stat, 0, sizeof(dumpi_dims_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Dims_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Dims_create);
    DUMPI_INT_FROM_INT(stat.nodes, nodes);
    DUMPI_INT_FROM_INT(stat.ndim, ndim);
    DUMPI_INT_FROM_INT_ARRAY_1(ndim, stat.dims.in, dims);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Dims_create);
  }
  retval = PMPI_Dims_create(nodes, ndim, dims);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Dims_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT_ARRAY_1(ndim, stat.dims.out, dims);
    libdumpi_lock_io();
    dumpi_write_dims_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(dims != NULL) DUMPI_FREE_INT_FROM_INT(stat.dims);
    DUMPI_STOP_OVERHEAD(DUMPI_Dims_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_DIMS_CREATE */

#ifndef DUMPI_SKIP_MPI_GRAPH_CREATE
int MPI_Graph_create(MPI_Comm oldcomm, int nodes, const int *index, const int *edges, int reorder, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_graph_create stat;
  memset(&stat, 0, sizeof(dumpi_graph_create));
  /* Extra declarations */
  int numedges = edges[nodes-1];
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Graph_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_create);
    DUMPI_INT_FROM_INT(stat.numedges, numedges);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_INT_FROM_INT(stat.nodes, nodes);
    DUMPI_INT_FROM_INT_ARRAY_1(nodes, stat.index, index);
    DUMPI_INT_FROM_INT_ARRAY_1(numedges, stat.edges, edges);
    DUMPI_INT_FROM_INT(stat.reorder, reorder);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_create);
  }
  retval = PMPI_Graph_create(oldcomm, nodes, index, edges, reorder, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_graph_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(index != NULL) DUMPI_FREE_INT_FROM_INT(stat.index);
    if(edges != NULL) DUMPI_FREE_INT_FROM_INT(stat.edges);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GRAPH_CREATE */

#ifndef DUMPI_SKIP_MPI_GRAPHDIMS_GET
int MPI_Graphdims_get(MPI_Comm comm, int *nodes, int *edges) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_graphdims_get stat;
  memset(&stat, 0, sizeof(dumpi_graphdims_get));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Graphdims_get);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graphdims_get);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Graphdims_get);
  }
  retval = PMPI_Graphdims_get(comm, nodes, edges);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graphdims_get);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.nodes, *nodes);
    DUMPI_INT_FROM_INT(stat.edges, *edges);
    libdumpi_lock_io();
    dumpi_write_graphdims_get(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Graphdims_get);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GRAPHDIMS_GET */

#ifndef DUMPI_SKIP_MPI_GRAPH_GET
int MPI_Graph_get(MPI_Comm comm, int maxindex, int maxedges, int *index, int *edges) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_graph_get stat;
  memset(&stat, 0, sizeof(dumpi_graph_get));
  /* Extra declarations */
  int totnodes;
  int totedges;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Graph_get);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_get);
    PMPI_Graphdims_get(comm, &totnodes, &totedges);
    DUMPI_INT_FROM_INT(stat.totedges, MIN(maxedges, totedges));
    DUMPI_INT_FROM_INT(stat.totnodes, MIN(maxindex, totnodes));
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.maxindex, maxindex);
    DUMPI_INT_FROM_INT(stat.maxedges, maxedges);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_get);
  }
  retval = PMPI_Graph_get(comm, maxindex, maxedges, index, edges);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_get);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT_ARRAY_1(MIN(maxindex, totnodes), stat.index, index);
    DUMPI_INT_FROM_INT_ARRAY_1(MIN(maxedges, totedges), stat.edges, edges);
    libdumpi_lock_io();
    dumpi_write_graph_get(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(index != NULL) DUMPI_FREE_INT_FROM_INT(stat.index);
    if(edges != NULL) DUMPI_FREE_INT_FROM_INT(stat.edges);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_get);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GRAPH_GET */

#ifndef DUMPI_SKIP_MPI_CARTDIM_GET
int MPI_Cartdim_get(MPI_Comm comm, int *ndim) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cartdim_get stat;
  memset(&stat, 0, sizeof(dumpi_cartdim_get));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cartdim_get);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cartdim_get);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cartdim_get);
  }
  retval = PMPI_Cartdim_get(comm, ndim);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cartdim_get);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.ndim, *ndim);
    libdumpi_lock_io();
    dumpi_write_cartdim_get(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Cartdim_get);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CARTDIM_GET */

#ifndef DUMPI_SKIP_MPI_CART_GET
int MPI_Cart_get(MPI_Comm comm, int maxdims, int *dims, int *periods, int *coords) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cart_get stat;
  memset(&stat, 0, sizeof(dumpi_cart_get));
  /* Extra declarations */
  int ndim;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cart_get);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_get);
    PMPI_Cartdim_get(comm, &ndim);
    DUMPI_INT_FROM_INT(stat.ndim, MIN(maxdims, ndim));
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.maxdims, maxdims);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_get);
  }
  retval = PMPI_Cart_get(comm, maxdims, dims, periods, coords);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_get);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT_ARRAY_1(MIN(maxdims, ndim), stat.dims, dims);
    DUMPI_INT_FROM_INT_ARRAY_1(MIN(maxdims, ndim), stat.periods, periods);
    DUMPI_INT_FROM_INT_ARRAY_1(MIN(maxdis, ndim), stat.coords, coords);
    libdumpi_lock_io();
    dumpi_write_cart_get(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(stat.dims != NULL) DUMPI_FREE_INT_FROM_INT(stat.dims);
    if(stat.periods != NULL) DUMPI_FREE_INT_FROM_INT(stat.periods);
    if(stat.coords != NULL) DUMPI_FREE_INT_FROM_INT(stat.coords);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_get);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CART_GET */

#ifndef DUMPI_SKIP_MPI_CART_RANK
int MPI_Cart_rank(MPI_Comm comm, const int *coords, int *rank) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cart_rank stat;
  memset(&stat, 0, sizeof(dumpi_cart_rank));
  /* Extra declarations */
  int ndim;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cart_rank);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_rank);
    PMPI_Cartdim_get(comm, &ndim);
    DUMPI_INT_FROM_INT(stat.ndim, ndim);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT_ARRAY_1(ndim, stat.coords, coords);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_rank);
  }
  retval = PMPI_Cart_rank(comm, coords, rank);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_rank);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.rank, *rank);
    libdumpi_lock_io();
    dumpi_write_cart_rank(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(coords != NULL) DUMPI_FREE_INT_FROM_INT(stat.coords);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_rank);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CART_RANK */

#ifndef DUMPI_SKIP_MPI_CART_COORDS
int MPI_Cart_coords(MPI_Comm comm, int rank, int maxdims, int *coords) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cart_coords stat;
  memset(&stat, 0, sizeof(dumpi_cart_coords));
  /* Extra declarations */
  int ndim;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cart_coords);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_coords);
    PMPI_Cartdim_get(comm, &ndim);
    DUMPI_INT_FROM_INT(stat.ndim, MIN(maxdims, ndim));
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.rank, rank);
    DUMPI_INT_FROM_INT(stat.maxdims, maxdims);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_coords);
  }
  retval = PMPI_Cart_coords(comm, rank, maxdims, coords);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_coords);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT_ARRAY_1(MIN(maxdims, ndim), stat.coords, coords);
    libdumpi_lock_io();
    dumpi_write_cart_coords(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(coords != NULL) DUMPI_FREE_INT_FROM_INT(stat.coords);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_coords);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CART_COORDS */

#ifndef DUMPI_SKIP_MPI_GRAPH_NEIGHBORS_COUNT
int MPI_Graph_neighbors_count(MPI_Comm comm, int rank, int *nneigh) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_graph_neighbors_count stat;
  memset(&stat, 0, sizeof(dumpi_graph_neighbors_count));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Graph_neighbors_count);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_neighbors_count);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.rank, rank);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_neighbors_count);
  }
  retval = PMPI_Graph_neighbors_count(comm, rank, nneigh);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_neighbors_count);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.nneigh, *nneigh);
    libdumpi_lock_io();
    dumpi_write_graph_neighbors_count(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_neighbors_count);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GRAPH_NEIGHBORS_COUNT */

#ifndef DUMPI_SKIP_MPI_GRAPH_NEIGHBORS
int MPI_Graph_neighbors(MPI_Comm comm, int rank, int maxneighbors, int *neighbors) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_graph_neighbors stat;
  memset(&stat, 0, sizeof(dumpi_graph_neighbors));
  /* Extra declarations */
  int nneigh;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Graph_neighbors);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_neighbors);
    PMPI_Graph_neighbors_count(comm, rank, &nneigh);
    DUMPI_INT_FROM_INT(stat.nneigh, MIN(maxneighbors, nneigh));
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.rank, rank);
    DUMPI_INT_FROM_INT(stat.maxneighbors, maxneighbors);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_neighbors);
  }
  retval = PMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_neighbors);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT_ARRAY_1(MIN(maxneighbors, nneigh), stat.neighbors, neighbors);
    libdumpi_lock_io();
    dumpi_write_graph_neighbors(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(neighbors != NULL) DUMPI_FREE_INT_FROM_INT(stat.neighbors);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_neighbors);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GRAPH_NEIGHBORS */

#ifndef DUMPI_SKIP_MPI_CART_SHIFT
int MPI_Cart_shift(MPI_Comm comm, int direction, int displ, int *source, int *dest) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cart_shift stat;
  memset(&stat, 0, sizeof(dumpi_cart_shift));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cart_shift);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_shift);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.direction, direction);
    DUMPI_INT_FROM_INT(stat.displ, displ);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_shift);
  }
  retval = PMPI_Cart_shift(comm, direction, displ, source, dest);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_shift);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_SOURCE_FROM_INT(stat.source, *source);
    DUMPI_DEST_FROM_INT(stat.dest, *dest);
    libdumpi_lock_io();
    dumpi_write_cart_shift(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_shift);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CART_SHIFT */

#ifndef DUMPI_SKIP_MPI_CART_SUB
int MPI_Cart_sub(MPI_Comm oldcomm, const int *remain_dims, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cart_sub stat;
  memset(&stat, 0, sizeof(dumpi_cart_sub));
  /* Extra declarations */
  int ndim;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cart_sub);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_sub);
    PMPI_Cartdim_get(oldcomm, &ndim);
    DUMPI_INT_FROM_INT(stat.ndim, ndim);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_INT_FROM_INT_ARRAY_1(ndim, stat.remain_dims, remain_dims);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_sub);
  }
  retval = PMPI_Cart_sub(oldcomm, remain_dims, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_sub);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_cart_sub(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(remain_dims != NULL) DUMPI_FREE_INT_FROM_INT(stat.remain_dims);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_sub);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CART_SUB */

#ifndef DUMPI_SKIP_MPI_CART_MAP
int MPI_Cart_map(MPI_Comm comm, int ndim, const int *dims, const int *period, int *newrank) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_cart_map stat;
  memset(&stat, 0, sizeof(dumpi_cart_map));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Cart_map);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_map);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.ndim, ndim);
    DUMPI_INT_FROM_INT_ARRAY_1(ndim, stat.dims, dims);
    DUMPI_INT_FROM_INT_ARRAY_1(ndim, stat.period, period);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_map);
  }
  retval = PMPI_Cart_map(comm, ndim, dims, period, newrank);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Cart_map);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.newrank, *newrank);
    libdumpi_lock_io();
    dumpi_write_cart_map(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(dims != NULL) DUMPI_FREE_INT_FROM_INT(stat.dims);
    if(period != NULL) DUMPI_FREE_INT_FROM_INT(stat.period);
    DUMPI_STOP_OVERHEAD(DUMPI_Cart_map);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CART_MAP */

#ifndef DUMPI_SKIP_MPI_GRAPH_MAP
int MPI_Graph_map(MPI_Comm comm, int nodes, const int *index, const int *edges, int *newrank) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_graph_map stat;
  memset(&stat, 0, sizeof(dumpi_graph_map));
  /* Extra declarations */
  int numedges;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Graph_map);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_map);
    numedges = edges[nodes-1];
    DUMPI_INT_FROM_INT(stat.numedges, numedges);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.nodes, nodes);
    DUMPI_INT_FROM_INT_ARRAY_1(nodes, stat.index, index);
    DUMPI_INT_FROM_INT_ARRAY_1(numedges, stat.edges, edges);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_map);
  }
  retval = PMPI_Graph_map(comm, nodes, index, edges, newrank);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Graph_map);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.newrank, *newrank);
    libdumpi_lock_io();
    dumpi_write_graph_map(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(index != NULL) DUMPI_FREE_INT_FROM_INT(stat.index);
    if(edges != NULL) DUMPI_FREE_INT_FROM_INT(stat.edges);
    DUMPI_STOP_OVERHEAD(DUMPI_Graph_map);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GRAPH_MAP */

#ifndef DUMPI_SKIP_MPI_GET_PROCESSOR_NAME
int MPI_Get_processor_name(char *name, int *resultlen) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_get_processor_name stat;
  memset(&stat, 0, sizeof(dumpi_get_processor_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Get_processor_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_processor_name);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Get_processor_name);
  }
  retval = PMPI_Get_processor_name(name, resultlen);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_processor_name);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(*resultlen, stat.name, name);
    DUMPI_INT_FROM_INT(stat.resultlen, *resultlen);
    libdumpi_lock_io();
    dumpi_write_get_processor_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(name != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.name);
    DUMPI_STOP_OVERHEAD(DUMPI_Get_processor_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GET_PROCESSOR_NAME */

#ifndef DUMPI_SKIP_MPI_GET_VERSION
int MPI_Get_version(int *version, int *subversion) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_get_version stat;
  memset(&stat, 0, sizeof(dumpi_get_version));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Get_version);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_version);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Get_version);
  }
  retval = PMPI_Get_version(version, subversion);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_version);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.version, *version);
    DUMPI_INT_FROM_INT(stat.subversion, *subversion);
    libdumpi_lock_io();
    dumpi_write_get_version(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Get_version);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GET_VERSION */

#ifndef DUMPI_SKIP_MPI_ERRHANDLER_CREATE
int MPI_Errhandler_create(MPI_Handler_function *function, MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_errhandler_create stat;
  memset(&stat, 0, sizeof(dumpi_errhandler_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Errhandler_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Errhandler_create);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Errhandler_create);
  }
  retval = PMPI_Errhandler_create(function, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Errhandler_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    libdumpi_lock_io();
    dumpi_write_errhandler_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Errhandler_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ERRHANDLER_CREATE */

#ifndef DUMPI_SKIP_MPI_ERRHANDLER_SET
int MPI_Errhandler_set(MPI_Comm comm, MPI_Errhandler errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_errhandler_set stat;
  memset(&stat, 0, sizeof(dumpi_errhandler_set));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Errhandler_set);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Errhandler_set);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, errhandler);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Errhandler_set);
  }
  retval = PMPI_Errhandler_set(comm, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Errhandler_set);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_errhandler_set(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Errhandler_set);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ERRHANDLER_SET */

#ifndef DUMPI_SKIP_MPI_ERRHANDLER_GET
int MPI_Errhandler_get(MPI_Comm comm, MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_errhandler_get stat;
  memset(&stat, 0, sizeof(dumpi_errhandler_get));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Errhandler_get);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Errhandler_get);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Errhandler_get);
  }
  retval = PMPI_Errhandler_get(comm, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Errhandler_get);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    libdumpi_lock_io();
    dumpi_write_errhandler_get(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Errhandler_get);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ERRHANDLER_GET */

#ifndef DUMPI_SKIP_MPI_ERRHANDLER_FREE
int MPI_Errhandler_free(MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_errhandler_free stat;
  memset(&stat, 0, sizeof(dumpi_errhandler_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Errhandler_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Errhandler_free);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Errhandler_free);
  }
  retval = PMPI_Errhandler_free(errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Errhandler_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_errhandler_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Errhandler_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ERRHANDLER_FREE */

#ifndef DUMPI_SKIP_MPI_ERROR_STRING
int MPI_Error_string(int errorcode, char *errorstring, int *resultlen) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_error_string stat;
  memset(&stat, 0, sizeof(dumpi_error_string));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Error_string);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Error_string);
    DUMPI_INT_FROM_INT(stat.errorcode, errorcode);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Error_string);
  }
  retval = PMPI_Error_string(errorcode, errorstring, resultlen);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Error_string);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(*resultlen, stat.errorstring, errorstring);
    DUMPI_INT_FROM_INT(stat.resultlen, *resultlen);
    libdumpi_lock_io();
    dumpi_write_error_string(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(errorstring != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.errorstring);
    DUMPI_STOP_OVERHEAD(DUMPI_Error_string);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ERROR_STRING */

#ifndef DUMPI_SKIP_MPI_ERROR_CLASS
int MPI_Error_class(int errorcode, int *errorclass) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_error_class stat;
  memset(&stat, 0, sizeof(dumpi_error_class));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Error_class);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Error_class);
    DUMPI_INT_FROM_INT(stat.errorcode, errorcode);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Error_class);
  }
  retval = PMPI_Error_class(errorcode, errorclass);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Error_class);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.errorclass, *errorclass);
    libdumpi_lock_io();
    dumpi_write_error_class(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Error_class);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ERROR_CLASS */

#ifndef DUMPI_SKIP_MPI_WTIME
double MPI_Wtime(void ) {
  int16_t thread;
  dumpi_time cpu, wall;
  double retval;
  dumpi_wtime stat;
  memset(&stat, 0, sizeof(dumpi_wtime));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Wtime);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Wtime);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Wtime);
  }
  retval = PMPI_Wtime();
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Wtime);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_wtime(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Wtime);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WTIME */

#ifndef DUMPI_SKIP_MPI_WTICK
double MPI_Wtick(void ) {
  int16_t thread;
  dumpi_time cpu, wall;
  double retval;
  dumpi_wtick stat;
  memset(&stat, 0, sizeof(dumpi_wtick));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Wtick);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Wtick);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Wtick);
  }
  retval = PMPI_Wtick();
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Wtick);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_wtick(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Wtick);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WTICK */

#ifndef DUMPI_SKIP_MPI_INIT
int MPI_Init(int *argc, char ***argv) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_init stat;
  memset(&stat, 0, sizeof(dumpi_init));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Init);
  if(dumpi_debug & DUMPI_DEBUG_MPICALLS)
    fprintf(stderr, "[DUMPI-MPI] MPI_Init preamble complete.  profiling=%d\n",
	    profiling);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Init);
    if(argc) {
      DUMPI_INT_FROM_INT(stat.argc, *argc);
    }
    else {
      stat.argc = 0;
    }
    if(argv && *argv) {
      DUMPI_CHAR_FROM_CHAR_ARRAY_2(argc, DUMPI_CSTRING, stat.argv, *argv);
    }
    else {
      stat.argc = 0;
      stat.argv = NULL;
    }
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Init);
  }
  if(dumpi_debug & DUMPI_DEBUG_MPICALLS) {
    fprintf(stderr, "[DUMPI-MPI] Calling PMPI_Init(%d, %p)\n",
            (argc ? *argc : 0), (argv ? *argv : NULL));
  }
  retval = PMPI_Init(argc, argv); 
  /* Init is not always a barrier - make sure EVERYONE has started
       to avoid recording absurdly long launch delays as compute time */
  if (retval == MPI_SUCCESS) PMPI_Barrier(MPI_COMM_WORLD);
  if(dumpi_debug & DUMPI_DEBUG_MPICALLS) {
    fprintf(stderr, "[DUMPI-MPI] PMPI_Init returned %d (%s)\n", retval,
	    (retval==MPI_SUCCESS ? "success" : "failure"));
  }
  /* PMPI_Init is tagged as an INITIAL call */
  if(retval == MPI_SUCCESS) {
    libdumpi_open_files();
    libdumpi_init_maps();
    register_builtin_datatypes();
    DUMPI_START_TIME(cpu, wall); /* We cheat on timings for INIT. */
    if(profiling) {
      DUMPI_START_OVERHEAD(DUMPI_Init);
      DUMPI_STOP_TIME(cpu, wall);
      libdumpi_lock_io();
      dumpi_write_init(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
      libdumpi_unlock_io();
      if(argv != NULL) {
	DUMPI_FREE_CHAR_FROM_CHAR_ARRAY_2(argc, stat.argv);
      }
      DUMPI_STOP_OVERHEAD(DUMPI_Init);
    }
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INIT */

#ifndef DUMPI_SKIP_MPI_FINALIZE
int MPI_Finalize(void ) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_finalize stat;
  memset(&stat, 0, sizeof(dumpi_finalize));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Finalize);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Finalize);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Finalize);
  }
  
  retval = PMPI_Finalize();
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Finalize);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_finalize(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Finalize);
  }
  DUMPI_INSERT_POSTAMBLE;
  /* PMPI_Finalize is tagged as a FINAL call */
#ifndef DUMPI_ENABLE_INSTRUMENTATION
  libdumpi_finalize();
#endif
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FINALIZE */

#ifndef DUMPI_SKIP_MPI_INITIALIZED
int MPI_Initialized(int *result) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_initialized stat;
  memset(&stat, 0, sizeof(dumpi_initialized));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Initialized);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Initialized);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Initialized);
  }
  retval = PMPI_Initialized(result);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Initialized);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.result, *result);
    libdumpi_lock_io();
    dumpi_write_initialized(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Initialized);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INITIALIZED */

#ifndef DUMPI_SKIP_MPI_ABORT
int MPI_Abort(MPI_Comm comm, int errorcode) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_abort stat;
  memset(&stat, 0, sizeof(dumpi_abort));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Abort);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Abort);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.errorcode, errorcode);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Abort);
  }
  /* PMPI_Abort is tagged as a NORETURN call */
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Abort);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_abort(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Abort);
  }
  libdumpi_finalize();
  retval = PMPI_Abort(comm, errorcode);
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ABORT */

#ifndef DUMPI_SKIP_MPI_CLOSE_PORT
int MPI_Close_port(const char *portname) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_close_port stat;
  memset(&stat, 0, sizeof(dumpi_close_port));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Close_port);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Close_port);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.portname, portname);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Close_port);
  }
  retval = PMPI_Close_port(portname);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Close_port);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_close_port(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(portname != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.portname);
    DUMPI_STOP_OVERHEAD(DUMPI_Close_port);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_CLOSE_PORT */

#ifndef DUMPI_SKIP_MPI_COMM_ACCEPT
int MPI_Comm_accept(const char *portname, MPI_Info info, int root, MPI_Comm oldcomm, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_accept stat;
  memset(&stat, 0, sizeof(dumpi_comm_accept));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_accept);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_accept);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.portname, portname);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_accept);
  }
  retval = PMPI_Comm_accept(portname, info, root, oldcomm, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_accept);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_comm_accept(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(portname != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.portname);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_accept);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_ACCEPT */

#ifndef DUMPI_SKIP_MPI_COMM_CONNECT
int MPI_Comm_connect(const char *portname, MPI_Info info, int root, MPI_Comm oldcomm, MPI_Comm *newcomm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_connect stat;
  memset(&stat, 0, sizeof(dumpi_comm_connect));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_connect);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_connect);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.portname, portname);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_connect);
  }
  retval = PMPI_Comm_connect(portname, info, root, oldcomm, newcomm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_connect);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    libdumpi_lock_io();
    dumpi_write_comm_connect(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(portname != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.portname);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_connect);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_CONNECT */

#ifndef DUMPI_SKIP_MPI_COMM_DISCONNECT
int MPI_Comm_disconnect(MPI_Comm *comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_disconnect stat;
  memset(&stat, 0, sizeof(dumpi_comm_disconnect));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_disconnect);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_disconnect);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, *comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_disconnect);
  }
  retval = PMPI_Comm_disconnect(comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_disconnect);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_comm_disconnect(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_disconnect);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_DISCONNECT */

#ifndef DUMPI_SKIP_MPI_COMM_GET_PARENT
int MPI_Comm_get_parent(MPI_Comm *parent) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_get_parent stat;
  memset(&stat, 0, sizeof(dumpi_comm_get_parent));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_get_parent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_get_parent);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_get_parent);
  }
  retval = PMPI_Comm_get_parent(parent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_get_parent);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.parent, *parent);
    libdumpi_lock_io();
    dumpi_write_comm_get_parent(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_get_parent);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_GET_PARENT */

#ifndef DUMPI_SKIP_MPI_COMM_JOIN
int MPI_Comm_join(int fd, MPI_Comm *comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_join stat;
  memset(&stat, 0, sizeof(dumpi_comm_join));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_join);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_join);
    DUMPI_INT_FROM_INT(stat.fd, fd);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_join);
  }
  retval = PMPI_Comm_join(fd, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_join);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, *comm);
    libdumpi_lock_io();
    dumpi_write_comm_join(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_join);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_JOIN */

#ifndef DUMPI_SKIP_MPI_COMM_SPAWN
int MPI_Comm_spawn(const char *command, char **argv, int maxprocs, MPI_Info info, int root, MPI_Comm oldcomm, MPI_Comm *newcomm, int *errcodes) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_spawn stat;
  memset(&stat, 0, sizeof(dumpi_comm_spawn));
  /* Extra declarations */
  int oldcommrank;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_spawn);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_spawn);
    PMPI_Comm_rank(oldcomm, &oldcommrank);
    DUMPI_INT_FROM_INT(stat.oldcommrank, oldcommrank);
    if(oldcommrank==root)    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.command, command);
    if(oldcommrank==root)    DUMPI_CHAR_FROM_CHAR_ARRAY_2(DUMPI_NULLTERM, DUMPI_CSTRING, stat.argv, argv);
    if(oldcommrank==root)    DUMPI_INT_FROM_INT(stat.maxprocs, maxprocs);
    if(oldcommrank==root)    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_spawn);
  }
  retval = PMPI_Comm_spawn(command, argv, maxprocs, info, root, oldcomm, newcomm, errcodes);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_spawn);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    if(oldcommrank==root)    DUMPI_ERRCODE_FROM_INT_ARRAY_1(maxprocs, stat.errcodes, errcodes);
    libdumpi_lock_io();
    dumpi_write_comm_spawn(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(command != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.command);
    if(argv != NULL)
      DUMPI_FREE_CHAR_FROM_CHAR_ARRAY_2(DUMPI_NULLTERM, stat.argv);
    if(errcodes != NULL) DUMPI_FREE_ERRCODE_FROM_INT(stat.errcodes);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_spawn);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_SPAWN */

#ifndef DUMPI_SKIP_MPI_COMM_SPAWN_MULTIPLE
int MPI_Comm_spawn_multiple(int count, char **commands, char ***argvs, const int *maxprocs, const MPI_Info *info, int root, MPI_Comm oldcomm, MPI_Comm *newcomm, int *errcodes) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_spawn_multiple stat;
  memset(&stat, 0, sizeof(dumpi_comm_spawn_multiple));
  /* Extra declarations */
  int oldcommrank;
  int totprocs;
  int i;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_spawn_multiple);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_spawn_multiple);
    PMPI_Comm_rank(oldcomm, &oldcommrank); totprocs=0; if(oldcommrank==root) for(i=0; i<count;++i) totprocs+=maxprocs[i];
    DUMPI_INT_FROM_INT(stat.totprocs, totprocs);
    DUMPI_INT_FROM_INT(stat.oldcommrank, oldcommrank);
    if(oldcommrank==root)    DUMPI_INT_FROM_INT(stat.count, count);
    if(oldcommrank==root)    DUMPI_CHAR_FROM_CHAR_ARRAY_2(count, DUMPI_CSTRING, stat.commands, commands);
    if(oldcommrank==root)    DUMPI_CHAR_FROM_CHAR_ARRAY_3(count, DUMPI_NULLTERM, DUMPI_CSTRING, stat.argvs, argvs);
    if(oldcommrank==root)    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.maxprocs, maxprocs);
    if(oldcommrank==root)    DUMPI_INFO_FROM_MPI_INFO_ARRAY_1(count, stat.info, info);
    DUMPI_INT_FROM_INT(stat.root, root);
    DUMPI_COMM_FROM_MPI_COMM(stat.oldcomm, oldcomm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_spawn_multiple);
  }
  retval = PMPI_Comm_spawn_multiple(count, commands, argvs, maxprocs, info, root, oldcomm, newcomm, errcodes);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_spawn_multiple);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_FROM_MPI_COMM(stat.newcomm, *newcomm);
    if(oldcommrank==root)    DUMPI_ERRCODE_FROM_INT_ARRAY_1(totprocs, stat.errcodes, errcodes);
    libdumpi_lock_io();
    dumpi_write_comm_spawn_multiple(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(commands != NULL)
      DUMPI_FREE_CHAR_FROM_CHAR_ARRAY_2(count, stat.commands);
    if(argvs != NULL)
      DUMPI_FREE_CHAR_FROM_CHAR_ARRAY_3(count, DUMPI_NULLTERM, stat.argvs);
    if(maxprocs != NULL) DUMPI_FREE_INT_FROM_INT(stat.maxprocs);
    if(info != NULL) DUMPI_FREE_INFO_FROM_MPI_INFO(stat.info);
    if(errcodes != NULL) DUMPI_FREE_ERRCODE_FROM_INT(stat.errcodes);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_spawn_multiple);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_SPAWN_MULTIPLE */

#ifndef DUMPI_SKIP_MPI_LOOKUP_NAME
int MPI_Lookup_name(const char *servicename, MPI_Info info, char *portname) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_lookup_name stat;
  memset(&stat, 0, sizeof(dumpi_lookup_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Lookup_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Lookup_name);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.servicename, servicename);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Lookup_name);
  }
  retval = PMPI_Lookup_name(servicename, info, portname);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Lookup_name);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.portname, portname);
    libdumpi_lock_io();
    dumpi_write_lookup_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(servicename != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.servicename);
    if(portname != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.portname);
    DUMPI_STOP_OVERHEAD(DUMPI_Lookup_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_LOOKUP_NAME */

#ifndef DUMPI_SKIP_MPI_OPEN_PORT
int MPI_Open_port(MPI_Info info, char *portname) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_open_port stat;
  memset(&stat, 0, sizeof(dumpi_open_port));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Open_port);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Open_port);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Open_port);
  }
  retval = PMPI_Open_port(info, portname);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Open_port);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.portname, portname);
    libdumpi_lock_io();
    dumpi_write_open_port(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(portname != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.portname);
    DUMPI_STOP_OVERHEAD(DUMPI_Open_port);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_OPEN_PORT */

#ifndef DUMPI_SKIP_MPI_PUBLISH_NAME
int MPI_Publish_name(const char *servicename, MPI_Info info, const char *portname) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_publish_name stat;
  memset(&stat, 0, sizeof(dumpi_publish_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Publish_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Publish_name);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.servicename, servicename);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.portname, portname);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Publish_name);
  }
  retval = PMPI_Publish_name(servicename, info, portname);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Publish_name);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_publish_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(servicename != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.servicename);
    if(portname != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.portname);
    DUMPI_STOP_OVERHEAD(DUMPI_Publish_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_PUBLISH_NAME */

#ifndef DUMPI_SKIP_MPI_UNPUBLISH_NAME
int MPI_Unpublish_name(const char *servicename, MPI_Info info, const char *portname) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_unpublish_name stat;
  memset(&stat, 0, sizeof(dumpi_unpublish_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Unpublish_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Unpublish_name);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.servicename, servicename);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.portname, portname);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Unpublish_name);
  }
  retval = PMPI_Unpublish_name(servicename, info, portname);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Unpublish_name);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_unpublish_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(servicename != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.servicename);
    if(portname != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.portname);
    DUMPI_STOP_OVERHEAD(DUMPI_Unpublish_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_UNPUBLISH_NAME */

#ifndef DUMPI_SKIP_MPI_ACCUMULATE
int MPI_Accumulate(const void *buf, int origincount, MPI_Datatype origintype, int targetrank, MPI_Aint targetdisp, int targetcount, MPI_Datatype targettype, MPI_Op op, MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_accumulate stat;
  memset(&stat, 0, sizeof(dumpi_accumulate));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Accumulate);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Accumulate);
    DUMPI_INT_FROM_INT(stat.origincount, origincount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.origintype, origintype);
    DUMPI_INT_FROM_INT(stat.targetrank, targetrank);
    DUMPI_INT_FROM_MPI_AINT(stat.targetdisp, targetdisp);
    DUMPI_INT_FROM_INT(stat.targetcount, targetcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.targettype, targettype);
    DUMPI_OP_FROM_MPI_OP(stat.op, op);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Accumulate);
  }
  retval = PMPI_Accumulate(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, op, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Accumulate);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_accumulate(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Accumulate);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ACCUMULATE */

#ifndef DUMPI_SKIP_MPI_GET
int MPI_Get(void *buf, int origincount, MPI_Datatype origintype, int targetrank, MPI_Aint targetdisp, int targetcount, MPI_Datatype targettype, MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_get stat;
  memset(&stat, 0, sizeof(dumpi_get));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Get);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get);
    DUMPI_INT_FROM_INT(stat.origincount, origincount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.origintype, origintype);
    DUMPI_INT_FROM_INT(stat.targetrank, targetrank);
    DUMPI_INT_FROM_MPI_AINT(stat.targetdisp, targetdisp);
    DUMPI_INT_FROM_INT(stat.targetcount, targetcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.targettype, targettype);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Get);
  }
  retval = PMPI_Get(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_get(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Get);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GET */

#ifndef DUMPI_SKIP_MPI_PUT
int MPI_Put(const void *buf, int origincount, MPI_Datatype origintype, int targetrank, MPI_Aint targetdisp, int targetcount, MPI_Datatype targettype, MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_put stat;
  memset(&stat, 0, sizeof(dumpi_put));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Put);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Put);
    DUMPI_INT_FROM_INT(stat.origincount, origincount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.origintype, origintype);
    DUMPI_INT_FROM_INT(stat.targetrank, targetrank);
    DUMPI_INT_FROM_MPI_AINT(stat.targetdisp, targetdisp);
    DUMPI_INT_FROM_INT(stat.targetcount, targetcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.targettype, targettype);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Put);
  }
  retval = PMPI_Put(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Put);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_put(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Put);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_PUT */

#ifndef DUMPI_SKIP_MPI_WIN_COMPLETE
int MPI_Win_complete(MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_complete stat;
  memset(&stat, 0, sizeof(dumpi_win_complete));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_complete);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_complete);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_complete);
  }
  retval = PMPI_Win_complete(win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_complete);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_complete(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_complete);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_COMPLETE */

#ifndef DUMPI_SKIP_MPI_WIN_CREATE
int MPI_Win_create(void *addr, MPI_Aint size, int dispunit, MPI_Info info, MPI_Comm comm, MPI_Win *win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_create stat;
  memset(&stat, 0, sizeof(dumpi_win_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_create);
    DUMPI_INT_FROM_MPI_AINT(stat.size, size);
    DUMPI_INT_FROM_INT(stat.dispunit, dispunit);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_create);
  }
  retval = PMPI_Win_create(addr, size, dispunit, info, comm, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, *win);
    libdumpi_lock_io();
    dumpi_write_win_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_CREATE */

#ifndef DUMPI_SKIP_MPI_WIN_FENCE
int MPI_Win_fence(int assertion, MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_fence stat;
  memset(&stat, 0, sizeof(dumpi_win_fence));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_fence);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_fence);
    DUMPI_WIN_ASSERT_FROM_INT(stat.assertion, assertion);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_fence);
  }
  retval = PMPI_Win_fence(assertion, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_fence);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_fence(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_fence);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_FENCE */

#ifndef DUMPI_SKIP_MPI_WIN_FREE
int MPI_Win_free(MPI_Win *win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_free stat;
  memset(&stat, 0, sizeof(dumpi_win_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_free);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, *win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_free);
  }
  retval = PMPI_Win_free(win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_FREE */

#ifndef DUMPI_SKIP_MPI_WIN_GET_GROUP
int MPI_Win_get_group(MPI_Win win, MPI_Group *group) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_get_group stat;
  memset(&stat, 0, sizeof(dumpi_win_get_group));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_get_group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_get_group);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_get_group);
  }
  retval = PMPI_Win_get_group(win, group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_get_group);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, *group);
    libdumpi_lock_io();
    dumpi_write_win_get_group(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_get_group);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_GET_GROUP */

#ifndef DUMPI_SKIP_MPI_WIN_LOCK
int MPI_Win_lock(int locktype, int winrank, int assertion, MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_lock stat;
  memset(&stat, 0, sizeof(dumpi_win_lock));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_lock);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_lock);
    DUMPI_LOCKTYPE_FROM_INT(stat.locktype, locktype);
    DUMPI_INT_FROM_INT(stat.winrank, winrank);
    DUMPI_WIN_ASSERT_FROM_INT(stat.assertion, assertion);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_lock);
  }
  retval = PMPI_Win_lock(locktype, winrank, assertion, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_lock);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_lock(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_lock);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_LOCK */

#ifndef DUMPI_SKIP_MPI_WIN_POST
int MPI_Win_post(MPI_Group group, int assertion, MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_post stat;
  memset(&stat, 0, sizeof(dumpi_win_post));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_post);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_post);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_WIN_ASSERT_FROM_INT(stat.assertion, assertion);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_post);
  }
  retval = PMPI_Win_post(group, assertion, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_post);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_post(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_post);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_POST */

#ifndef DUMPI_SKIP_MPI_WIN_START
int MPI_Win_start(MPI_Group group, int assertion, MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_start stat;
  memset(&stat, 0, sizeof(dumpi_win_start));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_start);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_start);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, group);
    DUMPI_WIN_ASSERT_FROM_INT(stat.assertion, assertion);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_start);
  }
  retval = PMPI_Win_start(group, assertion, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_start);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_start(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_start);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_START */

#ifndef DUMPI_SKIP_MPI_WIN_TEST
int MPI_Win_test(MPI_Win win, int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_test stat;
  memset(&stat, 0, sizeof(dumpi_win_test));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_test);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_test);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_test);
  }
  retval = PMPI_Win_test(win, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_test);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_win_test(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_test);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_TEST */

#ifndef DUMPI_SKIP_MPI_WIN_UNLOCK
int MPI_Win_unlock(int winrank, MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_unlock stat;
  memset(&stat, 0, sizeof(dumpi_win_unlock));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_unlock);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_unlock);
    DUMPI_INT_FROM_INT(stat.winrank, winrank);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_unlock);
  }
  retval = PMPI_Win_unlock(winrank, win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_unlock);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_unlock(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_unlock);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_UNLOCK */

#ifndef DUMPI_SKIP_MPI_WIN_WAIT
int MPI_Win_wait(MPI_Win win) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_wait stat;
  memset(&stat, 0, sizeof(dumpi_win_wait));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_wait);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_wait);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_wait);
  }
  retval = PMPI_Win_wait(win);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_wait);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_wait(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_wait);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_WAIT */

#ifndef DUMPI_SKIP_MPI_ALLTOALLW
int MPI_Alltoallw(const void *sendbuf, const int *sendcounts, const int *senddispls, const MPI_Datatype *sendtypes, void *recvbuf, const int *recvcounts, const int *recvdispls, const MPI_Datatype *recvtypes, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_alltoallw stat;
  memset(&stat, 0, sizeof(dumpi_alltoallw));
  /* Extra declarations */
  int commsize;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Alltoallw);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Alltoallw);
    PMPI_Comm_size(comm, &commsize);
    DUMPI_INT_FROM_INT(stat.commsize, commsize);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.sendcounts, sendcounts);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.senddispls, senddispls);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_ARRAY_1(commsize, stat.sendtypes, sendtypes);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.recvcounts, recvcounts);
    DUMPI_INT_FROM_INT_ARRAY_1(commsize, stat.recvdispls, recvdispls);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_ARRAY_1(commsize, stat.recvtypes, recvtypes);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Alltoallw);
  }
  retval = PMPI_Alltoallw(sendbuf, sendcounts, senddispls, sendtypes, recvbuf, recvcounts, recvdispls, recvtypes, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Alltoallw);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_alltoallw(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(sendcounts != NULL) DUMPI_FREE_INT_FROM_INT(stat.sendcounts);
    if(senddispls != NULL) DUMPI_FREE_INT_FROM_INT(stat.senddispls);
    if(sendtypes != NULL) DUMPI_FREE_DATATYPE_FROM_MPI_DATATYPE(stat.sendtypes);
    if(recvcounts != NULL) DUMPI_FREE_INT_FROM_INT(stat.recvcounts);
    if(recvdispls != NULL) DUMPI_FREE_INT_FROM_INT(stat.recvdispls);
    if(recvtypes != NULL) DUMPI_FREE_DATATYPE_FROM_MPI_DATATYPE(stat.recvtypes);
    DUMPI_STOP_OVERHEAD(DUMPI_Alltoallw);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ALLTOALLW */

#ifndef DUMPI_SKIP_MPI_EXSCAN
int MPI_Exscan(const void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_exscan stat;
  memset(&stat, 0, sizeof(dumpi_exscan));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Exscan);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Exscan);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_OP_FROM_MPI_OP(stat.op, op);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Exscan);
  }
  retval = PMPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Exscan);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_exscan(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Exscan);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_EXSCAN */

#ifndef DUMPI_SKIP_MPI_ADD_ERROR_CLASS
int MPI_Add_error_class(int *errorclass) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_add_error_class stat;
  memset(&stat, 0, sizeof(dumpi_add_error_class));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Add_error_class);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Add_error_class);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Add_error_class);
  }
  retval = PMPI_Add_error_class(errorclass);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Add_error_class);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.errorclass, *errorclass);
    libdumpi_lock_io();
    dumpi_write_add_error_class(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Add_error_class);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ADD_ERROR_CLASS */

#ifndef DUMPI_SKIP_MPI_ADD_ERROR_CODE
int MPI_Add_error_code(int errorclass, int *errorcode) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_add_error_code stat;
  memset(&stat, 0, sizeof(dumpi_add_error_code));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Add_error_code);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Add_error_code);
    DUMPI_INT_FROM_INT(stat.errorclass, errorclass);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Add_error_code);
  }
  retval = PMPI_Add_error_code(errorclass, errorcode);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Add_error_code);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.errorcode, *errorcode);
    libdumpi_lock_io();
    dumpi_write_add_error_code(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Add_error_code);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ADD_ERROR_CODE */

#ifndef DUMPI_SKIP_MPI_ADD_ERROR_STRING
int MPI_Add_error_string(int errorcode, const char *errorstring) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_add_error_string stat;
  memset(&stat, 0, sizeof(dumpi_add_error_string));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Add_error_string);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Add_error_string);
    DUMPI_INT_FROM_INT(stat.errorcode, errorcode);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.errorstring, errorstring);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Add_error_string);
  }
  retval = PMPI_Add_error_string(errorcode, errorstring);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Add_error_string);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_add_error_string(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(errorstring != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.errorstring);
    DUMPI_STOP_OVERHEAD(DUMPI_Add_error_string);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ADD_ERROR_STRING */

#ifndef DUMPI_SKIP_MPI_COMM_CALL_ERRHANDLER
int MPI_Comm_call_errhandler(MPI_Comm comm, int errorcode) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_call_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_comm_call_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_call_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_call_errhandler);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_INT_FROM_INT(stat.errorcode, errorcode);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_call_errhandler);
  }
  retval = PMPI_Comm_call_errhandler(comm, errorcode);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_call_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_comm_call_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_call_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_CALL_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_COMM_CREATE_KEYVAL
int MPI_Comm_create_keyval(MPI_Comm_copy_attr_function *copyfunc, MPI_Comm_delete_attr_function *deletefunc, int *keyval, void *extrastate) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_create_keyval stat;
  memset(&stat, 0, sizeof(dumpi_comm_create_keyval));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_create_keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_create_keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_create_keyval);
  }
  retval = PMPI_Comm_create_keyval(copyfunc, deletefunc, keyval, extrastate);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_create_keyval);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_COMM_KEYVAL_FROM_INT(stat.keyval, *keyval);
    libdumpi_lock_io();
    dumpi_write_comm_create_keyval(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_create_keyval);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_CREATE_KEYVAL */

#ifndef DUMPI_SKIP_MPI_COMM_DELETE_ATTR
int MPI_Comm_delete_attr(MPI_Comm comm, int keyval) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_delete_attr stat;
  memset(&stat, 0, sizeof(dumpi_comm_delete_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_delete_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_delete_attr);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_COMM_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_delete_attr);
  }
  retval = PMPI_Comm_delete_attr(comm, keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_delete_attr);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_comm_delete_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_delete_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_DELETE_ATTR */

#ifndef DUMPI_SKIP_MPI_COMM_FREE_KEYVAL
int MPI_Comm_free_keyval(int *keyval) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_free_keyval stat;
  memset(&stat, 0, sizeof(dumpi_comm_free_keyval));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_free_keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_free_keyval);
    DUMPI_COMM_KEYVAL_FROM_INT(stat.keyval, *keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_free_keyval);
  }
  retval = PMPI_Comm_free_keyval(keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_free_keyval);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_comm_free_keyval(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_free_keyval);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_FREE_KEYVAL */

#ifndef DUMPI_SKIP_MPI_COMM_GET_ATTR
int MPI_Comm_get_attr(MPI_Comm comm, int keyval, void *value, int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_get_attr stat;
  memset(&stat, 0, sizeof(dumpi_comm_get_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_get_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_get_attr);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_COMM_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_get_attr);
  }
  retval = PMPI_Comm_get_attr(comm, keyval, value, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_get_attr);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_comm_get_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_get_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_GET_ATTR */

#ifndef DUMPI_SKIP_MPI_COMM_GET_NAME
int MPI_Comm_get_name(MPI_Comm comm, char *name, int *resultlen) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_get_name stat;
  memset(&stat, 0, sizeof(dumpi_comm_get_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_get_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_get_name);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_get_name);
  }
  retval = PMPI_Comm_get_name(comm, name, resultlen);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_get_name);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(*resultlen, stat.name, name);
    DUMPI_INT_FROM_INT(stat.resultlen, *resultlen);
    libdumpi_lock_io();
    dumpi_write_comm_get_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(name != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.name);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_get_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_GET_NAME */

#ifndef DUMPI_SKIP_MPI_COMM_SET_ATTR
int MPI_Comm_set_attr(MPI_Comm comm, int keyval, void *value) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_set_attr stat;
  memset(&stat, 0, sizeof(dumpi_comm_set_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_set_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_set_attr);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_COMM_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_set_attr);
  }
  retval = PMPI_Comm_set_attr(comm, keyval, value);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_set_attr);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_comm_set_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_set_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_SET_ATTR */

#ifndef DUMPI_SKIP_MPI_COMM_SET_NAME
int MPI_Comm_set_name(MPI_Comm comm, const char *name) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_set_name stat;
  memset(&stat, 0, sizeof(dumpi_comm_set_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_set_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_set_name);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.name, name);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_set_name);
  }
  retval = PMPI_Comm_set_name(comm, name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_set_name);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_comm_set_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(name != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.name);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_set_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_SET_NAME */

#ifndef DUMPI_SKIP_MPI_FILE_CALL_ERRHANDLER
int MPI_File_call_errhandler(MPI_File file, int errorcode) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_call_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_file_call_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_call_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_call_errhandler);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.errorcode, errorcode);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_call_errhandler);
  }
  retval = PMPI_File_call_errhandler(file, errorcode);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_call_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_call_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_call_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_CALL_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_GREQUEST_COMPLETE
int MPI_Grequest_complete(MPI_Request request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_grequest_complete stat;
  memset(&stat, 0, sizeof(dumpi_grequest_complete));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Grequest_complete);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Grequest_complete);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, request);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Grequest_complete);
  }
  retval = PMPI_Grequest_complete(request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Grequest_complete);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_grequest_complete(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Grequest_complete);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GREQUEST_COMPLETE */

#ifndef DUMPI_SKIP_MPI_GREQUEST_START
int MPI_Grequest_start(MPI_Grequest_query_function *queryfunc, MPI_Grequest_free_function *freefunc, MPI_Grequest_cancel_function *cancelfunc, void *state, MPI_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_grequest_start stat;
  memset(&stat, 0, sizeof(dumpi_grequest_start));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Grequest_start);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Grequest_start);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Grequest_start);
  }
  retval = PMPI_Grequest_start(queryfunc, freefunc, cancelfunc, state, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Grequest_start);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_grequest_start(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Grequest_start);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GREQUEST_START */

#ifndef DUMPI_SKIP_MPI_INIT_THREAD
int MPI_Init_thread(int *argc, char ***argv, int required, int *provided) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_init_thread stat;
  memset(&stat, 0, sizeof(dumpi_init_thread));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Init_thread);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Init_thread);
    if(argc)
      DUMPI_INT_FROM_INT(stat.argc, *argc);
    else
      stat.argc = 0;
    if(argv && *argv)
      DUMPI_CHAR_FROM_CHAR_ARRAY_2(argc, DUMPI_CSTRING, stat.argv, *argv);
    else {
      stat.argc = 0;
      stat.argv = NULL;
    }
    DUMPI_THREADLEVEL_FROM_INT(stat.required, required);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Init_thread);
  }
  retval = PMPI_Init_thread(argc, argv, required, provided);
  if(retval == MPI_SUCCESS) {
    /* PMPI_Init_thread is tagged as an INITIAL call */
    libdumpi_open_files();
    libdumpi_init_maps();
    register_builtin_datatypes();
    DUMPI_START_TIME(cpu, wall); /* We cheat on timings for INIT. */
    if(profiling) {
      DUMPI_START_OVERHEAD(DUMPI_Init_thread);
      DUMPI_STOP_TIME(cpu, wall);
      DUMPI_THREADLEVEL_FROM_INT(stat.provided, *provided);
      libdumpi_lock_io();
    dumpi_write_init_thread(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
      if(argv != NULL)
	DUMPI_FREE_CHAR_FROM_CHAR_ARRAY_2(argc, stat.argv);
      DUMPI_STOP_OVERHEAD(DUMPI_Init_thread);
    }
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INIT_THREAD */

#ifndef DUMPI_SKIP_MPI_IS_THREAD_MAIN
int MPI_Is_thread_main(int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_is_thread_main stat;
  memset(&stat, 0, sizeof(dumpi_is_thread_main));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Is_thread_main);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Is_thread_main);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Is_thread_main);
  }
  retval = PMPI_Is_thread_main(flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Is_thread_main);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_is_thread_main(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Is_thread_main);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_IS_THREAD_MAIN */

#ifndef DUMPI_SKIP_MPI_QUERY_THREAD
int MPI_Query_thread(int *supported) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_query_thread stat;
  memset(&stat, 0, sizeof(dumpi_query_thread));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Query_thread);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Query_thread);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Query_thread);
  }
  retval = PMPI_Query_thread(supported);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Query_thread);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_THREADLEVEL_FROM_INT(stat.supported, *supported);
    libdumpi_lock_io();
    dumpi_write_query_thread(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Query_thread);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_QUERY_THREAD */

#ifndef DUMPI_SKIP_MPI_STATUS_SET_CANCELLED
int MPI_Status_set_cancelled(MPI_Status *status, int flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_status_set_cancelled stat;
  memset(&stat, 0, sizeof(dumpi_status_set_cancelled));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Status_set_cancelled);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Status_set_cancelled);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    DUMPI_INT_FROM_INT(stat.flag, flag);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Status_set_cancelled);
  }
  retval = PMPI_Status_set_cancelled(status, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Status_set_cancelled);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_status_set_cancelled(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Status_set_cancelled);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_STATUS_SET_CANCELLED */

#ifndef DUMPI_SKIP_MPI_STATUS_SET_ELEMENTS
int MPI_Status_set_elements(MPI_Status *status, MPI_Datatype datatype, int count) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_status_set_elements stat;
  memset(&stat, 0, sizeof(dumpi_status_set_elements));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Status_set_elements);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Status_set_elements);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Status_set_elements);
  }
  retval = PMPI_Status_set_elements(status, datatype, count);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Status_set_elements);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_status_set_elements(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Status_set_elements);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_STATUS_SET_ELEMENTS */

#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_KEYVAL
int MPI_Type_create_keyval(MPI_Type_copy_attr_function *copyfunc, MPI_Type_delete_attr_function *deletefunc, int *keyval, void *state) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_create_keyval stat;
  memset(&stat, 0, sizeof(dumpi_type_create_keyval));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_create_keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_keyval);
  }
  retval = PMPI_Type_create_keyval(copyfunc, deletefunc, keyval, state);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_keyval);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_TYPE_KEYVAL_FROM_INT(stat.keyval, *keyval);
    libdumpi_lock_io();
    dumpi_write_type_create_keyval(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_keyval);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_KEYVAL */

#ifndef DUMPI_SKIP_MPI_TYPE_DELETE_ATTR
int MPI_Type_delete_attr(MPI_Datatype datatype, int keyval) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_delete_attr stat;
  memset(&stat, 0, sizeof(dumpi_type_delete_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_delete_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_delete_attr);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_TYPE_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_delete_attr);
  }
  retval = PMPI_Type_delete_attr(datatype, keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_delete_attr);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_type_delete_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_delete_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_DELETE_ATTR */

#ifndef DUMPI_SKIP_MPI_TYPE_DUP
int MPI_Type_dup(MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_dup stat;
  memset(&stat, 0, sizeof(dumpi_type_dup));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_dup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_dup);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_dup);
  }
  retval = PMPI_Type_dup(oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_dup);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_dup(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_dup);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_DUP */

#ifndef DUMPI_SKIP_MPI_TYPE_FREE_KEYVAL
int MPI_Type_free_keyval(int *keyval) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_free_keyval stat;
  memset(&stat, 0, sizeof(dumpi_type_free_keyval));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_free_keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_free_keyval);
    DUMPI_TYPE_KEYVAL_FROM_INT(stat.keyval, *keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_free_keyval);
  }
  retval = PMPI_Type_free_keyval(keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_free_keyval);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_type_free_keyval(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_free_keyval);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_FREE_KEYVAL */

#ifndef DUMPI_SKIP_MPI_TYPE_GET_ATTR
int MPI_Type_get_attr(MPI_Datatype datatype, int keyval, void *value, int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_get_attr stat;
  memset(&stat, 0, sizeof(dumpi_type_get_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_get_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_attr);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.datatype, datatype);
    DUMPI_TYPE_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_attr);
  }
  retval = PMPI_Type_get_attr(datatype, keyval, value, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_attr);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_type_get_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_GET_ATTR */

#ifndef DUMPI_SKIP_MPI_TYPE_GET_CONTENTS
int MPI_Type_get_contents(MPI_Datatype datatype, int maxintegers, int maxaddresses, int maxdatatypes, int *arrintegers, MPI_Aint *arraddresses, MPI_Datatype *arrdatatypes) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_get_contents stat;
  memset(&stat, 0, sizeof(dumpi_type_get_contents));
  /* Extra declarations */
  int numintegers;
  int numaddresses;
  int numdatatypes;
  int combiner;
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_get_contents);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_contents);
    PMPI_Type_get_envelope(datatype, &numintegers, &numaddresses, &numdatatypes, &combiner);
    DUMPI_INT_FROM_INT(stat.numdatatypes, MIN(maxdatatypes, numdatatypes));
    DUMPI_INT_FROM_INT(stat.numaddresses, MIN(maxaddresses, numaddresses));
    DUMPI_INT_FROM_INT(stat.numintegers, MIN(maxintegers, numintegers));
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_INT_FROM_INT(stat.maxintegers, maxintegers);
    DUMPI_INT_FROM_INT(stat.maxaddresses, maxaddresses);
    DUMPI_INT_FROM_INT(stat.maxdatatypes, maxdatatypes);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_contents);
  }
  retval = PMPI_Type_get_contents(datatype, maxintegers, maxaddresses, maxdatatypes, arrintegers, arraddresses, arrdatatypes);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_contents);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT_ARRAY_1(MIN(maxintegers, numintegers), stat.arrintegers, arrintegers);
    DUMPI_INT_FROM_MPI_AINT_ARRAY_1(MIN(maxaddresses, numaddresses), stat.arraddresses, arraddresses);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_ARRAY_1(MIN(maxdatatypes, numdatatypes), stat.arrdatatypes, arrdatatypes);
    libdumpi_lock_io();
    dumpi_write_type_get_contents(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(arrintegers != NULL) DUMPI_FREE_INT_FROM_INT(stat.arrintegers);
    if(arraddresses != NULL) DUMPI_FREE_INT_FROM_MPI_AINT(stat.arraddresses);
    if(arrdatatypes != NULL) DUMPI_FREE_DATATYPE_FROM_MPI_DATATYPE(stat.arrdatatypes);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_contents);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_GET_CONTENTS */

#ifndef DUMPI_SKIP_MPI_TYPE_GET_ENVELOPE
int MPI_Type_get_envelope(MPI_Datatype datatype, int *numintegers, int *numaddresses, int *numdatatypes, int *combiner) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_get_envelope stat;
  memset(&stat, 0, sizeof(dumpi_type_get_envelope));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_get_envelope);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_envelope);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_envelope);
  }
  retval = PMPI_Type_get_envelope(datatype, numintegers, numaddresses, numdatatypes, combiner);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_envelope);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.numintegers, *numintegers);
    DUMPI_INT_FROM_INT(stat.numaddresses, *numaddresses);
    DUMPI_INT_FROM_INT(stat.numdatatypes, *numdatatypes);
    DUMPI_COMBINER_FROM_INT(stat.combiner, *combiner);
    libdumpi_lock_io();
    dumpi_write_type_get_envelope(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_envelope);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_GET_ENVELOPE */

#ifndef DUMPI_SKIP_MPI_TYPE_GET_NAME
int MPI_Type_get_name(MPI_Datatype datatype, char *name, int *resultlen) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_get_name stat;
  memset(&stat, 0, sizeof(dumpi_type_get_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_get_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_name);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_name);
  }
  retval = PMPI_Type_get_name(datatype, name, resultlen);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_name);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.name, name);
    DUMPI_INT_FROM_INT(stat.resultlen, *resultlen);
    libdumpi_lock_io();
    dumpi_write_type_get_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(name != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.name);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_GET_NAME */

#ifndef DUMPI_SKIP_MPI_TYPE_SET_ATTR
int MPI_Type_set_attr(MPI_Datatype datatype, int keyval, void *state) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_set_attr stat;
  memset(&stat, 0, sizeof(dumpi_type_set_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_set_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_set_attr);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.datatype, datatype);
    DUMPI_TYPE_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_set_attr);
  }
  retval = PMPI_Type_set_attr(datatype, keyval, state);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_set_attr);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_type_set_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_set_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_SET_ATTR */

#ifndef DUMPI_SKIP_MPI_TYPE_SET_NAME
int MPI_Type_set_name(MPI_Datatype datatype, const char *name) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_set_name stat;
  memset(&stat, 0, sizeof(dumpi_type_set_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_set_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_set_name);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.datatype, datatype);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.name, name);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_set_name);
  }
  retval = PMPI_Type_set_name(datatype, name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_set_name);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_type_set_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(name != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.name);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_set_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_SET_NAME */

#ifndef DUMPI_SKIP_MPI_TYPE_MATCH_SIZE
int MPI_Type_match_size(int typeclass, int size, MPI_Datatype *datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_match_size stat;
  memset(&stat, 0, sizeof(dumpi_type_match_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_match_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_match_size);
    DUMPI_TYPECLASS_FROM_INT(stat.typeclass, typeclass);
    DUMPI_INT_FROM_INT(stat.size, size);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_match_size);
  }
  retval = PMPI_Type_match_size(typeclass, size, datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_match_size);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, *datatype);
    libdumpi_lock_io();
    dumpi_write_type_match_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_match_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_MATCH_SIZE */

#ifndef DUMPI_SKIP_MPI_WIN_CALL_ERRHANDLER
int MPI_Win_call_errhandler(MPI_Win win, int errorcode) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_call_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_win_call_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_call_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_call_errhandler);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_INT_FROM_INT(stat.errorcode, errorcode);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_call_errhandler);
  }
  retval = PMPI_Win_call_errhandler(win, errorcode);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_call_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_call_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_call_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_CALL_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_WIN_CREATE_KEYVAL
int MPI_Win_create_keyval(MPI_Win_copy_attr_function *copyfun, MPI_Win_delete_attr_function *deletefun, int *keyval, void *state) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_create_keyval stat;
  memset(&stat, 0, sizeof(dumpi_win_create_keyval));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_create_keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_create_keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_create_keyval);
  }
  retval = PMPI_Win_create_keyval(copyfun, deletefun, keyval, state);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_create_keyval);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_WIN_KEYVAL_FROM_INT(stat.keyval, *keyval);
    libdumpi_lock_io();
    dumpi_write_win_create_keyval(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_create_keyval);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_CREATE_KEYVAL */

#ifndef DUMPI_SKIP_MPI_WIN_DELETE_ATTR
int MPI_Win_delete_attr(MPI_Win win, int keyval) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_delete_attr stat;
  memset(&stat, 0, sizeof(dumpi_win_delete_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_delete_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_delete_attr);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_WIN_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_delete_attr);
  }
  retval = PMPI_Win_delete_attr(win, keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_delete_attr);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_delete_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_delete_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_DELETE_ATTR */

#ifndef DUMPI_SKIP_MPI_WIN_FREE_KEYVAL
int MPI_Win_free_keyval(int *keyval) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_free_keyval stat;
  memset(&stat, 0, sizeof(dumpi_win_free_keyval));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_free_keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_free_keyval);
    DUMPI_WIN_KEYVAL_FROM_INT(stat.keyval, *keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_free_keyval);
  }
  retval = PMPI_Win_free_keyval(keyval);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_free_keyval);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_free_keyval(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_free_keyval);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_FREE_KEYVAL */

#ifndef DUMPI_SKIP_MPI_WIN_GET_ATTR
int MPI_Win_get_attr(MPI_Win win, int keyval, void *state, int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_get_attr stat;
  memset(&stat, 0, sizeof(dumpi_win_get_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_get_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_get_attr);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_WIN_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_get_attr);
  }
  retval = PMPI_Win_get_attr(win, keyval, state, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_get_attr);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_win_get_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_get_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_GET_ATTR */

#ifndef DUMPI_SKIP_MPI_WIN_GET_NAME
int MPI_Win_get_name(MPI_Win win, char *name, int *resultlen) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_get_name stat;
  memset(&stat, 0, sizeof(dumpi_win_get_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_get_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_get_name);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_get_name);
  }
  retval = PMPI_Win_get_name(win, name, resultlen);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_get_name);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.name, name);
    DUMPI_INT_FROM_INT(stat.resultlen, *resultlen);
    libdumpi_lock_io();
    dumpi_write_win_get_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(name != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.name);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_get_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_GET_NAME */

#ifndef DUMPI_SKIP_MPI_WIN_SET_ATTR
int MPI_Win_set_attr(MPI_Win win, int keyval, void *state) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_set_attr stat;
  memset(&stat, 0, sizeof(dumpi_win_set_attr));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_set_attr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_set_attr);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_WIN_KEYVAL_FROM_INT(stat.keyval, keyval);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_set_attr);
  }
  retval = PMPI_Win_set_attr(win, keyval, state);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_set_attr);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_set_attr(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_set_attr);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_SET_ATTR */

#ifndef DUMPI_SKIP_MPI_WIN_SET_NAME
int MPI_Win_set_name(MPI_Win win, const char *name) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_set_name stat;
  memset(&stat, 0, sizeof(dumpi_win_set_name));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_set_name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_set_name);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.name, name);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_set_name);
  }
  retval = PMPI_Win_set_name(win, name);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_set_name);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_win_set_name(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(name != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.name);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_set_name);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_SET_NAME */

#ifndef DUMPI_SKIP_MPI_ALLOC_MEM
int MPI_Alloc_mem(MPI_Aint size, MPI_Info info, void *baseptr) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_alloc_mem stat;
  memset(&stat, 0, sizeof(dumpi_alloc_mem));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Alloc_mem);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Alloc_mem);
    DUMPI_INT_FROM_MPI_AINT(stat.size, size);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Alloc_mem);
  }
  retval = PMPI_Alloc_mem(size, info, baseptr);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Alloc_mem);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_alloc_mem(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Alloc_mem);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_ALLOC_MEM */

#ifndef DUMPI_SKIP_MPI_COMM_CREATE_ERRHANDLER
int MPI_Comm_create_errhandler(MPI_Comm_errhandler_fn *errfun, MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_create_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_comm_create_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_create_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_create_errhandler);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_create_errhandler);
  }
  retval = PMPI_Comm_create_errhandler(errfun, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_create_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    libdumpi_lock_io();
    dumpi_write_comm_create_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_create_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_CREATE_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_COMM_GET_ERRHANDLER
int MPI_Comm_get_errhandler(MPI_Comm comm, MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_get_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_comm_get_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_get_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_get_errhandler);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_get_errhandler);
  }
  retval = PMPI_Comm_get_errhandler(comm, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_get_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    libdumpi_lock_io();
    dumpi_write_comm_get_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_get_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_GET_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_COMM_SET_ERRHANDLER
int MPI_Comm_set_errhandler(MPI_Comm comm, MPI_Errhandler errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_comm_set_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_comm_set_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Comm_set_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_set_errhandler);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, errhandler);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_set_errhandler);
  }
  retval = PMPI_Comm_set_errhandler(comm, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Comm_set_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_comm_set_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Comm_set_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_COMM_SET_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_FILE_CREATE_ERRHANDLER
int MPI_File_create_errhandler(MPI_File_errhandler_fn *errfun, MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_create_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_file_create_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_create_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_create_errhandler);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_create_errhandler);
  }
  retval = PMPI_File_create_errhandler(errfun, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_create_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    libdumpi_lock_io();
    dumpi_write_file_create_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_create_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_CREATE_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_FILE_GET_ERRHANDLER
int MPI_File_get_errhandler(MPI_File file, MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_file_get_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_errhandler);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_errhandler);
  }
  retval = PMPI_File_get_errhandler(file, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    libdumpi_lock_io();
    dumpi_write_file_get_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_FILE_SET_ERRHANDLER
int MPI_File_set_errhandler(MPI_File file, MPI_Errhandler errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_set_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_file_set_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_set_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_errhandler);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, errhandler);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_errhandler);
  }
  retval = PMPI_File_set_errhandler(file, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_set_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_SET_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_FINALIZED
int MPI_Finalized(int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_finalized stat;
  memset(&stat, 0, sizeof(dumpi_finalized));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Finalized);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Finalized);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Finalized);
  }
  retval = PMPI_Finalized(flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Finalized);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_finalized(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Finalized);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FINALIZED */

#ifndef DUMPI_SKIP_MPI_FREE_MEM
int MPI_Free_mem(void *buf) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_free_mem stat;
  memset(&stat, 0, sizeof(dumpi_free_mem));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Free_mem);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Free_mem);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Free_mem);
  }
  retval = PMPI_Free_mem(buf);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Free_mem);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_free_mem(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Free_mem);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FREE_MEM */

#ifndef DUMPI_SKIP_MPI_GET_ADDRESS
int MPI_Get_address(const void *buf, MPI_Aint *address) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_get_address stat;
  memset(&stat, 0, sizeof(dumpi_get_address));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Get_address);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_address);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Get_address);
  }
  retval = PMPI_Get_address(buf, address);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Get_address);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.address, *address);
    libdumpi_lock_io();
    dumpi_write_get_address(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Get_address);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_GET_ADDRESS */

#ifndef DUMPI_SKIP_MPI_INFO_CREATE
int MPI_Info_create(MPI_Info *info) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_create stat;
  memset(&stat, 0, sizeof(dumpi_info_create));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_create);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_create);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_create);
  }
  retval = PMPI_Info_create(info);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_create);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, *info);
    libdumpi_lock_io();
    dumpi_write_info_create(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Info_create);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_CREATE */

#ifndef DUMPI_SKIP_MPI_INFO_DELETE
int MPI_Info_delete(MPI_Info info, const char *key) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_delete stat;
  memset(&stat, 0, sizeof(dumpi_info_delete));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_delete);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_delete);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.key, key);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_delete);
  }
  retval = PMPI_Info_delete(info, key);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_delete);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_info_delete(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(key != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.key);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_delete);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_DELETE */

#ifndef DUMPI_SKIP_MPI_INFO_DUP
int MPI_Info_dup(MPI_Info oldinfo, MPI_Info *newinfo) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_dup stat;
  memset(&stat, 0, sizeof(dumpi_info_dup));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_dup);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_dup);
    DUMPI_INFO_FROM_MPI_INFO(stat.oldinfo, oldinfo);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_dup);
  }
  retval = PMPI_Info_dup(oldinfo, newinfo);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_dup);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INFO_FROM_MPI_INFO(stat.newinfo, *newinfo);
    libdumpi_lock_io();
    dumpi_write_info_dup(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Info_dup);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_DUP */

#ifndef DUMPI_SKIP_MPI_INFO_FREE
int MPI_Info_free(MPI_Info *info) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_free stat;
  memset(&stat, 0, sizeof(dumpi_info_free));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_free);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_free);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, *info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_free);
  }
  retval = PMPI_Info_free(info);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_free);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_info_free(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Info_free);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_FREE */

#ifndef DUMPI_SKIP_MPI_INFO_GET
int MPI_Info_get(MPI_Info info, const char *key, int valuelength, char *value, int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_get stat;
  memset(&stat, 0, sizeof(dumpi_info_get));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_get);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_get);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.key, key);
    DUMPI_INT_FROM_INT(stat.valuelength, valuelength);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_get);
  }
  retval = PMPI_Info_get(info, key, valuelength, value, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_get);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.value, value);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_info_get(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(key != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.key);
    if(value != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.value);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_get);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_GET */

#ifndef DUMPI_SKIP_MPI_INFO_GET_NKEYS
int MPI_Info_get_nkeys(MPI_Info info, int *nkeys) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_get_nkeys stat;
  memset(&stat, 0, sizeof(dumpi_info_get_nkeys));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_get_nkeys);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_get_nkeys);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_get_nkeys);
  }
  retval = PMPI_Info_get_nkeys(info, nkeys);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_get_nkeys);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.nkeys, *nkeys);
    libdumpi_lock_io();
    dumpi_write_info_get_nkeys(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Info_get_nkeys);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_GET_NKEYS */

#ifndef DUMPI_SKIP_MPI_INFO_GET_NTHKEY
int MPI_Info_get_nthkey(MPI_Info info, int n, char *key) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_get_nthkey stat;
  memset(&stat, 0, sizeof(dumpi_info_get_nthkey));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_get_nthkey);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_get_nthkey);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_INT_FROM_INT(stat.n, n);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_get_nthkey);
  }
  retval = PMPI_Info_get_nthkey(info, n, key);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_get_nthkey);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.key, key);
    libdumpi_lock_io();
    dumpi_write_info_get_nthkey(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(key != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.key);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_get_nthkey);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_GET_NTHKEY */

#ifndef DUMPI_SKIP_MPI_INFO_GET_VALUELEN
int MPI_Info_get_valuelen(MPI_Info info, const char *key, int *valuelen, int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_get_valuelen stat;
  memset(&stat, 0, sizeof(dumpi_info_get_valuelen));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_get_valuelen);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_get_valuelen);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.key, key);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_get_valuelen);
  }
  retval = PMPI_Info_get_valuelen(info, key, valuelen, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_get_valuelen);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.valuelen, *valuelen);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_info_get_valuelen(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(key != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.key);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_get_valuelen);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_GET_VALUELEN */

#ifndef DUMPI_SKIP_MPI_INFO_SET
int MPI_Info_set(MPI_Info info, const char *key, const char *value) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_info_set stat;
  memset(&stat, 0, sizeof(dumpi_info_set));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Info_set);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_set);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.key, key);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.value, value);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_set);
  }
  retval = PMPI_Info_set(info, key, value);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Info_set);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_info_set(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(key != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.key);
    if(value != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.value);
    DUMPI_STOP_OVERHEAD(DUMPI_Info_set);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_INFO_SET */

#ifndef DUMPI_SKIP_MPI_PACK_EXTERNAL
int MPI_Pack_external(const char *datarep, const void *inbuf, int incount, MPI_Datatype intype, void *outbuf, MPI_Aint outcount, MPI_Aint *position) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_pack_external stat;
  memset(&stat, 0, sizeof(dumpi_pack_external));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Pack_external);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Pack_external);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.datarep, datarep);
    DUMPI_INT_FROM_INT(stat.incount, incount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.intype, intype);
    DUMPI_INT_FROM_MPI_AINT(stat.outcount, outcount);
    DUMPI_INT_FROM_MPI_AINT(stat.position.in, *position);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Pack_external);
  }
  retval = PMPI_Pack_external(datarep, inbuf, incount, intype, outbuf, outcount, position);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Pack_external);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.position.out, *position);
    libdumpi_lock_io();
    dumpi_write_pack_external(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(datarep != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.datarep);
    DUMPI_STOP_OVERHEAD(DUMPI_Pack_external);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_PACK_EXTERNAL */

#ifndef DUMPI_SKIP_MPI_PACK_EXTERNAL_SIZE
int MPI_Pack_external_size(const char *datarep, int incount, MPI_Datatype datatype, MPI_Aint *size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_pack_external_size stat;
  memset(&stat, 0, sizeof(dumpi_pack_external_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Pack_external_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Pack_external_size);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.datarep, datarep);
    DUMPI_INT_FROM_INT(stat.incount, incount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Pack_external_size);
  }
  retval = PMPI_Pack_external_size(datarep, incount, datatype, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Pack_external_size);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.size, *size);
    libdumpi_lock_io();
    dumpi_write_pack_external_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(datarep != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.datarep);
    DUMPI_STOP_OVERHEAD(DUMPI_Pack_external_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_PACK_EXTERNAL_SIZE */

#ifndef DUMPI_SKIP_MPI_REQUEST_GET_STATUS
int MPI_Request_get_status(MPI_Request request, int *flag, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_request_get_status stat;
  memset(&stat, 0, sizeof(dumpi_request_get_status));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Request_get_status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Request_get_status);
    DUMPI_REQUEST_FROM_MPI_REQUEST(stat.request, request);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Request_get_status);
  }
  retval = PMPI_Request_get_status(request, flag, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Request_get_status);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    if(*flag!=0)    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_request_get_status(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_Request_get_status);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_REQUEST_GET_STATUS */

#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_DARRAY
int MPI_Type_create_darray(int size, int rank, int ndims, const int *gsizes, const int *distribs, const int *dargs, const int *psizes, int order, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_create_darray stat;
  memset(&stat, 0, sizeof(dumpi_type_create_darray));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_create_darray);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_darray);
    DUMPI_INT_FROM_INT(stat.size, size);
    DUMPI_INT_FROM_INT(stat.rank, rank);
    DUMPI_INT_FROM_INT(stat.ndims, ndims);
    DUMPI_INT_FROM_INT_ARRAY_1(ndims, stat.gsizes, gsizes);
    DUMPI_DISTRIBUTION_FROM_INT_ARRAY_1(ndims, stat.distribs, distribs);
    DUMPI_INT_FROM_INT_ARRAY_1(ndims, stat.dargs, dargs);
    DUMPI_INT_FROM_INT_ARRAY_1(ndims, stat.psizes, psizes);
    DUMPI_ORDERING_FROM_INT(stat.order, order);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_darray);
  }
  retval = PMPI_Type_create_darray(size, rank, ndims, gsizes, distribs, dargs, psizes, order, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_darray);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_create_darray(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(gsizes != NULL) DUMPI_FREE_INT_FROM_INT(stat.gsizes);
    if(distribs != NULL) DUMPI_FREE_DISTRIBUTION_FROM_INT(stat.distribs);
    if(dargs != NULL) DUMPI_FREE_INT_FROM_INT(stat.dargs);
    if(psizes != NULL) DUMPI_FREE_INT_FROM_INT(stat.psizes);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_darray);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_DARRAY */

#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_HINDEXED
int MPI_Type_create_hindexed(int count, const int *blocklengths, const MPI_Aint *displacements, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_create_hindexed stat;
  memset(&stat, 0, sizeof(dumpi_type_create_hindexed));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_create_hindexed);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_hindexed);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.blocklengths, blocklengths);
    DUMPI_INT_FROM_MPI_AINT_ARRAY_1(count, stat.displacements, displacements);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_hindexed);
  }
  retval = PMPI_Type_create_hindexed(count, blocklengths, displacements, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_hindexed);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_create_hindexed(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(blocklengths != NULL) DUMPI_FREE_INT_FROM_INT(stat.blocklengths);
    if(displacements != NULL) DUMPI_FREE_INT_FROM_MPI_AINT(stat.displacements);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_hindexed);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_HINDEXED */

#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_HVECTOR
int MPI_Type_create_hvector(int count, int blocklength, MPI_Aint stride, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_create_hvector stat;
  memset(&stat, 0, sizeof(dumpi_type_create_hvector));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_create_hvector);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_hvector);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT(stat.blocklength, blocklength);
    DUMPI_INT_FROM_MPI_AINT(stat.stride, stride);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_hvector);
  }
  retval = PMPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_hvector);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_create_hvector(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_hvector);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_HVECTOR */

#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_INDEXED_BLOCK
int MPI_Type_create_indexed_block(int count, int blocklength, const int *displacments, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_create_indexed_block stat;
  memset(&stat, 0, sizeof(dumpi_type_create_indexed_block));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_create_indexed_block);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_indexed_block);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT(stat.blocklength, blocklength);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.displacments, displacments);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_indexed_block);
  }
  retval = PMPI_Type_create_indexed_block(count, blocklength, displacments, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_indexed_block);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_create_indexed_block(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(displacments != NULL) DUMPI_FREE_INT_FROM_INT(stat.displacments);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_indexed_block);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_INDEXED_BLOCK */

#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_RESIZED
int MPI_Type_create_resized(MPI_Datatype oldtype, MPI_Aint lb, MPI_Aint extent, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_create_resized stat;
  memset(&stat, 0, sizeof(dumpi_type_create_resized));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_create_resized);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_resized);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_INT_FROM_MPI_AINT(stat.lb, lb);
    DUMPI_INT_FROM_MPI_AINT(stat.extent, extent);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_resized);
  }
  retval = PMPI_Type_create_resized(oldtype, lb, extent, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_resized);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_create_resized(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_resized);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_RESIZED */

#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_STRUCT
int MPI_Type_create_struct(int count, const int *blocklengths, const MPI_Aint *displacements, const MPI_Datatype *oldtypes, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_create_struct stat;
  memset(&stat, 0, sizeof(dumpi_type_create_struct));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_create_struct);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_struct);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_INT_FROM_INT_ARRAY_1(count, stat.blocklengths, blocklengths);
    DUMPI_INT_FROM_MPI_AINT_ARRAY_1(count, stat.displacements, displacements);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_ARRAY_1(count, stat.oldtypes, oldtypes);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_struct);
  }
  retval = PMPI_Type_create_struct(count, blocklengths, displacements, oldtypes, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_struct);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_create_struct(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(blocklengths != NULL) DUMPI_FREE_INT_FROM_INT(stat.blocklengths);
    if(displacements != NULL) DUMPI_FREE_INT_FROM_MPI_AINT(stat.displacements);
    if(oldtypes != NULL) DUMPI_FREE_DATATYPE_FROM_MPI_DATATYPE(stat.oldtypes);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_struct);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_STRUCT */

#ifndef DUMPI_SKIP_MPI_TYPE_CREATE_SUBARRAY
int MPI_Type_create_subarray(int ndims, const int *sizes, const int *subsizes, const int *starts, int order, MPI_Datatype oldtype, MPI_Datatype *newtype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_create_subarray stat;
  memset(&stat, 0, sizeof(dumpi_type_create_subarray));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_create_subarray);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_subarray);
    DUMPI_INT_FROM_INT(stat.ndims, ndims);
    DUMPI_INT_FROM_INT_ARRAY_1(ndims, stat.sizes, sizes);
    DUMPI_INT_FROM_INT_ARRAY_1(ndims, stat.subsizes, subsizes);
    DUMPI_INT_FROM_INT_ARRAY_1(ndims, stat.starts, starts);
    DUMPI_ORDERING_FROM_INT(stat.order, order);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.oldtype, oldtype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_subarray);
  }
  retval = PMPI_Type_create_subarray(ndims, sizes, subsizes, starts, order, oldtype, newtype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_create_subarray);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(stat.newtype, *newtype);
    libdumpi_lock_io();
    dumpi_write_type_create_subarray(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(sizes != NULL) DUMPI_FREE_INT_FROM_INT(stat.sizes);
    if(subsizes != NULL) DUMPI_FREE_INT_FROM_INT(stat.subsizes);
    if(starts != NULL) DUMPI_FREE_INT_FROM_INT(stat.starts);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_create_subarray);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_CREATE_SUBARRAY */

#ifndef DUMPI_SKIP_MPI_TYPE_GET_EXTENT
int MPI_Type_get_extent(MPI_Datatype datatype, MPI_Aint *lb, MPI_Aint *extent) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_get_extent stat;
  memset(&stat, 0, sizeof(dumpi_type_get_extent));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_get_extent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_extent);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_extent);
  }
  retval = PMPI_Type_get_extent(datatype, lb, extent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_extent);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.lb, *lb);
    DUMPI_INT_FROM_MPI_AINT(stat.extent, *extent);
    libdumpi_lock_io();
    dumpi_write_type_get_extent(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_extent);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_GET_EXTENT */

#ifndef DUMPI_SKIP_MPI_TYPE_GET_TRUE_EXTENT
int MPI_Type_get_true_extent(MPI_Datatype datatype, MPI_Aint *lb, MPI_Aint *extent) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_type_get_true_extent stat;
  memset(&stat, 0, sizeof(dumpi_type_get_true_extent));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Type_get_true_extent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_true_extent);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_true_extent);
  }
  retval = PMPI_Type_get_true_extent(datatype, lb, extent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Type_get_true_extent);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.lb, *lb);
    DUMPI_INT_FROM_MPI_AINT(stat.extent, *extent);
    libdumpi_lock_io();
    dumpi_write_type_get_true_extent(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Type_get_true_extent);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_TYPE_GET_TRUE_EXTENT */

#ifndef DUMPI_SKIP_MPI_UNPACK_EXTERNAL
int MPI_Unpack_external(const char *datarep, const void *inbuf, MPI_Aint insize, MPI_Aint *position, void *outbuf, int outcount, MPI_Datatype datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_unpack_external stat;
  memset(&stat, 0, sizeof(dumpi_unpack_external));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Unpack_external);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Unpack_external);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.datarep, datarep);
    DUMPI_INT_FROM_MPI_AINT(stat.insize, insize);
    DUMPI_INT_FROM_INT(stat.outcount, outcount);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_INT_FROM_MPI_AINT(stat.position.in, *position);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Unpack_external);
  }
  retval = PMPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Unpack_external);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.position.out, *position);
    libdumpi_lock_io();
    dumpi_write_unpack_external(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(datarep != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.datarep);
    DUMPI_STOP_OVERHEAD(DUMPI_Unpack_external);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_UNPACK_EXTERNAL */

#ifndef DUMPI_SKIP_MPI_WIN_CREATE_ERRHANDLER
int MPI_Win_create_errhandler(MPI_Win_errhandler_fn *errfun, MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_create_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_win_create_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_create_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_create_errhandler);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_create_errhandler);
  }
  retval = PMPI_Win_create_errhandler(errfun, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_create_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    libdumpi_lock_io();
    dumpi_write_win_create_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_create_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_CREATE_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_WIN_GET_ERRHANDLER
int MPI_Win_get_errhandler(MPI_Win win, MPI_Errhandler *errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_get_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_win_get_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_get_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_get_errhandler);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_get_errhandler);
  }
  retval = PMPI_Win_get_errhandler(win, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_get_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, *errhandler);
    libdumpi_lock_io();
    dumpi_write_win_get_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_get_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_GET_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_WIN_SET_ERRHANDLER
int MPI_Win_set_errhandler(MPI_Win win, MPI_Errhandler errhandler) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_win_set_errhandler stat;
  memset(&stat, 0, sizeof(dumpi_win_set_errhandler));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Win_set_errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_set_errhandler);
    DUMPI_WIN_FROM_MPI_WIN(stat.win, win);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Win_set_errhandler);
  }
  retval = PMPI_Win_set_errhandler(win, errhandler);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Win_set_errhandler);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(stat.errhandler, errhandler);
    libdumpi_lock_io();
    dumpi_write_win_set_errhandler(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_Win_set_errhandler);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_WIN_SET_ERRHANDLER */

#ifndef DUMPI_SKIP_MPI_FILE_OPEN
int MPI_File_open(MPI_Comm comm, const char *filename, int amode, MPI_Info info, MPI_File *file) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_open stat;
  memset(&stat, 0, sizeof(dumpi_file_open));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_open);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_open);
    DUMPI_COMM_FROM_MPI_COMM(stat.comm, comm);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.filename, filename);
    DUMPI_FILEMODE_FROM_INT(stat.amode, amode);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_open);
  }
  retval = PMPI_File_open(comm, filename, amode, info, file);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_open);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, *file);
    libdumpi_lock_io();
    dumpi_write_file_open(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(filename != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.filename);
    DUMPI_STOP_OVERHEAD(DUMPI_File_open);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_OPEN */

#ifndef DUMPI_SKIP_MPI_FILE_CLOSE
int MPI_File_close(MPI_File *file) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_close stat;
  memset(&stat, 0, sizeof(dumpi_file_close));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_close);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_close);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, *file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_close);
  }
  retval = PMPI_File_close(file);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_close);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_close(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_close);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_CLOSE */

#ifndef DUMPI_SKIP_MPI_FILE_DELETE
int MPI_File_delete(const char *filename, MPI_Info info) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_delete stat;
  memset(&stat, 0, sizeof(dumpi_file_delete));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_delete);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_delete);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.filename, filename);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_delete);
  }
  retval = PMPI_File_delete(filename, info);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_delete);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_delete(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(filename != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.filename);
    DUMPI_STOP_OVERHEAD(DUMPI_File_delete);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_DELETE */

#ifndef DUMPI_SKIP_MPI_FILE_SET_SIZE
int MPI_File_set_size(MPI_File file, MPI_Offset size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_set_size stat;
  memset(&stat, 0, sizeof(dumpi_file_set_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_set_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_size);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.size, size);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_size);
  }
  retval = PMPI_File_set_size(file, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_size);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_set_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_SET_SIZE */

#ifndef DUMPI_SKIP_MPI_FILE_PREALLOCATE
int MPI_File_preallocate(MPI_File file, MPI_Offset size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_preallocate stat;
  memset(&stat, 0, sizeof(dumpi_file_preallocate));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_preallocate);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_preallocate);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.size, size);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_preallocate);
  }
  retval = PMPI_File_preallocate(file, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_preallocate);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_preallocate(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_preallocate);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_PREALLOCATE */

#ifndef DUMPI_SKIP_MPI_FILE_GET_SIZE
int MPI_File_get_size(MPI_File file, MPI_Offset *size) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_size stat;
  memset(&stat, 0, sizeof(dumpi_file_get_size));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_size);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_size);
  }
  retval = PMPI_File_get_size(file, size);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_size);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.size, *size);
    libdumpi_lock_io();
    dumpi_write_file_get_size(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_size);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_SIZE */

#ifndef DUMPI_SKIP_MPI_FILE_GET_GROUP
int MPI_File_get_group(MPI_File file, MPI_Group *group) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_group stat;
  memset(&stat, 0, sizeof(dumpi_file_get_group));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_group);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_group);
  }
  retval = PMPI_File_get_group(file, group);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_group);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_GROUP_FROM_MPI_GROUP(stat.group, *group);
    libdumpi_lock_io();
    dumpi_write_file_get_group(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_group);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_GROUP */

#ifndef DUMPI_SKIP_MPI_FILE_GET_AMODE
int MPI_File_get_amode(MPI_File file, int *amode) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_amode stat;
  memset(&stat, 0, sizeof(dumpi_file_get_amode));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_amode);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_amode);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_amode);
  }
  retval = PMPI_File_get_amode(file, amode);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_amode);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_FILEMODE_FROM_INT(stat.amode, *amode);
    libdumpi_lock_io();
    dumpi_write_file_get_amode(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_amode);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_AMODE */

#ifndef DUMPI_SKIP_MPI_FILE_SET_INFO
int MPI_File_set_info(MPI_File file, MPI_Info info) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_set_info stat;
  memset(&stat, 0, sizeof(dumpi_file_set_info));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_set_info);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_info);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_info);
  }
  retval = PMPI_File_set_info(file, info);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_info);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_set_info(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_info);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_SET_INFO */

#ifndef DUMPI_SKIP_MPI_FILE_GET_INFO
int MPI_File_get_info(MPI_File file, MPI_Info *info) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_info stat;
  memset(&stat, 0, sizeof(dumpi_file_get_info));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_info);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_info);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_info);
  }
  retval = PMPI_File_get_info(file, info);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_info);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, *info);
    libdumpi_lock_io();
    dumpi_write_file_get_info(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_info);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_INFO */

#ifndef DUMPI_SKIP_MPI_FILE_SET_VIEW
int MPI_File_set_view(MPI_File file, MPI_Offset offset, MPI_Datatype hosttype, MPI_Datatype filetype, const char *datarep, MPI_Info info) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_set_view stat;
  memset(&stat, 0, sizeof(dumpi_file_set_view));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_set_view);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_view);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.hosttype, hosttype);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.filetype, filetype);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.datarep, datarep);
    DUMPI_INFO_FROM_MPI_INFO(stat.info, info);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_view);
  }
  retval = PMPI_File_set_view(file, offset, hosttype, filetype, datarep, info);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_view);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_set_view(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(datarep != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.datarep);
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_view);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_SET_VIEW */

#ifndef DUMPI_SKIP_MPI_FILE_GET_VIEW
int MPI_File_get_view(MPI_File file, MPI_Offset *offset, MPI_Datatype *hosttype, MPI_Datatype *filetype, char *datarep) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_view stat;
  memset(&stat, 0, sizeof(dumpi_file_get_view));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_view);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_view);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_view);
  }
  retval = PMPI_File_get_view(file, offset, hosttype, filetype, datarep);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_view);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, *offset);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.hosttype, *hosttype);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.filetype, *filetype);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.datarep, datarep);
    libdumpi_lock_io();
    dumpi_write_file_get_view(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(datarep != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.datarep);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_view);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_VIEW */

#ifndef DUMPI_SKIP_MPI_FILE_READ_AT
int MPI_File_read_at(MPI_File file, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_at stat;
  memset(&stat, 0, sizeof(dumpi_file_read_at));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_at);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_at);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_at);
  }
  retval = PMPI_File_read_at(file, offset, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_at);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read_at(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_at);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_AT */

#ifndef DUMPI_SKIP_MPI_FILE_READ_AT_ALL
int MPI_File_read_at_all(MPI_File file, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_at_all stat;
  memset(&stat, 0, sizeof(dumpi_file_read_at_all));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_at_all);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_at_all);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_at_all);
  }
  retval = PMPI_File_read_at_all(file, offset, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_at_all);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read_at_all(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_at_all);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_AT_ALL */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_AT
int MPI_File_write_at(MPI_File file, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_at stat;
  memset(&stat, 0, sizeof(dumpi_file_write_at));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_at);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_at);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_at);
  }
  retval = PMPI_File_write_at(file, offset, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_at);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write_at(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_at);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_AT */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL
int MPI_File_write_at_all(MPI_File file, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_at_all stat;
  memset(&stat, 0, sizeof(dumpi_file_write_at_all));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_at_all);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_at_all);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_at_all);
  }
  retval = PMPI_File_write_at_all(file, offset, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_at_all);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write_at_all(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_at_all);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL */

#ifndef DUMPI_SKIP_MPI_FILE_IREAD_AT
int MPI_File_iread_at(MPI_File file, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype, MPIO_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_iread_at stat;
  memset(&stat, 0, sizeof(dumpi_file_iread_at));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_iread_at);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iread_at);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_iread_at);
  }
  retval = PMPI_File_iread_at(file, offset, buf, count, datatype, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iread_at);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_file_iread_at(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_iread_at);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_IREAD_AT */

#ifndef DUMPI_SKIP_MPI_FILE_IWRITE_AT
int MPI_File_iwrite_at(MPI_File file, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype, MPIO_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_iwrite_at stat;
  memset(&stat, 0, sizeof(dumpi_file_iwrite_at));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_iwrite_at);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iwrite_at);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_iwrite_at);
  }
  retval = PMPI_File_iwrite_at(file, offset, buf, count, datatype, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iwrite_at);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_file_iwrite_at(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_iwrite_at);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_IWRITE_AT */

#ifndef DUMPI_SKIP_MPI_FILE_READ
int MPI_File_read(MPI_File file, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read stat;
  memset(&stat, 0, sizeof(dumpi_file_read));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read);
  }
  retval = PMPI_File_read(file, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ */

#ifndef DUMPI_SKIP_MPI_FILE_READ_ALL
int MPI_File_read_all(MPI_File file, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_all stat;
  memset(&stat, 0, sizeof(dumpi_file_read_all));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_all);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_all);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_all);
  }
  retval = PMPI_File_read_all(file, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_all);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read_all(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_all);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_ALL */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE
int MPI_File_write(MPI_File file, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write stat;
  memset(&stat, 0, sizeof(dumpi_file_write));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write);
  }
  retval = PMPI_File_write(file, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_ALL
int MPI_File_write_all(MPI_File file, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_all stat;
  memset(&stat, 0, sizeof(dumpi_file_write_all));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_all);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_all);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_all);
  }
  retval = PMPI_File_write_all(file, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_all);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write_all(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_all);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_ALL */

#ifndef DUMPI_SKIP_MPI_FILE_IREAD
int MPI_File_iread(MPI_File file, void *buf, int count, MPI_Datatype datatype, MPIO_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_iread stat;
  memset(&stat, 0, sizeof(dumpi_file_iread));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_iread);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iread);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_iread);
  }
  retval = PMPI_File_iread(file, buf, count, datatype, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iread);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_file_iread(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_iread);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_IREAD */

#ifndef DUMPI_SKIP_MPI_FILE_IWRITE
int MPI_File_iwrite(MPI_File file, const void *buf, int count, MPI_Datatype datatype, MPIO_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_iwrite stat;
  memset(&stat, 0, sizeof(dumpi_file_iwrite));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_iwrite);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iwrite);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_iwrite);
  }
  retval = PMPI_File_iwrite(file, buf, count, datatype, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iwrite);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_file_iwrite(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_iwrite);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_IWRITE */

#ifndef DUMPI_SKIP_MPI_FILE_SEEK
int MPI_File_seek(MPI_File file, MPI_Offset offset, int whence) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_seek stat;
  memset(&stat, 0, sizeof(dumpi_file_seek));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_seek);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_seek);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_WHENCE_FROM_INT(stat.whence, whence);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_seek);
  }
  retval = PMPI_File_seek(file, offset, whence);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_seek);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_seek(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_seek);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_SEEK */

#ifndef DUMPI_SKIP_MPI_FILE_GET_POSITION
int MPI_File_get_position(MPI_File file, MPI_Offset *offset) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_position stat;
  memset(&stat, 0, sizeof(dumpi_file_get_position));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_position);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_position);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_position);
  }
  retval = PMPI_File_get_position(file, offset);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_position);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, *offset);
    libdumpi_lock_io();
    dumpi_write_file_get_position(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_position);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_POSITION */

#ifndef DUMPI_SKIP_MPI_FILE_GET_BYTE_OFFSET
int MPI_File_get_byte_offset(MPI_File file, MPI_Offset offset, MPI_Offset *bytes) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_byte_offset stat;
  memset(&stat, 0, sizeof(dumpi_file_get_byte_offset));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_byte_offset);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_byte_offset);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_byte_offset);
  }
  retval = PMPI_File_get_byte_offset(file, offset, bytes);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_byte_offset);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.bytes, *bytes);
    libdumpi_lock_io();
    dumpi_write_file_get_byte_offset(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_byte_offset);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_BYTE_OFFSET */

#ifndef DUMPI_SKIP_MPI_FILE_READ_SHARED
int MPI_File_read_shared(MPI_File file, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_shared stat;
  memset(&stat, 0, sizeof(dumpi_file_read_shared));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_shared);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_shared);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_shared);
  }
  retval = PMPI_File_read_shared(file, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_shared);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read_shared(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_shared);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_SHARED */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_SHARED
int MPI_File_write_shared(MPI_File file, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_shared stat;
  memset(&stat, 0, sizeof(dumpi_file_write_shared));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_shared);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_shared);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_shared);
  }
  retval = PMPI_File_write_shared(file, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_shared);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write_shared(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_shared);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_SHARED */

#ifndef DUMPI_SKIP_MPI_FILE_IREAD_SHARED
int MPI_File_iread_shared(MPI_File file, void *buf, int count, MPI_Datatype datatype, MPIO_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_iread_shared stat;
  memset(&stat, 0, sizeof(dumpi_file_iread_shared));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_iread_shared);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iread_shared);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_iread_shared);
  }
  retval = PMPI_File_iread_shared(file, buf, count, datatype, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iread_shared);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_file_iread_shared(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_iread_shared);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_IREAD_SHARED */

#ifndef DUMPI_SKIP_MPI_FILE_IWRITE_SHARED
int MPI_File_iwrite_shared(MPI_File file, const void *buf, int count, MPI_Datatype datatype, MPIO_Request *request) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_iwrite_shared stat;
  memset(&stat, 0, sizeof(dumpi_file_iwrite_shared));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_iwrite_shared);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iwrite_shared);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_iwrite_shared);
  }
  retval = PMPI_File_iwrite_shared(file, buf, count, datatype, request);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_iwrite_shared);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST(stat.request, *request);
    libdumpi_lock_io();
    dumpi_write_file_iwrite_shared(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_iwrite_shared);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_IWRITE_SHARED */

#ifndef DUMPI_SKIP_MPI_FILE_READ_ORDERED
int MPI_File_read_ordered(MPI_File file, void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_ordered stat;
  memset(&stat, 0, sizeof(dumpi_file_read_ordered));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_ordered);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_ordered);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_ordered);
  }
  retval = PMPI_File_read_ordered(file, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_ordered);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read_ordered(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_ordered);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_ORDERED */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_ORDERED
int MPI_File_write_ordered(MPI_File file, const void *buf, int count, MPI_Datatype datatype, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_ordered stat;
  memset(&stat, 0, sizeof(dumpi_file_write_ordered));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_ordered);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_ordered);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_ordered);
  }
  retval = PMPI_File_write_ordered(file, buf, count, datatype, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_ordered);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write_ordered(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_ordered);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_ORDERED */

#ifndef DUMPI_SKIP_MPI_FILE_SEEK_SHARED
int MPI_File_seek_shared(MPI_File file, MPI_Offset offset, int whence) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_seek_shared stat;
  memset(&stat, 0, sizeof(dumpi_file_seek_shared));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_seek_shared);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_seek_shared);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_WHENCE_FROM_INT(stat.whence, whence);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_seek_shared);
  }
  retval = PMPI_File_seek_shared(file, offset, whence);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_seek_shared);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_seek_shared(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_seek_shared);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_SEEK_SHARED */

#ifndef DUMPI_SKIP_MPI_FILE_GET_POSITION_SHARED
int MPI_File_get_position_shared(MPI_File file, MPI_Offset *offset) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_position_shared stat;
  memset(&stat, 0, sizeof(dumpi_file_get_position_shared));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_position_shared);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_position_shared);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_position_shared);
  }
  retval = PMPI_File_get_position_shared(file, offset);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_position_shared);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, *offset);
    libdumpi_lock_io();
    dumpi_write_file_get_position_shared(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_position_shared);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_POSITION_SHARED */

#ifndef DUMPI_SKIP_MPI_FILE_READ_AT_ALL_BEGIN
int MPI_File_read_at_all_begin(MPI_File file, MPI_Offset offset, void *buf, int count, MPI_Datatype datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_at_all_begin stat;
  memset(&stat, 0, sizeof(dumpi_file_read_at_all_begin));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_at_all_begin);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_at_all_begin);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_at_all_begin);
  }
  retval = PMPI_File_read_at_all_begin(file, offset, buf, count, datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_at_all_begin);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_read_at_all_begin(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_at_all_begin);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_AT_ALL_BEGIN */

#ifndef DUMPI_SKIP_MPI_FILE_READ_AT_ALL_END
int MPI_File_read_at_all_end(MPI_File file, void *buf, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_at_all_end stat;
  memset(&stat, 0, sizeof(dumpi_file_read_at_all_end));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_at_all_end);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_at_all_end);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_at_all_end);
  }
  retval = PMPI_File_read_at_all_end(file, buf, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_at_all_end);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read_at_all_end(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_at_all_end);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_AT_ALL_END */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL_BEGIN
int MPI_File_write_at_all_begin(MPI_File file, MPI_Offset offset, const void *buf, int count, MPI_Datatype datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_at_all_begin stat;
  memset(&stat, 0, sizeof(dumpi_file_write_at_all_begin));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_at_all_begin);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_at_all_begin);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT64T_FROM_MPI_OFFSET(stat.offset, offset);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_at_all_begin);
  }
  retval = PMPI_File_write_at_all_begin(file, offset, buf, count, datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_at_all_begin);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_write_at_all_begin(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_at_all_begin);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL_BEGIN */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL_END
int MPI_File_write_at_all_end(MPI_File file, const void *buf, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_at_all_end stat;
  memset(&stat, 0, sizeof(dumpi_file_write_at_all_end));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_at_all_end);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_at_all_end);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_at_all_end);
  }
  retval = PMPI_File_write_at_all_end(file, buf, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_at_all_end);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write_at_all_end(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_at_all_end);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL_END */

#ifndef DUMPI_SKIP_MPI_FILE_READ_ALL_BEGIN
int MPI_File_read_all_begin(MPI_File file, void *buf, int count, MPI_Datatype datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_all_begin stat;
  memset(&stat, 0, sizeof(dumpi_file_read_all_begin));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_all_begin);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_all_begin);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_all_begin);
  }
  retval = PMPI_File_read_all_begin(file, buf, count, datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_all_begin);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_read_all_begin(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_all_begin);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_ALL_BEGIN */

#ifndef DUMPI_SKIP_MPI_FILE_READ_ALL_END
int MPI_File_read_all_end(MPI_File file, void *buf, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_all_end stat;
  memset(&stat, 0, sizeof(dumpi_file_read_all_end));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_all_end);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_all_end);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_all_end);
  }
  retval = PMPI_File_read_all_end(file, buf, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_all_end);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read_all_end(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_all_end);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_ALL_END */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_ALL_BEGIN
int MPI_File_write_all_begin(MPI_File file, const void *buf, int count, MPI_Datatype datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_all_begin stat;
  memset(&stat, 0, sizeof(dumpi_file_write_all_begin));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_all_begin);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_all_begin);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_all_begin);
  }
  retval = PMPI_File_write_all_begin(file, buf, count, datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_all_begin);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_write_all_begin(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_all_begin);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_ALL_BEGIN */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_ALL_END
int MPI_File_write_all_end(MPI_File file, const void *buf, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_all_end stat;
  memset(&stat, 0, sizeof(dumpi_file_write_all_end));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_all_end);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_all_end);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_all_end);
  }
  retval = PMPI_File_write_all_end(file, buf, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_all_end);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write_all_end(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_all_end);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_ALL_END */

#ifndef DUMPI_SKIP_MPI_FILE_READ_ORDERED_BEGIN
int MPI_File_read_ordered_begin(MPI_File file, void *buf, int count, MPI_Datatype datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_ordered_begin stat;
  memset(&stat, 0, sizeof(dumpi_file_read_ordered_begin));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_ordered_begin);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_ordered_begin);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_ordered_begin);
  }
  retval = PMPI_File_read_ordered_begin(file, buf, count, datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_ordered_begin);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_read_ordered_begin(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_ordered_begin);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_ORDERED_BEGIN */

#ifndef DUMPI_SKIP_MPI_FILE_READ_ORDERED_END
int MPI_File_read_ordered_end(MPI_File file, void *buf, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_read_ordered_end stat;
  memset(&stat, 0, sizeof(dumpi_file_read_ordered_end));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_read_ordered_end);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_ordered_end);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_ordered_end);
  }
  retval = PMPI_File_read_ordered_end(file, buf, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_read_ordered_end);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_read_ordered_end(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_read_ordered_end);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_READ_ORDERED_END */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_ORDERED_BEGIN
int MPI_File_write_ordered_begin(MPI_File file, const void *buf, int count, MPI_Datatype datatype) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_ordered_begin stat;
  memset(&stat, 0, sizeof(dumpi_file_write_ordered_begin));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_ordered_begin);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_ordered_begin);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_ordered_begin);
  }
  retval = PMPI_File_write_ordered_begin(file, buf, count, datatype);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_ordered_begin);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_write_ordered_begin(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_ordered_begin);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_ORDERED_BEGIN */

#ifndef DUMPI_SKIP_MPI_FILE_WRITE_ORDERED_END
int MPI_File_write_ordered_end(MPI_File file, const void *buf, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_write_ordered_end stat;
  memset(&stat, 0, sizeof(dumpi_file_write_ordered_end));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_write_ordered_end);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_ordered_end);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_ordered_end);
  }
  retval = PMPI_File_write_ordered_end(file, buf, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_write_ordered_end);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    libdumpi_lock_io();
    dumpi_write_file_write_ordered_end(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPI_File_write_ordered_end);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_WRITE_ORDERED_END */

#ifndef DUMPI_SKIP_MPI_FILE_GET_TYPE_EXTENT
int MPI_File_get_type_extent(MPI_File file, MPI_Datatype datatype, MPI_Aint *extent) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_type_extent stat;
  memset(&stat, 0, sizeof(dumpi_file_get_type_extent));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_type_extent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_type_extent);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_DATATYPE_FROM_MPI_DATATYPE(stat.datatype, datatype);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_type_extent);
  }
  retval = PMPI_File_get_type_extent(file, datatype, extent);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_type_extent);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_MPI_AINT(stat.extent, *extent);
    libdumpi_lock_io();
    dumpi_write_file_get_type_extent(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_type_extent);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_TYPE_EXTENT */

#ifndef DUMPI_SKIP_MPI_REGISTER_DATAREP
int MPI_Register_datarep(const char *name, MPI_Datarep_conversion_function *readfun, MPI_Datarep_conversion_function *writefun, MPI_Datarep_extent_function *extentfun, void *state) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_register_datarep stat;
  memset(&stat, 0, sizeof(dumpi_register_datarep));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_Register_datarep);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Register_datarep);
    DUMPI_CHAR_FROM_CHAR_ARRAY_1(DUMPI_CSTRING, stat.name, name);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_Register_datarep);
  }
  retval = PMPI_Register_datarep(name, readfun, writefun, extentfun, state);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_Register_datarep);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_register_datarep(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    if(name != NULL) DUMPI_FREE_CHAR_FROM_CHAR(stat.name);
    DUMPI_STOP_OVERHEAD(DUMPI_Register_datarep);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_REGISTER_DATAREP */

#ifndef DUMPI_SKIP_MPI_FILE_SET_ATOMICITY
int MPI_File_set_atomicity(MPI_File file, int flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_set_atomicity stat;
  memset(&stat, 0, sizeof(dumpi_file_set_atomicity));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_set_atomicity);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_atomicity);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_INT_FROM_INT(stat.flag, flag);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_atomicity);
  }
  retval = PMPI_File_set_atomicity(file, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_set_atomicity);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_set_atomicity(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_set_atomicity);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_SET_ATOMICITY */

#ifndef DUMPI_SKIP_MPI_FILE_GET_ATOMICITY
int MPI_File_get_atomicity(MPI_File file, int *flag) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_get_atomicity stat;
  memset(&stat, 0, sizeof(dumpi_file_get_atomicity));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_get_atomicity);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_atomicity);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_atomicity);
  }
  retval = PMPI_File_get_atomicity(file, flag);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_get_atomicity);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    libdumpi_lock_io();
    dumpi_write_file_get_atomicity(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_get_atomicity);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_GET_ATOMICITY */

#ifndef DUMPI_SKIP_MPI_FILE_SYNC
int MPI_File_sync(MPI_File file) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpi_file_sync stat;
  memset(&stat, 0, sizeof(dumpi_file_sync));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPI_File_sync);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_sync);
    DUMPI_FILE_FROM_MPI_FILE(stat.file, file);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPI_File_sync);
  }
  retval = PMPI_File_sync(file);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPI_File_sync);
    DUMPI_STOP_TIME(cpu, wall);
    libdumpi_lock_io();
    dumpi_write_file_sync(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    libdumpi_unlock_io();
    DUMPI_STOP_OVERHEAD(DUMPI_File_sync);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPI_FILE_SYNC */

#ifndef DUMPI_SKIP_MPIO_TEST
int MPIO_Test(MPIO_Request *request, int *flag, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpio_test stat;
  memset(&stat, 0, sizeof(dumpio_test));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPIO_Test);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Test);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST(stat.request, *request);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPIO_Test);
  }
  retval = PMPIO_Test(request, flag, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Test);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    if(*flag!=0)    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    dumpio_write_test(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPIO_Test);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPIO_TEST */

#ifndef DUMPI_SKIP_MPIO_WAIT
int MPIO_Wait(MPIO_Request *request, MPI_Status *status) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpio_wait stat;
  memset(&stat, 0, sizeof(dumpio_wait));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPIO_Wait);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Wait);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST(stat.request, *request);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPIO_Wait);
  }
  retval = PMPIO_Wait(request, status);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Wait);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_PTR(stat.status, status);
    dumpio_write_wait(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    if(status != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.status);
    DUMPI_STOP_OVERHEAD(DUMPIO_Wait);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPIO_WAIT */

#ifndef DUMPI_SKIP_MPIO_TESTALL
int MPIO_Testall(int count, MPIO_Request *requests, int *flag, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpio_testall stat;
  memset(&stat, 0, sizeof(dumpio_testall));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPIO_Testall);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Testall);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPIO_Testall);
  }
  retval = PMPIO_Testall(count, requests, flag, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Testall);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    if(*flag!=0)    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(count, stat.statuses, statuses);
    dumpio_write_testall(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    if(requests != NULL) DUMPIO_FREE_REQUEST_FROM_MPIO_REQUEST(stat.requests);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPIO_Testall);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPIO_TESTALL */

#ifndef DUMPI_SKIP_MPIO_WAITALL
int MPIO_Waitall(int count, MPIO_Request *requests, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpio_waitall stat;
  memset(&stat, 0, sizeof(dumpio_waitall));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPIO_Waitall);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Waitall);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPIO_Waitall);
  }
  retval = PMPIO_Waitall(count, requests, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Waitall);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(count, stat.statuses, statuses);
    dumpio_write_waitall(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    if(requests != NULL) DUMPIO_FREE_REQUEST_FROM_MPIO_REQUEST(stat.requests);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPIO_Waitall);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPIO_WAITALL */

#ifndef DUMPI_SKIP_MPIO_TESTANY
int MPIO_Testany(int count, MPIO_Request *requests, int *index, int *flag, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpio_testany stat;
  memset(&stat, 0, sizeof(dumpio_testany));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPIO_Testany);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Testany);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPIO_Testany);
  }
  retval = PMPIO_Testany(count, requests, index, flag, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Testany);
    DUMPI_STOP_TIME(cpu, wall);
    if(*flag!=0)    DUMPI_INT_FROM_INT(stat.index, *index);
    DUMPI_INT_FROM_INT(stat.flag, *flag);
    if(*flag!=0)    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(count, stat.statuses, statuses);
    dumpio_write_testany(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    if(requests != NULL) DUMPIO_FREE_REQUEST_FROM_MPIO_REQUEST(stat.requests);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPIO_Testany);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPIO_TESTANY */

#ifndef DUMPI_SKIP_MPIO_WAITANY
int MPIO_Waitany(int count, MPIO_Request *requests, int *index, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpio_waitany stat;
  memset(&stat, 0, sizeof(dumpio_waitany));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPIO_Waitany);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Waitany);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPIO_Waitany);
  }
  retval = PMPIO_Waitany(count, requests, index, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Waitany);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.index, *index);
    if(*index != MPI_UNDEFINED)    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(count, stat.statuses, statuses);
    dumpio_write_waitany(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    if(requests != NULL) DUMPIO_FREE_REQUEST_FROM_MPIO_REQUEST(stat.requests);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPIO_Waitany);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPIO_WAITANY */

#ifndef DUMPI_SKIP_MPIO_WAITSOME
int MPIO_Waitsome(int count, MPIO_Request *requests, int *outcount, int *indices, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpio_waitsome stat;
  memset(&stat, 0, sizeof(dumpio_waitsome));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPIO_Waitsome);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Waitsome);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPIO_Waitsome);
  }
  retval = PMPIO_Waitsome(count, requests, outcount, indices, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Waitsome);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.outcount, *outcount);
    DUMPI_INT_FROM_INT_ARRAY_1(*outcount, stat.indices, indices);
    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(*outcount, stat.statuses, statuses);
    dumpio_write_waitsome(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    if(requests != NULL) DUMPIO_FREE_REQUEST_FROM_MPIO_REQUEST(stat.requests);
    if(indices != NULL) DUMPI_FREE_INT_FROM_INT(stat.indices);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPIO_Waitsome);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPIO_WAITSOME */

#ifndef DUMPI_SKIP_MPIO_TESTSOME
int MPIO_Testsome(int count, MPIO_Request *requests, int *outcount, int *indices, MPI_Status *statuses) {
  int16_t thread;
  dumpi_time cpu, wall;
  int retval;
  dumpio_testsome stat;
  memset(&stat, 0, sizeof(dumpio_testsome));
  thread = libdumpi_get_thread_id();
  DUMPI_INSERT_PREAMBLE(DUMPIO_Testsome);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Testsome);
    DUMPI_INT_FROM_INT(stat.count, count);
    DUMPIO_REQUEST_FROM_MPIO_REQUEST_ARRAY_1(count, stat.requests, requests);
    DUMPI_START_TIME(cpu, wall);
    DUMPI_STOP_OVERHEAD(DUMPIO_Testsome);
  }
  retval = PMPIO_Testsome(count, requests, outcount, indices, statuses);
  if(profiling) {
    DUMPI_START_OVERHEAD(DUMPIO_Testsome);
    DUMPI_STOP_TIME(cpu, wall);
    DUMPI_INT_FROM_INT(stat.outcount, *outcount);
    DUMPI_INT_FROM_INT_ARRAY_1(*outcount, stat.indices, indices);
    DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(*outcount, stat.statuses, statuses);
    dumpio_write_testsome(&stat, thread, &cpu, &wall, dumpi_global->perf, dumpi_global->output, dumpi_global->profile);
    if(requests != NULL) DUMPIO_FREE_REQUEST_FROM_MPIO_REQUEST(stat.requests);
    if(indices != NULL) DUMPI_FREE_INT_FROM_INT(stat.indices);
    if(statuses != NULL) DUMPI_FREE_STATUS_FROM_MPI_STATUS(stat.statuses);
    DUMPI_STOP_OVERHEAD(DUMPIO_Testsome);
  }
  DUMPI_INSERT_POSTAMBLE;
  return retval;
}
#endif /* DUMPI_SKIP_MPIO_TESTSOME */

