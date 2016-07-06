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
