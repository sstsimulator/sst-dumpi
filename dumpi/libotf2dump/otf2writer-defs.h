#ifndef OTF2WRITERDEFS_H
#define OTF2WRITERDEFS_H

#include <otf2/otf2.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <functional>
#include <set>
#include <list>

namespace dumpi {
typedef int32_t comm_t;
typedef int32_t request_t;
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

enum COMM_EVENT_TYPE {
  CET_COMM_SPLIT = 0,
  CET_COMM_CREATE
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
  MPI_Comm_Struct(): name(""), parent(0), id(0), ndim(0) {}
  MPI_Comm_Struct(std::string name, unsigned int parent, int id, int group) : name(name), parent(parent), id(id), group(group), ndim(0){}
  MPI_Comm_Struct(const char* name, unsigned int parent, int id, int group) : name(name), parent(parent), id(id), group(group), ndim(0){}

  unsigned int parent;
  int id;
  int group;
  int ndim;
  std::vector<int> dims;
  std::string name;

  // helper for getting a unique id for groups and comms whose IDs are not specified by the trace.
  static int get_unique_comm_id() {return _comm_uid++;}
  static int get_unique_group_id() {return _group_uid++;}
private:
  static int _comm_uid;
  static int _group_uid;
};

/// Stores rank-specific states
struct RankContext {
  int rank = -1;
  int event_count = 0;
  OTF2_EvtWriter* evt_writer = nullptr;
  std::unordered_map<int, irecv_capture> irecv_requests;
  std::unordered_map<int, REQUEST_TYPE> request_type;
  std::unordered_map<comm_t, comm_t> comm_mapping; // Tracks arbitary local communicator IDs
  std::unordered_map<int, int> group_map; // maps arbitrary local group IDs go global ones (which an arbitrary construct created when comms are created).

  void incomplete_call(int request_id, REQUEST_TYPE type);
  void complete_call(request_t request_id, uint64_t timestamp);

  // returns the number of incomplete events
  int dispose(OTF2_Archive* archive);
};

struct CommAction {
  std::function<void()> action;
  otf2_time_t end_time; // used to handle ordering

  bool operator<(CommAction const &ca) const { return ca.end_time < end_time; }
};

// MPI_Comm_split is a collective that takes a parent collective and shards it into several children
// An instance of a split collective be identified across ranks by hashing the parent communicator, and the number of times the rank has previously done a split on that communicator.
// Tracking a split is done to know when the children communicators are done constructing.
struct CommEventIdentifier {
public:
  COMM_EVENT_TYPE comm_event_type;
  comm_t id;
  int event_number;
};

struct CommEventIdentifierHasher {
  long operator()(const dumpi::CommEventIdentifier& csi) const
  {
      return (std::hash<dumpi::comm_t>()(csi.id) ^ std::hash<int>()(csi.event_number) ^ std::hash<COMM_EVENT_TYPE>()(csi.comm_event_type));
  }
};

struct CommCreateIdentifier {
  comm_t id;
  int event_number;
  unsigned long group_hash;
};

struct CommCreateIdentifierHasher {
  long operator()(const dumpi::CommCreateIdentifier& cci) const
  {
    return std::hash<dumpi::comm_t>()(cci.id)
        ^ std::hash<int>()(cci.event_number)
        ^ cci.group_hash;
  }
};

// A base class for collectives that build communicators or groups. Used in
// the formation of mappings between local rank identifiers and global communicator groups
template<typename T>
class CollectiveIdRemapper {
public:
  const std::set<T> list_completed() {return completed; }
  virtual std::tuple<MPI_Comm_Struct, std::vector<int>> get_completed(T new_id) = 0;
  virtual std::vector<T> get_remapping(T new_id) = 0;
  virtual void clear(T new_id) = 0;

protected:
  std::set<T> completed;
  //std::unordered_map<T, std::unordered_map<int, int>> event_counter;

  struct RankMetadata {
  public:
    int global_rank;
    int parent_rank;
    comm_t local_new_comm_id;
    comm_t global_new_comm_id;
  };
};

class CommCreateConstructor : public CollectiveIdRemapper<comm_t> {
public:
  std::tuple<MPI_Comm_Struct, std::vector<int>> get_completed(comm_t comm);
  std::vector<comm_t> get_remapping(comm_t new_comm); // For a given communicator, creates a list that maps the ranks' arbitrary local communicator IDs to the global one
  void clear(comm_t new_comm);
  void add_call(int global_rank, int parent_rank, comm_t comm, int group_size, unsigned long group_hash, comm_t new_comm);

private:
  //struct RankMetadata : public CollectiveIdRemapper<comm_t>::RankMetadata {};

  struct CommCreateContext {
    int remaining_ranks;
    comm_t global_id = -1;
  };

  struct RankMetadata {
  public:
    int global_rank;
    comm_t local_comm_id;
  };

  std::unordered_map<CommCreateIdentifier, CommCreateContext, CommCreateIdentifierHasher> incomplete_comm_creates;
  std::unordered_map<comm_t, MPI_Comm_Struct> new_comm_metadata;
  std::map<comm_t, std::vector<RankMetadata>> new_comm_group;
  std::unordered_map<comm_t, std::unordered_map<int, std::unordered_map<int, int>>> event_counter;
};

// Handles the correct ordering of ranks and communicators in a comm_split
class CommSplitConstructor : public CollectiveIdRemapper<comm_t> {
public:
  std::tuple<MPI_Comm_Struct, std::vector<int>> get_completed(comm_t comm);
  std::vector<comm_t> get_remapping(comm_t new_comm); // For a given communicator, creates a list that maps the ranks' arbitrary local communicator IDs to the global one
  void clear(comm_t new_comm);

  void add_call(int global_rank, int parent_rank, int key, int color, comm_t old_comm, comm_t new_comm, int old_comm_size);
private:
  struct RankMetadata : public CollectiveIdRemapper<comm_t>::RankMetadata {
    int key;
  };

  struct CommSplitContext {
    int parent_size;
    int remaining_ranks;
    // A list of child communicators identified by the color parameter
    std::unordered_map<int, comm_t> color_to_comm_id;
  };

  // Some legwork is done here to count the number of ranks that have participated in a given MPI_Comm_split reduction
  // So the runtime knows when it finishes and propegate the information to child communicators.
  std::unordered_map<CommEventIdentifier, CommSplitContext, CommEventIdentifierHasher> incomplete_comm_splits;
  std::map<comm_t, std::list<RankMetadata>> new_comm_group;
  std::unordered_map<comm_t, MPI_Comm_Struct> new_comm_metadata;
  std::unordered_map<comm_t, std::unordered_map<int, int>> event_counter;
};
}

bool operator==(const dumpi::CommEventIdentifier& rhs, const dumpi::CommEventIdentifier& lhs);
bool operator==(const dumpi::CommCreateIdentifier& rhs, const dumpi::CommCreateIdentifier& lhs);

#endif // OTF2WRITERDEFS_H
