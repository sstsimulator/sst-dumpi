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

#ifndef OTF2WRITER_H
#define OTF2WRITER_H

#include "otf2writer-defs.h"

namespace dumpi {

struct tree_id {

  tree_id() : tree_index_(0) {}

  void add_level(){
    tree_.emplace_back(0);
    tree_index_ = 0;
  }

  void advance(){
    ++tree_index_;
    tree_.back() = tree_index_;
  }

  void remove_level(){
    tree_.pop_back();
  }

  bool equals(const tree_id& id) const {
    if (id.tree_.size() != tree_.size()) return false;

    int len = tree_.size();
    for (int i=0; i < len; ++i){
      if (tree_[i] != id.tree_[i]) return false;
    }

    return true;
  }

  uint32_t hash() const {
    int len = tree_.size();
    uint32_t hash = 0;
    for(int i = 0; i < len; ++i)
    {
      hash += tree_[i];
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
  }

 private:
  std::vector<uint32_t> tree_;
  uint32_t tree_index_;

};

static bool operator==(const tree_id& l, const tree_id& r){
  return l.equals(r);
}

} //end namespaced dumpi

namespace std {

template <> struct hash<dumpi::tree_id> {
  size_t operator()(const dumpi::tree_id& tree) const {
    return tree.hash();
  }
};

}

namespace dumpi {

struct global_id_assigner {
  global_id_assigner(int start_id) :
    id_counter(start_id) {}

  void assign_current(){
    ids[unique_id] = id_counter++;
  }

  void add_level(){
    unique_id.add_level();
  }

  void advance(){
    unique_id.advance();
  }

  void remove_level(){
    unique_id.remove_level();
  }

  int get_id(const tree_id& tid) const {
    auto iter = ids.find(tid);
    if (iter == ids.end()){
      return -1;
    } else {
      return iter->second;
    }
  }

 private:
  std::unordered_map<tree_id,int> ids;
  int id_counter;
  tree_id unique_id;
};


struct OTF2_MPI_Group {
  mpi_group_t local_id;
  mpi_group_t global_id;
  bool is_comm_world;
  bool is_comm_self;
  bool written;
  std::vector<int> global_ranks;

  int get_world_rank(int local_rank){
    if (is_comm_world) return local_rank;
    else if (is_comm_self) return 0;
    else return global_ranks[local_rank];
  }

  OTF2_MPI_Group() :
    is_comm_self(false), is_comm_world(false),
    written(false),
    local_id(-1), global_id(-1)
  {}
};

struct OTF2_MPI_Comm {
  mpi_comm_t local_id;
  mpi_comm_t global_id;
  bool is_root;
  OTF2_MPI_Group* group;
  OTF2_MPI_Comm* parent;
  const char* name;
  std::list<OTF2_MPI_Comm*> sub_comms;
  OTF2_MPI_Comm() :
    local_id(-1), global_id(-1),
    is_root(false), group(nullptr),
    name(nullptr), parent(nullptr)
  {}
};

/**
 * @brief The OTF2_Writer class makes emitting OTF2 traces a bit easier
 */
class OTF2_Writer {
 public:
  OTF2_Writer();

  struct worldConfig {
    int rank;
    int size;
  };

  std::string get_otf2_directory(){
    return directory_;
  }

  OTF2_WRITER_RESULT open_archive(const std::string& path);
  OTF2_WRITER_RESULT close_archive();
  void write_global_def_file(const std::vector<int>& event_counts,
                             const std::vector<OTF2_MPI_Comm*>& unique_comms,
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

  OTF2_WRITER_RESULT mpi_wait(otf2_time_t start, otf2_time_t stop, mpi_request_t request);
  OTF2_WRITER_RESULT mpi_waitall(otf2_time_t start, otf2_time_t stop, int count,
                                 const mpi_request_t* requests);
  OTF2_WRITER_RESULT mpi_waitany(otf2_time_t start, otf2_time_t stop, mpi_request_t request);
  OTF2_WRITER_RESULT mpi_waitsome(otf2_time_t start, otf2_time_t stop, const mpi_request_t* requests,
                                  int outcount, const int* indices);

  OTF2_WRITER_RESULT mpi_test(otf2_time_t start, otf2_time_t stop, mpi_request_t request, int flag);
  OTF2_WRITER_RESULT mpi_testall(otf2_time_t start, otf2_time_t stop, int count,
                                 const mpi_request_t* requests, int flag);
  OTF2_WRITER_RESULT mpi_testany(otf2_time_t start, otf2_time_t stop, const mpi_request_t* requests,
                                 int index, int flag);
  OTF2_WRITER_RESULT mpi_testsome(otf2_time_t start, otf2_time_t stop, const mpi_request_t* requests,
                                  int outcount, const int* indices);

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
  OTF2_WRITER_RESULT mpi_group_difference_first_pass(otf2_time_t start, otf2_time_t stop,
                                          mpi_group_t group1, mpi_group_t group2, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_excl(otf2_time_t start, otf2_time_t stop, mpi_group_t group,
                                    int count, const int*ranks, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_excl_first_pass(otf2_time_t start, otf2_time_t stop, int group,
                                    int count, const int*ranks, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_incl(otf2_time_t start, otf2_time_t stop, mpi_group_t group,
                                    int count, const int*ranks, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_incl_first_pass(otf2_time_t start, otf2_time_t stop, mpi_group_t group,
                                    int count, const int*ranks, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_intersection(otf2_time_t start, otf2_time_t stop,
                                            mpi_group_t group1, mpi_group_t group2, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_intersection_first_pass(otf2_time_t start, otf2_time_t stop,
                                            mpi_group_t group1, mpi_group_t group2, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_range_incl(otf2_time_t start, otf2_time_t stop, mpi_group_t group,
                                          int count, int**ranges, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_range_incl_first_pass(otf2_time_t start, otf2_time_t stop, mpi_group_t group,
                                          int count, int**ranges, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_group_union(otf2_time_t start, otf2_time_t stop,
                                     mpi_group_t group1, mpi_group_t group2, mpi_group_t newgroup);
  OTF2_WRITER_RESULT mpi_group_union_first_pass(otf2_time_t start, otf2_time_t stop,
                                                mpi_group_t group1, mpi_group_t group2, mpi_group_t newgroup);

  OTF2_WRITER_RESULT mpi_comm_dup(otf2_time_t start, otf2_time_t stop,
                                  mpi_comm_t oldcomm, mpi_comm_t newcomm);
  OTF2_WRITER_RESULT mpi_comm_dup_first_pass(otf2_time_t start, otf2_time_t stop,
                                             mpi_comm_t oldcomm, mpi_comm_t newcomm);

  OTF2_WRITER_RESULT mpi_comm_group_first_pass(otf2_time_t start, otf2_time_t stop, 
                                               mpi_comm_t comm, mpi_group_t group);
  OTF2_WRITER_RESULT mpi_comm_group(otf2_time_t start, otf2_time_t stop, mpi_comm_t comm, mpi_group_t group);

  OTF2_WRITER_RESULT mpi_comm_create(otf2_time_t start, otf2_time_t stop, mpi_comm_t oldcomm,
                                     mpi_group_t group, mpi_comm_t newcomm);
  OTF2_WRITER_RESULT mpi_comm_create_first_pass(otf2_time_t start, otf2_time_t stop, mpi_comm_t oldcomm,
                                                mpi_group_t group, mpi_comm_t newcomm);

  OTF2_WRITER_RESULT mpi_comm_split(otf2_time_t start, otf2_time_t stop, mpi_comm_t oldcomm,
                                    int key, int color, mpi_comm_t newcomm);
  OTF2_WRITER_RESULT mpi_comm_split_first_pass(otf2_time_t start, otf2_time_t stop, mpi_comm_t oldcomm,
                                    int key, int color, mpi_comm_t newcomm);

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

  void agree_global_ids(global_id_assigner& assigner,
                        std::vector<OTF2_MPI_Comm*>& unique_comms);

  void assign_global_ids(const global_id_assigner& global_ids);


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
  int get_world_rank(int comm_rank, mpi_comm_t comm);
  int get_comm_rank(int world_rank, mpi_comm_t comm);
  int get_comm_size(mpi_comm_t comm);
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

  void complete_call(mpi_request_t request_id, uint64_t timestamp);

  void agree_global_ids(const std::list<OTF2_MPI_Comm*>& subs,
                        global_id_assigner& assigner,
                        std::vector<OTF2_MPI_Comm*>& unique_comms);

  void agree_global_ids(OTF2_MPI_Comm* comm, global_id_assigner& assigner,
                        std::vector<OTF2_MPI_Comm*>& unique_comms);

  void assign_global_ids(const std::list<OTF2_MPI_Comm*>& subs,
                         const global_id_assigner& global_ids,
                         tree_id& local_ids);

  void assign_global_ids(OTF2_MPI_Comm* comm,
                         const global_id_assigner& global_ids,
                         tree_id& local_ids);

  mpi_comm_t get_global_comm(mpi_comm_t local_id) const;

  struct exception : public std::runtime_error {
    exception(const std::string& error) :
      std::runtime_error(error)
    {}
  };

  std::string directory_;

  std::unordered_map<mpi_type_t, int> type_sizes_;

  std::unordered_map<int, OTF2_MPI_Comm> comms_;
  std::unordered_map<int, OTF2_MPI_Group> groups_;

  OTF2DefTable otf2_strings_table_;
  OTF2DefTable otf2_regions_table_;

  OTF2_Archive* archive_ = nullptr;
  OTF2_TimeStamp start_time_ = ~0;
  OTF2_TimeStamp stop_time_ = 0;


  static constexpr int undefined_root = -1;
  OTF2_EvtWriter* evt_writer = nullptr;
  std::unordered_map<mpi_request_t, irecv_capture> irecv_requests;
  std::unordered_map<mpi_request_t, REQUEST_TYPE> request_type;

  worldConfig world_;

  mpi_request_t null_request_ = -1;
  mpi_comm_t comm_world_id_ = -1;
  mpi_comm_t comm_self_id_ = -1;
  mpi_comm_t comm_error_id_ = -1;
  mpi_comm_t comm_null_id_ = -1;

  int event_count_;

  OTF2_WRITER_VERBOSITY verbosity_ = OWV_NONE;
  uint64_t clock_resolution_ = 0;
};


}

#endif // OTF2WRITER_H
