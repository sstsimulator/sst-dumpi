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

#include <dumpi/common/iodefs.h>
#include <stdlib.h>
#include <assert.h>

  /* Grab a buffer to write into -- 128MB-8B should be enough for anybody :) */
#ifndef DUMPI_MEMBUF_SIZE
#define DUMPI_MEMBUF_SIZE 134217720
#endif /* ! DUMPI_MEMBUF_SIZE */

#ifndef DUMPI_MIN_MEMBUF_SIZE
#define DUMPI_MIN_MEMBUF_SIZE 4096
#endif /* ! DUMPI_MIN_MEMBUF_SIZE */

typedef struct dumpi_memory_buffer {
  size_t         length;
  size_t         pos;
  unsigned char *buffer;
} dumpi_memory_buffer;

/* static dumpi_memory_buffer *membuf = NULL; */

/*
 * Free a memory buffer.
 */
void dumpi_free_membuf(dumpi_memory_buffer *buf) {
  if(buf) {
    free(buf->buffer);
    free(buf);
  }
}

static size_t dumpi_membuf_pos(const dumpi_profile *profile) {
  if(profile->membuf)
    return profile->membuf->pos;
  else
    return 0;
}

void dumpi_membuf_flush(dumpi_profile *profile) {
  size_t written;
  assert(profile && profile->file);
  if(profile->membuf != NULL && profile->membuf->pos > 0) {
    written = fwrite(profile->membuf->buffer, 1,
		     profile->membuf->pos, profile->file);
    assert(written == profile->membuf->pos);
    profile->membuf->pos = 0;
  }
  fflush(profile->file);
}

void dumpi_membuf_write(dumpi_profile *profile,
			const void *ptr, size_t size, size_t nmemb)
{
  size_t bytes = size*nmemb;
  assert(profile != NULL);
  /*
  printf("dumpi_membuf_write(%p, %ld, %ld, %p) at buffer offset %ld\n",
	 ptr, (long)size, (long)nmemb, file, (membuf ? membuf->pos : 0));
  */
  if(bytes > 0 && ptr == NULL) {
    fprintf(stderr, "dumpi_membuf_write: Refusing to write %lld bytes starting "
	    " at a NULL pointer.\n", (long long)bytes);
    abort();
  }
  if(profile->membuf == NULL) {
    char *envsetting = NULL;
    profile->membuf = (dumpi_memory_buffer*)malloc(sizeof(dumpi_memory_buffer));
    assert(profile->membuf != NULL);
    if(profile->target_membuf_size >= DUMPI_MIN_MEMBUF_SIZE)
      profile->membuf->length = profile->target_membuf_size;
    else {
      profile->membuf->length = DUMPI_MEMBUF_SIZE;
      envsetting = getenv("DUMPI_MEMBUF_SIZE");
      if(envsetting != NULL) {
	profile->membuf->length = atoi(envsetting);
      }
      if(profile->membuf->length < DUMPI_MIN_MEMBUF_SIZE)
	profile->membuf->length = DUMPI_MIN_MEMBUF_SIZE;
    }
    profile->membuf->buffer = (unsigned char*)malloc(profile->membuf->length);
    if(!profile->membuf->buffer) {
      fprintf(stderr, "DUMPI:  Memory allocation failed for memory buffer\n");
      assert(profile->membuf->buffer != NULL);
    }
    profile->membuf->pos = 0;
  }
  if((profile->membuf->pos+bytes) >= profile->membuf->length) {
    if(profile->file != NULL) {
      DUMPI_FLUSH(profile);
    }
    else {
      /* We don't have a file -- next best thing is to grow the buffer */
      profile->membuf->length = 2*profile->membuf->length + bytes;
      profile->membuf->buffer =
	(unsigned char*)realloc(profile->membuf->buffer,
				profile->membuf->length);
      assert(profile->membuf->buffer != NULL);
    }
  }
  if(bytes > 0)
    memcpy(profile->membuf->buffer+profile->membuf->pos, (ptr), bytes);
  profile->membuf->pos += bytes;
}

void dumpi_membuf_read(dumpi_profile *profile, void *ptr,
		       size_t size, size_t nmemb)
{
  size_t entries;
  assert(profile && profile->file);
  entries = fread(ptr, size, nmemb, profile->file);
  /*
  printf("dumpi_membuf_read(%p, %ld, %ld, %p) at file offset %ld\n",
	 ptr, (long)size, (long)nmemb, file, ftello(file));
  */
  if(entries != (size_t)(nmemb)) {
    fprintf(stderr, "DUMPI_FREAD:  Expected %d values but got %d\n",
	    (int)(nmemb), (int)entries);
    assert(0);
  }
}

off_t dumpi_membuf_tell(dumpi_profile *profile) {
  if(profile->file != NULL) {
    return (ftello(profile->file) + dumpi_membuf_pos(profile));
  }
  else {
    return dumpi_membuf_pos(profile);
  }
}
