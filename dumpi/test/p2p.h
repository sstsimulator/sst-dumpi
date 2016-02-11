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

#ifndef DUMPI_TEST_P2P_H
#define DUMPI_TEST_P2P_H

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \addtogroup testmpi
   */
  /*@{*/

  /* Tests for point-to-point communications */
  
  /** Run through all p2p tests (sends, isends, deferred_sends, sendrecvs) */
  void all_p2p(int rank, int size);
  
  /** send, bsend, rsend, ssend, recv */
  void sends(int rank, int size);
  
  /** isend, ibsend, irsend, issend, irecv */
  void isends(int rank, int size);
  
  /** send_init, bsend_init, ssend_init, rsend_init, recv_init, start, startall 
   *
   * Still need to add MPI_Request_free here.
   **/
  void deferred_sends(int rank, int size);
  
  /** sendrecv, sendrecv_replace */
  void sendrecvs(int rank, int size);

  /*@}*/

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* DUMPI_TEST_P2P_H */
