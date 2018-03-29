#ifndef OTF2WRITER_H
#define OTF2WRITER_H

#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <otf2/otf2.h>

namespace dumpi {

  typedef int32_t request_t;
  typedef int32_t comm_t;
  typedef int16_t mpi_type_t;
  typedef uint64_t otf2_time_t;

  enum OTF2_WRITER_RESULT {
    OTF2_WRITER_SUCCESS = 0,
    OTF2_WRITER_ERROR_DIRECTORY_ALREADY_EXISTS,
    OTF2_WRITER_ERROR_DIRECTORY_PATH_DOES_NOT_EXIST,
    OTF2_WRITER_ERROR_NOT_IMPLEMENTED,
    OTF2_WRITER_ERROR_OUT_OF_RANGE,
    OTF2_WRITER_ERROR_NO_RANK_SET,
    OTF2_WRITER_ERROR_WRITER_NOT_SET,
    OTF2_WRITER_ERROR_ARCHIVE_ALREADY_OPEN,
    OTF2_WRITER_ERROR_UKNOWN_MPI_GROUP,
    OTF2_WRITER_ERROR_UKNOWN_MPI_COMM,
    OTF2_WRITER_ERROR_UKNOWN_MPI_TYPE
  };

  enum OTF2_WRITER_VERBOSITY {
    OWV_NONE = 0,
    OWV_ABORT,
    OWV_ERROR,
    OWV_WARN,
    OWV_INFO
  };

  enum REQUEST_TYPE {
    REQUEST_TYPE_ISEND = 0,
    REQUEST_TYPE_IRECV
  };

  struct irecv_capture {
    uint64_t count;
    int  type;
    uint32_t source;
    uint32_t tag;
    int comm;
    request_t request;
  };

  /**
   * @brief The OTF2DefTable class maps an incrementing id to a string, which it can later retrieve.
   */
  class OTF2DefTable {
  public:
    OTF2DefTable();
    int map(const std::string string);
    const std::string map(int index);

    // Operator overload sugar
    const std::string operator[] (int index);
    int operator[] (const std::string string);
    int operator[] (const char*);
    int size();
    bool added_last_lookup();
  private:
    std::unordered_map<std::string, int> _map;
    int _counter = 0;
    bool _added_last_lookup;
  };

  struct MPI_Comm_Struct {
    MPI_Comm_Struct(): name(""), parent(0), id(0), ndims(0) {}
    MPI_Comm_Struct(std::string name, int parent, int id, int group) : name(name), parent(parent), id(id), group(group), ndims(0){}
    MPI_Comm_Struct(const char* name, int parent, int id, int group) : name(name), parent(parent), id(id), group(group), ndims(0){}

    int parent;
    int id;
    int group;
    int ndims;
    std::vector<int> dims;
    std::string name;
  };

  /**
   * @brief The OTF2_Writer class makes emitting OTF2 traces a bit easier
   */
  class OTF2_Writer {
  public:
    OTF2_Writer();

    std::string get_otf2_directory();
    OTF2_WRITER_RESULT open_archive(std::string path, int num_ranks, bool force);
    OTF2_WRITER_RESULT close_archive();
    OTF2_WRITER_RESULT register_group(int id, std::vector<int> list);
    OTF2_WRITER_RESULT register_comm(std::string name, int id, int parent, int group);
    OTF2_WRITER_RESULT set_rank(int rank);

    void register_comm_world(comm_t id);
    void register_type(mpi_type_t type, int size);
    void register_null_request(request_t request);
    void set_verbosity(OTF2_WRITER_VERBOSITY verbosity);
    void set_clock_resolution(uint64_t ticks_per_second);

    // Remove calls that SST/macro does not need for trace replay.
    //void skip_unused_calls(bool);

    OTF2_WRITER_RESULT generic_call(otf2_time_t start, otf2_time_t stop, std::string name);

    OTF2_WRITER_RESULT mpi_send(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_bsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_ssend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_rsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);

    OTF2_WRITER_RESULT mpi_isend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);
    OTF2_WRITER_RESULT mpi_ibsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);
    OTF2_WRITER_RESULT mpi_issend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);
    OTF2_WRITER_RESULT mpi_irsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);

    OTF2_WRITER_RESULT mpi_recv(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_irecv(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag, request_t request);

    OTF2_WRITER_RESULT mpi_wait(otf2_time_t start, otf2_time_t stop, request_t request);
    OTF2_WRITER_RESULT mpi_waitall(otf2_time_t start, otf2_time_t stop, int count, request_t* requests);
    OTF2_WRITER_RESULT mpi_waitany(otf2_time_t start, otf2_time_t stop, request_t request);
    OTF2_WRITER_RESULT mpi_waitsome(otf2_time_t start, otf2_time_t stop, request_t* requests, int outcount, int* indices);

    OTF2_WRITER_RESULT mpi_test(otf2_time_t start, otf2_time_t stop, request_t request, int flag);
    OTF2_WRITER_RESULT mpi_testall(otf2_time_t start, otf2_time_t stop, int count, request_t* requests, int flag);
    OTF2_WRITER_RESULT mpi_testany(otf2_time_t start, otf2_time_t stop, request_t* requests, int index, int flag);
    OTF2_WRITER_RESULT mpi_testsome(otf2_time_t start, otf2_time_t stop, request_t* requests, int outcount, int* indices);

    OTF2_WRITER_RESULT mpi_allgather(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, comm_t comm);
    OTF2_WRITER_RESULT mpi_allreduce(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, comm_t comm);
    OTF2_WRITER_RESULT mpi_allgatherv(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int* recvcounts, mpi_type_t recvtype, comm_t comm);
    OTF2_WRITER_RESULT mpi_alltoall(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, comm_t comm);
    OTF2_WRITER_RESULT mpi_alltoallv(otf2_time_t start, otf2_time_t stop, int* sendcounts, mpi_type_t sendtype, int* recvcounts, mpi_type_t recvtype, comm_t comm);
    OTF2_WRITER_RESULT mpi_barrier(otf2_time_t start, otf2_time_t stop, comm_t comm);
    OTF2_WRITER_RESULT mpi_bcast(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, int root, comm_t comm);
    OTF2_WRITER_RESULT mpi_gather(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, comm_t comm);
    OTF2_WRITER_RESULT mpi_gatherv(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int* recvcounts, mpi_type_t recvtype, int root, comm_t comm);
    OTF2_WRITER_RESULT mpi_reduce(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, int root, comm_t comm);
    OTF2_WRITER_RESULT mpi_reduce_scatter(otf2_time_t start, otf2_time_t stop, int* recvcounts, mpi_type_t type, comm_t comm);
    OTF2_WRITER_RESULT mpi_scatter(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, comm_t comm);
    OTF2_WRITER_RESULT mpi_scatterv(otf2_time_t start, otf2_time_t stop, int* sendcounts, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, comm_t comm);
    OTF2_WRITER_RESULT mpi_scan(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t datatype, comm_t comm);
    //OTF2_WRITER_RESULT mpi_exscan(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t datatype, comm_t comm);

    OTF2_WRITER_RESULT mpi_group_difference(otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup);
    OTF2_WRITER_RESULT mpi_group_excl(otf2_time_t start, otf2_time_t stop, int group, int count, int*ranks, int newgroup);
    OTF2_WRITER_RESULT mpi_group_incl(otf2_time_t start, otf2_time_t stop, int group, int count, int*ranks, int newgroup);
    OTF2_WRITER_RESULT mpi_group_intersection(otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup);
    OTF2_WRITER_RESULT mpi_group_range_incl(otf2_time_t start, otf2_time_t stop, int group, int count, int**ranges, int newgroup);
    //OTF2_WRITER_RESULT mpi_group_range_excl(otf2_time_t start, otf2_time_t stop, int group, int count, int**ranges, int newgroup);
    OTF2_WRITER_RESULT mpi_group_union(otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup);

    OTF2_WRITER_RESULT mpi_comm_dup(otf2_time_t start, otf2_time_t stop, comm_t oldcomm, comm_t newcomm);
    OTF2_WRITER_RESULT mpi_comm_group(otf2_time_t start, otf2_time_t stop, comm_t comm, int group);
    OTF2_WRITER_RESULT mpi_comm_create(otf2_time_t start, otf2_time_t stop, comm_t oldcomm, int group, comm_t newcomm);

    //OTF2_WRITER_RESULT mpi_cart_create(otf2_time_t start, otf2_time_t stop, comm_t oldcomm, int ndims, int* dims, comm_t newcomm);
    //OTF2_WRITER_RESULT mpi_cart_sub(otf2_time_t start, otf2_time_t stop, comm_t oldcomm, const int* remain_dims, comm_t newcomm);

    // TODO: need to know the result of comm_split before recording gatherv, scatterv, allgatherv, alltoallv, and reduce_scatter
    //OTF2_WRITER_RESULT mpi_comm_split(otf2_time_t start, otf2_time_t stop, comm_t oldcomm, int color, int key, comm_t newcomm);

    // These are depricated in MPI v2.0
    OTF2_WRITER_RESULT mpi_type_contiguous(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_hvector(otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_vector(otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_indexed(otf2_time_t start, otf2_time_t stop, int count, int* lengths, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_hindexed(otf2_time_t start, otf2_time_t stop, int count, int* lengths, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_struct(otf2_time_t start, otf2_time_t stop, int count, int* lengths, mpi_type_t* oldtypes, mpi_type_t newtype);

    OTF2_WRITER_RESULT mpi_type_create_struct(otf2_time_t start, otf2_time_t stop, int count, int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_create_subarray(otf2_time_t start, otf2_time_t stop, int ndims, int* subsizes, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_create_hvector(otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_create_hindexed(otf2_time_t start, otf2_time_t stop, int count, int* lengths, mpi_type_t oldtype, mpi_type_t newtype);


  private:
    void incomplete_call(int request_id, REQUEST_TYPE type);
    int complete_call(request_t request_id, uint64_t timestamp);
    bool ranks_equivalent(int world_rank, int comm_rank, comm_t comm);
    int get_comm_rank(int world_rank, comm_t comm);
    int get_comm_size(comm_t comm);
    int mk_archive_dir(const char *path);
    void logger(OTF2_WRITER_VERBOSITY level, std::string);
    void logger(OTF2_WRITER_VERBOSITY level, const char *);
    void check_otf2(OTF2_ErrorCode status, const char* description);
    void close_evt_file();
    void write_def_files();
    //std::vector<int>& create_shadow_group();

    // Compact some redundant code
    template<typename T>
    int array_sum(const T* array, int count);
    uint64_t count_bytes(mpi_type_t type, uint64_t count);

    bool group_is_known(int group);
    bool comm_is_known(int comm);
    bool type_is_known(mpi_type_t type);

    // For MPI call variants
    OTF2_WRITER_RESULT mpi_send_inner(OTF2_EvtWriter* evt_writer, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_isend_inner(OTF2_EvtWriter* evt_writer, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);
    void mpi_t_struct_inner(const char* fname, int count, int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype);
    void mpi_t_vector_inner(const char* fname, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
    void mpi_t_indexed_inner(const char* name, int count, int* lengths, mpi_type_t oldtype, mpi_type_t newtype);


  private:
    std::string _directory;
    std::unordered_map<int, std::vector<int>> _mpi_group;
    std::map<int, MPI_Comm_Struct> _mpi_comm;
    std::unordered_map<int, int> _event_count;
    std::unordered_map<mpi_type_t, int> _type_sizes;
    std::set<comm_t> _unknown_comms; // Comms that have shown up in the event files but have not been registered.

    OTF2DefTable _string;
    OTF2DefTable _region;

    std::unordered_map<int, irecv_capture> _irecv_requests;
    std::unordered_map<int, REQUEST_TYPE> _request_type;

    OTF2_Archive* _archive = nullptr;
    OTF2_EvtWriter* _evt_writer = nullptr;
    OTF2_TimeStamp _start_time = ~0;
    OTF2_TimeStamp _stop_time = 0;
    int rank;
    int _num_ranks = -1;
    int _comm_world_id = -1;
    request_t _null_request = -1;
    bool _skip_unused_calls = false;

    OTF2_WRITER_VERBOSITY _verbosity = OWV_NONE;
    uint64_t _clock_resolution = 0;

    // The first three COMM_GROUPS are reserved.
    const uint64_t COMM_LOCATIONS_GROUP_ID = 0;
    const uint64_t COMM_WORLD_GROUP_ID = 1;
    const uint64_t COMM_SELF_GROUP_ID = 2;
    const uint64_t USER_DEF_COMM_GROUP_OFFSET = 3;
  };
}
#endif // OTF2WRITER_H
