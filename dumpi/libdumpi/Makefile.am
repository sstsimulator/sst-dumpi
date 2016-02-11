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
#AM_LDFLAGS = -lrt
library_includedir=$(includedir)/dumpi/libdumpi
library_include_HEADERS = \
    callprofile-addrset.h callprofile.h         data.h               \
    fused-bindings.h      init.h                libdumpi.h           \
    mpibindings-maps.h    mpibindings.h         mpibindings-utils.h  \
    tof77.h

lib_LTLIBRARIES = libdumpi.la

if WITH_MPIF77
lib_LTLIBRARIES += libdumpif77.la
endif

if WITH_MPIF90
lib_LTLIBRARIES += libdumpif90.la
endif

libdumpi_la_SOURCES = data.c init.c libdumpi.c callprofile.c \
	callprofile-addrset.c mpibindings-utils.c mpibindings-maps.c
	
if WITH_MPI_TWO
libdumpi_la_SOURCES += mpibindings2.c
endif

if WITH_MPI_THREE
libdumpi_la_SOURCES += mpibindings3.c
endif	
	
	
libdumpi_la_LDFLAGS = -version-info @DUMPI_LIBVERSION@
libdumpi_la_LIBADD = ../common/libdumpi_common.la

libdumpif77_la_SOURCES = tof77.c fromf77.f
libdumpif77_la_LDFLAGS = -version-info @DUMPI_LIBVERSION@
libdumpif77_la_LIBADD = ./libdumpi.la

libdumpif90_la_SOURCES = tof77.c fromf90.f90
libdumpif90_la_LDFLAGS = -version-info @DUMPI_LIBVERSION@
libdumpif90_la_LIBADD = ./libdumpi.la
