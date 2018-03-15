#ifndef DUMPI2OTF2_H
#define DUMPI2OTF2_H

#include <dumpi/libundumpi/callbacks.h>
#include <dumpi/common/constants.h>
#include <otf2/otf2.h>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <vector>
#include <climits>

/*
 * OTF2's time can be defined arbitrarily. We use nanoseconds because that
 * is dumpi's time granularity.
 */
#define DUPMI_TO_OTF2_TIMESTAMP(dtime) \
  ((OTF2_TimeStamp)((((uint64_t)dtime.sec) * 1E9) + (uint64_t)dtime.nsec))

/** Common preample for a dumpi callback */
#define DUMPI_ENTERING()                                                      \
  const char* fname = __func__ + 7;                                           \
  DumpiArgs* args = (DumpiArgs*)uarg;                                         \
  OTF2_EvtWriter_Enter(args->writer,                                          \
                       nullptr,                                               \
                       DUPMI_TO_OTF2_TIMESTAMP(wall->start),                  \
                       args->region[std::string(fname)]);

/** Common postamble for dumpi callback*/
#define DUMPI_RETURNING()                                                    \
  OTF2_EvtWriter_Leave(args->writer,                                         \
                       nullptr,                                              \
                       DUPMI_TO_OTF2_TIMESTAMP(wall->stop),                  \
                       args->region[std::string(fname)]);                    \
  args->event_count[args->rank] += 2;                                        \
  return 1;

/** Wrapper for collective callbacks */
#define COLLECTIVE_WRAPPER(collective, root, sent, received)                                                                                \
  OTF2_EvtWriter_MpiCollectiveBegin(args->writer, nullptr, DUPMI_TO_OTF2_TIMESTAMP(wall->start));                                           \
  OTF2_EvtWriter_MpiCollectiveEnd(args->writer, nullptr, DUPMI_TO_OTF2_TIMESTAMP(wall->stop), collective, prm->comm, root, sent, received); \
  args->event_count[args->rank] += 2;

// Looks up the size of a type and returns a bytecount
#define BYTE_COUNT(type, count) (args->type_sizes[type]*count)

// Wrapper that will skip translation of callbacks that SST/macro does not use
#define IF_SKIP_UNUSED_BEGIN(uarg) if (((DumpiArgs*)uarg)->program_options.skip_unused_calls) {
#define IF_SKIP_UNUSED_END() }

/** Prints a message saying method is not used */
#define WARN_UNUSED(verbose)                                \
  if(verbose) {                                             \
    printf("%s callback is not implemented\n", fname); \
  }

extern "C" {

#define DUMPI_UARG (void*)0xFF00FF00 /* debugging sanity check */

/**
 * The address mapping is stored in a shared container.
 */
typedef struct d2o2_addrmap {
  int        count;
  uint64_t  *address;
  char     **name;
} d2o2_addrmap;

/** The shared container for address mapping */
extern d2o2_addrmap *d2o2_addr;

/** Define all our callbacks. */
void set_callbacks(libundumpi_callbacks *cbacks);

}

/**
 * Provides string to id mappings
 */
class OTF2DefTable {
public:
  OTF2DefTable();
  int map(const std::string string);
  const std::string map(int index);

  // Operator overload sugar
  const std::string operator[] (int index);
  int operator[] (const std::string string);
  int size();
  bool added_last_lookup();
private:
  std::unordered_map<std::string, int> _map;
  int _counter = 0;
  bool _added_last_lookup;
};

/**
 * Program options struct.
 */
typedef struct d2o2opt {
  int verbose, help;
  const char *dumpi_archive;
  std::string dumpi_meta;
  std::vector<std::string> dumpi_bin;
  const char *output_archive;
  bool skip_unused_calls;
} d2o2opt;

// Represents an MPI communicator
class MPI_comm {
public:
  MPI_comm(): name(""), parent(0), id(0) {}
  MPI_comm(std::string name, int parent, int id, int group) : name(name), parent(parent), id(id), group(group){}
  MPI_comm(const char* name, int parent, int id, int group) : name(name), parent(parent), id(id), group(group){}

  int parent;
  int id;
  int group;
  std::string name;

  // STD map iterators will order their keys in ascending value
  // The value points to the parent's rank.
  // std::map<int,int> group_map;
};

/**
 * Struct a passed into callbacks.
 */
struct DumpiArgs {
  DumpiArgs(): start_time(INT_MAX), stop_time(INT_MIN) {
    auto root_comm = MPI_comm("MPI_COMM_WORLD", 0, DUMPI_COMM_WORLD, 0);
    mpi_comm[0] = root_comm;

    //TODO define global group
  }

  OTF2DefTable string;
  OTF2DefTable system_tree_node;
  OTF2DefTable region;
  //OTF2DefTable location;
  //OTF2DefTable location_group;
  std::unordered_map<int, std::vector<int>> mpi_group;
  std::unordered_map<int, MPI_comm> mpi_comm;
  //std::unordered_map<int, int> mpi_type_to_size;
  std::unordered_map<int, int> event_count;
  std::vector<int> type_sizes;

  OTF2_Archive* archive;
  OTF2_EvtWriter* writer;
  int rank;
  int num_ranks;
  d2o2opt program_options;
  OTF2_TimeStamp start_time;
  OTF2_TimeStamp stop_time;
};

#endif // DUMPI2OTF2_H
