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

#ifndef DUMPI_LIBUNDUMPI_BINDINGS_H
#define DUMPI_LIBUNDUMPI_BINDINGS_H

#include <dumpi/common/argtypes.h>
#include <dumpi/libundumpi/callbacks.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup libundumpi
   * \defgroup libundumpi_internal  libundumpi/internal: \
   *  Internal routines for libundumpi.
   */
  /*@{*/ /* Add subsequent comments to the libundumpi/internal module */
  
  /**
   * A "catch-all" function for callbacks.  This is (unfortunately)
   * needed to allow us to handle all callbacks using a common mechanism.
   * The actual values of the pointers come from the user-defined 
   * callbacks.
   * \param parsearg   The MPI call arguments (passed to the caller).
   * \param thread     Thread identifier
   * \param cpu        CPU usage at the time of the call
   * \param wall       Wall clock at the time of the call
   * \param perf       Performance counter information (or NULL)
   * \param userarg    Argument defined by user for undumpi_read_stream
   * \return  non-zero on success.  A zero return value will terminate parsing.
   */
  typedef int (*libundumpi_unsafe_fun)(const void *parsearg,
				       uint16_t thread,
				       const dumpi_time *cpu,
				       const dumpi_time *wall,
				       const dumpi_perfinfo *perf,
				       void *userarg);

  /**
   * The common signature for functions that handle parsing individual MPI 
   * calls.
   * \param profile   The input profile that gets read
   * \param callout   The user-defined callback function to notify
   * \param userarg   Argument defined by user for undumpi_read_stream.
   * \return   non-zero on success.  A zero return value will terminate parsing.
   */
  typedef int (*libdumpi_do_callback)(dumpi_profile *profile,
                             libundumpi_unsafe_fun callout, void *userarg);

  /**
   * Associate a handler (parser) and a user-defined callback function
   * with each MPI call.
   */
  typedef struct libundumpi_cbpair {
    /** Parser function for this MPI call.  This must not be NULL */
    libdumpi_do_callback  handler;
    /** Callback function for the MPI call.  
     * If this is NULL, the call is read from the stream but not processed. */
    libundumpi_unsafe_fun   callout;
  } libundumpi_cbpair;

  /**
   * Parse MPI send.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_send(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI recv.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_recv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI get_count.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_get_count(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI bsend.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_bsend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI ssend.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_ssend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI rsend.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_rsend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI buffer_attach.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_buffer_attach(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI buffer_detach.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_buffer_detach(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI isend.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_isend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI ibsend.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_ibsend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI issend.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_issend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI irsend.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_irsend(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI irecv.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_irecv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI wait.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_wait(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI test.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_test(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI request_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_request_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI waitany.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_waitany(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI testany.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_testany(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI waitall.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_waitall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI testall.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_testall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI waitsome.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_waitsome(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI testsome.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_testsome(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI iprobe.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_iprobe(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI probe.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_probe(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cancel.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cancel(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI test_cancelled.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_test_cancelled(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI send_init.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_send_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI bsend_init.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_bsend_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI ssend_init.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_ssend_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI rsend_init.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_rsend_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI recv_init.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_recv_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI start.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_start(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI startall.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_startall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI sendrecv.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_sendrecv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI sendrecv_replace.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_sendrecv_replace(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_contiguous.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_contiguous(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_vector.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_vector(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_hvector.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_hvector(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_indexed.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_indexed(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_hindexed.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_hindexed(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_struct.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_struct(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI address.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_address(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_extent.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_extent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_lb.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_lb(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_ub.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_ub(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_commit.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_commit(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI get_elements.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_get_elements(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI pack.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_pack(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI unpack.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_unpack(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI pack_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_pack_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI barrier.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_barrier(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI bcast.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_bcast(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI gather.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_gather(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI gatherv.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_gatherv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI scatter.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_scatter(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI scatterv.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_scatterv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI allgather.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_allgather(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI allgatherv.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_allgatherv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI alltoall.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_alltoall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI alltoallv.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_alltoallv(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI reduce.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_reduce(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI op_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_op_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI op_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_op_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI allreduce.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_allreduce(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI reduce_scatter.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_reduce_scatter(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI scan.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_scan(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_rank.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_rank(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_translate_ranks.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_translate_ranks(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_compare.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_compare(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_group.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_group(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_union.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_union(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_intersection.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_intersection(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_difference.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_difference(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_incl.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_incl(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_excl.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_excl(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_range_incl.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_range_incl(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_range_excl.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_range_excl(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI group_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_group_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_rank.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_rank(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_compare.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_compare(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_dup.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_dup(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_split.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_split(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_test_inter.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_test_inter(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_remote_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_remote_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_remote_group.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_remote_group(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI intercomm_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_intercomm_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI intercomm_merge.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_intercomm_merge(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI keyval_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_keyval_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI keyval_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_keyval_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI attr_put.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_attr_put(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI attr_get.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_attr_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI attr_delete.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_attr_delete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI topo_test.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_topo_test(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cart_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cart_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI dims_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_dims_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI graph_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_graph_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI graphdims_get.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_graphdims_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI graph_get.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_graph_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cartdim_get.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cartdim_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cart_get.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cart_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cart_rank.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cart_rank(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cart_coords.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cart_coords(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI graph_neighbors_count.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_graph_neighbors_count(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI graph_neighbors.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_graph_neighbors(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cart_shift.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cart_shift(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cart_sub.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cart_sub(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI cart_map.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_cart_map(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI graph_map.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_graph_map(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI get_processor_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_get_processor_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI get_version.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_get_version(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI errhandler_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_errhandler_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI errhandler_set.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_errhandler_set(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI errhandler_get.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_errhandler_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI errhandler_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_errhandler_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI error_string.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_error_string(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI error_class.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_error_class(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI wtime.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_wtime(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI wtick.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_wtick(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI init.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_init(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI finalize.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_finalize(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI initialized.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_initialized(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI abort.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_abort(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI pcontrol.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_pcontrol(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI close_port.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_close_port(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_accept.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_accept(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_connect.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_connect(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_disconnect.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_disconnect(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_get_parent.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_get_parent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_join.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_join(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_spawn.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_spawn(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_spawn_multiple.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_spawn_multiple(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI lookup_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_lookup_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI open_port.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_open_port(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI publish_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_publish_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI unpublish_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_unpublish_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI accumulate.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_accumulate(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI get.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI put.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_put(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_complete.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_complete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_fence.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_fence(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_get_group.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_get_group(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_lock.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_lock(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_post.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_post(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_start.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_start(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_test.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_test(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_unlock.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_unlock(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_wait.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_wait(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI alltoallw.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_alltoallw(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI exscan.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_exscan(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI add_error_class.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_add_error_class(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI add_error_code.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_add_error_code(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI add_error_string.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_add_error_string(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_call_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_call_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_create_keyval.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_create_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_delete_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_delete_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_free_keyval.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_free_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_get_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_get_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_get_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_get_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_set_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_set_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_set_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_set_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_call_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_call_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI grequest_complete.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_grequest_complete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI grequest_start.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_grequest_start(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI init_thread.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_init_thread(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI is_thread_main.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_is_thread_main(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI query_thread.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_query_thread(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI status_set_cancelled.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_status_set_cancelled(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI status_set_elements.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_status_set_elements(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_create_keyval.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_create_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_delete_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_delete_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_dup.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_dup(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_free_keyval.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_free_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_get_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_get_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_get_contents.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_get_contents(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_get_envelope.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_get_envelope(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_get_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_get_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_set_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_set_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_set_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_set_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_match_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_match_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_call_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_call_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_create_keyval.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_create_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_delete_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_delete_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_free_keyval.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_free_keyval(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_get_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_get_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_get_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_get_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_set_attr.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_set_attr(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_set_name.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_set_name(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI alloc_mem.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_alloc_mem(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_create_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_create_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_get_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_get_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI comm_set_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_comm_set_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_create_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_create_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_set_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_set_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI finalized.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_finalized(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI free_mem.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_free_mem(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI get_address.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_get_address(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_create.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_create(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_delete.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_delete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_dup.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_dup(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_free.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_free(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_get.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_get(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_get_nkeys.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_get_nkeys(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_get_nthkey.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_get_nthkey(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_get_valuelen.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_get_valuelen(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI info_set.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_info_set(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI pack_external.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_pack_external(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI pack_external_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_pack_external_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI request_get_status.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_request_get_status(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_create_darray.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_create_darray(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_create_hindexed.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_create_hindexed(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_create_hvector.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_create_hvector(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_create_indexed_block.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_create_indexed_block(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_create_resized.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_create_resized(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_create_struct.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_create_struct(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_create_subarray.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_create_subarray(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_get_extent.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_get_extent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI type_get_true_extent.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_type_get_true_extent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI unpack_external.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_unpack_external(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_create_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_create_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_get_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_get_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI win_set_errhandler.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_win_set_errhandler(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_open.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_open(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_close.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_close(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_delete.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_delete(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_set_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_set_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_preallocate.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_preallocate(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_size.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_size(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_group.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_group(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_amode.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_amode(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_set_info.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_set_info(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_info.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_info(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_set_view.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_set_view(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_view.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_view(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_at.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_at(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_at_all.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_at_all(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_at.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_at(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_at_all.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_at_all(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_iread_at.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_iread_at(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_iwrite_at.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_iwrite_at(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_all.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_all(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_all.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_all(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_iread.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_iread(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_iwrite.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_iwrite(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_seek.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_seek(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_position.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_position(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_byte_offset.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_byte_offset(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_shared.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_shared.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_iread_shared.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_iread_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_iwrite_shared.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_iwrite_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_ordered.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_ordered(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_ordered.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_ordered(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_seek_shared.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_seek_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_position_shared.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_position_shared(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_at_all_begin.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_at_all_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_at_all_end.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_at_all_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_at_all_begin.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_at_all_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_at_all_end.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_at_all_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_all_begin.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_all_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_all_end.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_all_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_all_begin.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_all_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_all_end.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_all_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_ordered_begin.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_ordered_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_read_ordered_end.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_read_ordered_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_ordered_begin.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_ordered_begin(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_write_ordered_end.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_write_ordered_end(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_type_extent.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_type_extent(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI register_datarep.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_register_datarep(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_set_atomicity.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_set_atomicity(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_get_atomicity.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_get_atomicity(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI file_sync.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_file_sync(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPIO test (which frequently is the same as MPI_Test)
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libdumpi_grab_io_test(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPIO wait (which frequently is the same as MPI_Wait)
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libdumpi_grab_io_wait(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPIO testall (which frequently is the same as MPI_Testall)
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libdumpi_grab_io_testall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPIO waitall (which frequently is the same as MPI_Waitall)
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libdumpi_grab_io_waitall(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPIO testany (which frequently is the same as MPI_Testany)
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libdumpi_grab_io_testany(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPIO waitany (which frequently is the same as MPI_Waitany)
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libdumpi_grab_io_waitany(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPIO waitsome (which frequently is the same as MPI_Waitsome)
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libdumpi_grab_io_waitsome(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPIO testsome (which frequently is the same as MPI_Testsome)
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libdumpi_grab_io_testsome(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI all_functions.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_all_functions(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI function_enter.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_function_enter(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Parse MPI function_exit.
   * \param profile   An profile positioned at the start of the MPI call.
   * \param callout   User-defined callback function (possibly NULL)
   * \param uarg      User-defined argument to pass to callback function.
   * \return non-zero on success.
   */
  int libundumpi_grab_function_exit(dumpi_profile *profile, libundumpi_unsafe_fun callout, void *uarg);

  /**
   * Internal function to associate user-specified callbacks with
   * MPI functions.
   * \param callback  The user-defined callback bindings.
   * \param callarr   The array that will store the associations.
   *                  This array must be at least of size DUMPI_END_OF_STREAM.
   */
  void libundumpi_populate_callbacks(const libundumpi_callbacks *callback, 
				     libundumpi_cbpair *callarr);

  /**
   * @brief libundumpi_populate_handlers  A callback has both a handler and a callout.
   *  In some cases it is beneficial to only populate the handlers if you want to walk the trace
   *  but not actually generate any function calls
   * @param callback
   * @param callarr
   */
  void libundumpi_populate_handlers(const libundumpi_callbacks *callback,
             libundumpi_cbpair *callarr);

  void libundumpi_populate_callouts(const libundumpi_callbacks *callback,
             libundumpi_cbpair *callarr);

  /*@}*/ /* close doxygen module */
#ifdef __cplusplus
} /* end of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBUNDUMPI_BINDINGS_H */