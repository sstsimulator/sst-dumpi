#ifndef DUMPI2OTF2_H
#define DUMPI2OTF2_H

#include <dumpi/libundumpi/callbacks.h>
#include <dumpi/libotf2dump/otf2writer.h>
#include <string>
#include <vector>

extern "C" {
  /** Define all our callbacks. */
  void set_callbacks(libundumpi_callbacks *cbacks);
  void set_first_pass_callbacks(libundumpi_callbacks *cbacks);

  /**
   * Program options struct.
   */
  typedef struct d2o2opt {
    int verbose, help;
    std::string dumpi_folder;
    std::string dumpi_meta;
    std::vector<std::string> dumpi_bin;
    std::string output_archive;
    bool print_progress = false;
  } d2o2opt;
}
#endif // DUMPI2OTF2_H
