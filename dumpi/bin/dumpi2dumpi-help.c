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