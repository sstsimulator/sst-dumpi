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
#include <dumpi/common/funcs.h>
#include <dumpi/common/settings.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

dumpi_function get_dumpi_id(const char *name, int *error) {
  int fid;
  for(fid = 0; fid < DUMPI_END_OF_STREAM; ++fid) {
    if(strcmp(name, dumpi_function_label((dumpi_function)fid)) == 0) {
      break;
    }
  }
  if(fid >= DUMPI_END_OF_STREAM) {
    fprintf(stderr, "Error:  Not a recognized MPI function: %s\n", name);
    *error = 5;
  }
  return (dumpi_function)fid;
}

int d2d_collect_opts(int argc, char **argv, d2dopts *opt) {
  int ch, i, error = 0;
  dumpi_function fid;
  static struct option longopts[] = {
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},
    {"output-mpi", no_argument, NULL, 'f'},
    {"skip-mpi", no_argument, NULL, 'F'},
    {"output-wall-time", no_argument, NULL, 'w'},
    {"skip-wall-time", no_argument, NULL, 'W'},
    {"output-cpu-time", no_argument, NULL, 'c'},
    {"skip-cpu-time", no_argument, NULL, 'C'},
    {"output-perfctrs", no_argument, NULL, 'p'},
    {"skip-perfctrs", no_argument, NULL, 'P'},
    {"with-user-functions", no_argument, NULL, 'u'},
    {"without-user-functions", no_argument, NULL, 'U'},
    {"with-mpi", required_argument, NULL, 'm'},
    {"without-mpi", required_argument, NULL, 'M'},
    {"infile", required_argument, NULL, 'i'},
    {"metafile", required_argument, NULL, 'I'},
    {"outfile", required_argument, NULL, 'o'}
  };
  assert(opt != NULL);
  memset(opt, 0, sizeof(d2dopts));
  opt->output.timestamps = DUMPI_TIME_FULL;
  opt->write_userfuncs = 1;
  for(i = 0; i < DUMPI_END_OF_STREAM; ++i) opt->output.function[i] = 1;
  
  while((ch = getopt_long(argc, argv, "hvfFwWcCpPuUm:M:i:I:o:",
			  longopts, NULL)) != -1)
    {
      switch(ch) {
      case 'h':
	opt->help = 1; break;
      case 'v':
	opt->verbose = 1; break;
      case 'f':
	for(i = 0; i < DUMPI_ALL_FUNCTIONS; ++i) opt->output.function[i] = 1;
	break;
      case 'F':
	for(i = 0; i < DUMPI_ALL_FUNCTIONS; ++i) opt->output.function[i] = 0;
	break;
    case 'w':
      opt->output.timestamps |= DUMPI_TIME_WALL; break;
      case 'W':
	opt->output.timestamps &= (~DUMPI_TIME_WALL); break;
      case 'c':
	opt->output.timestamps |= DUMPI_TIME_CPU; break;
      case 'C':
	opt->output.timestamps &= (~DUMPI_TIME_CPU); break;
      case 'p':
	opt->output.perfinfo = 1; break;
      case 'P':
	opt->output.perfinfo = 0; break;
      case 'u':
	opt->write_userfuncs = 1;
	opt->output.function[DUMPI_Function_enter] = 1;
	opt->output.function[DUMPI_Function_exit] = 1;
	break;
      case 'U':
	opt->write_userfuncs = 0;
	opt->output.function[DUMPI_Function_enter] = 0;
	opt->output.function[DUMPI_Function_exit] = 0;
	break;
      case 'm':
	fid = get_dumpi_id(optarg, &error);
	if(! error) opt->output.function[fid] = 1;
	break;
      case 'M':
	fid = get_dumpi_id(optarg, &error);
	if(! error) opt->output.function[fid] = 0;
	break;
      case 'i':
	if(opt->infile) {
	  fprintf(stderr, "The input file can only be specified once\n");
	  error = 2;
	}
	if(opt->metafile) {
	  fprintf(stderr, "You cannot specify both an infile and a metafile\n");
	  error = 2;
	}
	if(! error) opt->infile = strdup(optarg);
	break;
      case 'I':
	if(opt->metafile) {
	  fprintf(stderr, "The metafile can only be specified once\n");
	  error = 2;
	}
	if(opt->infile) {
	  fprintf(stderr, "You cannot specify both an infile and a metafile\n");
	  error = 2;
	}
	if(! error) opt->metafile = strdup(optarg);
	break;
      case 'o':
	opt->outfile = strdup(optarg);
	break;
      default:
	error = 1;
      }
      if(opt->help || error) break;
    }
  if(! opt->help) {
    if(! opt->infile && ! opt->metafile) {
      fprintf(stderr, "Error: Specify either an infile or a metafile\n");
      error = 4;
    }
    if(!opt->outfile) {
      fprintf(stderr, "Error: You must specify an outfile\n");
      error = 3;
    }
  }
  return error;
}