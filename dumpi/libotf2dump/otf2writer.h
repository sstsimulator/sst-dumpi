#ifndef OTF2WRITER_H
#define OTF2WRITER_H

#include "otf2writer-defs.h"

#include <queue>

/*
 * USAGE
 * This trace writer consists of a translation layer with an MPI-like API. At the end of its usage, the user should have a complete OTF2 archive
 * Its usage consists of several phases
 * 1) Initializing the environemnt and supplying critical information
 *    - Opening the archive with a valid path
 *    - Registering communicator constants, such as MPI_COMM_WORLD, MPI_COMM_NULL, etc
 *    - Setting clock resolution (tics/second)
 *    - Register MPI types (Needed to reconstruct byte sizes)
 *
 * 2) Calling the MPI Function hooks
 *    - Important calls have their own hooks (Collectives, P2P Communication, Group/Comm creation)
 *    - Use generic_call() for other functions if you need to record timestamps are wanted.
 *
 * 3) Closing the archive with close_archive()
 *
 * IMPLEMENTATION
 * - Phase 1 initializes an OTF2 trace directory, the caller must supply environment information
 *
 * - Phase 2
 *    - Opens an .evt file the first an MPI rank is used
 *    - Streams events to .evt files.
 *    - Communicator and Group creation events are captured as lambdas and saved to a queue
 *
 * - Phase 3
 *    - Captured Comm and Group creation lambdas are replayed to identify their characteristic traits
 *    - The global def file is written
 *    - Local def files are written to map local communicator identities to global ones.
 *    - File handles are closed
 *
 * TODO: update for new write modes.
 *
 * PERFORNANCE LIMITATIONS
 * Much of the runtime consists of pipelining application activity to event files on disk, and will
 * scale linearly with the runtime's scale.
 *
 * If ulimits has a low file limit, and the caller has a lot of ranks, the program may fail to
 * open enough files.
 *
 * For large rank counts, the current group creation implementation will be memory limited.
 * Groups are internally generated on each rank, and internally consist of a list of ranks and a few
 * references in a stl map. Registering MPI_COMM_WORLD will create a group on each rank of size N, with a
 * space complexity of O(N^2) overhead. i.e. a 100K rank run will create groups consuming about
 * 100K*100K*4 bytes, or 40GB of runtime memory. If all ranks in the application create a new comm
 * or of the same size, another 40GB overhead is created
 *
 * PERFORMANCE MITIGATION
 * MPI Groups will need to be created multiple times, but only need to be stored in memory once. If
 * scale becomes an issue, a group creation function can do a de-duplication check with linear time
 * complexity before writing.
 */

namespace dumpi {

  /**
   * @brief The OTF2_Writer class makes emitting OTF2 traces a bit easier
   */
  class OTF2_Writer {
  public:
    OTF2_Writer();

    static const int RANK_UNDEF = -1;
    static const int COMM_SIZE_NONE = 0;

    std::string get_otf2_directory();
    OTF2_WRITER_RESULT open_archive(std::string path, int num_ranks, bool force);
    OTF2_WRITER_RESULT close_archive();
    OTF2_WRITER_RESULT register_group(int id, std::vector<int> list);
    OTF2_WRITER_RESULT register_comm(std::string name, int id, int parent, int group);
    OTF2_WRITER_RESULT set_rank(int rank);

    void register_comm_world(comm_t id);
    void register_comm_self(comm_t id);
    void register_comm_error(comm_t id);
    void register_comm_null(comm_t id);
    void register_type(mpi_type_t type, int size);
    void register_null_request(request_t request);
    void set_verbosity(OTF2_WRITER_VERBOSITY verbosity);
    void set_clock_resolution(uint64_t ticks_per_second);

    // Enter a state where only MPI_Comm_* and MPI_Group_* calls are registered before any calls that may
    void set_comm_mode(COMM_MODE);

    OTF2_WRITER_RESULT generic_call(int rank, otf2_time_t start, otf2_time_t stop, std::string name);

    OTF2_WRITER_RESULT mpi_send(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_bsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_ssend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_rsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);

    OTF2_WRITER_RESULT mpi_isend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);
    OTF2_WRITER_RESULT mpi_ibsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);
    OTF2_WRITER_RESULT mpi_issend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);
    OTF2_WRITER_RESULT mpi_irsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);

    OTF2_WRITER_RESULT mpi_recv(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_irecv(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag, request_t request);

    OTF2_WRITER_RESULT mpi_wait(int rank, otf2_time_t start, otf2_time_t stop, request_t request);
    OTF2_WRITER_RESULT mpi_waitall(int rank, otf2_time_t start, otf2_time_t stop, int count, const request_t* requests);
    OTF2_WRITER_RESULT mpi_waitany(int rank, otf2_time_t start, otf2_time_t stop, request_t request);
    OTF2_WRITER_RESULT mpi_waitsome(int rank, otf2_time_t start, otf2_time_t stop, const request_t* requests, int outcount, const int* indices);

    OTF2_WRITER_RESULT mpi_test(int rank, otf2_time_t start, otf2_time_t stop, request_t request, int flag);
    OTF2_WRITER_RESULT mpi_testall(int rank, otf2_time_t start, otf2_time_t stop, int count, const request_t* requests, int flag);
    OTF2_WRITER_RESULT mpi_testany(int rank, otf2_time_t start, otf2_time_t stop, const request_t* requests, int index, int flag);
    OTF2_WRITER_RESULT mpi_testsome(int rank, otf2_time_t start, otf2_time_t stop, const request_t* requests, int outcount, const int* indices);

    OTF2_WRITER_RESULT mpi_allgather(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, comm_t comm);
    OTF2_WRITER_RESULT mpi_allreduce(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, comm_t comm);
    OTF2_WRITER_RESULT mpi_allgatherv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, int sendcount, mpi_type_t sendtype, const int* recvcounts, mpi_type_t recvtype, comm_t comm);
    OTF2_WRITER_RESULT mpi_alltoall(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, comm_t comm);
    OTF2_WRITER_RESULT mpi_alltoallv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, const int* sendcounts, mpi_type_t sendtype, const int* recvcounts, mpi_type_t recvtype, comm_t comm);
    OTF2_WRITER_RESULT mpi_barrier(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm);
    OTF2_WRITER_RESULT mpi_bcast(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, int root, bool is_root, comm_t comm);
    OTF2_WRITER_RESULT mpi_gather(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, bool is_root, comm_t comm);
    OTF2_WRITER_RESULT mpi_gatherv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, int sendcount, mpi_type_t sendtype, const int* recvcounts, mpi_type_t recvtype, int root, bool is_root, comm_t comm);
    OTF2_WRITER_RESULT mpi_reduce(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, int root, comm_t comm);
    OTF2_WRITER_RESULT mpi_reduce_scatter(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, const int* recvcounts, mpi_type_t type, comm_t comm);
    OTF2_WRITER_RESULT mpi_scatter(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, bool is_root, comm_t comm);
    OTF2_WRITER_RESULT mpi_scatterv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, const int* sendcounts, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, bool is_root, comm_t comm);
    OTF2_WRITER_RESULT mpi_scan(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t datatype, comm_t comm);
    //OTF2_WRITER_RESULT mpi_exscan(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t datatype, comm_t comm);

    OTF2_WRITER_RESULT mpi_group_difference(int rank, otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup);
    OTF2_WRITER_RESULT mpi_group_excl(int rank, otf2_time_t start, otf2_time_t stop, int group, int count, const int*ranks, int newgroup);
    OTF2_WRITER_RESULT mpi_group_incl(int rank, otf2_time_t start, otf2_time_t stop, int group, int count, const int*ranks, int newgroup);
    OTF2_WRITER_RESULT mpi_group_intersection(int rank, otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup);
    OTF2_WRITER_RESULT mpi_group_range_incl(int rank, otf2_time_t start, otf2_time_t stop, int group, int count, int**ranges, int newgroup);
    OTF2_WRITER_RESULT mpi_group_union(int rank, otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup);

    OTF2_WRITER_RESULT mpi_comm_dup(int rank, otf2_time_t start, otf2_time_t stop, comm_t oldcomm, comm_t newcomm);
    OTF2_WRITER_RESULT mpi_comm_group(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm, int group);
    OTF2_WRITER_RESULT mpi_comm_create(int rank, otf2_time_t start, otf2_time_t stop, comm_t oldcomm, int group, comm_t newcomm);

    OTF2_WRITER_RESULT mpi_comm_split(int rank, otf2_time_t start, otf2_time_t stop, comm_t oldcomm, int key, int color, comm_t newcomm);

    // Depricated in MPI v2.0
    OTF2_WRITER_RESULT mpi_type_contiguous(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_hvector(int rank, otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_vector(int rank, otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_indexed(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* lengths, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_hindexed(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* lengths, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_struct(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* lengths, mpi_type_t* oldtypes, mpi_type_t newtype);

    // Introduced in MPI v2.0
    OTF2_WRITER_RESULT mpi_type_create_struct(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_create_subarray(int rank, otf2_time_t start, otf2_time_t stop, int ndims, const int* subsizes, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_create_hvector(int rank, otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
    OTF2_WRITER_RESULT mpi_type_create_hindexed(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* lengths, mpi_type_t oldtype, mpi_type_t newtype);

    // The first three Communicator groups are reserved.
    static const uint64_t COMM_LOCATIONS_GROUP_ID = 0;
    static const uint64_t COMM_WORLD_GROUP_ID = 1;
    static const uint64_t COMM_SELF_GROUP_ID = 2;
    static const uint64_t USER_DEF_COMM_GROUP_OFFSET = 3;

    static const uint64_t MPI_COMM_WORLD_ID = 0;
    static const uint64_t MPI_COMM_SELF_ID = 1;
    static const uint64_t MPI_COMM_USER_OFFSET = 2;

  private:
    bool ranks_equivalent(int world_rank, int comm_rank, comm_t comm, RankContext* ctx = nullptr);
    int get_comm_rank(int world_rank, comm_t comm);
    int get_comm_size(comm_t comm);
    int mk_archive_dir(const char *path);
    void logger(OTF2_WRITER_VERBOSITY level, std::string);
    void logger(OTF2_WRITER_VERBOSITY level, const char *);
    void check_otf2(OTF2_ErrorCode status, const char* description);
    RankContext& fetch_context(int rank);
    //void close_evt_file(int rank);
    void write_def_files();
    void unwind_comm_events();

    // Compact some redundant code
    template<typename T>
    int array_sum(const T* array, int count);
    uint64_t count_bytes(mpi_type_t type, uint64_t count);

    std::vector<int>& get_group(comm_t comm);

    unsigned long hash_group(std::vector<int>& group);

    bool group_is_known(int group);
    bool comm_is_known(int comm);
    bool type_is_known(mpi_type_t type);

    // For MPI call variants
    OTF2_WRITER_RESULT mpi_send_inner(RankContext& ctx, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag);
    OTF2_WRITER_RESULT mpi_isend_inner(RankContext& ctx, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request);
    void mpi_t_struct_inner(const char* fname, int count, const int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype);
    void mpi_t_vector_inner(const char* fname, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype);
    void mpi_t_indexed_inner(const char* name, int count, const int* lengths, mpi_type_t oldtype, mpi_type_t newtype);

  private:
    std::string _directory;
    std::map<int, std::vector<int>> _mpi_group;
    std::map<int, MPI_Comm_Struct> _mpi_comm;
    std::unordered_map<mpi_type_t, int> _type_sizes;
    std::set<comm_t> _unknown_comms; // Comms that have shown up in the event files but have not been registered.

    OTF2DefTable _string;
    OTF2DefTable _region;

    std::map<int, RankContext> _archive_context;

    OTF2_Archive* _archive = nullptr;
    OTF2_TimeStamp _start_time = ~0;
    OTF2_TimeStamp _stop_time = 0;
    int _rank;
    int _num_ranks = -1;
    int _comm_world_id = -1;
    int _comm_self_id = -1;
    int _comm_error_id = -1;
    int _comm_null_id = -1;
    request_t _null_request = -1;
    CommSplitConstructor _comm_split_constructor;
    CommCreateConstructor _comm_create_constructor;

    // MPI calls that manipulate Comms and Groups are captured as lambdas and traced in order of when they ran.
    std::priority_queue<CommAction> _comm_actions;

    OTF2_WRITER_VERBOSITY _verbosity = OWV_NONE;
    COMM_MODE _comm_mode = COMM_MODE_NONE;
    uint64_t _clock_resolution = 0;
  };
}

#endif // OTF2WRITER_H
