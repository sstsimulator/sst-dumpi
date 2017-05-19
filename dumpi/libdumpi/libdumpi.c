/**
Copyright 2009-2017 National Technology and Engineering Solutions of Sandia, 
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S.  Government 
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly 
owned subsidiary of Honeywell International, Inc., for the U.S. Department of 
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2017, NTESS

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of Sandia Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Questions? Contact sst-macro-help@sandia.gov
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