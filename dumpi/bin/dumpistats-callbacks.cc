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

#include <dumpi/bin/dumpistats-callbacks.h>
#include <dumpi/common/funclabels.h>
#include <dumpi/common/argtypes.h>
#include <iostream>

namespace dumpi {

  template <typename thetype, dumpi_function thelabel>
  int report_generic(const thetype *prm, uint16_t thread,
                     const dumpi_time *cpu, const dumpi_time *wall,
                     const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    cb->handle(thelabel, thread, cpu, wall, perf, 0, -1, 0, -1, prm);
    return 1;
  }

  static
  int report_function_enter(const dumpi_func_call *prm, uint16_t thread,
                            const dumpi_time *cpu, const dumpi_time *wall,
                            const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    //std::cerr << "report_function_enter(" << prm << ", " << thread
    //          << ", " << cpu << ", " << wall << ", perfinfo*, " << uarg
    //          << ")\n";
    cb->annotate(prm, thread, cpu, wall, perf);
    return 1;
  }

  static
  int report_MPI_Send(const dumpi_send *prm, uint16_t thread,
                      const dumpi_time *cpu, const dumpi_time *wall,
                      const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Send, thread, cpu, wall, perf,
               prm->count * datasize, dest,
               0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Recv(const dumpi_recv *prm, uint16_t thread,
                      const dumpi_time *cpu, const dumpi_time *wall,
                      const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int source = cb->globalrank(prm->source, prm->comm, wall->start);
    cb->handle(DUMPI_Recv, thread, cpu, wall, perf, 0, -1,
               prm->count * datasize, source, prm);
    return 1;
  }

  static
  int report_MPI_Bsend(const dumpi_bsend *prm, uint16_t thread,
                       const dumpi_time *cpu, const dumpi_time *wall,
                       const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Bsend, thread, cpu, wall, perf,
               prm->count * datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Ssend(const dumpi_ssend *prm, uint16_t thread,
                       const dumpi_time *cpu, const dumpi_time *wall,
                       const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Ssend, thread, cpu, wall, perf,
               prm->count * datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Rsend(const dumpi_rsend *prm, uint16_t thread,
                       const dumpi_time *cpu, const dumpi_time *wall,
                       const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Rsend, thread, cpu, wall, perf,
               prm->count * datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Isend(const dumpi_isend *prm, uint16_t thread,
                       const dumpi_time *cpu, const dumpi_time *wall,
                       const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Isend, thread, cpu, wall, perf,
               prm->count * datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Ibsend(const dumpi_ibsend *prm, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Ibsend, thread, cpu, wall, perf,
               prm->count * datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Issend(const dumpi_issend *prm, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Issend, thread, cpu, wall, perf,
               prm->count * datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Irsend(const dumpi_irsend *prm, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Irsend, thread, cpu, wall, perf,
               prm->count * datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Irecv(const dumpi_irecv *prm, uint16_t thread,
                       const dumpi_time *cpu, const dumpi_time *wall,
                       const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int source = cb->globalrank(prm->source, prm->comm, wall->start);
    cb->handle(DUMPI_Irecv, thread, cpu, wall, perf, 0, -1,
               prm->count * datasize, source, prm);
    return 1;
  }

  static
  int report_MPI_Send_init(const dumpi_send_init *prm, uint16_t thread,
                           const dumpi_time *cpu, const dumpi_time *wall,
                           const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Send_init, thread, cpu, wall, perf,
               prm->count*datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Bsend_init(const dumpi_bsend_init *prm, uint16_t thread,
                            const dumpi_time *cpu, const dumpi_time *wall,
                            const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Bsend_init, thread, cpu, wall, perf,
               prm->count*datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Ssend_init(const dumpi_ssend_init *prm, uint16_t thread,
                            const dumpi_time *cpu, const dumpi_time *wall,
                            const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Ssend_init, thread, cpu, wall, perf,
               prm->count*datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Rsend_init(const dumpi_rsend_init *prm, uint16_t thread,
                            const dumpi_time *cpu, const dumpi_time *wall,
                            const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    cb->handle(DUMPI_Rsend_init, thread, cpu, wall, perf,
               prm->count*datasize, dest, 0, -1, prm);
    return 1;
  }

  static
  int report_MPI_Recv_init(const dumpi_recv_init *prm, uint16_t thread,
                           const dumpi_time *cpu, const dumpi_time *wall,
                           const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t datasize = cb->typesize(prm->datatype, wall->start);
    int source = cb->globalrank(prm->source, prm->comm, wall->start);
    cb->handle(DUMPI_Recv_init, thread, cpu, wall, perf, 0, -1,
               prm->count * datasize, source, prm);
    return 1;
  }

  static
  int report_MPI_Sendrecv(const dumpi_sendrecv *prm, uint16_t thread,
                          const dumpi_time *cpu, const dumpi_time *wall,
                          const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t sendsize = cb->typesize(prm->sendtype, wall->start);
    int64_t recvsize = cb->typesize(prm->recvtype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    int source = cb->globalrank(prm->source, prm->comm, wall->start);
    cb->handle(DUMPI_Sendrecv, thread, cpu, wall, perf,
               prm->sendcount * sendsize, dest,
               prm->recvcount * recvsize, source, prm);
    return 1;
  }

  static
  int report_MPI_Sendrecv_replace(const dumpi_sendrecv_replace *prm,
                                  uint16_t thread, const dumpi_time *cpu,
                                  const dumpi_time *wall,
                                  const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t size = cb->typesize(prm->datatype, wall->start);
    int dest = cb->globalrank(prm->dest, prm->comm, wall->start);
    int source = cb->globalrank(prm->source, prm->comm, wall->start);
    cb->handle(DUMPI_Sendrecv_replace, thread, cpu, wall, perf,
               prm->count * size, dest,
               prm->count * size, source, prm);
    return 1;
  }

  static
  int report_MPI_Bcast(const dumpi_bcast *prm, uint16_t thread,
                       const dumpi_time *cpu, const dumpi_time *wall,
                       const dumpi_perfinfo *perf, void *uarg)
  {
    // TODO:  Decide on what sort of send size we want to report.
    callbacks *cb = (callbacks*)uarg;
    int64_t size = prm->count * cb->typesize(prm->datatype, wall->start);
    // int commsize = cb->commsize(prm->comm, wall->start);
    if(cb->commrank(prm->comm, wall->start) == prm->root)
      cb->handle(DUMPI_Bcast, thread, cpu, wall, perf,
                 size /* *commsize */, -1, 0, 0, prm);
    else
      cb->handle(DUMPI_Bcast, thread, cpu, wall, perf, 0, -1, size, -1, prm);
    return 1;
  }

  static
  int report_MPI_Gather(const dumpi_gather *prm, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int dest = -1;
    int source = -1;
    int64_t recvsize = 0;
    if(cb->commrank(prm->comm, wall->start) == prm->root) {
      int64_t unitsend = (cb->commsize(prm->comm, wall->start))*prm->recvcount;
      int64_t typesize = cb->typesize(prm->recvtype, wall->start);
      recvsize = unitsend * typesize;
    }
    int64_t sendsize = (prm->sendcount *
                        cb->typesize(prm->sendtype, wall->start));
    cb->handle(DUMPI_Gather, thread, cpu, wall, perf, sendsize, dest,
               recvsize, source, prm);
    return 1;
  }

  static
  int report_MPI_Gatherv(const dumpi_gatherv *prm, uint16_t thread,
                         const dumpi_time *cpu, const dumpi_time *wall,
                         const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int dest = -1;
    int source = -1;
    int64_t recvsize = 0;
    if(cb->commrank(prm->comm, wall->start) == prm->root) {
      int commsize = cb->commsize(prm->comm, wall->start);
      int64_t typesize = cb->typesize(prm->recvtype, wall->start);
      for(int i = 0; i < commsize; ++i) {
        int64_t unitsend = prm->recvcounts[i];
        recvsize += unitsend * typesize;
      }
    }
    int64_t sendsize = (prm->sendcount *
                        cb->typesize(prm->sendtype, wall->start));
    cb->handle(DUMPI_Gatherv, thread, cpu, wall, perf, sendsize, dest,
               recvsize, source, prm);
    return 1;
  }

  static
  int report_MPI_Scatter(const dumpi_scatter *prm, uint16_t thread,
                         const dumpi_time *cpu, const dumpi_time *wall,
                         const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t sendsize = 0;
    int source = -1;
    int dest = -1;
    if(cb->commrank(prm->comm, wall->start) == prm->root) {
      int64_t unit = (cb->commsize(prm->comm, wall->start))*prm->sendcount;
      int64_t typesize = cb->typesize(prm->sendtype, wall->start);
      sendsize = unit * typesize;
    }
    int64_t recvsize = (prm->recvcount *
                        cb->typesize(prm->recvtype, wall->start));
    cb->handle(DUMPI_Scatter, thread, cpu, wall, perf, sendsize, dest,
               recvsize, source, prm);
    return 1;
  }

  static
  int report_MPI_Scatterv(const dumpi_scatterv *prm, uint16_t thread,
                          const dumpi_time *cpu, const dumpi_time *wall,
                          const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t sendsize = 0;
    int source = -1;
    int dest = -1;
    if(cb->commrank(prm->comm, wall->start) == prm->root) {
      int commsize = cb->commsize(prm->comm, wall->start);
      int64_t typesize = cb->typesize(prm->sendtype, wall->start);
      for(int i = 0; i < commsize; ++i) {
        int64_t unitsend = prm->sendcounts[i];
        sendsize += unitsend * typesize;
      }
    }
    int64_t recvsize = (prm->recvcount *
                        cb->typesize(prm->recvtype, wall->start));
    cb->handle(DUMPI_Scatterv, thread, cpu, wall, perf, sendsize, dest,
               recvsize, source, prm);
    return 1;
  }

  static
  int report_MPI_Allgather(const dumpi_allgather *prm, uint16_t thread,
                           const dumpi_time *cpu, const dumpi_time *wall,
                           const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t sendsize = cb->typesize(prm->sendtype, wall->start)*prm->sendcount;
    int64_t recvsize = cb->typesize(prm->recvtype, wall->start)*prm->recvcount;
    cb->handle(DUMPI_Allgather, thread, cpu, wall, perf,
               sendsize, -1, recvsize, -1, prm);
    return 1;
  }

  static
  int report_MPI_Allgatherv(const dumpi_allgatherv *prm, uint16_t thread,
                            const dumpi_time *cpu, const dumpi_time *wall,
                            const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int sendsize = cb->typesize(prm->sendtype, wall->start)*prm->sendcount;
    int recvsize = 0;
    int recvtypesize = cb->typesize(prm->recvtype, wall->start);
    int commsize = cb->commsize(prm->comm, wall->start);
    for(int i = 0; i < commsize; ++i)
      recvsize += recvtypesize * prm->recvcounts[i];
    cb->handle(DUMPI_Allgatherv, thread, cpu, wall, perf,
               sendsize, -1, recvsize, -1, prm);
    return 1;
  }

  static
  int report_MPI_Alltoall(const dumpi_alltoall *prm, uint16_t thread,
                          const dumpi_time *cpu, const dumpi_time *wall,
                          const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int commsize = cb->commsize(prm->comm, wall->start);
    int64_t sendtypesize = cb->typesize(prm->sendtype, wall->start);
    int64_t sendsize = sendtypesize * prm->sendcount * commsize;
    int64_t recvtypesize = cb->typesize(prm->recvtype, wall->start);
    int64_t recvsize = recvtypesize * prm->recvcount * commsize;
    cb->handle(DUMPI_Alltoall, thread, cpu, wall, perf,
               sendsize, -1, recvsize, -1, prm);
    return 1;
  }

  static
  int report_MPI_Alltoallv(const dumpi_alltoallv *prm, uint16_t thread,
                           const dumpi_time *cpu, const dumpi_time *wall,
                           const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t sendtypesize = cb->typesize(prm->sendtype, wall->start);
    int64_t recvtypesize = cb->typesize(prm->recvtype, wall->start);
    int commsize = cb->commsize(prm->comm, wall->start);
    int64_t sendsize = 0, recvsize = 0;
    for(int i = 0; i < commsize; ++i) {
      sendsize += sendtypesize * prm->sendcounts[i];
      recvsize += recvtypesize * prm->recvcounts[i];
    }
    cb->handle(DUMPI_Alltoallv, thread, cpu, wall, perf,
               sendsize, -1, recvsize, -1, prm);
    return 1;
  }

  static
  int report_MPI_Reduce(const dumpi_reduce *prm, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int dest = -1;
    int source = -1;
    int64_t typesize = cb->typesize(prm->datatype, wall->start);
    int64_t sendsize = prm->count * typesize;
    int64_t recvsize = 0;
    if(cb->commrank(prm->comm, wall->start) == prm->root) {
      recvsize = sendsize * cb->commsize(prm->comm, wall->start);
    }
    cb->handle(DUMPI_Reduce, thread, cpu, wall, perf,
               sendsize, dest, recvsize, source, prm);
    return 1;
  }

  static
  int report_MPI_Allreduce(const dumpi_allreduce *prm, uint16_t thread,
                           const dumpi_time *cpu, const dumpi_time *wall,
                           const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t typesize = cb->typesize(prm->datatype, wall->start);
    int64_t unitsize = prm->count * typesize;
    int64_t totalsize = unitsize; // * cb->commsize(prm->comm, wall->start);
    cb->handle(DUMPI_Allreduce, thread, cpu, wall, perf,
               totalsize, -1, totalsize, -1, prm);
    return 1;
  }

  static
  int report_MPI_Reduce_scatter(const dumpi_reduce_scatter *prm,
                                uint16_t thread, const dumpi_time *cpu,
                                const dumpi_time *wall,
                                const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t typesize = cb->typesize(prm->datatype, wall->start);
    int64_t commsize = cb->commsize(prm->comm, wall->start);
    int64_t elements = 0;
    for(int i = 0; i < commsize; ++i)
      elements += prm->recvcounts[i];
    int64_t basesize = elements * typesize;
    cb->handle(DUMPI_Reduce, thread, cpu, wall, perf,
               basesize, -1, basesize, -1, prm);  
    return 1;
  }

  static
  int report_MPI_Scan(const dumpi_scan *prm, uint16_t thread,
                      const dumpi_time *cpu, const dumpi_time *wall,
                      const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t typesize = cb->typesize(prm->datatype, wall->start);
    int64_t basesize = prm->count * typesize;
    int commsize = cb->commsize(prm->comm, wall->start);
    int commrank = cb->commrank(prm->comm, wall->start);
    int64_t sendsize = basesize, recvsize = basesize;
    int global_source = -1, global_dest = -1;
    if(commsize > 0) {
      if(commrank == 0) {
        recvsize = 0;
      }
      else if(commrank >= (commsize-1)) {
        sendsize = 0;
      }
    }
    cb->handle(DUMPI_Scan, thread, cpu, wall, perf,
               sendsize, global_dest, recvsize, global_source, prm);
    return 1;
  }

  static
  int report_MPI_Alltoallw(const dumpi_alltoallw *prm, uint16_t thread,
                           const dumpi_time *cpu, const dumpi_time *wall,
                           const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int commsize = cb->commsize(prm->comm, wall->start);
    int64_t sendsize = 0, recvsize = 0;
    for(int i = 0; i < commsize; ++i) {
      int64_t sendtypesize = cb->typesize(prm->sendtypes[i], wall->start);
      int64_t recvtypesize = cb->typesize(prm->recvtypes[i], wall->start);
      sendsize += sendtypesize * prm->sendcounts[i];
      recvsize += recvtypesize * prm->recvcounts[i];
    }
    cb->handle(DUMPI_Alltoallw, thread, cpu, wall, perf,
               sendsize, -1, recvsize, -1, prm);
    return 1;
  }

  static
  int report_MPI_Exscan(const dumpi_exscan *prm, uint16_t thread,
                        const dumpi_time *cpu, const dumpi_time *wall,
                        const dumpi_perfinfo *perf, void *uarg)
  {
    callbacks *cb = (callbacks*)uarg;
    int64_t typesize = cb->typesize(prm->datatype, wall->start);
    int64_t basesize = prm->count * typesize;
    int commsize = cb->commsize(prm->comm, wall->start);
    int commrank = cb->commrank(prm->comm, wall->start);
    int64_t sendsize = basesize, recvsize = basesize;
    int global_source = -1, global_dest = -1;
    if(commrank == 0) {
      recvsize = 0;
    }
    else if(commrank >= (commsize-1)) {
      sendsize = 0;
    }
    cb->handle(DUMPI_Scan, thread, cpu, wall, perf,
               sendsize, global_dest, recvsize, global_source, prm);
    return 1;
  }

  //
  // Set up callbacks.
  //
  callbacks::callbacks() {
    memset(&cb, 1, sizeof(libundumpi_callbacks));
    cb.on_send = report_MPI_Send;
    cb.on_recv = report_MPI_Recv;
    cb.on_get_count = report_generic<dumpi_get_count, DUMPI_Get_count>;
    cb.on_bsend = report_MPI_Bsend;
    cb.on_ssend = report_MPI_Ssend;
    cb.on_rsend = report_MPI_Rsend;
    cb.on_buffer_attach =
      report_generic<dumpi_buffer_attach, DUMPI_Buffer_attach>;
    cb.on_buffer_detach =
      report_generic<dumpi_buffer_detach, DUMPI_Buffer_detach>;
    cb.on_isend = report_MPI_Isend;
    cb.on_ibsend = report_MPI_Ibsend;
    cb.on_issend = report_MPI_Issend;
    cb.on_irsend = report_MPI_Irsend;
    cb.on_irecv = report_MPI_Irecv;
    cb.on_wait = report_generic<dumpi_wait, DUMPI_Wait>;
    cb.on_test = report_generic<dumpi_test, DUMPI_Test>;
    cb.on_request_free = report_generic<dumpi_request_free, DUMPI_Request_free>;
    cb.on_waitany = report_generic<dumpi_waitany, DUMPI_Waitany>;
    cb.on_testany = report_generic<dumpi_testany, DUMPI_Testany>;
    cb.on_waitall = report_generic<dumpi_waitall, DUMPI_Waitall>;
    cb.on_testall = report_generic<dumpi_testall, DUMPI_Testall>;
    cb.on_waitsome = report_generic<dumpi_waitsome, DUMPI_Waitsome>;
    cb.on_testsome = report_generic<dumpi_testsome, DUMPI_Testsome>;
    cb.on_iprobe = report_generic<dumpi_iprobe, DUMPI_Iprobe>;
    cb.on_probe = report_generic<dumpi_probe, DUMPI_Probe>;
    cb.on_cancel = report_generic<dumpi_cancel, DUMPI_Cancel>;
    cb.on_test_cancelled =
      report_generic<dumpi_test_cancelled, DUMPI_Test_cancelled>;
    cb.on_send_init = report_MPI_Send_init;
    cb.on_bsend_init = report_MPI_Bsend_init;
    cb.on_ssend_init = report_MPI_Ssend_init;
    cb.on_rsend_init = report_MPI_Rsend_init;
    cb.on_recv_init = report_MPI_Recv_init;
    cb.on_start = report_generic<dumpi_start, DUMPI_Start>;
    cb.on_startall = report_generic<dumpi_startall, DUMPI_Startall>;
    cb.on_sendrecv = report_MPI_Sendrecv;
    cb.on_sendrecv_replace = report_MPI_Sendrecv_replace;
    cb.on_type_contiguous =
      report_generic<dumpi_type_contiguous, DUMPI_Type_contiguous>;
    cb.on_type_vector = report_generic<dumpi_type_vector, DUMPI_Type_vector>;
    cb.on_type_hvector = report_generic<dumpi_type_hvector, DUMPI_Type_hvector>;
    cb.on_type_indexed = report_generic<dumpi_type_indexed, DUMPI_Type_indexed>;
    cb.on_type_hindexed =
      report_generic<dumpi_type_hindexed, DUMPI_Type_hindexed>;
    cb.on_type_struct = report_generic<dumpi_type_struct, DUMPI_Type_struct>;
    cb.on_address = report_generic<dumpi_address, DUMPI_Address>;
    cb.on_type_extent = report_generic<dumpi_type_extent, DUMPI_Type_extent>;
    cb.on_type_size = report_generic<dumpi_type_size, DUMPI_Type_size>;
    cb.on_type_lb = report_generic<dumpi_type_lb, DUMPI_Type_lb>;
    cb.on_type_ub = report_generic<dumpi_type_ub, DUMPI_Type_ub>;
    cb.on_type_commit = report_generic<dumpi_type_commit, DUMPI_Type_commit>;
    cb.on_type_free = report_generic<dumpi_type_free, DUMPI_Type_free>;
    cb.on_get_elements = report_generic<dumpi_get_elements, DUMPI_Get_elements>;
    cb.on_pack = report_generic<dumpi_pack, DUMPI_Pack>;
    cb.on_unpack = report_generic<dumpi_unpack, DUMPI_Unpack>;
    cb.on_pack_size = report_generic<dumpi_pack_size, DUMPI_Pack_size>;
    cb.on_barrier = report_generic<dumpi_barrier, DUMPI_Barrier>;
    cb.on_bcast = report_MPI_Bcast;
    cb.on_gather = report_MPI_Gather;
    cb.on_gatherv = report_MPI_Gatherv;
    cb.on_scatter = report_MPI_Scatter;
    cb.on_scatterv = report_MPI_Scatterv;
    cb.on_allgather = report_MPI_Allgather;
    cb.on_allgatherv = report_MPI_Allgatherv;
    cb.on_alltoall = report_MPI_Alltoall;
    cb.on_alltoallv = report_MPI_Alltoallv;
    cb.on_reduce = report_MPI_Reduce;
    cb.on_op_create = report_generic<dumpi_op_create, DUMPI_Op_create>;
    cb.on_op_free = report_generic<dumpi_op_free, DUMPI_Op_free>;
    cb.on_allreduce = report_MPI_Allreduce;
    cb.on_reduce_scatter = report_MPI_Reduce_scatter;
    cb.on_scan = report_MPI_Scan;
    cb.on_group_size = report_generic<dumpi_group_size, DUMPI_Group_size>;
    cb.on_group_rank = report_generic<dumpi_group_rank, DUMPI_Group_rank>;
    cb.on_group_translate_ranks =
      report_generic<dumpi_group_translate_ranks, DUMPI_Group_translate_ranks>;
    cb.on_group_compare =
      report_generic<dumpi_group_compare, DUMPI_Group_compare>;
    cb.on_comm_group = report_generic<dumpi_comm_group, DUMPI_Comm_group>;
    cb.on_group_union = report_generic<dumpi_group_union, DUMPI_Group_union>;
    cb.on_group_intersection =
      report_generic<dumpi_group_intersection, DUMPI_Group_intersection>;
    cb.on_group_difference =
      report_generic<dumpi_group_difference, DUMPI_Group_difference>;
    cb.on_group_incl = report_generic<dumpi_group_incl, DUMPI_Group_incl>;
    cb.on_group_excl = report_generic<dumpi_group_excl, DUMPI_Group_excl>;
    cb.on_group_range_incl =
      report_generic<dumpi_group_range_incl, DUMPI_Group_range_incl>;
    cb.on_group_range_excl =
      report_generic<dumpi_group_range_excl, DUMPI_Group_range_excl>;
    cb.on_group_free = report_generic<dumpi_group_free, DUMPI_Group_free>;
    cb.on_comm_size = report_generic<dumpi_comm_size, DUMPI_Comm_size>;
    cb.on_comm_rank = report_generic<dumpi_comm_rank, DUMPI_Comm_rank>;
    cb.on_comm_compare = report_generic<dumpi_comm_compare, DUMPI_Comm_compare>;
    cb.on_comm_dup = report_generic<dumpi_comm_dup, DUMPI_Comm_dup>;
    cb.on_comm_create = report_generic<dumpi_comm_create, DUMPI_Comm_create>;
    cb.on_comm_split = report_generic<dumpi_comm_split, DUMPI_Comm_split>;
    cb.on_comm_free = report_generic<dumpi_comm_free, DUMPI_Comm_free>;
    cb.on_comm_test_inter =
      report_generic<dumpi_comm_test_inter, DUMPI_Comm_test_inter>;
    cb.on_comm_remote_size =
      report_generic<dumpi_comm_remote_size, DUMPI_Comm_remote_size>;
    cb.on_comm_remote_group =
      report_generic<dumpi_comm_remote_group, DUMPI_Comm_remote_group>;
    cb.on_intercomm_create =
      report_generic<dumpi_intercomm_create, DUMPI_Intercomm_create>;
    cb.on_intercomm_merge =
      report_generic<dumpi_intercomm_merge, DUMPI_Intercomm_merge>;
    cb.on_keyval_create =
      report_generic<dumpi_keyval_create, DUMPI_Keyval_create>;
    cb.on_keyval_free = report_generic<dumpi_keyval_free, DUMPI_Keyval_free>;
    cb.on_attr_put = report_generic<dumpi_attr_put, DUMPI_Attr_put>;
    cb.on_attr_get = report_generic<dumpi_attr_get, DUMPI_Attr_get>;
    cb.on_attr_delete = report_generic<dumpi_attr_delete, DUMPI_Attr_delete>;
    cb.on_topo_test = report_generic<dumpi_topo_test, DUMPI_Topo_test>;
    cb.on_cart_create = report_generic<dumpi_cart_create, DUMPI_Cart_create>;
    cb.on_dims_create = report_generic<dumpi_dims_create, DUMPI_Dims_create>;
    cb.on_graph_create = report_generic<dumpi_graph_create, DUMPI_Graph_create>;
    cb.on_graphdims_get =
      report_generic<dumpi_graphdims_get, DUMPI_Graphdims_get>;
    cb.on_graph_get = report_generic<dumpi_graph_get, DUMPI_Graph_get>;
    cb.on_cartdim_get = report_generic<dumpi_cartdim_get, DUMPI_Cartdim_get>;
    cb.on_cart_get = report_generic<dumpi_cart_get, DUMPI_Cart_get>;
    cb.on_cart_rank = report_generic<dumpi_cart_rank, DUMPI_Cart_rank>;
    cb.on_cart_coords = report_generic<dumpi_cart_coords, DUMPI_Cart_coords>;
    cb.on_graph_neighbors_count =
      report_generic<dumpi_graph_neighbors_count, DUMPI_Graph_neighbors_count>;
    cb.on_graph_neighbors =
      report_generic<dumpi_graph_neighbors, DUMPI_Graph_neighbors>;
    cb.on_cart_shift = report_generic<dumpi_cart_shift, DUMPI_Cart_shift>;
    cb.on_cart_sub = report_generic<dumpi_cart_sub, DUMPI_Cart_sub>;
    cb.on_cart_map = report_generic<dumpi_cart_map, DUMPI_Cart_map>;
    cb.on_graph_map = report_generic<dumpi_graph_map, DUMPI_Graph_map>;
    cb.on_get_processor_name =
      report_generic<dumpi_get_processor_name, DUMPI_Get_processor_name>;
    cb.on_get_version = report_generic<dumpi_get_version, DUMPI_Get_version>;
    cb.on_errhandler_create =
      report_generic<dumpi_errhandler_create, DUMPI_Errhandler_create>;
    cb.on_errhandler_set =
      report_generic<dumpi_errhandler_set, DUMPI_Errhandler_set>;
    cb.on_errhandler_get =
      report_generic<dumpi_errhandler_get, DUMPI_Errhandler_get>;
    cb.on_errhandler_free =
      report_generic<dumpi_errhandler_free, DUMPI_Errhandler_free>;
    cb.on_error_string = report_generic<dumpi_error_string, DUMPI_Error_string>;
    cb.on_error_class = report_generic<dumpi_error_class, DUMPI_Error_class>;
    cb.on_wtime = report_generic<dumpi_wtime, DUMPI_Wtime>;
    cb.on_wtick = report_generic<dumpi_wtick, DUMPI_Wtick>;
    cb.on_init = report_generic<dumpi_init, DUMPI_Init>;
    cb.on_finalize = report_generic<dumpi_finalize, DUMPI_Finalize>;
    cb.on_initialized = report_generic<dumpi_initialized, DUMPI_Initialized>;
    cb.on_abort = report_generic<dumpi_abort, DUMPI_Abort>;
    cb.on_close_port = report_generic<dumpi_close_port, DUMPI_Close_port>;
    cb.on_comm_accept = report_generic<dumpi_comm_accept, DUMPI_Comm_accept>;
    cb.on_comm_connect = report_generic<dumpi_comm_connect, DUMPI_Comm_connect>;
    cb.on_comm_disconnect =
      report_generic<dumpi_comm_disconnect, DUMPI_Comm_disconnect>;
    cb.on_comm_get_parent =
      report_generic<dumpi_comm_get_parent, DUMPI_Comm_get_parent>;
    cb.on_comm_join = report_generic<dumpi_comm_join, DUMPI_Comm_join>;
    cb.on_comm_spawn = report_generic<dumpi_comm_spawn, DUMPI_Comm_spawn>;
    cb.on_comm_spawn_multiple =
      report_generic<dumpi_comm_spawn_multiple, DUMPI_Comm_spawn_multiple>;
    cb.on_lookup_name = report_generic<dumpi_lookup_name, DUMPI_Lookup_name>;
    cb.on_open_port = report_generic<dumpi_open_port, DUMPI_Open_port>;
    cb.on_publish_name = report_generic<dumpi_publish_name, DUMPI_Publish_name>;
    cb.on_unpublish_name =
      report_generic<dumpi_unpublish_name, DUMPI_Unpublish_name>;
    cb.on_accumulate = report_generic<dumpi_accumulate, DUMPI_Accumulate>;
    cb.on_get = report_generic<dumpi_get, DUMPI_Get>;
    cb.on_put = report_generic<dumpi_put, DUMPI_Put>;
    cb.on_win_complete = report_generic<dumpi_win_complete, DUMPI_Win_complete>;
    cb.on_win_create = report_generic<dumpi_win_create, DUMPI_Win_create>;
    cb.on_win_fence = report_generic<dumpi_win_fence, DUMPI_Win_fence>;
    cb.on_win_free = report_generic<dumpi_win_free, DUMPI_Win_free>;
    cb.on_win_get_group =
      report_generic<dumpi_win_get_group, DUMPI_Win_get_group>;
    cb.on_win_lock = report_generic<dumpi_win_lock, DUMPI_Win_lock>;
    cb.on_win_post = report_generic<dumpi_win_post, DUMPI_Win_post>;
    cb.on_win_start = report_generic<dumpi_win_start, DUMPI_Win_start>;
    cb.on_win_test = report_generic<dumpi_win_test, DUMPI_Win_test>;
    cb.on_win_unlock = report_generic<dumpi_win_unlock, DUMPI_Win_unlock>;
    cb.on_win_wait = report_generic<dumpi_win_wait, DUMPI_Win_wait>;
    cb.on_alltoallw = report_MPI_Alltoallw;
    cb.on_exscan = report_MPI_Exscan;
    cb.on_add_error_class =
      report_generic<dumpi_add_error_class, DUMPI_Add_error_class>;
    cb.on_add_error_code =
      report_generic<dumpi_add_error_code, DUMPI_Add_error_code>;
    cb.on_add_error_string =
      report_generic<dumpi_add_error_string, DUMPI_Add_error_string>;
    cb.on_comm_call_errhandler =
      report_generic<dumpi_comm_call_errhandler, DUMPI_Comm_call_errhandler>;
    cb.on_comm_create_keyval =
      report_generic<dumpi_comm_create_keyval, DUMPI_Comm_create_keyval>;
    cb.on_comm_delete_attr =
      report_generic<dumpi_comm_delete_attr, DUMPI_Comm_delete_attr>;
    cb.on_comm_free_keyval =
      report_generic<dumpi_comm_free_keyval, DUMPI_Comm_free_keyval>;
    cb.on_comm_get_attr =
      report_generic<dumpi_comm_get_attr, DUMPI_Comm_get_attr>;
    cb.on_comm_get_name =
      report_generic<dumpi_comm_get_name, DUMPI_Comm_get_name>;
    cb.on_comm_set_attr =
      report_generic<dumpi_comm_set_attr, DUMPI_Comm_set_attr>;
    cb.on_comm_set_name =
      report_generic<dumpi_comm_set_name, DUMPI_Comm_set_name>;
    cb.on_file_call_errhandler =
      report_generic<dumpi_file_call_errhandler, DUMPI_File_call_errhandler>;
    cb.on_grequest_complete =
      report_generic<dumpi_grequest_complete, DUMPI_Grequest_complete>;
    cb.on_grequest_start =
      report_generic<dumpi_grequest_start, DUMPI_Grequest_start>;
    cb.on_init_thread =
      report_generic<dumpi_init_thread, DUMPI_Init_thread>;
    cb.on_is_thread_main =
      report_generic<dumpi_is_thread_main, DUMPI_Is_thread_main>;
    cb.on_query_thread =
      report_generic<dumpi_query_thread, DUMPI_Query_thread>;
    cb.on_status_set_cancelled =
      report_generic<dumpi_status_set_cancelled, DUMPI_Status_set_cancelled>;
    cb.on_status_set_elements =
      report_generic<dumpi_status_set_elements, DUMPI_Status_set_elements>;
    cb.on_type_create_keyval =
      report_generic<dumpi_type_create_keyval, DUMPI_Type_create_keyval>;
    cb.on_type_delete_attr =
      report_generic<dumpi_type_delete_attr, DUMPI_Type_delete_attr>;
    cb.on_type_dup = report_generic<dumpi_type_dup, DUMPI_Type_dup>;
    cb.on_type_free_keyval =
      report_generic<dumpi_type_free_keyval, DUMPI_Type_free_keyval>;
    cb.on_type_get_attr =
      report_generic<dumpi_type_get_attr, DUMPI_Type_get_attr>;
    cb.on_type_get_contents =
      report_generic<dumpi_type_get_contents, DUMPI_Type_get_contents>;
    cb.on_type_get_envelope =
      report_generic<dumpi_type_get_envelope, DUMPI_Type_get_envelope>;
    cb.on_type_get_name =
      report_generic<dumpi_type_get_name, DUMPI_Type_get_name>;
    cb.on_type_set_attr =
      report_generic<dumpi_type_set_attr, DUMPI_Type_set_attr>;
    cb.on_type_set_name =
      report_generic<dumpi_type_set_name, DUMPI_Type_set_name>;
    cb.on_type_match_size =
      report_generic<dumpi_type_match_size, DUMPI_Type_match_size>;
    cb.on_win_call_errhandler =
      report_generic<dumpi_win_call_errhandler, DUMPI_Win_call_errhandler>;
    cb.on_win_create_keyval =
      report_generic<dumpi_win_create_keyval, DUMPI_Win_create_keyval>;
    cb.on_win_delete_attr =
      report_generic<dumpi_win_delete_attr, DUMPI_Win_delete_attr>;
    cb.on_win_free_keyval =
      report_generic<dumpi_win_free_keyval, DUMPI_Win_free_keyval>;
    cb.on_win_get_attr = report_generic<dumpi_win_get_attr, DUMPI_Win_get_attr>;
    cb.on_win_get_name = report_generic<dumpi_win_get_name, DUMPI_Win_get_name>;
    cb.on_win_set_attr = report_generic<dumpi_win_set_attr, DUMPI_Win_set_attr>;
    cb.on_win_set_name = report_generic<dumpi_win_set_name, DUMPI_Win_set_name>;
    cb.on_alloc_mem = report_generic<dumpi_alloc_mem, DUMPI_Alloc_mem>;
    cb.on_comm_create_errhandler =
      report_generic<dumpi_comm_create_errhandler, DUMPI_Comm_create_errhandler>;
    cb.on_comm_get_errhandler =
      report_generic<dumpi_comm_get_errhandler, DUMPI_Comm_get_errhandler>;
    cb.on_comm_set_errhandler =
      report_generic<dumpi_comm_set_errhandler, DUMPI_Comm_set_errhandler>;
    cb.on_file_create_errhandler =
      report_generic<dumpi_file_create_errhandler, DUMPI_File_create_errhandler>;
    cb.on_file_get_errhandler =
      report_generic<dumpi_file_get_errhandler, DUMPI_File_get_errhandler>;
    cb.on_file_set_errhandler =
      report_generic<dumpi_file_set_errhandler, DUMPI_File_set_errhandler>;
    cb.on_finalized = report_generic<dumpi_finalized, DUMPI_Finalized>;
    cb.on_free_mem = report_generic<dumpi_free_mem, DUMPI_Free_mem>;
    cb.on_get_address = report_generic<dumpi_get_address, DUMPI_Get_address>;
    cb.on_info_create = report_generic<dumpi_info_create, DUMPI_Info_create>;
    cb.on_info_delete = report_generic<dumpi_info_delete, DUMPI_Info_delete>;
    cb.on_info_dup = report_generic<dumpi_info_dup, DUMPI_Info_dup>;
    cb.on_info_free = report_generic<dumpi_info_free, DUMPI_Info_free>;
    cb.on_info_get = report_generic<dumpi_info_get, DUMPI_Info_get>;
    cb.on_info_get_nkeys =
      report_generic<dumpi_info_get_nkeys, DUMPI_Info_get_nkeys>;
    cb.on_info_get_nthkey =
      report_generic<dumpi_info_get_nthkey, DUMPI_Info_get_nthkey>;
    cb.on_info_get_valuelen =
      report_generic<dumpi_info_get_valuelen, DUMPI_Info_get_valuelen>;
    cb.on_info_set = report_generic<dumpi_info_set, DUMPI_Info_set>;
    cb.on_pack_external =
      report_generic<dumpi_pack_external, DUMPI_Pack_external>;
    cb.on_pack_external_size =
      report_generic<dumpi_pack_external_size, DUMPI_Pack_external_size>;
    cb.on_request_get_status =
      report_generic<dumpi_request_get_status, DUMPI_Request_get_status>;
    cb.on_type_create_darray =
      report_generic<dumpi_type_create_darray, DUMPI_Type_create_darray>;
    cb.on_type_create_hindexed =
      report_generic<dumpi_type_create_hindexed, DUMPI_Type_create_hindexed>;
    cb.on_type_create_hvector =
      report_generic<dumpi_type_create_hvector, DUMPI_Type_create_hvector>;
    cb.on_type_create_indexed_block =
      report_generic<dumpi_type_create_indexed_block,
      DUMPI_Type_create_indexed_block>;
    cb.on_type_create_resized =
      report_generic<dumpi_type_create_resized, DUMPI_Type_create_resized>;
    cb.on_type_create_struct =
      report_generic<dumpi_type_create_struct, DUMPI_Type_create_struct>;
    cb.on_type_create_subarray =
      report_generic<dumpi_type_create_subarray, DUMPI_Type_create_subarray>;
    cb.on_type_get_extent =
      report_generic<dumpi_type_get_extent, DUMPI_Type_get_extent>;
    cb.on_type_get_true_extent =
      report_generic<dumpi_type_get_true_extent, DUMPI_Type_get_true_extent>;
    cb.on_unpack_external =
      report_generic<dumpi_unpack_external, DUMPI_Unpack_external>;
    cb.on_win_create_errhandler =
      report_generic<dumpi_win_create_errhandler, DUMPI_Win_create_errhandler>;
    cb.on_win_get_errhandler =
      report_generic<dumpi_win_get_errhandler, DUMPI_Win_get_errhandler>;
    cb.on_win_set_errhandler =
      report_generic<dumpi_win_set_errhandler, DUMPI_Win_set_errhandler>;
    cb.on_file_open = report_generic<dumpi_file_open, DUMPI_File_open>;
    cb.on_file_close = report_generic<dumpi_file_close, DUMPI_File_close>;
    cb.on_file_delete = report_generic<dumpi_file_delete, DUMPI_File_delete>;
    cb.on_file_set_size =
      report_generic<dumpi_file_set_size, DUMPI_File_set_size>;
    cb.on_file_preallocate =
      report_generic<dumpi_file_preallocate, DUMPI_File_preallocate>;
    cb.on_file_get_size =
      report_generic<dumpi_file_get_size, DUMPI_File_get_size>;
    cb.on_file_get_group =
      report_generic<dumpi_file_get_group, DUMPI_File_get_group>;
    cb.on_file_get_amode =
      report_generic<dumpi_file_get_amode, DUMPI_File_get_amode>;
    cb.on_file_set_info =
      report_generic<dumpi_file_set_info, DUMPI_File_set_info>;
    cb.on_file_get_info =
      report_generic<dumpi_file_get_info, DUMPI_File_get_info>;
    cb.on_file_set_view =
      report_generic<dumpi_file_set_view, DUMPI_File_set_view>;
    cb.on_file_get_view =
      report_generic<dumpi_file_get_view, DUMPI_File_get_view>;
    cb.on_file_read_at = report_generic<dumpi_file_read_at, DUMPI_File_read_at>;
    cb.on_file_read_at_all =
      report_generic<dumpi_file_read_at_all, DUMPI_File_read_at_all>;
    cb.on_file_write_at =
      report_generic<dumpi_file_write_at, DUMPI_File_write_at>;
    cb.on_file_write_at_all =
      report_generic<dumpi_file_write_at_all, DUMPI_File_write_at_all>;
    cb.on_file_iread_at =
      report_generic<dumpi_file_iread_at, DUMPI_File_iread_at>;
    cb.on_file_iwrite_at =
      report_generic<dumpi_file_iwrite_at, DUMPI_File_iwrite_at>;
    cb.on_file_read =
      report_generic<dumpi_file_read, DUMPI_File_read>;
    cb.on_file_read_all =
      report_generic<dumpi_file_read_all, DUMPI_File_read_all>;
    cb.on_file_write = report_generic<dumpi_file_write, DUMPI_File_write>;
    cb.on_file_write_all =
      report_generic<dumpi_file_write_all, DUMPI_File_write_all>;
    cb.on_file_iread = report_generic<dumpi_file_iread, DUMPI_File_iread>;
    cb.on_file_iwrite = report_generic<dumpi_file_iwrite, DUMPI_File_iwrite>;
    cb.on_file_seek = report_generic<dumpi_file_seek, DUMPI_File_seek>;
    cb.on_file_get_position =
      report_generic<dumpi_file_get_position, DUMPI_File_get_position>;
    cb.on_file_get_byte_offset =
      report_generic<dumpi_file_get_byte_offset, DUMPI_File_get_byte_offset>;
    cb.on_file_read_shared =
      report_generic<dumpi_file_read_shared, DUMPI_File_read_shared>;
    cb.on_file_write_shared =
      report_generic<dumpi_file_write_shared, DUMPI_File_write_shared>;
    cb.on_file_iread_shared =
      report_generic<dumpi_file_iread_shared, DUMPI_File_iread_shared>;
    cb.on_file_iwrite_shared =
      report_generic<dumpi_file_iwrite_shared, DUMPI_File_iwrite_shared>;
    cb.on_file_read_ordered =
      report_generic<dumpi_file_read_ordered, DUMPI_File_read_ordered>;
    cb.on_file_write_ordered =
      report_generic<dumpi_file_write_ordered, DUMPI_File_write_ordered>;
    cb.on_file_seek_shared =
      report_generic<dumpi_file_seek_shared, DUMPI_File_seek_shared>;
    cb.on_file_get_position_shared =
      report_generic<dumpi_file_get_position_shared,
      DUMPI_File_get_position_shared>;
    cb.on_file_read_at_all_begin =
      report_generic<dumpi_file_read_at_all_begin, DUMPI_File_read_at_all_begin>;
    cb.on_file_read_at_all_end =
      report_generic<dumpi_file_read_at_all_end, DUMPI_File_read_at_all_end>;
    cb.on_file_write_at_all_begin =
      report_generic<dumpi_file_write_at_all_begin,
      DUMPI_File_write_at_all_begin>;
    cb.on_file_write_at_all_end =
      report_generic<dumpi_file_write_at_all_end, DUMPI_File_write_at_all_end>;
    cb.on_file_read_all_begin =
      report_generic<dumpi_file_read_all_begin, DUMPI_File_read_all_begin>;
    cb.on_file_read_all_end =
      report_generic<dumpi_file_read_all_end, DUMPI_File_read_all_end>;
    cb.on_file_write_all_begin =
      report_generic<dumpi_file_write_all_begin, DUMPI_File_write_all_begin>;
    cb.on_file_write_all_end =
      report_generic<dumpi_file_write_all_end, DUMPI_File_write_all_end>;
    cb.on_file_read_ordered_begin =
      report_generic<dumpi_file_read_ordered_begin,
      DUMPI_File_read_ordered_begin>;
    cb.on_file_read_ordered_end =
      report_generic<dumpi_file_read_ordered_end, DUMPI_File_read_ordered_end>;
    cb.on_file_write_ordered_begin =
      report_generic<dumpi_file_write_ordered_begin,
      DUMPI_File_write_ordered_begin>;
    cb.on_file_write_ordered_end =
      report_generic<dumpi_file_write_ordered_end, DUMPI_File_write_ordered_end>;
    cb.on_file_get_type_extent =
      report_generic<dumpi_file_get_type_extent, DUMPI_File_get_type_extent>;
    cb.on_register_datarep =
      report_generic<dumpi_register_datarep, DUMPI_Register_datarep>;
    cb.on_file_set_atomicity =
      report_generic<dumpi_file_set_atomicity, DUMPI_File_set_atomicity>;
    cb.on_file_get_atomicity =
      report_generic<dumpi_file_get_atomicity, DUMPI_File_get_atomicity>;
    cb.on_file_sync = report_generic<dumpi_file_sync, DUMPI_File_sync>;
    cb.on_iotest = report_generic<dumpio_test, DUMPIO_Test>;
    cb.on_iowait = report_generic<dumpio_wait, DUMPIO_Wait>;
    cb.on_iotestall = report_generic<dumpio_testall, DUMPIO_Testall>;
    cb.on_iowaitall = report_generic<dumpio_waitall, DUMPIO_Waitall>;
    cb.on_iotestany = report_generic<dumpio_testany, DUMPIO_Testany>;
    cb.on_iowaitany = report_generic<dumpio_waitany, DUMPIO_Waitany>;
    cb.on_iowaitsome = report_generic<dumpio_waitsome, DUMPIO_Waitsome>;
    cb.on_iotestsome = report_generic<dumpio_testsome, DUMPIO_Testsome>;
    cb.on_function_enter = report_function_enter;
    cb. on_function_exit =
      report_generic<dumpi_func_call, DUMPI_Function_exit>;  
  }

  //
  // Run through all the traces in the given metafile
  //
  void callbacks::go(const metadata &meta, std::vector<trace> &trace,
                     std::vector<binbase*> &bin)
  {
    trace_ = &trace;
    bin_ = &bin;
    for(current_trace_ = 0; current_trace_ < meta.traces(); ++current_trace_) {
      std::string tname = meta.tracename(current_trace_);
      dumpi_profile *prof = undumpi_open(tname.c_str());
      // Get function addresses.
      labels_.clear();
      int count;
      uint64_t *labels = NULL;
      char **names = NULL;
      dumpi_read_function_addresses(prof, &count, &labels, &names);
      for(int i = 0; i < count; ++i) {
        labels_[labels[i]] = names[i];
        free(names[i]);
      }
      free(labels);
      free(names);
      // Rest of the stuff.
      for(size_t hand = 0; hand < bin.size(); ++hand)
        bin[hand]->start_trace(current_trace_);
      undumpi_read_stream(prof, &cb, this);
      undumpi_close(prof);
    }
    for(size_t hand = 0; hand < bin.size(); ++hand)
      bin[hand]->reset_trace();
    bin_ = NULL;
    trace_ = NULL;
  }

  //
  // Forward a call to all handlers.
  //
  void callbacks::handle(dumpi_function func, uint16_t thread,
                         const dumpi_time *cpu, const dumpi_time *wall,
                         const dumpi_perfinfo *perf,
                         int64_t bytes_sent, int to_global_rank,
                         int64_t bytes_recvd, int from_global_rank,
                         const void *dumpi_arg)
  {
    for(size_t hand = 0;hand < bin_->size(); ++hand)
      bin_->at(hand)->handle(func, thread, cpu, wall, perf,
                             bytes_sent, to_global_rank,
                             bytes_recvd, from_global_rank,
                             dumpi_arg);
  }

  //
  // Given an annotation, pass it on to the handlers.
  //
  void callbacks::annotate(const dumpi_func_call *prm, uint16_t thread,
                           const dumpi_time *cpu, const dumpi_time *wall,
                           const dumpi_perfinfo *perf)
  {
    //std::cerr << "labels_.size() == " << labels_.size() << "\n";
    if(labels_.find(prm->fn) != labels_.end()) {
      //std::cerr << " -- labels_[" << prm->fn << "] == "
      //          << labels_[prm->fn] << "\n";
      const std::string &anno = labels_[prm->fn];
      if(anno.substr(0, 10) == "ANNOTATION") {
        for(size_t hand = 0; hand < bin_->size(); ++hand)
          bin_->at(hand)->annotate(anno.c_str(), thread, cpu, wall, perf);
      }
    }
    //else {
    //  std::cerr << " -- labels_[" << prm->fn << "] not found\n";
    //}
  }
 
  //
  // Get the size of the given type.
  //
  int callbacks::typesize(dumpi_datatype dtype, const dumpi_clock &when) const
  {
    return trace_->at(current_trace_).get_type(dtype, when).get_size();
  }

  //
  // Get the size of the given communicator.
  //
  int callbacks::commsize(dumpi_comm thecomm, const dumpi_clock &when) const {
    return trace_->at(current_trace_).get_comm(thecomm, when)
      .get_group().get_size();
  }

  /// Get the rank of the current node in the given communicator.
  int callbacks::commrank(dumpi_comm thecomm, const dumpi_clock &when) const  {
    return trace_->at(current_trace_).get_comm(thecomm, when)
      .get_group().get_local_rank();
  }

  /// Get the global rank of the given node.
  int callbacks::globalrank(int localrank, dumpi_comm comm,
                            const dumpi_clock &when) const
  {
    if(localrank >= 0)
      return trace_->at(current_trace_).get_comm(comm, when)
	.get_group().get_global_peer_rank(localrank);
    else
      return localrank;
  }

} // end of namespace dumpi