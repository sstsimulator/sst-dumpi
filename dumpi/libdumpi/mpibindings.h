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

#ifndef DUMPI_LIBDUMPI_MPIBINDINGS_H
#define DUMPI_LIBDUMPI_MPIBINDINGS_H

#include <dumpi/libdumpi/mpibindings-maps.h>
#include <dumpi/libdumpi/mpibindings-utils.h>
#include <dumpi/libdumpi/init.h>
#include <dumpi/libdumpi/data.h>
#include <dumpi/common/gettime.h>
#include <dumpi/common/perfctrs.h>
#include <dumpi/common/types.h>
#include <dumpi/common/funclabels.h>
#include <dumpi/common/debugflags.h>
#include <dumpi/dumpiconfig.h>
#include <assert.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */
 
  /**
   * \ingroup libdumpi_internal
   * \defgroup libdumpi_internal_mpibindings libdumpi/internal/mpibindings: \
   *    Utility routines for handling data used by the MPI bindings.
   */
  /*@{*/ /* Define doxygen comment scope */


  /** Test whether profiling is turned on for the given dumpi function label */
#define DUMPI_PROFILING(FUNC)                                                 \
  (dumpi_global->output->function[FUNC] && dumpi_global->output->function[DUMPI_ALL_FUNCTIONS])

  /** Increment the count for how often a given function has been
   * profiled but not output to the stream */
#define DUMPI_INCREMENT_IGNORED(FUNC) do {                 \
  ++dumpi_global->footer->ignored_count[FUNC];                \
  ++dumpi_global->footer->ignored_count[DUMPI_ALL_FUNCTIONS]; \
} while(0)

  /** Increment the count for how often a function has been called,
   * whether it was output to the stream or not */
#define DUMPI_INCREMENT_CALLED(FUNC) do {		      \
  ++dumpi_global->footer->call_count[FUNC];                   \
  ++dumpi_global->footer->call_count[DUMPI_ALL_FUNCTIONS];    \
} while(0)

  /** Common setup stuff used in all MPI_* functions.
   * This is an ugly side-effect of the auto-generated MPI bindings */
#define DUMPI_INSERT_PREAMBLE(FUNC)					\
  int profiling, call_depth;						\
  call_depth = libdumpi_enter_mpi();					\
  assert(call_depth > 0);						\
  if(dumpi_debug & DUMPI_DEBUG_MPICALLS)				\
    fprintf(stderr, "[DUMPI-MPI] Entered " #FUNC			\
	    " at MPI call depth %d\n", call_depth);			\
  if(dumpi_global == NULL && call_depth == 1) {				\
    if(dumpi_debug & DUMPI_DEBUG_MPICALLS)				\
      fprintf(stderr, "[DUMPI-MPI] Initializing libdumpi\n");		\
    libdumpi_init();							\
    if(dumpi_debug & DUMPI_DEBUG_MPICALLS)				\
      fprintf(stderr, "[DUMPI-MPI] libdumpi initialized\n");		\
  }									\
  assert(dumpi_global != NULL);						\
  profiling = (call_depth == 1) && DUMPI_PROFILING(FUNC);		\
  if(!profiling) DUMPI_INCREMENT_IGNORED(FUNC);				\
  DUMPI_INCREMENT_CALLED(FUNC)

  /** Common tear-down stuff used in all MPI_* functions.
   * Another ugly side effect of the auto-generated MPI bindings */
#define DUMPI_INSERT_POSTAMBLE do {					\
    call_depth = libdumpi_exit_mpi();					\
    if(dumpi_debug & DUMPI_DEBUG_MPICALLS)				\
      fprintf(stderr, "[DUMPI-MPI] MPI call complete at call depth %d\n", call_depth); \
} while(0)

  /** Special "magical" token indicating that we are processing a
   * C-style character std::string */
#define DUMPI_CSTRING -1

  /** Special "magical" token indicating that we are processing an array
   * of pointers, and that the last element in the array is a NULL pointer. */
#define DUMPI_NULLTERM -2

  /** Shared utility to retrieve time information at the start of an MPI call */
#define DUMPI_START_TIME(CPU, WALL) do {				\
    if(dumpi_global) {							\
      dumpi_get_time(&CPU.start, &WALL.start);				\
      dumpi_get_perfctrs(dumpi_global->profile, dumpi_global->perf, DUMPI_PERFCTR_IN); \
    }									\
} while(0)

  /** Shared utility to retrieve time information at the end of an MPI call */
#define DUMPI_STOP_TIME(CPU, WALL) do {					\
    dumpi_get_time(&CPU.stop, &WALL.stop);				\
    dumpi_get_perfctrs(dumpi_global->profile, dumpi_global->perf, DUMPI_PERFCTR_OUT); \
} while(0)

  /** 
   * Data type used to hold a wall clock timer and cpu timer together.
   * Used in the overhead profiling (currently disabled).
   */
  typedef struct dumpi_clock_pair {
    dumpi_clock wall;
    dumpi_clock cpu;
  } dumpi_clock_pair;

  /*
  static dumpi_clock_pair *overhead_start = NULL;
  static dumpi_clock_pair *overhead_stop = NULL;
  static dumpi_clock_pair *overhead_accum = NULL;
  */

  /** Utility definitino to find the difference between two time values */
#define DUMPI_SUBTRACT_TIME(DEST, LEFT, RIGHT) do {     \
  DEST.nsec = LEFT.nsec - RIGHT.nsec;                   \
  DEST.sec = LEFT.sec - RIGHT.sec;                      \
  /* Only worry about positive results. */              \
  if(DEST.nsec < 0 && DEST.sec > 0) {                   \
    DEST.nsec += 1000000000;                            \
    DEST.sec  -= 1;                                     \
  }                                                     \
} while(0)

  /** Utility routine for starting overhead counting.
   * Currently disabled
   */
#define DUMPI_START_OVERHEAD(FUNC)

  /**
   * Utility routine for stopping overhead counting.
   * Currently disabled
   */
#define DUMPI_STOP_OVERHEAD(FUNC)

/* #define DUMPI_START_OVERHEAD(FUNC) do {                                 \ */
/*   if(overhead_start == NULL) {                                          \ */
/*     overhead_start =                                                    \ */
/*       (dumpi_clock_pair*)malloc(DUMPI_ALL_FUNCTIONS*sizeof(dumpi_clock)); \ */
/*     assert(overhead_start != NULL);                                     \ */
/*   }                                                                     \ */
/*   dumpi_get_time(&overhead_start[FUNC].cpu, &overhead_start[FUNC].wall);\ */
/* } while(0) */

/* #define DUMPI_STOP_OVERHEAD(FUNC) do {                                  \ */
/*   if(overhead_accum == NULL) {                                          \ */
/*     overhead_accum =                                                    \ */
/*       (dumpi_clock_pair*)calloc(DUMPI_ALL_FUNCTIONS, sizeof(dumpi_clock));\ */
/*     assert(overhead_accum != NULL);                                     \ */
/*   }                                                                     \ */
/*   if(overhead_stop == NULL) {                                           \ */
/*     overhead_stop =                                                     \ */
/*       (dumpi_clock_pair*)malloc(DUMPI_ALL_FUNCTIONS*sizeof(dumpi_clock)); \ */
/*     assert(overhead_stop != NULL);                                      \ */
/*   }                                                                     \ */
/*   assert(overhead_start != NULL);                                       \ */
/*   dumpi_get_time(&overhead_stop[FUNC].cpu, &overhead_stop[FUNC].wall);  \ */
/*   DUMPI_SUBTRACT_TIME(overhead_accum[FUNC].wall,                        \ */
/*                       overhead_stop[FUNC].wall, overhead_start[FUNC].wall); \ */
/*   DUMPI_SUBTRACT_TIME(overhead_accum[FUNC].cpu,                         \ */
/*                       overhead_stop[FUNC].cpu, overhead_start[FUNC].cpu); \ */
/* } while(0) */

  /* Conversion/assignment routines */

  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_INT_FROM_INT(A, B) A=(B)
  /** Integer arrays are not copied by value -- just grab the pointer */
#define DUMPI_INT_FROM_INT_ARRAY_1(COUNT, A, B) A=((int*)B)
  /** A "free data" routine -- used by the auto-generated MPI bindings */
#define DUMPI_FREE_INT_FROM_INT(A) do {} while(0)

  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_INT_FROM_INT_ARRAY_2(ROWS, COLS, A, B) A = (B)
  /** A "free data" routine -- used by the auto-generated MPI bindings */
#define DUMPI_FREE_INT_FROM_INT_ARRAY_2(ROWS, A) do {} while(0)

  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_INT_FROM_INT_ARRAY_2_FIXBOUND(ROWS, COLS, A, B) do {    \
  int i, j;							\
  assert(((A) = (int**)malloc((ROWS)*sizeof(int*))) != NULL);	\
  assert(((A)[0] = (int*)malloc((ROWS)*(COLS)*sizeof(int))) != NULL);	\
  /* Set the pointers for elements 1 through n*/                \
  for(i = 1; i < (ROWS); ++i) {                                 \
    (A)[i] = (A)[0] + i*(COLS);                                 \
  }                                                             \
  /* Copy the data (this was left out -- bummer) */		\
  for(i = 0; i < (ROWS); ++i) {					\
    for(j = 0; j < (COLS); ++j) {				\
      (A)[i][j] = (B)[i][j];					\
    }								\
  }								\
} while(0)

  /** A "free data" routine -- used by the auto-generated MPI bindings */
#define DUMPI_FREE_INT_FROM_INT_ARRAY_2_FIXBOUND(ROWS, A) do {        \
  free((A)[0]);                                                 \
  free(A);                                                      \
} while(0)

  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_INT_FROM_MPI_AINT(A, B) A = (B)

  /* Unfortunately, we can't realy on MPI_Aint to have any specific size */
  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_INT_FROM_MPI_AINT_ARRAY_1(COUNT, A, B)  do {    \
  if((COUNT) > 0) {                                     \
    A = (int*)malloc((COUNT)*sizeof(int));              \
    assert(A != NULL);                                  \
    memcpy(A, (B), (COUNT)*sizeof(int));                \
  }                                                     \
  else {                                                \
  A = NULL;                                             \
  }                                                     \
} while(0)

  /* Avoid memory leaks when sizeof(MPI_Aint)!=sizeof(int) */
  /** A "free data" routine -- used by the auto-generated MPI bindings */
#define DUMPI_FREE_INT_FROM_MPI_AINT(A) do {          \
  if((A) != NULL) {                             \
    free(A);                                    \
  }                                             \
} while(0)

  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_INT64T_FROM_MPI_OFFSET(A, B) A=((int64_t)(B))

  /* Cheat a little on the character arrays by null-terminating them */
  /** A type conversion routine -- used by the auto-generated MPI bindings */
#ifdef IS_MPI_TWO
#define DUMPI_CHAR_FROM_CHAR_ARRAY_1(LEN, A, B) do {  \
  char* nonConst = (char*) B; \
  if(LEN >= 0) {                                \
    (nonConst)[LEN]='\0';                              \
  }                                             \
  A = (nonConst);                                      \
} while(0)

  /** A "free data" routine -- used by the auto-generated MPI bindings */
#define DUMPI_FREE_CHAR_FROM_CHAR(A) do {} while(0)
#else
  /* GRH: have to actually allocate, since everything is const in MPI 3 */
#define DUMPI_CHAR_FROM_CHAR_ARRAY_1(LEN, A, B) do {  \
  A = (char*)malloc(sizeof(char) * LEN);  \
  strcpy(A, B);   \
  if(LEN >= 0) {                                \
        (A)[LEN]='\0';                              \
      }   \
} while(0)

  /** A "free data" routine -- used by the auto-generated MPI bindings */
#define DUMPI_FREE_CHAR_FROM_CHAR(A) free(A);

#endif
  /* 
   * Cheat even more on 2D arrays of std::strings
   * by assuming that the output routines handle array dimensions
   * (if this fails in the future, we will need to copy the full std::string array).
   */
  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_CHAR_FROM_CHAR_ARRAY_2(ROWS, COLS, A, B) A=(B)
  /** A "free data" routine -- used by the auto-generated MPI bindings */
#define DUMPI_FREE_CHAR_FROM_CHAR_ARRAY_2(ROWS, A) do {} while(0)

  /* The same cheat as for 2D arrays of characters. */
  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_CHAR_FROM_CHAR_ARRAY_3(BLOCKS, ROWS, COLS, A, B) A=(B)
  /** A "free data" routine -- used by the auto-generated MPI bindings */
#define DUMPI_FREE_CHAR_FROM_CHAR_ARRAY_3(BLOCKS, ROWS, A) do {} while(0)

  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_TAG_FROM_INT(A, B) do {           \
  if((B) == MPI_ANY_TAG) A = DUMPI_ANY_TAG;     \
  else A = (B);                                 \
} while(0)

  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_SOURCE_FROM_INT(A, B) do {                \
  if((B) == MPI_ANY_SOURCE) A = DUMPI_ANY_SOURCE;       \
  else if((B) == MPI_ROOT)  A = DUMPI_ROOT;             \
  else A = (B);                                         \
} while(0)
  
  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_DEST_FROM_INT(A, B) do {                  \
  if((B) == MPI_ROOT)  A = DUMPI_ROOT;                  \
  else A = (B);                                         \
} while(0)

  /** A type conversion routine -- used by the auto-generated MPI bindings */
  static inline dumpi_combiner dumpi_combiner_from_int(int mpi_combiner) {
    switch(mpi_combiner) {
#if DUMPI_HAVE_MPI_COMBINER_NAMED
    case MPI_COMBINER_NAMED: return DUMPI_COMBINER_NAMED;
#endif
#if DUMPI_HAVE_MPI_COMBINER_DUP
    case MPI_COMBINER_DUP: return DUMPI_COMBINER_DUP;
#endif
#if DUMPI_HAVE_MPI_COMBINER_CONTIGUOUS
    case MPI_COMBINER_CONTIGUOUS: return DUMPI_COMBINER_CONTIGUOUS;
#endif
#if DUMPI_HAVE_MPI_COMBINER_VECTOR
    case MPI_COMBINER_VECTOR: return DUMPI_COMBINER_VECTOR;
#endif
#if DUMPI_HAVE_MPI_COMBINER_HVECTOR_INTEGER
    case MPI_COMBINER_HVECTOR_INTEGER: return DUMPI_COMBINER_HVECTOR_INTEGER;
#endif
#if DUMPI_HAVE_MPI_COMBINER_HVECTOR
    case MPI_COMBINER_HVECTOR: return DUMPI_COMBINER_HVECTOR;
#endif
#if DUMPI_HAVE_MPI_COMBINER_INDEXED
    case MPI_COMBINER_INDEXED: return DUMPI_COMBINER_INDEXED;
#endif
#if DUMPI_HAVE_MPI_COMBINER_HINDEXED_INTEGER
    case MPI_COMBINER_HINDEXED_INTEGER: return DUMPI_COMBINER_HINDEXED_INTEGER;
#endif
#if DUMPI_HAVE_MPI_COMBINER_HINDEXED
    case MPI_COMBINER_HINDEXED: return DUMPI_COMBINER_HINDEXED;
#endif
#if DUMPI_HAVE_MPI_COMBINER_INDEXED_BLOCK
    case MPI_COMBINER_INDEXED_BLOCK: return DUMPI_COMBINER_INDEXED_BLOCK;
#endif
#if DUMPI_HAVE_MPI_COMBINER_STRUCT_INTEGER
    case MPI_COMBINER_STRUCT_INTEGER: return DUMPI_COMBINER_STRUCT_INTEGER;
#endif
#if DUMPI_HAVE_MPI_COMBINER_STRUCT
    case MPI_COMBINER_STRUCT: return DUMPI_COMBINER_STRUCT;
#endif
#if DUMPI_HAVE_MPI_COMBINER_SUBARRAY
    case MPI_COMBINER_SUBARRAY: return DUMPI_COMBINER_SUBARRAY;
#endif
#if DUMPI_HAVE_MPI_COMBINER_DARRAY
    case MPI_COMBINER_DARRAY: return DUMPI_COMBINER_DARRAY;
#endif
#if DUMPI_HAVE_MPI_COMBINER_F90_REAL
    case MPI_COMBINER_F90_REAL: return DUMPI_COMBINER_F90_REAL;
#endif
#if DUMPI_HAVE_MPI_COMBINER_F90_COMPLEX
    case MPI_COMBINER_F90_COMPLEX: return DUMPI_COMBINER_F90_COMPLEX;
#endif
#if DUMPI_HAVE_MPI_COMBINER_F90_INTEGER
    case MPI_COMBINER_F90_INTEGER: return DUMPI_COMBINER_F90_INTEGER;
#endif
#if DUMPI_HAVE_MPI_COMBINER_RESIZED
    case MPI_COMBINER_RESIZED: return DUMPI_COMBINER_RESIZED;
#endif
    default: return DUMPI_COMBINER_ERROR;
    }
  }

#define DUMPI_COMBINER_FROM_INT(A, B) A = dumpi_combiner_from_int(B)
  
  /** A type conversion routine -- used by the auto-generated MPI bindings */
#define DUMPI_COMM_FROM_MPI_COMM(A, B)                  \
  A = libdumpi_get_comm(B);

  /** A type conversion routine -- used by the auto-generated MPI bindings */
  static inline dumpi_comparison dumpi_comparison_from_int(int mpi_comparison) {
    switch(mpi_comparison) {
#if DUMPI_HAVE_MPI_IDENT
    case MPI_IDENT: return DUMPI_IDENT;
#endif
#if DUMPI_HAVE_MPI_CONGRUENT
    case MPI_CONGRUENT: return DUMPI_CONGRUENT;
#endif
#if DUMPI_HAVE_MPI_SIMILAR
    case MPI_SIMILAR: return DUMPI_SIMILAR;
#endif
#if DUMPI_HAVE_MPI_UNEQUAL
    case MPI_UNEQUAL: return DUMPI_UNEQUAL;
#endif
    default: return DUMPI_COMPARISON_ERROR;
    }
  }

#define DUMPI_COMPARISON_FROM_INT(A, B) A = dumpi_comparison_from_int(B)

  /**
   * Register the type of a datatype (as reported by MPI_Type_size).
   * This will be enabled in the 0.7 release.
   */
  static void libdumpi_register_datatype_size(dumpi_datatype dumpitype,
                                              MPI_Datatype mpitype)
  {
    int good_to_go;
    PMPI_Initialized(&good_to_go);
    if(good_to_go) {
      if(dumpitype >= dumpi_global->typesize.count) {
        dumpi_datatype i;
        dumpi_global->typesize.size =
          (int*)realloc(dumpi_global->typesize.size, (dumpitype+1)*sizeof(int));
        assert(dumpi_global->typesize.size != NULL);
        for(i = dumpi_global->typesize.count; i <= dumpitype; ++i)
          dumpi_global->typesize.size[i] = 0;
        dumpi_global->typesize.count = dumpitype+1;
      }
      if(dumpi_global->typesize.size[dumpitype] <= 0) {
        if(mpitype != MPI_DATATYPE_NULL)
          PMPI_Type_size(mpitype, dumpi_global->typesize.size + dumpitype);
        else
          dumpi_global->typesize.size[dumpitype] = 0;
	if(dumpi_debug && DUMPI_DEBUG_MPICALLS) {
	  fprintf(stderr, "[DUMPI-MPI] registered datatype %d with size %d\n",
		  dumpitype, dumpi_global->typesize.size[dumpitype]);
	}
      }
    }
  }

  /** Utility definition to register a datatype size */
#define DUMPI_REGISTER_DATATYPE(MPITYPE_) do {                          \
    dumpi_datatype dumpitype;						\
    if(dumpi_debug & DUMPI_DEBUG_MPICALLS)				\
      fprintf(stderr, "[DUMPI-MPI] registering builtin datatype "	\
	      #MPITYPE_ "\n");						\
    dumpitype = libdumpi_get_datatype(MPITYPE_);                        \
    libdumpi_register_datatype_size(dumpitype, MPITYPE_);               \
} while(0)

  /** Register the sizes of built-in datatypes */
  inline static void register_builtin_datatypes() {
    int good_to_go;
    if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
      fprintf(stderr, "[DUMPI-LIBDUMPI] register_builtin_datatypes entering\n");
    PMPI_Initialized(&good_to_go);
    if(good_to_go) {
#if DUMPI_HAVE_MPI_CHAR
      DUMPI_REGISTER_DATATYPE(MPI_CHAR);
#endif
#if DUMPI_HAVE_MPI_SIGNED_CHAR
      DUMPI_REGISTER_DATATYPE(MPI_SIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_CHAR
      DUMPI_REGISTER_DATATYPE(MPI_UNSIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_BYTE
      DUMPI_REGISTER_DATATYPE(MPI_BYTE);
#endif
#if DUMPI_HAVE_MPI_WCHAR
      DUMPI_REGISTER_DATATYPE(MPI_WCHAR);
#endif
#if DUMPI_HAVE_MPI_SHORT
      DUMPI_REGISTER_DATATYPE(MPI_SHORT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_SHORT
      DUMPI_REGISTER_DATATYPE(MPI_UNSIGNED_SHORT);
#endif
#if DUMPI_HAVE_MPI_INT
      DUMPI_REGISTER_DATATYPE(MPI_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED
      DUMPI_REGISTER_DATATYPE(MPI_UNSIGNED);
#endif
#if DUMPI_HAVE_MPI_LONG
      DUMPI_REGISTER_DATATYPE(MPI_LONG);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG
      DUMPI_REGISTER_DATATYPE(MPI_UNSIGNED_LONG);
#endif
#if DUMPI_HAVE_MPI_FLOAT
      DUMPI_REGISTER_DATATYPE(MPI_FLOAT);
#endif
#if DUMPI_HAVE_MPI_DOUBLE
      DUMPI_REGISTER_DATATYPE(MPI_DOUBLE);
#endif
#if DUMPI_HAVE_MPI_LONG_DOUBLE
      DUMPI_REGISTER_DATATYPE(MPI_LONG_DOUBLE);
#endif
#if DUMPI_HAVE_MPI_LONG_LONG_INT
      DUMPI_REGISTER_DATATYPE(MPI_LONG_LONG_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG
      DUMPI_REGISTER_DATATYPE(MPI_UNSIGNED_LONG_LONG);
#endif
#if DUMPI_HAVE_MPI_LONG_LONG
      DUMPI_REGISTER_DATATYPE(MPI_LONG_LONG);
#endif
#if DUMPI_HAVE_MPI_PACKED
      DUMPI_REGISTER_DATATYPE(MPI_PACKED);
#endif
      /*
#if DUMPI_HAVE_MPI_LB
      DUMPI_REGISTER_DATATYPE(MPI_LB);
#endif
#if DUMPI_HAVE_MPI_UB
      DUMPI_REGISTER_DATATYPE(MPI_UB);
#endif
      */
#if DUMPI_HAVE_MPI_FLOAT_INT
      DUMPI_REGISTER_DATATYPE(MPI_FLOAT_INT);
#endif
#if DUMPI_HAVE_MPI_DOUBLE_INT
      DUMPI_REGISTER_DATATYPE(MPI_DOUBLE_INT);
#endif
#if DUMPI_HAVE_MPI_LONG_INT
      DUMPI_REGISTER_DATATYPE(MPI_LONG_INT);
#endif
#if DUMPI_HAVE_MPI_SHORT_INT
      DUMPI_REGISTER_DATATYPE(MPI_SHORT_INT);
#endif
#if DUMPI_HAVE_MPI_2INT
      DUMPI_REGISTER_DATATYPE(MPI_2INT);
#endif
#if DUMPI_HAVE_MPI_LONG_DOUBLE_INT
      DUMPI_REGISTER_DATATYPE(MPI_LONG_DOUBLE_INT);
#endif
    }
    if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
     fprintf(stderr, "[DUMPI-LIBDUMPI] register_builtin_datatypes returning\n");
  }
  
  /** Convert an MPI_Datatype object to a dumpi datatype,
   * but without registration (MPI_Type_size is unsafe before calling
   * MPI_Type_commit) */
#define DUMPI_DATATYPE_FROM_MPI_DATATYPE_NOREG(A, B) do {     \
    A = libdumpi_get_datatype(B);                             \
} while(0)

  /** Convert an MPI_Datatype object to a dumpi datatype */
#define DUMPI_DATATYPE_FROM_MPI_DATATYPE(A, B) do {     \
    A = libdumpi_get_datatype(B);                       \
    libdumpi_register_datatype_size(A, B);		\
} while(0)

  /** Convert an array of MPI_Datatype object to a dumpi datatypes */
#define DUMPI_DATATYPE_FROM_MPI_DATATYPE_ARRAY_1(COUNT, A, B) do {      \
  if(((COUNT) > 0) && ((B) != NULL)) {                                  \
    int i;                                                              \
    A = (dumpi_datatype*)malloc((COUNT)*sizeof(dumpi_datatype));        \
    assert(A != NULL);                                                  \
    for(i = 0; i < (COUNT); ++i)                                        \
      DUMPI_DATATYPE_FROM_MPI_DATATYPE((A)[i], (B)[i]);                 \
  }                                                                     \
  else {                                                                \
    A = NULL;                                                           \
  }                                                                     \
} while(0)

  /** Free dynamically allocated array of dumpi datatypes */
#define DUMPI_FREE_DATATYPE_FROM_MPI_DATATYPE(A) free(A)

  /** Convert an MPI distribution value to a dumpi distribution */
  static inline dumpi_distribution dumpi_distribution_from_int(int mpi_distribution) {
    switch(mpi_distribution) {
#if DUMPI_HAVE_MPI_DISTRIBUTE_BLOCK
    case MPI_DISTRIBUTE_BLOCK: return DUMPI_DISTRIBUTE_BLOCK;
#endif
#if DUMPI_HAVE_MPI_DISTRIBUTE_CYCLIC
    case MPI_DISTRIBUTE_CYCLIC: return DUMPI_DISTRIBUTE_CYCLIC;
#endif
#if DUMPI_HAVE_MPI_DISTRIBUTE_NONE
    case MPI_DISTRIBUTE_NONE: return DUMPI_DISTRIBUTE_NONE;
#endif
#if DUMPI_HAVE_MPI_DISTRIBUTE_DFLT_DARG
    case MPI_DISTRIBUTE_DFLT_DARG: return DUMPI_DISTRIBUTE_DFLT_DARG;
#endif
    default: return DUMPI_DISTRIBUTION_ERROR;
    }
}

#define DUMPI_DISTRIBUTION_FROM_INT(A, B) A = dumpi_distribution_from_int(B)

  /** Convert an array of MPI distributions to their dumpi counterparts */
#define DUMPI_DISTRIBUTION_FROM_INT_ARRAY_1(COUNT, A, B) do {           \
    if((COUNT) > 0) {                                                   \
      int i;                                                              \
      A = (dumpi_distribution*)malloc((COUNT)*sizeof(dumpi_distribution)); \
      assert((A) != NULL);                                              \
      for(i = 0; i < (COUNT); ++i)                                      \
        DUMPI_DISTRIBUTION_FROM_INT((A)[i], (B)[i]);                    \
    }                                                                   \
    else {                                                              \
      A = NULL;                                                         \
    }                                                                   \
} while(0)

  /** Free a dynamically allocated array of dumpi datatypes. */
#define DUMPI_FREE_DISTRIBUTION_FROM_INT(A) free(A)

  /** Convert an MPI errorcode to a dumpi errorcode */
#define DUMPI_ERRCODE_FROM_INT(A, B)            \
  A = ((B)==MPI_SUCCESS ? DUMPI_SUCCESS : (B))

  /** Convert an array of MPI errorcodes to dumpi errorcodes */
#define DUMPI_ERRCODE_FROM_INT_ARRAY_1(COUNT, A, B) do {                  \
    if((COUNT)>0 && (B)!=NULL) {                                        \
      int i;                                                            \
      A = (dumpi_errcode*)malloc((COUNT)*sizeof(dumpi_errcode));        \
      assert(A != NULL);                                                \
      for(i = 0; i < (COUNT); ++i) {                                    \
        DUMPI_ERRCODE_FROM_INT((A)[i], (B)[i]);                         \
      }                                                                 \
    }                                                                   \
} while(0)

  /** Free a dynamically allocated array of dumpi errorcodes */
#define DUMPI_FREE_ERRCODE_FROM_INT(A) free(A)

  /** Convert an MPI errhandler to a dumpi errhandler */
#define DUMPI_ERRHANDLER_FROM_MPI_ERRHANDLER(A, B) \
  A = libdumpi_get_errhandler(B);

  /** Convert an MPI file to a dumpi file */
#define DUMPI_FILE_FROM_MPI_FILE(A, B)          \
  A = libdumpi_get_file(B)

  /** Convert an MPI file mode to a dumpi file mode */
  static inline dumpi_filemode dumpi_filemode_from_int(int mpi_filemode) {
    switch(mpi_filemode) {
#if DUMPI_HAVE_MPI_MODE_RDONLY
    case MPI_MODE_RDONLY:          return DUMPI_MODE_RDONLY;
#endif
#if DUMPI_HAVE_MPI_MODE_RDWR
    case MPI_MODE_RDWR:            return DUMPI_MODE_RDWR;
#endif
#if DUMPI_HAVE_MPI_MODE_WRONLY
    case MPI_MODE_WRONLY:          return DUMPI_MODE_WRONLY;
#endif
#if DUMPI_HAVE_MPI_MODE_CREATE
    case MPI_MODE_CREATE:          return DUMPI_MODE_CREATE;
#endif
#if DUMPI_HAVE_MPI_MODE_EXCL
    case MPI_MODE_EXCL:            return DUMPI_MODE_EXCL;
#endif
#if DUMPI_HAVE_MPI_MODE_DELETE_ON_CLOSE
    case MPI_MODE_DELETE_ON_CLOSE: return DUMPI_MODE_DELETE_ON_CLOSE;
#endif
#if DUMPI_HAVE_MPI_MODE_UNIQUE_OPEN
    case MPI_MODE_UNIQUE_OPEN:     return DUMPI_MODE_UNIQUE_OPEN;
#endif
#if DUMPI_HAVE_MPI_MODE_SEQUENTIAL
    case MPI_MODE_SEQUENTIAL:      return DUMPI_MODE_SEQUENTIAL;
#endif
#if DUMPI_HAVE_MPI_MODE_APPEND
    case MPI_MODE_APPEND:          return DUMPI_MODE_APPEND;
#endif
    default:                       return DUMPI_FILEMODE_ERROR;
    }
  }

#define DUMPI_FILEMODE_FROM_INT(A, B) A = dumpi_filemode_from_int(B)

  /** Convert an MPI group to a dumpi group */
#define DUMPI_GROUP_FROM_MPI_GROUP(A, B)        \
  A = libdumpi_get_group(B)

  /** Convert an MPI_Info object to a dumpi info handle */
#define DUMPI_INFO_FROM_MPI_INFO(A, B)          \
  A = libdumpi_get_info(B)

  /** Convert an array of MPI_Info objects to their dumpi counterparts */
#define DUMPI_INFO_FROM_MPI_INFO_ARRAY_1(COUNT, A, B) do {      \
  if((COUNT) > 0 && (B) != NULL) {                              \
    int i;                                                      \
    A = (dumpi_info*)malloc((COUNT)*sizeof(dumpi_info));        \
    assert(A != NULL);                                          \
    for(i = 0; i < (COUNT); ++i) {                              \
      DUMPI_INFO_FROM_MPI_INFO((A)[i], (B)[i]);                 \
    }                                                           \
  }                                                             \
  else {                                                        \
    A = NULL;                                                   \
  }                                                             \
} while(0)

  /** Free a dynamically allocated array of dumpi info handles. */
#define DUMPI_FREE_INFO_FROM_MPI_INFO(A) free(A)

  /** Convert an MPI lock to a dumpi lock handle. */
  static inline dumpi_locktype dumpi_locktype_from_int(int mpi_locktype) {
    switch(mpi_locktype) {
#if DUMPI_HAVE_MPI_LOCK_SHARED
    case MPI_LOCK_SHARED:    return DUMPI_LOCK_SHARED;
#endif
#if DUMPI_HAVE_MPI_LOCK_EXCLUSIVE
    case MPI_LOCK_EXCLUSIVE: return DUMPI_LOCK_EXCLUSIVE;
#endif
    default: return DUMPI_LOCKTYPE_ERROR;
    }
}

#define DUMPI_LOCKTYPE_FROM_INT(A, B) A = dumpi_locktype_from_int(B)

  /** Convert an MPI_Op to its dumpi counterpart */
#define DUMPI_OP_FROM_MPI_OP(A, B)              \
  A = libdumpi_get_op(B)

  /** Convert an MPI ordering (C or Fortran) to its dumpi counterpart */
  static inline dumpi_ordering dumpi_ordering_from_int(int mpi_ordering) {
    switch(mpi_ordering) {
#if DUMPI_HAVE_MPI_ORDER_C
    case MPI_ORDER_C:       return DUMPI_ORDER_C;
#endif
#if DUMPI_HAVE_MPI_ORDER_FORTRAN
    case MPI_ORDER_FORTRAN: return DUMPI_ORDER_FORTRAN;
#endif
    default: return DUMPI_ORDERING_ERROR;
    }
  }

#define DUMPI_ORDERING_FROM_INT(A, B) A = dumpi_ordering_from_int(B)

  /** Get the dumpi request handle corresponding to an MPI_Request. */
#define DUMPI_REQUEST_FROM_MPI_REQUEST(A, B)          \
  A = libdumpi_get_request(B)

  /** Get the dumpi request handles corresponding to an array of MPI_Requests */
#define DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(COUNT, A, B) do {      \
  if((COUNT) > 0 && (B) != NULL) {                                      \
    int i;                                                              \
    A = (dumpi_request*)malloc((COUNT)*sizeof(dumpi_request));        \
    assert(A != NULL);                                                  \
    for(i = 0; i < (COUNT); ++i) {                                      \
      DUMPI_REQUEST_FROM_MPI_REQUEST((A)[i], (B)[i]);                 \
    }                                                                   \
  }                                                                     \
  else {                                                                \
    A = NULL;                                                           \
  }                                                                     \
} while(0)

  /** Free a dynamically allocated array of dumpi request handles */
#define DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(A) free(A)

  /** Autoconf test for whether MPIO_Request is the same as MPI_Request */
#ifdef MPIO_USES_MPI_REQUEST

#define DUMPIO_REQUEST_FROM_MPIO_REQUEST(A, B)  \
  DUMPI_REQUEST_FROM_MPI_REQUEST(A, B)

#define DUMPIO_REQUEST_FROM_MPIO_REQUEST_ARRAY_1(COUNT, A, B)   \
  DUMPI_REQUEST_FROM_MPI_REQUEST_ARRAY_1(COUNT, A, B)

#define DUMPIO_FREE_REQUEST_FROM_MPIO_REQUEST(A)        \
  DUMPI_FREE_REQUEST_FROM_MPI_REQUEST(A)

#else /* ! MPIO_USES_MPI_REQUEST */

#define DUMPIO_REQUEST_FROM_MPIO_REQUEST(A, B)          \
  A = libdumpi_get_iorequest(B)

#define DUMPIO_REQUEST_FROM_MPIO_REQUEST_ARRAY_1(COUNT, A, B) do {      \
  if((COUNT) > 0 && (B) != NULL) {                                      \
    int i;                                                              \
    A = (dumpio_request*)malloc((COUNT)*sizeof(dumpio_request));        \
    assert(A != NULL);                                                  \
    for(i = 0; i < (COUNT); ++i) {                                      \
      DUMPIO_REQUEST_FROM_MPIO_REQUEST((A)[i], (B)[i]);                 \
    }                                                                   \
  }                                                                     \
  else {                                                                \
    A = NULL;                                                           \
  }                                                                     \
} while(0)

#define DUMPIO_FREE_REQUEST_FROM_MPIO_REQUEST(A) free(A)

#endif /* MPIO_REQUEST_USES_MPI_REQUEST */

  /** Build a dumpi_status object from the given MPI_Status pointer. */
#define DUMPI_STATUS_FROM_MPI_STATUS_PTR(A, B)          \
  DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(1, A, B);

  /** Build an array of dumpi_status objects from the given MPI_Status pointer*/
#define DUMPI_STATUS_FROM_MPI_STATUS_ARRAY_1(COUNT, A, B) do {          \
  if(dumpi_global->output->statuses) {                                                \
    if(((COUNT)<=0) || ((B) == NULL) ||                                 \
       ((B) == MPI_STATUS_IGNORE) || ((B) == MPI_STATUSES_IGNORE))      \
      A = DUMPI_STATUS_IGNORE;                                          \
    else {                                                              \
      int i, scratch;                                                   \
      A = (dumpi_status*)malloc((COUNT)*sizeof(dumpi_status));          \
      assert(A != NULL);                                                \
      for(i = 0; i < (COUNT); ++i) {                                    \
        /* Figure out the status information. */                        \
        PMPI_Get_count((B)+i, MPI_CHAR, &A[i].bytes);                   \
        DUMPI_SOURCE_FROM_INT(A[i].source, (B)[i].MPI_SOURCE);          \
        DUMPI_TAG_FROM_INT(A[i].tag, (B)[i].MPI_TAG);                   \
        PMPI_Test_cancelled((B)+i, &scratch);                           \
        A[i].cancelled = scratch;                                       \
        A[i].error = (B)[i].MPI_ERROR;                                  \
      }                                                                 \
    }                                                                   \
  }                                                                     \
} while(0)

  /** Free a dynamically allocated dumpi_status */
#define DUMPI_FREE_STATUS_FROM_MPI_STATUS(A) free(A)

  /** Convert MPI thread level request to dumpi thread level value */
  static inline dumpi_threadlevel dumpi_threadlevel_from_int(int mpilevel) {
    switch(mpilevel) {
#if DUMPI_HAVE_MPI_THREAD_SINGLE
    case MPI_THREAD_SINGLE:     return DUMPI_THREAD_SINGLE;
#endif
#if DUMPI_HAVE_MPI_THREAD_FUNNELED
    case MPI_THREAD_FUNNELED:   return DUMPI_THREAD_FUNNELED;
#endif
#if DUMPI_HAVE_MPI_THREAD_SERIALIZED
    case MPI_THREAD_SERIALIZED: return DUMPI_THREAD_SERIALIZED;
#endif
#if DUMPI_HAVE_MPI_THREAD_MULTIPLE
    case MPI_THREAD_MULTIPLE:   return DUMPI_THREAD_MULTIPLE;
#endif
    default: return DUMPI_THREADLEVEL_ERROR;
    }
  }

#define DUMPI_THREADLEVEL_FROM_INT(A, B) A = dumpi_threadlevel_from_int(B)

  /** Convert MPI topology flag to dumpi topology value */
  static inline dumpi_topology dumpi_topology_from_int(int mpi_topology) {
    switch(mpi_topology) {
#if DUMPI_HAVE_MPI_GRAPH
    case MPI_GRAPH: return DUMPI_GRAPH;
#endif
#if DUMPI_HAVE_MPI_CART
    case MPI_CART: return DUMPI_CART;
#endif
#if DUMPI_HAVE_MPI_UNDEFINED
    case MPI_UNDEFINED: return DUMPI_UNDEFINED;
#endif
    default: return DUMPI_TOPOLOGY_ERROR; break;
    }
  }

#define DUMPI_TOPOLOGY_FROM_INT(A, B) A = dumpi_topology_from_int(B)

  /** Convert an MPI type class to the corresponding dumpi type class */
#define DUMPI_TYPECLASS_FROM_INT(A, B) A = dumpi_typeclass_from_int(B)

    static inline dumpi_typeclass dumpi_typeclass_from_int(int mpi_typeclass) {
      switch(mpi_typeclass) {
#if DUMPI_HAVE_MPI_TYPECLASS_REAL
      case MPI_TYPECLASS_REAL:    return DUMPI_TYPECLASS_REAL;
#endif
#if DUMPI_HAVE_MPI_TYPECLASS_INTEGER
      case MPI_TYPECLASS_INTEGER: return DUMPI_TYPECLASS_INTEGER;
#endif
#if DUMPI_HAVE_MPI_TYPECLASS_COMPLEX
      case MPI_TYPECLASS_COMPLEX: return DUMPI_TYPECLASS_COMPLEX;
#endif
      default: return DUMPI_TYPECLASS_ERROR;
      }
    }

  /** Convert an MPI whence value (for file seek) to the dumpi whence value */
  static inline dumpi_whence dumpi_whence_from_int(int mpi_whence) {
    switch(mpi_whence) {
#if DUMPI_HAVE_MPI_SEEK_SET
    case MPI_SEEK_SET: return DUMPI_SEEK_SET;
#endif
#if DUMPI_HAVE_MPI_SEEK_CUR
    case MPI_SEEK_CUR: return DUMPI_SEEK_CUR;
#endif
#if DUMPI_HAVE_MPI_SEEK_END
    case MPI_SEEK_END: return DUMPI_SEEK_END;
#endif
    default: return DUMPI_WHENCE_ERROR;
    }
  }

#define DUMPI_WHENCE_FROM_INT(A, B) A = dumpi_whence_from_int(B)

  /** convert an MPI win assertion (integer) to the proper dumpi value */
  static inline dumpi_win_assert dumpi_win_assert_from_int(int mpi_assert) {
    switch(mpi_assert) {
#if DUMPI_HAVE_MPI_MODE_NOCHECK
    case MPI_MODE_NOCHECK: return DUMPI_MODE_NOCHECK;
#endif
#if DUMPI_HAVE_MPI_MODE_NOSTORE
    case MPI_MODE_NOSTORE: return DUMPI_MODE_NOSTORE;
#endif
#if DUMPI_HAVE_MPI_MODE_NOPUT
    case MPI_MODE_NOPUT:   return DUMPI_MODE_NOPUT;
#endif
#if DUMPI_HAVE_MPI_MODE_NOPRECEDE
    case MPI_MODE_NOPRECEDE: return DUMPI_MODE_NOPRECEDE;
#endif
#if DUMPI_HAVE_MPI_MODE_NOSUCCEED
    case MPI_MODE_NOSUCCEED: return DUMPI_MODE_NOSUCCEED;
#endif
    default: return DUMPI_MODE_ERROR;
    }
  }

#define DUMPI_WIN_ASSERT_FROM_INT(A, B) A = dumpi_win_assert_from_int(B)

  /** Convert an MPI keyval handle (int) to the corresponding dumpi_keyval */
#define DUMPI_KEYVAL_FROM_INT(A, B)             \
  A = libdumpi_get_keyval(B)

  /** Convert an MPI comm keyval (int) to the corresponding dumpi_comm_keyval */
#define DUMPI_COMM_KEYVAL_FROM_INT(A, B)        \
  A = libdumpi_get_keyval(B)

  /** Convert an MPI type keyval (int) to the corresponding dumpi_type_keyval */
#define DUMPI_TYPE_KEYVAL_FROM_INT(A, B)        \
  A = libdumpi_get_keyval(B)

  /** Convert an MPI win keyval (int) to the corresponding dumpi_win_keyval */
#define DUMPI_WIN_KEYVAL_FROM_INT(A, B)         \
  A = libdumpi_get_keyval(B)

  /** Convert an MPI_Win value to the corresponding dumpi_win */
#define DUMPI_WIN_FROM_MPI_WIN(A, B)            \
  A = libdumpi_get_win(B)

  /*@}*/ /* close doxygen comment scope */

#ifdef __cplusplus
} /* End of extern C block */
#endif /* ! __cplusplus */

#endif /* DUMPI_LIBDUMPI_MPIBINDINGS_H */
