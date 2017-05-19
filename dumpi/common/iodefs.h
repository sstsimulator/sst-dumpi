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

#ifndef DUMPI_COMMON_IODEFS_H
#define DUMPI_COMMON_IODEFS_H

#include <dumpi/common/io.h>
#include <dumpi/common/types.h>
#include <dumpi/common/constants.h>
#include <dumpi/common/settings.h>
#include <dumpi/common/debugflags.h>
#include <dumpi/common/funcs.h>
#include <dumpi/dumpiconfig.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#ifndef DUMPI_ON_REDSTORM
#include <arpa/inet.h>
#endif /* ! DUMPI_ON_REDSTORM */

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common_io
   * \defgroup common_io_internal common/io/internal:  Internal functions \
   *     and utilities for shared I/O routines.
   */
  /*@{*/ /* plop the documentation into the current module */

  /**
   * Free a memory buffer.
   */
  void dumpi_free_membuf(struct dumpi_memory_buffer *buf);

  /**
   * Flush the memory buffer.
   * Writes and sets the buffer position to zero iff file is not NULL.
   */
  void dumpi_membuf_flush(dumpi_profile *profile);

  /**
   * Write to the memory buffer.
   * If the memory buffer overflows and the input file is not NULL, this
   * forces a dumpi_membuf_flush.
   * If the memory buffer overflows and the input file is NULL, the buffer
   * is doubled in size.
   */
  void dumpi_membuf_write(dumpi_profile *profile, const void *ptr, size_t size,
			  size_t nmemb);

  /**
   * Despite the name, this reads straight from the file (we no longer do
   * our own input file buffering).
   */
  void dumpi_membuf_read(dumpi_profile *profile, void *ptr,
			 size_t size, size_t nmemb);

  /**
   * If file is not NULL, return (ftello(file) + dumpi_membuf_pos())
   * else return dumpi_membuf_pos().
   */
  off_t dumpi_membuf_tell(dumpi_profile *profile);

  /**
   * Utility definition (dating back to when we used compressed files).
   * May be deprecated at a later time.
   */
#define DUMPI_FLUSH(PROFILE) dumpi_membuf_flush(PROFILE)

  /**
   * Utility definition (dating back to when we used compressed files).
   * May be deprecated at a later time.
   */
#define DUMPI_FWRITE(PROFILE, PTR, SIZ, NMB)		\
  dumpi_membuf_write(PROFILE, PTR, SIZ, NMB)

  /**
   * Utility definition (dating back to when we used compressed files).
   * May be deprecated at a later time.
   */
#define DUMPI_FREAD(PROFILE, PTR, SIZ, NMB)	\
  dumpi_membuf_read(PROFILE, PTR, SIZ, NMB)
  /*#define DUMPI_FREAD(PTR, SIZ, NMB, STR) assert(fread(PTR, SIZ, NMB, STR)  == NMB)*/

  /**
   * Utility definition (dating back to when we used compressed files).
   * May be deprecated at a later time.
   */
#define DUMPI_GETC(FILE) fgetc(FILE)

  /**
   * Utility definition (dating back to when we used compressed files).
   * May be deprecated at a later time.
   */
#define DUMPI_WRITE_TELL(PROFILE) dumpi_membuf_tell(PROFILE)

  /**
   * Utility definition (dating back to when we used compressed files).
   * May be deprecated at a later time.
   */
#define DUMPI_READ_TELL(PROFILE) ftello(PROFILE->file)

  /**
   * Utility definition (dating back to when we used compressed files).
   * May be deprecated at a later time.
   */
#define DUMPI_SEEK(PROFILE, OFFSET, WHENCE)	\
  fseeko(PROFILE->file, OFFSET, WHENCE)


  /** Utility routine to get a 8 bit integer from a binary stream. */
  static inline uint8_t get8(dumpi_profile *fp) {
    uint8_t scratch;
    off_t fpos = DUMPI_READ_TELL(fp);
    DUMPI_FREAD(fp, &scratch, sizeof(uint8_t), 1);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO_VERBOSE)
      fprintf(stderr, "[DUMPI-IO-VERBOSE] get8: Retrieved value %d (0x%02x) "
	      "at file offset 0x%llx\n", (int)scratch, (int)scratch,
	      (long long)fpos);
    return scratch;
  }

  /** Utility routine to write a 8 bit integer to a binary stream. */
  static inline void put8(dumpi_profile *fp, uint8_t value) {
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO_VERBOSE)
      fprintf(stderr, "[DUMPI-IO-VERBOSE] put8: Putting value %d (0x%02x) "
	      "at file offset 0x%llx\n", (int)value, (int)value,
	      (long long)DUMPI_WRITE_TELL(fp));
    DUMPI_FWRITE(fp, &value, sizeof(uint8_t), 1);
  }

  /** Utility routine to get a 16 bit integer from a binary stream. */
  static inline uint16_t get16(dumpi_profile *fp) {
    uint16_t scratch, retval;
    off_t fpos = DUMPI_READ_TELL(fp);
    DUMPI_FREAD(fp, &scratch, sizeof(uint16_t), 1);
    retval = ntohs(scratch);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO_VERBOSE)
      fprintf(stderr, "[DUMPI-IO-VERBOSE] get16: Retrieved value %hu from "
	      "stream value 0x%04hx at file offset 0x%llx\n", 
	      retval, scratch, (long long)fpos);
    return retval;
  }

  /** Utility routine to write a 16 bit integer to a binary stream. */
  static inline void put16(dumpi_profile *fp, uint16_t value) {
    uint16_t bevalue = htons(value);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO_VERBOSE)
      fprintf(stderr, "[DUMPI-IO-VERBOSE] put16: Putting value %hu as "
	      "stream value 0x%04hx at file offset 0x%llx\n", 
	      value, bevalue, (long long)DUMPI_WRITE_TELL(fp));
    DUMPI_FWRITE(fp, &bevalue, sizeof(uint16_t), 1);
  }

  /** Utility routine to get a 32 bit integer from a binary stream. */
  static inline uint32_t get32(dumpi_profile *fp) {
    uint32_t scratch, retval;
    off_t fpos = DUMPI_READ_TELL(fp);
    DUMPI_FREAD(fp, &scratch, sizeof(uint32_t), 1);
    retval = ntohl(scratch);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO_VERBOSE)
      fprintf(stderr, "[DUMPI-IO-VERBOSE] get32: Retrieved value %u from "
	      "stream value 0x%08x at file offset 0x%llx\n", 
	      retval, scratch, (long long)fpos);
    return retval;
  }

  /** Utility routine to write a 32 bit integer to a binary stream. */
  static inline void put32(dumpi_profile *fp, uint32_t value) {
    uint32_t bevalue = htonl(value);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO_VERBOSE)
      fprintf(stderr, "[DUMPI-IO-VERBOSE] put32: Putting value %u as "
	      "stream value 0x%08x at file offset 0x%llx\n", 
	      value, bevalue, (long long)DUMPI_WRITE_TELL(fp));
    DUMPI_FWRITE(fp, &bevalue, sizeof(uint32_t), 1);
  }

  /** Utility routine to get an array of 32-bit values. */
  static inline void get32arr(dumpi_profile *fp,
			      int32_t *count, int32_t **arr)
  {
    int i;
    *count = get32(fp);
    if(*count > 0)
      *arr = (int32_t*)malloc(*count * sizeof(int32_t));
    else
      *arr = NULL;
    for(i = 0; i < *count; ++i) {
      (*arr)[i] = get32(fp);
    }
  }

  /** Utility routine to write an array of 32-bit values. */
  static inline void put32arr(dumpi_profile *fp,
			      int32_t count, const int32_t *arr)
  {
    int i;
    put32(fp, count);
    for(i = 0; i < count; ++i) {
      put32(fp, arr[i]);
    }
  }

  /** Utility routine to read an array of characters. */
  static inline void getchararr(dumpi_profile *fp, int32_t *count, char **arr) {
    off_t startpos, endpos;
    assert(arr != NULL);
    startpos = DUMPI_READ_TELL(fp);
    *count = get32(fp);
    *arr = (char*)calloc((*count+1), sizeof(char));
    assert(*arr != NULL);
    if(*count > 0)
      DUMPI_FREAD(fp, *arr, (*count)*sizeof(char), 1);
    endpos = DUMPI_READ_TELL(fp);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO_VERBOSE)
      fprintf(stderr, "[DUMPI-IO-VERBOSE] getchararr:  Read std::string \"%s\" with "
	      "length %d starting at file offset 0x%llx and ending at 0x%llx\n",
	      *arr, *count, (long long)startpos, (long long)endpos);
  }

  /** Utility routine to write an array of 32-bit values. */
  static inline void putchararr(dumpi_profile *fp, 
				int64_t countarg, const char *arr)
  {
    int32_t count = (int32_t) countarg;
    off_t startpos, endpos;
    startpos = DUMPI_WRITE_TELL(fp);
    put32(fp, count);
    DUMPI_FWRITE(fp, arr, count*sizeof(char), 1);
    endpos = DUMPI_WRITE_TELL(fp);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO_VERBOSE)
      fprintf(stderr, "[DUMPI-IO-VERBOSE] putchararr:  Wrote std::string \"%s\" with"
	      " length %d starting at file offset 0x%llx and ending at 0x%llx\n",
	      arr, count, (long long)startpos, (long long)endpos);
  }

  /* Utility routine to write a 2D array of 32-bit values. */
  /*
    static inline
    void put32arr2(dumpi_profile *fp,
                   int32_t rows, int32_t cols, const int32_t * const* arr)
    {
      int i, j;
      put32(fp, rows);
      put32(fp, cols);
      for(i = 0; i < rows; ++i) {
        for(j = 0; j < cols; ++j) {
          put32(fp, arr[i][j]);
        }
      }
    }
  */

  /* Utility routine to read a 2D array of 32-bit values. */
  /*
    static inline
    void get32arr2(dumpi_profile *fp, int32_t *rows, int32_t *cols,
                   int32_t **arr)
    {
      int i, j;
      *rows = get32(fp);
      *cols = get32(fp);
      arr = (int**)calloc((*rows)+1, sizeof(int32_t*));
      assert(arr != NULL);
      for(i = 0; i < *rows; ++i) {
        arr[i] = (int*)malloc((*cols)*sizeof(int32_t));
        for(j = 0; j < *cols; ++j) {
          arr[i][j] = get32(fp);
        }
      }
    }
  */
  
  /** Utility routine to get a 64 bit integer from a binary stream. */
  static inline uint64_t get64(dumpi_profile *fp) {
    uint64_t value;
    uint32_t high32, low32;
    DUMPI_FREAD(fp, &high32, sizeof(uint32_t), 1);
    DUMPI_FREAD(fp, &low32, sizeof(uint32_t), 1);
    /* Pack the high and low values into a 64-bit value). */
    value = ntohl(high32);
    value <<= 32;
    value |= ntohl(low32);
    return value;
  }

  /** Utility routine to write a 64 bit integer to a binary stream. */
  static inline void put64(dumpi_profile *fp, uint64_t value) {
    uint32_t high32, low32;
    high32 = htonl((uint32_t)(value >> 32));
    low32  = htonl((uint32_t)(value));
    DUMPI_FWRITE(fp, &high32, sizeof(uint32_t), 1);
    DUMPI_FWRITE(fp, &low32,  sizeof(uint32_t), 1);
  }

  /** Utility routine to store a single request value */
  static inline void put_single_request(dumpi_profile *fp,
					dumpi_request request)
  {
    put32(fp, request);
  }

  /** Utility routine to retrieve a single request value */
  static inline dumpi_request get_single_request(dumpi_profile *fp) {
    return get32(fp);
  }

  /** Write an array of requests to the stream */
  static inline void put_requests(dumpi_profile *fp,
				  int count, dumpi_request *req)
  {
    int i;
    put32(fp, count);
    for(i = 0; i < count; ++i)
      put_single_request(fp, req[i]);
  }

  /* Retrieve an array of request handles from the stream */
  static inline void get_requests(dumpi_profile *fp,
				  int *count, dumpi_request **req)
  {
    int i;
    *count = get32(fp);
    *req = (dumpi_request*)malloc(*count * sizeof(dumpi_request));
    assert(req);
    for(i = 0; i < *count; ++i)
      (*req)[i] = get_single_request(fp);
  }

  /** Read the token for next function. */
  static inline dumpi_function dumpi_read_next_function(dumpi_profile *profile) 
  {
    uint16_t next = get16(profile);
    /*fprintf(stderr, "Encountered function label %d at offset 0x%llx\n",
              (int)next, (long long)DUMPI_READ_TELL(profile));*/
    assert(next <= DUMPI_END_OF_STREAM);  
    if(next != DUMPI_ALL_FUNCTIONS)
      return ((dumpi_function)next);
    return (dumpi_function)NULL;
  }

  /** Utility routine to put a function label. */
  static inline void put_function_label(dumpi_profile *profile,
					dumpi_function fun)
  {
    put16(profile, (uint16_t)fun);
  }

  /** Utility routine to get a function label. */
  static inline dumpi_function get_function_label(dumpi_profile *profile) {
    return ((dumpi_function)get16(profile));
  }

  /** Utility routine to store a config mask. */
  static inline void put_config_mask(dumpi_profile *profile,
				     const dumpi_perfinfo *perf,
				     const dumpi_outputs *output)
  {
    uint8_t mask = (uint8_t)(output->timestamps | output->statuses);
    if(output->perfinfo && (perf != NULL && perf->count > 0))
      mask |= DUMPI_PERFINFO_MASK;
    /* Added to output thread index. */
    mask |= DUMPI_THREADID_MASK;
    put8(profile, mask);
  }

  /** Utility routine to retrieve a config mask. */
  static inline uint8_t get_config_mask(dumpi_profile *profile) {
    uint8_t mask = get8(profile);
    return mask;
  }

  /** Test whether we are reading/writing CPU clock information */
#define DO_TIME_CPU(MASK)  (MASK & DUMPI_TIME_CPU)

  /** Test whether we are reading/writing wall time information */
#define DO_TIME_WALL(MASK) (MASK & DUMPI_TIME_WALL)

  /** Test whether we are reading/writing performance counter information */
#define DO_PERFINFO(MASK)  (MASK & DUMPI_PERFINFO_MASK)

  /** Utility routine write timestamps to the stream */
  static inline void put_times(dumpi_profile *profile,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       uint8_t config_mask)
  {
    if(DO_TIME_CPU(config_mask)) {
      put16(profile, (uint16_t)(cpu->start.sec - profile->cpu_time_offset));
      put32(profile, cpu->start.nsec);
      put16(profile, (uint16_t)(cpu->stop.sec - profile->cpu_time_offset));
      put32(profile, cpu->stop.nsec);
    }
    if(DO_TIME_WALL(config_mask)) {
      put16(profile, (uint16_t)(wall->start.sec - profile->wall_time_offset));
      put32(profile, wall->start.nsec);
      put16(profile, (uint16_t)(wall->stop.sec - profile->wall_time_offset));
      put32(profile, wall->stop.nsec);
    }
  }

  /* Utility routine to read timestamps from the stream */
  static inline void get_times(dumpi_profile *profile, dumpi_time *cpu,
			       dumpi_time *wall, uint8_t config_mask)
  {
    if(DO_TIME_CPU(config_mask)) {
      cpu->start.sec  = get16(profile) + profile->cpu_time_offset;
      cpu->start.nsec = get32(profile);
      cpu->stop.sec   = get16(profile) + profile->cpu_time_offset;
      cpu->stop.nsec  = get32(profile);
    }
    else {
      cpu->start.sec = cpu->start.nsec = 0;
      cpu->stop.sec  = cpu->stop.nsec  = 0;
    }
    if(DO_TIME_WALL(config_mask)) {
      wall->start.sec  = get16(profile) + profile->wall_time_offset;
      wall->start.nsec = get32(profile);
      wall->stop.sec   = get16(profile) + profile->wall_time_offset;
      wall->stop.nsec  = get32(profile);
    }
    else {
      wall->start.sec = wall->start.nsec = 0;
      wall->stop.sec  = wall->stop.nsec  = 0;
    }
  }

  /** Utility routine to write a std::string. */
  static inline void put_string(dumpi_profile *profile, const char *str) {
    uint16_t len = (str ? (uint16_t)strlen(str) : 0);
    put16(profile, len);
    DUMPI_FWRITE(profile, str, sizeof(char), len);
  }

  /** Utility routine to read (and allocate) a std::string. */
  static inline char* get_string(dumpi_profile *fp) {
    char *str;
    uint16_t len = get16(fp);
    str = (char*)malloc(len+1); str[len] = '\0';
    DUMPI_FREAD(fp, str, sizeof(char), len);
    return str;
  }

  /** Utility routine to read a std::string into a preallocated buffer. */
  static inline void get_fix_string(dumpi_profile *fp,
				    char *dest, size_t maxsize)
  {
    uint16_t len = get16(fp);
    assert(len < maxsize);
    DUMPI_FREAD(fp, dest, sizeof(char), len);
    dest[len] = '\0';
  }

  /** Put an array of std::strings. */
  static inline void put_string_arr(dumpi_profile *fp, int count, char **arr) {
    int i;
    put32(fp, count);
    for(i = 0; i < count; ++i)
      put_string(fp, arr[i]);
  }

  /** Get an array of std::strings */
  static inline void get_string_arr(dumpi_profile *fp, int *count, char ***arr){
    int i;
    *count = get32(fp);
    *arr = (char**)malloc(*count * sizeof(char*));
    for(i = 0; i < *count; ++i) {
      (*arr)[i] = get_string(fp);
    }
  }

  /** Write PAPI performance counter state to the stream */
  static inline void put_perfinfo(dumpi_profile *profile,
				  const dumpi_perfinfo *perf,
				  const dumpi_outputs *output)
  {
    assert(profile);
    if(output->perfinfo && (perf != NULL && perf->count > 0)) {
      int i;
      put8(profile, (uint8_t)perf->count);
      for(i = 0; i < perf->count; ++i) {
	/* put_string(profile, perf->counter_tag[i]); */
	put64(profile, perf->invalue[i]);
	put64(profile, perf->outvalue[i]);
      }
    }
  }

  /** Read PAPI performance counter state from the stream */
  static inline void get_perfinfo(dumpi_profile *profile,
				  dumpi_perfinfo *perf, uint8_t config_mask)
  {
    perf->count = 0;
    if(DO_PERFINFO(config_mask)) {
      int i;
      perf->count = get8(profile);
      /* Read the std::string labels for the performance counters */
      dumpi_read_perfctr_labels(profile, &perf->count, perf->counter_tag,
                                DUMPI_MAX_PERFCTR_NAME);
      /* Done reading std::string labels -- get the actual values */
      for(i = 0; i < perf->count; ++i) {
        perf->invalue[i] = get64(profile);
        perf->outvalue[i] = get64(profile);
      }
    }
  }

  /** Utility routine to put statuses as requested by the mask */
  static inline void put_statuses(dumpi_profile *profile,
				  int count, const dumpi_status *statuses, 
				  const dumpi_outputs *output)
  {
    if(output->statuses) {
      if(statuses != NULL) {
        int i;
        put32(profile, count);
        for(i = 0; i < count; ++i) {
          put32(profile, statuses[i].bytes);
          put32(profile, statuses[i].source);
          put8(profile, statuses[i].cancelled);
          put8(profile, statuses[i].error);
	  /* There was a mistake in versions prior to 0.6.3 where
	   * the tag associated with a status wasn't being saved/restored */
	  if(dumpi_have_version(profile->version, 0, 6, 3)) {
	    put32(profile, statuses[i].tag);
	  }
        }
      }
      else {
        put32(profile, 0);
      }
    }
  }

  /** Utility routine to get statuses as requested by the mask */
  static inline dumpi_status* get_statuses(dumpi_profile *profile,
					   uint8_t config_mask)
  {
    dumpi_status *statuses = NULL;
    if(config_mask & DUMPI_ENABLE) {
      int i;
      int count = get32(profile);
      if(count > 0) {
        statuses = (dumpi_status*)malloc(count * sizeof(dumpi_status));
        for(i = 0; i < count; ++i) {
          statuses[i].bytes = get32(profile);
          statuses[i].source = get32(profile);
          statuses[i].cancelled = get8(profile);
          statuses[i].error = get8(profile);
	  /* There was a mistake in versions prior to 0.6.3 where
	   * the tag associated with a status wasn't being saved/restored */
	  if(dumpi_have_version(profile->version, 0, 6, 3)) {
	    statuses[i].tag = get32(profile);
	  }
	  else {
	    statuses[i].tag = DUMPI_ANY_TAG;
	  }
        }
      }
      else {
        statuses = DUMPI_STATUS_IGNORE;
      }
    }
    return statuses;
  }
  
  /** Shared back-end stuff when starting a new profiled call */
#define STARTWRITE(PROFILE, LABEL)					\
  assert(val && profile);                                               \
  if(output->function[LABEL]) {                                         \
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO) {                             \
      fprintf(stderr, "[DUMPI-IO] Writing record for " #LABEL           \
              " at offset 0x%llx\n",					\
	      (long long)DUMPI_WRITE_TELL(PROFILE));			\
    }                                                                   \
    put_function_label(PROFILE, LABEL);					\
    put_config_mask(PROFILE, perf, output);				\
    put16(PROFILE, thread);						\
    put_times(PROFILE, cpu, wall, output->timestamps);			\
    put_perfinfo(PROFILE, perf, output);

  /** Shared back-end stuff when ending a profiled call */
#define ENDWRITE(PROFILE)						\
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO) {				\
      fprintf(stderr, "[DUMPI-IO] Completed record  at offset 0x%llx\n", \
  	      (long long)DUMPI_WRITE_TELL(PROFILE));			\
    }									\
  }

  /*
    #define STARTREAD(LABEL)						\
    uint8_t label, config_mask;						\
    assert(val && profile && profile);					\
    label = get_function_label(profile);				\
    assert(label == LABEL);						\
    config_mask = get_config_mask(profile);				\
    get_times(profile, &val->cpu, &val->wall, config_mask);
  */

  /** Shared back-end stuff when starting to read a profiled call */
#define STARTREAD(PROFILE, LABEL)					\
  uint8_t config_mask;                                                  \
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO) {                               \
    /* we need to correct the file offset by 2 bytes, because we */     \
    /* already gobbled up the function label (uint16_t) */              \
    fprintf(stderr, "[DUMPI-IO] Reading record for " #LABEL             \
            " at offset 0x%llx\n",					\
            (long long)DUMPI_READ_TELL(PROFILE)-sizeof(uint16_t));	\
  }                                                                     \
  assert(val && PROFILE && PROFILE->file);				\
  config_mask = get_config_mask(PROFILE);                               \
  if(config_mask & DUMPI_THREADID_MASK)                                 \
    *thread = get16(profile);						\
  get_times(PROFILE, cpu, wall, config_mask);				\
  get_perfinfo(PROFILE, perf, config_mask);

  /** Shared back-end stuff when finishing a read */
#define ENDREAD(PROFILE) do {						\
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO) {                               \
    fprintf(stderr, "[DUMPI-IO] Completed reading record at offset 0x%llx\n", \
            (long long)DUMPI_READ_TELL(PROFILE));			\
  }									\
} while(0)

  /** Utility definition for integer output */
#define PUT_INT(PROFILE, VAR) put32(PROFILE, ((uint32_t)(VAR)))
  /** Utility definition for integer input */
#define GET_INT(PROFILE) get32(profile)

  /** Utility definition for integer output */
#define PUT_INT16(PROFILE, VAR) put16(PROFILE, VAR)
  /** Utility definition for integer input */
#define GET_INT16(PROFILE) get16(PROFILE)

  /** Utility definition for integer output */
#define PUT_INT8(PROFILE, VAR) put8(PROFILE, VAR)
  /** Utility definition for integer input */
#define GET_INT8(PROFILE) get8(PROFILE)

  /** Utility definition for handling integer arrays */
#define PUT_INT_ARRAY_1(PROFILE, TERM, VALUE) do {	\
    int i0, len=0;                                      \
    for(i0 = 0; TERM; ++i0) ++len;                      \
    put32arr(PROFILE, len, VALUE);			\
  } while(0)

  /** Utility definition for handling integer arrays */
#define GET_INT_ARRAY_1(PROFILE, LEN, VALUE) do {	\
    get32arr(PROFILE, &(LEN), &(VALUE));		\
  } while(0)

  /** Utility definition for handling integer arrays */
#define PUT_INT_ARRAY_2(PROFILE, TERM0, TERM1, VALUE) do {	\
    int i0, i1, len0=0, len1=0;                         \
    for(i0 = 0; (TERM0); ++i0) ++len0;                  \
    put32(PROFILE, len0);				\
    /* Allow irregular arrays */                        \
    for(i0 = 0; i0 < len0; ++i0) {                      \
      len1 = 0;                                         \
      if((VALUE[i0]) == NULL) {                         \
        put32(PROFILE, 0);				\
      }                                                 \
      else {                                            \
        for(i1 = 0; TERM1; ++i1) ++len1;                \
        put32arr(PROFILE, len1, (VALUE)[i0]);		\
      }                                                 \
    }                                                   \
  } while(0)

#define PUT_INT_ARRAY_2B(PROFILE, LEN0, LEN1, VALUE) do {	\
    int i0;						\
    put32(PROFILE, LEN0);				\
    for(i0 = 0; i0 < LEN0; ++i0) {                      \
      put32arr(PROFILE, LEN1, (VALUE)[i0]);		\
    }                                                   \
  } while(0)

  /** Utility definition for handling integer arrays */
#define GET_INT_ARRAY_2(PROFILE, LEN0, LEN1, VALUE) do {	\
    int i0;                                             \
    LEN0 = get32(PROFILE);				\
    VALUE = (int**)calloc(LEN0+1, sizeof(int*));	\
    assert((VALUE) != NULL);				\
    for(i0 = 0; i0 < (LEN0); ++i0) {                    \
      get32arr(PROFILE, &(LEN1), (VALUE) + i0);		\
    }                                                   \
  } while(0)

  /** Utility definition for handling character arrays */
#define PUT_CHAR_ARRAY_1(PROFILE, TERM, VALUE) do {	\
    int i0, len=0;                                      \
    char v0 = VALUE[0];                                 \
    for(i0 = 0; TERM; ++i0) { ++len; v0=VALUE[len]; }   \
    putchararr(PROFILE, len, VALUE);			\
  } while(0)

  /** Utility definition for handling character arrays */
#define GET_CHAR_ARRAY_1(PROFILE, LEN, VALUE) do {	\
    getchararr(PROFILE, &(LEN), &(VALUE));		\
  } while(0)

  /** Utility definition for handling character arrays */
#define PUT_CHAR_ARRAY_2(PROFILE, TERM0, TERM1, VALUE) do {	\
    int i0, i1, len0=0, len1=0;                                 \
    char *v0;                                                   \
    char v1 = VALUE[0][0];                                      \
    if((VALUE) != NULL)                                         \
      v0 = VALUE[0];                                            \
    for(i0 = 0; (TERM0); ++i0) { ++len0; v0 = VALUE[len0]; }    \
    put32(PROFILE, len0);					\
    /* Allow irregular arrays */                                \
    for(i0 = 0; i0 < len0; ++i0) {                              \
      v1 = VALUE[i0][0];                                        \
      len1 = 0;                                                 \
      for(i1 = 0; TERM1; ++i1) { ++len1; v1=VALUE[i0][len1]; }  \
      putchararr(PROFILE, len1, (VALUE)[i0]);			\
    }                                                           \
  } while(0)

/*#define GET_CHAR_ARRAY_2(PROFILE, LEN0, LEN1, VALUE) do {	 \
    int i0;                                              \
    LEN0 = get32(PROFILE);                         \
    VALUE = (char**)calloc((LEN0)+1, sizeof(char*));     \
    for(i0 = 0; i0 < (LEN0); ++i0) {                     \
      getchararr(PROFILE, &(LEN1), ((VALUE)+i0));  \
    }                                                    \
  } while(0)
*/

  /** Utility definition for handling character arrays */
#define GET_CHAR_ARRAY_2(PROFILE, LEN0, LEN1, VALUE)	\
  do_get_char_array_2(profile, &LEN0, &LEN1, &VALUE)

  /** A "debuggable" version of how we retrieve an array of std::strings */
static inline
void do_get_char_array_2(dumpi_profile *profile, int *len0, int *len1,
			 char ***value)
{ 
    int i0;                                              
    *len0 = get32(profile);
    *value = (char**)calloc((*len0)+1, sizeof(char*));
    for(i0 = 0; i0 < (*len0); ++i0) {                     
      getchararr(profile, len1, ((*value)+i0));
    }                                                    
}

  /** Utility definition for handling character arrays */
#define PUT_CHAR_ARRAY_3(PROFILE, TERM0, TERM1, TERM2, VALUE) do {	\
    int i0, i1, i2, len0=0, len1=0, len2=0;                             \
    char **v0, *v1, v2;                                                 \
    if((VALUE) == NULL) {                                               \
      put32(PROFILE, 0);						\
    }                                                                   \
    else {                                                              \
      v0 = VALUE[0];                                                    \
      for(i0 = 0; (TERM0); ++i0) { ++len0; v0 = VALUE[len0]; }          \
      put32(PROFILE, len0);						\
      /* Allow irregular arrays */                                      \
      for(i0 = 0; i0 < len0; ++i0) {                                    \
        if(VALUE[i0] == NULL) {                                         \
          put32(PROFILE, 0);						\
        }                                                               \
        else {                                                          \
          v1 = VALUE[i0][0];                                            \
          len1 = 0;                                                     \
          for(i1 = 0; TERM1; ++i1) { ++len1; v1=VALUE[i0][len1]; }      \
          put32(PROFILE, len1);						\
          for(i1 = 0; i1 < len1; ++i1) {                                \
            v2 = VALUE[i0][i1][0];                                      \
            len2 = 0;                                                   \
            for(i2 = 0; TERM2; ++i2) { ++len2; v2=VALUE[i0][i1][len2]; } \
            putchararr(PROFILE, len2, (VALUE)[i0][i1]);			\
          }                                                             \
        }                                                               \
      }                                                                 \
    }                                                                   \
  } while(0)

  /** Utility definition for handling character arrays */
#define GET_CHAR_ARRAY_3(PROFILE, LEN0, LEN1, LEN2, VALUE) do {	  \
    int i0, i1;                                                   \
    LEN0 = get32(PROFILE);					  \
    VALUE = (char***)calloc((LEN0)+1, sizeof(char**));            \
    for(i0 = 0; i0 < (LEN0); ++i0) {                              \
      LEN1 = get32(PROFILE);					  \
      VALUE[i0] = (char**)calloc((LEN1)+1, sizeof(char*));        \
      for(i1 = 0; i1 < (LEN1); ++i1) {                            \
        getchararr(PROFILE, &(LEN2), ((VALUE[i0])+i1));		  \
      }                                                           \
    }                                                             \
  } while(0)

#define PUT_DUMPI_COMBINER(PROFILE, VALUE) PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_COMBINER(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_COMM(PROFILE, VALUE) PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_COMM(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_COMPARISON(PROFILE, VALUE)	\
  PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_COMPARISON(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_DATATYPE(PROFILE, VALUE)	\
  PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_DATATYPE(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_DATATYPE_ARRAY_1(PROFILE, TERM, VALUE) do {	\
    int i0, len0=0;						\
    for(i0 = 0; (TERM); ++i0) ++len0;				\
    put32(PROFILE, len0);					\
    for(i0 = 0; i0 < len0; ++i0)				\
      PUT_DUMPI_DATATYPE(PROFILE, VALUE[i0]);			\
  } while(0)
#define GET_DUMPI_DATATYPE_ARRAY_1(PROFILE, LEN, VALUE) do {		\
    int i0;                                                             \
    LEN = get32(PROFILE);						\
    VALUE = (dumpi_datatype*)calloc((LEN)+1, sizeof(dumpi_datatype));   \
    for(i0 = 0; i0 < LEN; ++i0)                                         \
      VALUE[i0] = GET_DUMPI_DATATYPE(PROFILE);				\
  } while(0)

#define PUT_DUMPI_DEST(PROFILE, VALUE) PUT_INT(PROFILE, VALUE)
#define GET_DUMPI_DEST(PROFILE) GET_INT(PROFILE)

#define PUT_DUMPI_DISTRIBUTION(PROFILE, VALUE)	\
  PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_DISTRIBUTION(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_DISTRIBUTION_ARRAY_1(PROFILE, TERM, VALUE)  do {	\
    int i0, len=0;							\
    for(i0 = 0; TERM; ++i0) ++len;					\
    put32(PROFILE, len);						\
    for(i0 = 0; i0 < len; ++i0)						\
      PUT_DUMPI_DISTRIBUTION(PROFILE, VALUE[i0]);			\
  } while(0)
#define GET_DUMPI_DISTRIBUTION_ARRAY_1(PROFILE, LEN, VALUE)  do {	\
    int i0;                                                             \
    LEN = get32(PROFILE);						\
    VALUE = (dumpi_distribution*)calloc((LEN)+1, sizeof(dumpi_info));   \
    for(i0 = 0; i0 < (LEN); ++i0)                                       \
      VALUE[i0] = GET_DUMPI_DISTRIBUTION(PROFILE);			\
  } while(0)

#define PUT_DUMPI_ERRCODE(PROFILE, VALUE) PUT_INT32(PROFILE, VALUE)
#define GET_DUMPI_ERRCODE(PROFILE) GET_INT32(PROFILE)

#define PUT_DUMPI_ERRCODE_ARRAY_1(PROFILE, TERM, VALUE) \
  PUT_INT_ARRAY_1(PROFILE, TERM, VALUE)
#define GET_DUMPI_ERRCODE_ARRAY_1(PROFILE, TERM, VALUE) \
  GET_INT_ARRAY_1(PROFILE, TERM, VALUE)

#define PUT_DUMPI_ERRHANDLER(PROFILE, VALUE)	\
  PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_ERRHANDLER(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_FILE(PROFILE, VALUE) PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_FILE(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_FILEMODE(PROFILE, VALUE) PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_FILEMODE(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_GROUP(PROFILE, VALUE) PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_GROUP(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_INFO(PROFILE, VALUE) PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_INFO(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_INFO_ARRAY_1(PROFILE, TERM, VALUE)  do {       \
    int i0, len=0;						 \
    for(i0 = 0; TERM; ++i0) ++len;				 \
    put32(PROFILE, len);					 \
    for(i0 = 0; i0 < len; ++i0)					 \
      PUT_DUMPI_INFO(PROFILE, VALUE[i0]);			 \
  } while(0)
#define GET_DUMPI_INFO_ARRAY_1(PROFILE, LEN, VALUE)  do {	\
    int i0;                                                     \
    LEN = get32(PROFILE);					\
    VALUE = (dumpi_info*)calloc((LEN)+1, sizeof(dumpi_info));   \
    for(i0 = 0; i0 < (LEN); ++i0)                               \
      VALUE[i0] = GET_DUMPI_INFO(PROFILE);			\
  } while(0)

#define PUT_DUMPI_KEYVAL(PROFILE, VALUE) PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_KEYVAL(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_COMM_KEYVAL(PROFILE, VALUE)	\
  PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_COMM_KEYVAL(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_TYPE_KEYVAL(PROFILE, VALUE)	\
  PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_TYPE_KEYVAL(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_WIN_KEYVAL(PROFILE, VALUE)	\
  PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_WIN_KEYVAL(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_LOCKTYPE(PROFILE, VALUE)	\
  PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_LOCKTYPE(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_OP(PROFILE, VALUE) PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_OP(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_ORDERING(PROFILE, VALUE) PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_ORDERING(PROFILE) GET_INT8(PROFILE)

  /* Due to a program bug, DUMPI prior to version 0.6.7 
   * stored requests as 8-bit values (inside a 32-bit memory region) */
#define PUT_DUMPI_REQUEST(PROFILE, VALUE) PUT_INT(PROFILE, VALUE)
#define GET_DUMPI_REQUEST(PROFILE) GET_INT(PROFILE)

#define PUT_DUMPI_REQUEST_ARRAY_1(PROFILE, TERM, VALUE) \
  PUT_INT_ARRAY_1(PROFILE, TERM, VALUE)
#define GET_DUMPI_REQUEST_ARRAY_1(PROFILE, TERM, VALUE) \
  GET_INT_ARRAY_1(PROFILE, TERM, VALUE)

#define PUT_DUMPI_SOURCE(PROFILE, VALUE) PUT_INT(PROFILE, VALUE)
#define GET_DUMPI_SOURCE(PROFILE) GET_INT(PROFILE)

#define PUT_DUMPI_STATUS(PROFILE, VALUE) put_statuses(PROFILE, 1, VALUE, output)
#define GET_DUMPI_STATUS(PROFILE) get_statuses(PROFILE, config_mask)

#define PUT_DUMPI_STATUS_ARRAY_1(PROFILE, TERM0, VALUE) do {	\
    int i0, len0=0;						\
    for(i0 = 0; (TERM0); ++i0) ++len0;				\
    put_statuses(PROFILE, len0, VALUE, output);			\
  } while(0)
#define GET_DUMPI_STATUS_ARRAY_1(PROFILE, LEN, VALUE)	\
  VALUE = get_statuses(PROFILE, config_mask)

#define PUT_DUMPI_TAG(PROFILE, VALUE) PUT_INT(PROFILE, VALUE)
#define GET_DUMPI_TAG(PROFILE) (dumpi_tag)GET_INT(PROFILE)

#define PUT_DUMPI_THREADLEVEL(PROFILE, VALUE)	\
  PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_THREADLEVEL(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_TOPOLOGY(PROFILE, VALUE) PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_TOPOLOGY(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_TYPECLASS(PROFILE, VALUE)	\
  PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_TYPECLASS(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_WHENCE(PROFILE, VALUE) PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_WHENCE(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPI_WIN(PROFILE, VALUE) PUT_INT16(PROFILE, ((uint16_t)(VALUE)))
#define GET_DUMPI_WIN(PROFILE) GET_INT16(PROFILE)

#define PUT_DUMPI_WIN_ASSERT(PROFILE, VALUE)	\
  PUT_INT8(PROFILE, ((uint8_t)(VALUE)))
#define GET_DUMPI_WIN_ASSERT(PROFILE) GET_INT8(PROFILE)

#define PUT_DUMPIO_REQUEST(PROFILE, VALUE) PUT_DUMPI_REQUEST(PROFILE, VALUE)
#define GET_DUMPIO_REQUEST(PROFILE) GET_DUMPI_REQUEST(PROFILE)

#define PUT_DUMPIO_REQUEST_ARRAY_1(PROFILE, TERM, VALUE) \
  PUT_DUMPI_REQUEST_ARRAY_1(PROFILE, TERM, VALUE)
#define GET_DUMPIO_REQUEST_ARRAY_1(PROFILE, TERM, VALUE) \
  GET_DUMPI_REQUEST_ARRAY_1(PROFILE, TERM, VALUE)

#define PUT_INT64_T(PROFILE, VALUE) put64(PROFILE, VALUE)
#define GET_INT64_T(PROFILE) get64(PROFILE)

  /*@}*/ /* Close the scope of the doxygen module */

#ifdef __cplusplus
}
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_IODEFS_H */