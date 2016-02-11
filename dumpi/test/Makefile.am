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

EXTRA_DIST = coll.h manip.h p2p.h probe.h \
	run_testmpi.sh run_testf77.sh run_testf90.sh
AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir)

noinst_PROGRAMS = testmpi testthreads
TESTS = run_testmpi.sh

if WITH_MPIF77
  noinst_PROGRAMS += testf77
  TESTS += run_testf77.sh
endif

if WITH_MPIF90
  noinst_PROGRAMS += testf90
  TESTS += run_testf90.sh
endif

testmpi_SOURCES = testmpi.c p2p.c coll.c manip.c
testmpi_LDADD = ../libdumpi/libdumpi.la

testf77_SOURCES = testf77.f
testf77_LDADD = ../libdumpi/libdumpif77.la

testf90_SOURCES = testf90.f90
testf90_LDADD = ../libdumpi/libdumpif90.la

testthreads_SOURCES = testthreads.c testthreads-multiple.c \
	testthreads-serialized.c testthreads-funneled.c
testthreads_LDADD = ../libdumpi/libdumpi.la
