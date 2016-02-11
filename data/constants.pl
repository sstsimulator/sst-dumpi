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

my @int_reqd = qw(MPI_SUCCESS MPI_ANY_TAG MPI_ANY_SOURCE MPI_ROOT);

my @int_opt = qw(
               MPI_COMBINER_NAMED           MPI_COMBINER_DUP
               MPI_COMBINER_CONTIGUOUS      MPI_COMBINER_VECTOR
               MPI_COMBINER_HVECTOR_INTEGER MPI_COMBINER_HVECTOR
               MPI_COMBINER_INDEXED         MPI_COMBINER_HINDEXED_INTEGER
               MPI_COMBINER_HINDEXED        MPI_COMBINER_INDEXED_BLOCK
               MPI_COMBINER_STRUCT_INTEGER  MPI_COMBINER_STRUCT
               MPI_COMBINER_SUBARRAY        MPI_COMBINER_DARRAY
               MPI_COMBINER_F90_REAL        MPI_COMBINER_F90_COMPLEX
               MPI_COMBINER_F90_INTEGER     MPI_COMBINER_RESIZED

               MPI_IDENT                    MPI_CONGRUENT
               MPI_SIMILAR                  MPI_UNEQUAL

               MPI_DISTRIBUTE_BLOCK         MPI_DISTRIBUTE_CYCLIC
               MPI_DISTRIBUTE_NONE          MPI_DISTRIBUTE_DFLT_DARG

               MPI_MODE_RDONLY              MPI_MODE_RDWR
               MPI_MODE_WRONLY              MPI_MODE_CREATE
               MPI_MODE_EXCL                MPI_MODE_DELETE_ON_CLOSE
               MPI_MODE_UNIQUE_OPEN         MPI_MODE_SEQUENTIAL
               MPI_MODE_APPEND

               MPI_SEEK_SET                 MPI_SEEK_CUR
               MPI_SEEK_END

               MPI_MODE_NOCHECK             MPI_MODE_NOSTORE
               MPI_MODE_NOPUT               MPI_MODE_NOPRECEDE
               MPI_MODE_NOSUCCEED

               MPI_LOCK_SHARED              MPI_LOCK_EXCLUSIVE

               MPI_ORDER_C                  MPI_ORDER_FORTRAN

               MPI_THREAD_SINGLE            MPI_THREAD_FUNNELED
               MPI_THREAD_SERIALIZED        MPI_THREAD_MULTIPLE

               MPI_GRAPH                    MPI_CART
               MPI_UNDEFINED

               MPI_TYPECLASS_REAL           MPI_TYPECLASS_INTEGER
               MPI_TYPECLASS_COMPLEX

               MPI_TAG_UB                   MPI_HOST
               MPI_IO                       MPI_WTIME_IS_GLOBAL
               MPI_UNIVERSE_SIZE            MPI_LASTUSEDCODE
               MPI_APPNUM                   MPI_WIN_BASE
               MPI_WIN_SIZE                 MPI_WIN_DISP_UNIT
);

my @misc_reqd = ({"type"=>"MPI_Comm",
		  "val"=>["MPI_COMM_NULL", "MPI_COMM_WORLD", "MPI_COMM_SELF"]},
		 {"type"=>"MPI_Group",
		  "val"=>["MPI_GROUP_NULL", "MPI_GROUP_EMPTY"]},
		 {"type"=>"MPI_Errhandler", "val"=>["MPI_ERRHANDLER_NULL"]},
		 {"type"=>"MPI_Request",    "val"=>["MPI_REQUEST_NULL"]},
		 
    );

my @misc_opt = ({"type"=>"MPI_File", "val"=>["MPI_FILE_NULL"]},
		{"type"=>"MPI_Info", "val"=>["MPI_INFO_NULL"]},
		{"type"=>"MPIO_Request", "val"=>["MPIO_REQUEST_NULL"]},
		{"type"=>"MPI_Win",      "val"=>["MPI_WIN_NULL"]}
    );

print <<EOF;
AC_DEFUN([DUMPI_TEST_MPI_CONSTANTS],
  [
    dnl Test whether MPI constants are defined properly
    AC_LANG_PUSH(C)

EOF

for my $constant (@int_reqd) {
  create_test("int", $constant, "AC_MSG_ERROR");
}
for my $constant (@int_opt) {
  create_test("int", $constant, "AC_MSG_RESULT"); 
}
for my $pair (@misc_reqd) {
  for my $constant (@{$pair->{"val"}}) {
    create_test($pair->{"type"}, $constant, "AC_MSG_ERROR");
  }
}
for my $pair (@misc_opt) {
  for my $constant (@{$pair->{"val"}}) {
    create_test($pair->{"type"}, $constant, "AC_MSG_RESULT");
  }
}

print <<EOF;
    AC_LANG_POP
  ]
)
EOF

sub create_test {
    die("create_test takes three arguments\n") unless @_ == 3;
    my $type = shift;
    my $constant = shift;
    my $failure = shift;
    print <<EOF;
    AH_TEMPLATE([DUMPI_HAVE_$constant], [True if $constant is defined])
    AC_MSG_CHECKING([whether the constant $constant is defined])
    AC_LINK_IFELSE(
      AC_LANG_PROGRAM(
        [
          #include <mpi.h>
        ], [
	    int *argc;
	    char ***argv;
	    $type value = $constant;
	    MPI_Init(argc, argv);
	    MPI_Finalize();
        ]),
      [
        AC_DEFINE(DUMPI_HAVE_$constant, [1])
        AC_MSG_RESULT([yes])
      ], [
        $failure([no])
      ]
    )

EOF
}
