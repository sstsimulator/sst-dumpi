/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
 */

#include <mpi.h>
#include <dumpi/libdumpi/libdumpi.h>
#include <dumpi/libdumpi/init.h>
#include <dumpi/libdumpi/data.h>
#include <dumpi/common/io.h>
#include <dumpi/common/funcs.h>
#include <dumpi/common/settings.h>
#include <assert.h>

/*
 * Turn off all MPI profiling (call counts are still gathered.
 */
void libdumpi_disable_profiling() {
  libdumpi_init();
  dumpi_global->output->function[DUMPI_ALL_FUNCTIONS] = DUMPI_DISABLE;
}

/*
 * Turn MPI profiling (back) to what it was before disable_profiling.
 */
void libdumpi_enable_profiling() {
  libdumpi_init();
  dumpi_global->output->function[DUMPI_ALL_FUNCTIONS] = DUMPI_ENABLE;
}
  
/*
 * Set profiling preferences for the given MPI function.
 * This can be done when profiling is disabled; the new value will take
 * effect when libdumpi_enable_profiling is called.
 * Valid values for setting are DUMPI_DISABLE, DUMPI_SUCCESS, and DUMPI_ENABLE
 */
void libdumpi_set_profiling(dumpi_function fun, dumpi_setting setting) {
  libdumpi_init();
  assert(fun <= DUMPI_ALL_FUNCTIONS);
  dumpi_global->output->function[fun] = setting;
}

/*
 * Specify the fileroot for profile output.
 * This call is only respected if it is made before MPI_Init.
 */
void libdumpi_set_output(const char *froot) {
  char *tmpfroot;
  libdumpi_init();
  if(dumpi_global->profile->file == NULL && froot != NULL) {
    if(dumpi_global->file_root) {
      tmpfroot = (char*)dumpi_global->file_root;
      free(tmpfroot);
      dumpi_global->file_root = NULL;
    }
    tmpfroot = (char*)malloc(strlen(froot)+1);
    assert(tmpfroot != NULL);
    strcpy(tmpfroot, froot);
    dumpi_global->file_root = tmpfroot;
  }
}

/*
 * Specify whether status information should be output in the
 * trace file.  This call can be made at any time (e.g. when trying
 * to figure out status problems in select calls).
 */
void libdumpi_set_status_output(dumpi_setting setting) {
  libdumpi_init();
  dumpi_global->output->statuses = setting;
}

/*
 * Access the count of MPI calls made to date.
 */
const uint32_t* libdumpi_call_counts() {
  libdumpi_init();
  return dumpi_global->footer->call_count;
}
