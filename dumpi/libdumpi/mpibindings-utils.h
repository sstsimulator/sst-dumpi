/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009-2011 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
 */

#ifndef DUMPI_LIBDUMPI_MPIBINDINGS_UTILS_H
#define DUMPI_LIBDUMPI_MPIBINDINGS_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * A global lock to protect access to dumpiio routines.
   */
  int libdumpi_lock_io(void);

  /**
   * A global lock for io routines.
   */
  int libdumpi_unlock_io(void);

  /**
   * Get a unique thread index for this thread.
   */
  int libdumpi_get_thread_id(void);

  /** 
   * Increase call depth counter for this thread.
   * If threads are not enabled, this will use a simple static counter.
   * A call count of 1 indicates that this MPI function was called directly
   * from user code.
   * \return updated call count (always > 0).
   */
  int libdumpi_enter_mpi(void);

  /**
   * Decrease call depth counter for this thread.
   * A call count of 0 indicates that the thread is completely outside MPI.
   */
  int libdumpi_exit_mpi(void);

  /**
   * Read current call depth.
   */
  int libdumpi_get_call_depth(void);

#ifdef __cplusplus
} /* end of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBDUMPI_MPIBINDINGS_UTILS_H */
