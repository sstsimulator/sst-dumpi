#include <dumpi/libotf2dump/otf2writer.h>
#include <sys/stat.h>
#include <algorithm>
#include <limits.h>
#include <cstring>
#include <unordered_set>
#include <functional>

#define CHECK_RANK(rank, _num_ranks) \
  if (rank < -1 || rank >= _num_ranks) return OTF2_WRITER_ERROR_NO_RANK_SET;

#define GET_ARCHIVE_CONTEXT(rank) \
  CHECK_RANK(rank, _num_ranks);   \
  RankContext& ctx = fetch_context(rank);

#define COMM_NULL_OR_ERROR(comm) (comm == _comm_null_id || comm == _comm_error_id)

// immediately returns an error code when the group is unknown
//#define UNKNOWN_GROUP_TRAP(group)              \
//  if (!group_is_known(group)) {                \
//    logger(OWV_ERROR, _fname + " failed");     \
//    return OTF2_WRITER_ERROR_UKNOWN_MPI_GROUP;}

#define UNKNOWN_GROUP_TRAP(group)

#define COMM_LAMBDA_BEGIN() _comm_actions.push({.action = [=]() {

#define COMM_LAMBDA_END() return OTF2_WRITER_SUCCESS; }, .end_time = stop});

#define _ENTER(fname)                         \
  GET_ARCHIVE_CONTEXT(rank)                   \
  logger(OWV_INFO, fname);                    \
  _start_time = std::min(_start_time, start); \
  _stop_time = std::max(_stop_time, stop);    \
  rank = (rank == RANK_UNDEF) ? _rank : rank; \
  auto this_region = _region[string(fname)];  \
  string _fname = fname;                      \
  if (_comm_mode != COMM_MODE_BUILD_COMM) {   \
    OTF2_EvtWriter_Enter(ctx.evt_writer,      \
                         nullptr,             \
                         start,               \
                         this_region);        \
    ctx.event_count++;                        \
  }

#define _LEAVE() {                         \
  if(_comm_mode != COMM_MODE_BUILD_COMM) { \
    OTF2_EvtWriter_Leave(ctx.evt_writer,  \
                         nullptr,         \
                         stop,            \
                         this_region);    \
    ctx.event_count++;                    \
  }                                       \
  return OTF2_WRITER_SUCCESS;}

// All collectives are recorded with the same function in OTF2. Some collectives don't have a root, so some calls need a bogus parameter.
#define UNDEFINED_ROOT int root = -1;

#define COLLECTIVE_WRAPPER(collective, sent, received)                                                    \
  OTF2_EvtWriter_MpiCollectiveBegin(ctx.evt_writer, nullptr, start);                                      \
  OTF2_EvtWriter_MpiCollectiveEnd(ctx.evt_writer, nullptr, stop, collective, comm, root, sent, received); \
  ctx.event_count += 2;

using std::string;
using std::to_string;

namespace dumpi {
  OTF2_Writer::OTF2_Writer() {}

  string OTF2_Writer::get_otf2_directory() {
    return _directory;
  }

  // OTF2 callbacks
  static OTF2_FlushType pre_flush( void* userData, OTF2_FileType fileType, OTF2_LocationRef location, void* callerData, bool final) { return OTF2_FLUSH; }
  static OTF2_TimeStamp post_flush(void* userData, OTF2_FileType fileType, OTF2_LocationRef location) { return 0; }

  OTF2_FlushCallbacks flush_callbacks =
  {
      .otf2_pre_flush  = pre_flush,
      .otf2_post_flush = post_flush
  };

  OTF2_WRITER_RESULT OTF2_Writer::open_archive(string path, int ranks, bool force) {
    if (_archive != nullptr)
      return OTF2_WRITER_ERROR_ARCHIVE_ALREADY_OPEN;

    struct stat sb;

    // test if directory already exists
    if (stat(path.c_str(), &sb) == 0 )
        return OTF2_WRITER_ERROR_DIRECTORY_ALREADY_EXISTS;

    mk_archive_dir(path.c_str());

    // test if parent directory exists
    if (stat((path + "/..").c_str(), &sb) && !force)
      return OTF2_WRITER_ERROR_DIRECTORY_ALREADY_EXISTS;

    _num_ranks = ranks;
    _directory = path;
    _archive = OTF2_Archive_Open(  _directory.c_str(),
                                   "traces",
                                   OTF2_FILEMODE_WRITE,
                                   1024 * 1024 /* event chunk size */,
                                   4 * 1024 * 1024 /* def chunk size */,
                                   OTF2_SUBSTRATE_POSIX,
                                   OTF2_COMPRESSION_NONE );

    OTF2_Archive_SetSerialCollectiveCallbacks(_archive);
    OTF2_Archive_SetFlushCallbacks( _archive, &flush_callbacks, NULL );
    OTF2_Archive_OpenEvtFiles(_archive);

    return OTF2_WRITER_SUCCESS;
  }

  int OTF2_Writer::mk_archive_dir(const char *path) {
    const size_t len = strlen(path);
    char _path[PATH_MAX];
    char *p;

    errno = 0;

    if (len > sizeof(_path)-1) {
      errno = ENAMETOOLONG;
      return -1;
    }
    strcpy(_path, path);

    for (p = _path + 1; *p; p++) {
      if (*p == '/') {
        *p = '\0';

        if (mkdir(_path, S_IRWXU) != 0) {
          if (errno != EEXIST)
            return -1;
        }
        *p = '/';
      }
    }

    return (mkdir(_path, S_IRWXU) != 0 && errno != EEXIST) ? -1 : 0;
  }

  // lousy logger implementation
  void OTF2_Writer::logger(OTF2_WRITER_VERBOSITY level, string msg) {
    logger(level, msg.c_str());
  }

  void OTF2_Writer::logger(OTF2_WRITER_VERBOSITY level, const char* msg) {
    if (_verbosity >= level && _verbosity != OWV_NONE) {
      const char* prepender = "";
      switch (level) {
        case OWV_ABORT:
        prepender = "OTF2 WRITER (ABRT): ";
        break;
        case OWV_ERROR:
        prepender = "OTF2 WRITER  (ERR): ";
        break;
        case OWV_WARN:
        prepender = "OTF2 WRITER  (WRN): ";
        break;
        case OWV_INFO:
        prepender = "OTF2 WRITER (INFO): ";
        break;
        default:
        prepender = "OTF2 WRITER  (UNK): ";
        break;
      }
      printf("%s%s\n", prepender, msg);
      fflush(stdout);
    }
  }

  RankContext& OTF2_Writer::fetch_context(int new_rank) {
    if (new_rank >= _num_ranks || new_rank < -1)
      logger(OWV_ERROR, string("Rank out of bounds: ") + to_string(new_rank));
    else if (new_rank == -1)
      // Use value from last call to 'set_rank()'
      return _archive_context[_rank];

    RankContext& ctx = _archive_context[new_rank];
    if (ctx.rank == -1) {
      // When has not been initialized yet
      ctx.rank = new_rank;
      ctx.evt_writer = OTF2_Archive_GetEvtWriter(_archive, new_rank);
    }
    return ctx;
  }

  void OTF2_Writer::check_otf2(OTF2_ErrorCode status, const char* description) {
    if ( status != OTF2_SUCCESS ) {
      auto msg = string("OTF2 Error: ");
      msg += description;
      msg += "\n\t ";
      msg += OTF2_Error_GetDescription(status);
      logger(OWV_ERROR, msg);
    }
  }

  void OTF2_Writer::write_def_files() {
    // OTF2 Reference Example
    // https://silc.zih.tu-dresden.de/otf2-current/group__usage__writing__mpi.htm

    // See ScoreP source for a local def writer in action
    //  vendor/otf2/src/tools/otf2_trace_gen/otf2_trace_gen.c

    check_otf2(OTF2_Archive_OpenDefFiles(_archive), "Opening definition files");
    OTF2_GlobalDefWriter* defwriter = OTF2_Archive_GetGlobalDefWriter(_archive);

    unwind_comm_events();

    if (_unknown_comms.size() > 0) {
      string str = "Unknown communicators: [ ";
      for (auto c = _unknown_comms.begin(); c != _unknown_comms.end(); c++) str += to_string(*c) + " ";
      str += "]";
      logger(OWV_ERROR, str);
    }

    if (_clock_resolution == 0) logger(OWV_ERROR, "Clock Resolution not set, use 'set_clock_resolution() to set ticks per second'");

    check_otf2(OTF2_GlobalDefWriter_WriteClockProperties( defwriter,
                                                          _clock_resolution,
                                                          _start_time,
                                                          _stop_time - _start_time), "Writing clock properties to global def file");

    // Strings must come first in the def file
    // Otherwise tools like otf2_print will report errors, even though all of the information is available
    int EMPTY_STRING = _string[""];
    _string["MPI"];
    _string["MPI_COMM_WORLD"];
    _string["MPI_COMM_SELF"];
    _string["LOCATIONS_GROUP"];

    for(int rank = 0; rank < _num_ranks; rank++) {
      _string[string("MPI Rank ") + to_string(rank)];
      _string[string("Master Thread ") + to_string(rank)];
    }

    for(int i = 0; i < _region.size(); i++) _string[_region[i]];
    for(auto comm_it = _mpi_comm.begin(); comm_it != _mpi_comm.end(); comm_it++) _string[comm_it->second.name];

    // STRINGS
    logger(OWV_INFO, "Writing STRINGs to the def file");
    for(int i = 0; i < _string.size(); i++) {
      check_otf2(OTF2_GlobalDefWriter_WriteString(defwriter, i, _string[i].c_str()), "Writing string to global def file");
    }

    //PARADIGM
    logger(OWV_INFO, "Writing PARADIGMs to the def file");
    check_otf2(OTF2_GlobalDefWriter_WriteParadigm( defwriter,
                                        OTF2_PARADIGM_MPI,
                                        _string["MPI"],
                                        OTF2_PARADIGM_CLASS_PROCESS),
                                        "Writing Paradigm to global def file");

    // REGIONS
    logger(OWV_INFO, "Writing REGIONs to the def file");
    for(int i = 0; i < _region.size(); i++) {
      string region_name = _region[i];
      auto str_id = _string[region_name];
      check_otf2(OTF2_GlobalDefWriter_WriteRegion(defwriter,
                                       i                             /* id */,
                                       str_id                        /* region name  */,
                                       str_id                        /* alternative name */,
                                       0                             /* description */,
                                       OTF2_REGION_ROLE_FUNCTION,
                                       OTF2_PARADIGM_MPI,
                                       OTF2_REGION_FLAG_NONE,
                                       0                             /* source file */,
                                       0                             /* begin lno */,
                                       0                             /* end lno */ ), "Writing Region to global def file");
    }

    // SYSTEM_TREE_NODE
    logger(OWV_INFO, "Writing SYSTEM_TREE_NODE to the def file");
    check_otf2(OTF2_GlobalDefWriter_WriteSystemTreeNode( defwriter,
                                              0 /* id */,
                                              0 /* name */,
                                              0 /* class */,
                                              OTF2_UNDEFINED_SYSTEM_TREE_NODE /* parent */ ),
                                              "Writing SystemTreeNode to global def file");

    // LOCATION_GROUP
    logger(OWV_INFO, "Writing LOCATION_GROUPs to the def file");
    // locationgroup appears to be used to identify computation localities. For example, all of the threads on a given node.
    for (int i = 0; i < _num_ranks; i++) {
      char rank_name[32];
      sprintf(rank_name, "MPI Rank %d", i);
      check_otf2(OTF2_GlobalDefWriter_WriteLocationGroup(defwriter,
                                              i,
                                              _string[rank_name],
                                              OTF2_LOCATION_GROUP_TYPE_PROCESS,
                                              0 /* This should point to the node this rank ran on. Not necessary for sst/macro trace replay.*/),
                                              "Writing Location Group to global def file");
    }

    // LOCATION
    logger(OWV_INFO, "Writing LOCATIONs to the def file");
    // Each location will identifies a rank. Must come before groups.
    for (int i = 0; i < _num_ranks; i++) {
      char loc_name[32];
      sprintf(loc_name, "Master Thread %d", i);
      check_otf2(OTF2_GlobalDefWriter_WriteLocation(defwriter,
                                         i,
                                         _string[loc_name],
                                         OTF2_LOCATION_TYPE_CPU_THREAD,
                                         _archive_context[i].event_count,
                                         0),
                                         "Writing Location to global def file");
    }

    // Here we emulate ScoreP's behavior: create a common preamble to every trace, and remap communicators as necesary
    //    GROUP                                0  Name: "" <0>, Type: COMM_LOCATIONS, Paradigm: "MPI" <4>, Flags: NONE, 4 Members: "Master thread" <0>, "Master thread" <1>, "Master thread" <2>, "Master thread" <3>
    //    GROUP                                1  Name: "" <0>, Type: COMM_GROUP, Paradigm: "MPI" <4>, Flags: NONE, 4 Members: 0 ("Master thread" <0>), 1 ("Master thread" <1>), 2 ("Master thread" <2>), 3 ("Master thread" <3>)
    //    GROUP                                2  Name: "" <0>, Type: COMM_SELF, Paradigm: "MPI" <4>, Flags: NONE, 0 Members:
    //    COMM                                 0  Name: "MPI_COMM_WORLD" <329>, Group: "" <1>, Parent: UNDEFINED
    //    COMM                                 1  Name: "MPI_COMM_SELF" <334>, Group: "" <2>, Parent: UNDEFINED

    // GROUP
    logger(OWV_INFO, "Writing GROUPs to the def file");

    if (_mpi_group.size() == 0)
      logger(OWV_ERROR, "No communicator groups registered, did you forget to call register_comm_world() and register_comm_self()");

    // ScoreP behavior: Each COMM points to a GROUP def of type OTF2_GROUP_TYPE_COMM_GROUP.
    // Althout the GROUP defs enumerate their ranks, Each must point to a root group of type OTF2_GROUP_TYPE_COMM_LOCATIONS.
    // ScoreP does not seem to nest groups, instead flattening each to the global LOCATIONS group that points directly to the COMM_LOCATIONS group.
    uint64_t* world_list = new uint64_t[_num_ranks];
    for (int i = 0; i < _num_ranks; i++)  world_list[i] = i;

    check_otf2(OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                    COMM_LOCATIONS_GROUP_ID,
                                    _string["LOCATIONS_GROUP"],
                                    OTF2_GROUP_TYPE_COMM_LOCATIONS,
                                    OTF2_PARADIGM_MPI,
                                    OTF2_GROUP_FLAG_NONE,
                                    _num_ranks,
                                    world_list),
                                    "Writing Locations Group to global def file");

    delete[] world_list;

    // The runtime will create duplicate comm groups, only use the ones referenced by communicators

    // Having non-contiguous group (integer) ID's makes otf2-print dump warnings. "otf2-print: warning: out of order Comm definition"
    // At this point all of the comm's and groups have been realized, and a last second remapping will solve the above warning
    std::map<int, int> group_id_map;
    int group_map_inc = 1;

    //for(auto group_it = _mpi_group.begin(); group_it != _mpi_group.end(); group_it++) {
    for(auto comm_it = _mpi_comm.begin(); comm_it != _mpi_comm.end(); comm_it++) {
      auto group_id = comm_it->second.group;
      auto& members = _mpi_group[group_id];
      group_id_map[group_id] = group_map_inc;
      uint64_t* group_list = new uint64_t[members.size()];
      for (int i = 0; i < members.size(); i++)  group_list[i] = (uint64_t)members[i];

      check_otf2(OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                      //group_it->first + USER_DEF_COMM_GROUP_OFFSET,
                                      group_map_inc++,
                                      EMPTY_STRING,
                                      OTF2_GROUP_TYPE_COMM_GROUP,
                                      OTF2_PARADIGM_MPI,
                                      OTF2_GROUP_FLAG_NONE,
                                      members.size(),
                                      group_list),
                                      "Writing group to global def file");

      delete [] group_list;
    }

    // COMM
    logger(OWV_INFO, "Writing COMMs to the def file");

    if (_comm_world_id == -1)
      logger(OWV_ERROR, "register_comm_world() not called");

    if (_comm_self_id == -1)
      logger(OWV_ERROR, "register_comm_self() not called");

    if (_comm_error_id == -1)
      logger(OWV_ERROR, "register_comm_error() not called");

    if (_comm_null_id == -1)
      logger(OWV_ERROR, "register_comm_null() not called");

    for(auto comm_it = _mpi_comm.begin(); comm_it != _mpi_comm.end(); comm_it++) {
      int comm_id = comm_it->first;
      MPI_Comm_Struct& comm = comm_it->second;

      check_otf2(OTF2_GlobalDefWriter_WriteComm( defwriter,
                                      comm_id,
                                      _string[comm.name],
                                      group_id_map[comm.group],
                                      comm.parent),
                                      "Writing a Communicator to global def file");
    }

    // local communicator -> global communicator mapping
    // Even without mapping information, this step is essential for creating local def files, which are required by a valid trace.
    for(int r = 0; r < _num_ranks; r++) {
      OTF2_DefWriter* local_def_writer = OTF2_Archive_GetDefWriter(_archive, r);

      // Getting rank-local metadata
      auto& rank_context = fetch_context(r);

      // initialize local COMM mappings table, include space for MPI_COMM_WORLD, MPI_COMM_SELF, and any user-defined communicators.
      OTF2_IdMap* _mpi_comm_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, rank_context.comm_mapping.size());

      // "comm_mapping" is populated on every rank in a registered communicator
      for(auto cm_it = rank_context.comm_mapping.begin(); cm_it != rank_context.comm_mapping.end(); cm_it++)
        check_otf2(OTF2_IdMap_AddIdPair(_mpi_comm_map, cm_it->first, cm_it->second), "Adding a communicator to the Def mapping list");

      // Serialize the table to a local def file
      check_otf2(OTF2_DefWriter_WriteMappingTable( local_def_writer,
                                        OTF2_MAPPING_COMM,
                                        _mpi_comm_map ),
                                        "Writing a communicator mapping table to the OTF2 archive");

      OTF2_IdMap_Free(_mpi_comm_map);
      check_otf2(OTF2_Archive_CloseDefWriter(_archive, local_def_writer), "Closing a local def writer");
    }

    check_otf2(OTF2_Archive_CloseDefFiles(_archive), "Closing definition files");
  }

  void OTF2_Writer::unwind_comm_events() {
    while(_comm_actions.size() > 0) {
      _comm_actions.top().action();
      _comm_actions.pop();
    }
  }

  OTF2_WRITER_RESULT OTF2_Writer::close_archive() {

    write_def_files();

    // Clean up the event archive for each rank
    for( auto ac_it = _archive_context.begin(); ac_it != _archive_context.end(); ac_it++) {
      int rank = ac_it->second.rank;
      int incomplete = ac_it->second.dispose(_archive);
      if (incomplete > 0) {
        logger(OWV_WARN, string("") + string("Closing archive with ") + to_string(incomplete) + string(" incomplete MPI calls on rank ") + to_string(rank));
      }
    }

    _archive_context.clear();

    check_otf2(OTF2_Archive_CloseEvtFiles(_archive), "Closing all event files");
    check_otf2(OTF2_Archive_Close(_archive), "Closing archive");

    _archive = nullptr;

    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::register_group(int id, std::vector<int> list) {
    _mpi_group[id] = list;
    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::register_comm(string name, int id, int parent, int group) {
    MPI_Comm_Struct& mcs = _mpi_comm[id];
    mcs.id = id;
    mcs.group = group;
    mcs.parent = parent;
    mcs.name = name;
    mcs.ndim = 0;
    mcs.name = "";

    return OTF2_WRITER_SUCCESS;
  }

  void OTF2_Writer::register_comm_world(comm_t id) {
    std::vector<int> w_group;
    for(int i = 0; i < _num_ranks; i++) {
      w_group.push_back(i);
      fetch_context(i).comm_mapping[id] = MPI_COMM_WORLD_ID;
    }
    _mpi_group[COMM_WORLD_GROUP_ID] = w_group;
    _comm_world_id = id;

    auto& comm = _mpi_comm[MPI_COMM_WORLD_ID];
    comm.parent=OTF2_UNDEFINED_COMM;
    comm.id=MPI_COMM_WORLD_ID;
    comm.group=(int)COMM_WORLD_GROUP_ID;
    comm.name="MPI_COMM_WORLD";

    _unknown_comms.erase(id);
  }

  void OTF2_Writer::register_comm_self(comm_t id) {
    std::vector<int> s_group = {0};
    for(int i = 0; i < _num_ranks; i++) {
      fetch_context(i).comm_mapping[id] = MPI_COMM_SELF_ID;
    }

    _mpi_group[COMM_SELF_GROUP_ID] = s_group;
    _comm_self_id = id;

    auto& comm = _mpi_comm[MPI_COMM_SELF_ID];
    comm.parent=OTF2_UNDEFINED_COMM;
    comm.id=MPI_COMM_SELF_ID;
    comm.group=(int)COMM_SELF_GROUP_ID;
    comm.name="MPI_COMM_SELF";

    _unknown_comms.erase(id);
  }

  void OTF2_Writer::register_comm_error(comm_t id) {
    _comm_error_id = id;
  }

  void OTF2_Writer::register_comm_null(comm_t id) {
    _comm_null_id = id;
  }

  void OTF2_Writer::register_null_request(request_t request) {
    _null_request = request;
  }

  void OTF2_Writer::set_verbosity(OTF2_WRITER_VERBOSITY verbosity) {
    _verbosity = verbosity;
  }

  void OTF2_Writer::set_clock_resolution(uint64_t ticks_per_second) {
    _clock_resolution = ticks_per_second;
  }

  // TODO type sizes should be rank specific.
  void OTF2_Writer::register_type(mpi_type_t type, int size) {
    _type_sizes[type] = size;
  }

  OTF2_WRITER_RESULT OTF2_Writer::set_rank(int rank) {
    CHECK_RANK(rank, _num_ranks);

    // set the new rank
    this->_rank = rank;

    // Open OTF2 event file for new rank
    fetch_context(rank).evt_writer = OTF2_Archive_GetEvtWriter(_archive, rank);

    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_send_inner(RankContext& ctx, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    OTF2_EvtWriter_MpiSend(ctx.evt_writer, nullptr, start, dest, comm, tag, _type_sizes[type]*count);
    ctx.event_count++;
    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_send(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Send");
    mpi_send_inner(ctx, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_bsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Bsend");
    mpi_send_inner(ctx, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_ssend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Ssend");
    mpi_send_inner(ctx, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_rsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Rsend");
    mpi_send_inner(ctx, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_recv(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag) {
    _ENTER("MPI_Recv");
    OTF2_EvtWriter_MpiRecv(ctx.evt_writer, nullptr, start, source, comm, tag, count_bytes(type, count));
    ctx.event_count++;
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_isend_inner(RankContext& ctx, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    ctx.incomplete_call(request, REQUEST_TYPE_ISEND);
    OTF2_EvtWriter_MpiIsend(ctx.evt_writer, nullptr, start, dest, comm, tag, count_bytes(type, count), request);
    ctx.event_count++;
    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_isend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Isend");
    mpi_isend_inner(ctx, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_ibsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Ibsend");
    mpi_isend_inner(ctx, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_issend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Issend");
    mpi_isend_inner(ctx, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_irsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Irsend");
    mpi_isend_inner(ctx, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_irecv(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Irecv");
    ctx.irecv_requests[request] = {count, type, source, tag, comm, request};
    ctx.incomplete_call(request, REQUEST_TYPE_IRECV);
    OTF2_EvtWriter_MpiIrecvRequest(ctx.evt_writer, nullptr, start, request);
    ctx.event_count++;
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::generic_call(int rank, otf2_time_t start, otf2_time_t stop, string name) {
    _ENTER(name);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_wait(int rank, otf2_time_t start, otf2_time_t stop, request_t request) {
    _ENTER("MPI_Wait");
    ctx.complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitany(int rank, otf2_time_t start, otf2_time_t stop, request_t request) {
    _ENTER("MPI_Waitany");
    ctx.complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitall(int rank, otf2_time_t start, otf2_time_t stop, int count, const request_t* requests) {
    _ENTER("MPI_Waitall");
    std::unordered_set<request_t> called;
    for(int i = 0; i < count; i++) {
      auto req = requests[i];
      if (req != _null_request && called.find(req) == called.end()) {
        ctx.complete_call(req, start);
        called.insert(req);
      }
    }
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitsome(int rank, otf2_time_t start, otf2_time_t stop, const request_t* requests, int outcount, const int* indices) {
    _ENTER("MPI_Waitsome");
    for (int i = 0; i < outcount; i++)
      ctx.complete_call(requests[indices[i]], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_test(int rank, otf2_time_t start, otf2_time_t stop, request_t request, int flag) {
    _ENTER("MPI_Test");
    if (flag)
      ctx.complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testany(int rank, otf2_time_t start, otf2_time_t stop, const request_t* requests, int index, int flag) {
    _ENTER("MPI_Testany");
    if (flag)
      ctx.complete_call(requests[index], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testall(int rank, otf2_time_t start, otf2_time_t stop, int count, const request_t* requests, int flag) {
    _ENTER("MPI_Testall");
    if (flag)
      for (int i = 0; i < count; i++)
        ctx.complete_call(requests[i], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testsome(int rank, otf2_time_t start, otf2_time_t stop, const request_t* requests, int outcount, const int* indices) {
    _ENTER("MPI_Testsome");
    for (int i = 0; i < outcount; i++)
      ctx.complete_call(requests[indices[i]], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_barrier(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm) {
    _ENTER("MPI_Barrier");
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_BARRIER, 0, 0);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_bcast(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, int root, bool is_root, comm_t comm) {
    _ENTER("MPI_Bcast");

    if (_comm_mode == COMM_MODE_NONE)
      is_root = ranks_equivalent(rank, root, comm, &ctx);
    else if (_comm_mode == COMM_MODE_BUILD_COMM)
      return OTF2_WRITER_ERROR_UKNOWN_MPI_COMM;

    int bytes = count_bytes(type, count);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_BCAST,
                       is_root ? bytes : 0,
                       is_root ? 0 : bytes);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_gather(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, bool is_root, comm_t comm) {
    _ENTER("MPI_Gather");

    if (_comm_mode == COMM_MODE_NONE)
      is_root = ranks_equivalent(rank, root, comm, &ctx);
    else if (_comm_mode == COMM_MODE_BUILD_COMM)
      return OTF2_WRITER_ERROR_UKNOWN_MPI_COMM;

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_GATHER,
                       count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_gatherv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, int sendcount, mpi_type_t sendtype, const int* recvcounts, mpi_type_t recvtype, int root, bool is_root, comm_t comm) {
    _ENTER("MPI_Gatherv");

    int send_count = 0;
    int recv_count = 0;

    if (_comm_mode == COMM_MODE_NONE)
      is_root = ranks_equivalent(rank, root, comm, &ctx);
    else if (_comm_mode == COMM_MODE_BUILD_COMM)
      return OTF2_WRITER_ERROR_UKNOWN_MPI_COMM;

    if (is_root)
      recv_count = count_bytes(recvtype, array_sum(recvcounts, comm_size));
    else
      send_count = count_bytes(sendtype, sendcount);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_GATHERV,
                       send_count,
                       recv_count);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scatter(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, bool is_root, comm_t comm) {
    _ENTER("MPI_Scatter");

    if (_comm_mode == COMM_MODE_NONE)
      is_root = ranks_equivalent(rank, root, comm, &ctx);
    else if (_comm_mode == COMM_MODE_BUILD_COMM)
      return OTF2_WRITER_ERROR_UKNOWN_MPI_COMM;

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCATTER,
                       is_root ? count_bytes(sendtype, sendcount) : 0,
                       is_root ? 0 : count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scatterv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, const int* sendcounts, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, bool is_root, comm_t comm) {
    _ENTER("MPI_Scatterv");
    int send_count = 0;
    int recv_count = 0;

    if (_comm_mode == COMM_MODE_NONE)
      is_root = ranks_equivalent(rank, root, comm, &ctx);
    else if (_comm_mode == COMM_MODE_BUILD_COMM)
      return OTF2_WRITER_ERROR_UKNOWN_MPI_COMM;

    if (is_root)
      send_count = count_bytes(sendtype, array_sum(sendcounts, comm_size));
    else
      recv_count = count_bytes(recvtype, recvcount);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCATTERV,
                       send_count,
                       recv_count);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scan(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t datatype, comm_t comm) {
    _ENTER("MPI_Scan");
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCAN,
                       count_bytes(datatype, count),
                       count_bytes(datatype, count));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allgather(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Allgather");
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLGATHER,
                       count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allgatherv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, int sendcount, mpi_type_t sendtype, const int* recvcounts, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Allgatherv");
    UNDEFINED_ROOT

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLGATHERV,
                       count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, array_sum(recvcounts, comm_size)));
    _LEAVE();
  }
  OTF2_WRITER_RESULT OTF2_Writer::mpi_alltoall(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Alltoall");
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLTOALL,
                       count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_alltoallv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, const int* sendcounts, mpi_type_t sendtype, const int* recvcounts, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Alltoallv");
    UNDEFINED_ROOT

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLTOALLV,
                       count_bytes(sendtype, array_sum(sendcounts, comm_size)),
                       count_bytes(recvtype, array_sum(recvcounts, comm_size)));
    _LEAVE();
  }

  // TODO need to know if root
  OTF2_WRITER_RESULT OTF2_Writer::mpi_reduce(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, int root, comm_t comm) {
    _ENTER("MPI_Reduce");

    int sent = count_bytes(type, count);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_REDUCE,
                       sent,
                       ranks_equivalent(rank, root, comm, &ctx) ? sent : 0);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allreduce(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, comm_t comm) {
    _ENTER("MPI_Allreduce");
    UNDEFINED_ROOT
    int bytes = count_bytes(type, count);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLREDUCE,
                       bytes,
                       bytes);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_reduce_scatter(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, const int* recvcounts, mpi_type_t type, comm_t comm) {
    _ENTER("MPI_Reduce_scatter");
    UNDEFINED_ROOT

    //comm_size = (comm_size == COMM_SIZE_NONE) ? get_comm_size(comm): comm_size;

    // TODO: double check the byte counts are accurate
    int bytes = count_bytes(type, array_sum(recvcounts, comm_size));

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_REDUCE_SCATTER,
                       bytes,
                       bytes);
    _LEAVE();
  }

  std::vector<int>& OTF2_Writer::get_group(comm_t comm) {
    return _mpi_group[_mpi_comm[comm].group];
  }

  bool OTF2_Writer::group_is_known(int group) {
    if (_mpi_group.find(group) == _mpi_group.end()) {
      logger(OWV_WARN, string("Unknown group (") + to_string(group) + ")");
      return false;
    } else return true;
  }

  // TODO wrap in lambda
  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_union(int rank, otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_union");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    COMM_LAMBDA_BEGIN();
    {
      GET_ARCHIVE_CONTEXT(rank);
      auto g1_it = _mpi_group[ctx.group_map[group1]].begin();
      auto g2_it = _mpi_group[ctx.group_map[group2]].begin();
      auto g1_end = _mpi_group[ctx.group_map[group1]].end();
      auto g2_end = _mpi_group[ctx.group_map[group2]].end();
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      auto dest = std::back_inserter(_mpi_group[global_id]);

      ctx.group_map[newgroup] = global_id;

      // Set Union
      while (true)
      {
        if (g1_it==g1_end) { std::copy(g2_it,g2_end,dest); break; }
        if (g2_it==g2_end) { std::copy(g1_it,g1_end,dest); break; }

        if (*g1_it<*g2_it) { *dest = *g1_it; ++g1_it; }
        else if (*g2_it<*g1_it) { *dest = *g2_it; ++g2_it; }
        else { *dest = *g1_it; ++g1_it; ++g2_it; }
        ++dest;
      }
    }
    COMM_LAMBDA_END();
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_difference(int rank, otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_difference");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    COMM_LAMBDA_BEGIN();
    {
      GET_ARCHIVE_CONTEXT(rank);
      auto g1_it = _mpi_group[ctx.group_map[group1]].begin();
      auto g2_it = _mpi_group[ctx.group_map[group2]].begin();
      auto g1_end = _mpi_group[ctx.group_map[group1]].end();
      auto g2_end = _mpi_group[ctx.group_map[group2]].end();
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      auto dest = std::back_inserter(_mpi_group[global_id]);

      ctx.group_map[newgroup] = global_id;

      // Set Difference
      while (g1_it != g1_end) {
        if (g2_it == g2_end) { std::copy(g1_it, g1_end, dest); break; }

        if (*g1_it < *g2_it) {
          *dest++ = *g1_it++;
        } else {
          if (! (*g2_it < *g1_it)) {
            ++g1_it;
          }
          ++g2_it;
        }
      }
    }
    COMM_LAMBDA_END();

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_intersection(int rank, otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_intersection");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    COMM_LAMBDA_BEGIN();
    {
      GET_ARCHIVE_CONTEXT(rank);
      auto g1_it = _mpi_group[ctx.group_map[group1]].begin();
      auto g2_it = _mpi_group[ctx.group_map[group2]].begin();
      auto g1_end = _mpi_group[ctx.group_map[group1]].end();
      auto g2_end = _mpi_group[ctx.group_map[group2]].end();
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      auto dest = std::back_inserter(_mpi_group[global_id]);

      ctx.group_map[newgroup] = global_id;

      // Set Intersection
      while (g1_it!=g1_end && g2_it!=g2_end)
      {
        if (*g1_it<*g2_it) ++g1_it;
        else if (*g2_it<*g1_it) ++g2_it;
        else {
          *dest = *g1_it;
          ++dest; ++g1_it; ++g2_it;
        }
      }
    }
    COMM_LAMBDA_END();

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_incl(int rank, otf2_time_t start, otf2_time_t stop, int group, int count, const int*ranks, int newgroup) {
    _ENTER("MPI_Group_incl");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    std::vector<int> r_vec(ranks, ranks + count);

    COMM_LAMBDA_BEGIN();
    {
      GET_ARCHIVE_CONTEXT(rank);
      auto& old_group = _mpi_group[ctx.group_map[group]];
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      ctx.group_map[newgroup] = global_id;
      auto& new_group = _mpi_group[global_id];

      int g_size = old_group.size();
      for (int i = 0; i < count; i++) {
        if (r_vec[i] >= g_size) logger(OWV_ERROR, string("MPI_Group_incl found ranks out of bounds! " + to_string(ranks[i]) + " >= " + to_string(g_size)));
        else new_group.push_back(old_group[r_vec[i]]);
      }
    }
    COMM_LAMBDA_END();

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_excl(int rank, otf2_time_t start, otf2_time_t stop, int group, int count, const int*ranks, int newgroup) {
    _ENTER("MPI_Group_excl");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    std::vector<int> r_vec(ranks, ranks + count);

    COMM_LAMBDA_BEGIN();
    {
      GET_ARCHIVE_CONTEXT(rank);
      auto& old_group = _mpi_group[ctx.group_map[group]];
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      ctx.group_map[newgroup] = global_id;
      auto& new_group = _mpi_group[global_id];

      for(auto og_it = old_group.begin(); og_it != old_group.end(); og_it++) {
        if(std::find(r_vec.begin(), r_vec.end(), *og_it) == r_vec.end())
          new_group.push_back(*og_it);
      }
    }
    COMM_LAMBDA_END();

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_range_incl(int rank, otf2_time_t start, otf2_time_t stop, int group, int count, int**ranges, int newgroup) {
    _ENTER("MPI_Group_range_incl");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    COMM_LAMBDA_BEGIN();
    {
      GET_ARCHIVE_CONTEXT(rank);
      auto& old_group = _mpi_group[ctx.group_map[group]];
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      ctx.group_map[newgroup] = global_id;
      auto& new_group = _mpi_group[global_id];

      for (int i = 0; i < count; i++)
        for(int j = ranges[i][0]; j < ranges[i][1]; j+=ranges[i][2])
           new_group.push_back(old_group[j]);
    }
    COMM_LAMBDA_END();

    _LEAVE();
  }

  bool OTF2_Writer::comm_is_known(comm_t comm) {
    if (_mpi_comm.find(comm) == _mpi_comm.end()) {
      logger(OWV_WARN, string("Unknown Communicator (") + to_string(comm) + ")");
      return false;
    }
    return true;
  }

  bool OTF2_Writer::type_is_known(mpi_type_t type) {
    if (_type_sizes.find(type) == _type_sizes.end()) {
      logger(OWV_WARN, string("Unknown Type (") + to_string(type));
      return false;
    }
    return true;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_dup(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm, comm_t newcomm) {
    _ENTER("MPI_Comm_dup");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    COMM_LAMBDA_BEGIN();
    {
        // Alias the existing global comm reference.
        // Duplication was intened for library writers to handle asynchronus calls without leaking.
        // Not useful information for endpoint model simulations, but it may affect profiling.
        auto& ctx = fetch_context(rank);
        ctx.comm_mapping[newcomm] = ctx.comm_mapping[comm];
    }
    COMM_LAMBDA_END();

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_group(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm, int group) {
    _ENTER("MPI_Comm_group");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    COMM_LAMBDA_BEGIN();
    {
      GET_ARCHIVE_CONTEXT(rank);

      // The comm points to a group at creation, alias this rank to that one.
      ctx.group_map[group] = _mpi_comm[ctx.comm_mapping[comm]].group;
    }
    COMM_LAMBDA_END();

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_create(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm, int group, comm_t newcomm) {
    _ENTER("MPI_Comm_create");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    if (!COMM_NULL_OR_ERROR(newcomm)) {
      COMM_LAMBDA_BEGIN();
      {
        // Ranks that participate in an MPI_Comm_create but do not fit in the group will return MPI_COMM_NULL
        GET_ARCHIVE_CONTEXT(rank);
        auto& gbl_comm = ctx.comm_mapping[comm];
        auto& group_vect = _mpi_group[ctx.group_map[group]];
        _comm_create_constructor.add_call(rank, get_comm_rank(rank, gbl_comm), gbl_comm, group_vect.size(), hash_group(group_vect), newcomm);

        auto completed = _comm_create_constructor.list_completed();
        for(auto c_it = completed.begin(); c_it != completed.end(); c_it++) {
          // Extract the new communicator's information
          auto c_tup = _comm_create_constructor.get_completed(*c_it);
          MPI_Comm_Struct& mcs = std::get<0>(c_tup);

          // Create the new comm
          auto& nc = _mpi_comm[mcs.id];
          nc = mcs;

          // Assign the group (this rank will have a correct reference)
          nc.group = ctx.group_map[group];

          // Copy over any rank-specific comm id remappings
          auto comm_remap_vect = _comm_create_constructor.get_remapping(mcs.id);
          for(int i = 0; i < comm_remap_vect.size(); i++) {
            //           (world rank id          ).(local comm id                 ) ->(global comm id)
            fetch_context(_mpi_group[nc.group][i]).comm_mapping[comm_remap_vect[i]] = mcs.id;
          }
          _comm_create_constructor.clear(*c_it);
        }
      }
      COMM_LAMBDA_END();
    }

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_split(int rank, otf2_time_t start, otf2_time_t stop, comm_t oldcomm, int key, int color, comm_t newcomm) {
    _ENTER("MPI_Comm_split");

    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      _LEAVE();

    COMM_LAMBDA_BEGIN();
    {
      // turn oldcomm (local communicator id) into a global communicator id
      auto& gbl_oc = fetch_context(rank).comm_mapping[oldcomm];
      _comm_split_constructor.add_call(rank, get_comm_rank(rank, gbl_oc), key, color, gbl_oc, newcomm, get_comm_size(gbl_oc));
      auto completed = _comm_split_constructor.list_completed();

      // Check for finished communicators
      for(auto c_it = completed.begin(); c_it != completed.end(); c_it++) {

        // Extract the new communicator's information
        auto c_tup = _comm_split_constructor.get_completed(*c_it);
        MPI_Comm_Struct& mcs = std::get<0>(c_tup);
        std::vector<int>& group_ranks = std::get<1>(c_tup);

        // Create the comm
        auto& nc = _mpi_comm[mcs.id];
        nc = mcs;

        // Copy the group ranks
        nc.group = MPI_Comm_Struct::get_unique_group_id();
        _mpi_group[nc.group] = group_ranks; // Copy constructor for the list

        // Copy over any rank-specific comm id remappings
        auto comm_remap_vect = _comm_split_constructor.get_remapping(mcs.id);
        for(int i = 0; i < comm_remap_vect.size(); i++) {
          //           (world rank id          ).(local comm id                 ) ->(global comm id)
          fetch_context(_mpi_group[nc.group][i]).comm_mapping[comm_remap_vect[i]] = mcs.id;
        }

        // Clean up the split constructor
        _comm_split_constructor.clear(mcs.id);
      }
    }
    COMM_LAMBDA_END();
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_contiguous(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_contiguous");

    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string("MPI_type_contiguous failed"));
      return OTF2_WRITER_ERROR_UKNOWN_MPI_TYPE;
    }

    _type_sizes[newtype] = count_bytes(oldtype, count);

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_hvector(int rank, otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hvector");
    mpi_t_vector_inner("MPI_Type_hvector", count, blocklength, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_vector(int rank, otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_vector");
    mpi_t_vector_inner("MPI_Type_vector", count, blocklength, oldtype, newtype);
    _LEAVE();
  }

  void OTF2_Writer::mpi_t_vector_inner(const char* fname, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string(fname) + " failed");
      return;
    }
    _type_sizes[newtype] = count_bytes(oldtype, blocklength) * count;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_indexed(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_indexed");
    mpi_t_indexed_inner("MPI_Type_indexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_hindexed(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hindexed");
    mpi_t_indexed_inner("MPI_Type_hindexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_hindexed(int rank, otf2_time_t start, otf2_time_t stop, int count, const int*lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hindexed");
    mpi_t_indexed_inner("MPI_Type_hindexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  void OTF2_Writer::mpi_t_indexed_inner(const char* name, int count, const int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string(name) + " failed");
      return;
    }

    _type_sizes[newtype] = count_bytes(oldtype, array_sum(lengths, count));
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_struct(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype) {
    _ENTER("MPI_Type_struct");
    mpi_t_struct_inner("MPI_Type_struct", count, blocklengths, oldtypes, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_struct(int rank, otf2_time_t start, otf2_time_t stop, int count, const int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype) {
    _ENTER("MPI_Type_create_struct");
    mpi_t_struct_inner("MPI_Type_create_struct", count, blocklengths, oldtypes, newtype);
    _LEAVE();
  }

  void OTF2_Writer::mpi_t_struct_inner(const char* fname, int count, const int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype) {
    int sum = 0;
    for(int i = 0; i < count; i++) {
      if (!type_is_known(oldtypes[i])){
        logger(OWV_ERROR, string(fname) + "failed");
        return;
      }
      sum += count_bytes(oldtypes[i], blocklengths[i]);
    }

    _type_sizes[newtype] = sum;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_subarray(int rank, otf2_time_t start, otf2_time_t stop, int ndims, const int* subsizes, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_create_subarray");

    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string("MPI_Type_create_subarray failed"));
      return OTF2_WRITER_ERROR_UKNOWN_MPI_TYPE;
    }

    _type_sizes[newtype] = count_bytes(oldtype, array_sum(subsizes, ndims));

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_hvector(int rank, otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_create_hvector");
    mpi_t_vector_inner("MPI_Type_create_hvector", count, blocklength, oldtype, newtype);
    _LEAVE();
  }

  template<typename T>
  int OTF2_Writer::array_sum(const T* array, int count) {
    int sum = 0;
    for(int i = 0; i < count; i++) sum += array[i];
    return sum;
  }

  bool OTF2_Writer::ranks_equivalent(int world_rank, int comm_rank, comm_t comm, RankContext* ctx) {

    // When the mapping is done on a context, translate the communicator into a global reference
    if (ctx != nullptr) {
      if (ctx->comm_mapping.find(comm) == ctx->comm_mapping.end()) {
        logger(OWV_WARN, "Rank " + to_string(ctx->rank) + " does not have a comm mapping for " + to_string(comm));
        return false;
      }
      comm = ctx->comm_mapping[comm];
    }

    if (!comm_is_known(comm)) {
      logger(OWV_WARN, string("Cannot compare ranks for an unknown communicator"));
      return false;
    }

    auto& group = get_group(comm);

    if (comm_rank < 0 || comm_rank >= group.size()) {
      logger(OWV_WARN, string("Comm rank out of range!"));
      return false;
    }

    return group[comm_rank] == world_rank;
  }

  int OTF2_Writer::get_comm_rank(int world_rank, comm_t comm) {
    auto& rank_list = get_group(comm);

    // slow, linear search...
    auto rank =  std::find(rank_list.begin(), rank_list.end(), world_rank);
    if (rank == rank_list.end()) return -1; // WE HAVE A PROBLEM
    else return rank - rank_list.begin();
  }

  int OTF2_Writer::get_comm_size(comm_t comm) {
    auto c = _mpi_comm[comm];
    if (c.ndim == 0)
      return _mpi_group[c.group].size();
    else {
      int sum = 0;
      for(int i = 0; i < c.ndim; i++) sum *= c.dims[i];
      return sum;
    }
  }

  uint64_t OTF2_Writer::count_bytes(mpi_type_t type, uint64_t count){
   if (_type_sizes.find(type) == _type_sizes.end()) {
     logger(OWV_ERROR, string("Unkown data type (") + to_string((int)type) + "). Assuming 4 bytes in size.");
     return 4*count;
   } else return _type_sizes[type]*count;
  }

  unsigned long OTF2_Writer::hash_group(std::vector<int>& group) {
    unsigned long hash = ~0;
    std::hash<int> hasher;

    for(auto rank : group)
      hash ^= hasher(rank) + 0x9e3779b9 + (hash<<6) + (hash>>2);

    return hash;
  }

  OTF2DefTable::OTF2DefTable() {}
  int OTF2DefTable::map(string string)
  {
    auto it = _map.find(string);
    _added_last_lookup = false;
    if ( it != _map.end()) return it->second;
    _map.insert(std::make_pair(string, _counter));
    _added_last_lookup = true;
    return _counter++;
  }

  const string OTF2DefTable::map(int index) {
    // Finds the string corresponding to the given int.
    for (auto elem = _map.begin(); elem != _map.end(); elem++)
      if (elem->second == index)
        return elem->first;
    return string();
  }

  const string OTF2DefTable::operator[] (int index) { return map(index); }
  int OTF2DefTable::operator[] (const char* index) { return map(string(index));}
  int OTF2DefTable::operator[] (string string) { return map(string); }
  int OTF2DefTable::size() { return _counter; }
  bool OTF2DefTable::added_last_lookup() {return _added_last_lookup;}

  int RankContext::dispose(OTF2_Archive* archive) {
    // close existing OTF2 event file if it is open
    if (evt_writer != nullptr) {
      OTF2_Archive_CloseEvtWriter(archive, evt_writer);
      evt_writer = nullptr;
    }

    rank = -1;
    event_count = 0;
    int incomplete_count = irecv_requests.size();
    irecv_requests.clear();
    request_type.clear();
    return incomplete_count;
  }

  // I-event handling
  void RankContext::incomplete_call(int request_id, REQUEST_TYPE type) {
    request_type[request_id] = type;
  }

  /*
   * ISends and IRecvs begin when invoked, but important information about
   * them are not recorded by OTF2 until they are completed, which occurs inside of Waits, Tests, etc.
   * Returns the number of events generated
   */
  void RankContext::complete_call(request_t request_id, uint64_t timestamp) {
    auto t = request_type.find(request_id);
    if (t == request_type.end()) {
      printf("Error: request id (%i) not found\n", request_id);
    }
    else if (t->second == REQUEST_TYPE_ISEND) {
      OTF2_EvtWriter_MpiIsendComplete(evt_writer, nullptr, timestamp, request_id);
      event_count++;
    }
    else if (t->second == REQUEST_TYPE_IRECV) {
      auto irecv_it = irecv_requests.find(request_id);
      irecv_capture irecv = irecv_it->second;
      if(irecv_it == irecv_requests.end()) {
        printf("Error: Request #(%i) not found while trying to complete MPI_IRecv\n", request_id);
      } else {
        OTF2_EvtWriter_MpiIrecv(evt_writer, nullptr, timestamp, irecv.source, irecv.comm, irecv.tag, irecv.count, request_id);
        irecv_requests.erase(irecv_it);
        event_count++;
      }
    }
    request_type.erase(t);
  }

  std::tuple<MPI_Comm_Struct, std::vector<int>> CommSplitConstructor::get_completed(comm_t comm) {

    std::vector<int> out;
    auto comms_it = new_comm_group.find(comm);

    if (comms_it != new_comm_group.end()) {
      auto& comm = comms_it->second;
      for(auto comm_it = comm.begin(); comm_it != comm.end(); comm_it++)
        out.push_back(comm_it->global_rank);
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
      for (auto child_comms = s_context_ptr->color_to_comm_id.begin(); child_comms != s_context_ptr->color_to_comm_id.end(); child_comms++)
        completed.insert(child_comms->second);
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

  void OTF2_Writer::set_comm_mode(COMM_MODE comm_mode) {
    if (_comm_mode == COMM_MODE_BUILD_COMM && comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)
      // This step realizes communicators and groups whose information is cached lambda queues.
      // The queues are necessary because comms and groups are dependent on collectives, which
      // may require all trace files to be read.
      unwind_comm_events();

    _comm_mode = comm_mode;
  }

  int MPI_Comm_Struct::_comm_uid = OTF2_Writer::MPI_COMM_USER_OFFSET;
  int MPI_Comm_Struct::_group_uid = OTF2_Writer::USER_DEF_COMM_GROUP_OFFSET;
}

namespace std {
  bool operator==(const dumpi::CommEventIdentifier& rhs, const dumpi::CommEventIdentifier& lhs){
    return lhs.id == rhs.id && lhs.event_number == rhs.event_number && lhs.comm_event_type == rhs.comm_event_type;
  }

  bool operator==(const dumpi::CommCreateIdentifier& rhs, const dumpi::CommCreateIdentifier& lhs){
    return lhs.id == rhs.id && lhs.event_number == rhs.event_number && lhs.group_hash == rhs.group_hash;
  }
}
