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

#ifndef DUMPI_LIBDUMPI_INIT_H
#define DUMPI_LIBDUMPI_INIT_H

#include <dumpi/common/settings.h>
#include <dumpi/common/funcs.h>
#include <dumpi/libdumpi/data.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup libdumpi
   * \defgroup libdumpi_internal libdumpi/internal:  \
   *     Internal functionality for libdumpi (not stable API)
   */
  /*@{*/ /* Define scope for subsequent comments */

  /**
   * Initialize storage and read in configuration file.
   * 
   * Storage gets freed up automatically at exit.
   * This is a no-op if(dumpi_global != NULL).
   */
  void libdumpi_init(void);

  /**
   * Open output files (each node creates its onw dump file;
   * node 0 also creates a human-readable metafile to describe the simulation).
   *
   * Note that in a parallel run (if MPI is initialized and MPI size > 0),
   * node 0 broadcasts its start time (as stored in
   * dumpi_global::header::starttime) to be used for all filenames.
   *
   * This method updates at a large portion of dumpi_global (at least
   * dumpi_global::file_root, dumpi_global::output_file,
   * and dumpi_global::profile).
   */
  void libdumpi_open_files(void);

  /**
   * Finalize libdumpi and free all dynamically allocated storage.
   * This gets called automatically at exit (using atexit(3)), but
   * it is safe to call this method manually.
   *
   * If dumpi_global::profile is NULL, this method calls
   * libdumpi_open_files before flushing the output buffer.
   */
  void libdumpi_finalize(void);

  /*@}*/ /* close comment scope */

#ifdef __cplusplus
} /* end of extern "C" */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBDUMPI_INIT_H */
