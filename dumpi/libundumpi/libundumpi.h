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

#ifndef DUMPI_LIBUNDUMPI_LIBUNDUMPI_H
#define DUMPI_LIBUNDUMPI_LIBUNDUMPI_H

#include <dumpi/libundumpi/callbacks.h>
#include <dumpi/common/io.h>
#include <dumpi/common/argtypes.h>
#include <dumpi/common/constants.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /// Forward declaration.
  struct libundumpi_cbpair;

  /**
   * \defgroup libundumpi libundumpi:  Parsing DUMPI trace files.
   */
  /*@{*/ /* put subsequent doxygen comments in group libundumpi */

  /**
   * Open a trace file for reading.
   * It is the caller's job to clean up using undumpi_close.
   * \param fname  the name of the binary trace file
   * \return a heap-allocated object on success, NULL on failure
   */
  dumpi_profile* undumpi_open(const char* fname);

  /**
   * All done.
   */
  void undumpi_close(dumpi_profile* profile);

  /**
   * Get the header information from the given file.
   * It is the caller's job to release the return value using
   * dumpi_free_header()).
   * \param profile a profile previously opened using undumpi_open
   * \return        the header record
   */
  dumpi_header* undumpi_read_header(dumpi_profile* profile);
 
  /**
   * Read a single MPI call off a stream starting at current position.
   * Note that you need to call dumpi_start_stream_read before calling
   * this method for the first time.
   * \param profile        the file that gets read
   * \param callarr        array of callbacks for MPI functions.
   * \param userarg        this argument gets sent back with each callback
   * \param mpi_finalized  state flag -- gets updated when MPI_Finalize is
   *                       encountered (should have value 0 on first call).
   *                       Unfortunately, this exposes internal operations --
   *                       please don't mess with this value.
   * \return               1 if the stream is still active.
   */
  int undumpi_read_single_call(dumpi_profile *profile,
			       struct libundumpi_cbpair *callarr,
			       void *userarg,
			       int *mpi_finalized);
  /**
   * Parse the stream of MPI commands.
   * \param profile  the file that gets read.
   * \param callback the functions that get called for each MPI function
   * \param userarg  this argument gets sent back with each callback.
   * \return 1 on success, 0 on failure.
   */
  int undumpi_read_stream_full(
    const char* metaname,
    dumpi_profile* profile,
    const libundumpi_callbacks *callback,
    void *userarg,
    bool print_progress,
    double percent_terminate);

  /**
   * Parse the stream of MPI commands.
   * \param profile  the file that gets read.
   * \param callback the functions that get called for each MPI function
   * \param userarg  this argument gets sent back with each callback.
   * \return 1 on success, 0 on failure.
   */
  int undumpi_read_stream(dumpi_profile* profile,
                          const libundumpi_callbacks *callback,
                          void *userarg);

  /**
   * Parse the keyval (user-populated) record from this file.
   * It is the caller's responsibility to free the returned object
   *  using dumpi_free_keyval after use.
   * \param profile   the file that gets read.
   * \return          the set of keyvals on success, NULL on failure
   */
  dumpi_keyval_record* undumpi_read_keyval_record(dumpi_profile* profile);

  /**
   * Get the footer information from the given file (call counts etc.).
   * \param profile  the file that gets read.
   * \return         the footer record that gets updated (release using
   *                 dumpi_free_keyval_record()).
   */
  dumpi_footer* undumpi_read_footer(dumpi_profile* profile);

  /**
   * Read datatype sizes.  Wrapper around dumpi_read_datatype_sizes.
   * It is the caller's responsibility to free the array of type sizes.
   * \return          A struct containing all registered type sizes
   *                  with zero size if no types were registered.
   */
  dumpi_sizeof undumpi_read_datatype_sizes(dumpi_profile *profile);

  /*@}*/ /* close doxygen comment group */

#ifdef __cplusplus
} /* End of extern "C" block */
#endif /* ! __cplusplus */

#endif /* DUMPI_LIBUNDUMPI_LIBUNDUMPI_H */