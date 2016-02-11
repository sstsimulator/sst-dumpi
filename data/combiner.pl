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

my @comb = qw(MPI_COMBINER_NAMED           MPI_COMBINER_DUP
              MPI_COMBINER_CONTIGUOUS      MPI_COMBINER_VECTOR
              MPI_COMBINER_HVECTOR_INTEGER MPI_COMBINER_HVECTOR
              MPI_COMBINER_INDEXED         MPI_COMBINER_HINDEXED_INTEGER
              MPI_COMBINER_HINDEXED        MPI_COMBINER_INDEXED_BLOCK
              MPI_COMBINER_STRUCT_INTEGER  MPI_COMBINER_STRUCT
              MPI_COMBINER_SUBARRAY        MPI_COMBINER_DARRAY
              MPI_COMBINER_F90_REAL        MPI_COMBINER_F90_COMPLEX
              MPI_COMBINER_F90_INTEGER     MPI_COMBINER_RESIZED);

print <<EOF;
AC_DEFUN([DUMPI_TEST_MPI_CONSTANTS],
  [
    dnl Test whether MPI constants are defined properly
    AC_LANG_PUSH(C)

EOF

for my $constant (@comb) {
    print <<EOF;
    AH_TEMPLATE([DUMPI_HAVE_$constant], [True if $constant is defined])
    AC_MSG_CHECKING([whether the integer constant $constant is defined])
    AC_LINK_IFELSE(
      AC_LANG_PROGRAM(
        [
          #include <mpi.h>
        ], [
	    int *argc;
	    char ***argv;
	    int value = $constant;
	    MPI_Init(argc, argv);
	    MPI_Finalize();
        ]),
      [
        AC_DEFINE(DUMPI_HAVE_$constant, [1])
        AC_MSG_RESULT([yes])
      ], [
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

