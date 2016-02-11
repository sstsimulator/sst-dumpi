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
