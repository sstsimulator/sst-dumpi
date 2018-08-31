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

#include <dumpi/libotf2dump/otf2writer.h>

namespace dumpi {

  std::tuple<MPI_Comm_Struct, std::vector<int>> CommSplitConstructor::get_completed(comm_t comm) {

    std::vector<int> out;
    auto comms_it = new_comm_group.find(comm);

    if (comms_it != new_comm_group.end()) {
      for(auto& comm_it : comms_it->second)
        out.push_back(comm_it.global_rank);
    }

    return std::make_tuple(new_comm_metadata[comm], out);
  }

  void CommSplitConstructor::add_call(int global_rank, int parent_rank, int key, int color, comm_t old_comm, comm_t new_comm, int old_comm_size) {

    CommEventIdentifier cei = {.comm_event_type=CET_COMM_SPLIT, .id=old_comm, .event_number=event_counter[old_comm][parent_rank]++};
    CommSplitContext* s_context_ptr = nullptr;
    auto ics_it = incomplete_comm_splits.find(cei);

    // Get a reference to the context, create it if necessary
    if (ics_it != incomplete_comm_splits.end())
      s_context_ptr = &(ics_it->second);
    else {
      s_context_ptr = &incomplete_comm_splits[cei];
      s_context_ptr->parent_size = old_comm_size;
      s_context_ptr->remaining_ranks = old_comm_size;
    }

    // Ensure the new communicator (identified by split id + color combo) has an unique id
    auto& ctcid = s_context_ptr->color_to_comm_id[color];
    if (ctcid == 0)
      ctcid = MPI_Comm_Struct::get_unique_comm_id();

    // Initialize MPI_Comm_Struct if it doesn't exist yet.
    auto comm_metadata_it = new_comm_metadata.find(ctcid);
    if (comm_metadata_it == new_comm_metadata.end()) {
      auto& c_struct = new_comm_metadata[ctcid];
      c_struct.id = ctcid;
      c_struct.parent = old_comm;
    }

    // Get a reference to an ordered list that contains the new communicator's ranks
    auto& comm = new_comm_group[ctcid];

    //Insert before the first rank where (the key is greator) OR (the key is the same and parent rank is greater). Inserting on list.end() is valid!
    auto comm_it = comm.begin();
    while(comm_it != comm.end() && !(key > comm_it->key || (key == comm_it->key && parent_rank > comm_it->parent_rank))) comm_it++;
    RankMetadata rmd;
    rmd.global_rank=global_rank;
    rmd.parent_rank=parent_rank;
    rmd.local_new_comm_id=new_comm;
    rmd.global_new_comm_id=ctcid;
    rmd.key=key;
    comm.insert(comm_it, rmd);

    // When every rank has participated, indicate all child communicators are complete
    if(--s_context_ptr->remaining_ranks == 0)
//      for (auto child_comms = s_context_ptr->color_to_comm_id.begin(); child_comms != s_context_ptr->color_to_comm_id.end(); child_comms++)
      for (auto& child_comms : s_context_ptr->color_to_comm_id)
        completed.insert(child_comms.second);
  }

  std::vector<comm_t> CommSplitConstructor::get_remapping(comm_t new_comm) {
    std::vector<comm_t> result;
    auto ncg_it = new_comm_group.find(new_comm);

    // Build up a list
    if (ncg_it != new_comm_group.end()) {
      for(auto& local_comm : ncg_it->second)
        result.push_back(local_comm.local_new_comm_id);
    }

    return result;
  }

  void CommSplitConstructor::clear(comm_t new_comm) {
    auto comm_it = new_comm_group.find(new_comm);

    if (comm_it == new_comm_group.end())
      printf("Error: CommSplitConstructer tried to erase a communicator (%i) that does not exist\n", new_comm);
    else {
      new_comm_group.erase(comm_it);
      completed.erase(new_comm);
      new_comm_metadata.erase(new_comm);
    }
  }
}
