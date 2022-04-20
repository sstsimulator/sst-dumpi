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

AM_LDFLAGS = 
library_includedir=$(includedir)/dumpi/libundumpi
library_include_HEADERS = \
    bindings.h callbacks.h dumpistate.h freedefs.h libundumpi.h
lib_LTLIBRARIES = libundumpi.la
libundumpi_la_SOURCES = libundumpi.c callbacks.c bindings.c
libundumpi_la_LDFLAGS = -version-info @DUMPI_LIBVERSION@
libundumpi_la_LIBADD = ../common/libdumpi_common.la
