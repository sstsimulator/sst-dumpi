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

EXTRA_DIST = comm.h dumpi2ascii-callbacks.h dumpi2ascii-defs.h dumpi2dumpi.h \
             group.h metadata.h otfcomplete.h otfwriter.h \
             sharedstate-commconstruct.h sharedstate.h timeutils.h trace.h \
             type.h type.h dumpistats-binbase.h dumpistats-timebin.h \
             dumpistats-gatherbin.h dumpistats-callbacks.h \
             dumpistats-handlers.h \
             test_dumpi2ascii.sh test_dumpi2dumpi.sh

TESTS = test_dumpi2ascii.sh test_dumpi2dumpi.sh

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir)
AM_LDFLAGS = 
bin_PROGRAMS = dumpi2ascii dumpi2dumpi dumpistats ascii2dumpi

if WITH_OTF
  bin_PROGRAMS += dumpi2otf  

  dumpi2otf_SOURCES = dumpi2otf.cc metadata.cc sharedstate.cc \
	  sharedstate-commconstruct.cc trace.cc otfwriter.cc otfcomplete.cc
  dumpi2otf_LDADD = ../libundumpi/libundumpi.la $(OTFDIR) -lotf
endif

dumpi2ascii_SOURCES = dumpi2ascii.c dumpi2ascii-callbacks.c
dumpi2ascii_LDADD = ../libundumpi/libundumpi.la

ascii2dumpi_SOURCES = ascii2dumpi.c
ascii2dumpi_LDADD = ../libundumpi/libundumpi.la

dumpi2dumpi_SOURCES = dumpi2dumpi.c dumpi2dumpi-opts.c dumpi2dumpi-help.c \
	dumpi2dumpi-meta.c dumpi2dumpi-callbacks.c
dumpi2dumpi_LDADD = ../libundumpi/libundumpi.la

dumpistats_SOURCES = dumpistats.cc dumpistats-timebin.cc \
	dumpistats-gatherbin.cc dumpistats-callbacks.cc dumpistats-handlers.cc \
	trace.cc metadata.cc sharedstate.cc sharedstate-commconstruct.cc
dumpistats_LDADD = ../libundumpi/libundumpi.la
