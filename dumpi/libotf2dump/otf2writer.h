/**
Copyright 2009-2022 National Technology and Engineering Solutions of Sandia,
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S. Government
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly
owned subsidiary of Honeywell International, Inc., for the U.S. Department of
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2022, NTESS

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of the copyright holder nor the names of its
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
/**
Copyright 2009-2022 National Technology and Engineering Solutions of Sandia,
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S. Government
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly
owned subsidiary of Honeywell International, Inc., for the U.S. Department of
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2022, NTESS

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

#ifndef OTF2WRITER_H
#define OTF2WRITER_H

#include "otf2writer-defs.h"
#include <memory>
#include <dumpi/common/constants.h>

namespace dumpi {

struct OTF2_MPI_Group {
  mpi_group_t local_id;
  mpi_group_t global_id;
  bool is_comm_world;
  bool is_comm_self;
  bool written;
  int world_size;
  std::vector<int> global_ranks;

  using shared_ptr = std::shared_ptr<OTF2_MPI_Group>;

  int size() const {
    if (is_comm_world) return world_size;
    else if (is_comm_self) return 1;
    else return global_ranks.size();
  }

  int get_world_rank(int local_rank){
    if (is_comm_world) return local_rank;
    else if (is_comm_self) return 0;
    else return global_ranks[local_rank];
  }

  OTF2_MPI_Group() :
    is_comm_self(false), is_comm_world(false),
    written(false),
    local_id(-1), global_id(-1), world_size(-1)
  {}
};

struct OTF2_MPI_Comm {
  using shared_ptr = std::shared_ptr<OTF2_MPI_Comm>;

  mpi_comm_t local_id;
  mpi_comm_t global_id;
  bool is_root;
  bool is_split;
  int color;
  int key;
  int local_rank;
  int world_rank;
  OTF2_MPI_Group::shared_ptr group;
  OTF2_MPI_Comm::shared_ptr parent;
  const char* name;

  OTF2_MPI_Comm() :
    local_id(-1), global_id(-1),
    is_root(false), is_split(false),
    group(nullptr),
    color(-1), key(-1),
    local_rank(-1), world_rank(-1),
    name(nullptr), parent(nullptr)
  {}

  void set_split(int c, int k){
    is_split = true;
    color = c;
    key = k;
  }

  int size() const {
    return group->size();
  }

};


/**
 * @brief The OTF2_Writer class makes emitting OTF2 traces a bit easier
 */
class OTF2_Writer {
 public:
  using mpi_status_t = dumpi_status;

  template <class Key, class T>
  struct versioned_shared_ptr_map {
    std::map<Key, std::list<std::shared_ptr<T>>> the_map;

    std::shared_ptr<T>& operator[](const Key& k){
      auto& entry = the_map[k];
      return entry.front();
    }

    typename std::map<Key, std::list<std::shared_ptr<T>>>::const_iterator
    begin() const {
      return the_map.begin();
    }

    typename std::map<Key, std::list<std::shared_ptr<T>>>::const_iterator
    end() const {
      return the_map.end();
    }

    std::shared_ptr<T>& make_new(const Key& k){
      auto& entry = the_map[k];
      entry.push_front(std::shared_ptr<T>(new T));
      return entry.front();
    }

    void make_new(const Key& k, const std::shared_ptr<T>& t){
      auto& entry = the_map[k];
      entry.push_front(t);
    }

    void move_back(const Key& k){
      auto& entry = the_map[k];
      entry.push_back(entry.front());
      entry.pop_front();
    }

    void reverse(){
      for (auto& pair : the_map){
        pair.second.reverse();
      }
    }

    std::shared_ptr<T> find(const Key& k) const {
      auto iter = the_map.find(k);
      if (iter == the_map.end()){
        return std::shared_ptr<T>();
      } else {
        return iter->second;
      }
    }

    const std::shared_ptr<T>& operator[](const Key& k) const {
      auto iter = the_map.find(k);
      if (iter == the_map.end()){
        throw std::runtime_error("Bad key for shared_ptr map");
      }
      return iter->second.front();
    }

  };


  OTF2_Writer();

  struct worldConfig {
    int rank;
    int size;
  };

  std::string get_otf2_directory(){
    return directory_;
  }

  dumpi::OTF2_MPI_Comm::shared_ptr make_new_comm(mpi_comm_t comm){
    return comms_.make_new(comm);
  }

  dumpi::OTF2_MPI_Comm::shared_ptr get_comm(mpi_comm_t comm) const {
    return comms_[comm];
  }

  OTF2_WRITER_RESULT open_archive(const std::string& path);
  OTF2_WRITER_RESULT close_archive();
  void write_global_def_file(const std::vector<int>& event_counts,
                             const std::vector<OTF2_MPI_Comm::shared_ptr>& unique_comms,
                             uint64_t min_start_time, uint64_t max_start_time);

  void write_local_def_file();

  int event_count() const {
    return event_count_;
  }

  uint64_t start_time() const {
    return start_time_;
  }

  uint64_t stop_time() const {
    return stop_time_;
  }

  /**
   * When doing another pass, all the versions of a comm/group
   * with the same local ID need to be "reset" or reversed
   */
  void reverse_versions();

  void register_comm_world(mpi_comm_t id, int size, int rank);
  void register_comm_self(mpi_comm_t id);
  void register_comm_error(mpi_comm_t id);
  void register_comm_null(mpi_comm_t id);
  void register_type(mpi_type_t type, int size);
  void register_null_request(mpi_request_t request);
  void set_verbosity(OTF2_WRITER_VERBOSITY verbosity);
  void set_clock_resolution(uint64_t ticks_per_second);

  OTF2_WRITER_RESULT generic_call(otf2_time_t start, otf2_time_t stop, const std::string& name);

  OTF2_WRITER_RESULT mpi_send(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                              uint64_t count, uint32_t dest, mpi_comm_t comm, uint32_t tag);
  OTF2_WRITER_RESULT mpi_bsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                               uint64_t count, uint32_t dest, mpi_comm_t comm, uint32_t tag);
  OTF2_WRITER_RESULT mpi_ssend(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                               uint64_t count, uint32_t dest, mpi_comm_t comm, uint32_t tag);
  OTF2_WRITER_RESULT mpi_rsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                               uint64_t count, uint32_t dest, mpi_comm_t comm, uint32_t tag);

  OTF2_WRITER_RESULT mpi_isend(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                               uint64_t count, uint32_t dest, mpi_comm_t comm,
                               uint32_t tag, mpi_request_t request);
  OTF2_WRITER_RESULT mpi_ibsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                                uint64_t count, uint32_t dest, mpi_comm_t comm,
                                uint32_t tag, mpi_request_t request);
  OTF2_WRITER_RESULT mpi_issend(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                                uint64_t count, uint32_t dest, mpi_comm_t comm,
                                uint32_t tag, mpi_request_t request);
  OTF2_WRITER_RESULT mpi_irsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                                uint64_t count, uint32_t dest, mpi_comm_t comm,
                                uint32_t tag, mpi_request_t request);

  OTF2_WRITER_RESULT mpi_recv(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                              uint64_t count, uint32_t source, mpi_comm_t comm, uint32_t tag);
  OTF2_WRITER_RESULT mpi_irecv(otf2_time_t start, otf2_time_t stop, mpi_type_t type,
                               uint64_t count, uint32_t source, mpi_comm_t comm,
                               uint32_t tag, mpi_request_t request);

  OTF2_WRITER_RESULT mpi_wait(otf2_time_t start, otf2_time_t stop, mpi_request_t request,
                              const mpi_status_t* status);
  OTF2_WRITER_RESULT mpi_waitall(otf2_time_t start, otf2_time_t stop, int count,
                                 const mpi_request_t* requests,
                                 const mpi_status_t* statuses);
  OTF2_WRITER_RESULT mpi_waitany(otf2_time_t start, otf2_time_t stop, mpi_request_t request,
                                 const mpi_status_t* status);
  OTF2_WRITER_RESULT mpi_waitsome(otf2_time_t start, otf2_time_t stop, const mpi_request_t* requests,
                                  int outcount, const int* indices,
                                  const mpi_status_t* statuses);

  OTF2_WRITER_RESULT mpi_test(otf2_time_t start, otf2_time_t stop, mpi_request_t request, int flag,
                              const mpi_status_t* status);
  OTF2_WRITER_RESULT mpi_testall(otf2_time_t start, otf2_time_t stop, int count,
                                 const mpi_request_t* requests, int flag,
                                 const mpi_status_t* statuses);
  OTF2_WRITER_RESULT mpi_testany(otf2_time_t start, otf2_time_t stop, const mpi_request_t* requests,
                                 int index, int flag, const mpi_status_t* status);
  OTF2_WRITER_RESULT mpi_testsome(otf2_time_t start, otf2_time_t stop, const mpi_request_t* requests,
                                  int outcount, const int* indices, const mpi_status_t* statuses);

  OTF2_WRITER_RESULT mpi_allgather(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype,
                                   int recvcount, mpi_type_t recvtype, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_allreduce(otf2_time_t start, otf2_time_t stop, int count,
                                   mpi_type_t type, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_allgatherv(otf2_time_t start, otf2_time_t stop, int comm_size,
                                    int sendcount, mpi_type_t sendtype,
                                    const int* recvcounts, mpi_type_t recvtype, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_alltoall(otf2_time_t start, otf2_time_t stop,
                                  int sendcount, mpi_type_t sendtype, int recvcount,
                                  mpi_type_t recvtype, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_alltoallv(otf2_time_t start, otf2_time_t stop, int comm_size,
                                   const int* sendcounts, mpi_type_t sendtype,
                                   const int* recvcounts, mpi_type_t recvtype, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_barrier(otf2_time_t start, otf2_time_t stop, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_bcast(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type,
                               int root, mpi_comm_t comm);

  OTF2_WRITER_RESULT mpi_bcast_leaf(otf2_time_t start, otf2_time_t stop, int count, int root,
                                    mpi_type_t type, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_bcast_root(otf2_time_t start, otf2_time_t stop, int count, int root,
                                    mpi_type_t type, mpi_comm_t comm);

  OTF2_WRITER_RESULT mpi_gather(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype,
                                int recvcount, mpi_type_t recvtype, int root, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_gatherv(otf2_time_t start, otf2_time_t stop, int comm_size, int sendcount,
                                 mpi_type_t sendtype, const int* recvcounts, mpi_type_t recvtype,
                                 int root, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_reduce(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type,
                                int root, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_reduce_scatter(otf2_time_t start, otf2_time_t stop, int comm_size,
                                        const int* recvcounts, mpi_type_t type, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_scatter(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype,
                                 int recvcount, mpi_type_t recvtype, int root, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_scatterv(otf2_time_t start, otf2_time_t stop, int comm_size,
                                  const int* sendcounts, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype,
                                  int root, mpi_comm_t comm);
  OTF2_WRITER_RESULT mpi_scan(otf2_time_t start, otf2_time_t stop, int count,
                              mpi_type_t datatype, mpi_comm_t comm);

  OTF2_WRITER_RESULT mpi_group_difference(otf2_time_t start, otf2_time_t stop,
                                          mpi_group_t group1, mpi_group_t group2, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_difference_first_pass(mpi_group_t group1,
                                                     mpi_group_t group2, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_excl(otf2_time_t start, otf2_time_t stop, mpi_group_t group,
                                    int count, const int*ranks, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_excl_first_pass(mpi_group_t group, int count,
                                               const int* ranks, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_incl(otf2_time_t start, otf2_time_t stop, mpi_group_t group,
                                    int count, const int*ranks, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_incl_first_pass(mpi_group_t group, int count,
                                               const int* ranks, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_intersection(otf2_time_t start, otf2_time_t stop,
                                            mpi_group_t group1, mpi_group_t group2, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_intersection_first_pass(mpi_group_t group1, mpi_group_t group2,
                                                       mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_range_incl(otf2_time_t start, otf2_time_t stop, mpi_group_t group,
                                          int count, int**ranges, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_range_incl_first_pass(mpi_group_t group, int count,
                                                     int** ranges, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_union(otf2_time_t start, otf2_time_t stop,
                                     mpi_group_t group1, mpi_group_t group2, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_union_first_pass(mpi_group_t group1, mpi_group_t group2,
                                                mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_comm_dup(otf2_time_t start, otf2_time_t stop,
                                  mpi_comm_t oldcomm, mpi_comm_t newcomm);
  OTF2_WRITER_RESULT mpi_comm_dup_first_pass(mpi_comm_t oldcomm, mpi_comm_t newcomm);

  OTF2_WRITER_RESULT mpi_comm_group_first_pass(mpi_comm_t comm, mpi_group_t group);
  OTF2_WRITER_RESULT mpi_comm_group(otf2_time_t start, otf2_time_t stop, mpi_comm_t comm, mpi_group_t group);

  OTF2_WRITER_RESULT mpi_comm_create(otf2_time_t start, otf2_time_t stop, mpi_comm_t oldcomm,
                                     mpi_group_t group, mpi_comm_t newcomm);
  OTF2_WRITER_RESULT mpi_comm_create_first_pass(mpi_comm_t oldcomm, mpi_group_t group, mpi_comm_t newcomm);

  OTF2_WRITER_RESULT mpi_comm_split(otf2_time_t start, otf2_time_t stop, mpi_comm_t oldcomm,
                                    int color, int key, mpi_comm_t newcomm);
  OTF2_WRITER_RESULT mpi_comm_split_first_pass(mpi_comm_t oldcomm, int color, int key, mpi_comm_t newcomm);

  OTF2_WRITER_RESULT mpi_comm_free(otf2_time_t start, otf2_time_t stop, mpi_comm_t comm);

  // Depricated in MPI v2.0
  OTF2_WRITER_RESULT mpi_type_contiguous(otf2_time_t start, otf2_time_t stop, int count,
                                         mpi_type_t oldtype, mpi_type_t newtype);
  OTF2_WRITER_RESULT mpi_type_hvector(otf2_time_t start, otf2_time_t stop, int count, int blocklength,
                                      mpi_type_t oldtype, mpi_type_t newtype);
  OTF2_WRITER_RESULT mpi_type_vector(otf2_time_t start, otf2_time_t stop, int count, int blocklength,
                                     mpi_type_t oldtype, mpi_type_t newtype);
  OTF2_WRITER_RESULT mpi_type_indexed(otf2_time_t start, otf2_time_t stop, int count, const int* lengths,
                                      mpi_type_t oldtype, mpi_type_t newtype);
  OTF2_WRITER_RESULT mpi_type_hindexed(otf2_time_t start, otf2_time_t stop, int count, const int* lengths,
                                       mpi_type_t oldtype, mpi_type_t newtype);
  OTF2_WRITER_RESULT mpi_type_struct(otf2_time_t start, otf2_time_t stop, int count, const int* lengths,
                                     mpi_type_t* oldtypes, mpi_type_t newtype);

  // Introduced in MPI v2.0
  OTF2_WRITER_RESULT mpi_type_create_struct(otf2_time_t start, otf2_time_t stop, int count,
                                            const int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype);
  OTF2_WRITER_RESULT mpi_type_create_subarray(otf2_time_t start, otf2_time_t stop, int ndims,
                                              const int* subsizes, mpi_type_t oldtype, mpi_type_t newtype);
  OTF2_WRITER_RESULT mpi_type_create_hvector(otf2_time_t start, otf2_time_t stop, int count,
                                             int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
  OTF2_WRITER_RESULT mpi_type_create_hindexed(otf2_time_t start, otf2_time_t stop, int count,
                                              const int* lengths, mpi_type_t oldtype, mpi_type_t newtype);


  OTF2_MPI_Comm::shared_ptr pending_comm() const {
    return pending_comm_;
  }

  void clear_pending_comm() {
    pending_comm_ = nullptr;
  }

  int world_rank() const {
    return world_.rank;
  }

  int world_size() const {
    return world_.size;
  }

  const std::vector<OTF2_MPI_Comm::shared_ptr>& unique_comms() const {
    return unique_comms_;
  }

  std::vector<OTF2_MPI_Comm::shared_ptr> find_unique_comms() const;

  const versioned_shared_ptr_map<mpi_comm_t, dumpi::OTF2_MPI_Comm>& all_comms() const {
    return comms_;
  }

  void set_write_global_comms(bool flag){
    write_global_comms_ = flag;
  }

  void add_region(const std::string& name){
    otf2_regions_table_.insert(name);
  }

  mpi_comm_t get_trace_comm(mpi_comm_t local) const {
    if (write_global_comms_){
      return comms_[local]->global_id;
    } else {
      return local;
    }
  }

  OTF2_MPI_Group::shared_ptr make_comm_split(OTF2_MPI_Comm::shared_ptr comm,
                                             const std::vector<int>& world_ranks);

  static constexpr mpi_group_t COMM_LOCATIONS_GROUP_ID{0};
  static constexpr mpi_group_t MPI_GROUP_WORLD_ID{1};
  static constexpr mpi_group_t MPI_GROUP_SELF_ID{2};
  static constexpr mpi_comm_t MPI_COMM_WORLD_ID{0};
  static constexpr mpi_comm_t MPI_COMM_SELF_ID{1};
  static constexpr mpi_comm_t MPI_COMM_USER_ID_OFFSET{2};

  static const mpi_group_t global_group_id_from_comm_id(mpi_comm_t id){
    return id + 1;
  }

 private:
  int get_group_rank(int world_rank, OTF2_MPI_Group::shared_ptr group);

  int mk_archive_dir(const char *path);
  void logger(OTF2_WRITER_VERBOSITY level, const std::string& msg);
  void logger(OTF2_WRITER_VERBOSITY level, const char* msg);
  void check_otf2(OTF2_ErrorCode status, const char* description);

  uint64_t count_bytes(mpi_type_t type, uint64_t count);

  bool type_is_known(mpi_type_t type);

  // For MPI call variants
  OTF2_WRITER_RESULT mpi_send_inner(otf2_time_t start, mpi_type_t type, uint64_t count,
                                    uint32_t dest, mpi_comm_t comm, uint32_t tag);
  OTF2_WRITER_RESULT mpi_isend_inner(otf2_time_t start, mpi_type_t type, uint64_t count,
                                     uint32_t dest, mpi_comm_t comm, uint32_t tag, mpi_request_t request);
  void mpi_t_struct_inner(const char* fname, int count, const int* blocklengths,
                          mpi_type_t* oldtypes, mpi_type_t newtype);
  void mpi_t_vector_inner(const char* fname, int count, int blocklength,
                          mpi_type_t oldtype, mpi_type_t newtype);
  void mpi_t_indexed_inner(const char* name, int count, const int* lengths,
                           mpi_type_t oldtype, mpi_type_t newtype);

 private:
  void incomplete_call(mpi_request_t request_id, REQUEST_TYPE type);

  void complete_call(mpi_request_t request_id, uint64_t timestamp,
                     const mpi_status_t* status);

  struct exception : public std::runtime_error {
    exception(const std::string& error) :
      std::runtime_error(error)
    {}
  };

  std::string directory_;

  std::unordered_map<mpi_type_t, int> type_sizes_;

  versioned_shared_ptr_map<int, OTF2_MPI_Comm> comms_;
  versioned_shared_ptr_map<int, OTF2_MPI_Group> groups_;

  OTF2DefTable otf2_strings_table_;
  OTF2DefTable otf2_regions_table_;

  OTF2_Archive* archive_ = nullptr;
  OTF2_TimeStamp start_time_;
  OTF2_TimeStamp stop_time_;

  std::vector<OTF2_MPI_Comm::shared_ptr> unique_comms_;


  static constexpr int undefined_root = -1;
  OTF2_EvtWriter* evt_writer_ = nullptr;
  std::unordered_map<mpi_request_t, irecv_capture> irecv_requests_;
  std::unordered_map<mpi_request_t, REQUEST_TYPE> request_type_;

  worldConfig world_;

  mpi_request_t null_request_;
  mpi_comm_t comm_world_id_;
  mpi_comm_t comm_self_id_;
  mpi_comm_t comm_error_id_;
  mpi_comm_t comm_null_id_;

  bool write_global_comms_;

  OTF2_MPI_Comm::shared_ptr pending_comm_;

  int event_count_;

  int next_group_id_;

  OTF2_WRITER_VERBOSITY verbosity_ = OWV_NONE;
  uint64_t clock_resolution_;
};


}

#endif // OTF2WRITER_H
