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

#include <dumpi/libdumpi/mpibindings-maps.h>
#include <dumpi/common/hashmap.h>
#include <dumpi/common/types.h>

/**
 * Create the bindings for a shared hash map for MPI_Comm
 */
DUMPI_DECLARE_HASHMAP(comm, MPI_Comm, dumpi_comm)

/**
 * Create a handle for the actual shared MPI_Comm hash map.
 */
static dumpi_hm_comm *comm_map = NULL;

/**
 * Add built-in communicators to the MPI_Comm hash map.
 */
void libdumpi_init_comm_map(void) {
  if(comm_map == NULL) {
    dumpi_hm_comm_init(&comm_map, DUMPI_FIRST_USER_COMM);
    dumpi_hm_comm_set(comm_map, MPI_COMM_NULL, DUMPI_COMM_NULL);
    dumpi_hm_comm_set(comm_map, MPI_COMM_WORLD, DUMPI_COMM_WORLD);
    dumpi_hm_comm_set(comm_map, MPI_COMM_SELF, DUMPI_COMM_SELF);
  }
}

/*
 * Get a communicator handle.
 */
dumpi_comm libdumpi_get_comm(MPI_Comm comm) {
  libdumpi_init_comm_map();
  return dumpi_hm_comm_get(comm_map, comm);
}

/**
 * Create the bindings for a shared hash map for MPI_Datatype.
 */
DUMPI_DECLARE_HASHMAP(datatype, MPI_Datatype, dumpi_datatype)

/**
 * Create a handle for the actual shared MPI_Datatype hash map.
 */
static dumpi_hm_datatype *datatype_map = NULL;

/**
 * Utility typedef to add named datatypes to the MPI_Datatype hash map.
 */
#define DUMPI_ADD_DATATYPE(MPITYPE_) do {                               \
    dumpi_hm_datatype_set(datatype_map, MPITYPE_, DU##MPITYPE_);        \
  } while(0)

/**
 * Add built-in datatypes to the MPI_Datatype hash map.
 */
void libdumpi_init_datatype_map(void) {
  if(datatype_map == NULL) {
    dumpi_hm_datatype_init(&datatype_map, DUMPI_FIRST_USER_DATATYPE);
    DUMPI_ADD_DATATYPE(MPI_DATATYPE_NULL);
#ifdef DUMPI_HAVE_MPI_CHAR
    DUMPI_ADD_DATATYPE(MPI_CHAR);
#endif
#ifdef DUMPI_HAVE_MPI_SIGNED_CHAR
    DUMPI_ADD_DATATYPE(MPI_SIGNED_CHAR);
#endif
#ifdef DUMPI_HAVE_MPI_UNSIGNED_CHAR
    DUMPI_ADD_DATATYPE(MPI_UNSIGNED_CHAR);
#endif
#ifdef DUMPI_HAVE_MPI_BYTE
    DUMPI_ADD_DATATYPE(MPI_BYTE);
#endif
#ifdef DUMPI_HAVE_MPI_WCHAR
    DUMPI_ADD_DATATYPE(MPI_WCHAR);
#endif
#ifdef DUMPI_HAVE_MPI_SHORT
    DUMPI_ADD_DATATYPE(MPI_SHORT);
#endif
#ifdef DUMPI_HAVE_MPI_UNSIGNED_SHORT
    DUMPI_ADD_DATATYPE(MPI_UNSIGNED_SHORT);
#endif
#ifdef DUMPI_HAVE_MPI_INT
    DUMPI_ADD_DATATYPE(MPI_INT);
#endif
#ifdef DUMPI_HAVE_MPI_UNSIGNED
    DUMPI_ADD_DATATYPE(MPI_UNSIGNED);
#endif
#ifdef DUMPI_HAVE_MPI_LONG
    DUMPI_ADD_DATATYPE(MPI_LONG);
#endif
#ifdef DUMPI_HAVE_MPI_UNSIGNED_LONG
    DUMPI_ADD_DATATYPE(MPI_UNSIGNED_LONG);
#endif
#ifdef DUMPI_HAVE_MPI_FLOAT
    DUMPI_ADD_DATATYPE(MPI_FLOAT);
#endif
#ifdef DUMPI_HAVE_MPI_DOUBLE
    DUMPI_ADD_DATATYPE(MPI_DOUBLE);
#endif
#ifdef DUMPI_HAVE_MPI_LONG_DOUBLE
    DUMPI_ADD_DATATYPE(MPI_LONG_DOUBLE);
#endif
#ifdef DUMPI_HAVE_MPI_LONG_LONG_INT
    DUMPI_ADD_DATATYPE(MPI_LONG_LONG_INT);
#endif
#ifdef DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG
    DUMPI_ADD_DATATYPE(MPI_UNSIGNED_LONG_LONG);
#endif
#ifdef DUMPI_HAVE_MPI_LONG_LONG
    DUMPI_ADD_DATATYPE(MPI_LONG_LONG);
#endif
#ifdef DUMPI_HAVE_MPI_PACKED
    DUMPI_ADD_DATATYPE(MPI_PACKED);
#endif
#ifdef DUMPI_HAVE_MPI_LB
    DUMPI_ADD_DATATYPE(MPI_LB);
#endif
#ifdef DUMPI_HAVE_MPI_UB
    DUMPI_ADD_DATATYPE(MPI_UB);
#endif
#ifdef DUMPI_HAVE_MPI_FLOAT_INT
    DUMPI_ADD_DATATYPE(MPI_FLOAT_INT);
#endif
#ifdef DUMPI_HAVE_MPI_DOUBLE_INT
    DUMPI_ADD_DATATYPE(MPI_DOUBLE_INT);
#endif
#ifdef DUMPI_HAVE_MPI_LONG_INT
    DUMPI_ADD_DATATYPE(MPI_LONG_INT);
#endif
#ifdef DUMPI_HAVE_MPI_SHORT_INT
    DUMPI_ADD_DATATYPE(MPI_SHORT_INT);
#endif
#ifdef DUMPI_HAVE_MPI_2INT
    DUMPI_ADD_DATATYPE(MPI_2INT);
#endif
#ifdef DUMPI_HAVE_MPI_LONG_DOUBLE_INT
    DUMPI_ADD_DATATYPE(MPI_LONG_DOUBLE_INT);
#endif
  }
}

/*
 * Get a dumpi datatype index corresponding to the given MPI_Datatype.
 */
dumpi_datatype libdumpi_get_datatype(MPI_Datatype dtype) {
  libdumpi_init_datatype_map();
  return dumpi_hm_datatype_get(datatype_map, dtype);
}

/**
 * Create the bindings for a shared hash map for MPI_Errhandler.
 */
DUMPI_DECLARE_HASHMAP(errhandler, MPI_Errhandler, dumpi_errhandler)

/**
 * Create a handle for the actual shared MPI_Errhandler hash map.
 */
static dumpi_hm_errhandler *errhandler_map = NULL;

/**
 * Add built-in errhandlers to the MPI_Errhandler map.
 */
void libdumpi_init_errhandler_map(void) {
  if(errhandler_map == NULL) {
    dumpi_hm_errhandler_init(&errhandler_map, DUMPI_FIRST_USER_ERRHANDLER);
    dumpi_hm_errhandler_set(errhandler_map, MPI_ERRHANDLER_NULL,
                            DUMPI_ERRHANDLER_NULL);
  }
}

/*
 * Get a dumpi errhandler index corresponding to the given MPI_Errhandler.
 */
dumpi_datatype libdumpi_get_errhandler(MPI_Errhandler ehandler) {
  libdumpi_init_errhandler_map();
  return dumpi_hm_errhandler_get(errhandler_map, ehandler);
}

/**
 * Create the bindings for a shared hash map for MPI_File objects.
 */
DUMPI_DECLARE_HASHMAP(file, MPI_File, dumpi_file)

/**
 * Create a handle for the shared MPI_File hash map.
 */
static dumpi_hm_file *file_map = NULL;

/**
 * Add built-in file handles to the MPI_File hash map.
 */ 
void libdumpi_init_file_map(void) {
  if(file_map == NULL) {
    dumpi_hm_file_init(&file_map, DUMPI_FIRST_USER_FILE);
#ifdef DUMPI_HAVE_MPI_FILE_NULL
    dumpi_hm_file_set(file_map, MPI_FILE_NULL, DUMPI_FILE_NULL);
#endif
  }
}

/*
 * Get a dumpi file index corresponding to the given MPI_File.
 */
dumpi_datatype libdumpi_get_file(MPI_File mfile) {
  libdumpi_init_file_map();
  return dumpi_hm_file_get(file_map, mfile);
}

/**
 * Create the bindings for a shared hash map of MPI_Group objects.
 */
DUMPI_DECLARE_HASHMAP(group, MPI_Group, dumpi_group)

/**
 * Create a handle for the shared MPI_Group hash map.
 */
static dumpi_hm_group *group_map = NULL;

/**
 * Add built-in MPI_Group objects to the hash map.
 */
void libdumpi_init_group_map(void) {
  if(group_map == NULL) {
    dumpi_hm_group_init(&group_map, DUMPI_FIRST_USER_GROUP);
    dumpi_hm_group_set(group_map, MPI_GROUP_NULL, DUMPI_GROUP_NULL);
    dumpi_hm_group_set(group_map, MPI_GROUP_EMPTY, DUMPI_GROUP_EMPTY);
  }
}

/*
 * Get a dumpi group index corresponding to the given MPI_Group.
 */
dumpi_datatype libdumpi_get_group(MPI_Group mgroup) {
  libdumpi_init_group_map();
  return dumpi_hm_group_get(group_map, mgroup);
}

/**
 * Create the bindings for a shared hash map of MPI_Info objects.
 */
DUMPI_DECLARE_HASHMAP(info, MPI_Info, dumpi_info)

/**
 * Create a handle for the shared MPI_Info hash map.
 */
static dumpi_hm_info *info_map = NULL;

/**
 * Add built-in MPI_Info objects to the hash map.
 */
void libdumpi_init_info_map(void) {
  if(info_map == NULL) {
    dumpi_hm_info_init(&info_map, DUMPI_FIRST_USER_INFO);
#ifdef DUMPI_HAVE_MPI_INFO_NULL
    dumpi_hm_info_set(info_map, MPI_INFO_NULL, DUMPI_INFO_NULL);
#endif
  }
}

/*
 * Get a dumpi info index corresponding to the given MPI_Info.
 */
dumpi_datatype libdumpi_get_info(MPI_Info minfo) {
  libdumpi_init_info_map();
  return dumpi_hm_info_get(info_map, minfo);
}

/**
 * Create the bindings for a shared hash map of MPI_Op objects.
 */
DUMPI_DECLARE_HASHMAP(op, MPI_Op, dumpi_op)

/**
 * Create a handle for the shared MPI_Op hash map.
 */
static dumpi_hm_op *op_map = NULL;

/**
 * Add built in MPI operations to the hash map.
 */
void libdumpi_init_op_map(void) {
  if(op_map == NULL) {
    dumpi_hm_op_init(&op_map, DUMPI_FIRST_USER_OP);
    dumpi_hm_op_set(op_map, MPI_OP_NULL, DUMPI_OP_NULL);
#ifdef DUMPI_HAVE_MPI_MAX
    dumpi_hm_op_set(op_map, MPI_MAX, DUMPI_MAX);
#endif
#ifdef DUMPI_HAVE_MPI_MIN
    dumpi_hm_op_set(op_map, MPI_MIN, DUMPI_MIN);
#endif
#ifdef DUMPI_HAVE_MPI_SUM
    dumpi_hm_op_set(op_map, MPI_SUM, DUMPI_SUM);
#endif
#ifdef DUMPI_HAVE_MPI_PROD
    dumpi_hm_op_set(op_map, MPI_PROD, DUMPI_PROD);
#endif
#ifdef DUMPI_HAVE_MPI_LAND
    dumpi_hm_op_set(op_map, MPI_LAND, DUMPI_LAND);
#endif
#ifdef DUMPI_HAVE_MPI_BAND
    dumpi_hm_op_set(op_map, MPI_BAND, DUMPI_BAND);
#endif
#ifdef DUMPI_HAVE_MPI_LOR
    dumpi_hm_op_set(op_map, MPI_LOR, DUMPI_LOR);
#endif
#ifdef DUMPI_HAVE_MPI_BOR
    dumpi_hm_op_set(op_map, MPI_BOR, DUMPI_BOR);
#endif
#ifdef DUMPI_HAVE_MPI_LXOR
    dumpi_hm_op_set(op_map, MPI_LXOR, DUMPI_LXOR);
#endif
#ifdef DUMPI_HAVE_MPI_BXOR
    dumpi_hm_op_set(op_map, MPI_BXOR, DUMPI_BXOR);
#endif
#ifdef DUMPI_HAVE_MPI_MINLOC
    dumpi_hm_op_set(op_map, MPI_MINLOC, DUMPI_MINLOC);
#endif
#ifdef DUMPI_HAVE_MPI_MAXLOC
    dumpi_hm_op_set(op_map, MPI_MAXLOC, DUMPI_MAXLOC);
#endif
#ifdef DUMPI_HAVE_MPI_REPLACE
    dumpi_hm_op_set(op_map, MPI_REPLACE, DUMPI_REPLACE);
#endif
  }
}

/*
 * Get a dumpi op index corresponding to the given MPI_Op.
 */
dumpi_datatype libdumpi_get_op(MPI_Op mop) {
  libdumpi_init_op_map();
  return dumpi_hm_op_get(op_map, mop);
}

/**
 * Create the bindings for a hash map of MPIO_Request objects.
 * In many implementations this is the same type as MPI_Request,
 * but we keep them separate here.
 */
DUMPI_DECLARE_HASHMAP(iorequest, MPIO_Request, dumpio_request)

/**
 * Create the handle for a shared hash map of MPIO_Requests.
 */
static dumpi_hm_iorequest *iorequest_map = NULL;

/**
 * Add built-in MPIO_Request objects to the hash map.
 */
void libdumpi_init_iorequest_map(void) {
  if(iorequest_map == NULL) {
    dumpi_hm_iorequest_init(&iorequest_map, DUMPIO_FIRST_USER_REQUEST);
#ifdef DUMPI_HAVE_MPIO_REQUEST_NULL
    dumpi_hm_iorequest_set(iorequest_map, MPIO_REQUEST_NULL,
                           DUMPIO_REQUEST_NULL);
#endif /* ! DUMPI_HAVE_MPIO_REQUEST_NULL */
  }
}

/*
 * Get a dumpi request index corresponding to the given MPIO_Request.
 */
dumpi_datatype libdumpi_get_iorequest(MPIO_Request mrequest) {
  libdumpi_init_iorequest_map();
  return dumpi_hm_iorequest_get(iorequest_map, mrequest);
}

/**
 * Create the bindings for a hash map of MPI_Request objects.
 */
DUMPI_DECLARE_HASHMAP(request, MPI_Request, dumpi_request)

/**
 * Create the handle for a shared map of MPI_Request objects.
 */
static dumpi_hm_request *request_map = NULL;

/**
 * Add built-in MPI_Request objects to the hash map.
 */
void libdumpi_init_request_map(void) {
  if(request_map == NULL) {
    dumpi_hm_request_init(&request_map, DUMPI_FIRST_USER_REQUEST);
#ifdef DUMPI_HAVE_MPI_REQUEST_NULL
    dumpi_hm_request_set(request_map, MPI_REQUEST_NULL, DUMPI_REQUEST_NULL);
#endif
  }
}

/*
 * Get a dumpi request index corresponding to the given MPI_Request.
 */
dumpi_datatype libdumpi_get_request(MPI_Request mrequest) {
  libdumpi_init_request_map();
  return dumpi_hm_request_get(request_map, mrequest);
}

/** 
 * Create the bindings for a hash map of MPI keyval handles.
 * The MPI standard defines those as int, but they are a
 * dedicated typedef in dumpi.
 */
DUMPI_DECLARE_HASHMAP(keyval, int, dumpi_keyval)

/**
 * Create a handle for the shared MPI keyval handles.
 */
static dumpi_hm_keyval *keyval_map = NULL;

/**
 * Add built-in (predefined) keyval handles to the hash map.
 */
void libdumpi_init_keyval_map(void) {
  if(keyval_map == NULL) {
    dumpi_hm_keyval_init(&keyval_map, DUMPI_FIRST_USER_KEYVAL);
#ifdef DUMPI_HAVE_MPI_TAG_UB
    dumpi_hm_keyval_set(keyval_map, MPI_TAG_UB, DUMPI_TAG_UB);
#endif
#ifdef DUMPI_HAVE_MPI_HOST
    dumpi_hm_keyval_set(keyval_map, MPI_HOST, DUMPI_HOST);
#endif
#ifdef DUMPI_HAVE_MPI_IO
    dumpi_hm_keyval_set(keyval_map, MPI_IO, DUMPI_IO);
#endif
#ifdef DUMPI_HAVE_MPI_WTIME_IS_GLOBAL
    dumpi_hm_keyval_set(keyval_map, MPI_WTIME_IS_GLOBAL,
                        DUMPI_WTIME_IS_GLOBAL);
#endif
#ifdef DUMPI_HAVE_MPI_UNIVERSE_SIZE
    dumpi_hm_keyval_set(keyval_map, MPI_UNIVERSE_SIZE, DUMPI_UNIVERSE_SIZE);
#endif
#ifdef DUMPI_HAVE_MPI_LASTUSEDCODE
    dumpi_hm_keyval_set(keyval_map, MPI_LASTUSEDCODE, DUMPI_LASTUSEDCODE);
#endif
#ifdef DUMPI_HAVE_MPI_APPNUM
    dumpi_hm_keyval_set(keyval_map, MPI_APPNUM, DUMPI_APPNUM);
#endif
#ifdef DUMPI_HAVE_MPI_WIN_BASE
    dumpi_hm_keyval_set(keyval_map, MPI_WIN_BASE, DUMPI_WIN_BASE);
#endif
#ifdef DUMPI_HAVE_MPI_WIN_SIZE
    dumpi_hm_keyval_set(keyval_map, MPI_WIN_SIZE, DUMPI_WIN_SIZE);
#endif
#ifdef DUMPI_HAVE_MPI_WIN_DISP_UNIT
    dumpi_hm_keyval_set(keyval_map, MPI_WIN_DISP_UNIT, DUMPI_WIN_DISP_UNIT);
#endif
  }
}

/*
 * Get a dumpi keyval index corresponding to the given MPI key (integer).
 */
dumpi_datatype libdumpi_get_keyval(int mkeyval) {
  libdumpi_init_keyval_map();
  return dumpi_hm_keyval_get(keyval_map, mkeyval);
}

#ifdef DUMPI_HAVE_MPI_WIN
/**
 * Create the bindings for a hash map of MPI_Win objects.
 */
DUMPI_DECLARE_HASHMAP(win, MPI_Win, dumpi_win)

/**
 * Create a handle to the shared MPI_Win hash map.
 */
static dumpi_hm_win *win_map = NULL;

/**
 * Add predefined MPI_Win objects to the hash map.
 */
void libdumpi_init_win_map(void) {
  if(win_map == NULL) {
    dumpi_hm_win_init(&win_map, DUMPI_FIRST_USER_WIN);
#ifdef DUMPI_HAVE_MPI_WIN_NULL
    dumpi_hm_win_set(win_map, MPI_WIN_NULL, DUMPI_WIN_NULL);
#endif
  }
}

/*
 * Get a dumpi win index corresponding to the given MPI_Win.
 */
dumpi_datatype libdumpi_get_win(MPI_Win mwin) {
  libdumpi_init_win_map();
  return dumpi_hm_win_get(win_map, mwin);
}

#endif /* ! DUMPI_HAVE_MPI_WIN */

/**
 * Initialize all hash maps.
 */
void libdumpi_init_maps(void) {
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI] libdumpi_init_maps entering\n");
  libdumpi_init_comm_map();
  libdumpi_init_datatype_map();
  libdumpi_init_errhandler_map();
  libdumpi_init_file_map();
  libdumpi_init_group_map();
  libdumpi_init_info_map();
  libdumpi_init_op_map();
  libdumpi_init_iorequest_map();
  libdumpi_init_request_map();
  libdumpi_init_keyval_map();
#ifdef DUMPI_HAVE_MPI_WIN
  libdumpi_init_win_map();
#endif
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI] libdumpi_init_maps returning\n");
}

/**
 * Free dynamically allocated memory from the hash maps.
 */
void dumpi_free_maps(void) {
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI] dumpi_free_maps entering\n");
  dumpi_hm_comm_free(&comm_map);
  dumpi_hm_datatype_free(&datatype_map);
  dumpi_hm_errhandler_free(&errhandler_map);
  dumpi_hm_file_free(&file_map);
  dumpi_hm_group_free(&group_map);
  dumpi_hm_info_free(&info_map);
  dumpi_hm_op_free(&op_map);
  dumpi_hm_iorequest_free(&iorequest_map);
  dumpi_hm_request_free(&request_map);
  dumpi_hm_keyval_free(&keyval_map);
#ifdef DUMPI_HAVE_MPI_WIN
  dumpi_hm_win_free(&win_map);
#endif
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI] dumpi_free_maps returning\n");
}