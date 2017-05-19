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

#include <dumpi/common/io.h>
#include <dumpi/common/iodefs.h>
#include <dumpi/common/funcs.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <regex.h>
#include <getopt.h>
#include <limits.h>
#include <sys/types.h>
#include <time.h>

/* A simple conversion from ascii output from dumpi back to a binary trace */

/*
 * Recognized inputs:
 *
 * Header:
 *     version=\d+\.\d+\.\d+
 *     starttime=<string>
 *     hostname=<string>
 *     username=<string>
 *     meshdim=\d+
 *     meshsize=<array of length meshdim or empty block>
 *     meshcrd=<array of length meshdim or empty block>
 *
 * Stream:
 *     .*? entering at walltime \d+\.\d+, cputime \d+\.\d+ seconds in thread \d+
 *     (Perfcounters: [(.*?=\d+, )*(.*?=\d+)?])?
 *     ... # flags specific to each function type
 *     \1 returning at walltime \d+\.\d+, cputime \d+\.\d+ seconds in thread \d+
 *     (Perfcounters: [(.*?=\d+, )*(.*?=\d+)?])?
 *
 * Keyvals:
 *     Total keyvals: \d+
 *     key=value   # for each keyval
 *
 * Function counts:
 *     .*? called \d+ times and ignored \d+ times  # for each function name
 *
 * Perfcounters:
 *     Performance counters: \d+
 *     countertag    # for each performance counter
 *
 * Address labels:
 *     Function address \d+ has label <string>    # for each address label
 *
 * Datatype sizes:
 *     Datatype \d+ \(.*?\) has size \d+  # for each datatype
 */

static struct option options[] = {
  {"help", no_argument, NULL, 'h'},
  {"verbose", no_argument, NULL, 'v'},
  {"out", required_argument, NULL, 'o'},
  {NULL, 0, NULL, '\0'}
};

typedef struct kvpair {
  const char *key;
  const char *value;
} kvpair;

typedef struct settings {
  int help;
  int verbose;
  char *out;
  int infiles;
  char **in;
  /* This is the output profile.  It can only be "formally" initialized
   * once we know the time bias */
  dumpi_profile *profile;
  dumpi_outputs outputs;
  char counter_tag[DUMPI_MAX_PERFCTRS][DUMPI_MAX_PERFCTR_NAME];
  dumpi_header header;
  dumpi_footer footer;
  dumpi_keyval_record keyval;
  dumpi_sizeof typesize;
  int labels;
  uint64_t *label_value;
  char **label_name;
  /*
  int active_perfctrs;
  dumpi_perflabel_t *perfctr_labels;
  */
  /* Stash info about the input file that may conflict with the output. */
  int input_version[3];
} settings;

/* Unfortunately, ungetc only guarantees a single character push-back
 * (we really need a one-line push-back).  Fix this by providing a wrapper
 * around the file and stashing a single line as needed.
 * Wrapper returns NULL when input file is depleted. */
typedef struct fwrapper {
  FILE *fp;
  char *buff;
  size_t buffsize;
  int hot;
  long lineno;
  const char *fname;
} fwrapper;
static inline fwrapper wrap_create(FILE *fp, const char *fname) {
  fwrapper w = {fp, NULL, 0, 0, 0, fname};
  return w;
}
static inline void wrap_free(fwrapper *wrap) {
  free(wrap->buff);
  wrap->buff = NULL;
  wrap->buffsize = 0;
  wrap->lineno = 0;
  wrap->fname = NULL;
}
static inline char* wrap_getline(fwrapper *wrap) {
  /* This is easy if we already had an ungetline call */
  ++wrap->lineno;
  if(wrap->hot) {
    wrap->hot = 0;
    return wrap->buff;
  }
  /* Ensure we have a buffer */
  if(! wrap->buff) {
    wrap->buffsize = 8192;
    assert((wrap->buff = (char*)calloc(wrap->buffsize, sizeof(char))) != NULL);
  }
  /* Grab next string.  Dynamically reallocate the buffer if needed */
  if(fgets(wrap->buff, wrap->buffsize, wrap->fp) == NULL)
    return NULL;
  /*
    fprintf(stderr, "Initial read: %s\n", wrap->buff);
    fprintf(stderr, "strlen(wrap->buff)=%d, wrap->buffsize-1=%d\n", strlen(wrap->buff), wrap->buffsize-1);
  */
  while(strlen(wrap->buff) >= (wrap->buffsize-1) && !(wrap->buff[wrap->buffsize-2]=='\n' || feof(wrap->fp) || ferror(wrap->fp))) {
    assert((wrap->buff =
            (char*)realloc(wrap->buff, 2*wrap->buffsize*sizeof(char))) != NULL);
    fgets(wrap->buff + wrap->buffsize - 1, wrap->buffsize+1, wrap->fp); 
    wrap->buff[2*wrap->buffsize-1] = '\0';
    wrap->buffsize *= 2;
    /*
      fprintf(stderr, "Increased read starting at %d: %s has length %d\n", wrap->buffsize-1, wrap->buff, strlen(wrap->buff));
    */
    /*
      fprintf(stderr, "Updated strlen(wrap->buff)=%d, wrap->buffsize-1=%d\n", strlen(wrap->buff), wrap->buffsize-1);
    */
  }
  /*fprintf(stderr, "Read: %s of length %lu\n", wrap->buff, strlen(wrap->buff));*/
  if(wrap->buff[strlen(wrap->buff)-1] == '\n')
    wrap->buff[strlen(wrap->buff)-1] = '\0';
  return wrap->buff;
}

static inline void wrap_ungetline(fwrapper *wrap) {
  --wrap->lineno;
  wrap->hot = 1;
}

typedef void (*mpihandler)(fwrapper *wrapper, settings *opts);
mpihandler get_handler(const char *func, const fwrapper *wrap);

static void print_help();
static void parse_opts(int argc, char **argv, settings *opts);
static void parse_infile(const char *fname, FILE *fp, settings *opts);

int main(int argc, char **argv) {
  int i;
  struct settings opts;
  /* These have to be right to avoid pain and suffering */
  memset(&opts, 0, sizeof(struct settings));
  opts.header.version[0] = dumpi_version;
  opts.header.version[1] = dumpi_subversion;
  opts.header.version[2] = dumpi_subsubversion;
  parse_opts(argc, argv, &opts);
  if(opts.help) {
    print_help(argv[0]);
    return 1;
  }
  opts.outputs.timestamps = DUMPI_TIME_FULL;
  opts.outputs.statuses = DUMPI_ENABLE;
  opts.outputs.perfinfo = 0; /* until we find out whether it's there or not */
  for(i = 0; i <= DUMPI_ALL_FUNCTIONS; ++i)
    opts.outputs.function[i] = DUMPI_ENABLE;
  if(opts.infiles > 0) {
    int i;
    FILE *fp;
    for(i = 0; i < opts.infiles; ++i) {
      if((fp = fopen(opts.in[i], "r")) == NULL) {
        fprintf(stderr, "Failed to open \"%s\" for reading:  %s\n",
                opts.in[i], strerror(errno));
        return 3;
      }
      if(opts.verbose) fprintf(stderr, "Processing infile %s\n", opts.in[i]);
      parse_infile(opts.in[i], fp, &opts);
      fclose(fp);
    }
  }
  else {
    if(opts.verbose) fprintf(stderr, "Processing standard input\n");
    parse_infile("<stdin>", stdin, &opts);
  }
  /* Clean up. */
  if(opts.profile && opts.profile->file) {
    if(opts.verbose)
      fprintf(stderr, "Writing binary trace (DUMPI version %d.%d.%d)\n",
            (int)opts.header.version[0],
            (int)opts.header.version[1],
            (int)opts.header.version[2]);
    dumpi_write_header(opts.profile, &opts.header);
    dumpi_write_footer(opts.profile, &opts.footer);
    dumpi_write_keyval_record(opts.profile, &opts.keyval);
    dumpi_write_datatype_sizes(opts.profile, &opts.typesize);
    /*
      dumpi_write_perfctr_labels(opts.profile, opts.active_perfctrs,
      opts.perfctr_labels);
    */
    dumpi_write_datatype_sizes(opts.profile, &opts.typesize);;
    dumpi_write_function_addresses(opts.profile, opts.labels,
                                   opts.label_value, opts.label_name);
    dumpi_write_index(opts.profile);
    dumpi_free_output_profile(opts.profile);
  }
  else {
    fprintf(stderr, "Profile not opened (no stream data encountered). "
            "No output will be generated\n");
  }
  return 0;
}

void print_help(const char *name) {
  fprintf(stderr,
          "%s: Convert ASCII dumpi trace back to binary form\n"
          "    Usage: %s [-h] [-v] -o outfile [infile(s)]\n"
          "    If one or more infiles are given, all infiles will be parsed\n"
          "    to generate the single outfile.  If no infiles are given,\n"
          "    data will be read from standard input\n", name, name);
}

void parse_opts(int argc, char **argv, settings *opts) {
  int i, j, ch;
  char *shopts = NULL;
  for(i = 0; options[i].name != NULL; ++i) { /* just counting i */ }
  assert((shopts = (char*)calloc(2*i+1, sizeof(char))) != NULL);
  for(i = 0, j = 0; options[i].name != NULL; ++i, ++j) {
    shopts[j] = options[i].val;
    if(options[i].has_arg != no_argument) shopts[++j] = ':';
  }
  /* */
  opts->verbose = opts->help = opts->infiles = 0;
  opts->in = NULL;
  opts->out = NULL;
  while((ch = getopt_long(argc, argv, shopts, options, NULL)) != -1) {
    switch(ch) {
    case 'v': opts->verbose = 1; break;
    case 'h': opts->help = 1; break;
    case 'o': assert((opts->out = strdup(optarg)) != NULL); break;
    default:
      fprintf(stderr, "Invalid option %c\n", ch);
      exit(1);
    }
  }
  free(shopts);
  if(opts->help)
    return;
  /* We require the -o argument */
  if(opts->out == NULL) {
    fprintf(stderr, "ERROR:  The -o argument is not optional\n");
    exit(2);
  }
  /* Any remaining arguments are input files. */
  argc -= optind;
  argv += optind;
  opts->infiles = argc;
  if(opts->infiles) {
    assert((opts->in = (char**)calloc(opts->infiles, sizeof(char*))) != NULL);
    for(i = 0; i < opts->infiles; ++i)
      assert((opts->in[i] = strdup(argv[i])) != NULL);
  }
}

/* The regular expressions recognized as input keys. */
#define REHEAD_1 "version=[0-9]+\\.[0-9]+\\.[0-9]+)|starttime=.*|hostname=.*"
#define REHEAD_2 "username=.*|meshdim=[0-9]+|meshsize=(.*|\\[.*\\])|meshcrd=(.*|\\[.*\\]"
#define REHEADER "^(" REHEAD_1 "|" REHEAD_2 ")$"
#define RETIME "[0-9]+\\.[0-9]+"
#define RESTREAM_START "[^ ]+ entering at walltime " RETIME \
  ", cputime " RETIME " seconds in thread [0-9]+"
#define RESTREAM_STOP "[^ ]+ returning at walltime " RETIME \
  ", cputime " RETIME " seconds in thread [0-9]+"
#define RESTREAM_PERF "Perfcounters: [([^= ]+?=[0-9]+(, )?)*]"
#define REKEYVAL_START "^Total keyvals: [0-9]+$"
#define REFUNCCOUNT "[^ ]+ called [0-9]+ times and ignored [0-9]+ times"
#define REPERFCTR_START "Performance counters: [0-9]+"
#define READDRLABEL "Function adress [0-9]+ has label"
#define REDATATYPE "Datatype [0-9]+ (\\([-A-Za-z0-9_]+\\) )?has size [0-9]+"

typedef struct matchmap {
  const char *rexp;
  regex_t *preg;
  int (*handler)(fwrapper *wrapper, settings *opts);
} matchmap;

static int parse_header_entry(fwrapper *wrapper, settings *opts);
static int start_stream_entry(fwrapper *wrapper, settings *opts);
static int parse_keyval_entries(fwrapper *wrapper, settings *opts);
static int add_function_entry(fwrapper *wrapper, settings *opts);
static int parse_perfctr_entries(fwrapper *wrapper, settings *opts);
static int add_label_entry(fwrapper *wrapper, settings *opts);
static int add_datatype_size_entry(fwrapper *wrapper, settings *opts);

static matchmap matches[] = {
  {REHEADER, NULL, parse_header_entry},
  {RESTREAM_START, NULL, start_stream_entry},
  {REKEYVAL_START, NULL, parse_keyval_entries},
  {REFUNCCOUNT, NULL, add_function_entry},
  {REPERFCTR_START, NULL, parse_perfctr_entries},
  {READDRLABEL, NULL, add_label_entry},
  {REDATATYPE, NULL, add_datatype_size_entry},
  {NULL, NULL, NULL}
};

static inline regex_t* precompile_regex(const char *rexp) {
  int rv;
  regex_t *preg;
  assert((preg = (regex_t*)calloc(1, sizeof(regex_t))) != NULL);
  if((rv = regcomp(preg, rexp, REG_EXTENDED|REG_NOSUB)) != 0) {
    fprintf(stderr, "Failed to compile regex \"%s\":  error=%d\n", rexp, rv);
    abort();
  }
  return preg;
}

void parse_infile(const char *fname, FILE *fp, settings *opts) {
  int i;
  fwrapper wrap = wrap_create(fp, fname);
  /* Precompile regexes on first pass */
  for(i = 0; matches[i].rexp != NULL; ++i) {
    if(matches[i].preg == NULL) {
      matches[i].preg = precompile_regex(matches[i].rexp);
    }
  }
  while(! (feof(fp) || ferror(fp))) {
    int matched = 0;
    const char *buff = wrap_getline(&wrap);
    if(! buff) break; /* All done */
    for(i = 0; matches[i].rexp != NULL; ++i) {
      if(regexec(matches[i].preg, buff, 0, NULL, 0) == 0) {
        wrap_ungetline(&wrap);
        /*fprintf(stderr, "Match:\n    %s\nmatches\n    %s\n\n", buff, matches[i].rexp);*/
        matches[i].handler(&wrap, opts);
        matched = 1;
        break;
      }
    }
    if(! matched) {
      fprintf(stderr, "No match for \"%s\" at %s:%ld\n",
              buff, wrap.fname, wrap.lineno);
    }
  }
}

static inline long long to_int(const char *ent, const char *caller,
                          const fwrapper *wrap)
{
  char *end;
  long long val = strtoll(ent, &end, 10);
  if((*ent == '\0') || (*end != '\0')) {
    fprintf(stderr, "%s:  Invalid integer value \"%s\" at %s:%ld\n",
            caller, ent, wrap->fname, wrap->lineno);
    abort();
  }
  return val;
}

void copymatch(char *scratch, const char *ivalue, regmatch_t *match) {
  int len = match->rm_eo - match->rm_so;
  strncpy(scratch, ivalue + match->rm_so, len);
  scratch[len] = '\0';
}

static inline void save_old_version(char *value, settings *opts,
                                    const fwrapper *wrap)
{
  int depth = 0;
  char *ent, *sav;
  /*fprintf(stderr, "save_old_version(%s, %p)\n", value, opts);*/
  for(ent = strtok_r(value, ".", &sav); ent; ent = strtok_r(NULL, ".", &sav)) {
    int vers = to_int(ent, "version numbering", wrap);
    if(depth > 2) break;
    opts->input_version[depth] = vers;
    ++depth;
  }
  if(depth != 3) {
    fprintf(stderr, "Truncated version information -- depth=%d\n", depth);
    abort();
  }
}

static inline void save_old_starttime(char *value, settings *opts,
                                      const fwrapper *wrap)
{
  /* strptime converts ascii string to struct tm */
  /* mktime converts struct tm to time_t */
  struct tm tmo;
  time_t tout;
  strptime(value, "%a %b %d %T %Z %Y", &tmo);
  tout = mktime(&tmo);
  if(tout <= 0) {
    strptime(value, "%a %b %d %T %Y", &tmo);
    tout = mktime(&tmo);
  }
  if(tout <= 0) {
    fprintf(stderr, "Invalid time value \"%s\" at %s:%ld\n",
            value, wrap->fname, wrap->lineno);
    abort();
  }
  opts->header.starttime = tout;
}

static inline void save_old_hostname(char *value, settings *opts) {
  opts->header.hostname = strdup(value);
}

static inline void save_old_username(char *value, settings *opts) {
  opts->header.username = strdup(value);
}

static inline void save_old_meshdim(char *value, settings *opts,
                                    const fwrapper *wrap)
{
  int md;
  /*fprintf(stderr, "save_old_meshdim: \"%s\"\n", value);*/
  md = to_int(value, "ascii2dumpi: mesh dimension", wrap);
  opts->header.meshdim = md;
}

static inline void save_old_meshsize(char *value, settings *opts,
                                     const fwrapper *wrap)
{
  int count;
  char *tok, *end;
  int *mesh = NULL;
  if(value[0] != '[' || value[strlen(value)-1] != ']') {
    fprintf(stderr, "Mesh dimension not terminated by branchets (%s)\n", value);
    abort();
  }
  ++value;
  value[strlen(value)-1] = '\0';
  count = 0;
  if(opts->header.meshdim > 0)
    assert((mesh = (int*)calloc(opts->header.meshdim, sizeof(int))) != NULL);
  opts->header.meshsize = mesh;
  for(tok = strtok_r(value, ", ", &end); tok; tok = strtok_r(NULL, ", ", &end)){
    int ent = to_int(tok, "ascii2dumpi: mesh size", wrap);
    /*fprintf(stderr, "Mesh size entry: %s\n", tok);*/
    if(count >= opts->header.meshdim) {
      fprintf(stderr, "save_old_meshsize:  Input meshdim is %d, but at least "
              "%d entries in mesh size array\n", opts->header.meshdim, count+1);
      abort();
    }
    opts->header.meshsize[count] = ent;
    ++count;
  }
  if(count < opts->header.meshdim) {
      fprintf(stderr, "save_old_meshsize:  Input meshdim is %d, but only "
              "%d entries in mesh size array\n", opts->header.meshdim, count+1);
      abort();
  }
}

static inline void save_old_meshcrd(char *value, settings *opts,
                                    const fwrapper *wrap) {
  int count;
  char *tok, *end;
  int *mesh = NULL;
  if(value[0] != '[' || value[strlen(value)-1] != ']') {
    fprintf(stderr, "Mesh dimension not terminated by branchets (%s)\n", value);
    abort();
  }
  ++value;
  value[strlen(value)-1] = '\0';
  count = 0;
  if(opts->header.meshdim > 0)
    assert((mesh = (int*)calloc(opts->header.meshdim, sizeof(int))) != NULL);
  opts->header.meshcrd = mesh;
  for(tok = strtok_r(value, ", ", &end); tok; tok = strtok_r(NULL, ", ", &end)){
    int ent = to_int(tok, "ascii2dumpi: mesh size", wrap);
    /*fprintf(stderr, "Mesh coordinate entry: %s\n", tok);*/
    if(count >= opts->header.meshdim) {
      fprintf(stderr, "save_old_meshsize:  Input meshdim is %d, but at least "
              "%d entries in mesh size array\n", opts->header.meshdim, count+1);
      abort();
    }
    opts->header.meshcrd[count] = ent;
    ++count;
  }
  if(count < opts->header.meshdim) {
      fprintf(stderr, "save_old_meshsize:  Input meshdim is %d, but only "
              "%d entries in mesh size array\n", opts->header.meshdim, count+1);
      abort();
  }
}

int parse_header_entry(fwrapper *wrapper, settings *opts) {
  char *key, *val;
  key = wrap_getline(wrapper);
  val = strchr(key, '=');
  if((! val) || (*val == '\0')) {
    fprintf(stderr, "parse_header_entry:  Not a valid key=value at %s%ld\n", 
            wrapper->fname, wrapper->lineno);
    abort();
  }
  *val = '\0';
  ++val;
  /*
  fprintf(stderr, "parse_header_entry:  key=\"%s\" ; value=\"%s\"\n", key, val);
  */
  if(strcmp(key, "version") == 0) save_old_version(val, opts, wrapper);
  else if(strcmp(key, "starttime") == 0) save_old_starttime(val, opts, wrapper);
  else if(strcmp(key, "hostname") == 0) save_old_hostname(val, opts);
  else if(strcmp(key, "username") == 0) save_old_username(val, opts);
  else if(strcmp(key, "meshdim") == 0) save_old_meshdim(val, opts, wrapper);
  else if(strcmp(key, "meshsize") == 0) save_old_meshsize(val, opts, wrapper);
  else if(strcmp(key, "meshcrd") == 0) save_old_meshcrd(val, opts, wrapper);
  else fprintf(stderr, "WARNING: parse_header_entry:  Unhandled key %s\n", key);
  return 1;
}

int start_stream_entry(fwrapper *wrapper, settings *opts) {
  mpihandler handler = NULL;
  char *buf, *tok;
  buf = strdup(wrap_getline(wrapper));
  wrap_ungetline(wrapper);
  tok = strtok(buf, " ");
  handler = get_handler(tok, wrapper);
  handler(wrapper, opts);
  free(buf);
  return 1;
}

int parse_keyval_entries(fwrapper *wrapper, settings *opts) {
  int i;
  char *buf, *totv, *key, *val;
  buf = wrap_getline(wrapper);
  /* first line is 'Total keyvals: [0-9]+' */
  totv = strchr(buf, ':');
  if(! totv) {
    fprintf(stderr, "parse_keyval_entries:  Invalid header line \"%s\" at %s:%ld\n", buf, wrapper->fname, wrapper->lineno);
    abort();
  }
  ++totv;
  opts->keyval.count = to_int(totv, "parse_keyval_entries", wrapper);
  for(i = 0; i < opts->keyval.count; ++i) {
    dumpi_keyval_entry *kv =
      (dumpi_keyval_entry*)calloc(1, sizeof(dumpi_keyval_entry));
    assert(kv != NULL);
    if(! opts->keyval.head) opts->keyval.head = kv;
    if(opts->keyval.tail) opts->keyval.tail->next = kv;
    opts->keyval.tail = kv;
    /* subsequent lines are key=value (spaces are significant) */
    buf = wrap_getline(wrapper);
    key = strtok(buf, "=");
    val = strtok(NULL, "= ");
    if((! key) || (! val)) {
      fprintf(stderr, "parse_keyval_entries:  Invalid key=val "
              "at %s:%ld\n", wrapper->fname, wrapper->lineno);
      abort();
    }
    kv->key = strdup(key);
    kv->val = strdup(val);
  }
  return 1;
}

typedef struct functag {
  const char *name;
  int index;
} functag;
static functag *funcnames = NULL;
static int funccmp(const void *a, const void *b) {
  const functag *left = (functag*)a;
  const functag *right = (functag*)b;
  return strcmp(left->name, right->name);
}

static inline int find_funcname(const char *name, fwrapper *wrapper) {
  int pivot, old_pivot, lower_bound, upper_bound;
  if(! funcnames) {
    int i;
    assert((funcnames =
            (functag*)calloc(DUMPI_END_OF_STREAM+1, sizeof(functag))) != NULL);
    for(i = 0; i < DUMPI_END_OF_STREAM; ++i) {
      funcnames[i].name = dumpi_function_names[i];
      funcnames[i].index = i;
    }
    qsort(funcnames, DUMPI_END_OF_STREAM, sizeof(functag), funccmp);
  }
  /* Bisect search to find the right element. */
  lower_bound = 0;
  upper_bound = DUMPI_END_OF_STREAM;
  pivot = upper_bound / 2;
  old_pivot = 0;
  while(1) {
    int cmp = strcmp(name, funcnames[pivot].name);
    if(cmp == 0)
      break;
    else if(cmp < 0)
      upper_bound = pivot;
    else if(cmp > 0)
      lower_bound = pivot;
    pivot = lower_bound + (upper_bound - lower_bound) / 2;
    if(pivot == old_pivot) {
      fprintf(stderr, "find_funcname:  Failed to find \"%s\" at %s:%ld\n", 
              name, wrapper->fname, wrapper->lineno);
      abort();
    }
    old_pivot = pivot;
  }
  return funcnames[pivot].index;
}

int add_function_entry(fwrapper *wrapper, settings *opts) {
  int i, funcid;
  char *buf, *funcname, *called, *ignored;
  buf = wrap_getline(wrapper);
  funcname = strtok(buf, " "); /* MPI.* */
  strtok(NULL, " ");           /* called */
  called = strtok(NULL, " ");  /* [0-9]+ */
  for(i = 0; i < 3; ++i) strtok(NULL, " "); /* times and ignored */
  ignored = strtok(NULL, " "); /* [0-9]+ */
  funcid = find_funcname(funcname, wrapper);

    for(i = 0; i < DUMPI_END_OF_STREAM; ++i) {
    if(strcmp(dumpi_function_names[i], funcname) == 0)
      break;
  }
  if(i >= DUMPI_END_OF_STREAM) {
    fprintf(stderr, "add_function_entry:  Invalid name \"%s\" at %s:%ld\n", 
            funcname, wrapper->fname, wrapper->lineno);
    abort();
  }
  opts->footer.call_count[i] = to_int(called, "add_function_entry", wrapper);
  opts->footer.ignored_count[i] = to_int(ignored, "add_function_entry", wrapper);
  return 1;
}

static regex_t *perfctr_header_re = NULL;
static regmatch_t *perfctr_match = NULL;
int parse_perfctr_entries(fwrapper *wrapper, settings *opts) {
  /* Skipping perfctrs for now (running out of time). */
  int i, counters;
  char *buf = wrap_getline(wrapper);
  (void)wrapper;
  (void)opts;
  if(! perfctr_header_re) {
    assert((perfctr_header_re = (regex_t*)calloc(1, sizeof(regex_t))) != NULL);
    assert((perfctr_match = (regmatch_t*)calloc(2, sizeof(regmatch_t)))!=NULL);
    assert(regcomp(perfctr_header_re, "^Performance counters: ([0-9]+)$",
                   REG_EXTENDED) == 0);
  }
  if(regexec(perfctr_header_re, buf, 2, perfctr_match, 0) != 0) {
    fprintf(stderr, "parse_perfctr_entries:  Invalid header line \"%s\" at %s:%ld\n", buf, wrapper->fname, wrapper->lineno);
    abort();
  }
  buf[perfctr_match[1].rm_eo] = '\0';
  counters =
    to_int(buf + perfctr_match[1].rm_so, "parse_perfctr_entries", wrapper);
  for(i = 0; i < counters; ++i)
    wrap_getline(wrapper);
  return 1;
  /*
  int i;
  char *buf;
  if(! perfctr_header_re) {
    assert((perfctr_header_re = (regex_t*)calloc(1, sizeof(regex_t))) != NULL);
    assert((perfctr_entry_re = (regex_t*)calloc(1, sizeof(regex_t))) != NULL);
    assert((perfctr_match = (regmatch_t*)calloc(2, sizeof(regmatch_t)))!=NULL);
    assert(regcomp(perfctr_header_re, "^Performance counters: ([0-9]+)$",
                   REG_EXTENDED) == 0);
    assert(regcomp(perfctr_entry_re, "^ *([A-Z0-9_]+)$", REG_EXTENDED) == 0);
  }
  buf = wrap_getline(wrapper);
  if(regexec(perfctr_header_re, buf, 2, perfctr_match, 0) != 0) {
    fprintf(stderr, "parse_perfctr_entries:  Invalid header line \"%s\" at %s:%ld\n", buf, wrapper->fname, wrapper->lineno);
    abort();
  }
  buf[perfctr_match[1].rm_eo] = '\0';
  opts->active_perfctrs =
    to_int(buf + perfctr_match[1].rm_so, "parse_perfctr_entries", wrapper);
  if(opts->active_perfctrs > 0) 
    assert((opts->perfctr_labels =
            (dumpi_perflabel_t*)calloc(opts->active_perfctrs,
                                       sizeof(dumpi_perflabel_t))) != NULL);
  for(i = 0; i < opts->active_perfctrs; ++i) {
    if(regexec(perfctr_entry_re, buf, 3, perfctr_match, 0) != 0) {
      fprintf(stderr, "parse_perfctr_entries:  Invalid key=val \"%s\" at %s:%ld\n", buf, wrapper->fname, wrapper->lineno);
      abort();
    }
    buf[perfctr_match[1].rm_eo] = '\0';
    opts->input_perfctr[i] = to_int(buf + perfctr_match[1].rm_so,
                                    "parse_perfctr_entries", wrapper);
  }
  return 1;
  */
}

static regex_t *label_re = NULL;
static regmatch_t *label_match = NULL;
static const char *label_string = "Function address ([0-9]+) has label (.*)";
int add_label_entry(fwrapper *wrapper, settings *opts) {
  char *addr, *lbl;
  char *buf = wrap_getline(wrapper);
  if(! label_re) {
    assert((label_re = (regex_t*)calloc(1, sizeof(regex_t))) != NULL);
    assert((label_match = (regmatch_t*)calloc(3, sizeof(regmatch_t))) != NULL);
    assert(regcomp(label_re, label_string, REG_EXTENDED) == 0);
  }
  if(regexec(label_re, buf, 3, label_match, 0) != 0) {
    fprintf(stderr, "Failed to match \"%s\" with the regex \"%s\" at %s:%ld\n",
            buf, label_string, wrapper->fname, wrapper->lineno);
    abort();
  }
  buf[label_match[1].rm_eo] = '\0';
  buf[label_match[2].rm_eo] = '\0';
  addr = buf + label_match[1].rm_so;
  lbl = buf + label_match[2].rm_so;
  opts->label_value = (uint64_t*)realloc(opts->label_value,
                                         (opts->labels+1)*sizeof(uint64_t));
  opts->label_name = (char**)realloc(opts->label_name,
                                     (opts->labels+1)*sizeof(char*));
  opts->label_value[opts->labels] = to_int(addr, "add_label_entry", wrapper);
  opts->label_name[opts->labels] = strdup(lbl);
  ++opts->labels;
  return 1;
}

static regex_t *datatype_re = NULL;
static regmatch_t *datatype_match = NULL;
static const char *datatype_string = 
  "Datatype ([0-9]+) (\\([-A-Za-z0-9_]+\\) )?has size ([0-9]+)";
int add_datatype_size_entry(fwrapper *wrapper, settings *opts) {
  int index, size;
  char *buf = wrap_getline(wrapper);
  if(! datatype_re) {
    assert((datatype_re = (regex_t*)calloc(1, sizeof(regex_t))) != NULL);
    assert((datatype_match=(regmatch_t*)calloc(4, sizeof(regmatch_t))) != NULL);
    assert(regcomp(datatype_re, datatype_string, REG_EXTENDED) == 0);
  }
  if(regexec(datatype_re, buf, 4, datatype_match, 0) != 0) {
    fprintf(stderr, "add_label_entry: Failed to match \"%s\" with the "
            "regex \"%s\" at %s:%ld\n", buf, datatype_string,
            wrapper->fname, wrapper->lineno);
    abort();
  }
  buf[datatype_match[1].rm_eo] = '\0';
  buf[datatype_match[3].rm_eo] = '\0';
  index = to_int(buf + datatype_match[1].rm_so,
                 "add_datatype_size_entry", wrapper);
  size = to_int(buf + datatype_match[3].rm_so,
                "add_datatype_size_entry", wrapper);
  if(index < 0 || index > 100000) {
    fprintf(stderr, "Bogus (negative) datatype index at %s:%ld\n",
            wrapper->fname, wrapper->lineno);
    abort();
  }
  if(index >= opts->typesize.count) {
    int i, lasthead = opts->typesize.count;
    assert((opts->typesize.size =
            (int*)realloc(opts->typesize.size, (index + 1)*sizeof(int)))
           != NULL);
    for(i = lasthead; i < index; ++i) opts->typesize.size[i] = 0;
    opts->typesize.count = index + 1;
  }
  opts->typesize.size[index] = size;
  return 1;
}

static long read_int(const char *type, const char *name, fwrapper *wrapper) {
  char *buf, *itype, *iname, *ivalue;
  int retval;
  buf = wrap_getline(wrapper);
  itype = strtok(buf, " ");
  iname = strtok(NULL, "=");
  ivalue = strtok(NULL, "] ");
  if(ivalue[0] == '[') ivalue[0] = ' '; /* some entries are a little messed up */
  if(strcmp(type, itype) != 0) {
    fprintf(stderr, "Invalid type name at %s%ld: "
            "Expected \"%s\" but got \"%s\"\n",
            wrapper->fname, wrapper->lineno, type, itype);
    abort();
  }
  if(strcmp(name, iname) != 0) {
    fprintf(stderr, "Invalid variable name at %s%ld: "
            "Expected \"%s\" but got \"%s\"\n",
            wrapper->fname, wrapper->lineno, name, iname);
    abort();
  }
  if(! ivalue) {
    fprintf(stderr, "Insufficient tokens at %s%ld: "
            "Expected an integer value\n",
            wrapper->fname, wrapper->lineno);
    abort();
  }
  retval = to_int(ivalue, "read_int", wrapper);
  /*fprintf(stderr, "Now have %s %s=%d\n", itype, iname, retval);*/
  return retval;
}

int read_int16vec(int16_t **outvec, const char *type,
                  const char *name, fwrapper *wrapper)
{
  char *buf, *itype, *iname, *ilen, *rawvec, *entry;
  long arrlen, i;
  buf = wrap_getline(wrapper);
  itype = strtok(buf, " ");
  iname = strtok(NULL, "[");
  ilen = strtok(NULL, "]");
  if(strcmp(type, itype) != 0) {
    fprintf(stderr, "read_int16vec:  Expected type label \"%s\""
	    " but found \"%s\" at %s:%ld\n",
	    type, itype, wrapper->fname, wrapper->lineno);
    abort();
  }
  if(strcmp(name, iname) != 0) {
    fprintf(stderr, "read_int16vec:  Expected variable name \"%s\""
	    " but found \"%s\" at %s:%ld\n",
	    name, iname, wrapper->fname, wrapper->lineno);
    abort();
  }
  strtok(NULL, "[");
  rawvec = strtok(NULL, "]");
  arrlen = to_int(ilen, "read_int16vec", wrapper);
  if(arrlen > 0) assert((*outvec = (int16_t*)calloc(arrlen, sizeof(int16_t))) != NULL);
  else *outvec = NULL;
  for(i = 0; i < arrlen; ++i) {
    entry = strtok((i ? NULL : rawvec), ", ");
    if(! entry) {
      fprintf(stderr, "read_int16vec:  Insufficient vector length.  Expected %ld"
	      " entries but found %ld at %s:%ld\n", arrlen, i, wrapper->fname,
	      wrapper->lineno);
      abort();
    }
    (*outvec)[i] = to_int(entry, "read_intvec", wrapper);
  }
  /*
  fprintf(stderr, "Now have %s %s[%ld]=[", itype, iname, arrlen);
  for(i = 0; i < arrlen; ++i)
    fprintf(stderr, "%d%s", (*outvec)[i], (i < arrlen-1 ? ", " : ""));
  fprintf(stderr, "]\n");
  */
  return arrlen;
}

int read_intvec(int **outvec, const char *type,
                const char *name, fwrapper *wrapper)
{
  char *buf, *itype, *iname, *ilen, *rawvec, *entry;
  long arrlen, i;
  buf = wrap_getline(wrapper);
  itype = strtok(buf, " ");
  iname = strtok(NULL, "[");
  ilen = strtok(NULL, "]");
  if(strcmp(type, itype) != 0) {
    fprintf(stderr, "read_intvec:  Expected type label \"%s\""
	    " but found \"%s\" at %s:%ld\n",
	    type, itype, wrapper->fname, wrapper->lineno);
    abort();
  }
  if(strcmp(name, iname) != 0) {
    fprintf(stderr, "read_intvec:  Expected variable name \"%s\""
	    " but found \"%s\" at %s:%ld\n",
	    name, iname, wrapper->fname, wrapper->lineno);
    abort();
  }
  strtok(NULL, "[");
  rawvec = strtok(NULL, "]");
  arrlen = to_int(ilen, "read_intvec", wrapper);
  if(arrlen > 0) assert((*outvec = (int*)calloc(arrlen, sizeof(int))) != NULL);
  else *outvec = NULL;
  for(i = 0; i < arrlen; ++i) {
    entry = strtok((i ? NULL : rawvec), ", ");
    if(! entry) {
      fprintf(stderr, "read_intvec:  Insufficient vector length.  Expected %ld"
	      " entries but found %ld at %s:%ld\n", arrlen, i, wrapper->fname,
	      wrapper->lineno);
      abort();
    }
    (*outvec)[i] = to_int(entry, "read_intvec", wrapper);
  }
  /*
  fprintf(stderr, "Now have %s %s[%ld]=[", itype, iname, arrlen);
  for(i = 0; i < arrlen; ++i)
    fprintf(stderr, "%d%s", (*outvec)[i], (i < arrlen-1 ? ", " : ""));
  fprintf(stderr, "]\n");
  */
  return arrlen;
}

static regex_t *status_re = NULL;
static regmatch_t *status_match = NULL;
static const char *status_pat =
  "("
    "<IGNORED>"
  "|"
    "\\{bytes=([0-9]+), cancelled=([0-9]+), source=([0-9]+), "
    "tag=([0-9]+), error=([0-9]+)\\}"
  ")";
inline static void init_status_re() {
  if(! status_re) {
    int rv;
    assert((status_re = (regex_t*)calloc(1, sizeof(regex_t))) != NULL);
    assert((status_match = (regmatch_t*)calloc(8, sizeof(regmatch_t))) != NULL);
    rv = regcomp(status_re, status_pat, REG_EXTENDED);
    if(rv != 0) {
      char buff[256]; buff[255] = '\0';
      regerror(rv, status_re, buff, 255);
      fprintf(stderr, "init_status_re: regcomp failed: %s\n", buff);
      abort();
    }
  }
}

dumpi_status* read_status(const char *name, fwrapper *wrapper) {
  int i;
  char *buf, *itype, *iname, *ivalue;
  char scratch[256];
  dumpi_status *retval = NULL;
  init_status_re();
  buf = wrap_getline(wrapper);
  itype = strtok(buf, " ");
  iname = strtok(NULL, "=");
  ivalue = strtok(NULL, "]");
  if(strcmp(itype, "MPI_Status") != 0) {
    fprintf(stderr, "read_status:  Invalid type \"%s\" declared at %s:%ld\n",
	    itype, wrapper->fname, wrapper->lineno);
    abort();
  }
  if(strcmp(iname, name) != 0) {
    fprintf(stderr, "read_status:  Invalid variable name at %s:%ld.  "
	    "Expected \"%s\" but got \"%s\"\n",
	    wrapper->fname, wrapper->lineno, name, iname);
    abort();
  }
  if((! ivalue) || (ivalue[0] == '\0')) {
    fprintf(stderr, "read_status:  Invalid blank status at %s:%ld\n",
	    wrapper->fname, wrapper->lineno);
    abort();
  }
  if(regexec(status_re, ivalue, 8, status_match, 0) != 0) {
    fprintf(stderr, "read_status:  Failed matching \"%s\" to the regex "
	    "\"%s\" at %s:%ld\n", ivalue, status_pat,
	    wrapper->fname, wrapper->lineno);
    abort();
  }
  for(i = 0; i < 8; ++i) {
    int len = status_match[i].rm_eo - status_match[i].rm_so;
    if(len > 255) len = 255;
    strncpy(scratch, ivalue+status_match[i].rm_so, len);
    scratch[len] = '\0';
  }
  if(strncmp(ivalue+status_match[1].rm_so, "<IGNORED>", 
	     status_match[1].rm_eo - status_match[1].rm_so) != 0)
    {
      assert((retval = (dumpi_status*)calloc(1, sizeof(dumpi_status))) != NULL);
      copymatch(scratch, ivalue, status_match+2);
      retval->bytes = to_int(scratch, "read_status", wrapper);
      copymatch(scratch, ivalue, status_match+3);
      retval->cancelled = to_int(scratch, "read_status", wrapper);
      copymatch(scratch, ivalue, status_match+4);
      retval->source = to_int(scratch, "read_status", wrapper);
      copymatch(scratch, ivalue, status_match+5);
      retval->tag = to_int(scratch, "read_status", wrapper);
      copymatch(scratch, ivalue, status_match+6);
      retval->error = to_int(scratch, "read_status", wrapper);
    }
  /*
  fprintf(stderr, "We now have MPI_Status %s=", iname);
  if(! retval) fprintf(stderr, "<IGNORED>\n");
  else fprintf(stderr, "{bytes=%d, cancelled=%d, source=%d, tag=%d, error=%d}\n",
	       retval->bytes, retval->cancelled, retval->source, retval->tag, retval->error);
  */
  return retval;
}

static regex_t *statuses_re = NULL;
static regmatch_t *statuses_match = NULL;
static const char *statuses_line =
  "MPI_Status ([A-Za-z][A-Za-z0-9_]*)\\[([0-9]+)\\]=(\\[(.*)\\]|<IGNORED>)";
static inline void init_statuses_re() {
  if(! statuses_re) {
    assert((statuses_re = (regex_t*)calloc(1, sizeof(regex_t))) != NULL);
    assert((statuses_match = (regmatch_t*)calloc(8, sizeof(regmatch_t))) != NULL);
    assert(regcomp(statuses_re, statuses_line, REG_EXTENDED) == 0);    
  }
}

dumpi_status* read_statuses(int *arrlen, const char *name, fwrapper *wrapper) {
  int i, ignored;
  char *buf;
  char scratch[256];
  dumpi_status *retval = NULL;
  buf = wrap_getline(wrapper);
  init_status_re();
  init_statuses_re();
  if(regexec(statuses_re, buf, 8, statuses_match, 0) != 0) {
    fprintf(stderr, "read_statuses: Failed matching \"%s\" to pattern \"%s\" "
	    "at %s:%ld.\n",
            buf, statuses_line, wrapper->fname, wrapper->lineno);
    if(regexec(status_re, buf, 0, NULL, 0) != 0) {
      abort();
    }
    else {
      fprintf(stderr, "  -- Input line matches a single-status pattern.  "
              "Creating a return result of MPI_STATUSES_IGNORE\n");
      *arrlen = 0;
      return NULL;
    }
  }
  /*
  for(i = 0; i < 8; ++i) {
    copymatch(scratch, buf, statuses_match+i);
    fprintf(stderr, "read_statuses: Match %d is \"%s\"\n", i, scratch);
  }
  */
  copymatch(scratch, buf, statuses_match+1);
  if(strcmp(scratch, name) != 0) {
    fprintf(stderr, "read_statuses: Expected variable named \"%s\" but found "
	    "\"%s\" at %s:%ld\n", name, scratch, wrapper->fname, wrapper->lineno);
    abort();
  }
  copymatch(scratch, buf, statuses_match+2);
  *arrlen = to_int(scratch, "read_statuses", wrapper);
  if(*arrlen < 0) {
    fprintf(stderr, "read_statuses:  Invalid (negative) array length at "
	    "%s:%ld\n", wrapper->fname, wrapper->lineno);
    abort();
  }
  ignored = (strncmp(buf+statuses_match[3].rm_so, "<IGNORED>", 
                     statuses_match[3].rm_eo - statuses_match[3].rm_so)
             == 0);
  if((*arrlen > 0) && (! ignored)) {
    /* Store a pointer to the start of the actual array */
    char *basepos = buf + statuses_match[4].rm_so;
    assert((retval = (dumpi_status*)calloc(*arrlen, sizeof(dumpi_status))) != NULL);
    for(i = 0; i < *arrlen; ++i) {
      if(regexec(status_re, basepos, 8, status_match, 0) != 0) {
	fprintf(stderr, "read_statuses:  Failed to extract status output at "
		"%s:%ld\n", wrapper->fname, wrapper->lineno);
	abort();
      }
      /* Reset base pointer to be unmatched part of regex */
      copymatch(scratch, basepos, status_match+2);
      retval[i].bytes = to_int(scratch, "read_statuses", wrapper);
      copymatch(scratch, basepos, status_match+3);
      retval[i].cancelled = to_int(scratch, "read_statuses", wrapper);
      copymatch(scratch, basepos, status_match+4);
      retval[i].source = to_int(scratch, "read_statuses", wrapper);
      copymatch(scratch, basepos, status_match+5);
      retval[i].tag = to_int(scratch, "read_statuses", wrapper);
      copymatch(scratch, basepos, status_match+6);
      retval[i].error = to_int(scratch, "read_statuses", wrapper);
      basepos = basepos + status_match[6].rm_eo;
      /*
      fprintf(stderr, "Status entry %d is {bytes=%d, cancelled=%d, source=%d, tag=%d, error=%d}\n",
	      i, retval[i].bytes, retval[i].cancelled, retval[i].source, retval[i].tag, retval[i].error);
      */
    }
  }
  return retval;
}

static char* read_string(const char *type, const char *name, fwrapper *wrapper)
{
  char *itype, *iname, *ivalue;
  char *buf = wrap_getline(wrapper);
  itype = strtok(buf, " ");
  iname = strtok(buf, " =\"");
  ivalue = strtok(buf, "\"");
  if((! itype) || (strcmp(itype, type) != 0)) {
    fprintf(stderr, "read_string: Invalid type declaration: Expected \"%s\" "
            "but received \"%s\" at %s:%ld\n", type, itype,
            wrapper->fname, wrapper->lineno);
    abort();
  }
  if((! iname) || (strcmp(iname, name) != 0)) {
    fprintf(stderr, "read_string: Invalid variable name: Expected \"%s\" "
            "but received \"%s\" at %s:%ld\n", name, iname,
            wrapper->fname, wrapper->lineno);
    abort();
  }
  if(! ivalue) {
    fprintf(stderr, "read_string: Invalid null value at %s:%ld\n",
            wrapper->fname, wrapper->lineno);
    abort();
  }
  return strdup(ivalue);
}

static char** read_stringarr(int *arrlen, const char *type, const char *name, fwrapper *wrapper) {
  int i;
  char *buf, *itype, *iname, *iarr;
  char **retval = NULL;
  buf = wrap_getline(wrapper);
  itype = strtok(buf, " ");
  if(strcmp(itype, type) != 0) {
    fprintf(stderr, "read_stringarr: Expected type label \"%s\" but got \"%s\" "
	    "at %s:%ld\n", type, itype, wrapper->fname, wrapper->lineno);
    abort();
  }
  iname = strtok(NULL, " [");
  if(strcmp(iname, name) != 0) {
    fprintf(stderr, "read_stringarr: Expected name label \"%s\" but got \"%s\" "
	    "at %s:%ld\n", name, iname, wrapper->fname, wrapper->lineno);
    abort();
  }  
  iarr = strtok(NULL, "]=[");
  if(iarr)
    *arrlen = to_int(iarr, "read_stringarr", wrapper);
  else {
    fprintf(stderr, "read_stringarr: Invalid array length at %s:%ld\n",
	    wrapper->fname, wrapper->lineno);
    abort();
  }
  if(*arrlen < 0) {
    fprintf(stderr, "read_stringarr: Negative array length at %s:%ld\n",
	    wrapper->fname, wrapper->lineno);
    abort();
  }
  if(*arrlen > 0) {
    char *tok = strtok(NULL, "]");
    if(! tok) {
      fprintf(stderr, "read_stringarr:  Failed to find end of array at %s:%ld\n",
	      wrapper->fname, wrapper->lineno);
      abort();
    }
    assert((retval = (char**)calloc(*arrlen+1, sizeof(char*))) != NULL);
    for(i = 0; i < *arrlen; ++i) {
      char *end;
      tok = strchr(tok, '"');
      if(! tok) {
	fprintf(stderr, "read_stringarr:  Failed to find opening quote"
		" at %s:%ld\n", wrapper->fname, wrapper->lineno);
	abort();
      }
      ++tok;
      end = strchr(tok, '"');
      if(! end) {
	fprintf(stderr, "read_stringarr:  Failed to find closing quote"
		" at %s:%ld\n", wrapper->fname, wrapper->lineno);
	abort();
      }
      *end = '\0';
      assert((retval[i] = strdup(tok)) != NULL);
      ++end;
      tok = end;
    }
  }
  /*
  fprintf(stderr, "read_stringarr:  Returning array of length %d: [", *arrlen);
  if(*arrlen > 0) {
    for(i = 0; i < *arrlen; ++i)
      fprintf(stderr, "\"%s\"%s", retval[i], ((i < *arrlen-1) ? ", " : ""));
  }
  fprintf(stderr, "]\n");
  */
  return retval;
}

/* The actual (messy) MPI function mappings start here */
/* MPI_.* (entering|returning) at walltime [0-9]+\.[0-9]+, cputime [0-9]+\.[0-9]+ seconds in thread [0-9]+ */
#define DUMPI_ENTERING(MPICALL, PRM, THREAD, CPU, WALL, PERF, OPTS)     \
  THREAD = dumpi_entering(#MPICALL, &CPU, &WALL, &PERF, wrapper, OPTS);

static uint16_t dumpi_entering(const char *mpicall, dumpi_time *cpu,
                               dumpi_time *wall, dumpi_perfinfo *perf,
                               fwrapper *wrap, settings *opts)
{
  int i;
  char *buf, *iname, *iwall_sec, *iwall_nsec, *icpu_sec, *icpu_nsec, *ithread;
  int thread;
  buf = wrap_getline(wrap);
  iname = strtok(buf, " ");                  /* MPI_.* */
  for(i = 0; i < 3; ++i) strtok(NULL, " ");  /* entering at walltime */
  iwall_sec = strtok(NULL, ".");             /* xxx */
  iwall_nsec = strtok(NULL, ", ");           /* yyy */
  strtok(NULL, " ");                         /* cputime */
  icpu_sec = strtok(NULL, ".");              /* xxx */
  icpu_nsec = strtok(NULL, ", ");            /* yyy */
  for(i = 0; i < 3; ++i) strtok(NULL, " ");  /* seconds in thread */
  ithread = strtok(NULL, ". ");              /* ttt */
  if(! (iname && iwall_sec && iwall_nsec && icpu_sec && icpu_nsec && ithread)) {
    fprintf(stderr, "iname=%s, iwall_sec=%s, iwall_nsec=%s, icpu_sec=%s, "
            "icpu_nsec=%s, ithread=%s\n", iname, iwall_sec, iwall_nsec,
            icpu_sec, icpu_nsec, ithread);
    fprintf(stderr, "report_%s Failed reading entry line at %s:%ld\n",
            mpicall, wrap->fname, wrap->lineno);
    abort();
  }
  if(strcmp(iname, mpicall) != 0) {
    fprintf(stderr, "report_%s: Incorrect MPI function name %s at "
            "%s:%ld\n", mpicall, iname, wrap->fname, wrap->lineno);
    abort();
  }
  wall->start.sec = to_int(iwall_sec, mpicall, wrap);
  wall->start.nsec = to_int(iwall_nsec, mpicall, wrap);
  cpu->start.sec = to_int(icpu_sec, mpicall, wrap);
  cpu->start.nsec = to_int(icpu_nsec, mpicall, wrap);
  thread = to_int(ithread, mpicall, wrap);
  if(thread < 0) {
    fprintf(stderr, "report_%s:  Invalid thread id %s at %s:%ld\n",
            mpicall, ithread, wrap->fname, wrap->lineno);
    abort();
  }
  /* If we get here, we have enough info to set up the profile. */
  if(! opts->profile) {
    int cs = cpu->start.sec, ws = wall->start.sec;
    assert((opts->profile = dumpi_alloc_output_profile(cs, ws, 4096)) != NULL);
    if((opts->profile->file = dumpi_open_output_file(opts->out)) == NULL) {
      fprintf(stderr, "Failed to create output file %s: %s\n",
              opts->out, strerror(errno));
      abort();
    }
  }
  /* Now check whether we have any performance counters. */
  buf = wrap_getline(wrap);
  if(strncmp(buf, "Perfcounters:", 12) == 0) {
    /* yes */
    int counterid = 0;
    char *ctrs, *tmp;
    ctrs = strchr(buf, '[');
    tmp = strchr(ctrs, ']');
    if(! (ctrs && tmp)) {
      fprintf(stderr, "Invalid perfcounter line \"%s\" at %s:%ld\n",
              buf, wrap->fname, wrap->lineno);
      abort();
    }
    ++ctrs;
    tmp = strchr(ctrs, ']');
    *tmp = '\0';
    for(tmp = strtok(ctrs, ", "); tmp; tmp = strtok(NULL, ", ")) {
      char *key, *val; 
      key = strtok(tmp, "= ");
      val = strtok(NULL, "= ");
      if(! (key && val)) {
        fprintf(stderr, "Invalid perfcounter entry at %s:%ld\n",
                wrap->fname, wrap->lineno);
        abort();
      }
      strncpy(perf->counter_tag[counterid], key, DUMPI_MAX_PERFCTR_NAME-1);
      perf->invalue[counterid] = to_int(val, mpicall, wrap);
      ++counterid;      
    }
  }
  else {
    wrap_ungetline(wrap);
  }
  return thread;
}
  
#define DUMPI_RETURNING(MPICALL, WRITE, PRM, THREAD, CPU, WALL, PERF, WRAPPER, OPTS) do { \
    dumpi_returning(#MPICALL, THREAD, &CPU, &WALL, &PERF, WRAPPER, OPTS); \
    WRITE(&PRM, THREAD, &CPU, &WALL, &PERF, &OPTS->outputs, OPTS->profile); \
  } while(0)

static uint16_t dumpi_returning(const char *mpicall, uint16_t enter_thread,
                                dumpi_time *cpu, dumpi_time *wall,
                                dumpi_perfinfo *perf, fwrapper *wrap,
                                settings *opts)
{
  int i;
  char *buf, *iname, *iwall_sec, *iwall_nsec, *icpu_sec, *icpu_nsec, *ithread;
  int thread;
  (void)opts;
  buf = wrap_getline(wrap);
  iname = strtok(buf, " ");                  /* MPI_.* */
  for(i = 0; i < 3; ++i) strtok(NULL, " ");  /* returning at walltime */
  iwall_sec = strtok(NULL, ".");             /* xxx */
  iwall_nsec = strtok(NULL, ", ");           /* yyy */
  strtok(NULL, " ");                         /* cputime */
  icpu_sec = strtok(NULL, ".");              /* xxx */
  icpu_nsec = strtok(NULL, ", ");            /* yyy */
  for(i = 0; i < 3; ++i) strtok(NULL, " ");  /* seconds in thread */
  ithread = strtok(NULL, ". ");              /* ttt */
  if(! (iname && iwall_sec && iwall_nsec && icpu_sec && icpu_nsec && ithread)) {
    fprintf(stderr, "report_%s Failed reading exit line at %s:%ld\n",
            mpicall, wrap->fname, wrap->lineno);
    abort();
  }
  if(strcmp(iname, mpicall) != 0) {
    fprintf(stderr, "report_%s: Incorrect MPI function name %s at "
            "%s:%ld\n", mpicall, iname, wrap->fname, wrap->lineno);
    abort();
  }
  wall->stop.sec = to_int(iwall_sec, mpicall, wrap);
  wall->stop.nsec = to_int(iwall_nsec, mpicall, wrap);
  cpu->stop.sec = to_int(icpu_sec, mpicall, wrap);
  cpu->stop.nsec = to_int(icpu_nsec, mpicall, wrap);
  thread = to_int(ithread, mpicall, wrap);
  if(thread != (int)enter_thread) {
    fprintf(stderr, "report_%s:  Changed thread id from %d to %d at %s:%ld\n",
            mpicall, enter_thread, thread, wrap->fname, wrap->lineno);
    abort();
  }
  /* Now check whether we have any performance counters. */
  buf = wrap_getline(wrap);
  if(buf) {
    if(strncmp(buf, "Perfcounters:", 12) == 0) {
      /* yes */
      int counterid = 0;
      char *ctrs, *tmp;
      ctrs = strchr(buf, '[');
      tmp = strchr(ctrs, ']');
      if(! (ctrs && tmp)) {
        fprintf(stderr, "Invalid perfcounter line \"%s\" at %s:%ld\n",
                buf, wrap->fname, wrap->lineno);
        abort();
      }
      ++ctrs;
      tmp = strchr(ctrs, ']');
      *tmp = '\0';
      for(tmp = strtok(ctrs, ", "); tmp; tmp = strtok(NULL, ", ")) {
        char *key, *val; 
        key = strtok(tmp, "= ");
        val = strtok(NULL, "= ");
        if(! (key && val)) {
          fprintf(stderr, "Invalid perfcounter entry at %s:%ld\n",
                  wrap->fname, wrap->lineno);
          abort();
        }
        assert(strcmp(perf->counter_tag[counterid], key) == 0);
        perf->outvalue[counterid] = to_int(val, mpicall, wrap);
        ++counterid;      
      }
    }
    else {
      wrap_ungetline(wrap);
    }
  }
  else {
    wrap_ungetline(wrap);
  }
  return thread;
}

#define DUMPI_CONVERT_INT(NAME) do {            \
    prm.NAME = read_int("int", #NAME, wrapper); \
  } while(0)

#define DUMPI_CONVERT_INT64_T(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_DEST(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_SOURCE(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_TAG(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_COMPARISON(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_KEYVAL(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_TOPOLOGY(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_ERRHANDLER(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_WIN(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_WIN_ASSERT(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_LOCKTYPE(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_COMM_KEYVAL(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_THREADLEVEL(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_TYPE_KEYVAL(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_WIN_KEYVAL(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_COMBINER(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_TYPECLASS(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_ORDERING(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_FILEMODE(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_WHENCE(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_WHENCE(NAME) DUMPI_CONVERT_INT(NAME)
#define DUMPI_CONVERT_FUNC(NAME) DUMPI_CONVERT_INT(NAME)

#define DUMPI_CONVERT_DATATYPE(NAME) do {                \
    prm.NAME = read_int("MPI_Datatype", #NAME, wrapper); \
  } while(0)

#define DUMPI_CONVERT_OP(NAME) do {                \
    prm.NAME = read_int("MPI_Op", #NAME, wrapper); \
  } while(0)

#define DUMPI_CONVERT_COMM(NAME) do {                   \
    prm.NAME = read_int("MPI_Comm", #NAME, wrapper);    \
  } while(0)

#define DUMPI_CONVERT_GROUP(NAME) do {                   \
    prm.NAME = read_int("MPI_Group", #NAME, wrapper);    \
  } while(0)

#define DUMPI_CONVERT_STATUS_PTR(NAME) do {     \
    prm.NAME = read_status(#NAME, wrapper);     \
  } while(0)

#define DUMPI_CONVERT_REQUEST(NAME) do {                \
    prm.NAME = read_int("MPI_Request", #NAME, wrapper); \
  } while(0)

#define DUMPIO_CONVERT_REQUEST(NAME) do {                \
    prm.NAME = read_int("MPIO_Request", #NAME, wrapper); \
  } while(0)

#define DUMPI_CONVERT_INFO(NAME) do {                \
    prm.NAME = read_int("MPI_Info", #NAME, wrapper); \
  } while(0)

#define DUMPI_CONVERT_FILE(NAME) do {                \
    prm.NAME = read_int("MPI_File", #NAME, wrapper); \
  } while(0)

#define DUMPI_CONVERT_INT_ARRAY_1(COUNT, NAME) do {             \
    prm.COUNT = read_intvec(&prm.NAME, "int", #NAME, wrapper);  \
  } while(0)

#define DUMPI_CONVERT_INT16_ARRAY_1(COUNT, NAME) do {                 \
    prm.COUNT = read_int16vec(&prm.NAME, "int", #NAME, wrapper);      \
  } while(0)

#define DUMPI_CONVERT_DISTRIBUTION_ARRAY_1(COUNT, NAME) \
  DUMPI_CONVERT_INT16_ARRAY_1(COUNT, NAME)

#define DUMPI_CONVERT_REQUEST_ARRAY_1(COUNT, NAME) do {                 \
    prm.COUNT = read_intvec(&prm.NAME, "MPI_Request", #NAME, wrapper);  \
  } while(0)

#define DUMPIO_CONVERT_REQUEST_ARRAY_1(COUNT, NAME) do {                 \
    prm.COUNT = read_intvec(&prm.NAME, "MPIO_Request", #NAME, wrapper);  \
  } while(0)

#define DUMPI_CONVERT_DATATYPE_ARRAY_1(COUNT, NAME) do {                \
    prm.COUNT = read_int16vec(&prm.NAME, "MPI_Datatype", #NAME, wrapper); \
  } while(0)

#define DUMPI_CONVERT_STATUS_ARRAY_1(COUNT, NAME) do {     \
    prm.NAME = read_statuses(&prm.COUNT, #NAME, wrapper);  \
  } while(0)

#define DUMPI_CONVERT_CHAR_ARRAY_1(NAME) do {    \
    prm.NAME = read_string("string", #NAME, wrapper);     \
  } while(0)

#define DUMPI_CONVERT_CHAR_ARRAY_2(COUNT, NAME) do {                    \
    prm.NAME = read_stringarr(&prm.COUNT, "string", #NAME, wrapper);       \
  } while(0)

#define PREAMBLE(DUMPITYPE)                     \
  DUMPITYPE prm;                                \
  uint16_t thread;                              \
  dumpi_time cpu, wall;                         \
  dumpi_perfinfo perf;                          \
  memset(&prm, 0, sizeof(DUMPITYPE))

void report_MPI_Send(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_send);
  DUMPI_ENTERING(MPI_Send, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Send, dumpi_write_send, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Recv(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_recv);
  DUMPI_ENTERING(MPI_Recv, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_SOURCE(source);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Recv, dumpi_write_recv, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Get_count(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_get_count);
  DUMPI_ENTERING(MPI_Get_count, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(count);
  DUMPI_RETURNING(MPI_Get_count, dumpi_write_get_count, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Bsend(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_bsend);
  DUMPI_ENTERING(MPI_Bsend, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Bsend, dumpi_write_bsend, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Ssend(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_ssend);
  DUMPI_ENTERING(MPI_Ssend, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Ssend, dumpi_write_ssend, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Rsend(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_rsend);
  DUMPI_ENTERING(MPI_Rsend, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Rsend, dumpi_write_rsend, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Buffer_attach(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_buffer_attach);
  DUMPI_ENTERING(MPI_Buffer_attach, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(size);
  DUMPI_RETURNING(MPI_Buffer_attach, dumpi_write_buffer_attach, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Buffer_detach(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_buffer_detach);
  DUMPI_ENTERING(MPI_Buffer_detach, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(size);
  DUMPI_RETURNING(MPI_Buffer_detach, dumpi_write_buffer_detach, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Isend(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_isend);
  DUMPI_ENTERING(MPI_Isend, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_INT(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Isend, dumpi_write_isend, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Ibsend(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_ibsend);
  DUMPI_ENTERING(MPI_Ibsend, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Ibsend, dumpi_write_ibsend, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Issend(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_issend);
  DUMPI_ENTERING(MPI_Issend, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Issend, dumpi_write_issend, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Irsend(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_irsend);
  DUMPI_ENTERING(MPI_Irsend, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Irsend, dumpi_write_irsend, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Irecv(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_irecv);
  DUMPI_ENTERING(MPI_Irecv, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_SOURCE(source);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Irecv, dumpi_write_irecv, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Wait(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_wait);
  DUMPI_ENTERING(MPI_Wait, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Wait, dumpi_write_wait, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Test(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_test);
  DUMPI_ENTERING(MPI_Test, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_CONVERT_INT(flag);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Test, dumpi_write_test, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Request_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_request_free);
  DUMPI_ENTERING(MPI_Request_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Request_free, dumpi_write_request_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Waitany(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_waitany);
  DUMPI_ENTERING(MPI_Waitany, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(index);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Waitany, dumpi_write_waitany, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.status) free(prm.status);
}

void report_MPI_Testany(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_testany);
  DUMPI_ENTERING(MPI_Testany, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(index);
  DUMPI_CONVERT_INT(flag);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Testany, dumpi_write_testany, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.status) free(prm.status);
}

void report_MPI_Waitall(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_waitall);
  DUMPI_ENTERING(MPI_Waitall, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_STATUS_ARRAY_1(count, statuses);
  DUMPI_RETURNING(MPI_Waitall, dumpi_write_waitall, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.statuses) free(prm.statuses);
}

void report_MPI_Testall(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_testall);
  DUMPI_ENTERING(MPI_Testall, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(flag);
  DUMPI_CONVERT_STATUS_ARRAY_1(count, statuses);
  DUMPI_RETURNING(MPI_Testall, dumpi_write_testall, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.statuses) free(prm.statuses);
}

void report_MPI_Waitsome(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_waitsome);
  DUMPI_ENTERING(MPI_Waitsome, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(outcount);
  DUMPI_CONVERT_INT_ARRAY_1(outcount, indices);
  DUMPI_CONVERT_STATUS_ARRAY_1(outcount, statuses);
  DUMPI_RETURNING(MPI_Waitsome, dumpi_write_waitsome, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.indices) free(prm.indices);
  if(prm.statuses) free(prm.statuses);
}

void report_MPI_Testsome(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_testsome);
  DUMPI_ENTERING(MPI_Testsome, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(outcount);
  DUMPI_CONVERT_INT_ARRAY_1(outcount, indices);
  DUMPI_CONVERT_STATUS_ARRAY_1(outcount, statuses);
  DUMPI_RETURNING(MPI_Testsome, dumpi_write_testsome, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.indices) free(prm.indices);
  if(prm.statuses) free(prm.statuses);
}

void report_MPI_Iprobe(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_iprobe);
  DUMPI_ENTERING(MPI_Iprobe, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_SOURCE(source);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(flag);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Iprobe, dumpi_write_iprobe, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Probe(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_probe);
  DUMPI_ENTERING(MPI_Probe, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_SOURCE(source);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Probe, dumpi_write_probe, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Cancel(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cancel);
  DUMPI_ENTERING(MPI_Cancel, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Cancel, dumpi_write_cancel, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Test_cancelled(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_test_cancelled);
  DUMPI_ENTERING(MPI_Test_cancelled, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_CONVERT_INT(cancelled);
  DUMPI_RETURNING(MPI_Test_cancelled, dumpi_write_test_cancelled, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Send_init(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_send_init);
  DUMPI_ENTERING(MPI_Send_init, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Send_init, dumpi_write_send_init, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Bsend_init(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_bsend_init);
  DUMPI_ENTERING(MPI_Bsend_init, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Bsend_init, dumpi_write_bsend_init, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Ssend_init(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_ssend_init);
  DUMPI_ENTERING(MPI_Ssend_init, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Ssend_init, dumpi_write_ssend_init, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Rsend_init(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_rsend_init);
  DUMPI_ENTERING(MPI_Rsend_init, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Rsend_init, dumpi_write_rsend_init, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Recv_init(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_recv_init);
  DUMPI_ENTERING(MPI_Recv_init, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_SOURCE(source);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Recv_init, dumpi_write_recv_init, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Start(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_start);
  DUMPI_ENTERING(MPI_Start, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Start, dumpi_write_start, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Startall(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_startall);
  DUMPI_ENTERING(MPI_Startall, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_RETURNING(MPI_Startall, dumpi_write_startall, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Sendrecv(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_sendrecv);
  DUMPI_ENTERING(MPI_Sendrecv, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(sendcount);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(sendtag);
  DUMPI_CONVERT_INT(recvcount);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_SOURCE(source);
  DUMPI_CONVERT_TAG(recvtag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Sendrecv, dumpi_write_sendrecv, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Sendrecv_replace(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_sendrecv_replace);
  DUMPI_ENTERING(MPI_Sendrecv_replace, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_CONVERT_TAG(sendtag);
  DUMPI_CONVERT_SOURCE(source);
  DUMPI_CONVERT_TAG(recvtag);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Sendrecv_replace, dumpi_write_sendrecv_replace, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Type_contiguous(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_contiguous);
  DUMPI_ENTERING(MPI_Type_contiguous, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_contiguous, dumpi_write_type_contiguous, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_vector(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_vector);
  DUMPI_ENTERING(MPI_Type_vector, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT(blocklength);
  DUMPI_CONVERT_INT(stride);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_vector, dumpi_write_type_vector, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_hvector(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_hvector);
  DUMPI_ENTERING(MPI_Type_hvector, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT(blocklength);
  DUMPI_CONVERT_INT(stride);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_hvector, dumpi_write_type_hvector, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_indexed(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_indexed);
  DUMPI_ENTERING(MPI_Type_indexed, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_1(count, lengths);
  DUMPI_CONVERT_INT_ARRAY_1(count, indices);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_indexed, dumpi_write_type_indexed, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.lengths) free(prm.lengths);
  if(prm.indices) free(prm.indices);
}

void report_MPI_Type_hindexed(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_hindexed);
  DUMPI_ENTERING(MPI_Type_hindexed, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_1(count, lengths);
  DUMPI_CONVERT_INT_ARRAY_1(count, indices);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_hindexed, dumpi_write_type_hindexed, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.lengths) free(prm.lengths);
  if(prm.indices) free(prm.indices);
}

void report_MPI_Type_struct(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_struct);
  DUMPI_ENTERING(MPI_Type_struct, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_1(count, lengths);
  DUMPI_CONVERT_INT_ARRAY_1(count, indices);
  DUMPI_CONVERT_DATATYPE_ARRAY_1(count, oldtypes);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_struct, dumpi_write_type_struct, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.lengths) free(prm.lengths);
  if(prm.indices) free(prm.indices);
  if(prm.oldtypes) free(prm.oldtypes);
}

void report_MPI_Address(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_address);
  DUMPI_ENTERING(MPI_Address, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(address);
  DUMPI_RETURNING(MPI_Address, dumpi_write_address, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_extent(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_extent);
  DUMPI_ENTERING(MPI_Type_extent, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(extent);
  DUMPI_RETURNING(MPI_Type_extent, dumpi_write_type_extent, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_size);
  DUMPI_ENTERING(MPI_Type_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(size);
  DUMPI_RETURNING(MPI_Type_size, dumpi_write_type_size, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_lb(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_lb);
  DUMPI_ENTERING(MPI_Type_lb, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(lb);
  DUMPI_RETURNING(MPI_Type_lb, dumpi_write_type_lb, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_ub(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_ub);
  DUMPI_ENTERING(MPI_Type_ub, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(ub);
  DUMPI_RETURNING(MPI_Type_ub, dumpi_write_type_ub, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_commit(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_commit);
  DUMPI_ENTERING(MPI_Type_commit, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_Type_commit, dumpi_write_type_commit, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_free);
  DUMPI_ENTERING(MPI_Type_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_Type_free, dumpi_write_type_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Get_elements(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_get_elements);
  DUMPI_ENTERING(MPI_Get_elements, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(elements);
  DUMPI_RETURNING(MPI_Get_elements, dumpi_write_get_elements, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Pack(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_pack);
  DUMPI_ENTERING(MPI_Pack, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(incount);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(outcount);
  DUMPI_CONVERT_INT(position.in);
  DUMPI_CONVERT_INT(position.out);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Pack, dumpi_write_pack, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Unpack(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_unpack);
  DUMPI_ENTERING(MPI_Unpack, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(incount);
  DUMPI_CONVERT_INT(position.in);
  DUMPI_CONVERT_INT(position.out);
  DUMPI_CONVERT_INT(outcount);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Unpack, dumpi_write_unpack, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Pack_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_pack_size);
  DUMPI_ENTERING(MPI_Pack_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(incount);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(size);
  DUMPI_RETURNING(MPI_Pack_size, dumpi_write_pack_size, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Barrier(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_barrier);
  DUMPI_ENTERING(MPI_Barrier, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Barrier, dumpi_write_barrier, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Bcast(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_bcast);
  DUMPI_ENTERING(MPI_Bcast, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Bcast, dumpi_write_bcast, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Gather(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_gather);
  DUMPI_ENTERING(MPI_Gather, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commrank);
  DUMPI_CONVERT_INT(sendcount);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_INT(recvcount);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Gather, dumpi_write_gather, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Gatherv(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_gatherv);
  DUMPI_ENTERING(MPI_Gatherv, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commrank);
  DUMPI_CONVERT_INT(commsize);
  DUMPI_CONVERT_INT(sendcount);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, recvcounts);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, displs);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Gatherv, dumpi_write_gatherv, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.recvcounts) free(prm.recvcounts);
  if(prm.displs) free(prm.displs);
}

void report_MPI_Scatter(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_scatter);
  DUMPI_ENTERING(MPI_Scatter, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commrank);
  DUMPI_CONVERT_INT(sendcount);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_INT(recvcount);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Scatter, dumpi_write_scatter, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Scatterv(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_scatterv);
  DUMPI_ENTERING(MPI_Scatterv, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commrank);
  DUMPI_CONVERT_INT(commsize);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, sendcounts);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, displs);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_INT(recvcount);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Scatterv, dumpi_write_scatterv, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.sendcounts) free(prm.sendcounts);
  if(prm.displs) free(prm.displs);
}

void report_MPI_Allgather(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_allgather);
  DUMPI_ENTERING(MPI_Allgather, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(sendcount);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_INT(recvcount);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Allgather, dumpi_write_allgather, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Allgatherv(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_allgatherv);
  DUMPI_ENTERING(MPI_Allgatherv, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commsize);
  DUMPI_CONVERT_INT(sendcount);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, recvcounts);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, displs);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Allgatherv, dumpi_write_allgatherv, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.recvcounts) free(prm.recvcounts);
  if(prm.displs) free(prm.displs);
}

void report_MPI_Alltoall(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_alltoall);
  DUMPI_ENTERING(MPI_Alltoall, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(sendcount);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_INT(recvcount);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Alltoall, dumpi_write_alltoall, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Alltoallv(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_alltoallv);
  DUMPI_ENTERING(MPI_Alltoallv, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commsize);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, sendcounts);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, senddispls);
  DUMPI_CONVERT_DATATYPE(sendtype);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, recvcounts);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, recvdispls);
  DUMPI_CONVERT_DATATYPE(recvtype);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Alltoallv, dumpi_write_alltoallv, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.sendcounts) free(prm.sendcounts);
  if(prm.senddispls) free(prm.senddispls);
  if(prm.recvcounts) free(prm.recvcounts);
  if(prm.recvdispls) free(prm.recvdispls);
}

void report_MPI_Reduce(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_reduce);
  DUMPI_ENTERING(MPI_Reduce, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_OP(op);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Reduce, dumpi_write_reduce, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Op_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_op_create);
  DUMPI_ENTERING(MPI_Op_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commute);
  DUMPI_CONVERT_OP(op);
  DUMPI_RETURNING(MPI_Op_create, dumpi_write_op_create, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Op_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_op_free);
  DUMPI_ENTERING(MPI_Op_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_OP(op);
  DUMPI_RETURNING(MPI_Op_free, dumpi_write_op_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Allreduce(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_allreduce);
  DUMPI_ENTERING(MPI_Allreduce, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_OP(op);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Allreduce, dumpi_write_allreduce, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Reduce_scatter(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_reduce_scatter);
  DUMPI_ENTERING(MPI_Reduce_scatter, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commsize);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, recvcounts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_OP(op);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Reduce_scatter, dumpi_write_reduce_scatter, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.recvcounts) free(prm.recvcounts);
}

void report_MPI_Scan(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_scan);
  DUMPI_ENTERING(MPI_Scan, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_OP(op);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Scan, dumpi_write_scan, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Group_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_size);
  DUMPI_ENTERING(MPI_Group_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_INT(size);
  DUMPI_RETURNING(MPI_Group_size, dumpi_write_group_size, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Group_rank(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_rank);
  DUMPI_ENTERING(MPI_Group_rank, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_INT(rank);
  DUMPI_RETURNING(MPI_Group_rank, dumpi_write_group_rank, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Group_translate_ranks(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_translate_ranks);
  DUMPI_ENTERING(MPI_Group_translate_ranks, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group1);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_1(count, ranks1);
  DUMPI_CONVERT_GROUP(group2);
  DUMPI_CONVERT_INT_ARRAY_1(count, ranks2);
  DUMPI_RETURNING(MPI_Group_translate_ranks, dumpi_write_group_translate_ranks, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.ranks1) free(prm.ranks1);
  if(prm.ranks2) free(prm.ranks2);
}

void report_MPI_Group_compare(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_compare);
  DUMPI_ENTERING(MPI_Group_compare, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group1);
  DUMPI_CONVERT_GROUP(group2);
  DUMPI_CONVERT_COMPARISON(result);
  DUMPI_RETURNING(MPI_Group_compare, dumpi_write_group_compare, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_group(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_group);
  DUMPI_ENTERING(MPI_Comm_group, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_RETURNING(MPI_Comm_group, dumpi_write_comm_group, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Group_union(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_union);
  DUMPI_ENTERING(MPI_Group_union, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group1);
  DUMPI_CONVERT_GROUP(group2);
  DUMPI_CONVERT_GROUP(newgroup);
  DUMPI_RETURNING(MPI_Group_union, dumpi_write_group_union, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Group_intersection(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_intersection);
  DUMPI_ENTERING(MPI_Group_intersection, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group1);
  DUMPI_CONVERT_GROUP(group2);
  DUMPI_CONVERT_GROUP(newgroup);
  DUMPI_RETURNING(MPI_Group_intersection, dumpi_write_group_intersection, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Group_difference(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_difference);
  DUMPI_ENTERING(MPI_Group_difference, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group1);
  DUMPI_CONVERT_GROUP(group2);
  DUMPI_CONVERT_GROUP(newgroup);
  DUMPI_RETURNING(MPI_Group_difference, dumpi_write_group_difference, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Group_incl(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_incl);
  DUMPI_ENTERING(MPI_Group_incl, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_1(count, ranks);
  DUMPI_CONVERT_GROUP(newgroup);
  DUMPI_RETURNING(MPI_Group_incl, dumpi_write_group_incl, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.ranks) free(prm.ranks);
}

void report_MPI_Group_excl(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_excl);
  DUMPI_ENTERING(MPI_Group_excl, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_1(count, ranks);
  DUMPI_CONVERT_GROUP(newgroup);
  DUMPI_RETURNING(MPI_Group_excl, dumpi_write_group_excl, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.ranks) free(prm.ranks);
}

/*
void report_MPI_Group_range_incl(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_range_incl);
  DUMPI_ENTERING(MPI_Group_range_incl, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_2(count, 3, ranges);
  DUMPI_CONVERT_GROUP(newgroup);
  DUMPI_RETURNING(MPI_Group_range_incl, dumpi_write_group_range_incl, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Group_range_excl(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_range_excl);
  DUMPI_ENTERING(MPI_Group_range_excl, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_2(count, 3, ranges);
  DUMPI_CONVERT_GROUP(newgroup);
  DUMPI_RETURNING(MPI_Group_range_excl, dumpi_write_group_range_excl, prm, thread, cpu, wall, perf, wrapper, opts);
}
*/

void report_MPI_Group_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_group_free);
  DUMPI_ENTERING(MPI_Group_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_RETURNING(MPI_Group_free, dumpi_write_group_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_size);
  DUMPI_ENTERING(MPI_Comm_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(size);
  DUMPI_RETURNING(MPI_Comm_size, dumpi_write_comm_size, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_rank(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_rank);
  DUMPI_ENTERING(MPI_Comm_rank, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(rank);
  DUMPI_RETURNING(MPI_Comm_rank, dumpi_write_comm_rank, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_compare(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_compare);
  DUMPI_ENTERING(MPI_Comm_compare, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm1);
  DUMPI_CONVERT_COMM(comm2);
  DUMPI_CONVERT_COMPARISON(result);
  DUMPI_RETURNING(MPI_Comm_compare, dumpi_write_comm_compare, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_dup(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_dup);
  DUMPI_ENTERING(MPI_Comm_dup, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Comm_dup, dumpi_write_comm_dup, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_create);
  DUMPI_ENTERING(MPI_Comm_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Comm_create, dumpi_write_comm_create, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_split(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_split);
  DUMPI_ENTERING(MPI_Comm_split, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_INT(color);
  DUMPI_CONVERT_INT(key);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Comm_split, dumpi_write_comm_split, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_free);
  DUMPI_ENTERING(MPI_Comm_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Comm_free, dumpi_write_comm_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_test_inter(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_test_inter);
  DUMPI_ENTERING(MPI_Comm_test_inter, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(inter);
  DUMPI_RETURNING(MPI_Comm_test_inter, dumpi_write_comm_test_inter, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_remote_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_remote_size);
  DUMPI_ENTERING(MPI_Comm_remote_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(size);
  DUMPI_RETURNING(MPI_Comm_remote_size, dumpi_write_comm_remote_size, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_remote_group(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_remote_group);
  DUMPI_ENTERING(MPI_Comm_remote_group, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_RETURNING(MPI_Comm_remote_group, dumpi_write_comm_remote_group, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Intercomm_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_intercomm_create);
  DUMPI_ENTERING(MPI_Intercomm_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(localcomm);
  DUMPI_CONVERT_INT(localleader);
  DUMPI_CONVERT_COMM(remotecomm);
  DUMPI_CONVERT_INT(remoteleader);
  DUMPI_CONVERT_TAG(tag);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Intercomm_create, dumpi_write_intercomm_create, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Intercomm_merge(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_intercomm_merge);
  DUMPI_ENTERING(MPI_Intercomm_merge, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(high);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Intercomm_merge, dumpi_write_intercomm_merge, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Keyval_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_keyval_create);
  DUMPI_ENTERING(MPI_Keyval_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_KEYVAL(key);
  DUMPI_RETURNING(MPI_Keyval_create, dumpi_write_keyval_create, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Keyval_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_keyval_free);
  DUMPI_ENTERING(MPI_Keyval_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_KEYVAL(key);
  DUMPI_RETURNING(MPI_Keyval_free, dumpi_write_keyval_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Attr_put(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_attr_put);
  DUMPI_ENTERING(MPI_Attr_put, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(key);
  DUMPI_RETURNING(MPI_Attr_put, dumpi_write_attr_put, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Attr_get(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_attr_get);
  DUMPI_ENTERING(MPI_Attr_get, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(key);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Attr_get, dumpi_write_attr_get, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Attr_delete(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_attr_delete);
  DUMPI_ENTERING(MPI_Attr_delete, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(key);
  DUMPI_RETURNING(MPI_Attr_delete, dumpi_write_attr_delete, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Topo_test(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_topo_test);
  DUMPI_ENTERING(MPI_Topo_test, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_TOPOLOGY(topo);
  DUMPI_RETURNING(MPI_Topo_test, dumpi_write_topo_test, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Cart_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cart_create);
  DUMPI_ENTERING(MPI_Cart_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_INT(ndim);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, dims);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, periods);
  DUMPI_CONVERT_INT(reorder);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Cart_create, dumpi_write_cart_create, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.dims) free(prm.dims);
  if(prm.periods) free(prm.periods);
}

void report_MPI_Dims_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_dims_create);
  DUMPI_ENTERING(MPI_Dims_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(nodes);
  DUMPI_CONVERT_INT(ndim);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, dims.in);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, dims.out);
  DUMPI_RETURNING(MPI_Dims_create, dumpi_write_dims_create, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.dims.in) free(prm.dims.in);
  if(prm.dims.out) free(prm.dims.out);
}

void report_MPI_Graph_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_graph_create);
  DUMPI_ENTERING(MPI_Graph_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(numedges);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_INT(nodes);
  DUMPI_CONVERT_INT_ARRAY_1(nodes, index);
  DUMPI_CONVERT_INT_ARRAY_1(numedges, edges);
  DUMPI_CONVERT_INT(reorder);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Graph_create, dumpi_write_graph_create, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.index) free(prm.index);
  if(prm.edges) free(prm.edges);
}

void report_MPI_Graphdims_get(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_graphdims_get);
  DUMPI_ENTERING(MPI_Graphdims_get, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(nodes);
  DUMPI_CONVERT_INT(edges);
  DUMPI_RETURNING(MPI_Graphdims_get, dumpi_write_graphdims_get, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Graph_get(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_graph_get);
  DUMPI_ENTERING(MPI_Graph_get, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(totedges);
  DUMPI_CONVERT_INT(totnodes);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(maxindex);
  DUMPI_CONVERT_INT(maxedges);
  DUMPI_CONVERT_INT_ARRAY_1(totnodes, index);
  DUMPI_CONVERT_INT_ARRAY_1(totedges, edges);
  DUMPI_RETURNING(MPI_Graph_get, dumpi_write_graph_get, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.index) free(prm.index);
  if(prm.edges) free(prm.edges);
}

void report_MPI_Cartdim_get(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cartdim_get);
  DUMPI_ENTERING(MPI_Cartdim_get, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(ndim);
  DUMPI_RETURNING(MPI_Cartdim_get, dumpi_write_cartdim_get, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Cart_get(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cart_get);
  DUMPI_ENTERING(MPI_Cart_get, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(ndim);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(maxdims);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, dims);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, periods);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, coords);
  DUMPI_RETURNING(MPI_Cart_get, dumpi_write_cart_get, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.dims) free(prm.dims);
  if(prm.periods) free(prm.periods);
  if(prm.coords) free(prm.coords);
}

void report_MPI_Cart_rank(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cart_rank);
  DUMPI_ENTERING(MPI_Cart_rank, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(ndim);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, coords);
  DUMPI_CONVERT_INT(rank);
  DUMPI_RETURNING(MPI_Cart_rank, dumpi_write_cart_rank, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.coords) free(prm.coords);
}

void report_MPI_Cart_coords(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cart_coords);
  DUMPI_ENTERING(MPI_Cart_coords, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(ndim);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(rank);
  DUMPI_CONVERT_INT(maxdims);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, coords);
  DUMPI_RETURNING(MPI_Cart_coords, dumpi_write_cart_coords, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.coords) free(prm.coords);
}

void report_MPI_Graph_neighbors_count(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_graph_neighbors_count);
  DUMPI_ENTERING(MPI_Graph_neighbors_count, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(rank);
  DUMPI_CONVERT_INT(nneigh);
  DUMPI_RETURNING(MPI_Graph_neighbors_count, dumpi_write_graph_neighbors_count, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Graph_neighbors(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_graph_neighbors);
  DUMPI_ENTERING(MPI_Graph_neighbors, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(nneigh);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(rank);
  DUMPI_CONVERT_INT(maxneighbors);
  DUMPI_CONVERT_INT_ARRAY_1(nneigh, neighbors);
  DUMPI_RETURNING(MPI_Graph_neighbors, dumpi_write_graph_neighbors, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.neighbors) free(prm.neighbors);
}

void report_MPI_Cart_shift(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cart_shift);
  DUMPI_ENTERING(MPI_Cart_shift, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(direction);
  DUMPI_CONVERT_INT(displ);
  DUMPI_CONVERT_SOURCE(source);
  DUMPI_CONVERT_DEST(dest);
  DUMPI_RETURNING(MPI_Cart_shift, dumpi_write_cart_shift, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Cart_sub(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cart_sub);
  DUMPI_ENTERING(MPI_Cart_sub, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(ndim);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, remain_dims);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Cart_sub, dumpi_write_cart_sub, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.remain_dims) free(prm.remain_dims);
}

void report_MPI_Cart_map(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_cart_map);
  DUMPI_ENTERING(MPI_Cart_map, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(ndim);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, dims);
  DUMPI_CONVERT_INT_ARRAY_1(ndim, period);
  DUMPI_CONVERT_INT(newrank);
  DUMPI_RETURNING(MPI_Cart_map, dumpi_write_cart_map, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.dims) free(prm.dims);
  if(prm.period) free(prm.period);
}

void report_MPI_Graph_map(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_graph_map);
  DUMPI_ENTERING(MPI_Graph_map, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(numedges);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(nodes);
  DUMPI_CONVERT_INT_ARRAY_1(nodes, index);
  DUMPI_CONVERT_INT_ARRAY_1(numedges, edges);
  DUMPI_CONVERT_INT(newrank);
  DUMPI_RETURNING(MPI_Graph_map, dumpi_write_graph_map, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.index) free(prm.index);
  if(prm.edges) free(prm.edges);
}

void report_MPI_Get_processor_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_get_processor_name);
  DUMPI_ENTERING(MPI_Get_processor_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(name);
  DUMPI_CONVERT_INT(resultlen);
  DUMPI_RETURNING(MPI_Get_processor_name, dumpi_write_get_processor_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.name) free(prm.name);
}

void report_MPI_Get_version(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_get_version);
  DUMPI_ENTERING(MPI_Get_version, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(version);
  DUMPI_CONVERT_INT(subversion);
  DUMPI_RETURNING(MPI_Get_version, dumpi_write_get_version, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Errhandler_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_errhandler_create);
  DUMPI_ENTERING(MPI_Errhandler_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Errhandler_create, dumpi_write_errhandler_create, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Errhandler_set(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_errhandler_set);
  DUMPI_ENTERING(MPI_Errhandler_set, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Errhandler_set, dumpi_write_errhandler_set, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Errhandler_get(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_errhandler_get);
  DUMPI_ENTERING(MPI_Errhandler_get, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Errhandler_get, dumpi_write_errhandler_get, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Errhandler_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_errhandler_free);
  DUMPI_ENTERING(MPI_Errhandler_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Errhandler_free, dumpi_write_errhandler_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Error_string(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_error_string);
  DUMPI_ENTERING(MPI_Error_string, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(errorcode);
  DUMPI_CONVERT_CHAR_ARRAY_1(errorstring);
  DUMPI_CONVERT_INT(resultlen);
  DUMPI_RETURNING(MPI_Error_string, dumpi_write_error_string, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.errorstring) free(prm.errorstring);
}

void report_MPI_Error_class(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_error_class);
  DUMPI_ENTERING(MPI_Error_class, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(errorcode);
  DUMPI_CONVERT_INT(errorclass);
  DUMPI_RETURNING(MPI_Error_class, dumpi_write_error_class, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Wtime(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_wtime);
  (void)prm;
  DUMPI_ENTERING(MPI_Wtime, prm, thread, cpu, wall, perf, opts);
  DUMPI_RETURNING(MPI_Wtime, dumpi_write_wtime, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Wtick(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_wtick);
  (void)prm;
  DUMPI_ENTERING(MPI_Wtick, prm, thread, cpu, wall, perf, opts);
  DUMPI_RETURNING(MPI_Wtick, dumpi_write_wtick, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Init(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_init);
  DUMPI_ENTERING(MPI_Init, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(argc);
  DUMPI_CONVERT_CHAR_ARRAY_2(argc, argv);
  DUMPI_RETURNING(MPI_Init, dumpi_write_init, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.argv) {
    int i;
    for(i = 0; i < prm.argc; ++i) free(prm.argv[i]);
    free(prm.argv);
  }
}

void report_MPI_Finalize(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_finalize);
  (void)prm;
  DUMPI_ENTERING(MPI_Finalize, prm, thread, cpu, wall, perf, opts);
  DUMPI_RETURNING(MPI_Finalize, dumpi_write_finalize, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Initialized(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_initialized);
  DUMPI_ENTERING(MPI_Initialized, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(result);
  DUMPI_RETURNING(MPI_Initialized, dumpi_write_initialized, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Abort(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_abort);
  DUMPI_ENTERING(MPI_Abort, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(errorcode);
  DUMPI_RETURNING(MPI_Abort, dumpi_write_abort, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Close_port(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_close_port);
  DUMPI_ENTERING(MPI_Close_port, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(portname);
  DUMPI_RETURNING(MPI_Close_port, dumpi_write_close_port, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.portname) free(prm.portname);
}

void report_MPI_Comm_accept(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_accept);
  DUMPI_ENTERING(MPI_Comm_accept, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(portname);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Comm_accept, dumpi_write_comm_accept, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.portname) free(prm.portname);
}

void report_MPI_Comm_connect(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_connect);
  DUMPI_ENTERING(MPI_Comm_connect, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(portname);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_RETURNING(MPI_Comm_connect, dumpi_write_comm_connect, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.portname) free(prm.portname);
}

void report_MPI_Comm_disconnect(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_disconnect);
  DUMPI_ENTERING(MPI_Comm_disconnect, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Comm_disconnect, dumpi_write_comm_disconnect, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_get_parent(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_get_parent);
  DUMPI_ENTERING(MPI_Comm_get_parent, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(parent);
  DUMPI_RETURNING(MPI_Comm_get_parent, dumpi_write_comm_get_parent, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_join(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_join);
  DUMPI_ENTERING(MPI_Comm_join, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(fd);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Comm_join, dumpi_write_comm_join, prm, thread, cpu, wall, perf, wrapper, opts);
}

/*
void report_MPI_Comm_spawn(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_spawn);
  DUMPI_ENTERING(MPI_Comm_spawn, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(oldcommrank);
  DUMPI_CONVERT_CHAR_ARRAY_1(command);
  DUMPI_CONVERT_CHAR_ARRAY_2(maxprocs, argv);
  DUMPI_CONVERT_INT(maxprocs);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_CONVERT_ERRCODE_ARRAY_1(maxprocs, errcodes);
  DUMPI_RETURNING(MPI_Comm_spawn, dumpi_write_comm_spawn, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_spawn_multiple(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_spawn_multiple);
  DUMPI_ENTERING(MPI_Comm_spawn_multiple, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(totprocs);
  DUMPI_CONVERT_INT(oldcommrank);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_CHAR_ARRAY_2(count, commands);
  DUMPI_CONVERT_CHAR_ARRAY_3(count, DUMPI_NULLTERM, DUMPI_CSTRING, argvs);
  DUMPI_CONVERT_INT_ARRAY_1(count, maxprocs);
  DUMPI_CONVERT_INFO_ARRAY_1(count, info);
  DUMPI_CONVERT_INT(root);
  DUMPI_CONVERT_COMM(oldcomm);
  DUMPI_CONVERT_COMM(newcomm);
  DUMPI_CONVERT_ERRCODE_ARRAY_1(totprocs, errcodes);
  DUMPI_RETURNING(MPI_Comm_spawn_multiple, dumpi_write_comm_spawn_multiple, prm, thread, cpu, wall, perf, wrapper, opts);
}
*/

void report_MPI_Lookup_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_lookup_name);
  DUMPI_ENTERING(MPI_Lookup_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(servicename);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_CHAR_ARRAY_1(portname);
  DUMPI_RETURNING(MPI_Lookup_name, dumpi_write_lookup_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.servicename) free(prm.servicename);
  if(prm.portname) free(prm.portname);
}

void report_MPI_Open_port(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_open_port);
  DUMPI_ENTERING(MPI_Open_port, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_CHAR_ARRAY_1(portname);
  DUMPI_RETURNING(MPI_Open_port, dumpi_write_open_port, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.portname) free(prm.portname);
}

void report_MPI_Publish_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_publish_name);
  DUMPI_ENTERING(MPI_Publish_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(servicename);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_CHAR_ARRAY_1(portname);
  DUMPI_RETURNING(MPI_Publish_name, dumpi_write_publish_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.servicename) free(prm.servicename);
}

void report_MPI_Unpublish_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_unpublish_name);
  DUMPI_ENTERING(MPI_Unpublish_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(servicename);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_CHAR_ARRAY_1(portname);
  DUMPI_RETURNING(MPI_Unpublish_name, dumpi_write_unpublish_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.servicename) free(prm.servicename);
  if(prm.portname) free(prm.portname);
}

void report_MPI_Accumulate(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_accumulate);
  DUMPI_ENTERING(MPI_Accumulate, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(origincount);
  DUMPI_CONVERT_DATATYPE(origintype);
  DUMPI_CONVERT_INT(targetrank);
  DUMPI_CONVERT_INT(targetdisp);
  DUMPI_CONVERT_INT(targetcount);
  DUMPI_CONVERT_DATATYPE(targettype);
  DUMPI_CONVERT_OP(op);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Accumulate, dumpi_write_accumulate, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Get(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_get);
  DUMPI_ENTERING(MPI_Get, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(origincount);
  DUMPI_CONVERT_DATATYPE(origintype);
  DUMPI_CONVERT_INT(targetrank);
  DUMPI_CONVERT_INT(targetdisp);
  DUMPI_CONVERT_INT(targetcount);
  DUMPI_CONVERT_DATATYPE(targettype);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Get, dumpi_write_get, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Put(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_put);
  DUMPI_ENTERING(MPI_Put, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(origincount);
  DUMPI_CONVERT_DATATYPE(origintype);
  DUMPI_CONVERT_INT(targetrank);
  DUMPI_CONVERT_INT(targetdisp);
  DUMPI_CONVERT_INT(targetcount);
  DUMPI_CONVERT_DATATYPE(targettype);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Put, dumpi_write_put, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_complete(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_complete);
  DUMPI_ENTERING(MPI_Win_complete, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_complete, dumpi_write_win_complete, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_create);
  DUMPI_ENTERING(MPI_Win_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(size);
  DUMPI_CONVERT_INT(dispunit);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_create, dumpi_write_win_create, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_fence(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_fence);
  DUMPI_ENTERING(MPI_Win_fence, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN_ASSERT(assertion);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_fence, dumpi_write_win_fence, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_free);
  DUMPI_ENTERING(MPI_Win_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_free, dumpi_write_win_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_get_group(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_get_group);
  DUMPI_ENTERING(MPI_Win_get_group, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_RETURNING(MPI_Win_get_group, dumpi_write_win_get_group, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_lock(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_lock);
  DUMPI_ENTERING(MPI_Win_lock, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_LOCKTYPE(locktype);
  DUMPI_CONVERT_INT(winrank);
  DUMPI_CONVERT_WIN_ASSERT(assertion);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_lock, dumpi_write_win_lock, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_post(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_post);
  DUMPI_ENTERING(MPI_Win_post, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_WIN_ASSERT(assertion);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_post, dumpi_write_win_post, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_start(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_start);
  DUMPI_ENTERING(MPI_Win_start, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_CONVERT_WIN_ASSERT(assertion);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_start, dumpi_write_win_start, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_test(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_test);
  DUMPI_ENTERING(MPI_Win_test, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Win_test, dumpi_write_win_test, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_unlock(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_unlock);
  DUMPI_ENTERING(MPI_Win_unlock, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(winrank);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_unlock, dumpi_write_win_unlock, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_wait(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_wait);
  DUMPI_ENTERING(MPI_Win_wait, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_RETURNING(MPI_Win_wait, dumpi_write_win_wait, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Alltoallw(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_alltoallw);
  DUMPI_ENTERING(MPI_Alltoallw, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(commsize);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, sendcounts);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, senddispls);
  DUMPI_CONVERT_DATATYPE_ARRAY_1(commsize, sendtypes);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, recvcounts);
  DUMPI_CONVERT_INT_ARRAY_1(commsize, recvdispls);
  DUMPI_CONVERT_DATATYPE_ARRAY_1(commsize, recvtypes);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Alltoallw, dumpi_write_alltoallw, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.sendcounts) free(prm.sendcounts);
  if(prm.senddispls) free(prm.senddispls);
  if(prm.sendtypes) free(prm.sendtypes);
  if(prm.recvcounts) free(prm.recvcounts);
  if(prm.recvdispls) free(prm.recvdispls);
  if(prm.recvtypes) free(prm.recvtypes);
}

void report_MPI_Exscan(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_exscan);
  DUMPI_ENTERING(MPI_Exscan, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_OP(op);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_RETURNING(MPI_Exscan, dumpi_write_exscan, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Add_error_class(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_add_error_class);
  DUMPI_ENTERING(MPI_Add_error_class, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(errorclass);
  DUMPI_RETURNING(MPI_Add_error_class, dumpi_write_add_error_class, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Add_error_code(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_add_error_code);
  DUMPI_ENTERING(MPI_Add_error_code, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(errorclass);
  DUMPI_CONVERT_INT(errorcode);
  DUMPI_RETURNING(MPI_Add_error_code, dumpi_write_add_error_code, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Add_error_string(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_add_error_string);
  DUMPI_ENTERING(MPI_Add_error_string, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(errorcode);
  DUMPI_CONVERT_CHAR_ARRAY_1(errorstring);
  DUMPI_RETURNING(MPI_Add_error_string, dumpi_write_add_error_string, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.errorstring) free(prm.errorstring);
}

void report_MPI_Comm_call_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_call_errhandler);
  DUMPI_ENTERING(MPI_Comm_call_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_INT(errorcode);
  DUMPI_RETURNING(MPI_Comm_call_errhandler, dumpi_write_comm_call_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_create_keyval(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_create_keyval);
  DUMPI_ENTERING(MPI_Comm_create_keyval, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Comm_create_keyval, dumpi_write_comm_create_keyval, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_delete_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_delete_attr);
  DUMPI_ENTERING(MPI_Comm_delete_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_COMM_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Comm_delete_attr, dumpi_write_comm_delete_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_free_keyval(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_free_keyval);
  DUMPI_ENTERING(MPI_Comm_free_keyval, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Comm_free_keyval, dumpi_write_comm_free_keyval, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_get_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_get_attr);
  DUMPI_ENTERING(MPI_Comm_get_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_COMM_KEYVAL(keyval);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Comm_get_attr, dumpi_write_comm_get_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_get_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_get_name);
  DUMPI_ENTERING(MPI_Comm_get_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_CHAR_ARRAY_1(name);
  DUMPI_CONVERT_INT(resultlen);
  DUMPI_RETURNING(MPI_Comm_get_name, dumpi_write_comm_get_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.name) free(prm.name);
}

void report_MPI_Comm_set_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_set_attr);
  DUMPI_ENTERING(MPI_Comm_set_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_COMM_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Comm_set_attr, dumpi_write_comm_set_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_set_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_set_name);
  DUMPI_ENTERING(MPI_Comm_set_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_CHAR_ARRAY_1(name);
  DUMPI_RETURNING(MPI_Comm_set_name, dumpi_write_comm_set_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.name) free(prm.name);
}

void report_MPI_File_call_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_call_errhandler);
  DUMPI_ENTERING(MPI_File_call_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(errorcode);
  DUMPI_RETURNING(MPI_File_call_errhandler, dumpi_write_file_call_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Grequest_complete(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_grequest_complete);
  DUMPI_ENTERING(MPI_Grequest_complete, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Grequest_complete, dumpi_write_grequest_complete, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Grequest_start(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_grequest_start);
  DUMPI_ENTERING(MPI_Grequest_start, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_Grequest_start, dumpi_write_grequest_start, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Init_thread(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_init_thread);
  DUMPI_ENTERING(MPI_Init_thread, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(argc);
  DUMPI_CONVERT_CHAR_ARRAY_2(argc, argv);
  DUMPI_CONVERT_THREADLEVEL(required);
  DUMPI_CONVERT_THREADLEVEL(provided);
  DUMPI_RETURNING(MPI_Init_thread, dumpi_write_init_thread, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.argv) {
    int i;
    for(i = 0; i < prm.argc; ++i) free(prm.argv[i]);
    free(prm.argv);
  }
}

void report_MPI_Is_thread_main(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_is_thread_main);
  DUMPI_ENTERING(MPI_Is_thread_main, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Is_thread_main, dumpi_write_is_thread_main, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Query_thread(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_query_thread);
  DUMPI_ENTERING(MPI_Query_thread, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_THREADLEVEL(supported);
  DUMPI_RETURNING(MPI_Query_thread, dumpi_write_query_thread, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Status_set_cancelled(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_status_set_cancelled);
  DUMPI_ENTERING(MPI_Status_set_cancelled, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Status_set_cancelled, dumpi_write_status_set_cancelled, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Status_set_elements(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_status_set_elements);
  DUMPI_ENTERING(MPI_Status_set_elements, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(count);
  DUMPI_RETURNING(MPI_Status_set_elements, dumpi_write_status_set_elements, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Type_create_keyval(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_create_keyval);
  DUMPI_ENTERING(MPI_Type_create_keyval, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_TYPE_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Type_create_keyval, dumpi_write_type_create_keyval, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_delete_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_delete_attr);
  DUMPI_ENTERING(MPI_Type_delete_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_TYPE_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Type_delete_attr, dumpi_write_type_delete_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_dup(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_dup);
  DUMPI_ENTERING(MPI_Type_dup, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_dup, dumpi_write_type_dup, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_free_keyval(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_free_keyval);
  DUMPI_ENTERING(MPI_Type_free_keyval, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_TYPE_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Type_free_keyval, dumpi_write_type_free_keyval, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_get_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_get_attr);
  DUMPI_ENTERING(MPI_Type_get_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_TYPE_KEYVAL(keyval);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Type_get_attr, dumpi_write_type_get_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_get_contents(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_get_contents);
  DUMPI_ENTERING(MPI_Type_get_contents, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(numdatatypes);
  DUMPI_CONVERT_INT(numaddresses);
  DUMPI_CONVERT_INT(numintegers);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(maxintegers);
  DUMPI_CONVERT_INT(maxaddresses);
  DUMPI_CONVERT_INT(maxdatatypes);
  DUMPI_CONVERT_INT_ARRAY_1(numintegers, arrintegers);
  DUMPI_CONVERT_INT_ARRAY_1(numaddresses, arraddresses);
  DUMPI_CONVERT_DATATYPE_ARRAY_1(numdatatypes, arrdatatypes);
  DUMPI_RETURNING(MPI_Type_get_contents, dumpi_write_type_get_contents, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.arrintegers) free(prm.arrintegers);
  if(prm.arraddresses) free(prm.arraddresses);
  if(prm.arrdatatypes) free(prm.arrdatatypes);
}

void report_MPI_Type_get_envelope(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_get_envelope);
  DUMPI_ENTERING(MPI_Type_get_envelope, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(numintegers);
  DUMPI_CONVERT_INT(numaddresses);
  DUMPI_CONVERT_INT(numdatatypes);
  DUMPI_CONVERT_COMBINER(combiner);
  DUMPI_RETURNING(MPI_Type_get_envelope, dumpi_write_type_get_envelope, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_get_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_get_name);
  DUMPI_ENTERING(MPI_Type_get_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_CHAR_ARRAY_1(name);
  DUMPI_CONVERT_INT(resultlen);
  DUMPI_RETURNING(MPI_Type_get_name, dumpi_write_type_get_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.name) free(prm.name);
}

void report_MPI_Type_set_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_set_attr);
  DUMPI_ENTERING(MPI_Type_set_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_TYPE_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Type_set_attr, dumpi_write_type_set_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_set_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_set_name);
  DUMPI_ENTERING(MPI_Type_set_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_CHAR_ARRAY_1(name);
  DUMPI_RETURNING(MPI_Type_set_name, dumpi_write_type_set_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.name) free(prm.name);
}

void report_MPI_Type_match_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_match_size);
  DUMPI_ENTERING(MPI_Type_match_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_TYPECLASS(typeclass);
  DUMPI_CONVERT_INT(size);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_Type_match_size, dumpi_write_type_match_size, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_call_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_call_errhandler);
  DUMPI_ENTERING(MPI_Win_call_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_INT(errorcode);
  DUMPI_RETURNING(MPI_Win_call_errhandler, dumpi_write_win_call_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_create_keyval(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_create_keyval);
  DUMPI_ENTERING(MPI_Win_create_keyval, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Win_create_keyval, dumpi_write_win_create_keyval, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_delete_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_delete_attr);
  DUMPI_ENTERING(MPI_Win_delete_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_WIN_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Win_delete_attr, dumpi_write_win_delete_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_free_keyval(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_free_keyval);
  DUMPI_ENTERING(MPI_Win_free_keyval, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Win_free_keyval, dumpi_write_win_free_keyval, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_get_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_get_attr);
  DUMPI_ENTERING(MPI_Win_get_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_WIN_KEYVAL(keyval);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Win_get_attr, dumpi_write_win_get_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_get_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_get_name);
  DUMPI_ENTERING(MPI_Win_get_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_CHAR_ARRAY_1(name);
  DUMPI_CONVERT_INT(resultlen);
  DUMPI_RETURNING(MPI_Win_get_name, dumpi_write_win_get_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.name) free(prm.name);
}

void report_MPI_Win_set_attr(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_set_attr);
  DUMPI_ENTERING(MPI_Win_set_attr, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_WIN_KEYVAL(keyval);
  DUMPI_RETURNING(MPI_Win_set_attr, dumpi_write_win_set_attr, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_set_name(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_set_name);
  DUMPI_ENTERING(MPI_Win_set_name, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_CHAR_ARRAY_1(name);
  DUMPI_RETURNING(MPI_Win_set_name, dumpi_write_win_set_name, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.name) free(prm.name);
}

void report_MPI_Alloc_mem(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_alloc_mem);
  DUMPI_ENTERING(MPI_Alloc_mem, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(size);
  DUMPI_CONVERT_INFO(info);
  DUMPI_RETURNING(MPI_Alloc_mem, dumpi_write_alloc_mem, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_create_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_create_errhandler);
  DUMPI_ENTERING(MPI_Comm_create_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Comm_create_errhandler, dumpi_write_comm_create_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_get_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_get_errhandler);
  DUMPI_ENTERING(MPI_Comm_get_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Comm_get_errhandler, dumpi_write_comm_get_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Comm_set_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_comm_set_errhandler);
  DUMPI_ENTERING(MPI_Comm_set_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Comm_set_errhandler, dumpi_write_comm_set_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_create_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_create_errhandler);
  DUMPI_ENTERING(MPI_File_create_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_File_create_errhandler, dumpi_write_file_create_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_errhandler);
  DUMPI_ENTERING(MPI_File_get_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_File_get_errhandler, dumpi_write_file_get_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_set_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_set_errhandler);
  DUMPI_ENTERING(MPI_File_set_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_File_set_errhandler, dumpi_write_file_set_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Finalized(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_finalized);
  DUMPI_ENTERING(MPI_Finalized, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Finalized, dumpi_write_finalized, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Free_mem(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_free_mem);
  (void)prm;
  DUMPI_ENTERING(MPI_Free_mem, prm, thread, cpu, wall, perf, opts);
  DUMPI_RETURNING(MPI_Free_mem, dumpi_write_free_mem, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Get_address(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_get_address);
  DUMPI_ENTERING(MPI_Get_address, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(address);
  DUMPI_RETURNING(MPI_Get_address, dumpi_write_get_address, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Info_create(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_create);
  DUMPI_ENTERING(MPI_Info_create, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_RETURNING(MPI_Info_create, dumpi_write_info_create, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Info_delete(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_delete);
  DUMPI_ENTERING(MPI_Info_delete, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_CHAR_ARRAY_1(key);
  DUMPI_RETURNING(MPI_Info_delete, dumpi_write_info_delete, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.key) free(prm.key);
}

void report_MPI_Info_dup(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_dup);
  DUMPI_ENTERING(MPI_Info_dup, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(oldinfo);
  DUMPI_CONVERT_INFO(newinfo);
  DUMPI_RETURNING(MPI_Info_dup, dumpi_write_info_dup, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Info_free(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_free);
  DUMPI_ENTERING(MPI_Info_free, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_RETURNING(MPI_Info_free, dumpi_write_info_free, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Info_get(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_get);
  DUMPI_ENTERING(MPI_Info_get, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_CHAR_ARRAY_1(key);
  DUMPI_CONVERT_INT(valuelength);
  DUMPI_CONVERT_CHAR_ARRAY_1(value);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Info_get, dumpi_write_info_get, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.key) free(prm.key);
  if(prm.value) free(prm.value);
}

void report_MPI_Info_get_nkeys(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_get_nkeys);
  DUMPI_ENTERING(MPI_Info_get_nkeys, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_INT(nkeys);
  DUMPI_RETURNING(MPI_Info_get_nkeys, dumpi_write_info_get_nkeys, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Info_get_nthkey(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_get_nthkey);
  DUMPI_ENTERING(MPI_Info_get_nthkey, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_INT(n);
  DUMPI_CONVERT_CHAR_ARRAY_1(key);
  DUMPI_RETURNING(MPI_Info_get_nthkey, dumpi_write_info_get_nthkey, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.key) free(prm.key);
}

void report_MPI_Info_get_valuelen(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_get_valuelen);
  DUMPI_ENTERING(MPI_Info_get_valuelen, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_CHAR_ARRAY_1(key);
  DUMPI_CONVERT_INT(valuelen);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_Info_get_valuelen, dumpi_write_info_get_valuelen, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.key) free(prm.key);
}

void report_MPI_Info_set(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_info_set);
  DUMPI_ENTERING(MPI_Info_set, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_CHAR_ARRAY_1(key);
  DUMPI_CONVERT_CHAR_ARRAY_1(value);
  DUMPI_RETURNING(MPI_Info_set, dumpi_write_info_set, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.key) free(prm.key);
  if(prm.value) free(prm.value);
}

void report_MPI_Pack_external(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_pack_external);
  DUMPI_ENTERING(MPI_Pack_external, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(datarep);
  DUMPI_CONVERT_INT(incount);
  DUMPI_CONVERT_DATATYPE(intype);
  DUMPI_CONVERT_INT(outcount);
  DUMPI_CONVERT_INT(position.in);
  DUMPI_CONVERT_INT(position.out);
  DUMPI_RETURNING(MPI_Pack_external, dumpi_write_pack_external, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.datarep) free(prm.datarep);
}

void report_MPI_Pack_external_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_pack_external_size);
  DUMPI_ENTERING(MPI_Pack_external_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(datarep);
  DUMPI_CONVERT_INT(incount);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(size);
  DUMPI_RETURNING(MPI_Pack_external_size, dumpi_write_pack_external_size, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.datarep) free(prm.datarep);
}

void report_MPI_Request_get_status(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_request_get_status);
  DUMPI_ENTERING(MPI_Request_get_status, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_REQUEST(request);
  DUMPI_CONVERT_INT(flag);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_Request_get_status, dumpi_write_request_get_status, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_Type_create_darray(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_create_darray);
  DUMPI_ENTERING(MPI_Type_create_darray, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(size);
  DUMPI_CONVERT_INT(rank);
  DUMPI_CONVERT_INT(ndims);
  DUMPI_CONVERT_INT_ARRAY_1(ndims, gsizes);
  DUMPI_CONVERT_DISTRIBUTION_ARRAY_1(ndims, distribs);
  DUMPI_CONVERT_INT_ARRAY_1(ndims, dargs);
  DUMPI_CONVERT_INT_ARRAY_1(ndims, psizes);
  DUMPI_CONVERT_ORDERING(order);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_create_darray, dumpi_write_type_create_darray, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.gsizes) free(prm.gsizes);
  if(prm.distribs) free(prm.distribs);
  if(prm.dargs) free(prm.dargs);
  if(prm.psizes) free(prm.psizes);
}

void report_MPI_Type_create_hindexed(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_create_hindexed);
  DUMPI_ENTERING(MPI_Type_create_hindexed, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_1(count, blocklengths);
  DUMPI_CONVERT_INT_ARRAY_1(count, displacements);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_create_hindexed, dumpi_write_type_create_hindexed, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.blocklengths) free(prm.blocklengths);
  if(prm.displacements) free(prm.displacements);
}

void report_MPI_Type_create_hvector(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_create_hvector);
  DUMPI_ENTERING(MPI_Type_create_hvector, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT(blocklength);
  DUMPI_CONVERT_INT(stride);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_create_hvector, dumpi_write_type_create_hvector, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_create_indexed_block(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_create_indexed_block);
  DUMPI_ENTERING(MPI_Type_create_indexed_block, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT(blocklength);
  DUMPI_CONVERT_INT_ARRAY_1(count, displacments);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_create_indexed_block, dumpi_write_type_create_indexed_block, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.displacments) free(prm.displacments);
}

void report_MPI_Type_create_resized(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_create_resized);
  DUMPI_ENTERING(MPI_Type_create_resized, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_INT(lb);
  DUMPI_CONVERT_INT(extent);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_create_resized, dumpi_write_type_create_resized, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_create_struct(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_create_struct);
  DUMPI_ENTERING(MPI_Type_create_struct, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_INT_ARRAY_1(count, blocklengths);
  DUMPI_CONVERT_INT_ARRAY_1(count, displacements);
  DUMPI_CONVERT_DATATYPE_ARRAY_1(count, oldtypes);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_create_struct, dumpi_write_type_create_struct, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.blocklengths) free(prm.blocklengths);
  if(prm.displacements) free(prm.displacements);
}

void report_MPI_Type_create_subarray(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_create_subarray);
  DUMPI_ENTERING(MPI_Type_create_subarray, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(ndims);
  DUMPI_CONVERT_INT_ARRAY_1(ndims, sizes);
  DUMPI_CONVERT_INT_ARRAY_1(ndims, subsizes);
  DUMPI_CONVERT_INT_ARRAY_1(ndims, starts);
  DUMPI_CONVERT_ORDERING(order);
  DUMPI_CONVERT_DATATYPE(oldtype);
  DUMPI_CONVERT_DATATYPE(newtype);
  DUMPI_RETURNING(MPI_Type_create_subarray, dumpi_write_type_create_subarray, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.sizes) free(prm.sizes);
  if(prm.subsizes) free(prm.subsizes);
  if(prm.starts) free(prm.starts);
}

void report_MPI_Type_get_extent(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_get_extent);
  DUMPI_ENTERING(MPI_Type_get_extent, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(lb);
  DUMPI_CONVERT_INT(extent);
  DUMPI_RETURNING(MPI_Type_get_extent, dumpi_write_type_get_extent, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Type_get_true_extent(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_type_get_true_extent);
  DUMPI_ENTERING(MPI_Type_get_true_extent, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(lb);
  DUMPI_CONVERT_INT(extent);
  DUMPI_RETURNING(MPI_Type_get_true_extent, dumpi_write_type_get_true_extent, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Unpack_external(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_unpack_external);
  DUMPI_ENTERING(MPI_Unpack_external, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(datarep);
  DUMPI_CONVERT_INT(insize);
  DUMPI_CONVERT_INT(position.in);
  DUMPI_CONVERT_INT(position.out);
  DUMPI_CONVERT_INT(outcount);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_Unpack_external, dumpi_write_unpack_external, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_create_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_create_errhandler);
  DUMPI_ENTERING(MPI_Win_create_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Win_create_errhandler, dumpi_write_win_create_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_get_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_get_errhandler);
  DUMPI_ENTERING(MPI_Win_get_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Win_get_errhandler, dumpi_write_win_get_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Win_set_errhandler(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_win_set_errhandler);
  DUMPI_ENTERING(MPI_Win_set_errhandler, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_WIN(win);
  DUMPI_CONVERT_ERRHANDLER(errhandler);
  DUMPI_RETURNING(MPI_Win_set_errhandler, dumpi_write_win_set_errhandler, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_open(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_open);
  DUMPI_ENTERING(MPI_File_open, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_COMM(comm);
  DUMPI_CONVERT_CHAR_ARRAY_1(filename);
  DUMPI_CONVERT_FILEMODE(amode);
  DUMPI_CONVERT_INFO(info);
  DUMPI_CONVERT_FILE(file);
  DUMPI_RETURNING(MPI_File_open, dumpi_write_file_open, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.filename) free(prm.filename);
}

void report_MPI_File_close(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_close);
  DUMPI_ENTERING(MPI_File_close, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_RETURNING(MPI_File_close, dumpi_write_file_close, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_delete(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_delete);
  DUMPI_ENTERING(MPI_File_delete, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(filename);
  DUMPI_CONVERT_INFO(info);
  DUMPI_RETURNING(MPI_File_delete, dumpi_write_file_delete, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.filename) free(prm.filename);
}

void report_MPI_File_set_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_set_size);
  DUMPI_ENTERING(MPI_File_set_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(size);
  DUMPI_RETURNING(MPI_File_set_size, dumpi_write_file_set_size, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_preallocate(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_preallocate);
  DUMPI_ENTERING(MPI_File_preallocate, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(size);
  DUMPI_RETURNING(MPI_File_preallocate, dumpi_write_file_preallocate, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_size(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_size);
  DUMPI_ENTERING(MPI_File_get_size, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(size);
  DUMPI_RETURNING(MPI_File_get_size, dumpi_write_file_get_size, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_group(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_group);
  DUMPI_ENTERING(MPI_File_get_group, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_GROUP(group);
  DUMPI_RETURNING(MPI_File_get_group, dumpi_write_file_get_group, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_amode(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_amode);
  DUMPI_ENTERING(MPI_File_get_amode, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_FILEMODE(amode);
  DUMPI_RETURNING(MPI_File_get_amode, dumpi_write_file_get_amode, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_set_info(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_set_info);
  DUMPI_ENTERING(MPI_File_set_info, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INFO(info);
  DUMPI_RETURNING(MPI_File_set_info, dumpi_write_file_set_info, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_info(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_info);
  DUMPI_ENTERING(MPI_File_get_info, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INFO(info);
  DUMPI_RETURNING(MPI_File_get_info, dumpi_write_file_get_info, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_set_view(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_set_view);
  DUMPI_ENTERING(MPI_File_set_view, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_DATATYPE(hosttype);
  DUMPI_CONVERT_DATATYPE(filetype);
  DUMPI_CONVERT_CHAR_ARRAY_1(datarep);
  DUMPI_CONVERT_INFO(info);
  DUMPI_RETURNING(MPI_File_set_view, dumpi_write_file_set_view, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_view(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_view);
  DUMPI_ENTERING(MPI_File_get_view, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_DATATYPE(hosttype);
  DUMPI_CONVERT_DATATYPE(filetype);
  DUMPI_CONVERT_CHAR_ARRAY_1(datarep);
  DUMPI_RETURNING(MPI_File_get_view, dumpi_write_file_get_view, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.datarep) free(prm.datarep);
}

void report_MPI_File_read_at(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_at);
  DUMPI_ENTERING(MPI_File_read_at, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read_at, dumpi_write_file_read_at, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_read_at_all(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_at_all);
  DUMPI_ENTERING(MPI_File_read_at_all, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read_at_all, dumpi_write_file_read_at_all, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write_at(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_at);
  DUMPI_ENTERING(MPI_File_write_at, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write_at, dumpi_write_file_write_at, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write_at_all(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_at_all);
  DUMPI_ENTERING(MPI_File_write_at_all, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write_at_all, dumpi_write_file_write_at_all, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_iread_at(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_iread_at);
  DUMPI_ENTERING(MPI_File_iread_at, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPIO_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_File_iread_at, dumpi_write_file_iread_at, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_iwrite_at(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_iwrite_at);
  DUMPI_ENTERING(MPI_File_iwrite_at, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPIO_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_File_iwrite_at, dumpi_write_file_iwrite_at, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_read(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read);
  DUMPI_ENTERING(MPI_File_read, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read, dumpi_write_file_read, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_read_all(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_all);
  DUMPI_ENTERING(MPI_File_read_all, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read_all, dumpi_write_file_read_all, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write);
  DUMPI_ENTERING(MPI_File_write, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write, dumpi_write_file_write, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write_all(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_all);
  DUMPI_ENTERING(MPI_File_write_all, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write_all, dumpi_write_file_write_all, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_iread(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_iread);
  DUMPI_ENTERING(MPI_File_iread, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPIO_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_File_iread, dumpi_write_file_iread, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_iwrite(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_iwrite);
  DUMPI_ENTERING(MPI_File_iwrite, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPIO_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_File_iwrite, dumpi_write_file_iwrite, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_seek(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_seek);
  DUMPI_ENTERING(MPI_File_seek, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_WHENCE(whence);
  DUMPI_RETURNING(MPI_File_seek, dumpi_write_file_seek, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_position(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_position);
  DUMPI_ENTERING(MPI_File_get_position, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_RETURNING(MPI_File_get_position, dumpi_write_file_get_position, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_byte_offset(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_byte_offset);
  DUMPI_ENTERING(MPI_File_get_byte_offset, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT64_T(bytes);
  DUMPI_RETURNING(MPI_File_get_byte_offset, dumpi_write_file_get_byte_offset, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_read_shared(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_shared);
  DUMPI_ENTERING(MPI_File_read_shared, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read_shared, dumpi_write_file_read_shared, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write_shared(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_shared);
  DUMPI_ENTERING(MPI_File_write_shared, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write_shared, dumpi_write_file_write_shared, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_iread_shared(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_iread_shared);
  DUMPI_ENTERING(MPI_File_iread_shared, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPIO_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_File_iread_shared, dumpi_write_file_iread_shared, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_iwrite_shared(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_iwrite_shared);
  DUMPI_ENTERING(MPI_File_iwrite_shared, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPIO_CONVERT_REQUEST(request);
  DUMPI_RETURNING(MPI_File_iwrite_shared, dumpi_write_file_iwrite_shared, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_read_ordered(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_ordered);
  DUMPI_ENTERING(MPI_File_read_ordered, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read_ordered, dumpi_write_file_read_ordered, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write_ordered(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_ordered);
  DUMPI_ENTERING(MPI_File_write_ordered, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write_ordered, dumpi_write_file_write_ordered, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_seek_shared(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_seek_shared);
  DUMPI_ENTERING(MPI_File_seek_shared, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_WHENCE(whence);
  DUMPI_RETURNING(MPI_File_seek_shared, dumpi_write_file_seek_shared, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_position_shared(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_position_shared);
  DUMPI_ENTERING(MPI_File_get_position_shared, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_RETURNING(MPI_File_get_position_shared, dumpi_write_file_get_position_shared, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_read_at_all_begin(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_at_all_begin);
  DUMPI_ENTERING(MPI_File_read_at_all_begin, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_File_read_at_all_begin, dumpi_write_file_read_at_all_begin, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_read_at_all_end(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_at_all_end);
  DUMPI_ENTERING(MPI_File_read_at_all_end, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read_at_all_end, dumpi_write_file_read_at_all_end, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write_at_all_begin(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_at_all_begin);
  DUMPI_ENTERING(MPI_File_write_at_all_begin, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT64_T(offset);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_File_write_at_all_begin, dumpi_write_file_write_at_all_begin, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_write_at_all_end(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_at_all_end);
  DUMPI_ENTERING(MPI_File_write_at_all_end, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write_at_all_end, dumpi_write_file_write_at_all_end, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_read_all_begin(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_all_begin);
  DUMPI_ENTERING(MPI_File_read_all_begin, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_File_read_all_begin, dumpi_write_file_read_all_begin, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_read_all_end(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_all_end);
  DUMPI_ENTERING(MPI_File_read_all_end, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read_all_end, dumpi_write_file_read_all_end, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write_all_begin(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_all_begin);
  DUMPI_ENTERING(MPI_File_write_all_begin, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_File_write_all_begin, dumpi_write_file_write_all_begin, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_write_all_end(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_all_end);
  DUMPI_ENTERING(MPI_File_write_all_end, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write_all_end, dumpi_write_file_write_all_end, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_read_ordered_begin(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_ordered_begin);
  DUMPI_ENTERING(MPI_File_read_ordered_begin, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_File_read_ordered_begin, dumpi_write_file_read_ordered_begin, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_read_ordered_end(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_read_ordered_end);
  DUMPI_ENTERING(MPI_File_read_ordered_end, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_read_ordered_end, dumpi_write_file_read_ordered_end, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_write_ordered_begin(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_ordered_begin);
  DUMPI_ENTERING(MPI_File_write_ordered_begin, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(count);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_RETURNING(MPI_File_write_ordered_begin, dumpi_write_file_write_ordered_begin, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_write_ordered_end(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_write_ordered_end);
  DUMPI_ENTERING(MPI_File_write_ordered_end, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPI_File_write_ordered_end, dumpi_write_file_write_ordered_end, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPI_File_get_type_extent(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_type_extent);
  DUMPI_ENTERING(MPI_File_get_type_extent, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_DATATYPE(datatype);
  DUMPI_CONVERT_INT(extent);
  DUMPI_RETURNING(MPI_File_get_type_extent, dumpi_write_file_get_type_extent, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_Register_datarep(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_register_datarep);
  DUMPI_ENTERING(MPI_Register_datarep, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_CHAR_ARRAY_1(name);
  DUMPI_RETURNING(MPI_Register_datarep, dumpi_write_register_datarep, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.name) free(prm.name);
}

void report_MPI_File_set_atomicity(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_set_atomicity);
  DUMPI_ENTERING(MPI_File_set_atomicity, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_File_set_atomicity, dumpi_write_file_set_atomicity, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_get_atomicity(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_get_atomicity);
  DUMPI_ENTERING(MPI_File_get_atomicity, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_CONVERT_INT(flag);
  DUMPI_RETURNING(MPI_File_get_atomicity, dumpi_write_file_get_atomicity, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPI_File_sync(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_file_sync);
  DUMPI_ENTERING(MPI_File_sync, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FILE(file);
  DUMPI_RETURNING(MPI_File_sync, dumpi_write_file_sync, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_MPIO_Test(fwrapper *wrapper, settings *opts) {
  dumpio_test prm;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  DUMPI_ENTERING(MPIO_Test, prm, thread, cpu, wall, perf, opts);
  DUMPIO_CONVERT_REQUEST(request);
  DUMPI_CONVERT_INT(flag);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPIO_Test, dumpio_write_test, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPIO_Wait(fwrapper *wrapper, settings *opts) {
  dumpio_wait prm;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  DUMPI_ENTERING(MPIO_Wait, prm, thread, cpu, wall, perf, opts);
  DUMPIO_CONVERT_REQUEST(request);
  DUMPI_CONVERT_STATUS_PTR(status);
  DUMPI_RETURNING(MPIO_Wait, dumpio_write_wait, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.status) free(prm.status);
}

void report_MPIO_Testall(fwrapper *wrapper, settings *opts) {
  dumpio_testall prm;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  DUMPI_ENTERING(MPIO_Testall, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPIO_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(flag);
  DUMPI_CONVERT_STATUS_ARRAY_1(count, statuses);
  DUMPI_RETURNING(MPIO_Testall, dumpio_write_testall, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.statuses) free(prm.statuses);
}

void report_MPIO_Waitall(fwrapper *wrapper, settings *opts) {
  dumpio_waitall prm;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  DUMPI_ENTERING(MPIO_Waitall, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPIO_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_STATUS_ARRAY_1(count, statuses);
  DUMPI_RETURNING(MPIO_Waitall, dumpio_write_waitall, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.statuses) free(prm.statuses);
}

void report_MPIO_Testany(fwrapper *wrapper, settings *opts) {
  dumpio_testany prm;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  DUMPI_ENTERING(MPIO_Testany, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPIO_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(index);
  DUMPI_CONVERT_INT(flag);
  DUMPI_CONVERT_STATUS_ARRAY_1(count, statuses);
  DUMPI_RETURNING(MPIO_Testany, dumpio_write_testany, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.statuses) free(prm.statuses);
}

void report_MPIO_Waitany(fwrapper *wrapper, settings *opts) {
  dumpio_waitany prm;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  DUMPI_ENTERING(MPIO_Waitany, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPIO_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(index);
  DUMPI_CONVERT_STATUS_ARRAY_1(count, statuses);
  DUMPI_RETURNING(MPIO_Waitany, dumpio_write_waitany, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.statuses) free(prm.statuses);
}

void report_MPIO_Waitsome(fwrapper *wrapper, settings *opts) {
  dumpio_waitsome prm;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  DUMPI_ENTERING(MPIO_Waitsome, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPIO_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(outcount);
  DUMPI_CONVERT_INT_ARRAY_1(outcount, indices);
  DUMPI_CONVERT_STATUS_ARRAY_1(outcount, statuses);
  DUMPI_RETURNING(MPIO_Waitsome, dumpio_write_waitsome, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.indices) free(prm.indices);
  if(prm.statuses) free(prm.statuses);
}

void report_MPIO_Testsome(fwrapper *wrapper, settings *opts) {
  dumpio_testsome prm;
  uint16_t thread;
  dumpi_time cpu, wall;
  dumpi_perfinfo perf;
  DUMPI_ENTERING(MPIO_Testsome, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_INT(count);
  DUMPIO_CONVERT_REQUEST_ARRAY_1(count, requests);
  DUMPI_CONVERT_INT(outcount);
  DUMPI_CONVERT_INT_ARRAY_1(outcount, indices);
  DUMPI_CONVERT_STATUS_ARRAY_1(outcount, statuses);
  DUMPI_RETURNING(MPIO_Testsome, dumpio_write_testsome, prm, thread, cpu, wall, perf, wrapper, opts);
  if(prm.requests) free(prm.requests);
  if(prm.indices) free(prm.indices);
  if(prm.statuses) free(prm.statuses);
}

void report_Function_enter(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_func_call);
  DUMPI_ENTERING(Function_enter, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FUNC(fn);
  DUMPI_RETURNING(Function_enter, dumpi_write_func_enter, prm, thread, cpu, wall, perf, wrapper, opts);
}

void report_Function_exit(fwrapper *wrapper, settings *opts) {
  PREAMBLE(dumpi_func_call);
  DUMPI_ENTERING(Function_exit, prm, thread, cpu, wall, perf, opts);
  DUMPI_CONVERT_FUNC(fn);
  DUMPI_RETURNING(Function_exit, dumpi_write_func_exit, prm, thread, cpu, wall, perf, wrapper, opts);
}

/* Map function names to handlers */
typedef struct funmap {
  const char *label;
  mpihandler handler;
} funmap;

#define DMAP(NAME) { #NAME, report_ ## NAME }

static funmap handlers[] = {
  DMAP(MPI_Send),            DMAP(MPI_Recv),            DMAP(MPI_Get_count),
  DMAP(MPI_Bsend),           DMAP(MPI_Ssend),           DMAP(MPI_Rsend),
  DMAP(MPI_Buffer_attach),   DMAP(MPI_Buffer_detach),   DMAP(MPI_Isend),
  DMAP(MPI_Ibsend),          DMAP(MPI_Issend),          DMAP(MPI_Irsend),
  DMAP(MPI_Irecv),           DMAP(MPI_Wait),            DMAP(MPI_Test),
  DMAP(MPI_Request_free),    DMAP(MPI_Waitany),         DMAP(MPI_Testany),
  DMAP(MPI_Waitall),         DMAP(MPI_Testall),         DMAP(MPI_Waitsome),
  DMAP(MPI_Testsome),        DMAP(MPI_Iprobe),          DMAP(MPI_Probe),
  DMAP(MPI_Cancel),          DMAP(MPI_Test_cancelled),  DMAP(MPI_Send_init),
  DMAP(MPI_Bsend_init),      DMAP(MPI_Ssend_init),      DMAP(MPI_Rsend_init),
  DMAP(MPI_Recv_init),       DMAP(MPI_Start),           DMAP(MPI_Startall),
  DMAP(MPI_Sendrecv),        DMAP(MPI_Sendrecv_replace),
  DMAP(MPI_Type_contiguous), DMAP(MPI_Type_vector),    DMAP(MPI_Type_hvector),
  DMAP(MPI_Type_indexed),    DMAP(MPI_Type_hindexed),  DMAP(MPI_Type_struct),
  DMAP(MPI_Address),         DMAP(MPI_Type_extent),    DMAP(MPI_Type_size),
  DMAP(MPI_Type_lb),         DMAP(MPI_Type_ub),        DMAP(MPI_Type_commit),
  DMAP(MPI_Type_free),       DMAP(MPI_Get_elements),   DMAP(MPI_Pack),
  DMAP(MPI_Unpack),          DMAP(MPI_Pack_size),      DMAP(MPI_Barrier),
  DMAP(MPI_Bcast),           DMAP(MPI_Gather),         DMAP(MPI_Gatherv),
  DMAP(MPI_Scatter),         DMAP(MPI_Scatterv),       DMAP(MPI_Allgather),
  DMAP(MPI_Allgatherv),      DMAP(MPI_Alltoall),       DMAP(MPI_Alltoallv),
  DMAP(MPI_Reduce),          DMAP(MPI_Op_create),      DMAP(MPI_Op_free),
  DMAP(MPI_Allreduce),       DMAP(MPI_Reduce_scatter), DMAP(MPI_Scan),
  DMAP(MPI_Group_size),      DMAP(MPI_Group_rank),   
  DMAP(MPI_Group_translate_ranks),
  DMAP(MPI_Group_compare),   DMAP(MPI_Comm_group),     DMAP(MPI_Group_union),  
  DMAP(MPI_Group_intersection),
  DMAP(MPI_Group_difference),
  DMAP(MPI_Group_incl),      DMAP(MPI_Group_excl),
  /*
  DMAP(MPI_Group_range_incl),
  DMAP(MPI_Group_range_excl),
  */
  DMAP(MPI_Group_free),      DMAP(MPI_Comm_size),      DMAP(MPI_Comm_rank),
  DMAP(MPI_Comm_compare),    DMAP(MPI_Comm_dup),       DMAP(MPI_Comm_create),
  DMAP(MPI_Comm_split),      DMAP(MPI_Comm_free),     
  DMAP(MPI_Comm_test_inter),
  DMAP(MPI_Comm_remote_size),           DMAP(MPI_Comm_remote_group),
  DMAP(MPI_Intercomm_create),           DMAP(MPI_Intercomm_merge),
  DMAP(MPI_Keyval_create),              DMAP(MPI_Keyval_free),
  DMAP(MPI_Attr_put),        DMAP(MPI_Attr_get),       DMAP(MPI_Attr_delete),
  DMAP(MPI_Topo_test),       DMAP(MPI_Cart_create),    DMAP(MPI_Dims_create),
  DMAP(MPI_Graph_create),    DMAP(MPI_Graphdims_get),  DMAP(MPI_Graph_get),
  DMAP(MPI_Cartdim_get),     DMAP(MPI_Cart_get),       DMAP(MPI_Cart_rank),
  DMAP(MPI_Cart_coords),     DMAP(MPI_Graph_neighbors_count),
  DMAP(MPI_Graph_neighbors), DMAP(MPI_Cart_shift),     DMAP(MPI_Cart_sub),
  DMAP(MPI_Cart_map),        DMAP(MPI_Graph_map),
  DMAP(MPI_Get_processor_name),         DMAP(MPI_Get_version),
  DMAP(MPI_Errhandler_create),          DMAP(MPI_Errhandler_set),
  DMAP(MPI_Errhandler_get),  DMAP(MPI_Errhandler_free),DMAP(MPI_Error_string),
  DMAP(MPI_Error_class),     DMAP(MPI_Wtime),          DMAP(MPI_Wtick),
  DMAP(MPI_Init),            DMAP(MPI_Finalize),       DMAP(MPI_Initialized),
  DMAP(MPI_Abort),           DMAP(MPI_Close_port),     DMAP(MPI_Comm_accept),
  DMAP(MPI_Comm_connect),    DMAP(MPI_Comm_disconnect),
  DMAP(MPI_Comm_get_parent), DMAP(MPI_Comm_join),
  /*
  DMAP(MPI_Comm_spawn),
  DMAP(MPI_Comm_spawn_multiple),
  */
  DMAP(MPI_Lookup_name),     DMAP(MPI_Open_port),      DMAP(MPI_Publish_name),
  DMAP(MPI_Unpublish_name),  DMAP(MPI_Accumulate),     DMAP(MPI_Get),
  DMAP(MPI_Put),             DMAP(MPI_Win_complete),   DMAP(MPI_Win_create),
  DMAP(MPI_Win_fence),       DMAP(MPI_Win_free),       DMAP(MPI_Win_get_group),
  DMAP(MPI_Win_lock),        DMAP(MPI_Win_post),       DMAP(MPI_Win_start),
  DMAP(MPI_Win_test),        DMAP(MPI_Win_unlock),     DMAP(MPI_Win_wait),
  DMAP(MPI_Alltoallw),       DMAP(MPI_Exscan),       
  DMAP(MPI_Add_error_class),            DMAP(MPI_Add_error_code),
  DMAP(MPI_Add_error_string),           DMAP(MPI_Comm_call_errhandler),
  DMAP(MPI_Comm_create_keyval),         DMAP(MPI_Comm_delete_attr),
  DMAP(MPI_Comm_free_keyval),           DMAP(MPI_Comm_get_attr),
  DMAP(MPI_Comm_get_name),   DMAP(MPI_Comm_set_attr),  DMAP(MPI_Comm_set_name),
  DMAP(MPI_File_call_errhandler),       DMAP(MPI_Grequest_complete),
  DMAP(MPI_Grequest_start),  DMAP(MPI_Init_thread),    DMAP(MPI_Is_thread_main),
  DMAP(MPI_Query_thread),               DMAP(MPI_Status_set_cancelled),
  DMAP(MPI_Status_set_elements),        DMAP(MPI_Type_create_keyval),
  DMAP(MPI_Type_delete_attr),           DMAP(MPI_Type_dup),
  DMAP(MPI_Type_free_keyval),           DMAP(MPI_Type_get_attr),
  DMAP(MPI_Type_get_contents),          DMAP(MPI_Type_get_envelope),
  DMAP(MPI_Type_get_name),              DMAP(MPI_Type_set_attr),
  DMAP(MPI_Type_set_name),              DMAP(MPI_Type_match_size),
  DMAP(MPI_Win_call_errhandler),        DMAP(MPI_Win_create_keyval),
  DMAP(MPI_Win_delete_attr),            DMAP(MPI_Win_free_keyval),
  DMAP(MPI_Win_get_attr),    DMAP(MPI_Win_get_name),   DMAP(MPI_Win_set_attr),
  DMAP(MPI_Win_set_name),    DMAP(MPI_Alloc_mem),
  DMAP(MPI_Comm_create_errhandler),     DMAP(MPI_Comm_get_errhandler),
  DMAP(MPI_Comm_set_errhandler),        DMAP(MPI_File_create_errhandler),
  DMAP(MPI_File_get_errhandler),        DMAP(MPI_File_set_errhandler),
  DMAP(MPI_Finalized),       DMAP(MPI_Free_mem),       DMAP(MPI_Get_address),
  DMAP(MPI_Info_create),     DMAP(MPI_Info_delete),    DMAP(MPI_Info_dup),
  DMAP(MPI_Info_free),       DMAP(MPI_Info_get),       DMAP(MPI_Info_get_nkeys),
  DMAP(MPI_Info_get_nthkey),            DMAP(MPI_Info_get_valuelen),
  DMAP(MPI_Info_set),                   DMAP(MPI_Pack_external),
  DMAP(MPI_Pack_external_size),         DMAP(MPI_Request_get_status),
  DMAP(MPI_Type_create_darray),         DMAP(MPI_Type_create_hindexed),
  DMAP(MPI_Type_create_hvector),        DMAP(MPI_Type_create_indexed_block),
  DMAP(MPI_Type_create_resized),        DMAP(MPI_Type_create_struct),
  DMAP(MPI_Type_create_subarray),       DMAP(MPI_Type_get_extent),
  DMAP(MPI_Type_get_true_extent),       DMAP(MPI_Unpack_external),
  DMAP(MPI_Win_create_errhandler),      DMAP(MPI_Win_get_errhandler),
  DMAP(MPI_Win_set_errhandler),         DMAP(MPI_File_open),
  DMAP(MPI_File_close),                 DMAP(MPI_File_delete),
  DMAP(MPI_File_set_size),              DMAP(MPI_File_preallocate),
  DMAP(MPI_File_get_size),              DMAP(MPI_File_get_group),
  DMAP(MPI_File_get_amode),             DMAP(MPI_File_set_info),
  DMAP(MPI_File_get_info),              DMAP(MPI_File_set_view),
  DMAP(MPI_File_get_view),              DMAP(MPI_File_read_at),
  DMAP(MPI_File_read_at_all),           DMAP(MPI_File_write_at),
  DMAP(MPI_File_write_at_all),          DMAP(MPI_File_iread_at),
  DMAP(MPI_File_iwrite_at),             DMAP(MPI_File_read),
  DMAP(MPI_File_read_all),              DMAP(MPI_File_write),
  DMAP(MPI_File_write_all),             DMAP(MPI_File_iread),
  DMAP(MPI_File_iwrite),                DMAP(MPI_File_seek),
  DMAP(MPI_File_get_position),          DMAP(MPI_File_get_byte_offset),
  DMAP(MPI_File_read_shared),           DMAP(MPI_File_write_shared),
  DMAP(MPI_File_iread_shared),          DMAP(MPI_File_iwrite_shared),
  DMAP(MPI_File_read_ordered),          DMAP(MPI_File_write_ordered),
  DMAP(MPI_File_seek_shared),           DMAP(MPI_File_get_position_shared),
  DMAP(MPI_File_read_at_all_begin),     DMAP(MPI_File_read_at_all_end),
  DMAP(MPI_File_write_at_all_begin),    DMAP(MPI_File_write_at_all_end),
  DMAP(MPI_File_read_all_begin),        DMAP(MPI_File_read_all_end),
  DMAP(MPI_File_write_all_begin),       DMAP(MPI_File_write_all_end),
  DMAP(MPI_File_read_ordered_begin),    DMAP(MPI_File_read_ordered_end),
  DMAP(MPI_File_write_ordered_begin),   DMAP(MPI_File_write_ordered_end),
  DMAP(MPI_File_get_type_extent),       DMAP(MPI_Register_datarep),
  DMAP(MPI_File_set_atomicity),         DMAP(MPI_File_get_atomicity),
  DMAP(MPI_File_sync),                  DMAP(MPIO_Test),
  DMAP(MPIO_Wait),                      DMAP(MPIO_Testall),
  DMAP(MPIO_Waitall),                   DMAP(MPIO_Testany),
  DMAP(MPIO_Waitany),                   DMAP(MPIO_Waitsome),
  DMAP(MPIO_Testsome),
  {"Function_enter", report_Function_enter},
  {"Function_exit", report_Function_exit},
  {NULL, NULL}
};

static int handler_count = 0;

static int funcomp(const void *a, const void *b) {
  const funmap *left = (funmap*)a;
  const funmap *right = (funmap*)b;
  return strcmp(left->label, right->label);
}

/* Get a function corresponding to a label */
mpihandler get_handler(const char *func, const fwrapper *wrap) {
  int pivot, old_pivot, lower_bound, upper_bound;
  /* mpihandler retval = NULL; */
  if(! handler_count) {
    for(handler_count = 0; handlers[handler_count].handler; ++handler_count)
      { /* running the counter */ }
    qsort(handlers, handler_count, sizeof(funmap), funcomp);
  }
  /* Bisectional search */
  lower_bound = 0;
  upper_bound = handler_count;
  pivot = upper_bound / 2;
  old_pivot = 0;
  while(1) {
    int cmp = strcmp(func, handlers[pivot].label);
    if(cmp == 0)
      break;
    if(cmp < 0)
      upper_bound = pivot;
    if(cmp > 0)
      lower_bound = pivot;
    pivot = lower_bound + (upper_bound - lower_bound)/2;
    if(pivot == old_pivot) {
      fprintf(stderr, "get_handler:  No handler found for \"%s\" at %s:%ld\n",
              func, wrap->fname, wrap->lineno);
      abort();
    }
    old_pivot = pivot;
  }
  return handlers[pivot].handler;

  /*
  for(i = 0; handlers[i].handler; ++i) {
    if(strcmp(func, handlers[i].label) == 0) {
      retval = handlers[i].handler;
      break;
    }
  }
  if(! retval) {
    fprintf(stderr, "get_handler:  No handler defined for the label %s "
            "at %s:%ld\n", func, wrap->fname, wrap->lineno);
    abort();
  }
  return retval;
  */
}