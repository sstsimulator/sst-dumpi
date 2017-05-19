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

#include <dumpi/bin/sharedstate-commconstruct.h>
#include <iostream>

namespace dumpi {

  sharedstate::commconstruct::commconstruct(int size) :
    target_size_(size)
  {}

  sharedstate::commconstruct::~commconstruct() {
  }

  void sharedstate::commconstruct::mark_fetched(int rank) {
    fetched_.insert(rank);
  }

  sharedstate::commdup::commdup(commid new_id, int size) :
    commconstruct(size), id_(new_id)
  {}

  sharedstate::commdup::~commdup() {
  }

  void sharedstate::commdup::add(const comm &incomm) {
    if(result_.empty()) {
      // First in :)
      if(incomm.get_id() < 0 || incomm.get_group().get_size() < 1)
	throw "sharedstate::commdup::add:  will not dup a null communicator.";
      templateid_ = incomm.get_id();
    }
    // Check that we have a conforming communicator.
    if(incomm.get_id() != templateid_ ||
       target_size() != incomm.get_group().get_size())
      throw "sharedstate::commdup::add:  wrong communicator index or size.";
    int rr = incomm.get_group().get_global_rank();
    if(result_.find(rr) != result_.end())
      throw "sharedstate::commdup::add:  Attempt to redefine existing rank.";
    result_[rr] = comm(id_, incomm.get_group());
  }

  bool sharedstate::commdup::is_complete() const {
    return result_.size() == size_t(target_size());
  }

  size_t sharedstate::commdup::size() const {
    return result_.size();
  }

  const comm& sharedstate::commdup::get_comm(int global_rank) {
    result_t::iterator it = result_.find(global_rank);
    if(! this->is_complete())
      throw "sharedstate::commconstruct::get_comm:  Not complete.";
    if(it == result_.end())
      throw "sharedstate::commconstruct::get_comm:  Invalid rank.";
    this->mark_fetched(global_rank);
    return it->second;
  }

  sharedstate::commconstruct* sharedstate::commdup::clone() const {
    return new commdup(*this);
  }

  sharedstate::commcreate::commcreate(commid new_id, int size) :
    commconstruct(size), id_(new_id)
  {}

  sharedstate::commcreate::~commcreate() {
  }

  void sharedstate::commcreate::add(const comm &incomm, const group &membership)
  {
    if(result_.empty()) {
      // First in :)
      templateid_ = incomm.get_id();
      // Stash the node indices.
      nodes_.clear();
      const std::vector<int> &nn = membership.get_peers();
      nodes_.insert(nn.begin(), nn.end());
      if(nn.size() != nodes_.size())
	throw "sharedstate::commcreate::add:  Group contains duplicates.";
    }
    // Check that we have a conforming communicator.
    if(incomm.get_id() != templateid_)
      throw "sharedstate::commdup::add:  wrong communicator index.";
    int rr = incomm.get_group().get_global_rank();
    const std::vector<int> &nn = membership.get_peers();
    if(nn.size() != nodes_.size())
      throw "sharedstate::commcreate::add:  Wrong input group size.";
    // here we might want to make sure that the input groups match (maybe).
    if(result_.find(rr) != result_.end())
      throw "sharedstate::commcreate::add:  Attempt to redefine rank.";
    int global_rank = incomm.get_group().get_global_rank();
    int local_rank = -1;
    for(size_t id = 0; id < nn.size(); ++id) {
      if(nn[id] == global_rank) {
	local_rank = id;
	break;
      }
    }
    // add.
    if(local_rank >= 0)
      result_[rr] = comm(id_, group(local_rank, nn));
    else
      result_[rr] = comm();
  }

  bool sharedstate::commcreate::is_complete() const {
    return result_.size() == size_t(target_size());
  }

  size_t sharedstate::commcreate::size() const {
    return result_.size();
  }

  const comm& sharedstate::commcreate::get_comm(int global_rank) {
    result_t::iterator it = result_.find(global_rank);
    if(! this->is_complete())
      throw "sharedstate::commcreate::get_comm:  Not complete.";
    if(it == result_.end())
      throw "sharedstate::commcreate::get_comm:  Invalid rank.";
    this->mark_fetched(global_rank);
    return it->second;
  }

  sharedstate::commconstruct* sharedstate::commcreate::clone() const {
    return new commcreate(*this);
  }

  sharedstate::commsplit::commsplit(commid first_id, commid not_to_reach,
				    int size) :
    commconstruct(size), begin_(first_id), end_(not_to_reach), entered_(0)
  {}

  sharedstate::commsplit::~commsplit() {
  }

  void sharedstate::commsplit::add(const comm &incomm, int color, int key)
  {
    if(entered_ <= 0) {
      // First in.
      if(incomm.get_id() < 0 || incomm.get_group().get_size() != target_size())
	throw "sharedstate::commsplit::add:  Invalid input communicator.";
      templateid_ = incomm.get_id();
    }
    // All inbound communicators go through here.
    entered_++;
    //std::cerr << "commsplit::add:  Now have " << entered_ << " of "
    //	    << target_size() << " members in splitter\n";
    if(entered_ > target_size())
      throw "sharedstate::commsplit::add:  Over-full splitter.";
    if(incomm.get_id() != templateid_)
      throw "sharedstate::commsplit::add:  Wrong communicator id.";
    int rank = incomm.get_group().get_global_rank();
    if(rank < 0)
      throw "sharedstate::commsplit::add:  Bogus communicator (negative rank)";
    // Insert this entrant.
    color_group_[color].insert(std::make_pair(key, rank));
    // We may be done.
    if(entered_ >= target_size()) {
      //std::cerr << "commsplit::add:  Splitter fully populated\n";
      if(color_group_.size() > size_t(end_ - begin_))
	throw "sharedstate::commsplit:  Overflowed allowed communicator ids.";
      color_group_t::iterator it;
      commid currid = begin_;
      for(it = color_group_.begin(); it != color_group_.end(); ++it, ++currid) {
	if(it->first >= 0) {
	  // Non-negative color is an "actual" group.
	  // Figure out who is in this group.
	  std::vector<int> peers;
	  peers.reserve(it->second.size());
	  key_to_global_rank_t::iterator peer;
	  for(peer = it->second.begin(); peer != it->second.end(); ++peer) {
	    peers.push_back(peer->second);
	  }
	  // All nodes that are in this peer list now get a corresponding comm.
	  for(size_t local = 0; local < peers.size(); ++local) {
	    int global = peers.at(local);
	    group the_group(local, peers);
	    result_[global] = comm(currid, the_group);
	  }
	}
	else {
	  // Negative color is a "leave me out of this" group.
	  key_to_global_rank_t::iterator peer;
	  for(peer = it->second.begin(); peer != it->second.end(); ++peer) {
	    int global = peer->second;
	    result_[global] = comm();
	  }
	}
      }
    }
  }

  bool sharedstate::commsplit::is_complete() const {
    return entered_ == target_size();
  }

  size_t sharedstate::commsplit::size() const {
    return entered_;
  }

  const comm& sharedstate::commsplit::get_comm(int global_rank) {
    result_t::iterator it = result_.find(global_rank);
    if(! this->is_complete())
      throw "sharedstate::commcreate::get_comm:  Not complete.";
    if(it == result_.end())
      throw "sharedstate::commcreate::get_comm:  Invalid rank.";
    this->mark_fetched(global_rank);
    return it->second;
  }

  sharedstate::commconstruct* sharedstate::commsplit::clone() const {
    return new commsplit(*this);
  }

} // end of namespace dumpi