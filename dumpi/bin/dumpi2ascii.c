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

#include <dumpi/bin/dumpi2ascii-callbacks.h>
#include <dumpi/bin/dumpi2ascii-defs.h>
#include <dumpi/common/funcs.h>
#include <dumpi/common/perfctrs.h>
#include <dumpi/common/debugflags.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <assert.h>

extern int optind;
FILE *dumpfh = NULL;

/*
 * User options.
 */
typedef struct d2aopt {
  int verbose, help;
  int read_header, read_stream, read_keyval, read_footer, read_perf;
  int read_addresses, read_sizes;
  const char *file;
} d2aopt;

static int parse_options(int argc, char **argv, d2aopt *opt);
static void print_header(const dumpi_header *head);
static void print_keyval(const dumpi_keyval_record *kv);
static void print_footer(const dumpi_footer *foot);
static void print_perflbl(const dumpi_perfinfo *pinfo);
static void print_addresses(int count, const uint64_t *addresses,char **names);
static void print_sizes(const dumpi_sizeof *sizes);

int main(int argc, char **argv) {
  dumpi_profile *profile;
  libundumpi_callbacks cback;
  d2aopt opt;
  dumpfh = stdout;
  
  if(parse_options(argc, argv, &opt) == 0) {
    return 1;
  }
  libundumpi_clear_callbacks(&cback);
  set_callbacks(&cback);
  
  if((profile = undumpi_open(opt.file)) == NULL) {
    return 2;
  }
  if(opt.read_header) {
    dumpi_header *head = undumpi_read_header(profile);
    print_header(head);
    dumpi_free_header(head);
  }
  if(opt.read_stream) {
    int i;
    d2a_addr = (d2a_addrmap*)calloc(1, sizeof(d2a_addrmap));
    assert(d2a_addr != NULL);
    dumpi_read_function_addresses(profile, &(d2a_addr->count),
				  &(d2a_addr->address), &(d2a_addr->name));
    undumpi_read_stream(profile, &cback, DUMPI_UARG);
    for(i = 0; i < d2a_addr->count; ++i)
      free(d2a_addr->name[i]);
    free(d2a_addr->address);
    free(d2a_addr->name);
    free(d2a_addr);
    d2a_addr = NULL;
  }
  if(opt.read_keyval) {
    dumpi_keyval_record *kv = undumpi_read_keyval_record(profile);
    print_keyval(kv);
    dumpi_free_keyval_record(kv);
  }
  if(opt.read_footer) {
    dumpi_footer *foot = undumpi_read_footer(profile);
    print_footer(foot);
    dumpi_free_footer(foot);
  }
  if(opt.read_perf) {
    dumpi_perfinfo pinfo;
    dumpi_read_perfctr_labels(profile, &pinfo.count, pinfo.counter_tag,
			      DUMPI_MAX_PERFCTR_NAME);
    print_perflbl(&pinfo);
  }
  if(opt.read_addresses) {
    int i;
    int count = 0;
    uint64_t *addresses = NULL;
    char **names = NULL;
    dumpi_read_function_addresses(profile, &count, &addresses, &names);
    print_addresses(count, addresses, names);
    free(addresses);
    for(i = 0; i < count; ++i)
      free(names[i]);
    free(names);
  }
  if(opt.read_sizes) {
    dumpi_sizeof sizes;
    dumpi_read_datatype_sizes(profile, &sizes);
    print_sizes(&sizes);
    free(sizes.size);
  }

  return 0;
}

/*
  int verbose, help;
  int read_header, read_stream, read_keyval, read_footer;
  const char *file;
*/
int parse_options(int argc, char **argv, d2aopt *settings) {
  int opt;
  assert(settings != NULL);
  memset(settings, 0, sizeof(d2aopt));
  while((opt = getopt(argc, argv, "vhaHSKFPAXf:")) != -1) {
    switch(opt) {
    case 'v':
      if(settings->verbose) dumpi_debug = DUMPI_DEBUG_ALL;
      settings->verbose = 1;
      break;
    case '?':
    case 'h':
      settings->help = 1; break;
    case 'a':
      settings->read_header = settings->read_stream = 1;
      settings->read_keyval = settings->read_footer = 1;
      settings->read_perf = settings->read_addresses = 1;
      settings->read_sizes = 1;
      break;
    case 'H':
      settings->read_header = 1; break;
    case 'S':
      settings->read_stream = 1; break;
    case 'K':
      settings->read_keyval = 1; break;
    case 'F':
      settings->read_footer = 1; break;
    case 'P':
      settings->read_perf = 1;   break;
    case 'A':
      settings->read_addresses = 1; break;
    case 'X':
      settings->read_sizes = 1; break;
    case 'f':
      settings->file = strdup(optarg);
      break;
    default:
      fprintf(stderr, "Invalid argument %c.\n", opt);
      settings->help = 1;
      break;
    }
    if(settings->help) {
      fprintf(stderr, 
	      "Usage:  %s [-h] [-v] [-HSKF] [-f] filename\n"
	      "   Options:\n"
	      "        -h               Print this help\n"
	      "        -v               Verbose status output\n"
              "        -a               Equivalent to -HSKFPAX\n"
	      "        -H               Print header record\n"
	      "        -S               Print stream of MPI calls (default)\n"
	      "        -K               Print keyval record(s)\n"
	      "        -F               Print footer record\n"
	      "        -P               Print PAPI counter information\n"
	      "        -A               Print function address labels\n"
	      "        -X               Print type sizes\n"
	      "        -f  filename     Read the given binary tracefile\n",
	      argv[0]);
      break;
    }
  }
  if(settings->file == NULL && optind < argc)
    settings->file = argv[optind];
  if((settings->read_header + settings->read_stream + settings->read_keyval +
      settings->read_footer + settings->read_perf + settings->read_addresses +
      settings->read_sizes) == 0)
    settings->read_stream = 1;
  return (settings->help == 0);
}

void print_header(const dumpi_header *head) {
  /*
    char      version[3];
    uint64_t  starttime;
    char     *hostname;
    char     *username;
    int       meshdim;
    int      *meshcrd;
    int      *meshsize;
  */
  int i;
  time_t timev = (time_t)head->starttime;
  fprintf(dumpfh, "version=%d.%d.%d\n"
	  "starttime=%s"
	  "hostname=%s\n"
	  "username=%s\n"
	  "meshdim=%d\n",
	  head->version[0], head->version[1], head->version[2],
	  ctime(&timev), head->hostname, head->username, head->meshdim);
  fprintf(dumpfh, "meshsize=[");
  for(i = 0; i < head->meshdim; ++i) {
    fprintf(dumpfh, "%d", head->meshsize[i]);
    if(i < (head->meshdim-1)) fprintf(dumpfh, ", ");
  }
  fprintf(dumpfh, "]\n");
  fprintf(dumpfh, "meshcrd=[");
  for(i = 0; i < head->meshdim; ++i) {
    fprintf(dumpfh, "%d", head->meshcrd[i]);
    if(i < (head->meshdim-1)) fprintf(dumpfh, ", ");
  }
  fprintf(dumpfh, "]\n");
}

void print_keyval(const dumpi_keyval_record *kv) {
  dumpi_keyval_entry *curr;
  assert(kv != NULL);
  fprintf(dumpfh, "Total keyvals: %d\n", kv->count);
  curr = kv->head;
  while(curr) {
    fprintf(dumpfh, "%s=%s\n", curr->key, curr->val);
    curr = curr->next;
  }
}

void print_footer(const dumpi_footer *foot) {
  int i;
  assert(foot != NULL);
  for(i = 0; i <= DUMPI_ALL_FUNCTIONS; ++i) {
    fprintf(dumpfh, "%s called %d times and ignored %d times\n",
	    dumpi_function_names[i], foot->call_count[i],
	    foot->ignored_count[i]);
  }
}

void print_perflbl(const dumpi_perfinfo *pinfo) {
  int i;
  fprintf(dumpfh, "Performance counters: %d\n", pinfo->count);
  for(i = 0; i < pinfo->count; ++i)
    fprintf(dumpfh, "    %s\n", pinfo->counter_tag[i]);
}

void print_addresses(int count, const uint64_t *addresses, char **names)
{
  int i;
  for(i = 0; i < count; ++i) {
    fprintf(dumpfh, "Function address %llu has label %s\n", addresses[i], names[i]);
  }
}

void print_sizes(const dumpi_sizeof *sizes) {
  int i;
  for(i = 0; i < sizes->count; ++i)
    fprintf(dumpfh, "Datatype %d (%s) has size %d\n",
	   i, dumpi_get_datatype_name(i), sizes->size[i]);
}