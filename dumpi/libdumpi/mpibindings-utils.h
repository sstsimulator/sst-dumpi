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