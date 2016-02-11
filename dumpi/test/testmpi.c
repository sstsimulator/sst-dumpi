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

/* Run through a series of MPI tests (intended to test dumpi capture) */

#include <dumpi/test/p2p.h>
#include <dumpi/test/coll.h>
#include <dumpi/test/manip.h>
#include <mpi.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int rank, size;
  /*
  char buf[1024]; buf[1023] = '\0';
  snprintf(buf, 1023, "testmpi-stderr-%05d", (int)getpid());
  fprintf(stderr, "Redirecting stderr to %s\n", buf);
  freopen(buf, "w", stderr);
  */
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  /* fprintf(stderr, "main:  Rank %d\n", rank); */
  if(! rank)
    printf("point-to-point tests\n");
  all_p2p(rank, size);
  if(! rank)
    printf("collective tests\n");
  all_collectives(rank, size);
  if(! rank)
    printf("various manipulations\n");
  all_manips(rank, size);

  MPI_Finalize();
  return 0;
}
