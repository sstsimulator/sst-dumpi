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

#include <dumpi/bin/sharedstate-commconstruct.h>
#include <iostream>

namespace dumpi {

  //
  // Create a new shared state. Constructs world.
  //
  sharedstate::sharedstate(int size) :
    nextid_(3), ranks_(size), nexthandle_(1)
  {
    for(int i = 0; i < size; ++i)
      ranks_[i] = i;
  }

  //
  // Get the world communicator for the given rank.
  //
  comm sharedstate::retrieve_world(int rank) {
    return comm(0, group(rank, ranks_));
  }

  //
  // Get a 'self' communicator for the given rank.
  //
  comm sharedstate::retrieve_self(int rank) {
    return comm(1, group(0, std::vector<int>(1, rank)));
  }

  //
  // Get the newly constructed communicator corresponding to the given handle.
  //
  comm sharedstate::retrieve_comm(commhandle handle) {
    //std::cerr << "DEBUG:  sharestate::retrieve_comm(" << handle << ")\n";
    handle_t::const_iterator it = handles_.find(handle);
    if(it == handles_.end())
      throw "sharedstate::retrieve_comm:  Invalid handle.";
    if(! it->second.ptr->is_complete())
      throw "sharedstate::retrieve_comm:  Communicator is not complete.";
    comm result = it->second.ptr->get_comm(it->second.rank);
    if(it->second.ptr->is_fully_fetched()) {
      // Currently leaking this object -- it's pretty harmless
      // in terms of memory overhead.
      //std::cerr << "DEBUG:  sharedstate::retrieve_comm:  Communicator is "
      //      << "fully fetched.\n";
    }
    //std::cerr << "  Fetched communicator of size "
    //	    << result.get_group().get_size() << "\n";
    return result;
  }

  //
  // Test whether the given communicator is complete.
  //
  bool sharedstate::is_complete(commhandle handle) const {
    handle_t::const_iterator it = handles_.find(handle);
    if(it == handles_.end())
      throw "sharedstate::is_complete:  Invalid handle";
    return it->second.ptr->is_complete();
  }

  //
  // Duplicate an existing communicator (MPI_Comm_dup).
  // Returns a handle to retrieve the completed communicator.
  //
  commhandle sharedstate::comm_dup(const comm &incomm) {
    //std::cerr << "DEBUG:  sharedstate::comm_dup(commid=" << incomm.get_id()
    //	    << "; rank=" << incomm.get_group().get_global_rank() << ")\n";
    commid id = incomm.get_id();
    pending_t::iterator it = pending_.find(id);
    if(it == pending_.end()) {
      pending_[id] = new commdup(nextid_++, incomm.get_group().get_size());
    }
    commconstruct *raw = pending_[id];
    commdup* dup = dynamic_cast<commdup*>(raw);
    if(! dup) {
      throw "sharedstate::comm_dup:  Incorrect commconstruct type.";
    }
    commhandle the_handle = nexthandle_++;
    handles_[the_handle] = entrant(incomm.get_group().get_global_rank(), dup);
    dup->add(incomm);
    // Test whether we're done making this comm.
    if(dup->is_complete()) {
      pending_.erase(id);
    }
    return the_handle;
  }

  //
  // Create a communicator (MPI_Comm_create) from the given 
  // input communicator.  Returns a handle that can be used to
  // retrieve the completed communicator once it is complete.
  //
  commhandle sharedstate::comm_create(const comm &incomm, const group &membership)
  {
    //std::cerr << "DEBUG:  sharedstate::comm_create(commid=" << incomm.get_id()
    //	    << "; rank=" << incomm.get_group().get_global_rank()
    //	    << "; members=" << membership.get_size() << ")\n";
    commid id = incomm.get_id();
    pending_t::iterator it = pending_.find(id);
    if(it == pending_.end()) {
      pending_[id] = new commcreate(nextid_++, incomm.get_group().get_size());
    }
    commconstruct *raw = pending_[id];
    commcreate* crt = dynamic_cast<commcreate*>(raw);
    if(! crt) {
      throw "sharedstate::comm_create:  Incorrect commconstruct type.";
    }
    commhandle the_handle = nexthandle_++;
    handles_[the_handle] = entrant(incomm.get_group().get_global_rank(), crt);
    crt->add(incomm, membership);
    // Test whether anybody else needs to join this comm.
    if(crt->is_complete()) {
      pending_.erase(id);
    }
    return the_handle;
  }

  //
  // Create a group of communicators (MPI_Comm_split) at the same time.
  // Returns a handle to retrieve the completed communicator.
  //
  commhandle sharedstate::comm_split(const comm &incomm, int color, int key) {
    //std::cerr << "DEBUG:  sharedstate::comm_split(commid=" << incomm.get_id()
    //	    << ", color=" << color << ", key=" << key << ")\n";
    commid id = incomm.get_id();
    pending_t::iterator it = pending_.find(id);
    if(it == pending_.end()) {
      commid beginid = nextid_;
      nextid_ += incomm.get_group().get_size();
      commid endid = nextid_;
      pending_[id] =
        new commsplit(beginid, endid, incomm.get_group().get_size());
    }
    commconstruct *raw = pending_[id];
    commsplit* crt = dynamic_cast<commsplit*>(raw);
    if(! crt) {
      throw "sharedstate::comm_split:  Incorrect commconstruct type.";
    }
    commhandle the_handle = nexthandle_++;
    handles_[the_handle] = entrant(incomm.get_group().get_global_rank(), crt);
    crt->add(incomm, color, key);
    // Test whether anybody else needs to join this comm.
    if(crt->is_complete()) {
      pending_.erase(id);
    }
    return the_handle;
  }

} // end of namespace dumpi
