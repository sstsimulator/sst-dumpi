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

#include <mpi.h>
#include <stdio.h>
#include <string.h>

/* Test MPI+pthreads with various thread support levels. */
/* MPI_THREAD_MULTIPLE:  Any/all threads may communicate concurrently */
/* MPI_THREAD_SERIALIZED: Any thread may communicate, but only one at a time */
/* MPI_THREAD_FUNNELED:  Multiple threads, but only one communicates */
/* MPI_THREAD_SINGLE:  Only a single thread will execute */

static inline const char* tsupp(int level) {
#define TSUP(LEVEL) case LEVEL: return #LEVEL
  switch(level) {
    TSUP(MPI_THREAD_MULTIPLE);
    TSUP(MPI_THREAD_SERIALIZED);
    TSUP(MPI_THREAD_FUNNELED);
    TSUP(MPI_THREAD_SINGLE);
  }
  /* We get here on bogus input. */
  fprintf(stderr, "Invalid thread support level %d\n", level);
  return "INVALID";
}

static inline int ssupp(int argc, char **argv) {
#define SSUPP(ARG, LEVEL) if(strcmp(ARG, #LEVEL) == 0) retval = LEVEL
  int i, retval = MPI_THREAD_SINGLE - 1;
  for(i = 1; i < argc; ++i) {
    char *arg = argv[i];
    SSUPP(arg, MPI_THREAD_MULTIPLE);
    SSUPP(arg, MPI_THREAD_SERIALIZED);
    SSUPP(arg, MPI_THREAD_FUNNELED);
    SSUPP(arg, MPI_THREAD_SINGLE);
  }
  return retval;
}

void test_thread_multiple();
void test_thread_serialized();
void test_thread_funneled();

int main(int argc, char **argv) {
  int required, provided;
  /* technically, MPI_Init* is to be the first consumer of argc/argv */
  required = ssupp(argc, argv);
  if(required < MPI_THREAD_SINGLE) {
    fprintf(stderr, "Usage: %s (MPI_THREAD_MULTIPLE|MPI_THREAD_SERIALIZED|"
            "MPI_THREAD_FUNNELED|MPI_THREAD_SINGLE)\n", argv[0]);
    return 1;
  }
  MPI_Init_thread(&argc, &argv, required, &provided);
  if(provided < required) {
    fprintf(stderr, "Insufficient thread support.  Required %s, "
            "but only %s is provided\n", tsupp(required), tsupp(provided));
    return 2;
  }
  switch(required) {
  case MPI_THREAD_MULTIPLE:
    test_thread_multiple();
    break;
  case MPI_THREAD_SERIALIZED:
    test_thread_serialized();
    break;
  case MPI_THREAD_FUNNELED:
    test_thread_funneled();
    break;
  case MPI_THREAD_SINGLE:
    fprintf(stderr, "Use the 'testmpi' program to test single-threaded MPI\n");
    break;
  default:
    fprintf(stderr, "Invalid thread support level %d\n", required);
  }
  MPI_Finalize();
  return 0;
}