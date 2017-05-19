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
#include <stdlib.h>

int main(int argc, char **argv) {
  d2dopts opt;
  int error = 0;
  if((error = d2d_collect_opts(argc, argv, &opt))) goto abandon_ship;
  if(opt.help) {
    d2d_print_help(argc, argv, &opt);
    goto abandon_ship;
  }
  /* Set up the whole mess of callbacks */
  if(opt.verbose) fprintf(stderr, "Setting callbacks.\n");
  if((error = d2d_set_callbacks(&opt))) goto abandon_ship;
  if(opt.metafile) {
    d2dmeta meta;
    int rank;
    char *ifname=NULL, *ofname=NULL;
    if(opt.verbose) fprintf(stderr, "Parsing metadata.\n");
    if((error = d2d_parse_metadata(&opt, &meta))) goto abandon_ship;
    if(opt.verbose) fprintf(stderr, "Parsing tracefiles.\n");
    ifname = (char*)malloc(meta.maxname);
    ofname = (char*)malloc(meta.maxname);
    for(rank = 0; rank < meta.size; ++rank) {
      snprintf(ifname, meta.maxname, meta.traceformat, rank);
      snprintf(ofname, meta.maxname, meta.outformat, rank);
      if(opt.verbose) {
	fprintf(stderr, "  Processing rank %d\n"
		"  Trace input %s\n"
		"  Trace output %s\n",
		rank, ifname, ofname);
      }
      if((error = d2d_parse_stream(ifname, ofname, &opt))) goto abandon_ship;
    }
    if(opt.verbose) fprintf(stderr, "Writing new metadata.\n");
    if((error = d2d_write_metadata(&opt, &meta))) goto abandon_ship;
    free(ifname);
  }
  else {
    if(opt.verbose) fprintf(stderr, "Parsing tracefile.\n");
    error = d2d_parse_stream(opt.infile, opt.outfile, &opt);
  }

 abandon_ship:
  return error;
}