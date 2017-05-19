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

#include <dumpi/bin/trace.h>
#include <dumpi/bin/timeutils.h>
#include <dumpi/libundumpi/libundumpi.h>
#include <dumpi/common/io.h>
#include <dumpi/common/constants.h>
#include <set>
#include <exception>
#include <iostream>
#include <limits>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

namespace dumpi {

  //
  // Inline: test whether we have hit a new high or low in timestamps.
  //
  void trace::check_time(const dumpi_time *wall) {
    if(wall->start < this->start_time_) this->start_time_ = wall->start;
    if(wall->stop  > this->stop_time_)  this->stop_time_  = wall->stop;
  }

  //
  // Mark init time.  Calls check_time.
  //
  void trace::check_init(const dumpi_time *wall) {
    this->init_time_ = wall->start;
    check_time(wall);
  }

  //
  // Mark finalize time.  Calls check_time.
  //
  void trace::check_finalize(const dumpi_time *wall) {
    this->finalize_time_ = wall->stop;
    check_time(wall);
  }

  //
  // Creation.
  //
  trace::trace() :
    index_(-1), filename_(""), profile_(NULL), off_(0), state_(PREPARSE_FRESH),
    shared_(NULL), mpi_finalized_(0)
  {
    pending_communicator_.id = -1;
    memset(callarr_, 0, DUMPI_END_OF_STREAM * sizeof(libundumpi_cbpair));
    start_time_.sec = start_time_.nsec = std::numeric_limits<int32_t>::max();
    stop_time_.sec = stop_time_.nsec = std::numeric_limits<int32_t>::min();
    init_time_.sec = init_time_.nsec = 0;
    finalize_time_.sec = finalize_time_.nsec = 0;
  }

  //
  // Private method to assign default datatype sizes for old trace files.
  //
  struct ts_t { int label; int size; };
  void trace::assign_default_sizes() {
    static ts_t size[] = {
      {DUMPI_DATATYPE_ERROR, 0},
      {DUMPI_DATATYPE_NULL, 0},
      {DUMPI_CHAR, 1},
      {DUMPI_SIGNED_CHAR, 1},
      {DUMPI_UNSIGNED_CHAR, 1},
      {DUMPI_BYTE, 1},
      {DUMPI_WCHAR, 2},
      {DUMPI_SHORT, 2},
      {DUMPI_UNSIGNED_SHORT, 2},
      {DUMPI_INT, 4},
      {DUMPI_UNSIGNED, 4},
      {DUMPI_LONG, 8},
      {DUMPI_UNSIGNED_LONG, 8},
      {DUMPI_FLOAT, 4},
      {DUMPI_DOUBLE, 8},
      {DUMPI_LONG_DOUBLE, 16},
      {DUMPI_LONG_LONG_INT, 8},
      {DUMPI_UNSIGNED_LONG_LONG, 8},
      {DUMPI_LONG_LONG, 8},
      {DUMPI_PACKED, 1},
      {DUMPI_LB, 0},
      {DUMPI_UB, 0},
      {DUMPI_FLOAT_INT, 8},
      {DUMPI_DOUBLE_INT, 12},
      {DUMPI_LONG_INT, 12},
      {DUMPI_SHORT_INT, 6},
      {DUMPI_2INT, 8},
      {DUMPI_LONG_DOUBLE_INT, 20}
    };
    for(int i = 0; i < DUMPI_FIRST_USER_DATATYPE; ++i)
      types_.insert(std::make_pair(i, typeentry(type(size[i].size))));
  }

  //
  // Initialize the trace.
  //
  void trace::init(sharedstate *shared, const std::string &filename, int index) {
    assert(shared != NULL);
    assert(index >= 0);
    filename_ = filename;
    //std::cerr << "undumpi_open(" << filename_ << ")\n";
    profile_ = undumpi_open(filename_.c_str());
    if(! profile_) {
      std::cerr << "trace::init:  Failed to open " << filename_ << ": "
		<< strerror(errno) << "\n";
    }
    index_ = index;
    shared_ = shared;
    setup_callbacks();
    dumpi_start_stream_read(profile_);
    this->off_ = ftello(profile_->file);
    // Initialize built-in types.  This is, unfortunately, a bit of a mess.
    dumpi_sizeof size = undumpi_read_datatype_sizes(profile_);
    // We may need to set defaults if this trace file is very old.
    if(size.count < DUMPI_FIRST_USER_DATATYPE-1)
      assign_default_sizes();
    else
      for(int i = 0; i < size.count; ++i)
	types_.insert(std::make_pair(i, typeentry(type(size.size[i]))));
    free(size.size);
    // Done with the file.
    fclose(profile_->file);
    // Initialize containers.
    comm world = shared_->retrieve_world(index_);
    if(world.get_group().get_global_rank() != index_) {
      throw "Program syntax error:  Our world rank is wrong.";
    }
    comms_.insert(std::make_pair(DUMPI_COMM_NULL, commentry(comm())));
    comms_.insert(std::make_pair(DUMPI_COMM_WORLD, world));
    comms_.insert(std::make_pair(DUMPI_COMM_SELF,
				 commentry(shared_->retrieve_self(index_))));
    // Done initializing.
    state_ = PREPARSE_READY;
  }

  //
  // Preparse until we hit a blocking call or finish the trace stream.
  //
  trace::state trace::preparse() {
    //std::cerr << "preparse\n";
    if(state_ == PREPARSE_FRESH) {
      std::cerr << "trace::preparse called without initializing trace.\n";
      throw "a fit";
    }
    else if(state_ == PREPARSE_BLOCKED) {
      // We are on a blocked collective (communicator call) -- let's see
      // whehter we have a complete communicator now.
      //std::cerr << "this->pending_communicator_.id == "
      //      << this->pending_communicator_.id << "\n";
      if(this->pending_communicator_.id >= 0 &&
	 shared_->is_complete(pending_communicator_.handle))
	{
	  comm newcomm =
	    shared_->retrieve_comm(this->pending_communicator_.handle);
	  if(newcomm.get_group().get_global_rank() >= 0 &&
	     newcomm.get_group().get_global_rank() != index_)
	    {
	      std::cerr << "Node rank " << index_ << " received comm with 'me' "
			<< newcomm.get_group().get_global_rank() << "\n";
	      throw "trace::preparse:  Bogus communicator.";
	    }
	  //std::cerr << "Comm: " << newcomm.get_unique_index() << ".  Node: "
	  //	  << index_ << ".  My rank: " 
	  //	  << newcomm.get_group().get_global_rank() << "\n";
	  commentry entry(newcomm, pending_communicator_.created);
	  comms_.insert(std::make_pair(pending_communicator_.id, entry));
	  pending_communicator_.id = -1;
	  state_ = PREPARSE_READY;
	  //std::cerr << "Communicator fetch complete for rank " << index_ << "\n";
	}
      else {
	//std::cerr << "Communicator not complete for rank " << index_ << "\n";
      }
    }
    if(state_ != PREPARSE_READY) {
      // Still blocked
      return state_;
    }
    // We get here because we have PREPARSE_READY.
    assert((profile_->file = fopen(filename_.c_str(), "r")) != NULL);
    assert(profile_->file != NULL);
    assert(fseeko(profile_->file, this->off_, SEEK_SET) == 0);
    // Resume parsing until we hit end of stream or another comm. operation.
    do {
      int active_stream =
	undumpi_read_single_call(profile_, callarr_, this,
				 &this->mpi_finalized_);
      if(! active_stream) {
	state_ = PREPARSE_DONE;
	break;
      }
    } while(this->state_ != PREPARSE_BLOCKED);
    this->off_ = ftello(profile_->file);
    fclose(profile_->file);
    // We get here because we are either done or blocked.
    if(state_ != PREPARSE_DONE) {
      // Signal to the caller that we advanced until we hit a block.
      return PREPARSE_ADVANCED;
    }
    else {
      return state_;
    }
  }

  //
  // Test whether there is a communicator with the given index.
  //
  bool trace::has_comm(int commhandle) const {
    return (comms_.find(commhandle) != comms_.end());
  }

  //
  // Add a commentry.
  //
  void trace::add_commentry(int commhandle, const comm &the_comm) {
    std::pair<commmap_t::const_iterator, commmap_t::const_iterator> it =
      this->comms_.equal_range(commhandle);
    if(it.first != it.second) { // if a match is found
      // ensure that the newest commentry is not currently active.
      commmap_t::const_iterator ele = it.second;
      --ele;
      if(ele->second.freed <= ele->second.created)
	throw "trace::add_commentry:  Communicator index is currently active";
    }
    comms_.insert(std::make_pair(commhandle, commentry(the_comm)));
  }

  //
  // Get the most recent commentry with the given index.
  //
  trace::commentry& trace::get_commentry(int commhandle) {
    std::pair<commmap_t::iterator, commmap_t::iterator> it =
      this->comms_.equal_range(commhandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_commentry:  Invalid communicator index.";
    commmap_t::iterator ele = it.second;
    --ele;
    return ele->second;
  }

  //
  // Get the most recent communicator with the given index.
  //
  const comm& trace::get_comm(int commhandle, bool must_be_active) const {
    std::pair<commmap_t::const_iterator, commmap_t::const_iterator> it =
      this->comms_.equal_range(commhandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_comm:  Invalid communicator index.";
    // We assume that the last communicator in the range
    // is the newest one. The ordering in the multimap may not be
    // fully guaranteed by the standard, but implementations we care about
    // seem to retain insertion order within each key.
    commmap_t::const_iterator ele = it.second;
    ele--;
    if(must_be_active && ele->second.freed > ele->second.created) {
      std::cerr << "freed   = " << ele->second.freed << "\n"
		<< "created = " << ele->second.created << "\n";
      throw "trace::get_comm:  Communicator has already been freed.";
    }
    return ele->second.the_comm;
  }

  //
  // Get a communicator with the given index that was valid at the
  // given time stamp.
  //
  const comm& trace::get_comm(int commhandle, const dumpi_clock &at_time) const {
    std::pair<commmap_t::const_iterator, commmap_t::const_iterator> it =
      this->comms_.equal_range(commhandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_comm:  Invalid communicator index.";
    for(commmap_t::const_iterator ele = it.first; ele != it.second; ++ele) {
      if(ele->second.created <= at_time &&
	 (ele->second.freed<=ele->second.created || ele->second.freed>at_time))
	{
	  if(ele->second.the_comm.get_group().get_global_rank() != index_)
	    throw "trace::get_comm:  probable coding error: Invalid rank.";
	  return ele->second.the_comm;
	}
    }
    // Not found.
    throw "trace::get_comm:  No communicator valid at the given time.";
  }

  //
  // Test whether there is a group with the given index.
  //
  bool trace::has_group(int grouphandle) const {
    return (groups_.find(grouphandle) != groups_.end());
  }

  //
  // Add a groupentry.
  //
  void trace::add_groupentry(int grouphandle, const group &the_group) {
    std::pair<groupmap_t::const_iterator, groupmap_t::const_iterator> it =
      this->groups_.equal_range(grouphandle);
    if(it.first != it.second) { // if a match is found
      // ensure that the newest groupentry is not currently active.
      groupmap_t::const_iterator ele = it.second;
      --ele;
      if(ele->second.freed <= ele->second.created)
	throw "trace::add_groupentry:  Group index is currently active";
    }
    groups_.insert(std::make_pair(grouphandle, groupentry(the_group)));
  }

  //
  // Get the most recent groupentry with the given index.
  //
  trace::groupentry& trace::get_groupentry(int grouphandle) {
    std::pair<groupmap_t::iterator, groupmap_t::iterator> it =
      this->groups_.equal_range(grouphandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_groupentry:  Invalid group index.";
    groupmap_t::iterator ele = it.second;
    --ele;
    return ele->second;
  }

  //
  // Get the most recent group with the given index.
  //
  const group& trace::get_group(int grouphandle, bool must_be_active) const {
    //std::cerr << "trace::get_group(" << grouphandle << ")\n";
    std::pair<groupmap_t::const_iterator, groupmap_t::const_iterator> it =
      this->groups_.equal_range(grouphandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_group:  Invalid group index.";
    // We assume that the last group in the range
    // is the newest one. The ordering in the multimap may not be
    // fully guaranteed by the standard, but implementations we care about
    // seem to retain insertion order within each key.
    groupmap_t::const_iterator ele = it.second;
    ele--;
    if(must_be_active && ele->second.freed > ele->second.created) {
      std::cerr << "freed   = " << ele->second.freed << "\n"
		<< "created = " << ele->second.created << "\n";
      throw "trace::get_comm:  Group has already been freed.";
    }
    return ele->second.the_group;
  }

  //
  // Get a group with the given index that was valid at the
  // given time stamp.
  //
  const group& trace::get_group(int grouphandle, const dumpi_clock &at_time) const {
    std::pair<groupmap_t::const_iterator, groupmap_t::const_iterator> it =
      this->groups_.equal_range(grouphandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_group:  Invalid group index.";
    for(groupmap_t::const_iterator ele = it.first; ele != it.second; ++ele) {
      if(ele->second.created <= at_time &&
	 (ele->second.freed<=ele->second.created || ele->second.freed>at_time))
	{
	  return ele->second.the_group;
	}
    }
    // Not found.
    throw "trace::get_group:  No group valid at the given time.";
  }

  //
  // Test whether there is a type with the given index.
  //
  bool trace::has_type(int typehandle) const {
    return (types_.find(typehandle) != types_.end());
  }

  //
  // Add a typeentry.
  //
  void trace::add_typeentry(int typehandle, const type &the_type) {
    std::pair<typemap_t::const_iterator, typemap_t::const_iterator> it =
      this->types_.equal_range(typehandle);
    if(it.first != it.second) { // if a match is found
      // ensure that the newest typeentry is not currently active.
      typemap_t::const_iterator ele = it.second;
      --ele;
      if(ele->second.freed <= ele->second.created)
	throw "trace::add_typeentry:  Type index is currently active";
    }
    types_.insert(std::make_pair(typehandle, typeentry(the_type)));
  }

  //
  // Get the most recent typeentry with the given index.
  //
  trace::typeentry& trace::get_typeentry(int typehandle) {
    std::pair<typemap_t::iterator, typemap_t::iterator> it =
      this->types_.equal_range(typehandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_typeentry:  Invalid type index.";
    typemap_t::iterator ele = it.second;
    --ele;
    return ele->second;
  }

  //
  // Get the most recent type with the given index.
  //
  const type& trace::get_type(int typehandle, bool must_be_active) const {
    std::pair<typemap_t::const_iterator, typemap_t::const_iterator> it =
      this->types_.equal_range(typehandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_type:  Invalid type index.";
    typemap_t::const_iterator ele = it.second;
    ele--;
    if(must_be_active && 
       ((ele->second.created   > ele->second.committed) ||
        (ele->second.freed     > ele->second.created))) {
      std::cerr << "created    = " << ele->second.created << "\n"
		<< "committed  = " << ele->second.committed << "\n"
		<< "freed      = " << ele->second.freed << "\n";
      throw "trace::get_comm:  Type is not currently active";
    }
    return ele->second.the_type;
  }

  //
  // Get a type with the given index that was valid at the
  // given time stamp.
  //
  const type& trace::get_type(int typehandle, const dumpi_clock &at_time) const {
    std::pair<typemap_t::const_iterator, typemap_t::const_iterator> it =
      this->types_.equal_range(typehandle);
    if(it.first == it.second) // if no match was found.
      throw "trace::get_type:  Invalid type index.";
    for(typemap_t::const_iterator ele = it.first; ele != it.second; ++ele) {
      if(ele->second.created<=at_time && ele->second.committed<=at_time &&
	 (ele->second.freed<=ele->second.created || ele->second.freed>at_time))
	{
	  return ele->second.the_type;
	}
    }
    // Not found.
    throw "trace::get_type:  No type valid at the given time.";
  }

  //
  // Private method to setup callbacks.
  //
  void trace::setup_callbacks() {
    libundumpi_callbacks cb;
    memset(&cb, 0, sizeof(libundumpi_callbacks));
    memset(&callarr_, 0, sizeof(libundumpi_cbpair));
    libundumpi_set_callbacks(&cb, handle_generic);
    cb.on_init = (dumpi_init_call)handle_init;
    cb.on_init_thread = (dumpi_init_thread_call)handle_init;
    cb.on_finalize = (dumpi_finalize_call)handle_finalize;
    cb.on_abort = (dumpi_abort_call)handle_finalize;
    cb.on_comm_dup = handle_comm_dup;
    cb.on_comm_create = handle_comm_create;
    cb.on_comm_split = handle_comm_split;
    cb.on_comm_group = handle_comm_group;
    cb.on_comm_free = handle_comm_free;
    cb.on_group_union = handle_group_union;
    cb.on_group_intersection = handle_group_intersection;
    cb.on_group_difference = handle_group_difference;
    cb.on_group_incl = handle_group_incl;
    cb.on_group_excl = handle_group_excl;
    cb.on_group_range_incl = handle_group_range_incl;
    cb.on_group_range_excl = handle_group_range_excl;
    cb.on_group_free = handle_group_free;
    cb.on_type_contiguous = handle_type_contiguous;
    cb.on_type_vector = handle_type_vector;
    cb.on_type_hvector = handle_type_hvector;
    cb.on_type_indexed = handle_type_indexed;
    cb.on_type_hindexed = handle_type_hindexed;
    cb.on_type_struct = handle_type_struct;
    cb.on_type_dup = handle_type_dup;
    cb.on_type_create_darray = handle_type_create_darray;
    cb.on_type_create_hindexed = handle_type_create_hindexed;
    cb.on_type_create_hvector = handle_type_create_hvector;
    cb.on_type_create_indexed_block = handle_type_create_indexed_block;
    cb.on_type_create_resized = handle_type_create_resized;
    cb.on_type_create_struct = handle_type_create_struct;
    cb.on_type_create_subarray = handle_type_create_subarray;
    cb.on_type_commit = handle_type_commit;
    cb.on_type_free = handle_type_free;
    libundumpi_populate_callbacks(&cb, callarr_);
  }

  /*********************************************************************
   *
   * Everything below is callback methods for trace parsing
   *
   *********************************************************************/

  //
  // Generic catch-all to handle timing info.
  //
  int trace::handle_generic(const void*, uint16_t,
                            const dumpi_time*, const dumpi_time *wall,
                            const dumpi_perfinfo*, void *userarg)
  {
    trace *self = (trace*)userarg;
    self->check_time(wall);
    return 1;
  }

  //
  // Grab MPI_Init and MPI_Init_thread.
  //
  int trace::handle_init(const void*, uint16_t,
                            const dumpi_time*, const dumpi_time *wall,
                            const dumpi_perfinfo*, void *userarg)
  {
    trace *self = (trace*)userarg;
    self->check_init(wall);
    return 1;
  }

  //
  // Grab MPI_Finalize and MPI_Abort.
  //
  int trace::handle_finalize(const void*, uint16_t,
                             const dumpi_time*, const dumpi_time *wall,
                             const dumpi_perfinfo*, void *userarg)
  {
    trace *self = (trace*)userarg;
    self->check_finalize(wall);
    return 1;
  }

  //
  // Precheck for comm create routines.
  //
  const comm& trace::comm_creation_precheck(int commhandle) {
    if(this->pending_communicator_.id >= 0) {
      // We don't support multithreaded calls into comm creation routines.
      // This may actually end up being hard to do, since we will have to
      // figure out (in the preparse routine) whether the next call on a stream
      // is for a blocked thread or not (we may effectively need to split the
      // stream onto separate threads), and we can have a situation where
      // thread A creates a communicator which is then used by thread B.
      throw "trace::handle_comm_dup:  already have a pending communicator.";
    }
    return this->get_comm(commhandle);
  }

  //
  // MPI_Comm_dup.  We make this a collective call to ensure that we can
  // giv the same unique id to all communicators created.
  //
  int trace::handle_comm_dup(const dumpi_comm_dup *prm, uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    trace *self = (trace*)userarg;
    //std::cerr << "DEBUG: trace::handle_comm_dup\n";
    const comm &oldcomm = self->comm_creation_precheck(prm->oldcomm);
    self->pending_communicator_.id = prm->newcomm;
    self->pending_communicator_.created = wall->stop;
    self->pending_communicator_.handle = self->shared_->comm_dup(oldcomm);
    self->state_ = PREPARSE_BLOCKED;
    return 1;
  }

  int trace::handle_comm_create(const dumpi_comm_create *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    trace *self = (trace*)userarg;
    //std::cerr << "DEBUG: trace::handle_comm_create\n";
    const comm &oldcomm = self->comm_creation_precheck(prm->oldcomm);
    //std::cerr << "    self->get_group(" << prm->group << ")\n";
    const group &ingroup = self->get_group(prm->group);
    self->pending_communicator_.id = prm->newcomm;
    self->pending_communicator_.created = wall->stop;
    self->pending_communicator_.handle =
      self->shared_->comm_create(oldcomm, ingroup);
    self->state_ = PREPARSE_BLOCKED;
    return 1;
  }

  int trace::handle_comm_split(const dumpi_comm_split *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    trace *self = (trace*)userarg;
    //std::cerr << "DEBUG: trace::handle_comm_split\n";
    const comm &oldcomm = self->comm_creation_precheck(prm->oldcomm);
    self->pending_communicator_.id = prm->newcomm;
    self->pending_communicator_.created = wall->stop;
    self->pending_communicator_.handle =
      self->shared_->comm_split(oldcomm, prm->color, prm->key);
    self->state_ = PREPARSE_BLOCKED;
    return 1;
  }

  int trace::handle_comm_group(const dumpi_comm_group *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    trace *self = (trace*)userarg;
    //std::cerr << "DEBUG:  trace::handle_comm_group(id=" << prm->group << ")\n";
    const comm &the_comm = self->get_comm(prm->comm);
    // It is possible that this group index has already been queried before.
    // We only add the group if this is not the case.
    if(! (self->has_group(prm->group) &&
	  self->get_group(prm->group).is_identical_to(the_comm.get_group())))
      {
	groupentry entry(the_comm.get_group(), wall->stop);
	//std::cerr << "  Insert group " << prm->group << "\n";
	self->groups_.insert(std::make_pair(prm->group, entry));
      }
    return 1;
  }

  int trace::handle_comm_free(const dumpi_comm_free *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    trace *self = (trace*)userarg;
    //std::cerr << "DEBUG:  trace::handle_comm_free\n";
    commentry &the_comm = self->get_commentry(prm->comm);
    the_comm.freed = wall->stop;
    return 1;
  }

  int trace::handle_group_union(const dumpi_group_union *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    assert(prm != NULL && userarg != NULL);
    //std::cerr << "DEBUG:  trace::handle_group_union(group1=" << prm->group1
    //	    << ", group2=" << prm->group2 << ", " << ", newgroup=" 
    //	    << prm->newgroup << ")\n";
    trace *self = (trace*)userarg;
    // Group union grabs all members from the first group (in order)
    /// and adds "new" members from the second group (again in order).
    const group &group1 = self->get_group(prm->group1);
    const group &group2 = self->get_group(prm->group2);
    // Now, it is possible that the current node is a member of only one
    // (or even neither) group.
    std::vector<int> members = group1.get_peers();
    const std::vector<int> &v2 = group2.get_peers();
    std::set<int> seen(members.begin(), members.end());
    for(std::vector<int>::const_iterator it = v2.begin(); it != v2.end(); ++it) {
      if(seen.find(*it) == seen.end()) {
	seen.insert(*it);
	members.push_back(*it);
      }
    }
    // Find the local rank of current node in the peer list.
    int global1 = group1.get_global_rank();
    int global2 = group2.get_global_rank();
    int global_rank = (global1 >= 0 ? global1 :
		       (global2 >= 0 ? global2 : 0));
    int local_rank = -1;
    if(global_rank >= 0) {
      for(size_t i = 0; i < members.size(); ++i) {
	if(members[i] == global_rank) {
	  local_rank = int(i);
	  break;
	}
      }
    }
    if(local_rank >= 0 && size_t(local_rank) >= members.size())
      throw "trace::handle_group_union:  Mangled output group.";
    // Create the new group.
    group gg(local_rank, members);
    groupentry entry(gg, wall->stop);
    //std::cerr << "  Insert group " << prm->newgroup << "\n";
    self->groups_.insert(std::make_pair(prm->newgroup, entry));  
    // All done.
    return 1;
  }

  int trace::handle_group_intersection(const dumpi_group_intersection *prm,
				       uint16_t thread, const dumpi_time *cpu,
				       const dumpi_time *wall,
				       const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    assert(prm != NULL && userarg != NULL);
    //std::cerr << "DEBUG:  trace::handle_group_intersection(group1=" <<prm->group1
    //	    << ", group2=" << prm->group2 << ", " << ", newgroup=" 
    //	    << prm->newgroup << ")\n";
    trace *self = (trace*)userarg;
    // Group intersection grabs members from first group that are also
    // seen in the second group.  Member order is preserved from first group.
    const group &group1 = self->get_group(prm->group1);
    const group &group2 = self->get_group(prm->group2);
    const std::vector<int> v1 = group1.get_peers();
    const std::vector<int> &v2 = group2.get_peers();
    std::set<int> seen(v2.begin(), v2.end());
    std::vector<int> members;
    for(std::vector<int>::const_iterator it = v1.begin(); it != v1.end(); ++it) {
      if(seen.find(*it) != seen.end()) {
	members.push_back(*it);
      }
    }
    // Find the local rank of current node in the peer list.
    int global1 = group1.get_global_rank();
    int global2 = group2.get_global_rank();
    int global_rank = (global1 >= 0 ? global1 :
		       (global2 >= 0 ? global2 : 0));
    int local_rank = -1;
    if(global_rank >= 0) {
      for(size_t i = 0; i < members.size(); ++i) {
	if(members[i] == global_rank) {
	  local_rank = int(i);
	  break;
	}
      }
    }
    // Create the new group.
    group gg(local_rank, members);
    groupentry entry(gg, wall->stop);
    //std::cerr << "  Insert group " << prm->newgroup << "\n";
    self->groups_.insert(std::make_pair(prm->newgroup, entry));  
    // All done.
    return 1;
  }

  int trace::handle_group_difference(const dumpi_group_difference *prm,
				     uint16_t thread, const dumpi_time *cpu,
				     const dumpi_time *wall,
				     const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    assert(prm != NULL && userarg != NULL);
    //std::cerr << "DEBUG:  trace::handle_group_difference(group1=" << prm->group1
    //	    << ", group2=" << prm->group2 << ", " << ", newgroup=" 
    //	    << prm->newgroup << ")\n";
    trace *self = (trace*)userarg;
    // Group difference grabs members from first group that are not
    // seen in the second group.  Member order is preserved from first group.
    const group &group1 = self->get_group(prm->group1);
    const group &group2 = self->get_group(prm->group2);
    const std::vector<int> v1 = group1.get_peers();
    const std::vector<int> &v2 = group2.get_peers();
    std::set<int> seen(v2.begin(), v2.end());
    std::vector<int> members;
    for(std::vector<int>::const_iterator it = v1.begin(); it != v1.end(); ++it) {
      if(seen.find(*it) == seen.end()) {
	members.push_back(*it);
      }
    }
    // Find the local rank of current node in the peer list.
    int global1 = group1.get_global_rank();
    int global2 = group2.get_global_rank();
    int global_rank = (global1 >= 0 ? global1 :
		       (global2 >= 0 ? global2 : 0));
    int local_rank = -1;
    if(global_rank >= 0) {
      for(size_t i = 0; i < members.size(); ++i) {
	if(members[i] == global_rank) {
	  local_rank = int(i);
	  break;
	}
      }
    }
    // Create the new group.
    group gg(local_rank, members);
    groupentry entry(gg, wall->stop);
    //std::cerr << "  Insert group " << prm->newgroup << "\n";
    self->groups_.insert(std::make_pair(prm->newgroup, entry));  
    // All done.
    return 1;
  }

  // MPI_Group_incl includes the specified members of an input group
  // in the order provided.
  int trace::handle_group_incl(const dumpi_group_incl *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    assert(prm != NULL && userarg != NULL);
    //std::cerr << "DEBUG:  trace::handle_group_incl(group=" << prm->group
    //	    << ", newgroup=" << prm->newgroup << ")\n";
    trace *self = (trace*)userarg;
    const group &ingroup = self->get_group(prm->group);
    const std::vector<int> &inmembers = ingroup.get_peers();
    std::vector<int> outmembers((prm->count > 0 ? prm->count : 0));
    for(int i = 0; i < prm->count; ++i) {
      outmembers.at(i) = inmembers.at(prm->ranks[i]);
    }
    // Find the local rank of current node.
    int global_rank = ingroup.get_global_rank();
    int local_rank = -1;
    if(global_rank >= 0) {
      for(size_t i = 0; i < outmembers.size(); ++i) {
	if(outmembers[i] == global_rank) {
	  local_rank = int(i);
	  break;
	}
      }
    }
    // Create the new group.
    group gg(local_rank, outmembers);
    groupentry entry(gg, wall->stop);
    //std::cerr << "  Insert group " << prm->newgroup << "\n";
    self->groups_.insert(std::make_pair(prm->newgroup, entry));  
    // All done.
    return 1;
  }

  // MPI_Group_excl includes all members of the input group
  // except the given ranks.  Remaining ranks retain the relative ordering
  // that they had in the input group.
  int trace::handle_group_excl(const dumpi_group_excl *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    assert(prm != NULL && userarg != NULL);
    //std::cerr << "DEBUG:  trace::handle_group_excl(group=" << prm->group
    //	    << ", newgroup=" << prm->newgroup << ")\n";
    trace *self = (trace*)userarg;
    const group &ingroup = self->get_group(prm->group);
    const std::vector<int> &inmembers = ingroup.get_peers();
    std::set<int> skip(prm->ranks, prm->ranks + prm->count);
    std::vector<int> outmembers;
    for(size_t i = 0; i < inmembers.size(); ++i) {
      if(skip.find(i) != skip.end()) {
	outmembers.push_back(inmembers[i]);
      }
    }
    // Find the local rank of current node.
    int global_rank = ingroup.get_global_rank();
    int local_rank = -1;
    if(global_rank >= 0) {
      for(size_t i = 0; i < outmembers.size(); ++i) {
	if(outmembers[i] == global_rank) {
	  local_rank = int(i);
	  break;
	}
      }
    }
    // Create the new group.
    group gg(local_rank, outmembers);
    groupentry entry(gg, wall->stop);
    //std::cerr << "  Insert group " << prm->newgroup << "\n";
    self->groups_.insert(std::make_pair(prm->newgroup, entry));  
    // All done.
    return 1;
  }

  int trace::handle_group_range_incl(const dumpi_group_range_incl *prm,
				     uint16_t thread, const dumpi_time *cpu,
				     const dumpi_time *wall,
				     const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf;
    assert(prm != NULL && userarg != NULL);
    //std::cerr << "DEBUG:  trace::handle_group_range_incl(group=" << prm->group
    //	    << ", newgroup=" << prm->newgroup << ")\n";
    // Lazy -- just compute the 'include' array and call handle_group_incl
    // (we could equivalently do this the other way around).
    std::set<int> seen;
    std::vector<int> incl;
    for(int i = 0; i < prm->count; ++i) {
      int *range = prm->ranges[i];
      //std::cerr << "  range = {" << range[0] << ", " << range[1]
      //	      << ", " << range[2] << "}\n";
      if(range[2] > 0) {
	// Counting up.
	for(int i = range[0]; i <= range[1]; i += range[2]) {
	  if(seen.find(i) == seen.end()) {
	    seen.insert(i);
	    incl.push_back(i);
	  }
	}
      }
      else if(range[2] < 0) {
	// Counting down.
	for(int i = range[0]; i >= range[1]; i -= range[2]) {
	  if(seen.find(i) == seen.end()) {
	    seen.insert(i);
	    incl.push_back(i);
	  }	
	}
      }
      else {
	throw "INVALID:  trace::handle_group_range_incl:  Zero stride.";
      }
    }
    // Now just make our lazy call.
    // For now, rely on std::vector being implemented as a raw array.
    dumpi_group_incl dgi = { 
      prm->group,
      int(incl.size()),
      (incl.size() > 0 ? &incl[0] : NULL),
      prm->newgroup
    };
    handle_group_incl(&dgi, thread, cpu, wall, perf, userarg);
    return 1;
  }

  int trace::handle_group_range_excl(const dumpi_group_range_excl *prm,
				     uint16_t thread, const dumpi_time *cpu,
				     const dumpi_time *wall,
				     const dumpi_perfinfo *perf, void *userarg)
  {
    (void)thread; (void)cpu; (void)perf; (void)userarg;
    assert(prm != NULL && userarg != NULL);
    //std::cerr << "DEBUG:  trace::handle_group_range_excl(group=" << prm->group
    //	    << ", newgroup=" << prm->newgroup << ")\n";
    // Lazy -- just compute the 'exclude' array and call handle_group_excl
    // (we could equivalently do this the other way around).
    std::set<int> seen;
    std::vector<int> excl;
    for(int i = 0; i < prm->count; ++i) {
      int *range = prm->ranges[i];
      if(range[2] > 0) {
	// Counting up.
	for(int i = range[0]; i <= range[1]; i += range[2]) {
	  if(seen.find(i) == seen.end()) {
	    seen.insert(i);
	    excl.push_back(i);
	  }
	}
      }
      else if(range[2] < 0) {
	// Counting down.
	for(int i = range[0]; i >= range[1]; i -= range[2]) {
	  if(seen.find(i) == seen.end()) {
	    seen.insert(i);
	    excl.push_back(i);
	  }	
	}
      }
      else {
	throw "INVALID:  trace::handle_group_range_excl:  Zero stride.";
      }
    }
    // Now just make our lazy call.
    // For now, rely on std::vector being implemented as a raw array.
    dumpi_group_excl dgi = { 
      prm->group,
      int(excl.size()), //explicit cast for c++11
      (excl.size() > 0 ? &excl[0] : NULL),
      prm->newgroup
    };
    handle_group_excl(&dgi, thread, cpu, wall, perf, userarg);
    return 1;
  }

  int trace::handle_group_free(const dumpi_group_free *prm, uint16_t thread,
			       const dumpi_time *cpu, const dumpi_time *wall,
			       const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    trace *self = (trace*)userarg;
    //std::cerr << "DEBUG:  trace::handle_group_free\n";
    groupentry &the_group = self->get_groupentry(prm->group);
    the_group.freed = wall->stop;
    return 1;
  }

  int trace::handle_type_contiguous(const dumpi_type_contiguous *prm,
				    uint16_t thread, const dumpi_time *cpu,
				    const dumpi_time *wall,
				    const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_contiguous\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int newsize = prm->count * oldtype.get_size();
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_vector(const dumpi_type_vector *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_vector\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int newsize = prm->count * prm->blocklength * oldtype.get_size();
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_hvector(const dumpi_type_hvector *prm, uint16_t thread,
				 const dumpi_time *cpu, const dumpi_time *wall,
				 const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_hvector\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int newsize = prm->count * prm->blocklength * oldtype.get_size();
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_indexed(const dumpi_type_indexed *prm, uint16_t thread,
				 const dumpi_time *cpu, const dumpi_time *wall,
				 const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_indexed\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int entries = 0;
    for(int i = 0; i < prm->count; ++i) entries += prm->lengths[i];
    int newsize = entries * oldtype.get_size();
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_hindexed(const dumpi_type_hindexed *prm, uint16_t thread,
				  const dumpi_time *cpu, const dumpi_time *wall,
				  const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_hindexed\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int entries = 0;
    for(int i = 0; i < prm->count; ++i) entries += prm->lengths[i];
    int newsize = entries * oldtype.get_size();
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_struct(const dumpi_type_struct *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_struct\n";
    trace *self = (trace*)userarg;
    int newsize = 0;
    for(int i = 0; i < prm->count; ++i) {
      const type &oldtype = self->get_type(prm->oldtypes[i]);
      newsize += prm->lengths[i] * oldtype.get_size();
    }
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_dup(const dumpi_type_dup *prm, uint16_t thread,
			     const dumpi_time *cpu, const dumpi_time *wall,
			     const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_dup\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    typeentry tentry(type(oldtype.get_size()), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_create_darray(const dumpi_type_create_darray *prm,
				       uint16_t thread, const dumpi_time *cpu,
				       const dumpi_time *wall,
				       const dumpi_perfinfo *perf, void *userarg)
  {
    // MPI_Type_create_darray is slightly insane -- I am making a best-effort
    // guess at the data layout based on the MPI standard documentation.
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_create_darray\n";
    trace *self = (trace*)userarg;
    // gsizes defines the number of data elements along each principal axis
    // distribs defines the distribution (dumpi_native_distributions) in each dim.
    // dargs defines the layout block (we consider any value <0 to be 'default')
    // psizes defines the processor grid layout.
    // First figure out where the current 'rank' fits in the grid.
    // The MPI standard specifies that the grid is laid out in row-major order.
    std::vector<int> gridpos(prm->size);
    int scaledown = 1;
    for(int dim = prm->ndims-1; dim >= 0; --dim) {
      int remain = prm->rank / scaledown;
      gridpos.at(dim) = remain % prm->psizes[dim];
      scaledown *= prm->psizes[dim];
    }
    // As outlined in the MPI manual, all distributions are variants
    // on MPI_DISTRIBUTE_CYCLIC, so we only have to tweak the 'dargs' array.
    // Default behavior is to do CYCLIC with block size 1.
    std::vector<int> blocks(prm->size, 1);
    for(int dim = 0; dim < prm->ndims; ++dim) {
      switch(prm->distribs[dim]) {
      case DUMPI_DISTRIBUTE_BLOCK:
	// Distribute block is identical to distribute cyclic except when
	// dargs <= 0.
	if(prm->dargs[dim] <= 0)
	  blocks.at(dim) = ((prm->gsizes[dim] + prm->psizes[dim] - 1)
			    / prm->psizes[dim]);
	break;
      case DUMPI_DISTRIBUTE_NONE:
	// Distribute none does not distribute data.
	blocks.at(dim) = prm->gsizes[dim];
	break;
      case DUMPI_DISTRIBUTE_CYCLIC:
	// Default distribution is a block size of 1.
	if(prm->dargs[dim] <= 0) blocks.at(dim) = 1;
	break;
      default:
	// Uknown distribution.  Raise hell.
	throw "trace::handle_type_create_darray:  Invalid distribution.";
      }
    }
    // Compute the data chunk (along each dimension) at the current grid point.
    std::vector<int> chunk(prm->size, 0);
    for(int dim = 0; dim < prm->ndims; ++dim) {
      // Deliberately allowing truncated integer division --
      // this is the base-number of blocks of size blocks.at(gdim)
      // that will be placed on each grid element.
      int uniform = prm->gsizes[dim] / (prm->psizes[dim] * blocks.at(dim));
      // Number of elements not accounted for (0 <= remain < prm->psizes[dim]).
      int remain = prm->gsizes[dim] - uniform*prm->psizes[dim]*blocks.at(dim);
      // Decide whether this node picks up extra elements or not.
      int extra = 0;
      if((gridpos.at(dim)+1)*blocks.at(dim) < remain) {
	// We can fit an entire block (chunk) here
	extra = blocks.at(dim);
      }
      else if(gridpos.at(dim)*blocks.at(dim) <= remain) {
	// We can fit a partial block here.
	extra = remain - gridpos.at(dim) * blocks.at(dim);
      }
      else {
	// No deal
	extra = 0;
      }
      // Finally, we compute how much data we hold along this axis.
      chunk.at(dim) = uniform * blocks.at(dim) + extra;
    }
    // Now we can compute the size of this datatype.
    const type &oldtype = self->get_type(prm->oldtype);
    int size = oldtype.get_size();
    for(size_t i = 0; i < chunk.size(); ++i)
      size *= chunk.at(i);
    // Create the new type.
    typeentry tentry(type(size), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    // Done.
    return 1;
  }

  int trace::handle_type_create_hindexed(const dumpi_type_create_hindexed *prm,
					 uint16_t thread, const dumpi_time *cpu,
					 const dumpi_time *wall,
					 const dumpi_perfinfo *perf,
					 void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_create_hindexed\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int entries = 0;
    for(int i = 0; i < prm->count; ++i) entries += prm->blocklengths[i];
    int newsize = entries * oldtype.get_size();
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_create_hvector(const dumpi_type_create_hvector *prm,
					uint16_t thread, const dumpi_time *cpu,
					const dumpi_time *wall,
					const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_create_hvector\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int newsize = prm->count * prm->blocklength * oldtype.get_size();
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_create_indexed_block
  (const dumpi_type_create_indexed_block *prm, uint16_t thread,
   const dumpi_time *cpu, const dumpi_time *wall,
   const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_create_indexed_block\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int newsize = prm->count * prm->blocklength * oldtype.get_size();
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_create_resized(const dumpi_type_create_resized *prm,
					uint16_t thread, const dumpi_time *cpu,
					const dumpi_time *wall,
					const dumpi_perfinfo *perf, void *userarg)
  {
    // Create a type with the same size but different extent than input type.
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_create_resized\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    typeentry tentry(type(oldtype.get_size()), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_create_struct(const dumpi_type_create_struct *prm,
				       uint16_t thread, const dumpi_time *cpu,
				       const dumpi_time *wall,
				       const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_create_struct\n";
    trace *self = (trace*)userarg;
    int newsize = 0;
    for(int i = 0; i < prm->count; ++i) {
      const type &oldtype = self->get_type(prm->oldtypes[i]);
      newsize += prm->blocklengths[i] * oldtype.get_size();
    }
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_create_subarray(const dumpi_type_create_subarray *prm,
					 uint16_t thread, const dumpi_time *cpu,
					 const dumpi_time *wall,
					 const dumpi_perfinfo *perf,
					 void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    //std::cerr << "trace::handle_type_create_subarray\n";
    trace *self = (trace*)userarg;
    const type &oldtype = self->get_type(prm->oldtype);
    int newsize = oldtype.get_size();
    for(int i = 0; i < prm->ndims; ++i) {
      newsize *= prm->subsizes[i];
    }
    typeentry tentry(type(newsize), wall->stop);
    self->types_.insert(std::make_pair(prm->newtype, tentry));
    return 1;
  }

  int trace::handle_type_commit(const dumpi_type_commit *prm, uint16_t thread,
				const dumpi_time *cpu, const dumpi_time *wall,
				const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    trace *self = (trace*)userarg;
    //std::cerr << "DEBUG:  trace::handle_type_commit\n";
    typeentry &the_type = self->get_typeentry(prm->datatype);
    the_type.committed = wall->stop;
    return 1;
  }

  int trace::handle_type_free(const dumpi_type_free *prm, uint16_t thread,
			      const dumpi_time *cpu, const dumpi_time *wall,
			      const dumpi_perfinfo *perf, void *userarg)
  {
    (void)prm; (void)thread; (void)cpu; (void)wall, (void)perf; (void)userarg;
    trace *self = (trace*)userarg;
    //std::cerr << "DEBUG:  trace::handle_type_free\n";
    typeentry &the_type = self->get_typeentry(prm->datatype);
    the_type.freed = wall->stop;
    return 1;
  }

} // end of namespace dumpi