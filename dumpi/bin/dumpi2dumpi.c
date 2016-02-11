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

/* Pick select records from a DUMPI trace file and write to a new trace */

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

