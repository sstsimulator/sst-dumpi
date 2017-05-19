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