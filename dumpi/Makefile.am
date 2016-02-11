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

SUBDIRS = common

if WITH_LIBDUMPI
  SUBDIRS += libdumpi 
endif

if WITH_LIBUNDUMPI
  SUBDIRS += libundumpi
endif

if WITH_BIN
  SUBDIRS += bin
endif

if WITH_TEST
  SUBDIRS +=  test
endif

AM_CPPFLAGS = -I$(top_builddir) -I$(top_srcdir)
library_includedir=$(includedir)/dumpi
library_include_HEADERS = dumpiconfig.h dumpiconfig-generated.h
