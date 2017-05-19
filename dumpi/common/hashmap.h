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

#ifndef DUMPI_COMMON_HASHMAP_H
#define DUMPI_COMMON_HASHMAP_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common
   * \defgroup common_internal  common/internal:  Internal stuff for common \
   * DUMPI functionality.  The functions and types in this module should not \
   * be considered to have a stable interface.
   */

  /*@{*/ /* plop this into common/internal */

  /* Unfortunately, the preprocessor on RedStorm is seriously borked, */
  /* so it screws up on token concatenations.                         */
  /* This is a work-around (not a deliberate obfuscation).            */
#define DUMPI_J2(A, B) A ## B
#define DUMPI_J3(A, B, C) A ## B ## C
#define DUMPI_J4(A, B, C, D) A ## B ## C ## D

  /**
   * A simple O(1) hashmap for dumpi type handles.
   * Currently for internal dumpi-consumption only.
   *
   * Calling DECLARE_HASHMAP(LABEL, KEY_TYPE, VALUE_TYPE)
   * creates the following key functionality:
   *     dumpi_hm_{LABEL}  is a hashmap container
   *     void dumpi_hm_{LABEL}_init(dumpi_hm_{LABEL}**, VALUE_TYPE first_value)
   *             initializes the map
   *     void dumpi_hm_{LABEL}_free(dumpi_hm_{LABEL}**)
   *             frees dumpi_hm resources
   *     VALUE_TYPE dumpi_hm_{LABEL}_get(dumpi_hm_{LABEL}*, KEY_TYPE}
   *             gets a value for the given key, inserting it if needed.
   *     void dumpi_hm_{LABEL}_erase(dumpi_hm_{LABEL}*, KEY_TYPE)
   *             erases the given key from the dumpi_hm.
   *     int dumpi_hm_{LABEL}_test(dumpi_hm_{LABEL}*, KEY_TYPE)
   *             tests whether the given key is set.
   */
#define DUMPI_DECLARE_HASHMAP(LABEL, KEY_TYPE, VALUE_TYPE)              \
                                                                        \
  /** Create an 8-bit key for a value of arbitrary size */		\
  static inline uint8_t DUMPI_J4(dumpi_hash_, LABEL, _, KEY_TYPE)(KEY_TYPE value) { \
    size_t i;                                                           \
    const char *bytes;                                                  \
    uint8_t hash = 0;                                                   \
    bytes = (char*)(&value);                                            \
    for(i = 0; i < sizeof(KEY_TYPE); ++i) {                             \
      hash ^= (uint8_t)bytes[i];					\
    }                                                                   \
    return hash;                                                        \
  }                                                                     \
                                                                        \
  /** A keyval pair mapping a key to a value. */			\
  typedef struct DUMPI_J2(keyval_, LABEL) {                             \
    KEY_TYPE    key;                                                    \
    VALUE_TYPE  value;                                                  \
    struct DUMPI_J2(keyval_,LABEL) *next;                               \
  } DUMPI_J2(keyval_, LABEL);                                           \
                                                                        \
  /** the hashmap has a buffer mapping keys to values (8-bit hash) */	\
  typedef struct DUMPI_J2(dumpi_hm_, LABEL) {                           \
    VALUE_TYPE next_value;                                              \
    DUMPI_J2(keyval_, LABEL) *bucket[256];                              \
  } DUMPI_J2(dumpi_hm_, LABEL);                                         \
                                                                        \
  /** Clear a dumpi_hm in preparation for use. */			\
  static inline void DUMPI_J3(dumpi_hm_,LABEL,_init)			\
    (DUMPI_J2(dumpi_hm_,LABEL) **hm, VALUE_TYPE first_value)            \
  {                                                                     \
    assert(hm != NULL);                                                 \
    *hm = (DUMPI_J2(dumpi_hm_,LABEL)*)                                  \
      calloc(1, sizeof(DUMPI_J2(dumpi_hm_,LABEL)));                     \
    assert(*hm != NULL);                                                \
    (*hm)->next_value = first_value;                                    \
    memset((*hm)->bucket, 0, sizeof(DUMPI_J2(keyval_,LABEL)*)*256);     \
  }                                                                     \
                                                                        \
  /** Done with a dumpi_hm -- clean up. */				\
  static inline void DUMPI_J3(dumpi_hm_,LABEL,_free)			\
    (DUMPI_J2(dumpi_hm_,LABEL) **hm)                                    \
  {                                                                     \
    int i;                                                              \
    DUMPI_J2(keyval_,LABEL) *node, *elem;                               \
    for(i = 0; i < 256; ++i) {                                          \
      node = (*hm)->bucket[i];                                          \
      while(node) {                                                     \
        elem = node;                                                    \
        node = elem->next;                                              \
        free(elem);                                                     \
      }                                                                 \
    }                                                                   \
    free(*hm);                                                          \
    *hm = NULL;                                                         \
  }                                                                     \
                                                                        \
  /** Get (possibly insert) a key-value pair into the dumpi_hm. */	\
  static inline VALUE_TYPE DUMPI_J3(dumpi_hm_,LABEL,_get)		\
    (DUMPI_J2(dumpi_hm_,LABEL) *hm, KEY_TYPE key)                       \
  {                                                                     \
    int buck;                                                           \
    assert(hm != NULL);                                                 \
    /*fprintf(stderr, "hashmap:  Getting value for %ld\n", (long)key);*/\
    buck = DUMPI_J4(dumpi_hash_,LABEL, _, KEY_TYPE)(key);               \
    DUMPI_J2(keyval_,LABEL) **node;					\
    node = &(hm->bucket[buck]);                                         \
    while(*node) {                                                      \
      if((*node)->key == key) {                                         \
        /*fprintf(stderr, "found with a value %d\n", (int)(*node)->value);*/\
        return (*node)->value;                                          \
      }                                                                 \
      node = &((*node)->next);                                          \
    }                                                                   \
    /* We get here if this is a new key */                              \
    *node = (DUMPI_J2(keyval_,LABEL)*)                                  \
      calloc(1, sizeof(DUMPI_J2(keyval_,LABEL)));                       \
    (*node)->key = key;                                                 \
    (*node)->value = hm->next_value++;                                  \
    /*fprintf(stderr, "new, assigned value %d\n", (int)(*node)->value);*/ \
    return (*node)->value;                                              \
  }                                                                     \
                                                                        \
  /** Set (insert) a key-value pair into the dumpi_hm. */		\
  static inline VALUE_TYPE DUMPI_J3(dumpi_hm_,LABEL,_set)		\
    (DUMPI_J2(dumpi_hm_,LABEL) *hm, KEY_TYPE key, VALUE_TYPE val)       \
  {                                                                     \
    DUMPI_J2(keyval_,LABEL) **node;                                     \
    /*fprintf(stderr, "hashmap:  Mapping %ld to %d\n", (long)key, (int)val);*/ \
    node = &hm->bucket[DUMPI_J4(dumpi_hash_,LABEL,_,KEY_TYPE)(key)];    \
    while(*node) {                                                      \
      if((*node)->key == key) {                                         \
        (*node)->value = val;                                           \
        if(val >= hm->next_value) hm->next_value = val+1;               \
        /*fprintf(stderr, "redefinition of %ld to value %d\n",*/        \
        /*        (long)(*node)->key, (int)(*node)->value);   */        \
        return (*node)->value;                                          \
      }                                                                 \
      node = &((*node)->next);                                          \
    }                                                                   \
    /* We get here if this is a new key */                              \
    *node = (DUMPI_J2(keyval_,LABEL)*)                                  \
      calloc(1, sizeof(DUMPI_J2(keyval_,LABEL)));                       \
    (*node)->key = key;                                                 \
    (*node)->value = val;                                               \
    if(val >= hm->next_value) hm->next_value = val+1;                   \
    /*fprintf(stderr, "new definition of %ld to value %d\n", */         \
    /*        (long)(*node)->key, (int)(*node)->value);      */         \
    return (*node)->value;                                              \
  }                                                                     \
                                                                        \
  /** Test whether a value is defined. */				\
  static inline int DUMPI_J3(dumpi_hm_,LABEL,_test)			\
    (DUMPI_J2(dumpi_hm_,LABEL) *hm, KEY_TYPE key)                       \
  {                                                                     \
    DUMPI_J2(keyval_,LABEL) **node;                                     \
    node = &hm->bucket[DUMPI_J4(dumpi_hash_,LABEL,_,KEY_TYPE)(key)];    \
    while(*node) {                                                      \
      if((*node)->key == key) {                                         \
        return 1;                                                       \
      }                                                                 \
      node = &((*node)->next);                                          \
    }                                                                   \
    /* We get here if this is a new key */                              \
    return 0;                                                           \
  }                                                                     \
                                                                        \
  /** Erase a value from the dumpi_hm. */				\
  static inline void DUMPI_J3(dumpi_hm_,LABEL,_erase)			\
    (DUMPI_J2(dumpi_hm_,LABEL) *hm, KEY_TYPE key)                       \
  {                                                                     \
    DUMPI_J2(keyval_,LABEL) **prev, *curr;                              \
    prev = &hm->bucket[DUMPI_J4(dumpi_hash_,LABEL,_,KEY_TYPE)(key)];    \
    curr = *prev;                                                       \
    while(curr) {                                                       \
      if(curr->key == key) {                                            \
        *prev = curr->next;                                             \
        free(curr);                                                     \
        return;                                                         \
      }                                                                 \
      prev = &((*prev)->next);                                          \
      curr = *prev;                                                     \
    }                                                                   \
  }                                                                     \
                                                                        \
  /* End of DECLARE_HASHMAP definition */

  /*@}*/ /* End of documentation module */

#ifdef __cplusplus
} /* End of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_HASHMAP_H */