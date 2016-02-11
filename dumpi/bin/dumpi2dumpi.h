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

#ifndef DUMPI_BIN_DUMPI2DUMPI_H
#define DUMPI_BIN_DUMPI2DUMPI_H

#include <dumpi/common/types.h>
#include <dumpi/libundumpi/callbacks.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \defgroup dumpi2dumpi  dumpi2dumpi:  Convert or trim dumpi files.
   */
  /*@{*/

  /**
   * Hold input settings for dumpi2dumpi.
   */
  typedef struct d2dopts {
    int help;
    int verbose;
    const char *infile;
    const char *metafile;
    const char *outfile;
    int write_userfuncs;
    dumpi_outputs output;
    libundumpi_callbacks cback;
    const char *outname; /* Used internally for parsing */
    dumpi_profile *oprofile;
    dumpi_footer footer;
  } d2dopts;

  /**
   * Extended metadata -- allows us to figure out file formats etc. only once.
   */
  typedef struct d2dmeta {
    /** The total number of nodes (numprocs in the input/output metafiles */
    int size;
    /** The format std::string for input trace files (complete with %0xd). */
    char *traceformat;
    /** The format std::string for output trace files */
    char *outformat;
    /** The longest tracefile name encountered for input or output */
    int maxname;
    /** Carried over unchanged from input metafile */
    char *hostname;
    char *username;
    char *starttime;
    /** Version, subversion, and subsubversion are reported for current dumpi */
  } d2dmeta;

  /** Poetry in motion */
  void d2d_print_help(int argc, char **argv, const d2dopts *opt);

  /** Grab input options for dumpi2dumpi */
  int d2d_collect_opts(int argc, char **argv, d2dopts *opt);

  /** Set callbacks for MPI functions. */
  int d2d_set_callbacks(d2dopts *opt);

  /** Given a metafile (opt.metafile), figure out the metadata settings */
  int d2d_parse_metadata(const d2dopts *opt, d2dmeta *meta);

  /** Parse input and write output for a single binary trace file. */
  int d2d_parse_stream(const char *in, const char *out, d2dopts *opt);

  /** Write new and updated metadata. */
  int d2d_write_metadata(const d2dopts *opt, const d2dmeta *meta);
 
  /*@}*/

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_BIN_DUMPI2DUMPI_H */
