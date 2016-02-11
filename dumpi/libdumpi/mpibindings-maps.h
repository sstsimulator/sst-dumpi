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

#ifndef DUMPI_LIBDUMPI_MPIBINDINGS_MAPS_H
#define DUMPI_LIBDUMPI_MPIBINDINGS_MAPS_H

#include <dumpi/common/hashmap.h>
#include <dumpi/common/types.h>
#include <dumpi/libdumpi/data.h>
#include <dumpi/common/constants.h>
#include <dumpi/common/debugflags.h>
#include <dumpi/dumpiconfig.h>
#include <mpi.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup libdumpi_internal
   * \defgroup libdumpi_internal_maps libdumpi/internal/maps \
   *    Hash maps for storing compact info about objects with built-in MPI types
   */
  /*@{*/ /* define scope for doxygen comments */

  /**
   * Initialize all hash maps.
   */
  void libdumpi_init_maps(void);

  /**
   * Free dynamically allocated memory from the hash maps.
   */
  void libdumpi_free_maps(void);

  /**
   * Get a dumpi communicator index corresponding to the given MPI_Comm.
   */
  dumpi_comm libdumpi_get_comm(MPI_Comm comm);

  /**
   * Get a dumpi datatype index corresponding to the given MPI_Datatype.
   */
  dumpi_datatype libdumpi_get_datatype(MPI_Datatype dtype);

  /**
   * Get a dumpi errhandler index corresponding to the given MPI_Errhandler
   */
  dumpi_datatype libdumpi_get_errhandler(MPI_Errhandler ehandler);

  /**
   * Get a dumpi file index corresponding to the given MPI_File.
   */
  dumpi_datatype libdumpi_get_file(MPI_File mfile);

  /**
   * Get a dumpi group index corresponding to the given MPI_Group.
   */
  dumpi_datatype libdumpi_get_group(MPI_Group mgroup);

  /**
   * Get a dumpi info index corresponding to the given MPI_Info.
   */
  dumpi_datatype libdumpi_get_info(MPI_Info minfo);

  /**
   * Get a dumpi op index corresponding to the given MPI_Op.
   */
  dumpi_datatype libdumpi_get_op(MPI_Op mop);

  /*
   * Get a dumpi request index corresponding to the given MPIO_Request.
   */
  dumpi_datatype libdumpi_get_iorequest(MPIO_Request mrequest);

  /**
   * Get a dumpi request index corresponding to the given MPI_Request.
   */
  dumpi_datatype libdumpi_get_request(MPI_Request mrequest);
  
  /**
   * Get a dumpi keyval index corresponding to the given MPI key (integer).
   */
  dumpi_datatype libdumpi_get_keyval(int mkeyval);

#if DUMPI_HAVE_MPI_WIN

  /**
   * Get a dumpi win index corresponding to the given MPI_Win.
   */
  dumpi_datatype libdumpi_get_win(MPI_Win mwin);

#endif /* ! DUMPI_HAVE_MPI_WIN */

  /*@}*/ /* close scope of doxygen comments */

#ifdef __cplusplus
} /* End of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBDUMPI_MPIBINDINGS_MAPS_H */
