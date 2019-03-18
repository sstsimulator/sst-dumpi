/**
Copyright 2009-2018 National Technology and Engineering Solutions of Sandia, 
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S.  Government 
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly 
owned subsidiary of Honeywell International, Inc., for the U.S. Department of 
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2018, NTESS

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of the copyright holder nor the names of its
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

#include <dumpi/bin/dumpi2sst-callbacks.h>
#include <dumpi/bin/dumpi2sst-defs.h>
#include <dumpi/common/funcs.h>
#include <dumpi/common/perfctrs.h>
#include <dumpi/common/debugflags.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <assert.h>
#include <dumpi/bin/metadata.h>

extern int optind;
FILE *dumpfh = NULL;
int rankProcessing = -1;

/*
 * User options.
 */
typedef struct d2aopt {
  const char *file;
} d2aopt;

static int parse_options(int argc, char **argv, d2aopt *opt);

int main(int argc, char **argv)
{
  dumpfh = stdout;
  libundumpi_callbacks cback;
  d2aopt opt;
  opt.file = nullptr;
  int rc = parse_options(argc, argv, &opt);
  if (rc) return rc;

  if (!opt.file){
    std::cerr << "No input file given with -f or last argument" << std::endl;
    return 1;
  }


  libundumpi_clear_callbacks(&cback);
  set_callbacks(&cback);


  dumpi::metadata md(opt.file);
  for (int rank=0; rank < md.numTraces(); ++rank){
    rankProcessing = rank;
    std::string trace_file = md.tracename(rank);
    auto* profile = undumpi_open(trace_file.c_str());
    if (!profile){
      throw std::runtime_error("failed opening trace " + trace_file);
    }
    undumpi_read_stream(profile, &cback, nullptr);
    undumpi_close(profile);
  }

  return 0;
}

static int parse_options(int argc, char **argv, d2aopt *settings) {
  int opt;
  assert(settings != NULL);
  memset(settings, 0, sizeof(d2aopt));
  while((opt = getopt(argc, argv, "f:")) != -1) {
    switch(opt) {
    case 'f':
      settings->file = strdup(optarg);
      break;
    default:
      std::cerr << "Invalid argument " << opt << std::endl;
      return 1;
    }
  }

  if(settings->file == NULL && optind < argc)
    settings->file = argv[optind];

  return 0;
}
