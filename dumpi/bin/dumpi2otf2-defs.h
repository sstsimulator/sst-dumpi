#ifndef DUMPI2OTF2_H
#define DUMPI2OTF2_H

#include <otf2/otf2.h>
#include <unordered_map>
#include <string>

/*
 * OTF2's time is arbitrarily assigned in the def file. We use nanoseconds because that
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
                       args->region[std::string(fname)]);                     \
  /*printf("Entered\n");*/

/** Common postamble for dumpi callback*/
#define DUMPI_RETURNING()                                                    \
  OTF2_EvtWriter_Leave(args->writer,                                         \
                       nullptr,                                              \
                       DUPMI_TO_OTF2_TIMESTAMP(wall->stop),                  \
                       args->region[std::string(fname)]);                    \
  /*printf("Leaving\n");*/                                                   \
  return 1;

#define COLLECTIVE_WRAPPER(collective, root, sent, received)                                                                           \
  OTF2_EvtWriter_MpiCollectiveBegin(args->writer, nullptr, DUPMI_TO_OTF2_TIMESTAMP(wall->start));                                           \
  OTF2_EvtWriter_MpiCollectiveEnd(args->writer, nullptr, DUPMI_TO_OTF2_TIMESTAMP(wall->stop), collective, prm->comm, root, sent, received);

// TODO: figure out how to get type size from dumpi to count bytes. Assumes every type is 1 byte for now.
#define BYTE_COUNT(type, count) (count)

/** Prints a message saying method is not used */
#define WARN_UNUSED(verbose)                                \
  if(verbose) {                                             \
    printf("%s callback is not implemented\n", fname); \
  }

class OTF2DefTable {
public:
  OTF2DefTable();
  int map(const std::string string);
  const std::string map(int index);

  // Operator overload sugar
  const std::string operator[] (int index);
  int operator[] (const std::string string);
  int size();
private:
  std::unordered_map<std::string, int> _map;
  int _counter = 0;
};

struct DumpiArgs {
  OTF2DefTable string;
  OTF2DefTable system_tree_node;
  OTF2DefTable region;
  //OTF2DefTable group;
  //OTF2DefTable location;
  //OTF2DefTable location_group;
  OTF2DefTable comm;

  OTF2_Archive* archive;
  OTF2_EvtWriter* writer;
};

#endif // DUMPI2OTF2_H
