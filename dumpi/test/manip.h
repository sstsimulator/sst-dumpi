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

#ifndef DUMPI_TEST_MANIP_H
#define DUMPI_TEST_MANIP_H

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \addtogroup testmpi
   */
  /*@{*/

  /** Run through all probing/waiting tests. */
  void all_manips(int rank, int size);

  /** Sizes, ranks, etc. */
  void queries(int rank, int size);

  /** Just probe calls. */
  void probes(int rank, int size);

  /** Tests, waits, and cancels */
  void tests(int rank, int size);

  /** Create and destroy datatypes */
  void types(int rank, int size);

  /** Create and destroy ops. */
  void ops(int rank, int size);

  /** Create and destroy groups. */
  void groups(int rank, int size);

  /** Create and destroy communicators. */
  void comms(int rank, int size);

  /*@}*/

#ifdef __cplusplus
} /* End of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_TEST_MANIP_H */
