/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009-2011 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
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

