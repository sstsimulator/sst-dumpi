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

#include <dumpi/libundumpi/libundumpi.h>
#include <dumpi/libundumpi/bindings.h>
#include <dumpi/common/funcs.h>
#include <dumpi/common/iodefs.h>
#include <assert.h>
#include <sys/time.h>

/*
typedef int (*unsafe_fun)(void *parsearg, void *userarg);
typedef int (*do_callback)(dumpi_profile *profile, unsafe_fun callout,
			   void *userarg);
*/

/* This gets called when we're ready to disregard type safety */
/*
static void populate_callbacks(const libundumpi_callbacks *callback,
			       libundumpi_cbpair *callarr);
*/

/*
 * Open a trace file for reading.
 * It is the caller's job to clean up using undumpi_close.
 */
dumpi_profile* undumpi_open(const char* fname) {
  return dumpi_open_input_file(fname);
}

void undumpi_close(dumpi_profile *profile) {
  assert(profile != NULL && profile->file != NULL);
  fclose(profile->file);
}

/*
 * Get the header information from the given file.
 * \param profile the file that gets read.
 * \retval        the header record (release using dumpi_header_free()).
 */
dumpi_header* undumpi_read_header(dumpi_profile* profile) {
  dumpi_header *retval = dumpi_build_header();
  dumpi_read_header(profile, retval);
  return retval;
}

/* Read a single MPI call off a stream starting at current position.
 * Returns 1 if the stream is still active, 0 if it is terminated.
 * Note that you need to call dumpi_start_stream_read before calling
 * this method for the first time */
int undumpi_read_single_call(dumpi_profile *profile,
			     libundumpi_cbpair *callarr,
			     void *uarg,
			     int *mpi_finalized)
{
  dumpi_function currfunc;
  int retval = 0;
  off_t end_stream = profile->footer;
  if((currfunc = dumpi_read_next_function(profile)) < DUMPI_END_OF_STREAM) {
    retval = 1;
    /*
    printf("  Currently %ld bytes into the stream\n",
	   (long)(ftello(profile->file) - profile->body));
    */
    assert(callarr[currfunc].handler != NULL);
    if(*mpi_finalized && (currfunc == 0)) {
      /* Backward compatibility issue -- we used to terminate differently
	 (this was before adding function profiling). */
      retval = 0;
    }
    if(currfunc == DUMPI_Finalize) {
      /* Backward compatibility issue -- we used to terminate the stream here */
      *mpi_finalized = 1;
    }
    assert(callarr[currfunc].handler(profile, callarr[currfunc].callout, uarg));
    /*
    printf("After reading function %d (%s), filepos is at %ld (end at %ld)\n",
	   (int)currfunc, dumpi_function_label(currfunc),
	   ftello(profile->file), end_stream);
    */
    if(ftello(profile->file) >= end_stream) {
      retval = 0;
    }
  }
  return retval;
}

int undumpi_read_stream(dumpi_profile* profile,
      const libundumpi_callbacks *callback,
      void *uarg)
{
  //by default, don't print progress or terminate early
  return undumpi_read_stream_full("", profile,callback,uarg,false,-1);
}

static double
get_time()
{
  struct timeval t_st;
  gettimeofday(&t_st, 0);
  double t = t_st.tv_sec + 1e-6 * t_st.tv_usec;
  return t;
}

/* Read all MPI calls off a stream */
int undumpi_read_stream_full(
  const char* metaname,
  dumpi_profile* profile,
  const libundumpi_callbacks *callback,
  void *uarg,
  bool print_progress,
  double percent_terminate)
{
  static bool terminated = false;
  int mpi_finalized = 0;
  /* dumpi_function currfunc; */
  libundumpi_cbpair callarr[DUMPI_END_OF_STREAM] = {{NULL, NULL}};
  assert(profile != NULL && profile->file != NULL && callback != NULL);

  libundumpi_populate_handlers(callback, callarr);

  /** First loop through and figure out number of fxns, if necessary */
  bool terminate_early = percent_terminate > 0;
  long num_dumpi_fxns = 0;
  if (print_progress || terminate_early){
    assert(dumpi_start_stream_read(profile) != 0);
    while (undumpi_read_single_call(profile, callarr, uarg, &mpi_finalized)){
      ++num_dumpi_fxns;
    }
  }

  libundumpi_populate_callouts(callback,callarr);

  double t_start = get_time();

  /* Go */
  mpi_finalized = 0;
  assert(dumpi_start_stream_read(profile) != 0);
  long num_fxns_called = 0;
   //print every percent progress
  int last_percent_done = 0;
  while(undumpi_read_single_call(profile, callarr, uarg, &mpi_finalized)) {
    ++num_fxns_called;
    double percent_done = 100. * ((double)num_fxns_called) / ((double)num_dumpi_fxns);
    //if anyone has terminated, hence the OR
    terminated = terminated || (terminate_early && percent_done >= percent_terminate);

    /** Check if we should print anything */
    int int_percent_done = (int) percent_done;
    if (print_progress && (int_percent_done > last_percent_done)){
      double t_elapsed = get_time() - t_start;
      //printf("DUMPI trace %3d percent complete: %20.12f seconds elapsed\n", 
      //  int_percent_done, t_elapsed);
      printf("DUMPI trace %3d percent complete: %s\n", 
        int_percent_done, metaname);
    }
    last_percent_done = int_percent_done;

    /** Check if we should terminate early */
    if (terminated){
      break;
    }
  }

#if 0
  off_t end_stream;
  end_stream = profile->footer;
  while((currfunc = dumpi_read_next_function(profile)) < DUMPI_END_OF_STREAM) {
    /*
    printf("  Currently %ld bytes into the stream\n",
	   (long)(ftello(profile->file) - profile->body));
    */
    assert(callarr[currfunc].handler != NULL);
    if(mpi_finalized && (currfunc == 0)) {
      /* Backward compatibility issue -- we used to terminate differently
	 (this was before adding function profiling). */
      break;
    }
    if(currfunc == DUMPI_Finalize) {
      /* Backward compatibility issue -- we used to terminate the stream here */
      mpi_finalized = 1;
    }
    assert(callarr[currfunc].handler(profile, callarr[currfunc].callout, uarg));
    /*
    printf("After reading function %d (%s), filepos is at %ld (end at %ld)\n",
	   (int)currfunc, dumpi_function_label(currfunc),
	   ftello(profile->file), end_stream);
    */
    if(ftello(profile->file) >= end_stream)
      break;
  }
#endif
  return 1;
}

dumpi_keyval_record* undumpi_read_keyval_record(dumpi_profile* profile) {
  dumpi_keyval_record* retval = (dumpi_keyval_record*)calloc(1, sizeof(dumpi_keyval_record));
  assert(retval != NULL);
  dumpi_read_keyval_record(profile, retval);
  return retval;
}

dumpi_footer* undumpi_read_footer(dumpi_profile* profile) {
  dumpi_footer* retval = (dumpi_footer*)calloc(1, sizeof(dumpi_footer));
  dumpi_read_footer(profile, retval);
  return retval;
}

dumpi_sizeof undumpi_read_datatype_sizes(dumpi_profile *profile) {
  dumpi_sizeof retval;
  dumpi_read_datatype_sizes(profile, &retval);
  return retval;
}


