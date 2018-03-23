#ifndef DUMPI2OTF2_H
#define DUMPI2OTF2_H

#include <dumpi/libundumpi/callbacks.h>
#include <dumpi/libotf2dump/otf2writer.h>
#include <string>
#include <vector>

extern "C" {

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

/**
 * Struct a passed into callbacks.
 */
struct DumpiArgs {
  d2o2opt program_options;
  dumpi::OTF2_Writer otf2_writer;
};

#endif // DUMPI2OTF2_H
