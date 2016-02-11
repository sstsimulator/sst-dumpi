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

#ifndef DUMPI_LIBDUMPI_FUSED_BINDINGS_H
#define DUMPI_LIBDUMPI_FUSED_BINDINGS_H

#include <dumpi/dumpiconfig.h>
#include <mpi.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup libdumpi
   * \addtogroup libdumpi_internal
   */
  /*@{*/ /* define scope for subsequent comments */

  /**
   * Utility definition intended for systems where autoconf doesn't work
   * correctly (that would be RSQ).  Not really appropriate for other machines.
   */
#ifdef DUMPI_MPIO_USES_MPI_REQUESTS
#ifndef MPIO_USES_MPI_REQUEST
  #define DUMPI_SKIP_MPIO_TEST
  #define DUMPI_SKIP_MPIO_TESTANY
  #define DUMPI_SKIP_MPIO_TESTSOME
  #define DUMPI_SKIP_MPIO_TESTALL
  #define DUMPI_SKIP_MPIO_WAIT
  #define DUMPI_SKIP_MPIO_WAITSOME
  #define DUMPI_SKIP_MPIO_WAITANY
  #define DUMPI_SKIP_MPIO_WAITALL
#endif
#endif

  /*@}*/ /* close scope of doxygen comments */

#ifdef __cplusplus
} /* End of extern C block */
#endif /* ! __cplusplus */

#endif /* DUMPI_LIBDUMPI_FUSED_BINDINGS_H */
