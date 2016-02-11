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

#ifndef DUMPI_TEST_COLL_H
#define DUMPI_TEST_COLL_H

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \defgroup testmpi  testmpi:  Coverage/correctness testing for the \
   *     DUMPI MPI bindings.
   */
  /*@{*/

  /** Run through all collective tests. */
  void all_collectives(int rank, int size);

  /* Families of collectives */
  
  /** bcast, scatter, scatterv, gather, gatherv, reduce, reduce_scatter
   * (reduce_scatter is actually not rootless, but it fits better here). */
  void rooted_collectives(int rank, int size);

  /** barrier, allgather, allgatherv, allreduce, alltoall, alltoallv, scan */
  void rootless_collectives(int rank, int size);

  /*@}*/

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_TEST_COLL_H */
