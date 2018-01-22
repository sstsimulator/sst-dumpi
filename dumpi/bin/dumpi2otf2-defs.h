#ifndef DUMPI2OTF2_H
#define DUMPI2OTF2_H

#include <otf2/otf2.h>

/** Common preample for a dumpi callback */
#define DUMPI_ENTERING()                              \
  const char* func_name = __func__ + 7;               \
  OTF2_EvtWriter* evt_writer = (OTF2_EvtWriter*)uarg; \
  printf("Entered\n");

/** Common postamble for dumpi callback*/
#define DUMPI_RETURNING()  \
  printf("Leaving\n");                           \
  return 1

/** Prints a message saying method is not used */
#define WARN_UNUSED(verbose)                                \
  if(verbose) {                                             \
    printf("%s callback is not implemented\n", func_name); \
  }

struct OTF2Defs;
class OTF2DefTable;

#endif // DUMPI2OTF2_H
