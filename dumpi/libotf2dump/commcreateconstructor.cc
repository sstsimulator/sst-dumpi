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
  std::tuple<MPI_Comm_Struct, std::vector<int>> CommCreateConstructor::get_completed(comm_t comm) {
    std::vector<int> out;
    auto comms_it = new_comm_group.find(comm);

    if (comms_it != new_comm_group.end()) {
      auto& comm = comms_it->second;
      for(auto& _comm : comm)
        out.push_back(_comm.global_rank);
    }

    return std::make_tuple(new_comm_metadata[comm], out);
  }

  void CommCreateConstructor::add_call(int global_rank, int parent_rank, comm_t comm, int group_size, unsigned long group_hash, comm_t new_comm) {
    CommCreateIdentifier cci = {.id=comm, .event_number=event_counter[comm][group_hash][parent_rank]++, .group_hash=group_hash};
    CommCreateContext* c_ctx_ptr = nullptr;
    auto icc_it = incomplete_comm_creates.find(cci);

    // Get a reference to the context, create if necessary
    if (icc_it != incomplete_comm_creates.end())
      c_ctx_ptr = &(icc_it->second);
    else {
      c_ctx_ptr = &incomplete_comm_creates[cci];
      c_ctx_ptr->remaining_ranks = group_size;
      c_ctx_ptr->global_id = MPI_Comm_Struct::get_unique_comm_id();

      auto& c_struct = new_comm_metadata[c_ctx_ptr->global_id];
      c_struct.id = c_ctx_ptr->global_id;
      c_struct.parent = comm;
    }

    RankMetadata rmd;
    rmd.global_rank = global_rank;
    rmd.local_comm_id = new_comm;
    new_comm_group[c_ctx_ptr->global_id].push_back(rmd);

    if (--c_ctx_ptr->remaining_ranks == 0)
      completed.insert(c_ctx_ptr->global_id);
  }

  void CommCreateConstructor::clear(comm_t new_comm) {
    auto comm_it1 = new_comm_group.find(new_comm);

    if (comm_it1 == new_comm_group.end()) {
      printf("Error: CommCreateConstructer tried to erase a communicator (%i) that does not exist\n", new_comm);
    } else {
      new_comm_group.erase(comm_it1);
      completed.erase(new_comm);
      new_comm_metadata.erase(new_comm);
    }
  }

  std::vector<comm_t> CommCreateConstructor::get_remapping(comm_t new_comm) {
    std::vector<comm_t> result;
    auto ncg_it = new_comm_group.find(new_comm);

    // Build up a list
    if (ncg_it != new_comm_group.end()) {
      for(auto& local_comm : ncg_it->second)
        result.push_back(local_comm.local_comm_id);
    }

    return result;
  }

}
