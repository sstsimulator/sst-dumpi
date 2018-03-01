#ifndef DUMPI2OTF2_H
#define DUMPI2OTF2_H

#include <dumpi/libundumpi/callbacks.h>
#include <otf2/otf2.h>
#include <unordered_map>
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

// Looks up the size of a type and returns a bytecount. If lookup fails, It will print and error and assume the size if 4
#define BYTE_COUNT(type, count) ((args->mpi_type_to_size.find(type) == args->mpi_type_to_size.end()) ? \
                                 printf("Error: %i type unknown assuming 4 bytes", type),4*count :     \
                                 args->mpi_type_to_size[type]*count)

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
} d2o2opt;


/**
 * Struct a passed into callbacks.
 */
struct DumpiArgs {
  DumpiArgs(): start_time(INT_MAX), stop_time(INT_MIN) {}

  OTF2DefTable string;
  OTF2DefTable system_tree_node;
  OTF2DefTable region;
  //OTF2DefTable group;
  //OTF2DefTable location;
  //OTF2DefTable location_group;
  OTF2DefTable comm;
  std::unordered_map<int, int> mpi_type_to_size;
  std::unordered_map<int, int> event_count;

  OTF2_Archive* archive;
  OTF2_EvtWriter* writer;
  int rank;
  int num_ranks;
  d2o2opt program_options;
  OTF2_TimeStamp start_time;
  OTF2_TimeStamp stop_time;
};

#endif // DUMPI2OTF2_H
