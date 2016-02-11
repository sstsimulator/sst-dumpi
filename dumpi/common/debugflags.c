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

#include <dumpi/common/debugflags.h>
#include <dumpi/dumpiconfig.h>

#ifndef DUMPI_DEBUG_SETTING
/*#define DUMPI_DEBUG_SETTING DUMPI_DEBUG_ALL */
#define DUMPI_DEBUG_SETTING DUMPI_DEBUG_NONE
#endif /* ! DUMPI_DEBUG_SETTING */

uint64_t dumpi_debug = DUMPI_DEBUG_SETTING;
