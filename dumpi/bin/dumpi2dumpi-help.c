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

#include <dumpi/bin/dumpi2dumpi.h>
#include <stdio.h>

void d2d_print_help(int argc, char **argv, const d2dopts *opt) {
  (void)argc; (void)opt;
  fprintf(stderr,
	  "Use: %s [options] [-i old_tracefile] [-o new_tracefile]\n"
	  "   Opts: (-h|--help)                       Print help\n"
	  "         (-v|--verbose)                    Verbose output\n"
	  "         (-f|--output-mpi)                 Output all MPI functions\n"
	  "         (-F|--skip-mpi)                   Output no MPI functions\n"
	  "         (-w|--output-wall-time)           Output wall time values\n"
	  "         (-W|--skip-wall-time)             Don't output wall times\n"
	  "         (-c|--output-cpu-time)            Output CPU time values\n"
	  "         (-C|--skip-cpu-time)              Don't output CPU times\n"
	  "         (-p|--output-perfctrs)            Output all perfctr info\n"
	  "         (-P|--skip-perfctrs)              Skip all perfctr info\n"
	  "         (-u|--with-user-functions         Output any user functions\n"
	  "         (-U|--without-user-functions      Omit any user functions\n"
	  "         (-m|--with-mpi)        FUNCTION   Output the named function\n"
	  "         (-M|--without-mpi)     FUNCTION   Omit the named function\n"
	  "         (-i|--infile)          FILENAME   Read the given trace file\n"
	  "         (-I|--metafile)        FILENAME   Read the given metafile\n"
	  "         (-o|--outfile)         FILENAME   Write to the given file\n"
	  "\n"
	  "Options are parsed in input order, so for example:\n"
	  "\n"
	  "    %s -F -P -m MPI_Init -m MPI_Finalize -i trace.bin -o foo.bin\n"
	  "will (1) read the binary trace file 'trace.bin'\n"
	  "     (2) output only MPI_Init and MPI_Finalize information (sans\n"
	  "         perfcounters) to the binary trace file foo.bin\n"
	  "\n"
	  "    %s -f -M MPI_Probe -M MPI_Iprobe -m trace.meta -o foo\n"
	  "will (1) read trace.meta to figure out which traces to parse\n"
	  "     (2) read each trace file and output all information except\n"
	  "         MPI_Probe and MPI_Iprobe to a file named foo-<rank>.bin\n"
	  "     (3) write a new metafile 'foo.meta' that points to the\n"
	  "         new trace files\n"
	  "\n"
	  "Note that the -i and -m options are mutually exclusive.\n",
	  argv[0], argv[0], argv[0]);	 
}
