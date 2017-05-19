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

#ifndef DUMPI_BIN_OTFWRITER_H
#define DUMPI_BIN_OTFWRITER_H

#include <dumpi/bin/trace.h>
#include <dumpi/bin/metadata.h>
#include <dumpi/bin/otfcomplete.h>
#include <dumpi/libundumpi/libundumpi.h>
#include <dumpi/libundumpi/bindings.h>
#include <otf.h>
#include <string>
#include <vector>
#include <map>

namespace dumpi {

  /**
   * \ingroup dumpi2otf
   */
  /*@{*/

  /**
   * Handle writing OTF traces given pre-parsed state information.
   */
  class otfwriter {
    /// The file manager for our writer.
    OTF_FileManager *mgr_;
    /// Our writer.
    OTF_Writer *writer_;

    /// We stash info about the metadata and trace info here to allow
    /// callbacks to proceed properly.
    const metadata *meta_;
    const std::vector<trace> *info_;
    /// The current rank we're working on (needed for callbacks).
    int current_rank_;

    /// The callbacks we use during preparsing
    libundumpi_callbacks callbacks_;

    /// Completions -- only MPI_Irecv is handled here (I*send operations
    /// are marked as soon as the Isend call is made).
    std::map<uint32_t, otfcomplete> complete_;

    /// Persistent sends or receives are handled here.
    //std::map<uint32_t, otfpersistent> persistent_;

    /// Map function addresses to OTF function labels (local to each node).
    /// We can't just use the address, because it may well have the high-bits set.
    typedef std::map<uint64_t, uint32_t> funcadd_t;
    /// Store a funcadd mapping for each stream.
    typedef std::map<uint32_t, funcadd_t> funclabel_t;
    funclabel_t funclabel_;

  public:
    /// Hello.
    explicit otfwriter(const std::string &fileroot);
  
    /// Goodbye.
    virtual ~otfwriter() throw();

    /// Make magic.
    void write(const metadata &meta, const std::vector<trace> &info);

  private:
    /// Define MPI function labels.  We define MPI functions globally (stream 0)
    void define_mpi_functions();

    /// Define traced functions (if any).  These are defined locally
    /// since there is no guarantee on how they will be ordered.
    void define_local_functions(uint32_t stream, dumpi_profile *profile);

    /// Define performance counter labels.  These are defined locally
    /// since it is not guaranteed (albeit very likely) that they will have
    /// the same value on all nodes.
    void define_perfctr_labels(uint32_t stream, dumpi_profile *profile);

    /// Write the state of perfcounters at function entry.
    void write_counters_enter(uint64_t start, const dumpi_time *cpu,
			      const dumpi_perfinfo *perf);

    /// Write the state of perfcounters at function exit.
    void write_counters_leave(uint64_t stop, const dumpi_time *cpu,
			      const dumpi_perfinfo *perf);

    /// Common functionality for functions that are 'tagged' (entry and exit)
    /// but don't do any communication.
    template <typename T, uint32_t functag>
      static int handle_generic(const T *prm,  uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg);

    /// Common functionality for send, bsend, ssend, rsend,
    /// isend, ibsend, issend, and irsend.  Currently, all send operations
    /// are timestamped when the send starts.  The corresponding function
    /// is also timestamped for entry and exit.
    template <typename T, uint32_t functag>
      static int handle_send(const T *prm,  uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg);

    /// Handle a recv operation.
    static int handle_recv(const dumpi_recv *prm, uint16_t thread,
			   const dumpi_time *cpu, const dumpi_time *wall,
			   const dumpi_perfinfo *perf, void *userarg);

    /// Handle an irecv operation.
    static int handle_irecv(const dumpi_irecv *prm, uint16_t thread,
			    const dumpi_time *cpu, const dumpi_time *wall,
			    const dumpi_perfinfo *perf, void *userarg);

    /// Handle the various test and wait calls.
    /// Unfortunately, there is no clean generic handling for these 
    /// (although one can get pretty far using functors
    /// if nobody objects to read-only code).
    static int handle_wait(const dumpi_wait *prm, uint16_t thread,
			   const dumpi_time *cpu, const dumpi_time *wall,
			   const dumpi_perfinfo *perf, void *userarg);
    static int handle_waitany(const dumpi_waitany *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg);
    static int handle_waitsome(const dumpi_waitsome *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg);
    static int handle_waitall(const dumpi_waitall *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg);
    static int handle_test(const dumpi_test *prm, uint16_t thread,
			   const dumpi_time *cpu, const dumpi_time *wall,
			   const dumpi_perfinfo *perf, void *userarg);
    static int handle_testany(const dumpi_testany *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg);
    static int handle_testsome(const dumpi_testsome *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg);
    static int handle_testall(const dumpi_testall *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg);

    /// Handle all barrier-like calls where we don't know how much info
    /// was transferred. There is often no nice way for us to decipher this
    /// information, so we pretend it's zero bytes sent and received.
    /// Note that otfwriter registers the collective id with the same value
    /// as the typetag.
    template <typename T, uint32_t typetag>
      static int handle_barrier(const T *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg);

    /// Handle a broadcast.  There may be a maintainable way to process
    /// some of these collectives in a more generic manner (but I haven't
    /// found it yet).
    static int handle_bcast(const dumpi_bcast *prm, uint16_t thread,
			    const dumpi_time *cpu, const dumpi_time *wall,
			    const dumpi_perfinfo *perf, void *userarg);

    /// Handle a gather.  Things get a little messy here.
    static int handle_gather(const dumpi_gather *prm, uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg);

    /// Handle a gatherv.  Things get messier still.
    static int handle_gatherv(const dumpi_gatherv *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg);

    /// Handle a scatter.  Largely similar (but opposite) to gather.
    static int handle_scatter(const dumpi_scatter *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg);

    /// Handle a scatterv.  Almost exactly as ugly as gatherv.
    static int handle_scatterv(const dumpi_scatterv *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg);

    /// Handle allgather.  This is actually a bit prettier than gather.
    /// Since we can't safely assume any specific implementation of allgather
    /// (or specific hardware capabilities for collectives) we simply assume
    /// that each node sends and receives N-1 copies of data (where N is the
    /// total number of nodes).
    static int handle_allgather(const dumpi_allgather *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg);

    /// Handle allgatherv.  This is actually a bit prettier than gather.
    static int handle_allgatherv(const dumpi_allgatherv *prm, uint16_t thread,
				 const dumpi_time *cpu, const dumpi_time *wall,
				 const dumpi_perfinfo *perf, void *userarg);

    /// Handle alltoall.  As with allgather, we just assume that every node
    /// sends N-1 copies of its data (this is a correct assumption for direct
    /// communication modes, but not as good for bisectional algorithms).
    static int handle_alltoall(const dumpi_alltoall *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg);

    /// Handle alltoallv.  "Conveniently," this function is designed such that
    /// it is generally impractical to use bisectional algorithms (so the
    /// direct algorithm is normally used instead).
    static int handle_alltoallv(const dumpi_alltoallv *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg);

    /// Handle alltoallw.  Comments regarding MPI design wisely not expressed here.
    static int handle_alltoallw(const dumpi_alltoallw *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg);

    /// Handle reduce, and allreduce.
    ///  We assume that each node sends and receives 
    /// 3/4*(messagelength)*log2(nodes) messages as a rough estimate of
    /// the communication overhead of a binary tree algorithm.
    template <typename T, uint32_t typetag>
      static int handle_reduce(const T *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg);

    /// Handle reduce_scatter.
    ///  We assume that each node sends and receives 
    /// 3/4*(messagelength)*log2(nodes) messages as a rough estimate of
    /// the communication overhead of a binary tree algorithm.
    static int handle_reduce_scatter(const dumpi_reduce_scatter *prm,
				     uint16_t thread,
				     const dumpi_time *cpu, const dumpi_time *wall,
				     const dumpi_perfinfo *perf, void *userarg);

    /// Handle scan and exscan.  We assume a simple chain, so first node only
    /// sends, last node only receives, and all nodes in between receive data
    /// once and send data once.
    template <typename T, uint32_t typetag>
      static int handle_scan(const T *prm, uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg);

    /// Define callbacks for DUMPI parsing.
    void set_callbacks();
  };

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_OTFWRITER_H