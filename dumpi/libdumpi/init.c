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

#include <dumpi/libdumpi/init.h>
#include <dumpi/libdumpi/data.h>
#include <dumpi/libdumpi/callprofile.h>
#include <dumpi/libdumpi/mpibindings-maps.h>
#include <dumpi/common/perfctrtags.h>
#include <dumpi/common/perfctrs.h>
#include <dumpi/common/io.h>
#include <dumpi/common/iodefs.h>
#include <dumpi/common/debugflags.h>
#include <dumpi/common/gettime.h>
#include <mpi.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>

#ifdef DUMPI_USE_PTHREADS
#include <pthread.h>

static pthread_mutex_t *init_lock = NULL;
#define DUMPI_LOCK_MUTEX   assert(pthread_mutex_lock(init_lock) == 0)
#define DUMPI_UNLOCK_MUTEX assert(pthread_mutex_unlock(init_lock) == 0)

#else

#define DUMPI_LOCK_MUTEX
#define DUMPI_UNLOCK_MUTEX

#endif

/*
 * Read configuration information from the given file.
 */
static void dumpi_setup(void);
static void dumpi_finish_profiling(void);
static void init_global_output(void);
static void default_config(void);
static void read_config(FILE* fp);
static void open_output_file(void);
static void process_keyval(const char *key, const char *value);
static void create_meta_file(void);


/****************************************************/

/*
 * Initialize storage and read in configuration file.
 * 
 * Storage gets freed up automatically at exit.
 * This is a no-op if(dumpi_global != NULL).
 */
void libdumpi_init(void) {
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: libdumpi_init entering\n");
  if(dumpi_global == NULL) {
    char *dumpiconf;
    FILE *fp;
#ifdef DUMPI_USE_PTHREADS
    if(init_lock == NULL) {
      init_lock = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
      assert(pthread_mutex_init(init_lock, NULL) == 0);
    }
#endif
    DUMPI_LOCK_MUTEX;
    /* Initialize storage */
    dumpi_setup();
    /* Read in dumpi configuration (if available) */
    dumpiconf = getenv("DUMPI_CONF");
    if(dumpiconf && ((fp = fopen(dumpiconf, "r")) != NULL)) {
      read_config(fp);
    }
    else if((fp = fopen("dumpi.conf", "r")) != NULL) {
      read_config(fp);
    }
    /* Set any undefined configuration options to a reasonable value */
    default_config();
    /* and initialize PAPI stuff (if requested and supported) */
    assert(dumpi_global != NULL);
    dumpi_init_perfctrs(dumpi_global->perf);
    /* Finally, initialize the profile but leave the file unopened */
    {
      dumpi_clock cpu, wall;
      dumpi_get_time(&cpu, &wall);
      int cpuoffset = cpu.sec;
      int walloffset = wall.sec;
      dumpi_global->profile =
        dumpi_alloc_output_profile(cpuoffset, walloffset, 0);
    }
  }
  assert(atexit(libdumpi_finalize) == 0);
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: libdumpi_init returning\n");
  DUMPI_UNLOCK_MUTEX;
}

/*
 * Open output files (each node creates its onw dump file;
 * node 0 also creates a human-readable metafile to describe the simulation).
 *
 * Note that in a parallel run (if size > 0), node 0 broadcasts
 * its start time (as stored in dumpi_global::header::starttime)
 * to be used for all filenames (this call updates at least
 * dumpi_global::file_root, dumpi_global::output_file,
 * and dumpi_global::profile).
 */
void libdumpi_open_files(void) {
  int mpi_active;
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: libdumpi_open_files entering\n");
  dumpi_setup();
  PMPI_Initialized(&mpi_active);
  if(! mpi_active) {
    fprintf(stderr, "libdumpi_open_files:  Error:  MPI is not initialized.");
    abort();
  }
  open_output_file();
  /* dumpi_start_stream_write(dumpi_global->profile); */
  create_meta_file();
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: libdumpi_open_files returning\n");
}

/*
 * Finalize libdumpi and free all dynamically allocated storage.
 * This gets called automatically at exit (using atexit(3)), but
 * it is safe to call this method manually.
 */
void libdumpi_finalize() {
  DUMPI_LOCK_MUTEX;
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: libdumpi_finalize entering\n");
  if(dumpi_global != NULL) {
    if(dumpi_global->profile == NULL) {
      libdumpi_open_files();
    }
    dumpi_finish_profiling();
    dumpi_free_keyval_record(dumpi_global->keyval);
    free(dumpi_global->output);
    free((void*)dumpi_global->output_file);
    free(dumpi_global->footer);
    free(dumpi_global->typesize.size);
    free(dumpi_global->perf);
    free((void*)dumpi_global->file_root);
#ifdef DUMPI_USE_PTHREADS
    pthread_mutex_destroy(&dumpi_global->mutex);
#endif /* ! DUMPI_USE_PTHREADS */
    free(dumpi_global);
    /* dumpi_free_maps(); */
    dumpi_global = NULL;
  }
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: libdumpi_finalize returning\n");
  DUMPI_UNLOCK_MUTEX;
}


/****************************************************/


/*
 * Initialize dumpi - read configure variables, etc.
 */
void dumpi_setup() {  
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: dumpi_setup entering\n");
  if(! dumpi_global) {
    dumpi_global = (dumpi_global_t*)calloc(1, sizeof(dumpi_global_t));
    assert(dumpi_global != NULL);
#ifdef DUMPI_USE_PTHREADS
    assert(pthread_mutex_init(&dumpi_global->mutex, NULL) == 0);
#endif /* ! DUMPI_USE_PTHREADS */
    dumpi_global->comm_rank = -getpid();
  }
  if(! dumpi_global->output) {
    init_global_output();
  }
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: dumpi_setup returning\n");
}

/*
 * Allocate a profile.
 */
void init_global_output(void) {
  int i;
  dumpi_global->output = (dumpi_outputs*)calloc(1,sizeof(dumpi_outputs));
  assert(dumpi_global->output != NULL);
  for(i = 0; i < DUMPI_END_OF_STREAM; ++i)
    dumpi_global->output->function[i] = -1;
  dumpi_global->output->function[DUMPI_Function_enter] = 1;
  dumpi_global->output->function[DUMPI_Function_exit] = 1;
  /* Other storage field in the dumpi_global */
  /* Note that dumpi_build_header populates all its fields (inc. starttime) */
  dumpi_global->header = dumpi_build_header();
  assert(dumpi_global->header);
  dumpi_global->footer = (dumpi_footer*)calloc(1, sizeof(dumpi_footer));
  assert(dumpi_global->footer != NULL);
  dumpi_global->perf = (dumpi_perfinfo*)calloc(1, sizeof(dumpi_perfinfo));
  assert(dumpi_global->perf != NULL);
  dumpi_global->output->timestamps = -1;
  dumpi_global->output->statuses = -1;
}

void dumpi_finish_profiling(void) {
  int i, addr_count;
  uint64_t *addrs = NULL;
  char **names = NULL;
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: dumpi_finish_profiling entering\n");  
  dumpi_write_header(dumpi_global->profile, dumpi_global->header);
  dumpi_write_footer(dumpi_global->profile, dumpi_global->footer);
  dumpi_write_keyval_record(dumpi_global->profile, dumpi_global->keyval);
  dumpi_write_perfctr_labels(dumpi_global->profile,
			     dumpi_active_perfctrs(), dumpi_perfctr_labels());
  dumpi_write_datatype_sizes(dumpi_global->profile, &dumpi_global->typesize);
  /* Need to add an index entry in the same way as for perfctr labels */
  addr_count = dumpi_profile_unique_address_count();
  addrs = dumpi_profile_unique_addresses();
  names = (char**)calloc(addr_count+1, sizeof(const char*));
  assert(names != NULL);
  for(i = 0; i < addr_count; ++i)
    names[i] = strdup(dumpi_function_name(addrs[i]));
  dumpi_write_function_addresses(dumpi_global->profile, 
				 addr_count, addrs, names);
  free(addrs);
  for(i = 0; i < addr_count; ++i) free(names[i]);
  free(names);
  dumpi_write_index(dumpi_global->profile);
  if(dumpi_debug & DUMPI_DEBUG_LIBDUMPI)
    fprintf(stderr, "[DUMPI-LIBDUMPI]: libdumpi_finish_profiling returning\n");
}

/*
 * Set up somewhat sensible default parameters.
 */
void default_config(void) {
  int fun;
  assert(dumpi_global->output);
  /* Assign default values to other things that haven't been configured yet. */
  if(dumpi_global->output->timestamps < 0)
    dumpi_global->output->timestamps = DUMPI_TIME_FULL;
  if(dumpi_global->output->statuses < 0)
    dumpi_global->output->statuses = DUMPI_ENABLE;
  if(dumpi_global->output->function[DUMPI_ALL_FUNCTIONS] < 0)
    dumpi_global->output->function[DUMPI_ALL_FUNCTIONS] = DUMPI_ENABLE;
  for(fun = 0; fun < DUMPI_ALL_FUNCTIONS; ++fun)
    if(dumpi_global->output->function[fun] < 0) 
      dumpi_global->output->function[fun] = dumpi_global->output->function[DUMPI_ALL_FUNCTIONS];
  /* Turn stuff on by default -- instrumented code can override this later */
  dumpi_global->output->function[DUMPI_ALL_FUNCTIONS] = DUMPI_ENABLE;
}

void read_config(FILE* fp) {
#define DUMPI_BUZ 1024
  int i, len;
  char buffer[DUMPI_BUZ];
  char strtok_buf[DUMPI_BUZ];
  char *sbp;
  char *key, *value;
  buffer[DUMPI_BUZ-1] = '\0';
  sbp = strtok_buf;
  while(! feof(fp)) {
    if(fgets(buffer, DUMPI_BUZ, fp) == 0) {
      break;
    }
    /* fprintf(stderr, "DEBUG:  input line %s\n", buffer); */
    len = strlen(buffer);
    /* Trim out comments */
    for(i = 0; i < len; ++i) {
      if(buffer[i] == '#' || buffer[i] == '\n') {
	buffer[i] = '\0';
	break;
      }
    }
    /* Tokenize */
    key = strtok_r(buffer, " \t=", &sbp);
    if(key) {
      value = strtok_r(NULL, " \t=", &sbp);
      if(value)
	process_keyval(key, value);
    }
  }
}

void open_output_file(void) {
  int mpi_initialized;
  int count;
  char *fname;
  const char *tmpfroot;
  char *scratch;
  long rank0time;
  time_t thetime;
  /*
  char scratchcwd[512];
  char *cwd;
  */
  struct tm loct;
  assert(dumpi_global != NULL);
  assert(dumpi_global->header != NULL);
  rank0time = dumpi_global->header->starttime;
  PMPI_Initialized(&mpi_initialized);
  if(mpi_initialized) {
    PMPI_Comm_rank(MPI_COMM_WORLD, &dumpi_global->comm_rank);
    PMPI_Comm_size(MPI_COMM_WORLD, &dumpi_global->comm_size);
  }
  else {
    dumpi_global->comm_rank = 0;
    dumpi_global->comm_size = 1;
  }
  if(dumpi_global->comm_size > 1) {
    assert(PMPI_Bcast(&rank0time,1,MPI_LONG,0,MPI_COMM_WORLD) == MPI_SUCCESS);
  }
  if(dumpi_global->file_root == NULL) {
    tmpfroot = "dumpi";
  }
  else {
    tmpfroot = dumpi_global->file_root;
  }
  count = strlen(tmpfroot) + 30;
  scratch = (char*)malloc(count); scratch[count-1] = '\0';
  thetime = rank0time;
  localtime_r(&thetime, &loct);
  snprintf(scratch, count-1, "%s-%04d.%02d.%02d.%02d.%02d.%02d",
	   tmpfroot, loct.tm_year+1900, loct.tm_mon+1, loct.tm_mday,
	   loct.tm_hour, loct.tm_min, loct.tm_sec);
  if(dumpi_global->file_root) {
    free((void*)dumpi_global->file_root);
  }
  dumpi_global->file_root = scratch;
  /*
  cwd = getcwd(scratchcwd, 512);
  count = (cwd ? strlen(cwd)+1 : 0) + strlen(dumpi_global->file_root) + 30;
  */
  count = strlen(dumpi_global->file_root) + 30;
  fname = (char*)malloc(count); fname[count-1] = '\0';
  /*
  snprintf(fname, count-1, "%s%s%s-%04d.bin",
	   (cwd ? cwd : ""), (cwd ? "/" : ""), 
	   dumpi_global->file_root, dumpi_global->comm_rank);
  */
  snprintf(fname, count-1, "%s-%04d.bin",
	   dumpi_global->file_root, dumpi_global->comm_rank);
  dumpi_global->output_file = fname;
  
  dumpi_global->profile->file = dumpi_open_output_file(fname);
  assert(dumpi_global->profile->file != NULL);
  dumpi_global->keyval = dumpi_alloc_keyval_record();
  assert(dumpi_global->profile != NULL && dumpi_global->profile->file != NULL);
}

dumpi_setting profiling_to_setting(const char *value) {
  if(strcmp(value, "disable") == 0) return DUMPI_DISABLE;
  if(strcmp(value, "success") == 0) return DUMPI_SUCCESS;
  if(strcmp(value, "enable")  == 0) return DUMPI_ENABLE;
  fprintf(stderr, "profiling_to_setting:  No match for the value %s\n", value);
  abort();
}

dumpi_timestamps get_timestamping(const char *value) {
  if(strcmp(value, "none") == 0) return DUMPI_TIME_NONE;
  if(strcmp(value, "cpu")  == 0) return DUMPI_TIME_CPU;
  if(strcmp(value, "wall") == 0) return DUMPI_TIME_WALL;
  if(strcmp(value, "full") == 0) return DUMPI_TIME_FULL;
  fprintf(stderr, "get_timestamp:  No match for the value %s\n", value);
  abort();
}

void process_keyval(const char *key, const char *value) {
  /* Ugly */
  int offset;
  /* fprintf(stderr, "key=%s   :  value=%s\n", key, value); */
  /* First test whether we are setting preferences for an MPI function name. */
  if(strncmp(key, "MPI", 3) == 0) {
    for(offset = 0; dumpi_function_names[offset] != NULL; ++offset) {
      if(strcmp(key, dumpi_function_names[offset]) == 0) {
	if(dumpi_global->output->function[offset] < 0) {
	  /* We don't want to override values set directly from the application */
	  /*fprintf(stderr, "Setting %s (%d) to %s\n", key, offset, value);*/
	  dumpi_global->output->function[offset] = profiling_to_setting(value);
	}
	return;
      }
    }
    /* Well, let's check whether we are grabbing MPI_Default. */
    if(strcmp(key, "MPI_Default") == 0) {
      if(dumpi_global->output->function[DUMPI_ALL_FUNCTIONS] < 0) {
	/* This option has not been previously specified */
	dumpi_global->output->function[DUMPI_ALL_FUNCTIONS] = profiling_to_setting(value);
      }
      return;
    } 
  }
  /*
   * Next test whether we are setting any of the PAPI flags.
   * We only set as many of those as will fit into dumpi_perfinfo::counter_tag
   */
  if(strncmp(key, "PAPI", 4) == 0) {
    if(dumpi_global->perf->count < DUMPI_MAX_PERFCTRS) {
      assert(strlen(value) < DUMPI_MAX_PERFCTR_NAME);
      strcpy(dumpi_global->perf->counter_tag[dumpi_global->perf->count], value);
      ++dumpi_global->perf->count;
      dumpi_global->output->perfinfo = 1;
      return;
    }
    else {
      fprintf(stderr, "Sorry, DUMPI currently only supports %d PAPI "
	      "perfcounter labels as input.  This will hopefully be fixed"
	      "in future versions.  Ignoring perfctr label %s\n",
	      (int)(DUMPI_MAX_PERFCTRS), value);
      return;
    }
  }
  /* Are we configuring call tracing? */
  if(strcmp(key, "tracing") == 0) {
    fprintf(stderr, "Call tracing: %s %s\n", key, value);
    if(strcmp(value, "enable") == 0) {
      dumpi_global->output->function[DUMPI_Function_enter] = 1;
      dumpi_global->output->function[DUMPI_Function_exit]  = 1;
    }
    else if(strcmp(value, "disable") == 0) {
      dumpi_global->output->function[DUMPI_Function_enter] = 0;
      dumpi_global->output->function[DUMPI_Function_exit]  = 0;
    }
    else if(strcmp(value, "enter") == 0) {
      dumpi_global->output->function[DUMPI_Function_enter] = 1;
    }
    else if(strcmp(value, "exit") == 0) {
      dumpi_global->output->function[DUMPI_Function_exit]  = 1;
    }
    else {
      fprintf(stderr, "dumpi:  Configure option \"%s\" with invalid value %s\n",
	      "tracing", value);
      assert(0);
    }
    return;
  }
  /* OK, so let's see whether this is the status flag. */
  if(strcmp(key, "statuses") == 0) {
    if(dumpi_global->output->statuses < 0) {
      /* This flag has not been previously specified */
      dumpi_global->output->statuses = profiling_to_setting(value);
    }
    return;
  }
  /* Is this the fileroot? */
  if(strcmp(key, "fileroot") == 0) {
    if(dumpi_global->file_root == NULL) {
      /* Only do this if a different fileroot has not been specified */
      char *tmp_file_root = (char*)malloc(strlen(value) + 1);
      strncpy(tmp_file_root, value, strlen(value)+1);
      dumpi_global->file_root = tmp_file_root;
      return;
    }
  }
  /* The second-to-last option is the timestamp setting */
  if(strcmp(key, "timestamp") == 0) {
    if(dumpi_global->output->timestamps < 0) {
      /* Not specified yet. */
      dumpi_global->output->timestamps = get_timestamping(value);
    }
    return;
  }
  /* Finally, we check for debug flags. */
  if(strcmp(key, "debug") == 0) {
    if((strcmp(value, "none") == 0) || (strcmp(value, "off") == 0))
      dumpi_debug = DUMPI_DEBUG_NONE;
    else if(strcmp(value, "all") == 0)
      dumpi_debug = DUMPI_DEBUG_ALL;
    else if(strcmp(value, "traceio") == 0)
      dumpi_debug |= DUMPI_DEBUG_TRACEIO;
    else if(strcmp(value, "mpicalls") == 0)
      dumpi_debug |= DUMPI_DEBUG_MPICALLS;
    else if(strcmp(value, "libdumpi") == 0)
      dumpi_debug |= DUMPI_DEBUG_LIBDUMPI;
    else
      fprintf(stderr, "dumpi: Unknown debug flag: %s\n", value);
    return;
  }
  /* We only get here if the key/value pair was not matched. */
  fprintf(stderr, "process_keyval:  No match for key %s and value %s\n", 
	  key, value);
}


void create_meta_file(void) {
  char buffer[100];
  
  sprintf(buffer, "%s.meta", dumpi_global->file_root);
  FILE *df = fopen(buffer, "w");
  assert(df != NULL);
  
  fprintf(df, "hostname=%s\n", dumpi_global->header->hostname);
  fprintf(df, "numprocs=%d\n", dumpi_global->comm_size);
  fprintf(df, "username=%s\n", dumpi_global->header->username);
  fprintf(df, "startime=%llu\n", (long long)dumpi_global->header->starttime);
  fprintf(df, "fileprefix=%s\n", dumpi_global->file_root);
  fprintf(df, "version=%d\nsubversion=%d\nsubsubversion=%d\n", 
	  dumpi_global->header->version[0], 
	  dumpi_global->header->version[1], 
	  dumpi_global->header->version[2]);
  fclose(df);
}


