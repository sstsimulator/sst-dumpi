#
#   This file is part of DUMPI: 
#                The MPI profiling library from the SST suite.
#   Copyright (c) 2009-2022 NTESS.
#   This software is distributed under the BSD License.
#   Under the terms of Contract DE-NA0003525 with NTESS,
#   the U.S. Government retains certain rights in this software.
#   For more information, see the LICENSE file in the top 
#   SST/macroscale directory.
#
include $(top_srcdir)/Makefile.common

SUBDIRS = common

if WITH_LIBDUMPI
  SUBDIRS += libdumpi 
endif

if WITH_LIBUNDUMPI
  SUBDIRS += libundumpi
endif

if WITH_OTF2
  SUBDIRS += libotf2dump
endif

if WITH_BIN
  SUBDIRS += bin
endif

if WITH_TEST
  SUBDIRS +=  test
endif


library_includedir=$(includedir)/dumpi
library_include_HEADERS = dumpiconfig.h 
nodist_library_include_HEADERS = dumpiconfig-generated.h

