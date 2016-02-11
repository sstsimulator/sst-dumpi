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

#ifndef DUMPI_COMMON_TYPES_H
#define DUMPI_COMMON_TYPES_H

#include <dumpi/common/funclabels.h>
#include <dumpi/common/perfctrtags.h>
#include <stdint.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \defgroup common common:  Functionality shared by the various parts of \
   * DUMPI (libdumpi, libundumpi, test programs, conversion utilities, etc.)
   */

  /**
   * \ingroup common
   * \defgroup types common/types:  Type definitions shared by various parts \
   * of DUMPI.
   */

  /*@{*/ /* Plop things into the common/types documentation module */
  
  /** A reasonably compact type handle for an MPI combiner */
  typedef int16_t dumpi_combiner;
  /** A reasonably compact type handle for an MPI comm */
  typedef int16_t dumpi_comm;
  /** A reasonably compact type handle for an MPI comparison */
  typedef int8_t  dumpi_comparison;
  /** A reasonably compact type handle for an MPI datatype */
  typedef int16_t dumpi_datatype;
  /** A reasonably compact type handle for an MPI dest */
  typedef int32_t dumpi_dest;
  /** A reasonably compact type handle for an MPI distribution */
  typedef int16_t dumpi_distribution;
  /** A reasonably compact type handle for an MPI errcode */
  typedef int32_t dumpi_errcode;
  /** A reasonably compact type handle for an MPI errhandler */
  typedef int16_t dumpi_errhandler;
  /** A reasonably compact type handle for an MPI file */
  typedef int16_t dumpi_file;
  /** A reasonably compact type handle for an MPI filemode */
  typedef int16_t dumpi_filemode;
  /** A reasonably compact type handle for an MPI group */
  typedef int16_t dumpi_group;
  /** A reasonably compact type handle for an MPI info */
  typedef int16_t dumpi_info;
  /** A reasonably compact type handle for an MPI keyval */
  typedef int16_t dumpi_keyval;
  /** A reasonably compact type handle for an MPI comm_keyval */
  typedef int16_t dumpi_comm_keyval;
  /** A reasonably compact type handle for an MPI type_keyval */
  typedef int16_t dumpi_type_keyval;
  /** A reasonably compact type handle for an MPI win_keyval */
  typedef int16_t dumpi_win_keyval;
  /** A reasonably compact type handle for an MPI locktype */
  typedef int16_t dumpi_locktype;
  /** A reasonably compact type handle for an MPI op */
  typedef int16_t dumpi_op;
  /** A reasonably compact type handle for an MPI ordering */
  typedef int8_t  dumpi_ordering;
  /** A reasonably compact type handle for an MPI source */
  typedef int32_t dumpi_source;
  /** Tag value was converted from 16-bit to 32-bit value in 0.6.5
   *  The value has always been stored as a 32-bit value in the file */
  typedef int32_t dumpi_tag;
  /** A reasonably compact type handle for an MPI threadlevel */
  typedef int16_t dumpi_threadlevel;
  /** A reasonably compact type handle for an MPI topology */
  typedef int16_t dumpi_topology;
  /** A reasonably compact type handle for an MPI typeclass */
  typedef int16_t dumpi_typeclass;
  /** A reasonably compact type handle for an MPI whence */
  typedef int16_t dumpi_whence;
  /** A reasonably compact type handle for an MPI win */
  typedef int16_t dumpi_win;
  /** A reasonably compact type handle for an MPI win_assert */
  typedef int16_t dumpi_win_assert;
  /** A reasonably compact type handle for an MPI request */
  typedef int32_t dumpi_request;
  /** A reasonably compact type handle for an MPIO request */
  typedef int32_t dumpio_request;

  /**
   * Reasonably compact status information (16 bytes is not exactly compact,
   * but it is hard to shrink this any further without risking overflow).
   */
  typedef struct dumpi_status {
    int32_t bytes;
    int32_t source;
    int32_t tag;
    int8_t  cancelled;
    int8_t  error;
  } dumpi_status;

  /** This typedef dates back to when we worked with compressed streams
   * (which turned out not to be worth the effort) */
#define DUMPIFILE FILE*
#define DUMPI_FPOS long
#define DUMPI_FOPEN(FP, MODE) fopen(FP, MODE)
#define DUMPI_FCLOSE(FP) fclose(FP)

  /**
   * A header record for dumpi output.
   */
  typedef struct dumpi_header {
    /** DUMPI version information */
    char      version[3];
    /** the output from time(2) when DUMPI was initialized */
    uint64_t  starttime;
    /** the value of the HOSTNAME environment variable */
    char     *hostname;
    /** the value reported by gethostname */
    char     *username;
    /** dimensions in the meshcrd variable.  This value is relevant on
     * RSQ (and possibly other CRAY platforms) and BlueGene machines.
     * On machines where we don't have (or know) mesh position, this is zero */
    int       meshdim;
    /** numerical id for network location.  Array of dimension meshdim */
    int      *meshcrd;
    /** dimensions of our mesh.  Array of dimension meshdim */
    int      *meshsize;
  } dumpi_header;

  /**
   * Create a fully populated header record.
   * It is the responsibility of the caller to free this object using
   * dumpi_free_header.
   */
  dumpi_header* dumpi_build_header(void);

  /**
   * Free up a header record.
   */
  void dumpi_free_header(dumpi_header *header);

  /**
   * A keyval entry.  Keyval entries are handled as a linked list
   * due to performance concerns in libdumpi.
   */
  typedef struct dumpi_keyval_entry {
    char *key;
    char *val;
    struct dumpi_keyval_entry *next;
  } dumpi_keyval_entry;

  /**
   * A keyval record -- stored as a linked list of dumpi_keyval_entry.
   */
  typedef struct dumpi_keyval_record {
    int count;
    dumpi_keyval_entry *head;
    dumpi_keyval_entry *tail;
  } dumpi_keyval_record;

  /**
   * Allocate a keyval record.
   */
  dumpi_keyval_record* dumpi_alloc_keyval_record(void);

  /**
   * Add an entry to the keyval.
   * This call allocates memory for the new keyval
   */
  void dumpi_push_keyval_entry(dumpi_keyval_record *record,
                               const char *key, const char *value);

  /**
   * Delete an entry from the keyval chain.
   * The entry is an actual pointer into the linked list.
   * The entry is deallocated iff it is found in this keyval record.
   * Return 1 if the entry was found and erased, 0 otherwise.
   */
  int dumpi_erase_keyval_entry(dumpi_keyval_record *record,
                                dumpi_keyval_entry *entry);

  /**
   * Clear a keyval record.
   * Frees all memory allocated to the keyval entries
   * and sets keyval->count to 0.
   */
  void dumpi_clear_keyval_record(dumpi_keyval_record *keyval);

  /**
   * Free up a keyval record.
   * Calls dumpi_clear_keyval_record and then frees the keyval object.
   */
  void dumpi_free_keyval_record(dumpi_keyval_record *keyval);

  /**
   * A footer record for dumpi output.
   */
  typedef struct dumpi_footer {
    /** The number of times each MPI function was called */
    uint32_t         call_count[DUMPI_ALL_FUNCTIONS+1];
    /** The number of times an MPI function was called but not profiles */
    uint32_t         ignored_count[DUMPI_ALL_FUNCTIONS+1];
  } dumpi_footer;

  /**
   * Free up a footer record.
   */
  void dumpi_free_footer(dumpi_footer *footer);

  /** Forward declaration of the memory buffer type (defined in iodefs.c). */
  struct dumpi_memory_buffer;

  /**
   * Specify what output gets written and keep track of call counts.
   * This is mainly for internal consumption (not instrumentation/undumping).
   */
  typedef struct dumpi_profile {
    /** The trace file we are operating on */
    DUMPIFILE        file;
    /** The bias (in seconds) for storing wall times on the stream.
     *  This allows time values to be stored in 2 fewer bytes each time.
     *  Note that this correction is applied transparently upon reading
     *  the trace file. */
    int32_t          cpu_time_offset;
    /** The bias (in seconds) for cpu wall times on the stream.
     *  This allows time values to be stored in 2 fewer bytes each time.
     *  Note that this correction is applied transparently upon reading
     *  the trace file. */
    int32_t          wall_time_offset;
    /** The file positions of the various record entries in a trace file */
    DUMPI_FPOS       header, body, footer, keyval, perflbl, addrlbl, sizelbl;
    /**
     * The version of DUMPI involved in generating a trace file.
     * This information is also available from the header record --
     * it was added here in version 0.6.3 due to backward compatibility issues
     */
    char             version[3];
    /**
     * The output buffer for writes (not used for reads).
     * This used to be a static object, but this is an unacceptable
     * limitation if we want the SST simulator to write dumpi tracefiles.
     */
    struct dumpi_memory_buffer *membuf;
    /**
     * Define the memory buffer size.
     * If this is set to a value less than DUMPI_MIN_MEMBUF_SIZE,
     * it will be set to DUMPI_MEMBUF_SIZE (by default 128 MB).
     */
    size_t target_membuf_size;
  } dumpi_profile;

  /**
   * Control data for what gets written to output files.
   * Values for timestamps and perfinfo are handled as booleans.
   * Values for statuses and function profiling are handled based on their
   * definition in the dumpi configuration (disable|success|enable).
   */
  typedef struct dumpi_outputs {
    int8_t           timestamps;
    int8_t           statuses;
    int8_t           perfinfo;
    int8_t           function[DUMPI_END_OF_STREAM];
  } dumpi_outputs;

  /**
   * Stash sizes for all types (built-in or user defined) that are encountered.
   * The stored size is the value reported by MPI_Type_size (which uses
   * int to report the type size).
   * Note that MPI_Type_size often returns a size of zero for types that
   * clearly have non-zero communiation cost (in particular the datatype
   * definitions commonly used for MPIO).
   * This functionality will be added in version 0.7.0, 
   * it is here as a placeholder.
   */
  typedef struct dumpi_sizeof {
    dumpi_datatype    count;
    int              *size;
  } dumpi_sizeof;

  /** The maximum number of perfcounters we support.
   * We may need to consider growing this */
#define DUMPI_MAX_PERFCTRS 128
  /** The maximum string name for a perfcounter.
   * We have yet to encounter a performance counter that gets anywhere
   * close to this name length */
#define DUMPI_MAX_PERFCTR_NAME 80
  /**
   * The container for PAPI perfcounter information.
   */
  typedef struct dumpi_perfinfo {
    /** The number of perfcounters collected. */
    int32_t count;
    /** The names of the performance counters we're using. */
    char counter_tag[DUMPI_MAX_PERFCTRS][DUMPI_MAX_PERFCTR_NAME];
    /** The value of each of the counters at DUMPI_PERFCTR_IN */
    int64_t invalue[DUMPI_MAX_PERFCTRS];
    /** The value of each of the counters at DUMPI_PERFCTR_OUT */
    int64_t outvalue[DUMPI_MAX_PERFCTRS];
  } dumpi_perfinfo;

  /**
   * This is effectively identical to struct timespec from time.h,
   * but some target platforms don't have high resolution timers.
   */
  typedef struct dumpi_clock {
    int32_t sec;
    int32_t nsec;
  } dumpi_clock;
  
  /**
   * Returns a dumpi_clock with the given time using the scale factor.
   */
  static inline dumpi_clock dumpi_clock_init_scale(int64_t t, int64_t scale) {
    dumpi_clock c = { (int32_t) (t/scale), (int32_t) (t%scale) };
    return c;
  }
  
  /**
   * Returns a dumpi_clock with the given time using the given field values.
   */
  static inline dumpi_clock dumpi_clock_init_time(int64_t tsec, int64_t tnsec) {
    dumpi_clock c = { (int32_t) tsec, (int32_t) tnsec };
    return c;
  }

  /**
   * Aggregate the start- and stop-time for a given function.
   */
  typedef struct dumpi_time {
    dumpi_clock start;   /* stored as 6 bytes */
    dumpi_clock stop;    /* stored as 6 bytes */
  } dumpi_time;

  /*@}*/ /* close the doxygen documentation module */

#ifdef __cplusplus
} /* End of extern "C" block */
#endif /* ! __cplusplus */

/* #include <dumpi/common/argtypes.h> */

#endif /* ! DUMPI_COMMON_TYPES_H */
