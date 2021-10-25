/**
Copyright 2009-2021 National Technology and Engineering Solutions of Sandia, 
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S.  Government 
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly 
owned subsidiary of Honeywell International, Inc., for the U.S. Department of 
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2021, NTESS

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of the copyright holder nor the names of its
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

#include <dumpi/libdumpi/data.h>
#include <dumpi/dumpiconfig.h>

dumpi_global_t *dumpi_global = NULL;

#define DUMPI_TEST_DATATYPE(LABEL)

const MPI_Datatype dumpi_mpi_datatype_match[DUMPI_FIRST_USER_DATATYPE] = {
  MPI_DATATYPE_NULL,
  MPI_DATATYPE_NULL,
  DUMPI_DATATYPE_MPI_CHAR,
  DUMPI_DATATYPE_MPI_SIGNED_CHAR,
  DUMPI_DATATYPE_MPI_UNSIGNED_CHAR,
  DUMPI_DATATYPE_MPI_BYTE,
  DUMPI_DATATYPE_MPI_WCHAR,
  DUMPI_DATATYPE_MPI_SHORT,
  DUMPI_DATATYPE_MPI_UNSIGNED_SHORT,
  DUMPI_DATATYPE_MPI_INT,
  DUMPI_DATATYPE_MPI_UNSIGNED,
  DUMPI_DATATYPE_MPI_LONG,
  DUMPI_DATATYPE_MPI_UNSIGNED_LONG,
  DUMPI_DATATYPE_MPI_FLOAT,
  DUMPI_DATATYPE_MPI_DOUBLE,
  DUMPI_DATATYPE_MPI_LONG_DOUBLE,
  DUMPI_DATATYPE_MPI_LONG_LONG_INT,
  DUMPI_DATATYPE_MPI_UNSIGNED_LONG_LONG,
  DUMPI_DATATYPE_MPI_LONG_LONG,
  DUMPI_DATATYPE_MPI_PACKED,
  DUMPI_DATATYPE_MPI_FLOAT_INT,
  DUMPI_DATATYPE_MPI_DOUBLE_INT,
  DUMPI_DATATYPE_MPI_LONG_INT,
  DUMPI_DATATYPE_MPI_SHORT_INT,
  DUMPI_DATATYPE_MPI_2INT,
  DUMPI_DATATYPE_MPI_LONG_DOUBLE_INT
};

const MPI_Op dumpi_mpi_op_match[DUMPI_FIRST_USER_OP] = {
  MPI_OP_NULL,
  DUMPI_OP_MPI_MAX,
  DUMPI_OP_MPI_MIN,
  DUMPI_OP_MPI_SUM,
  DUMPI_OP_MPI_PROD,
  DUMPI_OP_MPI_LAND,
  DUMPI_OP_MPI_BAND,
  DUMPI_OP_MPI_LOR,
  DUMPI_OP_MPI_BOR,
  DUMPI_OP_MPI_LXOR,
  DUMPI_OP_MPI_BXOR,
  DUMPI_OP_MPI_MINLOC,
  DUMPI_OP_MPI_MAXLOC,
  DUMPI_OP_MPI_REPLACE,
  MPI_OP_NULL
};
