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
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>

#define DUMPI_BLEN 1024

static inline int get_int(const char *val, int *error) {
  char *endptr;
  long vv = strtol(val, &endptr, 10);
  if(errno == ERANGE || vv > INT32_MAX) {
    fprintf(stderr, "Error:  Integer value out of 32-bit range: %s\n", val);
    *error = 11;
  }
  if(! (*val != '\0' && *endptr == '\0')) {
    fprintf(stderr, "Error:  Invalid integer value: %s\n", val);
    *error = 11;
  }
  return vv;
}

/* Given a metafile (opt.metafile), figure out the metadata settings */
int d2d_parse_metadata(const d2dopts *opt, d2dmeta *meta) {
  int error = 0, i, len, zeroes;
  char *ip, *key, *value, *dir = NULL, *prefix = NULL;
  char buf[DUMPI_BLEN];
  FILE *metafile = NULL;
  assert(opt != NULL);
  assert(meta != NULL);
  memset(meta, 0, sizeof(d2dmeta));
  dir = strdup(opt->metafile);
  for(i = strlen(dir)-1; i > 0; --i) {
    if(dir[i] == '/') break;
  }
  dir[i] = '\0';
  metafile = fopen(opt->metafile, "r");
  if(! metafile) {
    fprintf(stderr, "Error: Failed to open metafile \"%s\" for reading: %s\n",
	    opt->metafile, strerror(errno));
    error = 10;
    goto escape_hatch;
  }
  while(! feof(metafile)) {
    ip = fgets(buf, DUMPI_BLEN, metafile);
    if(ip == NULL) {
      /* Either the file is done or we have an error */
      if(! feof(metafile)) {
	if((error = ferror(metafile))) {
	  fprintf(stderr, "Error: Problem reading metafile: %s\n",
		  strerror(error));
	  fclose(metafile);
	  error = 10;
	  goto escape_hatch;
	}
	break;
      }
      break;
    }
    /* We have a string -- let's parse this mess. */
    key = value = NULL;
    for(i = 0; ip[i] != '\0'; ++i) {
      if(ip[i] == '=') {
	ip[i] = '\0';
	key = ip;
	value = ip+i+1;
      }
      else if(ip[i] == '\n')
	ip[i] = '\0';
    }
    if(key && *value) {
      if(strcmp(key, "numprocs") == 0)
	meta->size = get_int(value, &error);
      else if(strcmp(key, "fileprefix") == 0)
	prefix = strdup(value);
      else if(strcmp(key, "hostname") == 0) meta->hostname = strdup(value);
      else if(strcmp(key, "username") == 0) meta->username = strdup(value);
      else if(strcmp(key, "startime")== 0) meta->starttime= strdup(value);
      if(error) goto escape_hatch;
    }
  }
  /* We should now have a size and file prefix (or else) */
  if(meta->size < 0 || prefix == NULL) {
    fprintf(stderr, "Error: Invalid input in metafile (size or prefix)\n");
    error = 8;
    goto escape_hatch;
  }
  /* Figure out the fully qualified format for the input files */
  /* We assume that the user has not opted for more than 500 digits
   * to write the node rank (otherwise, the user is a twit) */
  /* We start by searching at %04d, since that is the default from dumpi */
  for(zeroes = 0; zeroes < 500; ++zeroes) {
    int all_found = 1;
    int fmtw = (zeroes+4)%500;
    FILE *tmpf;
    if((fmtw + strlen(prefix)) > DUMPI_BLEN) {
      continue; /* we simply skip names that would overflow our boundaryx */
    }
    for(i = 0; i < meta->size; ++i) {
      snprintf(buf, DUMPI_BLEN, "%s%s%s-%0*d.bin",
	       dir, (*dir != '\0' ? "/" : ""), prefix, fmtw, i);
      if((tmpf = fopen(buf, "r")) == NULL) {
	all_found = 0;
	break;
      }
      else {
	fclose(tmpf);
      }
    }
    if(all_found) {
      /* this is the format we want. */
      snprintf(buf, DUMPI_BLEN, "%s%s%s-%%0%dd.bin",
	       dir, (*dir != '\0' ? "/" : ""), prefix, fmtw);
      meta->traceformat = strdup(buf);
      meta->maxname = strlen(dir) + 1 + strlen(prefix) + fmtw + 50;
      break;
    }
  }
  /* Did we find the inroot? */
  if(! meta->traceformat) {
    fprintf(stderr, "Error:  Failed to find a set of trace files with "
	    "the common file prefix %s\n", prefix);
    error = 9;
    goto escape_hatch;
  }
  /* Now figure out what our outroot should be. */
  snprintf(buf, DUMPI_BLEN, "%s-%%04d.bin", opt->outfile);
  len = strlen(opt->outfile) + 50;
  if(len > meta->maxname) meta->maxname = len;
  meta->outformat = strdup(buf);
  if(opt->verbose)
    fprintf(stderr, "  Full input file format string: %s\n"
	    "  Full output file format string: %s\n",
	    meta->traceformat, meta->outformat);
  /* Clean up, clean up, everybody everywhere */
  free(dir);
  free(prefix);
  fclose(metafile);
  /* Exeunt */
 escape_hatch:
  return error;
}

/* Write new and updated metadata. */
int d2d_write_metadata(const d2dopts *opt, const d2dmeta *meta) {
  char buf[DUMPI_BLEN];
  FILE *mfile;
  int error = 0;
  snprintf(buf, DUMPI_BLEN, "%s.meta", opt->outfile);
  if((mfile = fopen(buf, "w")) == NULL) {
    fprintf(stderr, "Error: Failed to create metafile %s: %s\n",
	    buf, strerror(errno));
    error = 1;
    goto purgatory;
  }
  fprintf(mfile, "hostname=%s\n", meta->hostname);
  fprintf(mfile, "numprocs=%d\n", meta->size);
  fprintf(mfile, "username=%s\n", meta->username);
  fprintf(mfile, "startime=%s\n", meta->starttime);
  fprintf(mfile, "fileprefix=%s\n", opt->outfile);
  fprintf(mfile, "version=%d\n", (int)dumpi_version);
  fprintf(mfile, "subversion=%d\n", (int)dumpi_subversion);
  fprintf(mfile, "subsubversion=%d\n", (int)dumpi_subsubversion);  
  fclose(mfile);
 purgatory:
  return error;
}