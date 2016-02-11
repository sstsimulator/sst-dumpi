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
my @op = qw(MPI_OP_NULL MPI_MAX  MPI_MIN  MPI_SUM  MPI_PROD MPI_LAND MPI_BAND
            MPI_LOR  MPI_BOR  MPI_LXOR MPI_BXOR
            MPI_MINLOC MPI_MAXLOC MPI_REPLACE);

print <<EOF;
AC_DEFUN([DUMPI_TEST_MPI_OPS],
  [
    dnl Test whether MPI operations are defined properly
    dnl The MPI implementation on Hera at LLNL fails to define some of these
    AC_LANG_PUSH(C)

EOF

for my $operation (@op) {
    print <<EOF;
    AH_TEMPLATE([DUMPI_OP_$operation],
                [$operation if $operation is defined, else MPI_OP_NULL])
    AH_TEMPLATE([DUMPI_HAVE_$operation], [defined if $operation is available])
    AC_MSG_CHECKING([whether the operation $operation is present])
    AC_LINK_IFELSE(
      AC_LANG_PROGRAM(
        [
          #include <mpi.h>
        ], [
	    int *argc;
	    char ***argv;
	    MPI_Op op = $operation;
	    MPI_Init(argc, argv);
	    MPI_Finalize();
        ]),
      [
        AC_DEFINE(DUMPI_HAVE_$operation, [1])
        AC_DEFINE(DUMPI_OP_$operation, [$operation])
        AC_MSG_RESULT([yes])
      ], [
	AC_DEFINE(DUMPI_OP_$operation, [MPI_OP_NULL])
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

