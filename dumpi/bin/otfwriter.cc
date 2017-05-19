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

#include <dumpi/bin/otfwriter.h>
#include <dumpi/common/funclabels.h>
#include <iostream>
#include <sstream>
#include <assert.h>
#include <math.h>

namespace dumpi {

  //
  // OTF uses unsigned integer values to define a variety of groups, counters, etc.
  //
  static const uint32_t userfuncs = 2000;
  static const uint32_t papictrs  = 5000;
  static const uint32_t clockctrs = 6000;
  static const uint32_t cputimer  = 6001;

  //
  // Hello.
  //
  otfwriter::otfwriter(const std::string &fileroot) :
    mgr_(NULL), writer_(NULL), meta_(NULL), info_(NULL)
  {
    memset(&callbacks_, 0, sizeof(libundumpi_callbacks));
    // Unfortunately, the OTF writer expects a char* (not const char*).
    char *scratch = strdup(fileroot.c_str());
    assert(scratch != NULL);
    if((mgr_ = OTF_FileManager_open(16)) == NULL) {
      throw "otfwriter:  Failed to create file manager.";
    }
    if((writer_ = OTF_Writer_open(scratch, 0, mgr_)) == NULL) {
      std::cerr << "Problems opening OTF output file root \"" << fileroot <<"\"\n";
      throw "otfwriter:  Failed to create writer.";
    }
    OTF_Writer_setFormat(writer_, OTF_WSTREAM_FORMAT_SHORT);
    if(OTF_Writer_setCompression(writer_, OTF_FILECOMPRESSION_COMPRESSED) == 0) {
      ;//std::cerr << "otfwriter:  Writing uncompressed trace file (no zlib?)\n";
    }
    free(scratch);
  }
  
  //
  // Goodbye.
  //
  otfwriter::~otfwriter() throw() {
    if(writer_) OTF_Writer_close(writer_);
    if(mgr_) OTF_FileManager_close(mgr_);
  }

  //
  // Make magic.
  //
  void otfwriter::write(const metadata &meta, const std::vector<trace> &info) {
    meta_ = &meta;
    info_ = &info;
    set_callbacks();
    define_mpi_functions();
    // Define a special counter for CPU time.
    OTF_Writer_writeDefCounterGroup(writer_, 0, clockctrs,
				    "Timers");
    OTF_Writer_writeDefCounter(writer_, 0, cputimer, "CPU time",
			       OTF_COUNTER_TYPE_ACC + OTF_COUNTER_SCOPE_START,
			       clockctrs, 0);
    // Ready to start
    for(size_t i = 0; i < info.size(); ++i) {
      OTF_Writer_writeDefTimerResolution(writer_, i+1, uint64_t(1e9));
      std::string the_process("MPI process ");
      {
        std::stringstream ss;
        ss << i;
        the_process += ss.str();
      }
      OTF_Writer_writeDefProcess(writer_, i+1, i+1, the_process.c_str(), 0);
    }
    // Now parse the files one after the other.
    for(current_rank_ = 0; current_rank_ < int(info.size()); ++current_rank_) {
      std::string tracename = meta.tracename(current_rank_);
      dumpi_profile *profile = undumpi_open(tracename.c_str());
      if(! profile) {
	std::cerr << "otfwriter::write:  Failed to open " << tracename << ": "
		  << strerror(errno) << "\n";
      }
      define_local_functions(current_rank_+1, profile);
      define_perfctr_labels(current_rank_+1, profile);
      undumpi_read_stream(profile, &callbacks_, this);
      undumpi_close(profile);
    }
  }

  /*
    OTF_FileManager *otfmgr = OTF_FileManager_open(128);
    OTF_Writer *otfwriter = OTF_Writer_open("foo-otf", traces.size(), otfmgr);
    OTF_Writer_setFormat(otfwriter, OTF_WSTREAM_FORMAT_LONG);
    OTF_Writer_setCompression(otfwriter, OTF_FILECOMPRESSION_UNCOMPRESSED);
    // OTF_Writer_assignProcess(otfwriter, uint32_t process > 0, uint32_t stream);
    // OTF_Writer_write<RecordType>(otfwriter, ...); -- do stuff
    */

  //
  // Utility.
  //
  inline uint64_t to_nsec(const dumpi_clock &at) {
    return uint64_t((uint64_t(1e9)*at.sec) + at.nsec);
  }

  //
  // Common functionality for functions that are 'tagged' (entry and exit)
  // but don't do any communication.
  //
  template <typename T, uint32_t functag>
  int otfwriter::handle_generic(const T *prm,  uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = functag + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    if(self->info_->at(r).rank() != r) 
      throw "probable coding mistake:  Mismatched ranks.";
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Common functionality for send, bsend, ssend, and rsend.
  //
  template <typename T, uint32_t functag>
  int otfwriter::handle_send(const T *prm,  uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    static const int func = functag + 1;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    const comm &the_comm =self->info_->at(r).get_comm(prm->comm, wall->start);
    const type &the_type =self->info_->at(r).get_type(prm->datatype,wall->start);
    uint32_t sender = the_comm.get_group().get_global_rank() + 1;
    uint32_t recver = the_comm.get_group().get_global_peer_rank(prm->dest) + 1;
    uint32_t procgroup = 0;
    uint32_t tag = prm->tag;
    uint32_t length = the_type.get_size() * prm->count;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, sender, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeSendMsg(self->writer_, start, sender, recver, procgroup,
			    tag, length, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, sender, source);
    return 1;
  }

  //
  // Handle recv operations.
  //
  int otfwriter::handle_recv(const dumpi_recv *prm, uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Recv + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    if(self->info_->at(r).rank() != r) 
      throw "probable coding mistake:  Mismatched ranks.";
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    const type &the_type = self->info_->at(r).get_type(prm->datatype,wall->start);
    uint32_t recver = the_comm.get_group().get_global_rank() + 1;
    if(recver != uint32_t(r+1)) {
      std::cerr << "Problem in communicator " << prm->comm << "\n";
      std::cerr << "otf recver: " <<recver << ", current otf rank: " << r << "\n";
      throw "otfwriter::do_send:  Receiver is not the same as current rank\n";
    }
    uint32_t sender = the_comm.get_group().get_global_peer_rank(prm->source) + 1;
    uint32_t procgroup = 0;
    uint32_t tag = prm->tag;
    uint32_t length = the_type.get_size() * prm->count;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, recver, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeRecvMsg(self->writer_, stop, recver, sender, procgroup,
			    tag, length, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, recver, source);
    return 1;
  }

  //
  // Handle irecv operations.
  //
  int otfwriter::handle_irecv(const dumpi_irecv *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Irecv + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    if(self->info_->at(r).rank() != r) 
      throw "probable coding mistake:  Mismatched ranks.";
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    const type &the_type = self->info_->at(r).get_type(prm->datatype,wall->start);
    uint32_t recver = the_comm.get_group().get_global_rank() + 1;
    if(recver != uint32_t(r+1)) {
      std::cerr << "Problem in communicator " << prm->comm << "\n";
      std::cerr << "otf recver: " <<recver << ", current otf rank: " << r << "\n";
      throw "otfwriter::do_send:  Receiver is not the same as current rank\n";
    }
    uint32_t sender = the_comm.get_group().get_global_peer_rank(prm->source) + 1;
    uint32_t procgroup = 0;
    uint32_t tag = prm->tag;
    uint32_t length = the_type.get_size() * prm->count;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, recver, source);
    self->write_counters_enter(start, cpu, perf);
    self->complete_[recver].add(prm->request, recver, sender,
				procgroup, tag, length, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, recver, source);
    return 1;
  }

  //
  // Hande MPI_Wait
  //
  int otfwriter::handle_wait(const dumpi_wait *prm, uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg)
  {
    static const uint32_t func = DUMPI_Wait + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    self->complete_[otfrank].complete(self->writer_, stop, prm->request);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle MPI_Waitany.
  //
  int otfwriter::handle_waitany(const dumpi_waitany *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    static const uint32_t func = DUMPI_Waitany + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->complete_[otfrank].complete(self->writer_, stop,
				      prm->requests[prm->index]);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle MPI_Waitsome
  //
  int otfwriter::handle_waitsome(const dumpi_waitsome *prm, uint16_t thread,
				 const dumpi_time *cpu, const dumpi_time *wall,
				 const dumpi_perfinfo *perf, void *userarg) 
  {
    static const uint32_t func = DUMPI_Waitsome + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    for(int i = 0; i < prm->outcount; ++i)
      self->complete_[otfrank].complete(self->writer_, stop,
					prm->requests[prm->indices[i]]);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle MPI_Waitall.
  //
  int otfwriter::handle_waitall(const dumpi_waitall *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    static const uint32_t func = DUMPI_Waitall + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    self->complete_[otfrank].complete_all(self->writer_, stop,
					  prm->count, prm->requests);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle MPI_Test.
  //
  int otfwriter::handle_test(const dumpi_test *prm, uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg)
  {
    static const uint32_t func = DUMPI_Test + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    if(prm->flag)
      self->complete_[otfrank].complete(self->writer_, stop, prm->request);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle MPI_Testany.
  //
  int otfwriter::handle_testany(const dumpi_testany *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    static const uint32_t func = DUMPI_Testany + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    if(prm->flag)
      self->complete_[otfrank].complete(self->writer_, stop,
					prm->requests[prm->index]);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle MPI_Testsome.
  //
  int otfwriter::handle_testsome(const dumpi_testsome *prm, uint16_t thread,
				 const dumpi_time *cpu, const dumpi_time *wall,
				 const dumpi_perfinfo *perf, void *userarg)
  {
    static const uint32_t func = DUMPI_Testsome + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    for(int i = 0; i < prm->outcount; ++i)
      self->complete_[otfrank].complete(self->writer_, stop,
					prm->requests[prm->indices[i]]);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle MPI_Testall.
  //
  int otfwriter::handle_testall(const dumpi_testall *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    static const uint32_t func = DUMPI_Testall + 1;
    (void)prm; (void)thread; (void)cpu; (void)wall; (void)perf; (void)userarg;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    if(prm->flag)
      self->complete_[otfrank].complete_all(self->writer_, stop,
					    prm->count, prm->requests);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle all barrier-like calls.
  //
  template <typename T, uint32_t functag>
  int otfwriter::handle_barrier(const T *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)perf;
    static const int func = functag + 1;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    int otfrank = r+1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0, root = 0, sent = 0, recvd = 0, source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle a broadcast.
  //
  int otfwriter::handle_bcast(const dumpi_bcast *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Bcast + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const comm &the_comm =self->info_->at(r).get_comm(prm->comm, wall->start);
    const type &the_type =self->info_->at(r).get_type(prm->datatype,wall->start);
    uint32_t root = the_comm.get_group().get_global_peer_rank(prm->root) + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = 0, recvd = 0;
    uint32_t bytes = the_type.get_size() * prm->count;
    if(otfrank == root)
      sent = bytes;
    else
      recvd = bytes;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle a gather.
  //
  int otfwriter::handle_gather(const dumpi_gather *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Gather + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    type stype, rtype;
    if(prm->commrank == prm->root)
      rtype = self->info_->at(r).get_type(prm->recvtype, wall->start);
    else
      stype = self->info_->at(r).get_type(prm->sendtype,wall->start);
    uint32_t root = the_comm.get_group().get_global_peer_rank(prm->root) + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = 0, recvd = 0;
    if(prm->commrank == prm->root) {
      recvd = (rtype.get_size() * (the_comm.get_group().get_size() -1) *
	       prm->recvcount);
    }
    else {
      sent = stype.get_size() * prm->sendcount;
    }
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle a gatherv.
  //
  int otfwriter::handle_gatherv(const dumpi_gatherv *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Gatherv + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    type stype, rtype;
    if(prm->commrank == prm->root)
      rtype = self->info_->at(r).get_type(prm->recvtype, wall->start);
    else
      stype = self->info_->at(r).get_type(prm->sendtype,wall->start);
    uint32_t root = the_comm.get_group().get_global_peer_rank(prm->root) + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = 0, recvd = 0;
    if(prm->commrank == prm->root) {
      recvd = 0;
      for(int i = 0; i < prm->commsize; ++i) {
	if(i != prm->root)
	  recvd += prm->recvcounts[i];
      }
      recvd *= rtype.get_size();
    }
    else {
      sent = stype.get_size() * prm->sendcount;
    }
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle a scatter.
  //
  int otfwriter::handle_scatter(const dumpi_scatter *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Scatter + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    type stype, rtype;
    if(prm->commrank == prm->root)
      stype = self->info_->at(r).get_type(prm->recvtype, wall->start);
    else
      rtype = self->info_->at(r).get_type(prm->sendtype,wall->start);
    uint32_t root = the_comm.get_group().get_global_peer_rank(prm->root) + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = 0, recvd = 0;
    if(prm->commrank == prm->root) {
      sent = (rtype.get_size() * (the_comm.get_group().get_size() -1) *
	      prm->recvcount);
    }
    else {
      recvd = stype.get_size() * prm->sendcount;
    }
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle a scatterv.
  //
  int otfwriter::handle_scatterv(const dumpi_scatterv *prm, uint16_t thread,
				 const dumpi_time *cpu, const dumpi_time *wall,
				 const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Scatterv + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    type stype, rtype;
    if(prm->commrank == prm->root)
      stype = self->info_->at(r).get_type(prm->recvtype, wall->start);
    else
      rtype = self->info_->at(r).get_type(prm->sendtype,wall->start);
    uint32_t root = the_comm.get_group().get_global_peer_rank(prm->root) + 1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = 0, recvd = 0;
    if(prm->commrank == prm->root) {
      sent = 0;
      for(int i = 0; i < prm->commsize; ++i) {
	if(i != prm->root)
	  sent += prm->sendcounts[i];
      }
      sent *= stype.get_size();
    }
    else {
      recvd = rtype.get_size() * prm->recvcount;
    }
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle allgather.
  //
  int otfwriter::handle_allgather(const dumpi_allgather *prm, uint16_t thread,
				  const dumpi_time *cpu, const dumpi_time *wall,
				  const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Allgather + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    int commsize = the_comm.get_group().get_size();
    const type &stype = self->info_->at(r).get_type(prm->sendtype, wall->start);
    const type &rtype = self->info_->at(r).get_type(prm->recvtype, wall->start);
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = stype.get_size() * prm->sendcount * (commsize-1);
    uint32_t recvd = rtype.get_size() * prm->recvcount * (commsize-1);
    uint32_t root = 0;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle allgatherv.  This is actually a bit prettier than gather.
  //
  int otfwriter::handle_allgatherv(const dumpi_allgatherv *prm, uint16_t thread,
				   const dumpi_time *cpu, const dumpi_time *wall,
				   const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Allgatherv + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const type &stype = self->info_->at(r).get_type(prm->sendtype, wall->start);
    const type &rtype = self->info_->at(r).get_type(prm->recvtype, wall->start);
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = stype.get_size() * prm->sendcount * (prm->commsize - 1);
    uint32_t recvd = 0;
    for(int i = 0; i < prm->commsize; ++i) {
      if(i != self->current_rank_)
	recvd += prm->recvcounts[i];
    }
    recvd *= rtype.get_size();
    uint32_t root = 0;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle alltoall.
  //
  int otfwriter::handle_alltoall(const dumpi_alltoall *prm, uint16_t thread,
				 const dumpi_time *cpu, const dumpi_time *wall,
				 const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Alltoall + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    int commsize = the_comm.get_group().get_size();
    const type &stype = self->info_->at(r).get_type(prm->sendtype, wall->start);
    const type &rtype = self->info_->at(r).get_type(prm->recvtype, wall->start);
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = stype.get_size() * prm->sendcount * (commsize-1);
    uint32_t recvd = rtype.get_size() * prm->recvcount * (commsize-1);
    uint32_t root = 0;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  /// Handle alltoallv.  "Conveniently," this function is designed such that
  /// it is generally impractical to use bisectional algorithms (so the
  /// direct algorithm is normally used instead).
  int otfwriter::handle_alltoallv(const dumpi_alltoallv *prm, uint16_t thread,
				  const dumpi_time *cpu, const dumpi_time *wall,
				  const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Alltoallv + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const type &stype = self->info_->at(r).get_type(prm->sendtype, wall->start);
    const type &rtype = self->info_->at(r).get_type(prm->recvtype, wall->start);
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = 0, recvd = 0;
    for(int i = 0; i < prm->commsize; ++i) {
      if(i != self->current_rank_) {
	sent  += prm->sendcounts[i];
	recvd += prm->recvcounts[i];
      }
    }
    sent  *= stype.get_size();
    recvd *= rtype.get_size();
    uint32_t root = 0;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle alltoallw.  Comments regarding MPI design wisely not expressed here.
  //
  int otfwriter::handle_alltoallw(const dumpi_alltoallw *prm, uint16_t thread,
				  const dumpi_time *cpu, const dumpi_time *wall,
				  const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Alltoallw + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = 0, recvd = 0;
    for(int i = 0; i < prm->commsize; ++i) {
      if(i != self->current_rank_) {
	const type &stype
	  = self->info_->at(r).get_type(prm->sendtypes[i], wall->start);
	const type &rtype
	  = self->info_->at(r).get_type(prm->recvtypes[i], wall->start);
	sent  += prm->sendcounts[i] * stype.get_size();
	recvd += prm->recvcounts[i] * rtype.get_size();
      }
    }
    uint32_t root = 0;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle reduce.
  //
  template <typename T, uint32_t typetag>
  int otfwriter::handle_reduce(const T *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg)
  {  
    static const int func = typetag + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    const type &the_type = self->info_->at(r).get_type(prm->datatype, wall->start);
    uint32_t otfrank = r+1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t message_estimate = (prm->count * the_type.get_size() *
				 log2(the_comm.get_group().get_size()));
    uint32_t sent = message_estimate, recvd = message_estimate;
    uint32_t root = 0;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle reduce_scatter.
  //
  int otfwriter::handle_reduce_scatter(const dumpi_reduce_scatter *prm, 
				       uint16_t thread,
				       const dumpi_time *cpu,
				       const dumpi_time *wall,
				       const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Reduce_scatter + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    const type &the_type = self->info_->at(r).get_type(prm->datatype, wall->start);
    uint32_t otfrank = r+1;
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t message_estimate = 0;
    for(int i = 0; i < prm->commsize; ++i)
      message_estimate += prm->recvcounts[i];
    message_estimate *=  (the_type.get_size() *
			  log2(the_comm.get_group().get_size()));
    uint32_t sent = message_estimate, recvd = message_estimate;
    uint32_t root = 0;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Handle scan and exscan.
  //
  template <typename T, uint32_t typetag>
  int otfwriter::handle_scan(const T *prm, uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg)
  {
    static const int func = DUMPI_Alltoall + 1;
    (void)thread; (void)cpu; (void)perf;
    otfwriter *self = (otfwriter*)userarg;
    int r = self->current_rank_;
    uint32_t otfrank = r+1;
    const comm &the_comm = self->info_->at(r).get_comm(prm->comm, wall->start);
    const type &the_type = self->info_->at(r).get_type(prm->datatype, wall->start);
    uint64_t start = to_nsec(wall->start);
    uint64_t stop = to_nsec(wall->stop);
    uint64_t duration = stop - start;
    uint32_t procgroup = 0;
    uint32_t sent = prm->count * the_type.get_size();
    uint32_t recvd = prm->count * the_type.get_size();
    if(the_comm.get_group().get_local_rank() == 0)
      recvd = 0;
    if(the_comm.get_group().get_local_rank() == the_comm.get_group().get_size()-1)
      sent = 0;
    uint32_t root = 0;
    uint32_t source = 0;
    OTF_Writer_writeEnter(self->writer_, start, func, otfrank, source);
    self->write_counters_enter(start, cpu, perf);
    OTF_Writer_writeCollectiveOperation(self->writer_, start, otfrank,
					func, procgroup, root, sent, recvd,
					duration, source);
    self->write_counters_leave(stop, cpu, perf);
    OTF_Writer_writeLeave(self->writer_, stop, func, otfrank, source);
    return 1;
  }

  //
  // Define callbacks for MPI parsing.
  //
  void otfwriter::set_callbacks() {
    callbacks_.on_send = handle_send<dumpi_send, DUMPI_Send>;
    callbacks_.on_bsend = handle_send<dumpi_bsend, DUMPI_Bsend>;
    callbacks_.on_ssend = handle_send<dumpi_ssend, DUMPI_Ssend>;
    callbacks_.on_rsend = handle_send<dumpi_rsend, DUMPI_Rsend>;
    callbacks_.on_isend = handle_send<dumpi_isend, DUMPI_Isend>;
    callbacks_.on_ibsend = handle_send<dumpi_ibsend, DUMPI_Ibsend>;
    callbacks_.on_issend = handle_send<dumpi_issend, DUMPI_Issend>;
    callbacks_.on_irsend = handle_send<dumpi_irsend, DUMPI_Irsend>;
    callbacks_.on_recv = handle_recv;
    callbacks_.on_irecv = handle_irecv;
    callbacks_.on_wait = handle_wait;
    callbacks_.on_test = handle_test;
    callbacks_.on_waitany = handle_waitany;
    callbacks_.on_testany = handle_testany;
    callbacks_.on_waitall = handle_waitall;
    callbacks_.on_testall = handle_testall;
    callbacks_.on_waitsome = handle_waitsome;
    callbacks_.on_testsome = handle_testsome;
    callbacks_.on_init = handle_barrier<dumpi_init, DUMPI_Init>;
    callbacks_.on_init_thread =
      handle_barrier<dumpi_init_thread, DUMPI_Init_thread>;
    callbacks_.on_barrier = handle_barrier<dumpi_barrier, DUMPI_Barrier>;
    callbacks_.on_finalize = handle_barrier<dumpi_finalize, DUMPI_Finalize>;
    callbacks_.on_comm_spawn =
      handle_barrier<dumpi_comm_spawn, DUMPI_Comm_spawn>;
    callbacks_.on_comm_spawn_multiple =
      handle_barrier<dumpi_comm_spawn_multiple, DUMPI_Comm_spawn_multiple>;
    callbacks_.on_file_open = handle_barrier<dumpi_file_open, DUMPI_File_open>;
    callbacks_.on_file_close =
      handle_barrier<dumpi_file_close, DUMPI_File_close>;
    callbacks_.on_file_set_size =
      handle_barrier<dumpi_file_set_size, DUMPI_File_set_size>;
    callbacks_.on_file_preallocate =
      handle_barrier<dumpi_file_preallocate, DUMPI_File_preallocate>;
    callbacks_.on_file_set_info =
      handle_barrier<dumpi_file_set_info, DUMPI_File_set_info>;
    callbacks_.on_file_set_view =
      handle_barrier<dumpi_file_set_view, DUMPI_File_set_view>;
    callbacks_.on_comm_dup = handle_barrier<dumpi_comm_dup, DUMPI_Comm_dup>;
    callbacks_.on_comm_create =
      handle_barrier<dumpi_comm_create, DUMPI_Comm_create>;
    callbacks_.on_comm_split =
      handle_barrier<dumpi_comm_split, DUMPI_Comm_split>;
    callbacks_.on_comm_free = handle_barrier<dumpi_comm_free, DUMPI_Comm_free>;
    callbacks_.on_intercomm_create =
      handle_barrier<dumpi_intercomm_create, DUMPI_Intercomm_create>;
    callbacks_.on_bcast = handle_bcast;
    callbacks_.on_gather = handle_gather;
    callbacks_.on_gatherv = handle_gatherv;
    callbacks_.on_scatter = handle_scatter;
    callbacks_.on_scatterv = handle_scatterv;
    callbacks_.on_allgather = handle_allgather;
    callbacks_.on_allgatherv = handle_allgatherv;
    callbacks_.on_alltoall = handle_alltoall;
    callbacks_.on_alltoallv = handle_alltoallv;
    callbacks_.on_alltoallw = handle_alltoallw;
    callbacks_.on_reduce = handle_reduce<dumpi_reduce, DUMPI_Reduce>;
    callbacks_.on_allreduce = handle_reduce<dumpi_allreduce, DUMPI_Allreduce>;
    callbacks_.on_reduce_scatter = handle_reduce_scatter;
    callbacks_.on_scan = handle_scan<dumpi_scan, DUMPI_Scan>;
    callbacks_.on_exscan = handle_scan<dumpi_exscan, DUMPI_Exscan>;
    //
    // These are still waiting for better handlers (maybe).
    callbacks_.on_send_init =
      handle_generic<dumpi_send_init, DUMPI_Send_init>;
    callbacks_.on_bsend_init =
      handle_generic<dumpi_bsend_init, DUMPI_Bsend_init>;
    callbacks_.on_ssend_init =
      handle_generic<dumpi_ssend_init, DUMPI_Ssend_init>;
    callbacks_.on_rsend_init =
      handle_generic<dumpi_rsend_init, DUMPI_Rsend_init>;
    callbacks_.on_recv_init = handle_generic<dumpi_recv_init, DUMPI_Recv_init>;
    callbacks_.on_start = handle_generic<dumpi_start, DUMPI_Start>;
    callbacks_.on_startall = handle_generic<dumpi_startall, DUMPI_Startall>;
    callbacks_.on_sendrecv = handle_generic<dumpi_sendrecv, DUMPI_Sendrecv>;
    callbacks_.on_sendrecv_replace =
      handle_generic<dumpi_sendrecv_replace, DUMPI_Sendrecv_replace>;
    callbacks_.on_intercomm_merge =
      handle_generic<dumpi_intercomm_merge, DUMPI_Intercomm_merge>;
    callbacks_.on_comm_accept =
      handle_generic<dumpi_comm_accept, DUMPI_Comm_accept>;
    callbacks_.on_comm_connect =
      handle_generic<dumpi_comm_connect, DUMPI_Comm_connect>;
    callbacks_.on_comm_disconnect =
      handle_generic<dumpi_comm_disconnect, DUMPI_Comm_disconnect>;
    callbacks_.on_comm_get_parent =
      handle_generic<dumpi_comm_get_parent, DUMPI_Comm_get_parent>;
    callbacks_.on_comm_join =
      handle_generic<dumpi_comm_join, DUMPI_Comm_join>;
    callbacks_.on_accumulate =
      handle_generic<dumpi_accumulate, DUMPI_Accumulate>;
    callbacks_.on_get = handle_generic<dumpi_get, DUMPI_Get>;
    callbacks_.on_put = handle_generic<dumpi_put, DUMPI_Put>;
    callbacks_.on_win_complete =
      handle_generic<dumpi_win_complete, DUMPI_Win_complete>;
    callbacks_.on_win_create =
      handle_generic<dumpi_win_create, DUMPI_Win_create>;
    callbacks_.on_win_fence = handle_generic<dumpi_win_fence, DUMPI_Win_fence>;
    callbacks_.on_win_lock = handle_generic<dumpi_win_lock, DUMPI_Win_lock>;
    callbacks_.on_win_post = handle_generic<dumpi_win_post, DUMPI_Win_post>;
    callbacks_.on_win_start = handle_generic<dumpi_win_start, DUMPI_Win_start>;
    callbacks_.on_win_test = handle_generic<dumpi_win_test, DUMPI_Win_test>;
    callbacks_.on_win_unlock =
      handle_generic<dumpi_win_unlock, DUMPI_Win_unlock>;
    callbacks_.on_win_wait = handle_generic<dumpi_win_wait, DUMPI_Win_wait>;
    callbacks_.on_file_delete =
      handle_generic<dumpi_file_delete, DUMPI_File_delete>;
    //
    // A lot of the MPIO file routines are collective.
    // Other routines may involve communication -- need to figure out
    // what (if anything) to do about that.
    callbacks_.on_file_read_at_all =
      handle_generic<dumpi_file_read_at_all, DUMPI_File_read_at_all>;
    callbacks_.on_file_write_at_all =
      handle_generic<dumpi_file_write_at_all, DUMPI_File_write_at_all>;
    callbacks_.on_file_read_all =
      handle_generic<dumpi_file_read_all, DUMPI_File_read_all>;
    callbacks_.on_file_write_all =
      handle_generic<dumpi_file_write_all, DUMPI_File_write_all>;
    callbacks_.on_file_get_size =
      handle_generic<dumpi_file_get_size, DUMPI_File_get_size>;
    callbacks_.on_file_get_group =
      handle_generic<dumpi_file_get_group, DUMPI_File_get_group>;
    callbacks_.on_file_get_amode =
      handle_generic<dumpi_file_get_amode, DUMPI_File_get_amode>;
    callbacks_.on_file_get_info =
      handle_generic<dumpi_file_get_info, DUMPI_File_get_info>;
    callbacks_.on_file_get_view =
      handle_generic<dumpi_file_get_view, DUMPI_File_get_view>;
    callbacks_.on_file_read_at =
      handle_generic<dumpi_file_read_at, DUMPI_File_read_at>;
    callbacks_.on_file_write_at =
      handle_generic<dumpi_file_write_at, DUMPI_File_write_at>;
    callbacks_.on_file_iread_at =
      handle_generic<dumpi_file_iread_at, DUMPI_File_iread_at>;
    callbacks_.on_file_iwrite_at =
      handle_generic<dumpi_file_iwrite_at, DUMPI_File_iwrite_at>;
    callbacks_.on_file_read = handle_generic<dumpi_file_read, DUMPI_File_read>;
    callbacks_.on_file_write =
      handle_generic<dumpi_file_write, DUMPI_File_write>;
    callbacks_.on_file_iread =
      handle_generic<dumpi_file_iread, DUMPI_File_iread>;
    callbacks_.on_file_iwrite =
      handle_generic<dumpi_file_iwrite, DUMPI_File_iwrite>;
    callbacks_.on_file_seek =
      handle_generic<dumpi_file_seek, DUMPI_File_seek>;
    callbacks_.on_file_get_position =
      handle_generic<dumpi_file_get_position, DUMPI_File_get_position>;
    callbacks_.on_file_get_byte_offset =
      handle_generic<dumpi_file_get_byte_offset, DUMPI_File_get_byte_offset>;
    callbacks_.on_file_read_shared =
      handle_generic<dumpi_file_read_shared, DUMPI_File_read_shared>;
    callbacks_.on_file_write_shared =
      handle_generic<dumpi_file_write_shared, DUMPI_File_write_shared>;
    callbacks_.on_file_iread_shared =
      handle_generic<dumpi_file_iread_shared, DUMPI_File_iread_shared>;
    callbacks_.on_file_iwrite_shared =
      handle_generic<dumpi_file_iwrite_shared, DUMPI_File_iwrite_shared>;
    callbacks_.on_file_read_ordered =
      handle_generic<dumpi_file_read_ordered, DUMPI_File_read_ordered>;
    callbacks_.on_file_write_ordered =
      handle_generic<dumpi_file_write_ordered, DUMPI_File_write_ordered>;
    callbacks_.on_file_seek_shared =
      handle_generic<dumpi_file_seek_shared, DUMPI_File_seek_shared>;
    callbacks_.on_file_get_position_shared =
      handle_generic<dumpi_file_get_position_shared,
      DUMPI_File_get_position_shared>;
    callbacks_.on_file_read_at_all_begin =
      handle_generic<dumpi_file_read_at_all_begin, DUMPI_File_read_at_all_begin>;
    callbacks_.on_file_read_at_all_end =
      handle_generic<dumpi_file_read_at_all_end, DUMPI_File_read_at_all_end>;
    callbacks_.on_file_write_at_all_begin =
      handle_generic<dumpi_file_write_at_all_begin,
      DUMPI_File_write_at_all_begin>;
    callbacks_.on_file_write_at_all_end =
      handle_generic<dumpi_file_write_at_all_end, DUMPI_File_write_at_all_end>;
    callbacks_.on_file_read_all_begin =
      handle_generic<dumpi_file_read_all_begin, DUMPI_File_read_all_begin>;
    callbacks_.on_file_read_all_end =
      handle_generic<dumpi_file_read_all_end, DUMPI_File_read_all_end>;
    callbacks_.on_file_write_all_begin =
      handle_generic<dumpi_file_write_all_begin, DUMPI_File_write_all_begin>;
    callbacks_.on_file_write_all_end =
      handle_generic<dumpi_file_write_all_end, DUMPI_File_write_all_end>;
    callbacks_.on_file_read_ordered_begin =
      handle_generic<dumpi_file_read_ordered_begin,
      DUMPI_File_read_ordered_begin>;
    callbacks_.on_file_read_ordered_end =
      handle_generic<dumpi_file_read_ordered_end, DUMPI_File_read_ordered_end>;
    callbacks_.on_file_write_ordered_begin =
      handle_generic<dumpi_file_write_ordered_begin,
      DUMPI_File_write_ordered_begin>;
    callbacks_.on_file_write_ordered_end =
      handle_generic<dumpi_file_write_ordered_end, DUMPI_File_write_ordered_end>;
    callbacks_.on_file_get_type_extent =
      handle_generic<dumpi_file_get_type_extent, DUMPI_File_get_type_extent>;
    callbacks_.on_register_datarep =
      handle_generic<dumpi_register_datarep, DUMPI_Register_datarep>;
    callbacks_.on_file_set_atomicity =
      handle_generic<dumpi_file_set_atomicity, DUMPI_File_set_atomicity>;
    callbacks_.on_file_get_atomicity =
      handle_generic<dumpi_file_get_atomicity, DUMPI_File_get_atomicity>;
    callbacks_.on_file_sync = handle_generic<dumpi_file_sync, DUMPI_File_sync>;
    callbacks_.on_iotest = handle_generic<dumpio_test, DUMPIO_Test>;
    callbacks_.on_iowait = handle_generic<dumpio_wait, DUMPIO_Wait>;
    callbacks_.on_iotestall = handle_generic<dumpio_testall, DUMPIO_Testall>;
    callbacks_.on_iowaitall = handle_generic<dumpio_waitall, DUMPIO_Waitall>;
    callbacks_.on_iotestany = handle_generic<dumpio_testany, DUMPIO_Testany>;
    callbacks_.on_iowaitany = handle_generic<dumpio_waitany, DUMPIO_Waitany>;
    callbacks_.on_iowaitsome =
      handle_generic<dumpio_waitsome, DUMPIO_Waitsome>;
    callbacks_.on_iotestsome =
      handle_generic<dumpio_testsome, DUMPIO_Testsome>;
    //
    // All other functions can probably do just fine with generic handlers:
    callbacks_.on_get_count = handle_generic<dumpi_get_count, DUMPI_Get_count>;
    callbacks_.on_buffer_attach =
      handle_generic<dumpi_buffer_attach, DUMPI_Buffer_attach>;
    callbacks_.on_buffer_detach =
      handle_generic<dumpi_buffer_detach, DUMPI_Buffer_detach>;
    callbacks_.on_request_free =
      handle_generic<dumpi_request_free, DUMPI_Request_free>;
    callbacks_.on_iprobe = handle_generic<dumpi_iprobe, DUMPI_Iprobe>;
    callbacks_.on_probe = handle_generic<dumpi_probe, DUMPI_Probe>;
    callbacks_.on_cancel = handle_generic<dumpi_cancel, DUMPI_Cancel>;
    callbacks_.on_test_cancelled =
      handle_generic<dumpi_test_cancelled, DUMPI_Test_cancelled>;
    callbacks_.on_type_contiguous =
      handle_generic<dumpi_type_contiguous, DUMPI_Type_contiguous>;
    callbacks_.on_type_vector =
      handle_generic<dumpi_type_vector, DUMPI_Type_vector>;
    callbacks_.on_type_hvector =
      handle_generic<dumpi_type_hvector, DUMPI_Type_hvector>;
    callbacks_.on_type_indexed =
      handle_generic<dumpi_type_indexed, DUMPI_Type_indexed>;
    callbacks_.on_type_hindexed =
      handle_generic<dumpi_type_hindexed, DUMPI_Type_hindexed>;
    callbacks_.on_type_struct =
      handle_generic<dumpi_type_struct, DUMPI_Type_struct>;
    callbacks_.on_address = handle_generic<dumpi_address, DUMPI_Address>;
    callbacks_.on_type_extent =
      handle_generic<dumpi_type_extent, DUMPI_Type_extent>;
    callbacks_.on_type_size = handle_generic<dumpi_type_size, DUMPI_Type_size>;
    callbacks_.on_type_lb = handle_generic<dumpi_type_lb, DUMPI_Type_lb>;
    callbacks_.on_type_ub = handle_generic<dumpi_type_ub, DUMPI_Type_ub>;
    callbacks_.on_type_commit =
      handle_generic<dumpi_type_commit, DUMPI_Type_commit>;
    callbacks_.on_type_free =
      handle_generic<dumpi_type_free, DUMPI_Type_free>;
    callbacks_.on_get_elements =
      handle_generic<dumpi_get_elements, DUMPI_Get_elements>;
    callbacks_.on_pack = handle_generic<dumpi_pack, DUMPI_Pack>;
    callbacks_.on_unpack = handle_generic<dumpi_unpack, DUMPI_Unpack>;
    callbacks_.on_pack_size = handle_generic<dumpi_pack_size, DUMPI_Pack_size>;
    callbacks_.on_op_create = handle_generic<dumpi_op_create, DUMPI_Op_create>;
    callbacks_.on_op_free = handle_generic<dumpi_op_free, DUMPI_Op_free>;
    callbacks_.on_group_size =
      handle_generic<dumpi_group_size, DUMPI_Group_size>;
    callbacks_.on_group_rank =
      handle_generic<dumpi_group_rank, DUMPI_Group_rank>;
    callbacks_.on_group_translate_ranks =
      handle_generic<dumpi_group_translate_ranks, DUMPI_Group_translate_ranks>;
    callbacks_.on_group_compare =
      handle_generic<dumpi_group_compare, DUMPI_Group_compare>;
    callbacks_.on_comm_group =
      handle_generic<dumpi_comm_group, DUMPI_Comm_group>;
    callbacks_.on_group_union =
      handle_generic<dumpi_group_union, DUMPI_Group_union>;
    callbacks_.on_group_intersection =
      handle_generic<dumpi_group_intersection, DUMPI_Group_intersection>;
    callbacks_.on_group_difference =
      handle_generic<dumpi_group_difference, DUMPI_Group_difference>;
    callbacks_.on_group_incl =
      handle_generic<dumpi_group_incl, DUMPI_Group_incl>;
    callbacks_.on_group_excl =
      handle_generic<dumpi_group_excl, DUMPI_Group_excl>;
    callbacks_.on_group_range_incl =
      handle_generic<dumpi_group_range_incl, DUMPI_Group_range_incl>;
    callbacks_.on_group_range_excl =
      handle_generic<dumpi_group_range_excl, DUMPI_Group_range_excl>;
    callbacks_.on_group_free =
      handle_generic<dumpi_group_free, DUMPI_Group_free>;
    callbacks_.on_comm_size = handle_generic<dumpi_comm_size, DUMPI_Comm_size>;
    callbacks_.on_comm_rank = handle_generic<dumpi_comm_rank, DUMPI_Comm_rank>;
    callbacks_.on_comm_compare =
      handle_generic<dumpi_comm_compare, DUMPI_Comm_compare>;
    callbacks_.on_comm_test_inter =
      handle_generic<dumpi_comm_test_inter, DUMPI_Comm_test_inter>;
    callbacks_.on_comm_remote_size =
      handle_generic<dumpi_comm_remote_size, DUMPI_Comm_remote_size>;
    callbacks_.on_comm_remote_group =
      handle_generic<dumpi_comm_remote_group, DUMPI_Comm_remote_group>;
    callbacks_.on_keyval_create =
      handle_generic<dumpi_keyval_create, DUMPI_Keyval_create>;
    callbacks_.on_keyval_free =
      handle_generic<dumpi_keyval_free, DUMPI_Keyval_free>;
    callbacks_.on_attr_put =
      handle_generic<dumpi_attr_put, DUMPI_Attr_put>;
    callbacks_.on_attr_get =
      handle_generic<dumpi_attr_get, DUMPI_Attr_get>;
    callbacks_.on_attr_delete =
      handle_generic<dumpi_attr_delete, DUMPI_Attr_delete>;
    callbacks_.on_topo_test =
      handle_generic<dumpi_topo_test, DUMPI_Topo_test>;
    callbacks_.on_cart_create =
      handle_generic<dumpi_cart_create, DUMPI_Cart_create>;
    callbacks_.on_dims_create =
      handle_generic<dumpi_dims_create, DUMPI_Dims_create>;
    callbacks_.on_graph_create =
      handle_generic<dumpi_graph_create, DUMPI_Graph_create>;
    callbacks_.on_graphdims_get =
      handle_generic<dumpi_graphdims_get, DUMPI_Graphdims_get>;
    callbacks_.on_graph_get = handle_generic<dumpi_graph_get, DUMPI_Graph_get>;
    callbacks_.on_cartdim_get =
      handle_generic<dumpi_cartdim_get, DUMPI_Cartdim_get>;
    callbacks_.on_cart_get =
      handle_generic<dumpi_cart_get, DUMPI_Cart_get>;
    callbacks_.on_cart_rank =
      handle_generic<dumpi_cart_rank, DUMPI_Cart_rank>;
    callbacks_.on_cart_coords =
      handle_generic<dumpi_cart_coords, DUMPI_Cart_coords>;
    callbacks_.on_graph_neighbors_count =
      handle_generic<dumpi_graph_neighbors_count, DUMPI_Graph_neighbors_count>;
    callbacks_.on_graph_neighbors =
      handle_generic<dumpi_graph_neighbors, DUMPI_Graph_neighbors>;
    callbacks_.on_cart_shift =
      handle_generic<dumpi_cart_shift, DUMPI_Cart_shift>;
    callbacks_.on_cart_sub = handle_generic<dumpi_cart_sub, DUMPI_Cart_sub>;
    callbacks_.on_cart_map = handle_generic<dumpi_cart_map, DUMPI_Cart_map>;
    callbacks_.on_graph_map = handle_generic<dumpi_graph_map, DUMPI_Graph_map>;
    callbacks_.on_get_processor_name =
      handle_generic<dumpi_get_processor_name, DUMPI_Get_processor_name>;
    callbacks_.on_get_version =
      handle_generic<dumpi_get_version, DUMPI_Get_version>;
    callbacks_.on_errhandler_create =
      handle_generic<dumpi_errhandler_create, DUMPI_Errhandler_create>;
    callbacks_.on_errhandler_set =
      handle_generic<dumpi_errhandler_set, DUMPI_Errhandler_set>;
    callbacks_.on_errhandler_get =
      handle_generic<dumpi_errhandler_get, DUMPI_Errhandler_get>;
    callbacks_.on_errhandler_free =
      handle_generic<dumpi_errhandler_free, DUMPI_Errhandler_free>;
    callbacks_.on_error_string =
      handle_generic<dumpi_error_string, DUMPI_Error_string>;
    callbacks_.on_error_class =
      handle_generic<dumpi_error_class, DUMPI_Error_class>;
    callbacks_.on_wtime = handle_generic<dumpi_wtime, DUMPI_Wtime>;
    callbacks_.on_wtick = handle_generic<dumpi_wtick, DUMPI_Wtick>;
    callbacks_.on_initialized =
      handle_generic<dumpi_initialized, DUMPI_Initialized>;
    callbacks_.on_abort = handle_generic<dumpi_abort, DUMPI_Abort>;
    callbacks_.on_close_port =
      handle_generic<dumpi_close_port, DUMPI_Close_port>;
    callbacks_.on_lookup_name =
      handle_generic<dumpi_lookup_name, DUMPI_Lookup_name>;
    callbacks_.on_open_port = handle_generic<dumpi_open_port, DUMPI_Open_port>;
    callbacks_.on_publish_name =
      handle_generic<dumpi_publish_name, DUMPI_Publish_name>;
    callbacks_.on_unpublish_name =
      handle_generic<dumpi_unpublish_name, DUMPI_Unpublish_name>;
    callbacks_.on_win_free = handle_generic<dumpi_win_free, DUMPI_Win_free>;
    callbacks_.on_win_get_group =
      handle_generic<dumpi_win_get_group, DUMPI_Win_get_group>;
    callbacks_.on_add_error_class =
      handle_generic<dumpi_add_error_class, DUMPI_Add_error_class>;
    callbacks_.on_add_error_code =
      handle_generic<dumpi_add_error_code, DUMPI_Add_error_code>;
    callbacks_.on_add_error_string =
      handle_generic<dumpi_add_error_string, DUMPI_Add_error_string>;
    callbacks_.on_comm_call_errhandler =
      handle_generic<dumpi_comm_call_errhandler, DUMPI_Comm_call_errhandler>;
    callbacks_.on_comm_create_keyval =
      handle_generic<dumpi_comm_create_keyval, DUMPI_Comm_create_keyval>;
    callbacks_.on_comm_delete_attr =
      handle_generic<dumpi_comm_delete_attr, DUMPI_Comm_delete_attr>;
    callbacks_.on_comm_free_keyval =
      handle_generic<dumpi_comm_free_keyval, DUMPI_Comm_free_keyval>;
    callbacks_.on_comm_get_attr =
      handle_generic<dumpi_comm_get_attr, DUMPI_Comm_get_attr>;
    callbacks_.on_comm_get_name =
      handle_generic<dumpi_comm_get_name, DUMPI_Comm_get_name>;
    callbacks_.on_comm_set_attr =
      handle_generic<dumpi_comm_set_attr, DUMPI_Comm_set_attr>;
    callbacks_.on_comm_set_name =
      handle_generic<dumpi_comm_set_name, DUMPI_Comm_set_name>;
    callbacks_.on_file_call_errhandler =
      handle_generic<dumpi_file_call_errhandler, DUMPI_File_call_errhandler>;
    callbacks_.on_grequest_complete =
      handle_generic<dumpi_grequest_complete, DUMPI_Grequest_complete>;
    callbacks_.on_grequest_start =
      handle_generic<dumpi_grequest_start, DUMPI_Grequest_start>;
    callbacks_.on_is_thread_main =
      handle_generic<dumpi_is_thread_main, DUMPI_Is_thread_main>;
    callbacks_.on_query_thread =
      handle_generic<dumpi_query_thread, DUMPI_Query_thread>;
    callbacks_.on_status_set_cancelled =
      handle_generic<dumpi_status_set_cancelled, DUMPI_Status_set_cancelled>;
    callbacks_.on_status_set_elements =
      handle_generic<dumpi_status_set_elements, DUMPI_Status_set_elements>;
    callbacks_.on_type_create_keyval =
      handle_generic<dumpi_type_create_keyval, DUMPI_Type_create_keyval>;
    callbacks_.on_type_delete_attr =
      handle_generic<dumpi_type_delete_attr, DUMPI_Type_delete_attr>;
    callbacks_.on_type_dup = handle_generic<dumpi_type_dup, DUMPI_Type_dup>;
    callbacks_.on_type_free_keyval =
      handle_generic<dumpi_type_free_keyval, DUMPI_Type_free_keyval>;
    callbacks_.on_type_get_attr =
      handle_generic<dumpi_type_get_attr, DUMPI_Type_get_attr>;
    callbacks_.on_type_get_contents =
      handle_generic<dumpi_type_get_contents, DUMPI_Type_get_contents>;
    callbacks_.on_type_get_envelope =
      handle_generic<dumpi_type_get_envelope, DUMPI_Type_get_envelope>;
    callbacks_.on_type_get_name =
      handle_generic<dumpi_type_get_name, DUMPI_Type_get_name>;
    callbacks_.on_type_set_attr =
      handle_generic<dumpi_type_set_attr, DUMPI_Type_set_attr>;
    callbacks_.on_type_set_name =
      handle_generic<dumpi_type_set_name, DUMPI_Type_set_name>;
    callbacks_.on_type_match_size =
      handle_generic<dumpi_type_match_size, DUMPI_Type_match_size>;
    callbacks_.on_win_call_errhandler =
      handle_generic<dumpi_win_call_errhandler, DUMPI_Win_call_errhandler>;
    callbacks_.on_win_create_keyval =
      handle_generic<dumpi_win_create_keyval, DUMPI_Win_create_keyval>;
    callbacks_.on_win_delete_attr =
      handle_generic<dumpi_win_delete_attr, DUMPI_Win_delete_attr>;
    callbacks_.on_win_free_keyval =
      handle_generic<dumpi_win_free_keyval, DUMPI_Win_free_keyval>;
    callbacks_.on_win_get_attr =
      handle_generic<dumpi_win_get_attr, DUMPI_Win_get_attr>;
    callbacks_.on_win_get_name =
      handle_generic<dumpi_win_get_name, DUMPI_Win_get_name>;
    callbacks_.on_win_set_attr =
      handle_generic<dumpi_win_set_attr, DUMPI_Win_set_attr>;
    callbacks_.on_win_set_name =
      handle_generic<dumpi_win_set_name, DUMPI_Win_set_name>;
    callbacks_.on_alloc_mem = handle_generic<dumpi_alloc_mem, DUMPI_Alloc_mem>;
    callbacks_.on_comm_create_errhandler =
      handle_generic<dumpi_comm_create_errhandler, DUMPI_Comm_create_errhandler>;
    callbacks_.on_comm_get_errhandler =
      handle_generic<dumpi_comm_get_errhandler, DUMPI_Comm_get_errhandler>;
    callbacks_.on_comm_set_errhandler =
      handle_generic<dumpi_comm_set_errhandler, DUMPI_Comm_set_errhandler>;
    callbacks_.on_file_create_errhandler =
      handle_generic<dumpi_file_create_errhandler, DUMPI_File_create_errhandler>;
    callbacks_.on_file_get_errhandler =
      handle_generic<dumpi_file_get_errhandler, DUMPI_File_get_errhandler>;
    callbacks_.on_file_set_errhandler =
      handle_generic<dumpi_file_set_errhandler, DUMPI_File_set_errhandler>;
    callbacks_.on_finalized = handle_generic<dumpi_finalized, DUMPI_Finalized>;
    callbacks_.on_free_mem = handle_generic<dumpi_free_mem, DUMPI_Free_mem>;
    callbacks_.on_get_address =
      handle_generic<dumpi_get_address, DUMPI_Get_address>;
    callbacks_.on_info_create =
      handle_generic<dumpi_info_create, DUMPI_Info_create>;
    callbacks_.on_info_delete =
      handle_generic<dumpi_info_delete, DUMPI_Info_delete>;
    callbacks_.on_info_dup = handle_generic<dumpi_info_dup, DUMPI_Info_dup>;
    callbacks_.on_info_free = handle_generic<dumpi_info_free, DUMPI_Info_free>;
    callbacks_.on_info_get = handle_generic<dumpi_info_get, DUMPI_Info_get>;
    callbacks_.on_info_get_nkeys =
      handle_generic<dumpi_info_get_nkeys, DUMPI_Info_get_nkeys>;
    callbacks_.on_info_get_nthkey =
      handle_generic<dumpi_info_get_nthkey, DUMPI_Info_get_nthkey>;
    callbacks_.on_info_get_valuelen =
      handle_generic<dumpi_info_get_valuelen, DUMPI_Info_get_valuelen>;
    callbacks_.on_info_set = handle_generic<dumpi_info_set, DUMPI_Info_set>;
    callbacks_.on_pack_external =
      handle_generic<dumpi_pack_external, DUMPI_Pack_external>;
    callbacks_.on_pack_external_size =
      handle_generic<dumpi_pack_external_size, DUMPI_Pack_external_size>;
    callbacks_.on_request_get_status =
      handle_generic<dumpi_request_get_status, DUMPI_Request_get_status>;
    callbacks_.on_type_create_darray =
      handle_generic<dumpi_type_create_darray, DUMPI_Type_create_darray>;
    callbacks_.on_type_create_hindexed =
      handle_generic<dumpi_type_create_hindexed, DUMPI_Type_create_hindexed>;
    callbacks_.on_type_create_hvector =
      handle_generic<dumpi_type_create_hvector, DUMPI_Type_create_hvector>;
    callbacks_.on_type_create_indexed_block =
      handle_generic<dumpi_type_create_indexed_block,
      DUMPI_Type_create_indexed_block>;
    callbacks_.on_type_create_resized =
      handle_generic<dumpi_type_create_resized, DUMPI_Type_create_resized>;
    callbacks_.on_type_create_struct =
      handle_generic<dumpi_type_create_struct, DUMPI_Type_create_struct>;
    callbacks_.on_type_create_subarray =
      handle_generic<dumpi_type_create_subarray, DUMPI_Type_create_subarray>;
    callbacks_.on_type_get_extent =
      handle_generic<dumpi_type_get_extent, DUMPI_Type_get_extent>;
    callbacks_.on_type_get_true_extent =
      handle_generic<dumpi_type_get_true_extent, DUMPI_Type_get_true_extent>;
    callbacks_.on_unpack_external =
      handle_generic<dumpi_unpack_external, DUMPI_Unpack_external>;
    callbacks_.on_win_create_errhandler =
      handle_generic<dumpi_win_create_errhandler, DUMPI_Win_create_errhandler>;
    callbacks_.on_win_get_errhandler =
      handle_generic<dumpi_win_get_errhandler, DUMPI_Win_get_errhandler>;
    callbacks_.on_win_set_errhandler =
      handle_generic<dumpi_win_set_errhandler, DUMPI_Win_set_errhandler>;
    //callbacks_.on_function_enter = <?>;
    //callbacks_.on_function_exit = <?>;
  }

  //
  // Define function labels.
  //
  void otfwriter::define_mpi_functions() {
    static const int mpi_function_group = 1000;
    static const int stream = 0;
    static const int barr = OTF_COLLECTIVE_TYPE_BARRIER;
    static const int all2all = OTF_COLLECTIVE_TYPE_ALL2ALL;
    static const int one2all = OTF_COLLECTIVE_TYPE_ONE2ALL;
    static const int all2one = OTF_COLLECTIVE_TYPE_ALL2ONE;
    OTF_Writer_writeDefFunctionGroup(writer_, stream, mpi_function_group,
				     "Native MPI functions");
#define DUMPI_DFUNC(FUNC) do {						\
      OTF_Writer_writeDefFunction(writer_, stream, (DU ## FUNC)+1,	\
				  #FUNC, mpi_function_group, 0);	\
    } while(0)
#define DUMPI_DCOLL(FUNC, TYPE) do {					\
      DUMPI_DFUNC(FUNC);						\
      OTF_Writer_writeDefCollectiveOperation(writer_, stream, (DU ## FUNC)+1, \
					     #FUNC, TYPE);		\
    } while(0)
    DUMPI_DFUNC(MPI_Send);                      DUMPI_DFUNC(MPI_Recv);                      
    DUMPI_DFUNC(MPI_Get_count);                 DUMPI_DFUNC(MPI_Bsend);                     
    DUMPI_DFUNC(MPI_Ssend);                     DUMPI_DFUNC(MPI_Rsend);                     
    DUMPI_DFUNC(MPI_Buffer_attach);             DUMPI_DFUNC(MPI_Buffer_detach);             
    DUMPI_DFUNC(MPI_Isend);                     DUMPI_DFUNC(MPI_Ibsend);                    
    DUMPI_DFUNC(MPI_Issend);                    DUMPI_DFUNC(MPI_Irsend);                    
    DUMPI_DFUNC(MPI_Irecv);                     DUMPI_DFUNC(MPI_Wait);                      
    DUMPI_DFUNC(MPI_Test);                      DUMPI_DFUNC(MPI_Request_free);              
    DUMPI_DFUNC(MPI_Waitany);                   DUMPI_DFUNC(MPI_Testany);                   
    DUMPI_DFUNC(MPI_Waitall);                   DUMPI_DFUNC(MPI_Testall);                   
    DUMPI_DFUNC(MPI_Waitsome);                  DUMPI_DFUNC(MPI_Testsome);                  
    DUMPI_DFUNC(MPI_Iprobe);                    DUMPI_DFUNC(MPI_Probe);                     
    DUMPI_DFUNC(MPI_Cancel);                    DUMPI_DFUNC(MPI_Test_cancelled);            
    DUMPI_DFUNC(MPI_Send_init);                 DUMPI_DFUNC(MPI_Bsend_init);                
    DUMPI_DFUNC(MPI_Ssend_init);                DUMPI_DFUNC(MPI_Rsend_init);                
    DUMPI_DFUNC(MPI_Recv_init);                 DUMPI_DFUNC(MPI_Start);                     
    DUMPI_DFUNC(MPI_Startall);                  DUMPI_DFUNC(MPI_Sendrecv);                  
    DUMPI_DFUNC(MPI_Sendrecv_replace);          DUMPI_DFUNC(MPI_Type_contiguous);           
    DUMPI_DFUNC(MPI_Type_vector);               DUMPI_DFUNC(MPI_Type_hvector);              
    DUMPI_DFUNC(MPI_Type_indexed);              DUMPI_DFUNC(MPI_Type_hindexed);             
    DUMPI_DFUNC(MPI_Type_struct);               DUMPI_DFUNC(MPI_Address);                   
    DUMPI_DFUNC(MPI_Type_extent);               DUMPI_DFUNC(MPI_Type_size);                 
    DUMPI_DFUNC(MPI_Type_lb);                   DUMPI_DFUNC(MPI_Type_ub);                   
    DUMPI_DFUNC(MPI_Type_commit);               DUMPI_DFUNC(MPI_Type_free);                 
    DUMPI_DFUNC(MPI_Get_elements);              DUMPI_DFUNC(MPI_Pack);                      
    DUMPI_DFUNC(MPI_Unpack);                    DUMPI_DFUNC(MPI_Pack_size);                 
    DUMPI_DCOLL(MPI_Barrier, barr);             DUMPI_DCOLL(MPI_Bcast, one2all);
    DUMPI_DCOLL(MPI_Gather, all2one);           DUMPI_DCOLL(MPI_Gatherv, all2one);
    DUMPI_DCOLL(MPI_Scatter, one2all);          DUMPI_DCOLL(MPI_Scatterv, one2all);
    DUMPI_DCOLL(MPI_Allgather, all2all);        DUMPI_DCOLL(MPI_Allgatherv, all2all);
    DUMPI_DCOLL(MPI_Alltoall, all2all);         DUMPI_DCOLL(MPI_Alltoallv, all2all);
    DUMPI_DCOLL(MPI_Reduce, all2one);           DUMPI_DFUNC(MPI_Op_create);                 
    DUMPI_DFUNC(MPI_Op_free);                   DUMPI_DCOLL(MPI_Allreduce, all2all);
    DUMPI_DCOLL(MPI_Reduce_scatter, all2all);   DUMPI_DCOLL(MPI_Scan, all2all);
    DUMPI_DFUNC(MPI_Group_size);                DUMPI_DFUNC(MPI_Group_rank);                
    DUMPI_DFUNC(MPI_Group_translate_ranks);     DUMPI_DFUNC(MPI_Group_compare);             
    DUMPI_DFUNC(MPI_Comm_group);                DUMPI_DFUNC(MPI_Group_union);               
    DUMPI_DFUNC(MPI_Group_intersection);        DUMPI_DFUNC(MPI_Group_difference);          
    DUMPI_DFUNC(MPI_Group_incl);                DUMPI_DFUNC(MPI_Group_excl);                
    DUMPI_DFUNC(MPI_Group_range_incl);          DUMPI_DFUNC(MPI_Group_range_excl);          
    DUMPI_DFUNC(MPI_Group_free);                DUMPI_DFUNC(MPI_Comm_size);                 
    DUMPI_DFUNC(MPI_Comm_rank);                 DUMPI_DFUNC(MPI_Comm_compare);              
    DUMPI_DCOLL(MPI_Comm_dup, barr);            DUMPI_DCOLL(MPI_Comm_create, barr);         
    DUMPI_DCOLL(MPI_Comm_split, barr);          DUMPI_DCOLL(MPI_Comm_free, barr);         
    DUMPI_DFUNC(MPI_Comm_test_inter);           DUMPI_DFUNC(MPI_Comm_remote_size);          
    DUMPI_DFUNC(MPI_Comm_remote_group);         DUMPI_DCOLL(MPI_Intercomm_create, barr);  
    DUMPI_DFUNC(MPI_Intercomm_merge);           DUMPI_DFUNC(MPI_Keyval_create);             
    DUMPI_DFUNC(MPI_Keyval_free);               DUMPI_DFUNC(MPI_Attr_put);                  
    DUMPI_DFUNC(MPI_Attr_get);                  DUMPI_DFUNC(MPI_Attr_delete);               
    DUMPI_DFUNC(MPI_Topo_test);                 DUMPI_DFUNC(MPI_Cart_create);               
    DUMPI_DFUNC(MPI_Dims_create);               DUMPI_DFUNC(MPI_Graph_create);              
    DUMPI_DFUNC(MPI_Graphdims_get);             DUMPI_DFUNC(MPI_Graph_get);                 
    DUMPI_DFUNC(MPI_Cartdim_get);               DUMPI_DFUNC(MPI_Cart_get);                  
    DUMPI_DFUNC(MPI_Cart_rank);                 DUMPI_DFUNC(MPI_Cart_coords);               
    DUMPI_DFUNC(MPI_Graph_neighbors_count);     DUMPI_DFUNC(MPI_Graph_neighbors);           
    DUMPI_DFUNC(MPI_Cart_shift);                DUMPI_DFUNC(MPI_Cart_sub);                  
    DUMPI_DFUNC(MPI_Cart_map);                  DUMPI_DFUNC(MPI_Graph_map);                 
    DUMPI_DFUNC(MPI_Get_processor_name);        DUMPI_DFUNC(MPI_Get_version);               
    DUMPI_DFUNC(MPI_Errhandler_create);         DUMPI_DFUNC(MPI_Errhandler_set);            
    DUMPI_DFUNC(MPI_Errhandler_get);            DUMPI_DFUNC(MPI_Errhandler_free);           
    DUMPI_DFUNC(MPI_Error_string);              DUMPI_DFUNC(MPI_Error_class);               
    DUMPI_DFUNC(MPI_Wtime);                     DUMPI_DFUNC(MPI_Wtick);                     
    DUMPI_DCOLL(MPI_Init, barr);                DUMPI_DCOLL(MPI_Finalize, barr);
    DUMPI_DFUNC(MPI_Initialized);               DUMPI_DFUNC(MPI_Abort);                     
    DUMPI_DFUNC(MPI_Pcontrol);                  DUMPI_DFUNC(MPI_Close_port);                
    DUMPI_DFUNC(MPI_Comm_accept);               DUMPI_DFUNC(MPI_Comm_connect);              
    DUMPI_DFUNC(MPI_Comm_disconnect);           DUMPI_DFUNC(MPI_Comm_get_parent);           
    DUMPI_DFUNC(MPI_Comm_join);                 DUMPI_DCOLL(MPI_Comm_spawn, barr);
    DUMPI_DCOLL(MPI_Comm_spawn_multiple, barr); DUMPI_DFUNC(MPI_Lookup_name);
    DUMPI_DFUNC(MPI_Open_port);                 DUMPI_DFUNC(MPI_Publish_name);              
    DUMPI_DFUNC(MPI_Unpublish_name);            DUMPI_DFUNC(MPI_Accumulate);                
    DUMPI_DFUNC(MPI_Get);                       DUMPI_DFUNC(MPI_Put);                       
    DUMPI_DFUNC(MPI_Win_complete);              DUMPI_DFUNC(MPI_Win_create);                
    DUMPI_DFUNC(MPI_Win_fence);                 DUMPI_DFUNC(MPI_Win_free);                  
    DUMPI_DFUNC(MPI_Win_get_group);             DUMPI_DFUNC(MPI_Win_lock);                  
    DUMPI_DFUNC(MPI_Win_post);                  DUMPI_DFUNC(MPI_Win_start);                 
    DUMPI_DFUNC(MPI_Win_test);                  DUMPI_DFUNC(MPI_Win_unlock);                
    DUMPI_DFUNC(MPI_Win_wait);                  DUMPI_DCOLL(MPI_Alltoallw, all2all);
    DUMPI_DCOLL(MPI_Exscan, all2all);           DUMPI_DFUNC(MPI_Add_error_class);           
    DUMPI_DFUNC(MPI_Add_error_code);            DUMPI_DFUNC(MPI_Add_error_string);          
    DUMPI_DFUNC(MPI_Comm_call_errhandler);      DUMPI_DFUNC(MPI_Comm_create_keyval);        
    DUMPI_DFUNC(MPI_Comm_delete_attr);          DUMPI_DFUNC(MPI_Comm_free_keyval);          
    DUMPI_DFUNC(MPI_Comm_get_attr);             DUMPI_DFUNC(MPI_Comm_get_name);             
    DUMPI_DFUNC(MPI_Comm_set_attr);             DUMPI_DFUNC(MPI_Comm_set_name);             
    DUMPI_DFUNC(MPI_File_call_errhandler);      DUMPI_DFUNC(MPI_Grequest_complete);         
    DUMPI_DFUNC(MPI_Grequest_start);            DUMPI_DCOLL(MPI_Init_thread, barr);        
    DUMPI_DFUNC(MPI_Is_thread_main);            DUMPI_DFUNC(MPI_Query_thread);              
    DUMPI_DFUNC(MPI_Status_set_cancelled);      DUMPI_DFUNC(MPI_Status_set_elements);       
    DUMPI_DFUNC(MPI_Type_create_keyval);        DUMPI_DFUNC(MPI_Type_delete_attr);          
    DUMPI_DFUNC(MPI_Type_dup);                  DUMPI_DFUNC(MPI_Type_free_keyval);          
    DUMPI_DFUNC(MPI_Type_get_attr);             DUMPI_DFUNC(MPI_Type_get_contents);         
    DUMPI_DFUNC(MPI_Type_get_envelope);         DUMPI_DFUNC(MPI_Type_get_name);             
    DUMPI_DFUNC(MPI_Type_set_attr);             DUMPI_DFUNC(MPI_Type_set_name);             
    DUMPI_DFUNC(MPI_Type_match_size);           DUMPI_DFUNC(MPI_Win_call_errhandler);       
    DUMPI_DFUNC(MPI_Win_create_keyval);         DUMPI_DFUNC(MPI_Win_delete_attr);           
    DUMPI_DFUNC(MPI_Win_free_keyval);           DUMPI_DFUNC(MPI_Win_get_attr);              
    DUMPI_DFUNC(MPI_Win_get_name);              DUMPI_DFUNC(MPI_Win_set_attr);              
    DUMPI_DFUNC(MPI_Win_set_name);              DUMPI_DFUNC(MPI_Alloc_mem);                 
    DUMPI_DFUNC(MPI_Comm_create_errhandler);    DUMPI_DFUNC(MPI_Comm_get_errhandler);       
    DUMPI_DFUNC(MPI_Comm_set_errhandler);       DUMPI_DFUNC(MPI_File_create_errhandler);    
    DUMPI_DFUNC(MPI_File_get_errhandler);       DUMPI_DFUNC(MPI_File_set_errhandler);       
    DUMPI_DFUNC(MPI_Finalized);                 DUMPI_DFUNC(MPI_Free_mem);                  
    DUMPI_DFUNC(MPI_Get_address);               DUMPI_DFUNC(MPI_Info_create);               
    DUMPI_DFUNC(MPI_Info_delete);               DUMPI_DFUNC(MPI_Info_dup);                  
    DUMPI_DFUNC(MPI_Info_free);                 DUMPI_DFUNC(MPI_Info_get);                  
    DUMPI_DFUNC(MPI_Info_get_nkeys);            DUMPI_DFUNC(MPI_Info_get_nthkey);           
    DUMPI_DFUNC(MPI_Info_get_valuelen);         DUMPI_DFUNC(MPI_Info_set);                  
    DUMPI_DFUNC(MPI_Pack_external);             DUMPI_DFUNC(MPI_Pack_external_size);        
    DUMPI_DFUNC(MPI_Request_get_status);        DUMPI_DFUNC(MPI_Type_create_darray);        
    DUMPI_DFUNC(MPI_Type_create_hindexed);      DUMPI_DFUNC(MPI_Type_create_hvector);       
    DUMPI_DFUNC(MPI_Type_create_indexed_block); DUMPI_DFUNC(MPI_Type_create_resized);       
    DUMPI_DFUNC(MPI_Type_create_struct);        DUMPI_DFUNC(MPI_Type_create_subarray);      
    DUMPI_DFUNC(MPI_Type_get_extent);           DUMPI_DFUNC(MPI_Type_get_true_extent);      
    DUMPI_DFUNC(MPI_Unpack_external);           DUMPI_DFUNC(MPI_Win_create_errhandler);     
    DUMPI_DFUNC(MPI_Win_get_errhandler);        DUMPI_DFUNC(MPI_Win_set_errhandler);        
    DUMPI_DCOLL(MPI_File_open, barr);           DUMPI_DCOLL(MPI_File_close, barr);
    DUMPI_DFUNC(MPI_File_delete);               DUMPI_DCOLL(MPI_File_set_size, barr);
    DUMPI_DCOLL(MPI_File_preallocate, barr);    DUMPI_DFUNC(MPI_File_get_size);             
    DUMPI_DFUNC(MPI_File_get_group);            DUMPI_DFUNC(MPI_File_get_amode);            
    DUMPI_DCOLL(MPI_File_set_info, barr);       DUMPI_DFUNC(MPI_File_get_info);             
    DUMPI_DCOLL(MPI_File_set_view, barr);       DUMPI_DFUNC(MPI_File_get_view);             
    DUMPI_DFUNC(MPI_File_read_at);              DUMPI_DCOLL(MPI_File_read_at_all, barr);  
    DUMPI_DFUNC(MPI_File_write_at);             DUMPI_DCOLL(MPI_File_write_at_all, barr); 
    DUMPI_DFUNC(MPI_File_iread_at);             DUMPI_DFUNC(MPI_File_iwrite_at);            
    DUMPI_DFUNC(MPI_File_read);                 DUMPI_DCOLL(MPI_File_read_all, barr);     
    DUMPI_DFUNC(MPI_File_write);                DUMPI_DCOLL(MPI_File_write_all, barr);    
    DUMPI_DFUNC(MPI_File_iread);                DUMPI_DFUNC(MPI_File_iwrite);               
    DUMPI_DFUNC(MPI_File_seek);                 DUMPI_DFUNC(MPI_File_get_position);         
    DUMPI_DFUNC(MPI_File_get_byte_offset);      DUMPI_DFUNC(MPI_File_read_shared);          
    DUMPI_DFUNC(MPI_File_write_shared);         DUMPI_DFUNC(MPI_File_iread_shared);         
    DUMPI_DFUNC(MPI_File_iwrite_shared);        DUMPI_DCOLL(MPI_File_read_ordered, barr);  
    DUMPI_DFUNC(MPI_File_write_ordered);        DUMPI_DCOLL(MPI_File_seek_shared, barr);   
    DUMPI_DFUNC(MPI_File_get_position_shared);  DUMPI_DCOLL(MPI_File_read_at_all_begin, barr);
    DUMPI_DCOLL(MPI_File_read_at_all_end,barr); DUMPI_DCOLL(MPI_File_write_at_all_begin,barr);
    DUMPI_DCOLL(MPI_File_write_at_all_end,barr);DUMPI_DCOLL(MPI_File_read_all_begin, barr); 
    DUMPI_DCOLL(MPI_File_read_all_end, barr);   DUMPI_DCOLL(MPI_File_write_all_begin, barr); 
    DUMPI_DCOLL(MPI_File_write_all_end, barr);  DUMPI_DCOLL(MPI_File_read_ordered_begin,barr);
    DUMPI_DCOLL(MPI_File_read_ordered_end,barr);DUMPI_DCOLL(MPI_File_write_ordered_begin,barr);  
    DUMPI_DCOLL(MPI_File_write_ordered_end,barr);DUMPI_DFUNC(MPI_File_get_type_extent);      
    DUMPI_DFUNC(MPI_Register_datarep);          DUMPI_DCOLL(MPI_File_set_atomicity, barr); 
    DUMPI_DFUNC(MPI_File_get_atomicity);        DUMPI_DCOLL(MPI_File_sync, barr);           
    DUMPI_DFUNC(MPIO_Test);                     DUMPI_DFUNC(MPIO_Wait);                     
    DUMPI_DFUNC(MPIO_Testall);                  DUMPI_DFUNC(MPIO_Waitall);                  
    DUMPI_DFUNC(MPIO_Testany);                  DUMPI_DFUNC(MPIO_Waitany);                  
    DUMPI_DFUNC(MPIO_Waitsome);                 DUMPI_DFUNC(MPIO_Testsome);
#undef DUMPI_DFUNC
#undef DUMPI_DCOLL
  }

  //
  // Define traced functions locally.
  //
  void otfwriter::define_local_functions(uint32_t stream, dumpi_profile *profile) {
    int i;
    int count = 0;
    uint64_t *addresses = NULL;
    char **names = NULL;
    dumpi_read_function_addresses(profile, &count, &addresses, &names);
    if(count) {
      OTF_Writer_writeDefFunctionGroup(writer_, stream, userfuncs,
				       "User-profiled functions");
      for(int add = 0; add < count; ++add) {
	std::string lbl("Function address ");
        {
          std::stringstream ss;
          ss << addresses[add];
          lbl += ss.str();
        }
	if(names && names[add]) {
	  lbl += " (";
	  lbl += names[add];
	  lbl += ")";
	}
	uint32_t otffunc = add + 3000;
	OTF_Writer_writeDefFunction(writer_, stream, otffunc,
				    lbl.c_str(), userfuncs, 0);
	funclabel_[stream][otffunc] = addresses[add];
      }
      free(addresses);
      free(names[i]);
      free(names);
    }
  }

  //
  // Define perfcounter labels.
  //
  void otfwriter::define_perfctr_labels(uint32_t stream, dumpi_profile *profile) {
    dumpi_perfinfo pinfo;
    dumpi_read_perfctr_labels(profile, &pinfo.count, pinfo.counter_tag,
			      DUMPI_MAX_PERFCTR_NAME);
    if(pinfo.count) {
      OTF_Writer_writeDefCounterGroup(writer_, stream, papictrs,
				      "User-profiled functions");
      for(int i = 0; i < pinfo.count; ++i) {
	OTF_Writer_writeDefCounter(writer_, stream, i, pinfo.counter_tag[i],
				   OTF_COUNTER_TYPE_ACC + OTF_COUNTER_SCOPE_START,
				   papictrs, 0);
      }
    }
  }

  //
  // Write the state of perfcounters at function entry.
  //
  void otfwriter::write_counters_enter(uint64_t start, const dumpi_time *cpu,
				       const dumpi_perfinfo *perf)
  {
    if(cpu) {
      uint64_t value = to_nsec(cpu->start);
      OTF_Writer_writeCounter(writer_, start, current_rank_+1, cputimer, value);
    }
    if(perf && perf->count) {
      for(int i = 0; i < perf->count; ++i) {
	OTF_Writer_writeCounter(writer_, start, current_rank_+1,
				i, perf->invalue[i]);
      }
    }
  }

  //
  // Write the state of perfcounters at function exit.
  //
  void otfwriter::write_counters_leave(uint64_t stop, const dumpi_time *cpu,
				       const dumpi_perfinfo *perf)
  {
    if(cpu) {
      uint64_t value = to_nsec(cpu->stop);
      OTF_Writer_writeCounter(writer_, stop, current_rank_+1, cputimer, value);
    }
    if(perf && perf->count) {
      for(int i = 0; i < perf->count; ++i) {
	OTF_Writer_writeCounter(writer_, stop, current_rank_+1,
				i, perf->outvalue[i]);
      }
    }
  }

} // end of namespace dumpi