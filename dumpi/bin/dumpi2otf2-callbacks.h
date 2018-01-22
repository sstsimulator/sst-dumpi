#ifndef DUMPI2OTF2CALLBACKS_H
#define DUMPI2OTF2CALLBACKS_H

#include <dumpi/libundumpi/libundumpi.h>


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

#endif // DUMPI2OTF2CALLBACKS_H
