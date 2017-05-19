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

#include <dumpi/common/constants.h>

/* Utility definition to stringify enum values inside a case statement. */
#define DUMPI_CASE_RETURN(TOKEN) case DU##TOKEN: return #TOKEN; break

/* Get the string name of a topology. */
const char* dumpi_get_topology_name(dumpi_topology topo) {
  switch(topo) {
  case DUMPI_GRAPH:     return "MPI_GRAPH";     break;
  case DUMPI_CART:      return "MPI_CART";      break;
  case DUMPI_UNDEFINED: return "MPI_UNDEFINED"; break;
  default: return "DUMPI_TOPOLOGY_ERROR";
  }
}

/* Get the string name of a combiner. */
const char* dumpi_get_combiner_name(dumpi_combiner comb) {
  switch(comb) {
    DUMPI_CASE_RETURN(MPI_COMBINER_ERROR);
    DUMPI_CASE_RETURN(MPI_COMBINER_NAMED);
    DUMPI_CASE_RETURN(MPI_COMBINER_DUP);
    DUMPI_CASE_RETURN(MPI_COMBINER_CONTIGUOUS);
    DUMPI_CASE_RETURN(MPI_COMBINER_VECTOR);
    DUMPI_CASE_RETURN(MPI_COMBINER_HVECTOR_INTEGER);
    DUMPI_CASE_RETURN(MPI_COMBINER_HVECTOR);
    DUMPI_CASE_RETURN(MPI_COMBINER_INDEXED);
    DUMPI_CASE_RETURN(MPI_COMBINER_HINDEXED_INTEGER);
    DUMPI_CASE_RETURN(MPI_COMBINER_HINDEXED);
    DUMPI_CASE_RETURN(MPI_COMBINER_INDEXED_BLOCK);
    DUMPI_CASE_RETURN(MPI_COMBINER_STRUCT_INTEGER);
    DUMPI_CASE_RETURN(MPI_COMBINER_STRUCT);
    DUMPI_CASE_RETURN(MPI_COMBINER_SUBARRAY);
    DUMPI_CASE_RETURN(MPI_COMBINER_DARRAY);
    DUMPI_CASE_RETURN(MPI_COMBINER_F90_REAL);
    DUMPI_CASE_RETURN(MPI_COMBINER_F90_COMPLEX);
    DUMPI_CASE_RETURN(MPI_COMBINER_F90_INTEGER);
    DUMPI_CASE_RETURN(MPI_COMBINER_RESIZED);
    default: return "DUMPI_COMBINER_ERROR";
  }
}

/* Get the string name of an MPI datatype.
 * Returns "user-defined-datatype" if the value is >= DUMPI_FIRST_USER_DATATYPE */
const char* dumpi_get_datatype_name(dumpi_datatype dt) {
  switch (dt) {
    DUMPI_CASE_RETURN(MPI_DATATYPE_ERROR);
    DUMPI_CASE_RETURN(MPI_DATATYPE_NULL);
    DUMPI_CASE_RETURN(MPI_CHAR);
    DUMPI_CASE_RETURN(MPI_SIGNED_CHAR);
    DUMPI_CASE_RETURN(MPI_UNSIGNED_CHAR);
    DUMPI_CASE_RETURN(MPI_BYTE);
    DUMPI_CASE_RETURN(MPI_WCHAR);
    DUMPI_CASE_RETURN(MPI_SHORT);
    DUMPI_CASE_RETURN(MPI_UNSIGNED_SHORT);
    DUMPI_CASE_RETURN(MPI_INT);
    DUMPI_CASE_RETURN(MPI_UNSIGNED);
    DUMPI_CASE_RETURN(MPI_LONG);
    DUMPI_CASE_RETURN(MPI_UNSIGNED_LONG);
    DUMPI_CASE_RETURN(MPI_FLOAT);
    DUMPI_CASE_RETURN(MPI_DOUBLE);
    DUMPI_CASE_RETURN(MPI_LONG_DOUBLE);
    DUMPI_CASE_RETURN(MPI_LONG_LONG_INT);
    DUMPI_CASE_RETURN(MPI_UNSIGNED_LONG_LONG);
    DUMPI_CASE_RETURN(MPI_LONG_LONG);
    DUMPI_CASE_RETURN(MPI_PACKED);
    DUMPI_CASE_RETURN(MPI_LB);
    DUMPI_CASE_RETURN(MPI_UB);
    DUMPI_CASE_RETURN(MPI_FLOAT_INT);
    DUMPI_CASE_RETURN(MPI_DOUBLE_INT);
    DUMPI_CASE_RETURN(MPI_LONG_INT);
    DUMPI_CASE_RETURN(MPI_SHORT_INT);
    DUMPI_CASE_RETURN(MPI_2INT);
    DUMPI_CASE_RETURN(MPI_LONG_DOUBLE_INT);
    default: return "user-defined-datatype";
  }
}

/* "Guess" what a file name is */
const char* dumpi_get_file_name(dumpi_file fl) {
  if(fl != DUMPI_FILE_NULL) return "user-file";
  else return "DUMPI_FILE_NULL";
}

/* Get string representation of info name. */
const char* dumpi_get_info_name(dumpi_info inf) {
  if(inf >= DUMPI_FIRST_USER_INFO)
    return "user-info";
  else
    return "MPI_INFO_NULL";
}

/* Get the string name for a keyval. */
const char* dumpi_get_keyval_name(dumpi_keyval kv) {
  switch(kv) {
    DUMPI_CASE_RETURN(MPI_TAG_UB);
    DUMPI_CASE_RETURN(MPI_HOST);
    DUMPI_CASE_RETURN(MPI_IO);
    DUMPI_CASE_RETURN(MPI_WTIME_IS_GLOBAL);
    DUMPI_CASE_RETURN(MPI_UNIVERSE_SIZE);
    DUMPI_CASE_RETURN(MPI_LASTUSEDCODE);
    DUMPI_CASE_RETURN(MPI_APPNUM);
    DUMPI_CASE_RETURN(MPI_WIN_BASE);
    DUMPI_CASE_RETURN(MPI_WIN_SIZE);
    DUMPI_CASE_RETURN(MPI_WIN_DISP_UNIT);
    default: return "user-defined-keyval"; break;
  }
}

/* Get the string name of an MPI operation.
 * Returns "user-defined" if the value is >= DUMPI_FIRST_USER_DATATYPE */
const char* dumpi_get_op_name(dumpi_datatype op) {
  switch(op) {
    DUMPI_CASE_RETURN(MPI_OP_ERROR);
    DUMPI_CASE_RETURN(MPI_MAX);
    DUMPI_CASE_RETURN(MPI_MIN);
    DUMPI_CASE_RETURN(MPI_SUM);
    DUMPI_CASE_RETURN(MPI_PROD);
    DUMPI_CASE_RETURN(MPI_LAND);
    DUMPI_CASE_RETURN(MPI_BAND);
    DUMPI_CASE_RETURN(MPI_LOR);
    DUMPI_CASE_RETURN(MPI_BOR);
    DUMPI_CASE_RETURN(MPI_LXOR);
    DUMPI_CASE_RETURN(MPI_BXOR);
    DUMPI_CASE_RETURN(MPI_MINLOC);
    DUMPI_CASE_RETURN(MPI_MAXLOC);
    DUMPI_CASE_RETURN(MPI_REPLACE);
    DUMPI_CASE_RETURN(MPI_OP_NULL);
    default: return "user-defined-op";
  }
}

/* Get the string name of an MPI operation.
 * Returns "undefined-comparison" if the value is not defined */
const char* dumpi_get_comparison_name(dumpi_comparison cmp) {
  switch(cmp) {
    DUMPI_CASE_RETURN(MPI_COMPARISON_ERROR);
    DUMPI_CASE_RETURN(MPI_IDENT);
    DUMPI_CASE_RETURN(MPI_CONGRUENT);
    DUMPI_CASE_RETURN(MPI_SIMILAR);
    DUMPI_CASE_RETURN(MPI_UNEQUAL);
    default: return "undefined-comparison";
  }
}

/* Get the string name of an MPI group.
 * Returns "user-defined-group" if the value is >= DUMPI_FIRST_USER_GROUP. */
const char* dumpi_get_group_name(dumpi_group grp) {
  switch(grp) {
    DUMPI_CASE_RETURN(MPI_GROUP_ERROR);
    DUMPI_CASE_RETURN(MPI_GROUP_NULL);
    DUMPI_CASE_RETURN(MPI_GROUP_EMPTY);
    default: return "user-defined-group";
  }
}

/* Get the string name of an MPI communicator
 * Returns "user-defined-comm" if the value is >= DUMPI_FIRST_USER_COMM. */
const char* dumpi_get_comm_name(dumpi_comm comm) {
  switch(comm) {
    DUMPI_CASE_RETURN(MPI_COMM_ERROR);
    DUMPI_CASE_RETURN(MPI_COMM_NULL);
    DUMPI_CASE_RETURN(MPI_COMM_WORLD);
    DUMPI_CASE_RETURN(MPI_COMM_SELF);
    default: return "user-defined-comm";
  }
}

/* Get the string name of an MPI communicator
 * Returns "user-defined-request" if >= DUMPI_FIRST_USER_REQUEST. */
const char* dumpi_get_request_name(dumpi_request req) {
  switch(req) {
    DUMPI_CASE_RETURN(MPI_REQUEST_ERROR);
    DUMPI_CASE_RETURN(MPI_REQUEST_NULL);
    default: return "user-defined-request";
  }
}

/* Get string name for errhandlers. */
const char* dumpi_get_errhandler_name(dumpi_errhandler eh) {
  if(eh >= DUMPI_FIRST_USER_ERRHANDLER)
    return "user-defined-errhandler";
  else if(eh == DUMPI_ERRHANDLER_NULL) 
    return "MPI_ERRHANDLER_NULL";
  else
    return "DUMPI_ERRHANDLER_ERROR";
}

/* Get a string representation of a file mode. */
const char* dumpi_get_filemode_name(dumpi_filemode fm) {
  switch(fm) {
    DUMPI_CASE_RETURN(MPI_MODE_RDONLY);
    DUMPI_CASE_RETURN(MPI_MODE_RDWR);
    DUMPI_CASE_RETURN(MPI_MODE_WRONLY);
    DUMPI_CASE_RETURN(MPI_MODE_CREATE);
    DUMPI_CASE_RETURN(MPI_MODE_EXCL);
    DUMPI_CASE_RETURN(MPI_MODE_DELETE_ON_CLOSE);
    DUMPI_CASE_RETURN(MPI_MODE_UNIQUE_OPEN);
    DUMPI_CASE_RETURN(MPI_MODE_SEQUENTIAL);
    DUMPI_CASE_RETURN(MPI_MODE_APPEND);
    default: return "DUMPI_FILEMODE_ERROR";
  }
}

/* Get a string representation of a lock type */
const char* dumpi_get_locktype_name(dumpi_locktype lt) {
  switch(lt) {
    DUMPI_CASE_RETURN(MPI_LOCK_SHARED);
    DUMPI_CASE_RETURN(MPI_LOCK_EXCLUSIVE);
    default: return "DUMPI_LOCKTYPE_ERROR";
  }
}

/* Get a string representation of array ordering */
const char* dumpi_get_ordering_name(dumpi_ordering dr) {
  switch(dr) {
    DUMPI_CASE_RETURN(MPI_ORDER_C);
    DUMPI_CASE_RETURN(MPI_ORDER_FORTRAN);
    default: return "DUMPI_ORDERING_ERROR";
  }
}

/* Get a string representation of thread support level. */
const char* dumpi_get_threadlevel_name(dumpi_threadlevel tn) {
  switch(tn) {
    DUMPI_CASE_RETURN(MPI_THREAD_SINGLE);
    DUMPI_CASE_RETURN(MPI_THREAD_FUNNELED);
    DUMPI_CASE_RETURN(MPI_THREAD_SERIALIZED);
    DUMPI_CASE_RETURN(MPI_THREAD_MULTIPLE);
    default: return "DUMPI_THREADLEVEL_ERROR";
  }
}

/* Get a string representation of typeclass */
const char* dumpi_get_typeclass_name(dumpi_typeclass tc) {
  switch(tc) {
    DUMPI_CASE_RETURN(MPI_TYPECLASS_REAL);
    DUMPI_CASE_RETURN(MPI_TYPECLASS_INTEGER);
    DUMPI_CASE_RETURN(MPI_TYPECLASS_COMPLEX);
    default: return "DUMPI_TYPECLASS_ERROR";
  }
}

/* Get a string representation for file windows. */
const char* dumpi_get_win_name(dumpi_win win) {
  if(win != DUMPI_WIN_NULL) return "user-defined-win";
  else return "DUMPI_WIN_NULL";
}

/* Get a string representation of file positioning. */
const char* dumpi_get_whence_name(dumpi_whence whence) {
  switch(whence) {
  DUMPI_CASE_RETURN(MPI_SEEK_SET);
  DUMPI_CASE_RETURN(MPI_SEEK_CUR);
  DUMPI_CASE_RETURN(MPI_SEEK_END);
  default: return "DUMPI_WHENCE_ERROR";
  }
}

/* All possible combinations of assert flags. */
static const char *assertset[] = {
  "DUMPI_MODE_NONE",
  "MPI_MODE_NOCHECK",
  "MPI_MODE_NOSTORE",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSTORE",
  "MPI_MODE_NOPUT",
  "MPI_MODE_NOCHECK|MPI_MODE_NOPUT",
  "MPI_MODE_NOSTORE|MPI_MODE_NOPUT",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSTORE|MPI_MODE_NOPUT",
  "MPI_MODE_NOPRECEDE",
  "MPI_MODE_NOCHECK|MPI_MODE_NOPRECEDE",
  "MPI_MODE_NOSTORE|MPI_MODE_NOPRECEDE",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSTORE|MPI_MODE_NOPRECEDE",
  "MPI_MODE_NOPUT|MPI_MODE_NOPRECEDE",
  "MPI_MODE_NOCHECK|MPI_MODE_NOPUT|MPI_MODE_NOPRECEDE",
  "MPI_MODE_NOSTORE|MPI_MODE_NOPUT|MPI_MODE_NOPRECEDE",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSTORE|MPI_MODE_NOPUT|MPI_MODE_NOPRECEDE",
  "MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOSTORE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSTORE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOPUT|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOCHECK|MPI_MODE_NOPUT|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOSTORE|MPI_MODE_NOPUT|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSTORE|MPI_MODE_NOPUT|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOPRECEDE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOCHECK|MPI_MODE_NOPRECEDE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOSTORE|MPI_MODE_NOPRECEDE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSTORE|MPI_MODE_NOPRECEDE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOPUT|MPI_MODE_NOPRECEDE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOCHECK|MPI_MODE_NOPUT|MPI_MODE_NOPRECEDE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOSTORE|MPI_MODE_NOPUT|MPI_MODE_NOPRECEDE|MPI_MODE_NOSUCCEED",
  "MPI_MODE_NOCHECK|MPI_MODE_NOSTORE|MPI_MODE_NOPUT|MPI_MODE_NOPRECEDE|MPI_MODE_NOSUCCEED"  
};

/* Get a string representation of an assertion. */
const char* dumpi_get_win_assert_name(dumpi_win_assert dwa) {
  dumpi_win_assert maxval = (DUMPI_MODE_NOCHECK|DUMPI_MODE_NOSTORE|
			     DUMPI_MODE_NOPUT|DUMPI_MODE_NOPRECEDE|
			     DUMPI_MODE_NOSUCCEED);
  if(dwa < 0 || dwa > maxval)
    return "DUMPI_MODE_ERROR";
  else
    return assertset[dwa];
}