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

#include <dumpi/dumpiconfig.h>
#include <dumpi/common/types.h>
#include <dumpi/common/funcs.h>
#include <dumpi/dumpiconfig.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <assert.h>

#ifdef DUMPI_ON_REDSTORM
#include "rca_lib.h"
extern uint32_t _my_pnid;
#endif /* ! DUMPI_ON_REDSTORM */

#ifdef DUMPI_ON_BGP
#include <spi/kernel_interface.h>
#include <common/bgp_personality.h>
#include <common/bgp_personality_inlines.h>
#endif /* ! DUMPI_ON_BGP */

#ifdef DUMPI_ON_BGQ
#include <personality.h>
#endif

#define DUMPI_COPYENV(ENVNAME, VARNAME) do {			\
    char *tmp = NULL;						\
    const char *none = "<none>";				\
    const char *scratch;					\
    scratch = getenv(ENVNAME);					\
    if(scratch == NULL) {					\
      scratch = none;						\
    }								\
    tmp = (char*)malloc(strlen(scratch+1));			\
    assert(tmp != NULL);					\
    strcpy(tmp, scratch);					\
    VARNAME = tmp;						\
  } while(0)

dumpi_header* dumpi_build_header(void) {
  dumpi_header *header = (dumpi_header*)calloc(1, sizeof(dumpi_header));
  header->version[0] = dumpi_version;
  header->version[1] = dumpi_subversion;
  header->version[2] = dumpi_subsubversion;
  header->starttime  = (int64_t)time(NULL);
  {
    size_t len;
    char *tmp = NULL;
    const char *none = "<none>";
    const char *scratch = getenv("LOGNAME");
    if(scratch == NULL) {
      scratch = none;
    }
    len = strlen(scratch) + 2;
    tmp = (char*)malloc(len);
    tmp[len-1] = '\0';
    assert(tmp != NULL);
    strncpy(tmp, scratch, len);
    header->username = tmp;
  }
  header->hostname = (char*)malloc(DUMPI_HOST_NAME_MAX+1);
  header->hostname[DUMPI_HOST_NAME_MAX] = '\0';
  gethostname(header->hostname, DUMPI_HOST_NAME_MAX);

#ifdef DUMPI_ON_REDSTORM
  {
    rca_mesh_coord_t mesh_coord;
    if(rca_get_meshcoord( (uint16_t)_my_pnid, &mesh_coord ) < 0 ) {
      fprintf(stderr,"rca_get_meshcoord failed for pnid %d\n",_my_pnid);
      header->meshdim = 0;
      header->meshcrd = NULL;
      header->meshsize = NULL;
    }
    else {
      header->meshdim = 3;
      header->meshcrd = (int*)malloc(header->meshdim * sizeof(int));
      assert(header->meshcrd != NULL);
      header->meshcrd[0] = mesh_coord.mesh_x;
      header->meshcrd[1] = mesh_coord.mesh_y;
      header->meshcrd[2] = mesh_coord.mesh_z;
      /* meshsize isn't defined on redstorm, so leave defn but zeroed */
      header->meshsize = (int*)calloc(header->meshdim, sizeof(int));
    }
  }
#else /* ! DUMPI_ON_REDSTORM */
#ifdef DUMPI_ON_BGP
  {
    int torus_x, torus_y, torus_z;
    int xsize, ysize, zsize, procid;
    _BGP_Personality_t personality;
    Kernel_GetPersonality(&personality, sizeof(personality));
    xsize = personality.Network_Config.Xnodes;
    ysize = personality.Network_Config.Ynodes;
    zsize = personality.Network_Config.Znodes;
    torus_x = personality.Network_Config.Xcoord;
    torus_y = personality.Network_Config.Ycoord;
    torus_z = personality.Network_Config.Zcoord;
    header->meshdim = 3;
    header->meshcrd = (int*)malloc(header->meshdim * sizeof(int));
    assert(header->meshcrd != NULL);
    header->meshcrd[0] = torus_x;
    header->meshcrd[1] = torus_y;
    header->meshcrd[2] = torus_z;
    header->meshsize = (int*)calloc(header->meshdim, sizeof(int));
    assert(header->meshsize != NULL);
    header->meshsize[0] = xsize;
    header->meshsize[1] = ysize;
    header->meshsize[2] = zsize;
  }
#else /* ! DUMPI_ON_BGP */
#ifdef DUMPI_ON_BGQ
  {
    int torus_a, torus_b, torus_c, torus_d, torus_e;
    int asize, bsize, csize, dsize, esize, procid;
    Personality_t personality;
    Kernel_GetPersonality(&personality, sizeof(personality));
    asize = personality.Network_Config.Anodes;
    bsize = personality.Network_Config.Bnodes;
    csize = personality.Network_Config.Cnodes;
    dsize = personality.Network_Config.Dnodes;
    esize = personality.Network_Config.Enodes;
    torus_a = personality.Network_Config.Acoord;
    torus_b = personality.Network_Config.Bcoord;
    torus_c = personality.Network_Config.Ccoord;
    torus_d = personality.Network_Config.Dcoord;
    torus_e = personality.Network_Config.Ecoord;
    header->meshdim = 5;
    header->meshcrd = (int*)malloc(header->meshdim * sizeof(int));
    assert(header->meshcrd != NULL);
    header->meshcrd[0] = torus_a;
    header->meshcrd[1] = torus_b;
    header->meshcrd[2] = torus_c;
    header->meshcrd[3] = torus_d;
    header->meshcrd[4] = torus_e;
    header->meshsize = (int*)calloc(header->meshdim, sizeof(int));
    assert(header->meshsize != NULL);
    header->meshsize[0] = asize;
    header->meshsize[1] = bsize;
    header->meshsize[2] = csize;
    header->meshsize[3] = dsize;
    header->meshsize[4] = esize;
  }
#else
  header->meshdim = 0;
  header->meshcrd = NULL;
  header->meshsize = NULL;
#endif /* DUMPI_ON_BGQ */
#endif /* DUMPI_ON_BGP */
#endif /* DUMPI_ON_REDSTORM */
  return header;
}

void dumpi_free_header(dumpi_header *header) {
  if(header != NULL) {
    free(header->hostname);
    free(header->username);
    free(header->meshcrd);
    free(header->meshsize);
    free(header);
  }
}

dumpi_keyval_record* dumpi_alloc_keyval_record(void) {
  dumpi_keyval_record *retval =
    (dumpi_keyval_record*)calloc(1, sizeof(dumpi_keyval_record));
  assert(retval != NULL);
  return retval;
}

void dumpi_push_keyval_entry(dumpi_keyval_record *record,
			     const char *key, const char *val)
{
  dumpi_keyval_entry *entry;
  assert(record != NULL);
  assert((record->count == 0 && record->head == NULL && record->tail == NULL) ||
	 (record->count > 0  && record->head != NULL && record->tail != NULL));
  assert((entry = (dumpi_keyval_entry*)calloc(1, sizeof(dumpi_keyval_entry)))
	 != NULL);
  if(key) {
    size_t len = strlen(key);
    assert((entry->key = (char*)malloc(len+1)) != NULL);
    strcpy(entry->key, key);
  }
  if(val) {
    size_t len = strlen(val);
    assert((entry->val = (char*)malloc(len+1)) != NULL);
    strcpy(entry->val, val);
  }
  if(record->head) {
    record->tail->next = entry;
    record->tail = entry;
  }
  else {
    record->head = record->tail = entry;
  }
  ++record->count;
}

int dumpi_erase_keyval_entry(dumpi_keyval_record *record,
			     dumpi_keyval_entry *entry)
{
  int found = 0;
  dumpi_keyval_entry *prev, *curr;
  assert(record != NULL);
  assert((record->count == 0 && record->head == NULL && record->tail == NULL) ||
	 (record->count > 0  && record->head != NULL && record->tail != NULL));
  prev = NULL;
  curr = record->head;
  while(curr != NULL) {
    if(curr == entry) {
      found = 1;
      if(prev) {
	prev->next = curr->next;
      }
      if(curr == record->head) {
	record->head = curr->next;
      }
      if(curr == record->tail) {
	record->tail = prev;
      }
      free(curr->key);
      free(curr->val);
      free(curr);
      break;
    }
    prev = curr;
    curr = curr->next;
  }
  return found;
}

void dumpi_clear_keyval_record(dumpi_keyval_record *record) {
  dumpi_keyval_entry *curr;
  assert(record != NULL);
  assert((record->count == 0 && record->head == NULL && record->tail == NULL) ||
	 (record->count > 0  && record->head != NULL && record->tail != NULL));
  curr = record->head;
  while(curr != NULL) {
    dumpi_keyval_entry *next = curr->next;
    free(curr->key);
    free(curr->val);
    free(curr);
    curr = next;
  }
  record->count = 0;
  record->head = record->tail = NULL;
}

void dumpi_free_keyval_record(dumpi_keyval_record *record) {
  if(record)
    dumpi_clear_keyval_record(record);
  free(record);
}

void dumpi_free_footer(dumpi_footer *footer) {
  free(footer);
}
