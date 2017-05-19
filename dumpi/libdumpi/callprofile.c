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

#include <dumpi/libdumpi/callprofile.h>
#include <dumpi/libdumpi/callprofile-addrset.h>
#include <dumpi/libdumpi/mpibindings.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * We keep annotations and data blocks here.
 * The 'callprofile-addreset' utilities
 * are used to ensure that each key only gets stored once.
 */
typedef struct libdumpi_entry {
  uint64_t key;
  const char *data;
  struct libdumpi_entry *next;
} libdumpi_entry;

typedef libdumpi_entry *libdumpi_data_map;
static libdumpi_entry **data_map_ = NULL;

/* Use a simple xor hash to decide which bin an entry should go into. */
static inline uint8_t get_hash(uint64_t value) {
  uint8_t *p = (uint8_t*)&value;
  return (p[0] ^ p[1] ^ p[2] ^ p[3]);
}

void data_map_insert(uint64_t key, const char *data) {
  uint8_t hash;
  char *copy = NULL;
  libdumpi_entry *tail = NULL;
  if(! data_map_)
    assert((data_map_ = (libdumpi_entry**)calloc(256, sizeof(libdumpi_entry*)))
           != NULL);
  hash = get_hash(key);
  if(! data_map_[hash]) {
    assert((data_map_[hash] =
            (libdumpi_entry*)calloc(1, sizeof(libdumpi_entry))) != NULL);
    tail = data_map_[hash];
  }
  else {
    for(tail = data_map_[hash]; tail != NULL; tail = tail->next) {
      if(tail->key == key) {
        if((tail->data != NULL) && (data != NULL)) {
          /* this entry is already in the queue */
          return;
        }
      }
    }
    /* if we get down here, this is a new entry */
    tail = data_map_[hash];
    while(tail->next != NULL)
      tail = tail->next;
    assert((tail->next =
            (libdumpi_entry*)calloc(1, sizeof(libdumpi_entry))) != NULL);
    tail = tail->next;
  }
  if(data)
    assert((copy = strdup(data)) != NULL);
  tail->key = key;
  tail->data = copy;
}

size_t data_map_size() {
  size_t rv = 0;
  if(data_map_) {
    int i;
    libdumpi_entry *ent;
    for(i = 0; i < 256; ++i) {
      for(ent = data_map_[i]; ent != NULL; ent = ent->next) {
        rv++;
      }
    }
  }
  return rv;
}

void data_map_keys(uint64_t *arr) {
  if(data_map_) {
    int i;
    size_t offset = 0;
    libdumpi_entry *ent;
    for(i = 0; i < 256; ++i) {
      for(ent = data_map_[i]; ent != NULL; ent = ent->next)
        arr[offset++] = ent->key;
    }
  }
}

const char* data_map_find(uint64_t key) {
  const char *retval = NULL;
  if(data_map_) {
    libdumpi_entry *ent = NULL;
    uint8_t hash = get_hash(key);
    for(ent = data_map_[hash]; ent != NULL; ent = ent->next) {
      if(ent->key == key) {
        retval = ent->data;
        break;
      }
    }
  }
  return retval;
}

#ifdef DUMPI_ENABLE_INSTRUMENTATION

static int calldepth = 0;

/* Indicate entry into a profiled function. */
void __cyg_profile_func_enter(void *fn, void *site) {
  dumpi_func_call stat;
  uint16_t thread;
  dumpi_time cpu, wall;
  /*
  printf("__cyg_profile_func_enter(%p [\"%s\"], %p)\n",
	 fn, dumpi_function_name(fn), site);
  */
  if(dumpi_global == NULL)
    libdumpi_init();
  STARTTIME(cpu, wall);
  THREAD_ID(thread);
  (void)site;
  ++calldepth;
  libdumpi_insert_address(fn);
  if(dumpi_global->output->function[DUMPI_Function_enter]) {
    stat.fn = (uint64_t)fn;
    STOPTIME(cpu, wall);
    dumpi_write_func_enter(&stat, thread, &cpu, &wall, dumpi_global->perf,
			   dumpi_global->output, dumpi_global->profile);
  }
}

/* Indicate exit from a profiled function. */
void __cyg_profile_func_exit(void *fn, void *site) {
  dumpi_func_call stat;
  uint16_t thread;
  dumpi_time cpu, wall;
  if(dumpi_global == NULL)
    libdumpi_init();
  STARTTIME(cpu, wall);
  THREAD_ID(thread);
  (void)site;
  --calldepth;
  insert_address(fn); /* should not be needed - but better safe than sorry */
  if(dumpi_global->output->function[DUMPI_Function_exit]) {
    stat.fn = (uint64_t)fn;
    STOPTIME(cpu, wall);
    dumpi_write_func_exit(&stat, thread, &cpu, &wall, dumpi_global->perf,
			  dumpi_global->output, dumpi_global->profile);
  }
}

#endif /* ! DUMPI_ENABLE_INSTRUMENTATION */

/**
 * Insert an arbitrary data block.  This block may contain NULL characters,
 * although programs that expect string data (e.g. dumpi2ascii) may not
 * print such data correctly.
 * The input data is copied using memcpy (shallow copy).
 */
void libdumpi_insert_data(uint64_t key, const char *data) {
  data_map_insert(key, data);
}

#ifdef DUMPI_DLADDR_WORKS
#define _GNU_SOURCE 1
#define __USE_GNU 1
#include <dlfcn.h>

static const char* lookup_function_name(uint64_t fn) {
  Dl_info din;
  assert(dladdr((void*)fn, &din) != 0);
  return din.dli_sname;
}

#else /* ! DUMPI_DLADDR_WORKS */

static const char *none = "(none)";

const char* lookup_function_name(uint64_t fn) {
  (void)fn;
  return none;
}

#endif /* DUMPI_DLADDR_WORKS */

const char* dumpi_function_name(uint64_t key) {
  /* First check whether this is a keylist item. */
  const char *retval = data_map_find(key);
  /* if not, try a name lookup */
  if(retval == NULL) retval = lookup_function_name(key);
  return retval;
}

/*
 * Get the number of unique function addresses profiled so far.
 */
int dumpi_profile_unique_address_count(void) {
  dumpi_addr_iterator it;
  int retval = 0;
  for(it = libdumpi_addr_begin_iterator();
      libdumpi_addr_valid_iterator(&it);
      libdumpi_addr_increment_iterator(&it))
    {
      ++retval;
    }
  return data_map_size() + retval;
}
  
/*
 * Get a listing of all unique function addresses profiled so far.
 */
uint64_t* dumpi_profile_unique_addresses(void) {
  int curr;
  uint64_t *keys = NULL;
  dumpi_addr_iterator it;
  int annotations = 0;
  int funcnames = dumpi_profile_unique_address_count();
  annotations = data_map_size();
  keys = (uint64_t*)calloc(funcnames + annotations + 1, sizeof(uint64_t));
  assert(keys != NULL);
  curr = 0;
  for(it = libdumpi_addr_begin_iterator();
      libdumpi_addr_valid_iterator(&it);
      libdumpi_addr_increment_iterator(&it))
    {
      assert(curr < funcnames);
      keys[curr] = (uint64_t)libdumpi_addr_iterator_value(&it);
      ++curr;
    }
  data_map_keys(keys + curr);
  return keys;
}