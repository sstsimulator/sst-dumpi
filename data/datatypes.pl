#!/usr/bin/perl -w

#
#  This file is part of DUMPI: 
#               The MPI profiling library from the SST suite.
#  Copyright (c) 2009 Sandia Corporation.  
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the LICENSE file in the top 
#  SST/macroscale directory.
#

use strict;
my @dt = qw(MPI_DATATYPE_NULL
            MPI_CHAR MPI_SIGNED_CHAR MPI_UNSIGNED_CHAR MPI_BYTE
            MPI_WCHAR MPI_SHORT MPI_UNSIGNED_SHORT
            MPI_INT MPI_UNSIGNED MPI_LONG MPI_UNSIGNED_LONG
            MPI_FLOAT MPI_DOUBLE MPI_LONG_DOUBLE
            MPI_LONG_LONG_INT MPI_UNSIGNED_LONG_LONG MPI_LONG_LONG
            MPI_PACKED MPI_LB MPI_UB MPI_FLOAT_INT MPI_DOUBLE_INT
            MPI_LONG_INT MPI_SHORT_INT MPI_2INT MPI_LONG_DOUBLE_INT);

print <<EOF;
AC_DEFUN([DUMPI_TEST_MPI_TYPES],
  [
    dnl Test whether MPI datatypes are defined properly
    dnl The MPI implementation on Hera at LLNL fails to define some of these
    AC_LANG_PUSH(C)

EOF

for my $datatype (@dt) {
    print <<EOF;
    AH_TEMPLATE([DUMPI_DATATYPE_$datatype],
		[$datatype if $datatype is defined, else MPI_DATATYPE_NULL])
    AH_TEMPLATE([DUMPI_HAVE_$datatype], [defined if $datatype is available])
    AC_MSG_CHECKING([whether the datatype $datatype is present])
    AC_LINK_IFELSE(
      AC_LANG_PROGRAM(
        [
          #include <mpi.h>
        ], [
	    int *argc;
	    char ***argv;
	    MPI_Datatype dt = $datatype;
	    MPI_Init(argc, argv);
	    MPI_Finalize();
        ]),
      [
        AC_DEFINE(DUMPI_HAVE_$datatype, [1])
        AC_DEFINE(DUMPI_DATATYPE_$datatype, [$datatype])
        AC_MSG_RESULT([yes])
      ], [
	AC_DEFINE(DUMPI_DATATYPE_$datatype, [MPI_DATATYPE_NULL])
        AC_MSG_RESULT([no])
      ]
    )

EOF
}

print <<EOF;
    AC_LANG_POP
  ]
)
EOF
