/**
Copyright 2009-2022 National Technology and Engineering Solutions of Sandia,
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S. Government
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly
owned subsidiary of Honeywell International, Inc., for the U.S. Department of
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2022, NTESS

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
/**
Copyright 2009-2022 National Technology and Engineering Solutions of Sandia,
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S. Government
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly
owned subsidiary of Honeywell International, Inc., for the U.S. Department of
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2022, NTESS

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
    profile->pos = ftello(profile->file);
    if(profile->pos >= end_stream) {
      retval = 0;
    }
  }
  return retval;
}

int undumpi_read_stream(dumpi_profile* profile,
      const libundumpi_callbacks *callback,
      void *uarg, bool print_progress)
{
  //by default, don't print progress or terminate early
  return undumpi_read_stream_full("", profile,callback,uarg,print_progress);
}

/* Read all MPI calls off a stream */
int undumpi_read_stream_full(
  const char* metaname,
  dumpi_profile* profile,
  const libundumpi_callbacks *callback,
  void *uarg,
  bool print_progress)
{
  int mpi_finalized = 0;
  /* dumpi_function currfunc; */
  libundumpi_cbpair callarr[DUMPI_END_OF_STREAM] = {{NULL, NULL}};
  assert(profile != NULL && profile->file != NULL && callback != NULL);

  libundumpi_populate_handlers(callback, callarr);
  libundumpi_populate_callouts(callback, callarr);

  /* Go */
  mpi_finalized = 0;
  assert(dumpi_start_stream_read(profile) != 0);
   //print every percent progress
  int last_percent_done = 0;
  while(undumpi_read_single_call(profile, callarr, uarg, &mpi_finalized) &&
        profile->pos < profile->terminate_pos) {
    double percent_done = 100. * ((double)profile->pos) / ((double)profile->total_file_size);

    /** Check if we should print anything */
    int int_percent_done = (int) percent_done;
    if (print_progress && (int_percent_done > last_percent_done)){
      printf("DUMPI trace %3d percent complete: %s\n", 
        int_percent_done, metaname);
      fflush(stdout);
    }
    last_percent_done = int_percent_done;
  }
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
