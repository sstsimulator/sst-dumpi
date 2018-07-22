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
#include <sys/stat.h>
#include <algorithm>
#include <limits.h>
#include <cstring>
#include <unordered_set>
#include <functional>

#define CHECK_RANK(rank, _num_ranks) \
  if (rank < -1 || rank >= _num_ranks) return OTF2_WRITER_ERROR_NO_RANK_SET;

#define COMM_NULL_OR_ERROR(comm) (comm == _comm_null_id || comm == _comm_error_id)

// immediately returns an error code when the group is unknown
//#define UNKNOWN_GROUP_TRAP(group)              \
//  if (!group_is_known(group)) {                \
//    logger(OWV_ERROR, _fname + " failed");     \
//    return OTF2_WRITER_ERROR_UKNOWN_MPI_GROUP;}

#define UNKNOWN_GROUP_TRAP(group)

#define _ENTER(fname)                         \
  logger(OWV_INFO, fname);                    \
  start_time_ = std::min(start_time_, start); \
  stop_time_ = std::max(stop_time_, stop);    \
  auto this_region = region_[string(fname)];  \
  string _fname = fname;                      \
  OTF2_EvtWriter_Enter(context_.evt_writer,      \
                       nullptr,             \
                       start,               \
                       this_region);        \
  context_.event_count++;

#define _LEAVE()                         \
  OTF2_EvtWriter_Leave(context_.evt_writer,   \
                       nullptr,          \
                       stop,             \
                       this_region);     \
  context_.event_count++;                \
  return OTF2_WRITER_SUCCESS;

// All collectives are recorded with the same function in OTF2. Some collectives don't have a root, so some calls need a bogus parameter.
#define UNDEFINED_ROOT int root = -1;

#define COLLECTIVE_WRAPPER(collective, sent, received)                                                    \
  OTF2_EvtWriter_MpiCollectiveBegin(context_.evt_writer, nullptr, start);                                      \
  OTF2_EvtWriter_MpiCollectiveEnd(context_.evt_writer, nullptr, stop, collective, comm, root, sent, received); \
  context_.event_count += 2;

using std::string;
using std::to_string;

static OTF2_CallbackCode
otf2writer_get_rank_cb(void* userData,
                       OTF2_CollectiveContext* commContext,
                       uint32_t* rank)
{
  auto data = (dumpi::OTF2_Writer::worldConfig*) userData;
  *rank = data->rank;
  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
otf2writer_get_size_cb(void* userData,
                       OTF2_CollectiveContext* commContext,
                       uint32_t* size )
{

  auto data = (dumpi::OTF2_Writer::worldConfig*) userData;
  *size = data->size;
  return OTF2_CALLBACK_SUCCESS;
}

static OTF2_CallbackCode
otf2writer_barrier_cb(void* userData,
                      OTF2_CollectiveContext* commContext )
{
  return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
otf2writer_bcast_cb(void*                   userData,
                    OTF2_CollectiveContext* commContext,
                    void*                   data,
                    uint32_t                numberElements,
                    OTF2_Type               type,
                    uint32_t                root)
{
  return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
otf2writer_gather_cb(void*                   userData,
                     OTF2_CollectiveContext* commContext,
                     const void*             inData,
                     void*                   outData,
                     uint32_t                numberElements,
                     OTF2_Type               type,
                     uint32_t                root)
{
  return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
otf2writer_gatherv_cb(void*                   userData,
                     OTF2_CollectiveContext* commContext,
                     const void*             inData,
                     uint32_t                inElements,
                     void*                   outData,
                     const uint32_t*         outElements,
                     OTF2_Type               type,
                     uint32_t                root)
{
  return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
otf2writer_scatter_cb(void*                   userData,
                      OTF2_CollectiveContext* commContext,
                      const void*             inData,
                      void*                   outData,
                      uint32_t                numberElements,
                      OTF2_Type               type,
                      uint32_t                root)
{
  return OTF2_CALLBACK_SUCCESS;
}


static OTF2_CallbackCode
otf2writer_scatterv_cb(void*                   userData,
                       OTF2_CollectiveContext* commContext,
                       const void*             inData,
                       const uint32_t*         inElements,
                       void*                   outData,
                       uint32_t                outElements,
                       OTF2_Type               type,
                       uint32_t                root)
{
  return OTF2_CALLBACK_SUCCESS;
}

static const OTF2_CollectiveCallbacks dumpi_otf2writer_collectives =
{
  NULL,
  otf2writer_get_size_cb,
  otf2writer_get_rank_cb,
  NULL,
  NULL,
  otf2writer_barrier_cb,
  otf2writer_bcast_cb,
  otf2writer_gather_cb,
  otf2writer_gatherv_cb,
  otf2writer_scatter_cb,
  otf2writer_scatterv_cb
};

namespace dumpi {
  OTF2_Writer::OTF2_Writer() :
    directory_(""),
    mpi_group_(),
    mpi_comm_(),
    type_sizes_(),
    unknown_comms_(),
    string_(),
    region_(),
    archive_(nullptr),
    start_time_(~0),
    stop_time_(0),
    comm_world_id_(-1),
    comm_self_id_(-1),
    comm_error_id_(-1),
    comm_null_id_(-1),
    null_request_(-1),
    //_comm_split_constructor(),
    //_comm_create_constructor(),
    //_comm_actions(),
    _verbosity(OWV_NONE),
    _clock_resolution(1)
  {}

  string OTF2_Writer::get_otf2_directory() {
    return directory_;
  }

  // OTF2 callbacks
  static OTF2_FlushType pre_flush( void* userData, OTF2_FileType fileType, OTF2_LocationRef location, void* callerData, bool final) { return OTF2_FLUSH; }
  static OTF2_TimeStamp post_flush(void* userData, OTF2_FileType fileType, OTF2_LocationRef location) { return 0; }

  OTF2_FlushCallbacks flush_callbacks =
  {
      pre_flush,
      post_flush
  };

  OTF2_WRITER_RESULT OTF2_Writer::open_archive(string path, int size, int rank) {
    if (archive_ != nullptr)
      return OTF2_WRITER_ERROR_ARCHIVE_ALREADY_OPEN;

    struct stat sb;

    // test if directory already exists
    // if (stat(path.c_str(), &sb) == 0 )
    //     return OTF2_WRITER_ERROR_DIRECTORY_ALREADY_EXISTS;

    mk_archive_dir(path.c_str());

    // test if parent directory exists
    //if (stat((path + "/..").c_str(), &sb) && !force)
    //  return OTF2_WRITER_ERROR_DIRECTORY_ALREADY_EXISTS;

    world_.size = size;
    world_.rank = rank;
    directory_ = path;
    archive_ = OTF2_Archive_Open(  directory_.c_str(),
                                   "traces",
                                   OTF2_FILEMODE_WRITE,
                                   1024 * 1024 /* event chunk size */,
                                   4 * 1024 * 1024 /* def chunk size */,
                                   OTF2_SUBSTRATE_POSIX,
                                   OTF2_COMPRESSION_NONE );

    OTF2_Archive_SetCollectiveCallbacks(archive_, &dumpi_otf2writer_collectives, &world_, NULL, NULL);
    //OTF2_Archive_SetSerialCollectiveCallbacks(_archive);
    OTF2_Archive_SetFlushCallbacks( archive_, &flush_callbacks, NULL );
    OTF2_Archive_OpenEvtFiles(archive_);

    context_.rank = rank;
    context_.evt_writer = OTF2_Archive_GetEvtWriter(archive_, rank);

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

  void OTF2_Writer::write_def_files(const std::vector<int>& event_counts) {
    // OTF2 Reference Example
    // https://silc.zih.tu-dresden.de/otf2-current/group__usage__writing__mpi.htm

    // See ScoreP source for a local def writer in action
    //  vendor/otf2/src/tools/otf2_trace_gen/otf2_trace_gen.c

    check_otf2(OTF2_Archive_OpenDefFiles(archive_), "Opening definition files");
    OTF2_GlobalDefWriter* defwriter = OTF2_Archive_GetGlobalDefWriter(archive_);

    if (unknown_comms_.size() > 0) {
      string str = "Unknown communicators: [ ";
      for (auto c = unknown_comms_.begin(); c != unknown_comms_.end(); c++) str += to_string(*c) + " ";
      str += "]";
      logger(OWV_ERROR, str);
    }

    if (_clock_resolution == 0) logger(OWV_ERROR, "Clock Resolution not set, use 'set_clock_resolution() to set ticks per second'");

    check_otf2(OTF2_GlobalDefWriter_WriteClockProperties( defwriter,
                                                          _clock_resolution,
                                                          start_time_,
                                                          stop_time_ - start_time_), "Writing clock properties to global def file");

    // Strings must come first in the def file
    // Otherwise tools like otf2_print will report errors, even though all of the information is available
    int EMPTY_STRING = string_[""];
    string_["MPI"];
    string_["MPI_COMM_WORLD"];
    string_["MPI_COMM_SELF"];
    string_["LOCATIONS_GROUP"];

    for(int rank = 0; rank < world_.size; rank++) {
      string_[string("MPI Rank ") + to_string(rank)];
      string_[string("Master Thread ") + to_string(rank)];
    }

    for(int i = 0; i < region_.size(); i++) string_[region_[i]];
    for(auto comm_it = mpi_comm_.begin(); comm_it != mpi_comm_.end(); comm_it++) string_[comm_it->second.name];

    // STRINGS
    logger(OWV_INFO, "Writing STRINGs to the def file");
    for(int i = 0; i < string_.size(); i++) {
      check_otf2(OTF2_GlobalDefWriter_WriteString(defwriter, i, string_[i].c_str()), "Writing string to global def file");
    }

    //PARADIGM
    logger(OWV_INFO, "Writing PARADIGMs to the def file");
    check_otf2(OTF2_GlobalDefWriter_WriteParadigm( defwriter,
                                        OTF2_PARADIGM_MPI,
                                        string_["MPI"],
                                        OTF2_PARADIGM_CLASS_PROCESS),
                                        "Writing Paradigm to global def file");

    // REGIONS
    logger(OWV_INFO, "Writing REGIONs to the def file");
    for(int i = 0; i < region_.size(); i++) {
      string region_name = region_[i];
      auto str_id = string_[region_name];
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
    for (int i = 0; i < world_.size; i++) {
      char rank_name[32];
      sprintf(rank_name, "MPI Rank %d", i);
      check_otf2(OTF2_GlobalDefWriter_WriteLocationGroup(defwriter,
                                              i,
                                              string_[rank_name],
                                              OTF2_LOCATION_GROUP_TYPE_PROCESS,
                                              0 /* This should point to the node this rank ran on. Not necessary for sst/macro trace replay.*/),
                                              "Writing Location Group to global def file");
    }

    // LOCATION
    logger(OWV_INFO, "Writing LOCATIONs to the def file");
    // Each location will identifies a rank. Must come before groups.
    for (int i = 0; i < world_.size; i++) {
      char loc_name[32];
      sprintf(loc_name, "Master Thread %d", i);
      check_otf2(OTF2_GlobalDefWriter_WriteLocation(defwriter,
                                         i,
                                         string_[loc_name],
                                         OTF2_LOCATION_TYPE_CPU_THREAD,
                                         event_counts[i],
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

    if (mpi_group_.size() == 0)
      logger(OWV_ERROR, "No communicator groups registered, did you forget to call register_comm_world() and register_comm_self()");

    // ScoreP behavior: Each COMM points to a GROUP def of type OTF2_GROUP_TYPE_COMM_GROUP.
    // Althout the GROUP defs enumerate their ranks, Each must point to a root group of type OTF2_GROUP_TYPE_COMM_LOCATIONS.
    // ScoreP does not seem to nest groups, instead flattening each to the global LOCATIONS group that points directly to the COMM_LOCATIONS group.
    uint64_t* world_list = new uint64_t[world_.size];
    for (int i = 0; i < world_.size; i++)  world_list[i] = i;

    check_otf2(OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                    COMM_LOCATIONS_GROUP_ID,
                                    string_["LOCATIONS_GROUP"],
                                    OTF2_GROUP_TYPE_COMM_LOCATIONS,
                                    OTF2_PARADIGM_MPI,
                                    OTF2_GROUP_FLAG_NONE,
                                    world_.size,
                                    world_list),
                                    "Writing Locations Group to global def file");

    delete[] world_list;

    // The runtime will create duplicate comm groups, only use the ones referenced by communicators

    // Having non-contiguous group (integer) ID's makes otf2-print dump warnings. "otf2-print: warning: out of order Comm definition"
    // At this point all of the comm's and groups have been realized, and a last second remapping will solve the above warning
    std::map<int, int> group_id_map;
    int group_map_inc = 1;

    //for(auto group_it = _mpi_group.begin(); group_it != _mpi_group.end(); group_it++) {
    for(auto comm_it = mpi_comm_.begin(); comm_it != mpi_comm_.end(); comm_it++) {
      auto group_id = comm_it->second.group;
      auto& members = mpi_group_[group_id];
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

    if (comm_world_id_ == -1)
      logger(OWV_ERROR, "register_comm_world() not called");

    if (comm_self_id_ == -1)
      logger(OWV_ERROR, "register_comm_self() not called");

    if (comm_null_id_ == -1)
      logger(OWV_ERROR, "register_comm_null() not called");

    for(auto comm_it = mpi_comm_.begin(); comm_it != mpi_comm_.end(); comm_it++) {
      int comm_id = comm_it->first;
      MPI_Comm_Struct& comm = comm_it->second;

      check_otf2(OTF2_GlobalDefWriter_WriteComm( defwriter,
                                      comm_id,
                                      string_[comm.name],
                                      group_id_map[comm.group],
                                      comm.parent),
                                      "Writing a Communicator to global def file");
    }

    // local communicator -> global communicator mapping
    // Even without mapping information, this step is essential for creating local def files, which are required by a valid trace.
    for(int r = 0; r < world_.size; r++) {
      OTF2_DefWriter* local_def_writer = OTF2_Archive_GetDefWriter(archive_, r);
      check_otf2(OTF2_Archive_CloseDefWriter(archive_, local_def_writer), "Closing a local def writer");

      /** FIX ME
      // Getting rank-local metadata
      auto& rank_context = fetch_context(r);

      // initialize local COMM mappings table, include space for MPI_COMM_WORLD, MPI_COMM_SELF, and any user-defined communicators.
      OTF2_IdMap* _mpi_comm_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, context_.comm_mapping.size());

      // "comm_mapping" is populated on every rank in a registered communicator
      for(auto cm_it = rank_context.comm_mapping.begin(); cm_it != rank_context.comm_mapping.end(); cm_it++)
        check_otf2(OTF2_IdMap_AddIdPair(_mpi_comm_map, cm_it->first, cm_it->second),
                   "Adding a communicator to the Def mapping list");

      // Serialize the table to a local def file
      check_otf2(OTF2_DefWriter_WriteMappingTable( local_def_writer,
                                        OTF2_MAPPING_COMM,
                                        _mpi_comm_map ),
                                        "Writing a communicator mapping table to the OTF2 archive");

      OTF2_IdMap_Free(_mpi_comm_map);
      */
    }

    check_otf2(OTF2_Archive_CloseDefFiles(archive_), "Closing definition files");
  }

  OTF2_WRITER_RESULT OTF2_Writer::close_archive() {
    check_otf2(OTF2_Archive_CloseEvtFiles(archive_), "Closing all event files");
    check_otf2(OTF2_Archive_Close(archive_), "Closing archive");

    archive_ = nullptr;

    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::register_group(int id, std::vector<int> list) {
    mpi_group_[id] = list;
    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::register_comm(string name, int id, int parent, int group) {
    MPI_Comm_Struct& mcs = mpi_comm_[id];
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
    for(int i = 0; i < world_.size; i++) {
      w_group.push_back(i);
      context_.comm_mapping[id] = MPI_COMM_WORLD_ID;
    }
    mpi_group_[COMM_WORLD_GROUP_ID] = w_group;
    comm_world_id_ = id;

    auto& comm = mpi_comm_[MPI_COMM_WORLD_ID];
    comm.parent=OTF2_UNDEFINED_COMM;
    comm.id=MPI_COMM_WORLD_ID;
    comm.group=(int)COMM_WORLD_GROUP_ID;
    comm.name="MPI_COMM_WORLD";

    unknown_comms_.erase(id);
  }

  void OTF2_Writer::register_comm_self(comm_t id) {
    std::vector<int> s_group = {0};
    for(int i = 0; i < world_.size; i++) {
      context_.comm_mapping[id] = MPI_COMM_SELF_ID;
    }

    mpi_group_[COMM_SELF_GROUP_ID] = s_group;
    comm_self_id_ = id;

    auto& comm = mpi_comm_[MPI_COMM_SELF_ID];
    comm.parent=OTF2_UNDEFINED_COMM;
    comm.id=MPI_COMM_SELF_ID;
    comm.group=(int)COMM_SELF_GROUP_ID;
    comm.name="MPI_COMM_SELF";

    unknown_comms_.erase(id);
  }

  void OTF2_Writer::register_comm_error(comm_t id) {
    comm_error_id_ = id;
  }

  void OTF2_Writer::register_comm_null(comm_t id) {
    comm_null_id_ = id;
  }

  void OTF2_Writer::register_null_request(request_t request) {
    null_request_ = request;
    context_.null_request = null_request_;
  }

  void OTF2_Writer::set_verbosity(OTF2_WRITER_VERBOSITY verbosity) {
    _verbosity = verbosity;
  }

  void OTF2_Writer::set_clock_resolution(uint64_t ticks_per_second) {
    _clock_resolution = ticks_per_second;
  }

  // TODO type sizes should be rank specific.
  void OTF2_Writer::register_type(mpi_type_t type, int size) {
    type_sizes_[type] = size;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_send_inner(RankContext& ctx, otf2_time_t start, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    OTF2_EvtWriter_MpiSend(ctx.evt_writer, nullptr, start, dest, comm, tag, type_sizes_[type]*count);
    ctx.event_count++;
    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_send(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Send");
    mpi_send_inner(context_, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_bsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Bsend");
    mpi_send_inner(context_, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_ssend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Ssend");
    mpi_send_inner(context_, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_rsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t dest, int comm, uint32_t tag) {
    _ENTER("MPI_Rsend");
    mpi_send_inner(context_, start, type, count, dest, comm, tag);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_recv(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count, uint32_t source, int comm, uint32_t tag) {
    _ENTER("MPI_Recv");
    OTF2_EvtWriter_MpiRecv(context_.evt_writer, nullptr, start, source, comm, tag, count_bytes(type, count));
    context_.event_count++;
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_isend_inner(RankContext& ctx, otf2_time_t start, mpi_type_t type, uint64_t count,
                                                  uint32_t dest, int comm, uint32_t tag, request_t request) {
    context_.incomplete_call(request, REQUEST_TYPE_ISEND);
    OTF2_EvtWriter_MpiIsend(ctx.evt_writer, nullptr, start, dest, comm, tag, count_bytes(type, count), request);
    context_.event_count++;
    return OTF2_WRITER_SUCCESS;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_isend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count,
                                            uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Isend");
    mpi_isend_inner(context_, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_ibsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count,
                                             uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Ibsend");
    mpi_isend_inner(context_, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_issend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count,
                                             uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Issend");
    mpi_isend_inner(context_, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_irsend(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count,
                                             uint32_t dest, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Irsend");
    mpi_isend_inner(context_, start, type, count, dest, comm, tag, request);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_irecv(int rank, otf2_time_t start, otf2_time_t stop, mpi_type_t type, uint64_t count,
                                            uint32_t source, int comm, uint32_t tag, request_t request) {
    _ENTER("MPI_Irecv");
    context_.irecv_requests[request] = {count_bytes(type, count), source, tag, comm, request};
    context_.incomplete_call(request, REQUEST_TYPE_IRECV);
    OTF2_EvtWriter_MpiIrecvRequest(context_.evt_writer, nullptr, start, request);
    context_.event_count++;
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::generic_call(int rank, otf2_time_t start, otf2_time_t stop, string name) {
    _ENTER(name);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_wait(int rank, otf2_time_t start, otf2_time_t stop, request_t request) {
    _ENTER("MPI_Wait");
    context_.complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitany(int rank, otf2_time_t start, otf2_time_t stop, request_t request) {
    _ENTER("MPI_Waitany");
    context_.complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitall(int rank, otf2_time_t start, otf2_time_t stop, int count, const request_t* requests) {
    _ENTER("MPI_Waitall");
    std::unordered_set<request_t> called;
    for(int i = 0; i < count; i++) {
      auto req = requests[i];
      if (req != null_request_ && called.find(req) == called.end()) {
        context_.complete_call(req, start);
        called.insert(req);
      }
    }
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_waitsome(int rank, otf2_time_t start, otf2_time_t stop, const request_t* requests,
                                               int outcount, const int* indices) {
    _ENTER("MPI_Waitsome");
    for (int i = 0; i < outcount; i++)
      context_.complete_call(requests[indices[i]], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_test(int rank, otf2_time_t start, otf2_time_t stop, request_t request, int flag) {
    _ENTER("MPI_Test");
    if (flag)
      context_.complete_call(request, start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testany(int rank, otf2_time_t start, otf2_time_t stop, const request_t* requests,
                                              int index, int flag) {
    _ENTER("MPI_Testany");
    if (flag)
      context_.complete_call(requests[index], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testall(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                              const request_t* requests, int flag) {
    _ENTER("MPI_Testall");
    if (flag)
      for (int i = 0; i < count; i++)
        context_.complete_call(requests[i], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_testsome(int rank, otf2_time_t start, otf2_time_t stop,
                                               const request_t* requests, int outcount, const int* indices) {
    _ENTER("MPI_Testsome");
    for (int i = 0; i < outcount; i++)
      context_.complete_call(requests[indices[i]], start);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_barrier(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm) {
    _ENTER("MPI_Barrier");
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_BARRIER, 0, 0);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_bcast(int rank, otf2_time_t start, otf2_time_t stop, int count, mpi_type_t type,
                                            int root, comm_t comm) {
    _ENTER("MPI_Bcast");

    bool is_root = ranks_equivalent(rank, root, comm, &context_);
    int bytes = count_bytes(type, count);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_BCAST,
                       is_root ? bytes * get_comm_size(comm) : 0, // scorep multiplies the root's send by comm size
                       bytes);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_gather(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype,
                                             int recvcount, mpi_type_t recvtype, int root, comm_t comm) {
    _ENTER("MPI_Gather");

    bool is_root = ranks_equivalent(rank, root, comm, &context_);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_GATHER,
                       count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, recvcount) * get_comm_size(comm)); // scorep multiplies the root receive by comm size
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_gatherv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size, int sendcount,
                                              mpi_type_t sendtype, const int* recvcounts, mpi_type_t recvtype,
                                              int root, comm_t comm) {
    _ENTER("MPI_Gatherv");

    bool is_root = ranks_equivalent(rank, root, comm, &context_);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_GATHERV,
                       count_bytes(sendtype, sendcount),
                       is_root ? count_bytes(recvtype, array_sum(recvcounts, comm_size)) : 0);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scatter(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype,
                                              int recvcount, mpi_type_t recvtype, int root, comm_t comm) {
    _ENTER("MPI_Scatter");

    bool is_root = ranks_equivalent(rank, root, comm, &context_);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCATTER,
                       is_root ? count_bytes(sendtype, sendcount) * get_comm_size(comm) : 0, // scorep multiplies the root rank's send by commm size
                       count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scatterv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size,
                                               const int* sendcounts, mpi_type_t sendtype,
                                               int recvcount, mpi_type_t recvtype, int root, comm_t comm) {
    _ENTER("MPI_Scatterv");

    bool is_root = ranks_equivalent(rank, root, comm, &context_);

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCATTERV,
                       is_root ? count_bytes(sendtype, array_sum(sendcounts, comm_size)) : 0,
                       count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_scan(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                           mpi_type_t datatype, comm_t comm) {
    _ENTER("MPI_Scan");
    UNDEFINED_ROOT
    int comm_rank = get_comm_rank(comm, rank);
    int bytes = count_bytes(datatype, count);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCAN,
                       (get_comm_size(comm) - comm_rank - 1) * bytes,
                       (comm_rank + 1) * bytes);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allgather(int rank, otf2_time_t start, otf2_time_t stop, int sendcount, mpi_type_t sendtype,
                                                int recvcount, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Allgather");
    UNDEFINED_ROOT
    int comm_size =  get_comm_size(comm);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLGATHER,
                       comm_size * count_bytes(sendtype, sendcount),
                       comm_size * count_bytes(recvtype, recvcount));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allgatherv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size,
                                                 int sendcount, mpi_type_t sendtype,
                                                 const int* recvcounts, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Allgatherv");
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLGATHERV,
                       comm_size * count_bytes(sendtype, sendcount),
                       count_bytes(recvtype, array_sum(recvcounts, comm_size)));
    _LEAVE();
  }
  OTF2_WRITER_RESULT OTF2_Writer::mpi_alltoall(int rank, otf2_time_t start, otf2_time_t stop,
                                               int sendcount, mpi_type_t sendtype, int recvcount,
                                               mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Alltoall");
    UNDEFINED_ROOT
    int transmitted = get_comm_size(comm) * count_bytes(recvtype, recvcount);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLTOALL,
                       transmitted,
                       transmitted);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_alltoallv(int rank, otf2_time_t start, otf2_time_t stop, int comm_size,
                                                const int* sendcounts, mpi_type_t sendtype,
                                                const int* recvcounts, mpi_type_t recvtype, comm_t comm) {
    _ENTER("MPI_Alltoallv");
    UNDEFINED_ROOT
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLTOALLV,
                       count_bytes(sendtype, array_sum(sendcounts, comm_size)),
                       count_bytes(recvtype, array_sum(recvcounts, comm_size)));
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_reduce(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                             mpi_type_t type, int root, comm_t comm) {
    _ENTER("MPI_Reduce");

    int sent = count_bytes(type, count);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_REDUCE,
                       sent,
                       ranks_equivalent(rank, root, comm, &context_) ? sent * get_comm_size(comm) : 0);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_allreduce(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                mpi_type_t type, comm_t comm) {
    _ENTER("MPI_Allreduce");
    UNDEFINED_ROOT
    int bytes = count_bytes(type, count) * get_comm_size(comm);
    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLREDUCE,
                       bytes,
                       bytes);
    _LEAVE();
  }


  OTF2_WRITER_RESULT OTF2_Writer::mpi_reduce_scatter(int rank, otf2_time_t start, otf2_time_t stop, int comm_size,
                                                     const int* recvcounts, mpi_type_t type, comm_t comm) {
    _ENTER("MPI_Reduce_scatter");
    UNDEFINED_ROOT

    int sent = count_bytes(type, comm_size);
    int recv = comm_size * recvcounts[get_comm_rank(comm, rank)] * type_sizes_[type];

    COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_REDUCE_SCATTER,
                       sent,
                       recv);
    _LEAVE();
  }

  std::vector<int>& OTF2_Writer::get_group(comm_t comm) {
    return mpi_group_[mpi_comm_[comm].group];
  }

  bool OTF2_Writer::group_is_known(int group) {
    if (mpi_group_.find(group) == mpi_group_.end()) {
      logger(OWV_WARN, string("Unknown group (") + to_string(group) + ")");
      return false;
    } else return true;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_union(int rank, otf2_time_t start, otf2_time_t stop,
                                                  int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_union");
    _LEAVE();

    /** FIX ME
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


    COMM_LAMBDA_BEGIN();
    {
      auto g1_it = mpi_group_[context_.group_map[group1]].begin();
      auto g2_it = mpi_group_[context_.group_map[group2]].begin();
      auto g1_end = mpi_group_[context_.group_map[group1]].end();
      auto g2_end = mpi_group_[context_.group_map[group2]].end();
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      auto dest = std::back_inserter(mpi_group_[global_id]);

      context_.group_map[newgroup] = global_id;

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

    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_difference(int rank, otf2_time_t start, otf2_time_t stop,
                                                       int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_difference");
    _LEAVE();


    /**
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


    COMM_LAMBDA_BEGIN();
    {
      auto g1_it = mpi_group_[context_.group_map[group1]].begin();
      auto g2_it = mpi_group_[context_.group_map[group2]].begin();
      auto g1_end = mpi_group_[context_.group_map[group1]].end();
      auto g2_end = mpi_group_[context_.group_map[group2]].end();
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      auto dest = std::back_inserter(mpi_group_[global_id]);

      context_.group_map[newgroup] = global_id;

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
    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_intersection(int rank, otf2_time_t start, otf2_time_t stop,
                                                         int group1, int group2, int newgroup) {
    _ENTER("MPI_Group_intersection");
    _LEAVE();

    /**
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


    COMM_LAMBDA_BEGIN();
    {
      auto g1_it = mpi_group_[context_.group_map[group1]].begin();
      auto g2_it = mpi_group_[context_.group_map[group2]].begin();
      auto g1_end = mpi_group_[context_.group_map[group1]].end();
      auto g2_end = mpi_group_[context_.group_map[group2]].end();
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      auto dest = std::back_inserter(mpi_group_[global_id]);

      context_.group_map[newgroup] = global_id;

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
    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_incl(int rank, otf2_time_t start, otf2_time_t stop,
                                                 int group, int count, const int*ranks, int newgroup) {
    _ENTER("MPI_Group_incl");
    _LEAVE();

    /** FIX ME
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


    std::vector<int> r_vec(ranks, ranks + count);

    COMM_LAMBDA_BEGIN();
    {
      auto& old_group = mpi_group_[context_.group_map[group]];
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      context_.group_map[newgroup] = global_id;
      auto& new_group = mpi_group_[global_id];

      int g_size = old_group.size();
      for (int i = 0; i < count; i++) {
        if (r_vec[i] >= g_size) logger(OWV_ERROR, string("MPI_Group_incl found ranks out of bounds! " + to_string(ranks[i]) + " >= " + to_string(g_size)));
        else new_group.push_back(old_group[r_vec[i]]);
      }
    }
    COMM_LAMBDA_END();

    _LEAVE();
    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_excl(int rank, otf2_time_t start, otf2_time_t stop,
                                                 int group, int count, const int*ranks, int newgroup) {
    _ENTER("MPI_Group_excl");
    _LEAVE();

    /** FIX ME
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


    std::vector<int> r_vec(ranks, ranks + count);

    COMM_LAMBDA_BEGIN();
    {
      auto& old_group = mpi_group_[context_.group_map[group]];
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      context_.group_map[newgroup] = global_id;
      auto& new_group = mpi_group_[global_id];

      for(auto og_it = old_group.begin(); og_it != old_group.end(); og_it++) {
        if(std::find(r_vec.begin(), r_vec.end(), *og_it) == r_vec.end())
          new_group.push_back(*og_it);
      }
    }
    COMM_LAMBDA_END();

    _LEAVE();
    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_group_range_incl(int rank, otf2_time_t start, otf2_time_t stop,
                                                       int group, int count, int**ranges, int newgroup) {
    _ENTER("MPI_Group_range_incl");
    _LEAVE();

    /** FIX ME
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


    COMM_LAMBDA_BEGIN();
    {
      auto& old_group = _mpi_group[context_.group_map[group]];
      auto global_id = MPI_Comm_Struct::get_unique_group_id();
      context_.group_map[newgroup] = global_id;
      auto& new_group = _mpi_group[global_id];

      for (int i = 0; i < count; i++)
        for(int j = ranges[i][0]; j < ranges[i][1]; j+=ranges[i][2])
           new_group.push_back(old_group[j]);
    }
    COMM_LAMBDA_END();

    _LEAVE();
    */
  }

  bool OTF2_Writer::comm_is_known(comm_t comm) {
    if (mpi_comm_.find(comm) == mpi_comm_.end()) {
      logger(OWV_WARN, string("Unknown Communicator (") + to_string(comm) + ")");
      return false;
    }
    return true;
  }

  bool OTF2_Writer::type_is_known(mpi_type_t type) {
    if (type_sizes_.find(type) == type_sizes_.end()) {
      logger(OWV_WARN, string("Unknown Type (") + to_string(type));
      return false;
    }
    return true;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_dup(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm, comm_t newcomm) {
    _ENTER("MPI_Comm_dup");
    _LEAVE();

    /** FIX ME
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


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
    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_group(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm, int group) {
    _ENTER("MPI_Comm_group");
    _LEAVE();

    /** FIX ME
    COMM_LAMBDA_BEGIN();
    {
      // The comm points to a group at creation, alias this rank to that one.
      context_.group_map[group] = mpi_comm_[context_.comm_mapping[comm]].group;
    }
    COMM_LAMBDA_END();

    _LEAVE();
    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_create(int rank, otf2_time_t start, otf2_time_t stop, comm_t comm,
                                                  int group, comm_t newcomm) {
    _ENTER("MPI_Comm_create");
    _LEAVE();

    /** FIX ME
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


    if (!COMM_NULL_OR_ERROR(newcomm)) {
      COMM_LAMBDA_BEGIN();
      {
        // Ranks that participate in an MPI_Comm_create but do not fit in the group will return MPI_COMM_NULL
        auto& gbl_comm = context_.comm_mapping[comm];
        auto& group_vect = _mpi_group[context_.group_map[group]];
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
          nc.group = context_.group_map[group];

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
    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_comm_split(int rank, otf2_time_t start, otf2_time_t stop, comm_t oldcomm,
                                                 int key, int color, comm_t newcomm) {
    _ENTER("MPI_Comm_split");
    _LEAVE();

    /** FIX ME
    if(_comm_mode == COMM_MODE_BUILD_COMM_COMPLETE)


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
    */
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_contiguous(int rank, otf2_time_t start, otf2_time_t stop,
                                                      int count, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_contiguous");

    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string("MPI_type_contiguous failed"));
      return OTF2_WRITER_ERROR_UKNOWN_MPI_TYPE;
    }

    type_sizes_[newtype] = count_bytes(oldtype, count);

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_hvector(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                   int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hvector");
    mpi_t_vector_inner("MPI_Type_hvector", count, blocklength, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_vector(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                  int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_vector");
    mpi_t_vector_inner("MPI_Type_vector", count, blocklength, oldtype, newtype);
    _LEAVE();
  }

  void OTF2_Writer::mpi_t_vector_inner(const char* fname, int count, int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string(fname) + " failed");
      return;
    }
    type_sizes_[newtype] = count_bytes(oldtype, blocklength) * count;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_indexed(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                   const int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_indexed");
    mpi_t_indexed_inner("MPI_Type_indexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_hindexed(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                    const int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hindexed");
    mpi_t_indexed_inner("MPI_Type_hindexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_hindexed(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                           const int*lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_hindexed");
    mpi_t_indexed_inner("MPI_Type_hindexed", count, lengths, oldtype, newtype);
    _LEAVE();
  }

  void OTF2_Writer::mpi_t_indexed_inner(const char* name, int count, const int* lengths, mpi_type_t oldtype, mpi_type_t newtype) {
    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string(name) + " failed");
      return;
    }

    type_sizes_[newtype] = count_bytes(oldtype, array_sum(lengths, count));
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_struct(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                  const int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype) {
    _ENTER("MPI_Type_struct");
    mpi_t_struct_inner("MPI_Type_struct", count, blocklengths, oldtypes, newtype);
    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_struct(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                         const int* blocklengths, mpi_type_t* oldtypes, mpi_type_t newtype) {
    _ENTER("MPI_Type_create_struct");
    mpi_t_struct_inner("MPI_Type_create_struct", count, blocklengths, oldtypes, newtype);
    _LEAVE();
  }

  void OTF2_Writer::mpi_t_struct_inner(const char* fname, int count, const int* blocklengths,
                                       mpi_type_t* oldtypes, mpi_type_t newtype) {
    int sum = 0;
    for(int i = 0; i < count; i++) {
      if (!type_is_known(oldtypes[i])){
        logger(OWV_ERROR, string(fname) + "failed");
        return;
      }
      sum += count_bytes(oldtypes[i], blocklengths[i]);
    }

    type_sizes_[newtype] = sum;
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_subarray(int rank, otf2_time_t start, otf2_time_t stop,
                                                           int ndims, const int* subsizes, mpi_type_t oldtype, mpi_type_t newtype) {
    _ENTER("MPI_Type_create_subarray");

    if (!type_is_known(oldtype)){
      logger(OWV_ERROR, string("MPI_Type_create_subarray failed"));
      return OTF2_WRITER_ERROR_UKNOWN_MPI_TYPE;
    }

    type_sizes_[newtype] = count_bytes(oldtype, array_sum(subsizes, ndims));

    _LEAVE();
  }

  OTF2_WRITER_RESULT OTF2_Writer::mpi_type_create_hvector(int rank, otf2_time_t start, otf2_time_t stop, int count,
                                                          int blocklength, mpi_type_t oldtype, mpi_type_t newtype) {
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
    auto c = mpi_comm_[comm];
    if (c.ndim == 0)
      return mpi_group_[c.group].size();
    else {
      int sum = 0;
      for(int i = 0; i < c.ndim; i++) sum *= c.dims[i];
      return sum;
    }
  }

  uint64_t OTF2_Writer::count_bytes(mpi_type_t type, uint64_t count){
   if (type_sizes_.find(type) == type_sizes_.end()) {
     logger(OWV_ERROR, string("Unkown data type (") + to_string((int)type) + "). Assuming 4 bytes in size.");
     return 4*count;
   } else return type_sizes_[type]*count;
  }

  unsigned long OTF2_Writer::hash_group(std::vector<int>& group) {
    unsigned long hash = ~0;
    std::hash<int> hasher;

    for(auto rank : group)
      hash ^= hasher(rank) + 0x9e3779b9 + (hash<<6) + (hash>>2);

    return hash;
  }

  int MPI_Comm_Struct::_comm_uid = OTF2_Writer::MPI_COMM_USER_OFFSET;
  int MPI_Comm_Struct::_group_uid = OTF2_Writer::USER_DEF_COMM_GROUP_OFFSET;
}
