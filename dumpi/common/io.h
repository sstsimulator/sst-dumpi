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

#ifndef DUMPI_COMMON_IO_H
#define DUMPI_COMMON_IO_H

#include <dumpi/common/settings.h>
#include <dumpi/common/types.h>
#include <dumpi/common/perfctrs.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup common
   * \defgroup common_io  common/io:  Common I/O routines for DUMPI.
   */
  /*@{*/ /* The rest of the comments should go into common/io */

  /**
   * Initialize the given profile.
   * This simply clears all data in the profile, making it ready to 
   * be used for either reading or writing (see dumpi_open_input_file
   * and dumpi_open_output_file).
   */
  void dumpi_init_profile(dumpi_profile *profile);

  /** 
   * Write a header record to the given profile.
   * The record gets written at current file position,
   * and the file position gets updated to reflect the amount of
   * data written.
   * \param header  The header record to be written.
   * \param profile The profile to which we write.  The profile->header
   *        member will be updated to reflect the file position of the 
   *        header record 
   * \return non-zero on success.
   */
  int dumpi_write_header(dumpi_profile *profile, const dumpi_header *header);

  /**
   * Read a header record from the given profile.
   * As of version 0.6.3, this method sets the file position back to its
   * original (input) position before returning.
   * \param header  A non-null header object that will be updated with the
   *                header information from the profile.
   * \param profile A valid profile (open for reading).
   * \return non-zero on success.
   */
  int dumpi_read_header(dumpi_profile *profile, dumpi_header *header);

  /**
   * Write a footer record to the given profile.
   * The record gets written at current file position,
   * and the file position gets updated to reflect the amount of
   * data written.
   * \param footer  The footer record to be written.
   * \param profile The profile to which we write.  The profile->footer
   *                member will be updated to reflect the file position of
   *                the footer record 
   * \return non-zero on success.
   */
  int dumpi_write_footer(dumpi_profile *profile, const dumpi_footer *footer);

  /**
   * Read a footer record from the given profile.
   * Sets the file position back to its original (input) position
   * before returning.
   * \param footer  A non-null footer object that will be updated with the
   *                footer information from the profile.
   * \param profile A valid profile (open for reading).
   * \return non-zero on success
   */
  int dumpi_read_footer(dumpi_profile *profile, dumpi_footer *footer);

  /**
   * Write a keyval record to the given profile.
   * The record gets written at current file position,
   * and the file position gets updated to reflect the amount of
   * data written.
   * \param keyval  The keyval record to be written.
   * \param profile The profile to which we write.  The profile->keyval
   *                member will be updated to reflect the file position of
   *                the keyval record 
   * \return non-zero on success.
   */
  int dumpi_write_keyval_record(dumpi_profile *profile,
				const dumpi_keyval_record *keyval);

  /**
   * Read a keyval record from the given profile.
   * Sets the file position back to its original (input) position
   * before returning.
   * \param keyval  A non-null keyval object that will be updated with the
   *                keyval information from the profile.
   * \param profile A valid profile (open for reading).
   * \return non-zero on success
   */
  int dumpi_read_keyval_record(dumpi_profile *profile,
			       dumpi_keyval_record *keyval);

  /** 
   * Write labels for active PAPI counters.
   * Added in version 0.3.
   * The record gets written at current file position,
   * and the file position gets updated to reflect the amount of
   * data written.
   * \param counters The number of active PAPI counters.
   * \param labels   String names for the counters (array of length counters).
   * \param profile The profile to which we write.  The profile->perflbl
   *                member will be updated to reflect the file position of
   *                the record 
   * \return non-zero on success.
   */
  int dumpi_write_perfctr_labels(dumpi_profile *profile, int counters,
                                 dumpi_perflabel_t *labels);

  /**
   * Read labels for active PAPI counters.
   * Added in version 0.3.
   * If the value of profile->perflbl is 0, this profile is assumed to predate
   * version 0.3 (valid DUMPI files have a magic sequence in the first 8 bytes).
   * \param counters  Gets updated to hold the number of active PAPI counters.
   * \param labels    String names for the counters.  This needs to be a vaild 
   *                  array of length maxlabels.
   * \param maxlabels Length of the 'labels' array.  If this value is less
   *                  than *counters, only the first 'maxlabels' names will
   *                  be read.
   * \param profile   A valid input profile (open for reading). 
   * \return non-zero on success.
   */
  void dumpi_read_perfctr_labels(dumpi_profile *profile,
				 int *counters,
                                 dumpi_perflabel_t *labels,
                                 int maxlabels);

  /** 
   * Write labels for profiled functions.
   * Added in version 0.4 
   * The record gets written at current file position,
   * and the file position gets updated to reflect the amount of
   * data written.
   * \param count      The number of function addresses
   * \param addresses  The profiled addresses cast to unsigned 64-bit integers
   *                   Array of length count.
   * \param names      The name of each profiled function (blanks are ok).
   *                   Array of length count.
   * \param profile    The profile to which we write.  The profile->addrlbl
   *                   member will be updated to reflect the file position of
   *                   the record
   */
  int dumpi_write_function_addresses(dumpi_profile *profile,
				     int count,
                                     const uint64_t *addresses,
                                     char **names);

  /**
   * Read labels for profiled functions.
   * Added in v. 0.4.
   * The arrays for addresses (int64_t*) and names (char**) get allocated
   * inside this routine.  It is the responsibility of the caller to
   * free those up after use.
   * If the value of profile->perflbl is 0, this profile is assumed to predate
   * version 0.3 (valid DUMPI files have a magic sequence in the first 8 bytes).
   * \param count     Gets updated to hold the number of addresses.  This gets
   *                  set to zero if no addresses are in the trace file.
   * \param addresses The profiled addresses cast to unsigned 64-bit integers.
   *                  Array of length *count.
   * \param names     The name of each profiled function (may be blank).
   *                  Array of length *count.
   * \param profile   A valid input profile.
   */
  int dumpi_read_function_addresses(dumpi_profile *profile, int *count,
                                    uint64_t **addresses, char ***names);

  /**
   * Write size information for available built-in datatypes for the MPI
   * C interface and all user-defined datatypes.  Unfortunately, there is
   * no clean way of handling Fortran-specific datatypes (the solution would
   * all either require runtime-querying of a Fortran code segment or be
   * tied to a specific MPI implementation).
   * \param sizes    Size information.
   * \param profile  A valid output profile.
   */
  int dumpi_write_datatype_sizes(dumpi_profile *profile,
				 const dumpi_sizeof *sizes);

  /**
   * Read size information for a profiled run.
   * Added in v. 0.6.6.  Updated in v.0.6.7 to take dumpi_sizeof* 
   * instead of dumpi_sizeof** as first argument.
   * It is the caller's responsibility to free dumpi_sizeof::size after use.
   * \param sizes     Pointer to a valid object of type dumpi_sizeof.
   * \param profile   A valid input profile.
   */
  int dumpi_read_datatype_sizes(dumpi_profile *profile, dumpi_sizeof *sizes);

  /**
   * Write the index to a file.
   * This must be the the last write call made to a profile (closes the stream) 
   * Writes the values of all the offset markers in the profile.
   */
  int dumpi_write_index(dumpi_profile *profile);

  /* Open a profile file and read its header magic, footer magic,
   * and index table.
   * \param fname  The name of the file to be opened.
   * \return NULL if the file is not recognized as a valid dumpi file. */
  dumpi_profile* dumpi_open_input_file(const char *fname);

  /**
   * Create a new blank profile with a null file pointer.
   * This method is most appropriate for creating an output profile prior
   * to knowing the final file name. 
   * Initializes the profile::version array to current version information.
   */
  dumpi_profile* dumpi_alloc_output_profile(int cpu_bias, int wall_bias,
                                            int target_buffer_size);

  /**
   * Free dynamic memory for an output profile.
   */
  void dumpi_free_output_profile(dumpi_profile *profile);

  /**
   * Open a file for output.
   * Currently aborts on failure, but may be updated to return NULL on failure.
   */
  DUMPIFILE dumpi_open_output_file(const char *fname);

  /**
   * Write the lead-in for a stream.
   */
  int dumpi_start_stream_write(dumpi_profile *profile);

  /**
   * Move to the head of the MPI stream.
   */
  int dumpi_start_stream_read(dumpi_profile *profile);

  /*@}*/ /* Close the documentation module */

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#include <dumpi/common/dumpiio.h>

#endif /* ! DUMPI_COMMON_IO_H */
