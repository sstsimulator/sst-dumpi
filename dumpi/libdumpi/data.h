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
