dnl
dnl   This file is part of DUMPI: 
dnl                The MPI profiling library from the SST suite.
dnl

dnl Copyright 2009-2022 National Technology and Engineering Solutions of Sandia,
dnl LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S. Government
dnl retains certain rights in this software.
dnl
dnl Sandia National Laboratories is a multimission laboratory managed and operated
dnl by National Technology and Engineering Solutions of Sandia, LLC., a wholly
dnl owned subsidiary of Honeywell International, Inc., for the U.S. Department of
dnl Energy's National Nuclear Security Administration under contract DE-NA0003525.
dnl
dnl Copyright (c) 2009-2022, NTESS
dnl
dnl All rights reserved.
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions are
dnl met:
dnl
dnl * Redistributions of source code must retain the above copyright
dnl   notice, this list of conditions and the following disclaimer.
dnl
dnl * Redistributions in binary form must reproduce the above
dnl   copyright notice, this list of conditions and the following
dnl   disclaimer in the documentation and/or other materials provided
dnl   with the distribution.
dnl
dnl * Neither the name of the copyright holder nor the names of its
dnl   contributors may be used to endorse or promote products derived
dnl   from this software without specific prior written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
dnl "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
dnl LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
dnl A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
dnl OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
dnl LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
dnl DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
dnl THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
dnl (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
dnl OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

dnl Version info, used both in library versioning and inside dumpi.
m4_define([DUMPI_VERSION_TAG], 12)
m4_define([DUMPI_SUBVERSION_TAG], 0)
m4_define([DUMPI_SUBSUBVERSION_TAG], 0)
# Enable this for releases
dnl m4_define([DUMPI_SNAPSHOT_TAG])
# Enable this for development snapshots (should generally be enabled)
m4_define([DUMPI_SNAPSHOT_TAG], )
m4_define([DUMPI_ACVERSION_TAG], 
          [DUMPI_VERSION_TAG.DUMPI_SUBVERSION_TAG.DUMPI_SUBSUBVERSION_TAG''DUMPI_SNAPSHOT_TAG])

dnl Libtool library versioning is
dnl a useful idea implemented in a confusing way.
dnl -version-info A:B:C means:
dnl     Major version A
dnl     subversion B (backward compatible for all A:X where X <= B)
dnl     and A is backward compatible to major version A-C
dnl which means that C <= A,
dnl and C == 0 means no major-version backward compatibility.
dnl First use of library versioning implemented in version 0.6.4,
dnl labelled as library version 1.0.0 
dnl (because library versioning has to start somewhere).
dnl Dumpi version 0.7.0 given libversion 2.0.0 because call order has
dnl been changed for many common IO routines (now consistently passing
dnl dumpi_profile as the first argument for IO calls).
dnl Dumpi version 1.0.0 given libversion 3.0.0 because of several new 
dnl API calls, bug fixes, and renamings.
m4_define([DUMPI_MAJOR_LIBVERS], 12)
m4_define([DUMPI_MINOR_LIBVERS], 0)
m4_define([DUMPI_BACKW_LIBVERS], 0)
m4_define([DUMPI_LIBVERSION_TAG],
          [DUMPI_MAJOR_LIBVERS:DUMPI_MINOR_LIBVERS:DUMPI_BACKW_LIBVERS])

AC_INIT([dumpi],[DUMPI_ACVERSION_TAG],[jjwilke@sandia.gov])
AC_CONFIG_MACRO_DIR([acinclude])
AC_CONFIG_AUX_DIR(bin)
AC_CONFIG_HEADERS([dumpi/dumpiconfig-generated.h])

AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CXX
LT_INIT
AC_LANG([C])
LT_INIT

# Disable shared libraries by default.
#AC_DISABLE_SHARED
#AC_DISABLE_SHARED([])
LT_INIT

# Don't reorder options.
# AC_PRESERVE_HELP_ORDER

# Version info.
AH_TEMPLATE([DUMPI_VERSION], [Major version number])
AH_TEMPLATE([DUMPI_SUBVERSION], [Major version number])
AH_TEMPLATE([DUMPI_SUBSUBVERSION], [Major version number])
AC_DEFINE_UNQUOTED(DUMPI_VERSION, [DUMPI_VERSION_TAG])
AC_DEFINE_UNQUOTED(DUMPI_SUBVERSION, [DUMPI_SUBVERSION_TAG])
AC_DEFINE_UNQUOTED(DUMPI_SUBSUBVERSION, [DUMPI_SUBSUBVERSION_TAG])
AC_SUBST(DUMPI_LIBVERSION, [DUMPI_LIBVERSION_TAG])

CHECK_SDK()

CHECK_TIMERS()

CHECK_BG()

CHECK_CONST_QUALIFIERS()

CHECK_ENABLED_FEATURES()

CHECK_OTF2()

CHECK_CPP11()

CHECK_FORTRAN_ENABLED()

if test "$enable_libdumpi" = "yes"; then
  if test "$enable_f90" = "yes"; then
    AC_PROG_FC
    SANITY_CHECK_F90()
  fi
  if test "$enable_f77" = "yes"; then
    AC_PROG_F77
    SANITY_CHECK_F77()
  fi
fi

SANITY_CHECK_MPI()

CHECK_MPI_VERSION()

CONFIG_BUFFER_SIZES()

CHECK_PTHREADS()

CHECK_MPIIO()

CHECK_PAPI()

CHECK_INSTRUMENTATION()

# See if HOST_NAME_MAX is declared
AC_CHECK_DECL(HOST_NAME_MAX,
              [dumpi_hnm=HOST_NAME_MAX],
              [
	      dumpi_hnm=`getconf HOST_NAME_MAX`
              ],
              [#include <unistd.h>]
)
AC_DEFINE_UNQUOTED(DUMPI_HOST_NAME_MAX, $dumpi_hnm, [The maximum length of a host name string.])

# dot (from graphviz) is optional but can really help with doxygen documentation
AC_PATH_PROG([DOT], [dot])
if test -z "$DOT"; then
  HAVE_DOT=NO
else
  HAVE_DOT=YES
fi
AC_SUBST([HAVE_DOT])

AC_CONFIG_FILES([
 Makefile
 dumpi/Makefile
 dumpi/common/Makefile
 dumpi/libdumpi/Makefile
 dumpi/libundumpi/Makefile
 dumpi/libotf2dump/Makefile
 dumpi/bin/Makefile
 dumpi/test/Makefile
 tests/Makefile
 docs/doxygen.cfg
])
AC_OUTPUT

echo
echo "DUMPI Configuration Summary:"
echo "---------------------------------------------------------------"
echo "Install prefix     $prefix"
echo "C compiler         $CC"
echo "C++ compiler       $CXX"
echo "MAC SDK            $enable_sdk"
echo "CFLAGS             $CFLAGS"
echo "CXXFLAGS           $CXXFLAGS"
echo "CPPFLAGS           $CPPFLAGS"
echo "LDFLAGS            $LDFLAGS"
echo "LIBS               $LIBS"
echo "OTF2 support       $build_otf2"
echo "---------------------------------------------------------------"

