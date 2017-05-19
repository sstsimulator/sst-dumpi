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