#include <dumpi/libotf2dump/otf2writer.h>
#include <otf2/otf2.h>
#include <sys/stat.h>
#include <algorithm>
#include <limits.h>
#include <cstring>
#include <unordered_set>

#define FIXME_COMM_SELF 1000

#define CHECK_RANK(rank, _num_ranks) \
  if (rank < 0 || rank >= _num_ranks) return OTF2_WRITER_ERROR_NO_RANK_SET;

#define CHECK_EVT_WRITER() \
  if (_evt_writer == nullptr) return OTF2_WRITER_ERROR_WRITER_NOT_SET;

#define CHECK_COMM() \
  if (comm != _comm_world_id && _mpi_comm.find(comm) == _mpi_comm.end()) _unknown_comms.insert(comm);

// immediately returns an error code when the comm is unknown
#define UNKNOWN_COMM_TRAP()                                \
  if (!comm_is_known(comm)) {                              \
    logger(OWV_ERROR, _fname + " failed");                 \
    return OTF2_WRITER_ERROR_UKNOWN_MPI_COMM;}

// immediately returns an error code when the group is unknown
#define UNKNOWN_GROUP_TRAP(group)              \
  if (!group_is_known(group)) {                \
    logger(OWV_ERROR, _fname + " failed");     \
    return OTF2_WRITER_ERROR_UKNOWN_MPI_GROUP;}

#define _ENTER(fname)                       \
  CHECK_RANK(rank, _num_ranks);             \
  CHECK_EVT_WRITER();                       \
  logger(OWV_INFO, fname); fflush(stdout);  \
  _start_time = std::min(_start_time, start);             \
  _stop_time = std::max(_stop_time, stop);               \
  auto this_region = _region[string(fname)];\
  string _fname = fname;                    \
  OTF2_EvtWriter_Enter(_evt_writer,         \
                       nullptr,             \
                       start,               \
                       this_region);        \
  _event_count[rank]++;

#define _LEAVE()                  \
  OTF2_EvtWriter_Leave(_evt_writer,  \
                       nullptr,     \
                       stop,        \
                       this_region);\
  _event_count[rank]++;              \
  return OTF2_WRITER_SUCCESS;

// All collectives are recorded with the same function in OTF2. Some collectives don't have a root, so some calls need a bogus parameter.
#define UNDEFINED_ROOT int root = -1;

#define COLLECTIVE_WRAPPER(collective, sent, received)                                                \
  CHECK_COMM();                                                                                       \
  OTF2_EvtWriter_MpiCollectiveBegin(_evt_writer, nullptr, start);                                      \
  OTF2_EvtWriter_MpiCollectiveEnd(_evt_writer, nullptr, stop, collective, comm, root, sent, received); \
  _event_count[rank] += 2;

using std::string;
using std::to_string;

namespace dumpi {
  OTF2_Writer::OTF2_Writer() {
  }

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

    // See ScoreP source for local def writer examples
    //  vendor/otf2/src/tools/otf2_trace_gen/otf2_trace_gen.c

    check_otf2(OTF2_Archive_OpenDefFiles(_archive), "Opening definition files");
    OTF2_GlobalDefWriter* defwriter = OTF2_Archive_GetGlobalDefWriter(_archive);

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
    _string["LOCATIONS_GROUP"];

    for(int rank = 0; rank < _num_ranks; rank++) {
      _string[string("MPI Rank ") + to_string(rank)];
      _string[string("Master Thread ") + to_string(rank)];
    }

    for(int i = 0; i < _region.size(); i++) _string[_region[i]];
    for(auto comm_it = _mpi_comm.begin(); comm_it != _mpi_comm.end(); comm_it++) comm_it->second.name;

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
                                              0 /* TODO, This should point to the node this rank ran on. Not necessary for sst/macro trace replay.*/),
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
                                         _event_count[i],
                                         0),
                                         "Writing Location to global def file");
    }

    // GROUP
    logger(OWV_INFO, "Writing GROUPs to the def file");
    // ScoreP behavior: Each COMM points to a GROUP def of type OTF2_GROUP_TYPE_COMM_GROUP.
    // Althout the GROUP defs enumerate their ranks, Each must point to a root group of type OTF2_GROUP_TYPE_COMM_LOCATIONS.
    // ScoreP does not seem to nest groups, instead flattening each to the global LOCATIONS group.
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

    check_otf2(OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                    COMM_WORLD_GROUP_ID,
                                    EMPTY_STRING,
                                    OTF2_GROUP_TYPE_COMM_GROUP,
                                    OTF2_PARADIGM_MPI,
                                    OTF2_GROUP_FLAG_NONE,
                                    _num_ranks,
                                    world_list),
                                    "Writing MPI_COMM_WORLD group to global def file");
    delete world_list;

    check_otf2(OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                    COMM_SELF_GROUP_ID,
                                    EMPTY_STRING,
                                    OTF2_GROUP_TYPE_COMM_GROUP,
                                    OTF2_PARADIGM_MPI,
                                    OTF2_GROUP_FLAG_NONE,
                                    0,
                                    0),
                                    "Writing MPI_COMM_WORLD group to global def file");

    for(auto group_it = _mpi_group.begin(); group_it != _mpi_group.end(); group_it++) {
      auto members = group_it->second;
      uint64_t* group_list = new uint64_t[members.size()];
      for (int i = 0; i < members.size(); i++)  group_list[i] = (uint64_t)members[i];

      check_otf2(OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                      group_it->first + USER_DEF_COMM_GROUP_OFFSET,
                                      EMPTY_STRING,
                                      OTF2_GROUP_TYPE_COMM_GROUP,
                                      OTF2_PARADIGM_MPI,
                                      OTF2_GROUP_FLAG_NONE,
                                      members.size(),
                                      group_list),
                                      "Writing group to global def file");
      delete group_list;
    }

    // When no communicator manipulation occurs, mpi_group list will be empty because it is dynamically generated from MPI callbacks.
    // The trace needs to have at least one group for MPI_COMM_WORLD
    if (_mpi_group.size() == 0) {
      uint64_t* group_list = new uint64_t[_num_ranks];
      for (int i = 0; i < _num_ranks; i++)  group_list[i] = i;

      check_otf2(OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                      0 + USER_DEF_COMM_GROUP_OFFSET,
                                      EMPTY_STRING,
                                      OTF2_GROUP_TYPE_COMM_GROUP,
                                      OTF2_PARADIGM_MPI,
                                      OTF2_GROUP_FLAG_NONE,
                                      _num_ranks,
                                      group_list),
                                      "Writing group for MPI_COMM_WORLD go global def file");

      delete group_list;
    }

//    GROUP                                0  Name: "" <0>, Type: COMM_LOCATIONS, Paradigm: "MPI" <4>, Flags: NONE, 4 Members: "Master thread" <0>, "Master thread" <1>, "Master thread" <2>, "Master thread" <3>
//    GROUP                                1  Name: "" <0>, Type: COMM_GROUP, Paradigm: "MPI" <4>, Flags: NONE, 4 Members: 0 ("Master thread" <0>), 1 ("Master thread" <1>), 2 ("Master thread" <2>), 3 ("Master thread" <3>)
//    GROUP                                2  Name: "" <0>, Type: COMM_SELF, Paradigm: "MPI" <4>, Flags: NONE, 0 Members:
//    COMM                                 0  Name: "MPI_COMM_WORLD" <329>, Group: "" <1>, Parent: UNDEFINED
//    COMM                                 1  Name: "MPI_COMM_SELF" <334>, Group: "" <2>, Parent: UNDEFINED

    // COMM
    // Dealing with unset MPI_COMM_WORLD
    if (_comm_world_id == -1) {
      if (_unknown_comms.size() == 1) {
        _comm_world_id = *_unknown_comms.begin();
        _unknown_comms.clear();
        logger(OWV_WARN, string("MPI_COMM_WORLD was not registered! One unregistered ID found, setting to MPI_COMM_WORLD to" + to_string(_comm_world_id)));
      } else if (_unknown_comms.size() > 0) {
        _comm_world_id = *_unknown_comms.begin();
        _unknown_comms.erase(_unknown_comms.begin());
        logger(OWV_ERROR, string("MPI_COMM_WORLD was not registered! Setting MPI_COMM_WORLD to" + to_string(_comm_world_id)));
      } else {
        logger(OWV_ERROR, string("MPI_COMM_WORLD was not registered! Your traces will not define an MPI_COMM_WORLD communicator"));
      }
    }

    // Callers are not expected to set MPI_COMM_WORLD using register_comm.
    // As long as it was register_comm_world() was called, there is enough information to construct a communicator
    if (_mpi_comm.find(_comm_world_id) == _mpi_comm.end())
      check_otf2(OTF2_GlobalDefWriter_WriteComm(defwriter,
                                                _comm_world_id,
                                                _string["MPI_COMM_WORLD"],
                                                COMM_WORLD_GROUP_ID,
                                                OTF2_UNDEFINED_COMM),
                                                "Writing MPI_COMM_WORLD to global def file");

    for(auto comm_it = _mpi_comm.begin(); comm_it != _mpi_comm.end(); comm_it++) {
      int comm_id = comm_it->first;
      MPI_Comm_Struct& comm = comm_it->second;

      check_otf2(OTF2_GlobalDefWriter_WriteComm( defwriter,
                                      comm_id,
                                      _string[comm.name],
                                      comm.id == _comm_world_id ? COMM_WORLD_GROUP_ID : comm.group + USER_DEF_COMM_GROUP_OFFSET,
                                      comm.id == _comm_world_id ? OTF2_UNDEFINED_COMM : comm.parent),
                                      "Writing a Communicator to global def file");
    }


    // Register communicator mapping. Also essential for creating Local def files, which are not created automatically.
    for(int r = 0; r < _num_ranks; r++) {
      OTF2_DefWriter* local_def_writer = OTF2_Archive_GetDefWriter(_archive, r);

      // local COMM mappings
      OTF2_IdMap* _mpi_comm_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, 2 );

      // Each location uses its rank as the communicator id which maps to the global 0
      check_otf2(OTF2_IdMap_AddIdPair( _mpi_comm_map, r, _comm_world_id ), "Adding MPI_COMM_WORLD to a mapping table");
      check_otf2(OTF2_IdMap_AddIdPair( _mpi_comm_map, r + _num_ranks, FIXME_COMM_SELF ), "Adding MPI_COMM_SELF to a mapping table");
      check_otf2(OTF2_DefWriter_WriteMappingTable( local_def_writer,
                                        OTF2_MAPPING_COMM,
                                        _mpi_comm_map ),
                                        "Writing a communicator mapping table to the OTF2 archive");

      OTF2_IdMap_Free(_mpi_comm_map);
      check_otf2(OTF2_Archive_CloseDefWriter(_archive, local_def_writer), "Closing a local def writer");
    }

    check_otf2(OTF2_Archive_CloseDefFiles(_archive), "Closing definition files");
  }

  OTF2_WRITER_RESULT OTF2_Writer::close_archive() {
    if (_request_type.size() > 0)
      logger(OWV_WARN, string("") + string("Closing archive with ") + to_string(_request_type.size()) + string(" incomplete MPI calls on rank ") + to_string(rank));

    _request_type.clear();
    _irecv_requests.clear();

    // Close currently open event file (if necessary)
    close_evt_file();

    check_otf2(OTF2_Archive_CloseEvtFiles(_archive), "Closing all event files");
    write_def_files();
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

    return OTF2_WRITER_SUCCESS;
  }

  void OTF2_Writer::register_comm_world(comm_t id) {
    std::vector<int> w_group;
    for(int i = 0; i < _num_ranks; i++) w_group.push_back(i);
    _mpi_group[COMM_WORLD_GROUP_ID] = w_group;
    _comm_world_id = id;
    _mpi_comm[id] = {.name="MPI_COMM_WORLD", .parent=0, .id=id, .group=(int)COMM_WORLD_GROUP_ID};
    _unknown_comms.erase(id);
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

  void OTF2_Writer::register_type(mpi_type_t type, int size) {
    _type_sizes[type] = size;
  }

  void OTF2_Writer::close_evt_file() {
    // close existing OTF2 event file if it is open
    if (_evt_writer != nullptr) {
      check_otf2(OTF2_Archive_CloseEvtWriter(_archive, _evt_writer), "Closing event writer for current rank");
      _evt_writer = nullptr;
    }
  }

  OTF2_WRITER_RESULT OTF2_Writer::set_rank(int rank) {
    CHECK_RANK(rank, _num_ranks);

    // check for outstanding incomplete calls (ie isends, irecv)
    if (_request_type.size() > 0) {
      logger(OWV_WARN, string("Incomplete MPI calls on rank ") + to_string(this->rank));
    }

    // Close currently open event file (if necessary)
    close_evt_file();

    // set the new rank
    this->rank = rank;

    // Open OTF2 event file for new rank
    _evt_writer = OTF2_Archive_GetEvtWriter(_archive, rank);

    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_send_inner(OTF2_EvtWriter* _evt_writer, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    CHECK_COMM();
    OTF2_EvtWriter_MpiSend(_evt_writer, nullptr, start, dest, comm, tag, _type_sizes[type]*count);
    _event_count[rank]++;
    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_send(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Send");
    mpi_send_inner(_evt_writer, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_bsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Bsend");
    mpi_send_inner(_evt_writer, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_ssend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Ssend");
    mpi_send_inner(_evt_writer, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_rsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Rsend");
    mpi_send_inner(_evt_writer, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_recv(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag) {
    _ENTER("MPI_Recv");
    CHECK_COMM();
    OTF2_EvtWriter_MpiRecv(_evt_writer, nullptr, start, source, comm, tag, _type_sizes[type]*count);
    _event_count[rank]++;
    _LEAVE();
  }

  // TODO replace this mess with lambdas
  // I-event handling

  void OTF2_Writer::incomplete_call(int request_id, REQUEST_TYPE type) {
    _request_type[request_id] = type;
  }

  /*
   * ISends and IRecvs begin when invoked, but important information about
   * them are not recorded by OTF2 until they are completed, which occurs inside of Waits, Tests, etc.
   * Returns the number of events generated
   */
  int OTF2_Writer::complete_call(request_t request_id, uint64_t timestamp) {
    auto t = _request_type.find(request_id);
    int events = 0;
    if (t == _request_type.end()) {
      printf("Error: request id (%i) not found\n", request_id);
    }
    else if (t->second == REQUEST_TYPE_ISEND) {
      OTF2_EvtWriter_MpiIsendComplete(_evt_writer, nullptr, timestamp, request_id);
      events++;
    }
    else if (t->second == REQUEST_TYPE_IRECV) {
      auto irecv_it = _irecv_requests.find(request_id);
      irecv_capture irecv = irecv_it->second;
      if(irecv_it == _irecv_requests.end()) {
        printf("Error: Request #(%i) not found while trying to complete MPI_IRecv\n", request_id);
      } else {
        OTF2_EvtWriter_MpiIrecv(_evt_writer, nullptr, timestamp, irecv.source, irecv.comm, irecv.tag, irecv.count, request_id);
        _irecv_requests.erase(irecv_it);
        events++;
      }
    }
    _request_type.erase(t);
    return events;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_isend_inner(OTF2_EvtWriter* _evt_writer, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    CHECK_COMM();
    incomplete_call(request, REQUEST_TYPE_ISEND);
    //logger(OWV_INFO, string("  MPI isend request ") + to_string(request));
    OTF2_EvtWriter_MpiIsend(_evt_writer, nullptr, start, dest, comm, tag, _type_sizes[type]*count, request);
    _event_count[rank]++;
    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_isend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Isend");
    mpi_isend_inner(_evt_writer, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_ibsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Ibsend");
    mpi_isend_inner(_evt_writer, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_issend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Issend");
    mpi_isend_inner(_evt_writer, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_irsend(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Irsend");
    mpi_isend_inner(_evt_writer, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_irecv(otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Irecv");
    CHECK_COMM();
    //logger(OWV_INFO, string("  MPI irecv request ") + to_string(request));
    _irecv_requests[request] = {count, type, source, tag, comm, request};
    incomplete_call(request, REQUEST_TYPE_IRECV);
    OTF2_EvtWriter_MpiIrecvRequest(_evt_writer, nullptr, start, request);
    _event_count[rank]++;
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::generic_call(otf2_time_t start, otf2_time_t stop, string name) {
    if (stop < start) {
      printf("ERROR, end happened before start\n");
    }
    _ENTER(name);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_wait(otf2_time_t start, otf2_time_t stop, request_t request) {
    _ENTER("MPI_Wait");
    _event_count[rank] += complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitany(otf2_time_t start, otf2_time_t stop, request_t request) {
    _ENTER("MPI_Waitany");
    _event_count[rank] += complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitall(otf2_time_t start, otf2_time_t stop, int count, request_t* requests) {
    _ENTER("MPI_Waitall");
    int events = 0;
    std::unordered_set<request_t> called;
    for(int i = 0; i < count; i++) {
      auto req = requests[i];
      if (req != _null_request && called.find(req) == called.end()) {
        events += complete_call(req, start);
        called.insert(req);
      }
    }
    _event_count[rank] += events;
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitsome(otf2_time_t start, otf2_time_t stop, request_t* requests, int outcount, int* indices) {
    _ENTER("MPI_Waitsome");
    int events = 0;
    for (int i = 0; i < outcount; i++)
      events += complete_call(requests[indices[i]], start);
    _event_count[rank] += events;
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_test(otf2_time_t start, otf2_time_t stop, request_t request, int flag) {
    _ENTER("MPI_Test");
    if (flag)
      _event_count[rank] += complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testany(otf2_time_t start, otf2_time_t stop, request_t* requests, int index, int flag) {
    _ENTER("MPI_Testany");
    if (flag)
      _event_count[rank] += complete_call(requests[index], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testall(otf2_time_t start, otf2_time_t stop, int count, request_t* requests, int flag) {
    _ENTER("MPI_Testall");
    if (flag)
      for (int i = 0; i < count; i++)
        _event_count[rank] += complete_call(requests[i], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testsome(otf2_time_t start, otf2_time_t stop, request_t* requests, int outcount, int* indices) {
    _ENTER("MPI_Testsome");
    int events = 0;
    for (int i = 0; i < outcount; i++)
      events += complete_call(requests[indices[i]], start);
    _event_count[rank] += events;
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_barrier(otf2_time_t start, otf2_time_t stop, comm_t comm) {
    _ENTER("MPI_Barrier");
    UNKNOWN_COMM_TRAP();
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_BARRIER, 0, 0);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_bcast(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, int root, comm_t comm) {
    _ENTER("MPI_Bcast");
    UNKNOWN_COMM_TRAP();
    int bytes = count_bytes(type, count);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_BCAST,
                       ranks_equivalent(rank, root, comm) ? bytes : 0,
                       ranks_equivalent(rank, root, comm) ? 0 : bytes);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_gather(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, comm_t comm) {
    _ENTER("MPI_Gather");
    UNKNOWN_COMM_TRAP();

    int send_count = 0;
    int recv_count = 0;

    if (ranks_equivalent(rank, root, comm)) recv_count = count_bytes(recvtype, recvcount);
    else send_count = count_bytes(sendtype, sendcount);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_GATHER,
                       sendcount,
                       recvcount);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_gatherv(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int* recvcounts, mpi_type_t recvtype, int root, comm_t comm) {
    _ENTER("MPI_Gatherv");
    UNKNOWN_COMM_TRAP();

    int send_count = 0;
    int recv_count = 0;

    if (ranks_equivalent(rank, root, comm))
      recv_count = count_bytes(recvtype, array_sum(recvcounts, get_comm_size(comm)));
    else
      send_count = count_bytes(sendtype, sendcount);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_GATHERV,
                       send_count,
                       recv_count);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scatter(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, comm_t comm) {
    _ENTER("MPI_Scatter");
    UNKNOWN_COMM_TRAP();
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCATTER,
                       (ranks_equivalent(rank, root, comm)) ? count_bytes(sendtype, sendcount) : 0,
                       (ranks_equivalent(rank, root, comm)) ? 0 : count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scatterv(otf2_time_t start, otf2_time_t stop, int* sendcounts, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, int root, comm_t comm) {
    _ENTER("MPI_Scatterv");
    UNKNOWN_COMM_TRAP();
    int send_count = 0;
    int recv_count = 0;

    if (ranks_equivalent(rank, root, comm))
      send_count = count_bytes(sendtype, array_sum(sendcounts, get_comm_size(comm)));
    else
      recv_count = count_bytes(recvtype, recvcount);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCATTERV,
                       send_count,
                       recv_count);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scan(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t datatype, comm_t comm) {
    _ENTER("MPI_Scan");
    UNKNOWN_COMM_TRAP();
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCAN,
                       count_bytes(datatype, count),
                       count_bytes(datatype, count));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allgather(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Allgather");
    UNKNOWN_COMM_TRAP();
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLGATHER,
                       count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allgatherv(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int* recvcounts, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Allgatherv");
    UNKNOWN_COMM_TRAP();
    UNDEFINED_ROOT

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLGATHERV,
                       count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, array_sum(recvcounts, get_comm_size(comm))));
    _LEAVE();
  }
  OTF2_WRITER_RESULT OTF2_Writer::mpi_alltoall(otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype, int recvcount, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Alltoall");
    UNKNOWN_COMM_TRAP();
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLTOALL,
                       count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_alltoallv(otf2_time_t start, otf2_time_t stop, int* sendcounts, mpi_type_t sendtype, int* recvcounts, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Alltoallv");
    UNKNOWN_COMM_TRAP();
    UNDEFINED_ROOT
    int count = get_comm_size(comm);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLTOALLV,
                       count_bytes(sendtype, array_sum(sendcounts, count)),
                       count_bytes(recvtype, array_sum(recvcounts, count)));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_reduce(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, int root, comm_t comm) {
    _ENTER("MPI_Reduce");
    UNKNOWN_COMM_TRAP();

    int sent = count_bytes(type, count);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_REDUCE,
                       sent,
                       ranks_equivalent(rank, root, comm) ? sent : 0);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allreduce(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type, comm_t comm) {
    _ENTER("MPI_Allreduce");
    UNKNOWN_COMM_TRAP();
    UNDEFINED_ROOT
    int bytes = count_bytes(type, count);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLREDUCE,
                       bytes,
                       bytes);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_reduce_scatter(otf2_time_t start, otf2_time_t stop, int* recvcounts, mpi_type_t type, comm_t comm) {
    _ENTER("MPI_Reduce_scatter");
    UNKNOWN_COMM_TRAP();
    UNDEFINED_ROOT
    // TODO: double check the byte counts are accurate
    int bytes = count_bytes(type, array_sum(recvcounts, get_comm_size(comm)));

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_REDUCE_SCATTER,
                       bytes,
                       bytes);
    _LEAVE();
  }

  bool OTF2_Writer::group_is_known(int group) {
    if (_mpi_group.find(group) == _mpi_group.end()) {
      logger(OWV_WARN, string("Unknown group (") + to_string(group));
      return false;
    }
    return true;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_union(otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_union");
    UNKNOWN_GROUP_TRAP(group1);
    UNKNOWN_GROUP_TRAP(group2);

    auto g1_it = _mpi_group[group1].begin();
    auto g2_it = _mpi_group[group2].begin();
    auto g1_end = _mpi_group[group1].end();
    auto g2_end = _mpi_group[group2].end();
    auto dest = std::back_inserter(_mpi_group[newgroup]);

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
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_difference(otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_difference");
    UNKNOWN_GROUP_TRAP(group1);
    UNKNOWN_GROUP_TRAP(group2);

    auto g1_it = _mpi_group[group1].begin();
    auto g2_it = _mpi_group[group2].begin();
    auto g1_end = _mpi_group[group1].end();
    auto g2_end = _mpi_group[group2].end();
    auto dest = std::back_inserter(_mpi_group[newgroup]);

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

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_intersection(otf2_time_t start, otf2_time_t stop, int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_intersection");
    UNKNOWN_GROUP_TRAP(group1);
    UNKNOWN_GROUP_TRAP(group2);

    auto g1_it = _mpi_group[group1].begin();
    auto g2_it = _mpi_group[group2].begin();
    auto g1_end = _mpi_group[group1].end();
    auto g2_end = _mpi_group[group2].end();
    auto dest = std::back_inserter(_mpi_group[newgroup]);

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

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_incl(otf2_time_t start, otf2_time_t stop, int group, int count, int*ranks, int newgroup) {
    _ENTER("MPI_Group_incl");
    UNKNOWN_GROUP_TRAP(group);

    auto old_group = _mpi_group[group];
    auto new_group = _mpi_group[newgroup];
    int g_size = old_group.size();
    for (int i = 0; i < count; i++) {
      if (ranks[i] >= g_size) logger(OWV_ERROR, string("MPI_Group_incl found ranks out of bounds!"));
      else new_group.push_back(old_group[ranks[i]]);
    }

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_excl(otf2_time_t start, otf2_time_t stop, int group, int count, int*ranks, int newgroup) {
    _ENTER("MPI_Group_excl");
    UNKNOWN_GROUP_TRAP(group);

    auto old_group = _mpi_group[group];
    auto new_group = _mpi_group[newgroup];

    for (int i = 0; i < old_group.size(); i++) {
      bool exclude = false;
      for (int j = 0; j < count; j++) {
        if (ranks[j] == i) {
          exclude = true;
          break;
        }
      }
      if (!exclude) new_group.push_back(old_group[ranks[i]]);
    }

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_range_incl(otf2_time_t start, otf2_time_t stop, int group, int count, int**ranges, int newgroup) {
    _ENTER("MPI_Group_range_incl");
    UNKNOWN_GROUP_TRAP(group);

    auto old_group = _mpi_group[group];
    auto new_group = _mpi_group[newgroup];

    _mpi_group[newgroup] = _mpi_group[group];
    for (int i = 0; i < count; i++)
      for(int j = ranges[i][0]; j < ranges[i][1]; j+=ranges[i][2])
         _mpi_group[newgroup].push_back(old_group[j]);

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

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_dup(otf2_time_t start, otf2_time_t stop, comm_t comm, comm_t newcomm) {
    _ENTER("MPI_Comm_dup");
    UNKNOWN_COMM_TRAP();

    auto c = _mpi_comm[newcomm];
    c = _mpi_comm[comm];
    c.parent = comm;

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_group(otf2_time_t start, otf2_time_t stop, comm_t comm, int group) {
    _ENTER("MPI_Comm_group");
    UNKNOWN_COMM_TRAP();
    _mpi_comm[comm].group = group;

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_create(otf2_time_t start, otf2_time_t stop, comm_t comm, int group, comm_t newcomm) {
    _ENTER("MPI_Comm_create");

    UNKNOWN_COMM_TRAP();
    UNKNOWN_GROUP_TRAP(group);

    _mpi_comm[newcomm].group = group;
    _mpi_comm[newcomm].parent = comm;
    _mpi_comm[newcomm].id = newcomm;

    _LEAVE();
  }

//  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_split(otf2_time_t start, otf2_time_t stop, comm_t oldcomm, int color, int key, comm_t newcomm) {
//    _ENTER("MPI_Comm_split");

//    _LEAVE();
//  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_contiguous(otf2_time_t start, otf2_time_t stop, int count, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_contiguous");

    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string("MPI_type_contiguous failed"));
      return OTF2_WRITER_ERROR_UKNOWN_MPI_TYPE;
    }

    _type_sizes[newtype] = count_bytes(oldtype, count);

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_hvector(otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hvector");
    mpi_t_vector_inner("MPI_Type_hvector", count, blocklength, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_vector(otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
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

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_indexed(otf2_time_t start, otf2_time_t stop, int count, int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_indexed");
    mpi_t_indexed_inner("MPI_Type_indexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_hindexed(otf2_time_t start, otf2_time_t stop, int count, int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hindexed");
    mpi_t_indexed_inner("MPI_Type_hindexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_hindexed(otf2_time_t start, otf2_time_t stop, int count, int*lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hindexed");
    mpi_t_indexed_inner("MPI_Type_hindexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  void OTF2_Writer::mpi_t_indexed_inner(const char* name, int count, int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string(name) + " failed");
      return;
    }

    _type_sizes[newtype] = count_bytes(oldtype, array_sum(lengths, count));
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_struct(otf2_time_t start, otf2_time_t stop, int count, int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype) {
    _ENTER("MPI_Type_struct");
    mpi_t_struct_inner("MPI_Type_struct", count, blocklengths, oldtypes, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_struct(otf2_time_t start, otf2_time_t stop, int count, int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype) {
    _ENTER("MPI_Type_create_struct");
    mpi_t_struct_inner("MPI_Type_create_struct", count, blocklengths, oldtypes, newtype);
    _LEAVE();
  }

  void OTF2_Writer::mpi_t_struct_inner(const char* fname, int count, int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype) {
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

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_subarray(otf2_time_t start, otf2_time_t stop, int ndims, int* subsizes, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_create_subarray");

    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string("MPI_Type_create_subarray failed"));
      return OTF2_WRITER_ERROR_UKNOWN_MPI_TYPE;
    }

    _type_sizes[newtype] = count_bytes(oldtype, array_sum(subsizes, ndims));

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_hvector(otf2_time_t start, otf2_time_t stop, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
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

  bool OTF2_Writer::ranks_equivalent(int world_rank, int comm_rank, comm_t comm) {
    if (!comm_is_known(comm)) {
      logger(OWV_WARN, string("Cannot compare ranks for an unknown communicator"));
      return false;
    }

    auto group = _mpi_group[_mpi_comm[comm].group];

    if (comm_rank < 0 || comm_rank >= group.size()) {
      logger(OWV_WARN, string("Comm rank out of range!"));
      return false;
    }

    return group[comm_rank] == world_rank;
  }

  int OTF2_Writer::get_comm_rank(int world_rank, comm_t comm) {
    auto rank_list = _mpi_group[_mpi_comm[comm].group];

    // slow, linear search...
    auto rank =  std::find(rank_list.begin(), rank_list.end(), world_rank);
    if (rank == rank_list.end()) return -1; // WE HAVE A PROBLEM
    else return rank - rank_list.begin();
  }

  int OTF2_Writer::get_comm_size(comm_t comm) {
    return _mpi_group[_mpi_comm[comm].group].size();
  }

  uint64_t OTF2_Writer::count_bytes(mpi_type_t type, uint64_t count){
   if (_type_sizes.find(type) == _type_sizes.end()) {
     logger(OWV_ERROR, string("Unkown data type (") + to_string((int)type) + "). Assuming 4 bytes in size.");
     return 4*count;
   } else return _type_sizes[type]*count;
  }

//  void OTF2_Writer::skip_unused_calls(bool skip_unused) {
//    _skip_unused_calls = skip_unused;
//  }

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
    auto out = std::find_if(_map.begin(), _map.end(), [index] (const auto& elem) { return elem.second == index; });
    return out == _map.end() ? nullptr : out->first;
  }

  const string OTF2DefTable::operator[] (int index) { return map(index); }
  int OTF2DefTable::operator[] (const char* index) { return map(string(index));}
  int OTF2DefTable::operator[] (string string) { return map(string); }
  int OTF2DefTable::size() { return _counter; }
  bool OTF2DefTable::added_last_lookup() {return _added_last_lookup;}
}
