#
#   This file is part of DUMPI: 
#                The MPI profiling library from the SST suite.
#   Copyright (c) 2009 Sandia Corporation.
#   This software is distributed under the BSD License.
#   Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#   the U.S. Government retains certain rights in this software.
#   For more information, see the LICENSE file in the top 
#   SST/macroscale directory.
#

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir)
AM_LDFLAGS =
library_includedir=$(includedir)/dumpi/common
library_include_HEADERS = \
    argtypes.h    debugflags.h  funclabels.h  gettime.h     io.h        \
    perfctrs.h    settings.h    constants.h   dumpiio.h     funcs.h     \
    hashmap.h     iodefs.h      perfctrtags.h types.h       byteswap.h

libdumpi_common_la_SOURCES = types.c funcs.c io.c dumpiio.c funclabels.c \
	gettime.c constants.c perfctrs.c perfctrtags.c iodefs.c debugflags.c
libdumpi_common_la_LDFLAGS = 
noinst_LTLIBRARIES = libdumpi_common.la
