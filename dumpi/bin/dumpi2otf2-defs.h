#ifndef DUMPI2OTF2_H
#define DUMPI2OTF2_H

#include <otf2/otf2.h>
#include <unordered_map>
#include <string>

/** Common preample for a dumpi callback */
#define DUMPI_ENTERING()                                                      \
  const char* fname = __func__ + 7;                                           \
  DumpiArgs* args = (DumpiArgs*)uarg;                                         \
  OTF2_EvtWriter_Enter(args->writer,                                          \
                       nullptr,                                               \
                       (OTF2_TimeStamp)((((uint64_t)wall->start.sec) << 32) | \
                                          (uint64_t)wall->start.nsec),        \
                       args->region[std::string(fname)]);                     \
  printf("Entered\n");

/** Common postamble for dumpi callback*/
#define DUMPI_RETURNING()                                                    \
  OTF2_EvtWriter_Leave(args->writer,                                         \
                       nullptr,                                              \
                       (OTF2_TimeStamp)((((uint64_t)wall->stop.sec) << 32) | \
                       (uint64_t)wall->stop.nsec),                           \
                       args->region[std::string(fname)]);                    \
  printf("Leaving\n");                                                       \
  return 1;

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
