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

#include <dumpi/common/io.h>
#include <dumpi/common/iodefs.h>
#include <dumpi/common/funcs.h>
#include <dumpi/common/gettime.h>
#include <dumpi/common/debugflags.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

void dumpi_init_profile(dumpi_profile *profile) {
  assert(profile != NULL);
  memset(profile, 0, sizeof(dumpi_profile));
}

/* The magic at the lead-in to the file.
 * Unfortunately, ANSI-C only ensures that literals have storage size 'long',
 * which is generally only 32 bits on 32-bit platforms */ 
#define DUMPI_HEAD_MAGIC ((((uint64_t)(0xffaadd44))<<32) | 0x554d5049)

/* This gets output just before the footer for error checking */
#define DUMPI_FOOT_MAGIC ((uint64_t)(0xf007fee7))


/*
 * Common routines to read and write dumpi datatypes to a file.
 *
 * All these functions will eventually return 1 on success, 0 on error, and set
 * dumpi_errstr to indicate most recent error.
 */

int dumpi_write_header(dumpi_profile *profile, const dumpi_header *header) {
  int i;
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_write_header at offset 0x%llx\n",
	    ((long long)DUMPI_WRITE_TELL(profile)));
  put_function_label(profile, DUMPI_END_OF_STREAM);
  profile->header = DUMPI_WRITE_TELL(profile);
  for(i = 0; i < 3; ++i)
    put8(profile, header->version[i]);
  put64(profile, header->starttime);
  put_string(profile, header->hostname);
  put_string(profile, header->username);
  put32arr(profile, header->meshdim, header->meshcrd);
  put32arr(profile, header->meshdim, header->meshsize);
  return 1;
}

int dumpi_read_header(dumpi_profile *profile, dumpi_header *header) {
  int i;
  assert(header != NULL);
  assert(profile && profile->file);
  if(profile->header > 0) {
    DUMPI_FPOS callpos = DUMPI_READ_TELL(profile);
    assert(DUMPI_SEEK(profile, profile->header, SEEK_SET) == 0);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
      fprintf(stderr, "[DUMPI-IO] dumpi_read_header at offset 0x%llx\n",
	      ((long long)DUMPI_READ_TELL(profile)));
    for(i = 0; i < 3; ++i)
      header->version[i] = get8(profile);
    header->starttime = get64(profile);
    header->hostname  = get_string(profile);
    header->username  = get_string(profile);
    get32arr(profile, &header->meshdim, &header->meshcrd);
    if(header->meshdim && (header->version[0] > 0 || header->version[1] > 4)) {
      /* meshdim added in version 0.5 */
      get32arr(profile, &header->meshdim, &header->meshsize);
    }
    assert(DUMPI_SEEK(profile, callpos, SEEK_SET) == 0);
  }
  else {
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
      fprintf(stderr, "[DUMPI-IO]: dumpi_read_header without profile header\n");
    header->version[0] = header->version[1] = header->version[2] = 0;
    header->starttime = 0;
    header->hostname = strdup("");
    header->username = strdup("");
    header->meshdim = 0;
    header->meshcrd = NULL;
    header->meshsize = NULL;
  }
  return 1;
}

int dumpi_write_footer(dumpi_profile *profile, const dumpi_footer *footer) {
  int it;
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr,
	    "[DUMPI-IO] dumpi_write_footer(footer=%p, profile=%p{file=%p}) "
	    "at offset 0x%llx\n", footer, profile, 
	    (profile ? profile->file : NULL),
	    ((long long)DUMPI_WRITE_TELL(profile)));
  assert(footer != NULL);
  assert(profile);
  profile->footer = DUMPI_WRITE_TELL(profile);
  put64(profile, DUMPI_FOOT_MAGIC);
  /* Output call counts. */
  for(it = 0; it <= DUMPI_ALL_FUNCTIONS; ++it) {
    put32(profile, footer->call_count[it]);
  }
  /* Output ignored counts. */
  for(it = 0; it <= DUMPI_ALL_FUNCTIONS; ++it)
    put32(profile, footer->ignored_count[it]);
  /* Output overhead costs. */
  /*
  for(it = 0; it <= DUMPI_ALL_FUNCTIONS; ++it) {
    PUT_INT16(profile, overhead_accum[it].wall.sec);
    PUT_INT32(profile, overhead_accum[it].wall.nsec);
    PUT_INT16(profile, overhead_accum[it].cpu.sec);
    PUT_INT32(profile, overhead_accum[it].cpu.nsec);
  }
  */
  return 1;
}

int dumpi_read_footer(dumpi_profile *profile, dumpi_footer *footer) {
  int it;
  /* int8_t label; */
  int64_t foot;
  long callpos;
  assert(profile && profile->file);
  if(profile->footer > 0) {
    callpos = DUMPI_READ_TELL(profile);
    assert(DUMPI_SEEK(profile, profile->footer, SEEK_SET) == 0);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
      fprintf(stderr, "[DUMPI-IO] dumpi_read_footer at offset 0x%llx\n",
	      ((long long)DUMPI_READ_TELL(profile)));
    /*
      label = get_function_label(profile);
      assert(label == DUMPI_ALL_FUNCTIONS);
    */
    foot = get64(profile);
    if(foot != DUMPI_FOOT_MAGIC) {
      fprintf(stderr, "dumpi_read_footer:  Bad magic\n");
      abort();
    }
    for(it = 0; it <= DUMPI_ALL_FUNCTIONS; ++it)
      footer->call_count[it] = get32(profile);
    for(it = 0; it <= DUMPI_ALL_FUNCTIONS; ++it)
      footer->ignored_count[it] = get32(profile);
    /* Read overhead costs if they are in this footer. */
    /* THIS IS WHERE I AM AT */
    /* if((DUMPI_READ_TELL(profile) + 12*DUMPI_ALL_FUNCTIONS) <= */
    DUMPI_SEEK(profile, callpos, SEEK_SET);
  }
  else {
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
      fprintf(stderr, "[DUMPI-IO] dumpi_read_footer without profile footer\n");
    memset(footer, 0, sizeof(dumpi_footer));
  }
  return 1;
}

int dumpi_write_keyval_record(dumpi_profile *profile,
			      const dumpi_keyval_record *keyval)
{
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_write_keyval_record at offset 0x%llx\n",
	    ((long long)DUMPI_WRITE_TELL(profile)));
  assert(profile);
  if(keyval) {
    dumpi_keyval_entry *curr = keyval->head;
    put32(profile, keyval->count);
    while(curr) {
      put_string(profile, curr->key);
      put_string(profile, curr->val);
    }
  }
  else {
    put32(profile, 0);
  }
  return 1;
}

int dumpi_read_keyval_record(dumpi_profile *profile,
			     dumpi_keyval_record *keyval)
{
  int i, count;
  char *key, *val;
  long callpos;
  assert(keyval && profile && profile->file);
  if(profile->keyval > 0) {
    callpos = DUMPI_READ_TELL(profile);
    assert(DUMPI_SEEK(profile, profile->keyval, SEEK_SET) == 0);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
      fprintf(stderr, "[DUMPI-IO] dumpi_read_keyval_record at offset 0x%llx\n",
	      ((long long)DUMPI_READ_TELL(profile)));
    count = get32(profile);
    for(i = 0; i < count; ++i) {
      key = get_string(profile);
      val = get_string(profile);
      dumpi_push_keyval_entry(keyval, key, val);
      free(key);
      free(val);
      DUMPI_SEEK(profile, callpos, SEEK_SET);
    }
  }
  else {
    /*
    fprintf(stderr, "[DUMPI-IO]:  dumpi_read_keyval_record without a "
	    "keyval record in profile\n");
    */
    dumpi_clear_keyval_record(keyval);
  }
  return 1;
}

int dumpi_write_perfctr_labels(dumpi_profile *profile, int counters,
			       dumpi_perflabel_t *labels)
{
  int i;
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_write_perfctr_labels at offset 0x%llx\n",
	    ((long long)DUMPI_WRITE_TELL(profile)));
  assert(profile);
  assert((counters && labels) || (counters == 0));
  profile->perflbl = DUMPI_WRITE_TELL(profile);
  put32(profile, counters);
  for(i = 0; i < counters; ++i)
    put_string(profile, labels[i]);
  return 1;
}

void dumpi_read_perfctr_labels(dumpi_profile *profile, int *counters,
			       dumpi_perflabel_t *labels, int maxlabels)
{
  int i;
  long callpos;
  char scratch[DUMPI_MAX_PERFCTR_NAME];
  if(profile->perflbl > 0) {
    assert(profile && profile->file);
    assert(counters);
    assert((maxlabels && labels) || (maxlabels == 0));
    callpos = DUMPI_READ_TELL(profile);
    assert(DUMPI_SEEK(profile, profile->perflbl, SEEK_SET) == 0);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
      fprintf(stderr, "[DUMPI-IO] dumpi_read_perfctr_labels at offset 0x%llx\n",
	      ((long long)DUMPI_READ_TELL(profile)));
    *counters = get32(profile);
    for(i = 0; i < *counters; ++i) {
      if(i < maxlabels)
	get_fix_string(profile, labels[i], DUMPI_MAX_PERFCTR_NAME);
      else
	get_fix_string(profile, scratch, DUMPI_MAX_PERFCTR_NAME);
    }
    DUMPI_SEEK(profile, callpos, SEEK_SET);
  }
  else {
    fprintf(stderr, "[DUMPI-IO]:  dumpi_read_perfctr_labels without "
	    "perfctr labels in profile\n");
    *counters = 0;
  }
}

int dumpi_write_function_addresses(dumpi_profile *profile, int count,
				   const uint64_t *addresses,
				   char **names)
{
  int i;
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, 
	    "[DUMPI-IO] dumpi_write_function_addresses at offset 0x%llx\n",
	    ((long long)DUMPI_WRITE_TELL(profile)));
  assert(profile);
  assert((count && addresses && names) || (count == 0));
  profile->addrlbl = DUMPI_WRITE_TELL(profile);
  put32(profile, count);
  for(i = 0; i < count; ++i) {
    put64(profile, addresses[i]);
    put_string(profile, names[i]);
  }
  return 1;
}

int dumpi_read_function_addresses(dumpi_profile *profile, int *count,
				  uint64_t **addresses,
				  char ***names)
{
  int i;
  long callpos;
  if(profile->addrlbl > 0) {
    assert(profile && profile->file);
    callpos = DUMPI_READ_TELL(profile);
    assert(DUMPI_SEEK(profile, profile->addrlbl, SEEK_SET) == 0);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
      fprintf(stderr, "[DUMPI-IO] dumpi_read_function_addresses "
	      "at offset 0x%llx\n", ((long long)DUMPI_READ_TELL(profile)));
    assert(count != NULL);
    *count = get32(profile);
    *addresses = (uint64_t*)calloc(*count+1, sizeof(uint64_t));
    assert(addresses != NULL);
    *names = (char**)calloc(*count+1, sizeof(char*));
    assert(names != NULL);
    for(i = 0; i < *count; ++i) {
      (*addresses)[i] = get64(profile);
      (*names)[i] = get_string(profile);
    }
  }
  else {
    /*
    fprintf(stderr, "[DUMPI-IO] dumpi_read_function_addresses "
	    "without function addresses in profile\n");
    */
    *count = 0;
    *addresses = NULL;
    *names = NULL;
  }
  return 1;
}

int dumpi_write_datatype_sizes(dumpi_profile *profile,
			       const dumpi_sizeof *sizes)
{
  int i;
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_write_datatype_sizes at offset 0x%llx\n",
	    ((long long)DUMPI_WRITE_TELL(profile)));
  assert(sizes && profile);
  assert((sizes->count == 0) || (sizes->count > 0 && sizes->size != NULL));
  profile->sizelbl = DUMPI_WRITE_TELL(profile);
  put32(profile, sizes->count);
  for(i = 0; i < sizes->count; ++i) {
    put32(profile, sizes->size[i]);
  }
  return 1;  
}

int dumpi_read_datatype_sizes(dumpi_profile *profile, dumpi_sizeof *sizes) {
  int i;
  long callpos;
  assert(sizes != NULL);
  if(profile->sizelbl > 0) {
    assert(profile && profile->file);
    callpos = DUMPI_READ_TELL(profile);
    assert(DUMPI_SEEK(profile, profile->sizelbl, SEEK_SET) == 0);
    if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
      fprintf(stderr, "[DUMPI-IO] dumpi_datatype_sizes at offset 0x%llx\n",
	      ((long long)DUMPI_READ_TELL(profile)));
    sizes->count = get32(profile);
    assert(sizes->count >= 0);
    sizes->size = (int*)calloc(sizes->count, sizeof(int));
    assert(sizes->size != NULL);
    for(i = 0; i < sizes->count; ++i) {
      sizes->size[i] = get32(profile);
    }
  }
  else {
    /*
    fprintf(stderr, "[DUMPI-IO] dumpi_read_function_addresses "
	    "without function addresses in profile\n");
    */
    sizes->count = 0;
    sizes->size = (int*)malloc(0);
  }
  return 1;  
}

int dumpi_write_index(dumpi_profile *profile) {
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_write_index at offset 0x%llx\n",
	    ((long long)DUMPI_WRITE_TELL(profile)));
  if(profile && profile->file) {
    put64(profile, DUMPI_HEAD_MAGIC);
    put64(profile, profile->sizelbl); /* added in v. 0.6.6 */
    put64(profile, profile->addrlbl);
    put64(profile, profile->perflbl);
    put64(profile, profile->header);
    put64(profile, profile->body);
    put64(profile, profile->footer);
    put64(profile, profile->keyval);    
    DUMPI_FLUSH(profile);
    if(profile->file != stdout && profile->file != stderr)
      DUMPI_FCLOSE(profile->file);
    dumpi_free_membuf(profile->membuf);
    profile->file = NULL;
    profile->membuf = NULL;
  }
  return 1;
}

dumpi_profile *dumpi_open_input_file(const char *fname) {
  /* The file must start with magic. */
  dumpi_profile *retval;
  uint64_t magic;
  DUMPIFILE fp = DUMPI_FOPEN(fname, "r");
  retval = (dumpi_profile*)calloc(1, sizeof(dumpi_profile));
  assert(retval != NULL);
  retval->addrlbl = retval->perflbl = 0;
  retval->file = fp;
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_open_input_file\n");
  if(fp == NULL) {
    fprintf(stderr, "dumpi_open_input_file:  Failed to open \"%s\" for reading:"
	    "  errno=%d (%s)\n", fname, errno, strerror(errno));
    return NULL;
  }
  magic = get64(retval);
  if(magic != DUMPI_HEAD_MAGIC) {
    fprintf(stderr, "dumpi_open_input_file:  File \"%s\" does not start with "
	    "the correct magic incantation.  Not a valid DUMPI file.\n", fname);
    DUMPI_FCLOSE(fp);
    errno = EIO;
    return NULL;
  }
  if(DUMPI_SEEK(retval, -8*((long)sizeof(int64_t)), SEEK_END) != 0) {
    fprintf(stderr, "dumpi_open_input_file:  Cannot seek to index record in "
	    "\"%s\".  File might be truncated.\n", fname);
    return NULL;
  }
  magic = get64(retval);
  /* We might be dealing with an older tracefile -- 
   * sizelbl was pre-pended in version 0.6.6 (May 2010).
   */
  if(magic != DUMPI_HEAD_MAGIC) {
    /* We don't have a datatype size record. */
    magic = get64(retval);
  }
  else {
    retval->sizelbl = get64(retval);
  }
  /*
   * -- perflbl was pre-pended to the index records on Oct 21. 2009.
   * -- addrlbl was pre-pended to the index records on Dec. 8. 2009.
   * Both of those were added before consistent versioning was implemented.
   */
  if(magic != DUMPI_HEAD_MAGIC) {
    /* We don't have an address label record */
    magic = get64(retval);
  }
  else {
    retval->addrlbl = get64(retval);
  }
  if(magic != DUMPI_HEAD_MAGIC) {
    /* We don't have a perfctr record either */
    magic = get64(retval);
  }
  else {
    retval->perflbl = get64(retval);
  }
  /* At this point, we better have found a valid magic record entry */
  if(magic != DUMPI_HEAD_MAGIC) {
    /* Actually, we don't seem to have a valid index record at all */
    fprintf(stderr, "dumpi_open_input_file:  Index record in \"%s\" does not "
	    "start with correct magic incantation.  File may be truncated.\n",
	    fname);
    DUMPI_FCLOSE(fp);
    errno = EIO;
    free(retval);
    return NULL;
  }
  /* These are the four "original" index records. */
  retval->header  = get64(retval);
  retval->body    = get64(retval);
  retval->footer  = get64(retval);
  retval->keyval  = get64(retval);
  /* Finally, read in the version number */
  {
    int i;
    int version_cmp[3];
    dumpi_header header;
    assert(dumpi_read_header(retval, &header) != 0);
    for(i = 0; i < 3; ++i)
      retval->version[i] = header.version[i];
    /* Sanity check -- added in v.0.6.4 */
    version_cmp[0] = (dumpi_version > header.version[0] ? 1 :
		      (dumpi_version < header.version[0]  ? -1 : 0));
    version_cmp[1] = (dumpi_subversion > header.version[1] ? 1 :
		      (dumpi_subversion < header.version[1]  ? -1 : 0));
    version_cmp[2] = (dumpi_subsubversion > header.version[2] ? 1 :
		      (dumpi_subsubversion < header.version[2]  ? -1 : 0));
    if(version_cmp[0] < 0 ||
       (version_cmp[0] == 0 && version_cmp[1] < 0) ||
       (version_cmp[0] == 0 && version_cmp[1] == 0 && version_cmp[2] < 0))
      fprintf(stderr, "Warning:  This is dumpi version %d.%d.%d, but the "
	      "trace file is from dumpi version %d.%d.%d.  Things may not "
	      "work correctly (forward-compatibility is not guaranteed)\n",
	      (int)dumpi_version, (int)dumpi_subversion, 
	      (int)dumpi_subsubversion, (int)header.version[0],
	      (int)header.version[1], (int)header.version[2]);
  }
  return retval;
}

int dumpi_start_stream_write(dumpi_profile *profile) {
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_start_stream_write at offset 0x%llx\n",
	    ((long long)DUMPI_WRITE_TELL(profile)));
  assert(profile);
  profile->body = DUMPI_WRITE_TELL(profile);
  put32(profile, profile->cpu_time_offset);
  put32(profile, profile->wall_time_offset);
  return 1;
}

int dumpi_start_stream_read(dumpi_profile *profile) {
  assert(profile && profile->file);
  assert(DUMPI_SEEK(profile, profile->body, SEEK_SET) == 0);
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_start_stream_read at offset 0x%llx\n",
	    ((long long)DUMPI_READ_TELL(profile)));
  profile->cpu_time_offset  = get32(profile);
  profile->wall_time_offset = get32(profile);
  /*
    printf("Read time offsets %d (CPU) and %d (wall)\n",
	 profile->cpu_time_offset, profile->wall_time_offset);
  */
  return 1;
}

/**
 * Create output profile with null file pointer
 */
dumpi_profile *dumpi_alloc_output_profile(int cpu_offset, int wall_offset,
                                          int target_buffer_size)
{
  dumpi_profile *retval;
  retval = (dumpi_profile*)calloc(1, sizeof(dumpi_profile));
  if(retval == NULL) {
    fprintf(stderr, "dumpi_alloc_output_profile:  Failed to allocate dumpi_profile:"
	    "  errno=%d (%s)\n", errno, strerror(errno));
    return NULL;
  }
  retval->target_membuf_size = target_buffer_size;
  put64(retval, DUMPI_HEAD_MAGIC);
  retval->version[0] = dumpi_version;
  retval->version[1] = dumpi_subversion;
  retval->version[2] = dumpi_subsubversion;
  retval->cpu_time_offset = cpu_offset;
  retval->wall_time_offset = wall_offset;
  dumpi_start_stream_write(retval);
  return retval;
}

/*
 * Free dynamic memory for an output profile.
 */
void dumpi_free_output_profile(dumpi_profile *profile) {
  if(profile->membuf)
    dumpi_free_membuf(profile->membuf);
  free(profile);
}

/*
 * Open a file for output.
 */
DUMPIFILE dumpi_open_output_file(const char *fname) {
  DUMPIFILE fp;
  if(dumpi_debug & DUMPI_DEBUG_TRACEIO)
    fprintf(stderr, "[DUMPI-IO] dumpi_open_output_file(\"%s\")\n", fname);
  fp = fopen(fname, "w");
  if(fp == NULL) {
    fprintf(stderr, "Failed to open trace file \"%s\" for input: %s\n", 
	    fname, strerror(errno));
    assert(fp != NULL);
  }
  return fp;
}

/**
 * Initialize a profile given a file pointer.
 */
/* dumpi_profile *dumpi_open_input_file(const char *fname) { */
/*   dumpi_profile *retval; */
/*   DUMPIFILE fp = DUMPI_FOPEN(fname, "w"); */
/*   /\*fprintf(stderr, "dumpi_open_output_file(%s)\n", fname);*\/ */
/*   if(fp == NULL) { */
/*     fprintf(stderr, "dumpi_open_input_file:  Failed to open \"%s\" for reading:" */
/* 	    "  errno=%d (%s)\n", fname, errno, strerror(errno)); */
/*     return NULL; */
/*   } */
/*   retval = (dumpi_profile*)calloc(1, sizeof(dumpi_profile)); */
/*   if(retval == NULL) { */
/*     fprintf(stderr, "dumpi_open_input_file:  Failed to allocate dumpi_profile:" */
/* 	    "  errno=%d (%s)\n", errno, strerror(errno)); */
/*     return NULL; */
/*   } */
/*   retval->file = fp; */
/*   put64(retval, DUMPI_HEAD_MAGIC); */
/*   return retval; */
/* } */