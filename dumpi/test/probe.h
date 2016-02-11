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

#ifndef DUMPI_TEST_PROBE_H
#define DUMPI_TEST_PROBE_H

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \addtogroup testmpi
   */
  /*@{*/

  /** Run through all probing/waiting tests. */
  void all_probes(int rank, int size);

  /** Just probe calls. */
  void probes(int rank, int size);

  /** Just test calls. */
  void tests(int rank, int size);

  /** Just wait calls. */
  void waits(int rank, int size);

  /** Cancel and friends. */
  void cancels(int rank, int size);

  /*@}*/

#ifdef __cplusplus
} /* End of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_TEST_PROBE_H */
