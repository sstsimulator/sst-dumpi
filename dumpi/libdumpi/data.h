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

#ifndef DUMPI_LIBDUMPI_DATA_H
#define DUMPI_LIBDUMPI_DATA_H

#include <mpi.h>
#include <dumpi/common/types.h>
#include <dumpi/common/io.h>
#include <dumpi/common/constants.h>
#include <dumpi/dumpiconfig.h>

#ifdef DUMPI_USE_PTHREADS
#include <pthread.h>
#endif /* ! DUMPI_USE_PTHREADS */

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup libdumpi_internal
   * \defgroup libdumpi_internal_data libdumpi/internal/data \
   *     Shared data containers for libdumpi.  Purely private parts.
   */
  /*@{*/

  /**
   * Mapping of built-in MPI_Datatype values to dumpi_native_datatype labels.
   * Indexed using enum values of dumpi_native_datatype up to (not including)
   * DUMPI_FIRST_USER_DATATYPE.
   */
  extern const MPI_Datatype dumpi_mpi_datatype_match[DUMPI_FIRST_USER_DATATYPE];

  /**
   * Mapping of built-in MPI_Op values to dumpi_native_ops labels.
   * Indexed using enum values of dumpi_native_ops up to (but not including)
   * DUMPI_FIRST_USER_OP.
   */
  extern const MPI_Op dumpi_mpi_op_match[DUMPI_FIRST_USER_OP];
  
  /**
   * Collection of shared structures
   */
  typedef struct {
#ifdef DUMPI_USE_PTHREADS
    /* Mutex added to ensure consistency in multithreaded MPI programs */
    pthread_mutex_t      mutex;
#endif /* ! DUMPI_USE_PTHREADS */
    /* This is shared within libdumpi (sadly) */
    dumpi_header        *header;
    dumpi_profile       *profile;
    dumpi_footer        *footer;
    dumpi_perfinfo      *perf;
    dumpi_keyval_record *keyval;
    dumpi_outputs       *output;
    dumpi_sizeof         typesize;
    /* This is here to allow overriding of default output file root. */
    const char          *file_root;		/*preamble*/
    const char          *output_file; /*fully qualified name*/
    int                  comm_rank;   /* MPI communicator rank. */
    int                  comm_size;   /* MPI communicator size. */
  } dumpi_global_t;

  /**
   * Maintain state about the dumpi run.
   * dumpi_global is an unfortunate name for this type, since it actually
   * contains information about the local state (the only global info is
   * the timestamp at which rank 0 was initialized).
   */
  extern dumpi_global_t *dumpi_global;

  /*@}*/
	
#ifdef __cplusplus
} /* end of extern "C" */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBDUMPI_DATA_H */