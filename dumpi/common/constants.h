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

#ifndef DUMPI_COMMON_CONSTANTS_H
#define DUMPI_COMMON_CONSTANTS_H

#include <dumpi/common/types.h>
#include <dumpi/common/settings.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common
   * \defgroup common_constants  common/constants:  A collection of \
   *    constant values to parallel datatypes, operations, etc.     \
   *    from the MPI standard.
   */
  /*@{*/

  /** Special value to match MPI_ROOT */
#define DUMPI_ROOT -2
  /** Special value to match MPI_ANY_SOURCE */
#define DUMPI_ANY_SOURCE -1
  /** Special value to match MPI_ANY_TAG */
#define DUMPI_ANY_TAG -1

  /**
   * Topologies defined by the MPI standard (DUMPI_TOPOLOGY_ERROR is special).
   */
  typedef enum dumpi_native_topologies {
    DUMPI_TOPOLOGY_ERROR=0,
    DUMPI_GRAPH,
    DUMPI_CART,
    DUMPI_UNDEFINED
  } dumpi_native_topologies;

  /** Get the std::string name of a topology. */
  const char* dumpi_get_topology_name(dumpi_topology topo);

  /**
   * Combiners (the MPI-2 standard is completely insane)
   */
  typedef enum dumpi_native_combiners {
    DUMPI_COMBINER_ERROR=0,          DUMPI_COMBINER_NAMED,
    DUMPI_COMBINER_DUP,              DUMPI_COMBINER_CONTIGUOUS,
    DUMPI_COMBINER_VECTOR,           DUMPI_COMBINER_HVECTOR_INTEGER,
    DUMPI_COMBINER_HVECTOR,          DUMPI_COMBINER_INDEXED,
    DUMPI_COMBINER_HINDEXED_INTEGER, DUMPI_COMBINER_HINDEXED,
    DUMPI_COMBINER_INDEXED_BLOCK,    DUMPI_COMBINER_STRUCT_INTEGER,
    DUMPI_COMBINER_STRUCT,           DUMPI_COMBINER_SUBARRAY,
    DUMPI_COMBINER_DARRAY,           DUMPI_COMBINER_F90_REAL,
    DUMPI_COMBINER_F90_COMPLEX,      DUMPI_COMBINER_F90_INTEGER,
    DUMPI_COMBINER_RESIZED
  } dumpi_native_combiners;

  /** Get the std::string name of a combiner. */
  const char* dumpi_get_combiner_name(dumpi_combiner comb);

  /**
   * Datatypes.  This enum provides the standard datatypes and the offset for
   * the first user datatype.  In the tracefile these will be packed into
   * some small(ish) integer type, possibly along with a sanity mask (offset)
   * to make sure we are reading the right field.
   */
  typedef enum dumpi_native_datatypes {
    DUMPI_DATATYPE_ERROR=0,
    DUMPI_DATATYPE_NULL,
    DUMPI_CHAR,
    DUMPI_SIGNED_CHAR,
    DUMPI_UNSIGNED_CHAR,
    DUMPI_BYTE,
    DUMPI_WCHAR,
    DUMPI_SHORT,
    DUMPI_UNSIGNED_SHORT,
    DUMPI_INT,
    DUMPI_UNSIGNED,
    DUMPI_LONG,
    DUMPI_UNSIGNED_LONG,
    DUMPI_FLOAT,
    DUMPI_DOUBLE,
    DUMPI_LONG_DOUBLE,
    DUMPI_LONG_LONG_INT,
    DUMPI_UNSIGNED_LONG_LONG,
    DUMPI_LONG_LONG,
    DUMPI_PACKED,
    DUMPI_LB,
    DUMPI_UB,
    DUMPI_FLOAT_INT,
    DUMPI_DOUBLE_INT,
    DUMPI_LONG_INT,
    DUMPI_SHORT_INT,
    DUMPI_2INT,
    DUMPI_LONG_DOUBLE_INT,
    DUMPI_FIRST_USER_DATATYPE
  } dumpi_native_datatypes;
  
  /**
   * Get the std::string name of an MPI datatype.
   * Returns "user-defined" if the value is >= DUMPI_FIRST_USER_DATATYPE 
   */
  const char* dumpi_get_datatype_name(dumpi_datatype dt);

  /**
   * Special file values.
   */
  typedef enum dumpi_native_files {
    DUMPI_FILE_NULL=0,
    DUMPI_FIRST_USER_FILE
  } dumpi_native_files;

  /** String name for a file. */
  const char* dumpi_get_file_name(dumpi_file fl);

  typedef enum dumpi_native_infos {
    DUMPI_INFO_NULL=0,
    DUMPI_FIRST_USER_INFO
  } dumpi_native_infos;

  /** Get a std::string representation of info name. */
  const char* dumpi_get_info_name(dumpi_info inf);

  /**
   * Built-in keyvals recognized by MPI (other than DUMPI_FIRST_USER_KEYVAL).
   */
  typedef enum dumpi_native_keyvals {
    DUMPI_TAG_UB=0,        DUMPI_HOST,           DUMPI_IO,
    DUMPI_WTIME_IS_GLOBAL, DUMPI_UNIVERSE_SIZE,  DUMPI_LASTUSEDCODE,
    DUMPI_APPNUM,          DUMPI_WIN_BASE,       DUMPI_WIN_SIZE,
    DUMPI_WIN_DISP_UNIT,   DUMPI_FIRST_USER_KEYVAL
  } dumpi_native_keyvals;

  /** Get the std::string name for a keyval. */
  const char* dumpi_get_keyval_name(dumpi_keyval kv);

  /**
   * MPI operations.  The values for the operations are densely packed and
   * upped by a constant offset (just like datatypes).  User-defined ops
   * will be indexed starting at DUMPI_FIRST_USER_OP.
   */
  typedef enum dumpi_native_ops {
    DUMPI_OP_ERROR=0,
    DUMPI_MAX,
    DUMPI_MIN,
    DUMPI_SUM,
    DUMPI_PROD,
    DUMPI_LAND,
    DUMPI_BAND,
    DUMPI_LOR,
    DUMPI_BOR,
    DUMPI_LXOR,
    DUMPI_BXOR,
    DUMPI_MINLOC,
    DUMPI_MAXLOC,
    DUMPI_REPLACE,
    DUMPI_OP_NULL,
    DUMPI_FIRST_USER_OP
  } dumpi_native_ops;

  /**
   * Get the std::string name of an MPI operation.
   * Returns "user-defined-op" if the value is >= DUMPI_FIRST_USER_DATATYPE 
   */
  const char* dumpi_get_op_name(dumpi_op dt);

  /** Comparison flags */
  typedef enum dumpi_native_comparisons {
    DUMPI_COMPARISON_ERROR=0,
    DUMPI_IDENT,
    DUMPI_CONGRUENT,
    DUMPI_SIMILAR,
    DUMPI_UNEQUAL
  } dumpi_native_comparisons;

  /**
   * Get the std::string name of an MPI comparison.
   * Returns "undefined-comparison" if the value is not defined 
   */
  const char* dumpi_get_comparison_name(dumpi_comparison cmp);

  /**
   * Stash group info into a short integer.
   */
  typedef enum dumpi_native_groups {
    DUMPI_GROUP_ERROR=0,
    DUMPI_GROUP_NULL,
    DUMPI_GROUP_EMPTY,
    DUMPI_FIRST_USER_GROUP
  } dumpi_native_groups;

  /**
   * Get the std::string name of an MPI group.
   * Returns "user-defined-group" if the value is >= DUMPI_FIRST_USER_GROUP. 
   */
  const char* dumpi_get_group_name(dumpi_group grp);

  /**
   * Pack communicator index into a short integer value.
   */
  typedef enum dumpi_native_comms {
    DUMPI_COMM_ERROR=0,
    DUMPI_COMM_NULL,
    DUMPI_COMM_WORLD,
    DUMPI_COMM_SELF,
    DUMPI_FIRST_USER_COMM
  } dumpi_native_comms;

  /**
   * Get the std::string name of an MPI communicator
   * Returns "user-defined-comm" if the value is >= DUMPI_FIRST_USER_COMM. */
  const char* dumpi_get_comm_name(dumpi_comm comm);

  /**
   * Built-in request values.
   */
  typedef enum dumpi_native_requests {
    DUMPI_REQUEST_ERROR=0,
    DUMPI_REQUEST_NULL,
    DUMPI_FIRST_USER_REQUEST
  } dumpi_native_requests;

  /**
   * Request values associated with IO requests.
   */
  typedef enum dumpio_native_requests {
    DUMPIO_REQUEST_ERROR=0,
    DUMPIO_REQUEST_NULL,
    DUMPIO_FIRST_USER_REQUEST
  } dumpio_native_requests;

  /**
   * Get the std::string name of an MPI communicator
   * Returns "user-defined-request" if >= DUMPI_FIRST_USER_REQUEST. 
   */
  const char* dumpi_get_request_name(dumpi_request req);

  /** These are required to differ from a NULL pointer in the MPI standard */
#define DUMPI_STATUS_IGNORE NULL
#define DUMPI_STATUSES_IGNORE DUMPI_STATUS_IGNORE

  /**
   * Built-in distributions.
   */
  typedef enum dumpi_native_distributions {
    DUMPI_DISTRIBUTION_ERROR=0,
    DUMPI_DISTRIBUTE_BLOCK,
    DUMPI_DISTRIBUTE_CYCLIC,
    DUMPI_DISTRIBUTE_NONE,
    DUMPI_DISTRIBUTE_DFLT_DARG
  } dumpi_native_distributions;

  /**
   * Mapping of error handlers onto short integer values.
   */
  typedef enum dumpi_native_errhandlers {
    DUMPI_ERRHANDLER_ERROR=0,
    DUMPI_ERRHANDLER_NULL,
    DUMPI_FIRST_USER_ERRHANDLER
  } dumpi_native_errhandlers;

  /** Get the std::string name for an errhandler. */
  const char* dumpi_get_errhandler_name(dumpi_errhandler eh);
  
  /**
   * File modes.
   */
  typedef enum dumpi_native_filemode {
    DUMPI_FILEMODE_ERROR=0,
    DUMPI_MODE_RDONLY,
    DUMPI_MODE_RDWR,
    DUMPI_MODE_WRONLY,
    DUMPI_MODE_CREATE,
    DUMPI_MODE_EXCL,
    DUMPI_MODE_DELETE_ON_CLOSE,
    DUMPI_MODE_UNIQUE_OPEN,
    DUMPI_MODE_SEQUENTIAL,
    DUMPI_MODE_APPEND
  } dumpi_native_filemode;

  /** Get a std::string representation of a file mode. */
  const char* dumpi_get_filemode_name(dumpi_filemode fm);

  /**
   * File lock types.
   */
  typedef enum dumpi_native_locktypes {
    DUMPI_LOCKTYPE_ERROR=0,
    DUMPI_LOCK_SHARED,
    DUMPI_LOCK_EXCLUSIVE
  } dumpi_native_locktypes;

  /** Get a std::string representation of a lock type */
  const char* dumpi_get_locktype_name(dumpi_locktype lt);

  /**
   * Array orderings.
   */
  typedef enum dumpi_native_orderings {
    DUMPI_ORDERING_ERROR=0,
    DUMPI_ORDER_C,
    DUMPI_ORDER_FORTRAN
  } dumpi_native_orderings;

  /** Get a std::string representation of array ordering */
  const char* dumpi_get_ordering_name(dumpi_ordering dr);

  /**
   * Thread support levels.
   */
  typedef enum dumpi_native_threadlevels {
    DUMPI_THREADLEVEL_ERROR=0,
    DUMPI_THREAD_SINGLE,
    DUMPI_THREAD_FUNNELED,
    DUMPI_THREAD_SERIALIZED,
    DUMPI_THREAD_MULTIPLE
  } dumpi_native_threadlevels;

  /** Get a std::string representation of thread support level. */
  const char* dumpi_get_threadlevel_name(dumpi_threadlevel tn);

  /**
   * Type classes.
   */
  typedef enum dumpi_native_typeclasses {
    DUMPI_TYPECLASS_ERROR=0,
    DUMPI_TYPECLASS_REAL,
    DUMPI_TYPECLASS_INTEGER,
    DUMPI_TYPECLASS_COMPLEX
  } dumpi_native_typeclasses;

  /** Get a std::string representation of typeclass */
  const char* dumpi_get_typeclass_name(dumpi_typeclass tc);

  /**
   * Windows for MPI_File operations.
   */
  typedef enum dumpi_native_wins {
    DUMPI_WIN_NULL=0,
    DUMPI_FIRST_USER_WIN
  } dumpi_native_wins;

  /** Get a std::string representation for file windows. */
  const char* dumpi_get_win_name(dumpi_win win);

  /**
   * File positioning.
   */
  typedef enum dumpi_native_whence {
    DUMPI_WHENCE_ERROR=0,
    DUMPI_SEEK_SET,
    DUMPI_SEEK_CUR,
    DUMPI_SEEK_END
  } dumpi_native_whence;

  /** Get a std::string representation of file positioning. */
  const char* dumpi_get_whence_name(dumpi_whence whence);

  /**
   * Assertions.
   */
  typedef enum dumpi_native_win_assert {
    DUMPI_MODE_ERROR=-1,
    DUMPI_MODE_NONE=0,
    DUMPI_MODE_NOCHECK=1,
    DUMPI_MODE_NOSTORE=2,
    DUMPI_MODE_NOPUT=4,
    DUMPI_MODE_NOPRECEDE=8,
    DUMPI_MODE_NOSUCCEED=16
  } dumpi_native_win_assert;

  /** Get a std::string representation of an assertion. */
  const char* dumpi_get_win_assert_name(dumpi_win_assert dwa);

  /*@}*/

#ifdef __cplusplus
} /* End of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_COMMON_CONSTANTS_H */