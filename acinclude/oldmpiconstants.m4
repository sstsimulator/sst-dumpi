AC_DEFUN([OLD_DUMPI_TEST_MPI_CONSTANTS],
  [
    dnl Test whether MPI constants are defined properly
    AC_LANG_PUSH(C)

    AH_TEMPLATE([DUMPI_HAVE_MPI_SUCCESS], [True if MPI_SUCCESS is defined])
    AC_CACHE_CHECK([whether the constant MPI_SUCCESS is defined],
      [dumpi_cv_have_mpi_success],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_SUCCESS;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_success="yes"
        ], [
          dumpi_cv_have_mpi_success="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_success" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SUCCESS, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_ANY_TAG], [True if MPI_ANY_TAG is defined])
    AC_CACHE_CHECK([whether the constant MPI_ANY_TAG is defined],
      [dumpi_cv_have_mpi_any_tag],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_ANY_TAG;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_any_tag="yes"
        ], [
          dumpi_cv_have_mpi_any_tag="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_any_tag" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_ANY_TAG, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_ANY_SOURCE], [True if MPI_ANY_SOURCE is defined])
    AC_CACHE_CHECK([whether the constant MPI_ANY_SOURCE is defined],
      [dumpi_cv_have_mpi_any_source],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_ANY_SOURCE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_any_source="yes"
        ], [
          dumpi_cv_have_mpi_any_source="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_any_source" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_ANY_SOURCE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_ROOT], [True if MPI_ROOT is defined])
    AC_CACHE_CHECK([whether the constant MPI_ROOT is defined],
      [dumpi_cv_have_mpi_root],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_ROOT;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_root="yes"
        ], [
          dumpi_cv_have_mpi_root="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_root" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_ROOT, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_NAMED], [True if MPI_COMBINER_NAMED is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_NAMED is defined],
      [dumpi_cv_have_mpi_combiner_named],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_NAMED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_named="yes"
        ], [
          dumpi_cv_have_mpi_combiner_named="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_named" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_NAMED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_DUP], [True if MPI_COMBINER_DUP is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_DUP is defined],
      [dumpi_cv_have_mpi_combiner_dup],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_DUP;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_dup="yes"
        ], [
          dumpi_cv_have_mpi_combiner_dup="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_dup" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_DUP, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_CONTIGUOUS], [True if MPI_COMBINER_CONTIGUOUS is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_CONTIGUOUS is defined],
      [dumpi_cv_have_mpi_combiner_contiguous],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_CONTIGUOUS;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_contiguous="yes"
        ], [
          dumpi_cv_have_mpi_combiner_contiguous="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_contiguous" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_CONTIGUOUS, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_VECTOR], [True if MPI_COMBINER_VECTOR is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_VECTOR is defined],
      [dumpi_cv_have_mpi_combiner_vector],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_VECTOR;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_vector="yes"
        ], [
          dumpi_cv_have_mpi_combiner_vector="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_vector" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_VECTOR, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_HVECTOR_INTEGER], [True if MPI_COMBINER_HVECTOR_INTEGER is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_HVECTOR_INTEGER is defined],
      [dumpi_cv_have_mpi_combiner_hvector_integer],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_HVECTOR_INTEGER;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_hvector_integer="yes"
        ], [
          dumpi_cv_have_mpi_combiner_hvector_integer="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_hvector_integer" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_HVECTOR_INTEGER, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_HVECTOR], [True if MPI_COMBINER_HVECTOR is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_HVECTOR is defined],
      [dumpi_cv_have_mpi_combiner_hvector],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_HVECTOR;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_hvector="yes"
        ], [
          dumpi_cv_have_mpi_combiner_hvector="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_hvector" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_HVECTOR, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_INDEXED], [True if MPI_COMBINER_INDEXED is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_INDEXED is defined],
      [dumpi_cv_have_mpi_combiner_indexed],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_INDEXED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_indexed="yes"
        ], [
          dumpi_cv_have_mpi_combiner_indexed="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_indexed" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_INDEXED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_HINDEXED_INTEGER], [True if MPI_COMBINER_HINDEXED_INTEGER is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_HINDEXED_INTEGER is defined],
      [dumpi_cv_have_mpi_combiner_hindexed_integer],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_HINDEXED_INTEGER;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_hindexed_integer="yes"
        ], [
          dumpi_cv_have_mpi_combiner_hindexed_integer="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_hindexed_integer" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_HINDEXED_INTEGER, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_HINDEXED], [True if MPI_COMBINER_HINDEXED is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_HINDEXED is defined],
      [dumpi_cv_have_mpi_combiner_hindexed],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_HINDEXED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_hindexed="yes"
        ], [
          dumpi_cv_have_mpi_combiner_hindexed="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_hindexed" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_HINDEXED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_INDEXED_BLOCK], [True if MPI_COMBINER_INDEXED_BLOCK is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_INDEXED_BLOCK is defined],
      [dumpi_cv_have_mpi_combiner_indexed_block],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_INDEXED_BLOCK;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_indexed_block="yes"
        ], [
          dumpi_cv_have_mpi_combiner_indexed_block="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_indexed_block" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_INDEXED_BLOCK, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_STRUCT_INTEGER], [True if MPI_COMBINER_STRUCT_INTEGER is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_STRUCT_INTEGER is defined],
      [dumpi_cv_have_mpi_combiner_struct_integer],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_STRUCT_INTEGER;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_struct_integer="yes"
        ], [
          dumpi_cv_have_mpi_combiner_struct_integer="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_struct_integer" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_STRUCT_INTEGER, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_STRUCT], [True if MPI_COMBINER_STRUCT is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_STRUCT is defined],
      [dumpi_cv_have_mpi_combiner_struct],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_STRUCT;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_struct="yes"
        ], [
          dumpi_cv_have_mpi_combiner_struct="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_struct" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_STRUCT, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_SUBARRAY], [True if MPI_COMBINER_SUBARRAY is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_SUBARRAY is defined],
      [dumpi_cv_have_mpi_combiner_subarray],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_SUBARRAY;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_subarray="yes"
        ], [
          dumpi_cv_have_mpi_combiner_subarray="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_subarray" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_SUBARRAY, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_DARRAY], [True if MPI_COMBINER_DARRAY is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_DARRAY is defined],
      [dumpi_cv_have_mpi_combiner_darray],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_DARRAY;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_darray="yes"
        ], [
          dumpi_cv_have_mpi_combiner_darray="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_darray" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_DARRAY, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_F90_REAL], [True if MPI_COMBINER_F90_REAL is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_F90_REAL is defined],
      [dumpi_cv_have_mpi_combiner_f90_real],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_F90_REAL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_f90_real="yes"
        ], [
          dumpi_cv_have_mpi_combiner_f90_real="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_f90_real" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_F90_REAL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_F90_COMPLEX], [True if MPI_COMBINER_F90_COMPLEX is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_F90_COMPLEX is defined],
      [dumpi_cv_have_mpi_combiner_f90_complex],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_F90_COMPLEX;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_f90_complex="yes"
        ], [
          dumpi_cv_have_mpi_combiner_f90_complex="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_f90_complex" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_F90_COMPLEX, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_F90_INTEGER], [True if MPI_COMBINER_F90_INTEGER is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_F90_INTEGER is defined],
      [dumpi_cv_have_mpi_combiner_f90_integer],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_F90_INTEGER;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_f90_integer="yes"
        ], [
          dumpi_cv_have_mpi_combiner_f90_integer="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_f90_integer" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_F90_INTEGER, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMBINER_RESIZED], [True if MPI_COMBINER_RESIZED is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMBINER_RESIZED is defined],
      [dumpi_cv_have_mpi_combiner_resized],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_COMBINER_RESIZED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_combiner_resized="yes"
        ], [
          dumpi_cv_have_mpi_combiner_resized="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_combiner_resized" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMBINER_RESIZED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_IDENT], [True if MPI_IDENT is defined])
    AC_CACHE_CHECK([whether the constant MPI_IDENT is defined],
      [dumpi_cv_have_mpi_ident],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_IDENT;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_ident="yes"
        ], [
          dumpi_cv_have_mpi_ident="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_ident" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_IDENT, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_CONGRUENT], [True if MPI_CONGRUENT is defined])
    AC_CACHE_CHECK([whether the constant MPI_CONGRUENT is defined],
      [dumpi_cv_have_mpi_congruent],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_CONGRUENT;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_congruent="yes"
        ], [
          dumpi_cv_have_mpi_congruent="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_congruent" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_CONGRUENT, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_SIMILAR], [True if MPI_SIMILAR is defined])
    AC_CACHE_CHECK([whether the constant MPI_SIMILAR is defined],
      [dumpi_cv_have_mpi_similar],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_SIMILAR;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_similar="yes"
        ], [
          dumpi_cv_have_mpi_similar="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_similar" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SIMILAR, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_UNEQUAL], [True if MPI_UNEQUAL is defined])
    AC_CACHE_CHECK([whether the constant MPI_UNEQUAL is defined],
      [dumpi_cv_have_mpi_unequal],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_UNEQUAL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_unequal="yes"
        ], [
          dumpi_cv_have_mpi_unequal="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unequal" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UNEQUAL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_DISTRIBUTE_BLOCK], [True if MPI_DISTRIBUTE_BLOCK is defined])
    AC_CACHE_CHECK([whether the constant MPI_DISTRIBUTE_BLOCK is defined],
      [dumpi_cv_have_mpi_distribute_block],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_DISTRIBUTE_BLOCK;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_distribute_block="yes"
        ], [
          dumpi_cv_have_mpi_distribute_block="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_distribute_block" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_DISTRIBUTE_BLOCK, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_DISTRIBUTE_CYCLIC], [True if MPI_DISTRIBUTE_CYCLIC is defined])
    AC_CACHE_CHECK([whether the constant MPI_DISTRIBUTE_CYCLIC is defined],
      [dumpi_cv_have_mpi_distribute_cyclic],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_DISTRIBUTE_CYCLIC;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_distribute_cyclic="yes"
        ], [
          dumpi_cv_have_mpi_distribute_cyclic="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_distribute_cyclic" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_DISTRIBUTE_CYCLIC, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_DISTRIBUTE_NONE], [True if MPI_DISTRIBUTE_NONE is defined])
    AC_CACHE_CHECK([whether the constant MPI_DISTRIBUTE_NONE is defined],
      [dumpi_cv_have_mpi_distribute_none],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_DISTRIBUTE_NONE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_distribute_none="yes"
        ], [
          dumpi_cv_have_mpi_distribute_none="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_distribute_none" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_DISTRIBUTE_NONE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_DISTRIBUTE_DFLT_DARG], [True if MPI_DISTRIBUTE_DFLT_DARG is defined])
    AC_CACHE_CHECK([whether the constant MPI_DISTRIBUTE_DFLT_DARG is defined],
      [dumpi_cv_have_mpi_distribute_dflt_darg],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_DISTRIBUTE_DFLT_DARG;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_distribute_dflt_darg="yes"
        ], [
          dumpi_cv_have_mpi_distribute_dflt_darg="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_distribute_dflt_darg" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_DISTRIBUTE_DFLT_DARG, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_RDONLY], [True if MPI_MODE_RDONLY is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_RDONLY is defined],
      [dumpi_cv_have_mpi_mode_rdonly],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_RDONLY;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_rdonly="yes"
        ], [
          dumpi_cv_have_mpi_mode_rdonly="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_rdonly" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_RDONLY, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_RDWR], [True if MPI_MODE_RDWR is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_RDWR is defined],
      [dumpi_cv_have_mpi_mode_rdwr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_RDWR;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_rdwr="yes"
        ], [
          dumpi_cv_have_mpi_mode_rdwr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_rdwr" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_RDWR, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_WRONLY], [True if MPI_MODE_WRONLY is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_WRONLY is defined],
      [dumpi_cv_have_mpi_mode_wronly],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_WRONLY;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_wronly="yes"
        ], [
          dumpi_cv_have_mpi_mode_wronly="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_wronly" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_WRONLY, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_CREATE], [True if MPI_MODE_CREATE is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_CREATE is defined],
      [dumpi_cv_have_mpi_mode_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_CREATE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_create="yes"
        ], [
          dumpi_cv_have_mpi_mode_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_create" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_EXCL], [True if MPI_MODE_EXCL is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_EXCL is defined],
      [dumpi_cv_have_mpi_mode_excl],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_EXCL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_excl="yes"
        ], [
          dumpi_cv_have_mpi_mode_excl="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_excl" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_EXCL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_DELETE_ON_CLOSE], [True if MPI_MODE_DELETE_ON_CLOSE is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_DELETE_ON_CLOSE is defined],
      [dumpi_cv_have_mpi_mode_delete_on_close],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_DELETE_ON_CLOSE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_delete_on_close="yes"
        ], [
          dumpi_cv_have_mpi_mode_delete_on_close="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_delete_on_close" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_DELETE_ON_CLOSE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_UNIQUE_OPEN], [True if MPI_MODE_UNIQUE_OPEN is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_UNIQUE_OPEN is defined],
      [dumpi_cv_have_mpi_mode_unique_open],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_UNIQUE_OPEN;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_unique_open="yes"
        ], [
          dumpi_cv_have_mpi_mode_unique_open="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_unique_open" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_UNIQUE_OPEN, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_SEQUENTIAL], [True if MPI_MODE_SEQUENTIAL is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_SEQUENTIAL is defined],
      [dumpi_cv_have_mpi_mode_sequential],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_SEQUENTIAL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_sequential="yes"
        ], [
          dumpi_cv_have_mpi_mode_sequential="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_sequential" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_SEQUENTIAL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_APPEND], [True if MPI_MODE_APPEND is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_APPEND is defined],
      [dumpi_cv_have_mpi_mode_append],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_APPEND;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_append="yes"
        ], [
          dumpi_cv_have_mpi_mode_append="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_append" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_APPEND, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_SEEK_SET], [True if MPI_SEEK_SET is defined])
    AC_CACHE_CHECK([whether the constant MPI_SEEK_SET is defined],
      [dumpi_cv_have_mpi_seek_set],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_SEEK_SET;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_seek_set="yes"
        ], [
          dumpi_cv_have_mpi_seek_set="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_seek_set" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SEEK_SET, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_SEEK_CUR], [True if MPI_SEEK_CUR is defined])
    AC_CACHE_CHECK([whether the constant MPI_SEEK_CUR is defined],
      [dumpi_cv_have_mpi_seek_cur],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_SEEK_CUR;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_seek_cur="yes"
        ], [
          dumpi_cv_have_mpi_seek_cur="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_seek_cur" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SEEK_CUR, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_SEEK_END], [True if MPI_SEEK_END is defined])
    AC_CACHE_CHECK([whether the constant MPI_SEEK_END is defined],
      [dumpi_cv_have_mpi_seek_end],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_SEEK_END;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_seek_end="yes"
        ], [
          dumpi_cv_have_mpi_seek_end="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_seek_end" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SEEK_END, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_NOCHECK], [True if MPI_MODE_NOCHECK is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_NOCHECK is defined],
      [dumpi_cv_have_mpi_mode_nocheck],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_NOCHECK;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_nocheck="yes"
        ], [
          dumpi_cv_have_mpi_mode_nocheck="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_nocheck" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_NOCHECK, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_NOSTORE], [True if MPI_MODE_NOSTORE is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_NOSTORE is defined],
      [dumpi_cv_have_mpi_mode_nostore],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_NOSTORE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_nostore="yes"
        ], [
          dumpi_cv_have_mpi_mode_nostore="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_nostore" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_NOSTORE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_NOPUT], [True if MPI_MODE_NOPUT is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_NOPUT is defined],
      [dumpi_cv_have_mpi_mode_noput],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_NOPUT;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_noput="yes"
        ], [
          dumpi_cv_have_mpi_mode_noput="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_noput" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_NOPUT, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_NOPRECEDE], [True if MPI_MODE_NOPRECEDE is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_NOPRECEDE is defined],
      [dumpi_cv_have_mpi_mode_noprecede],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_NOPRECEDE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_noprecede="yes"
        ], [
          dumpi_cv_have_mpi_mode_noprecede="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_noprecede" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_NOPRECEDE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_MODE_NOSUCCEED], [True if MPI_MODE_NOSUCCEED is defined])
    AC_CACHE_CHECK([whether the constant MPI_MODE_NOSUCCEED is defined],
      [dumpi_cv_have_mpi_mode_nosucceed],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_MODE_NOSUCCEED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_mode_nosucceed="yes"
        ], [
          dumpi_cv_have_mpi_mode_nosucceed="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_mode_nosucceed" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MODE_NOSUCCEED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_LOCK_SHARED], [True if MPI_LOCK_SHARED is defined])
    AC_CACHE_CHECK([whether the constant MPI_LOCK_SHARED is defined],
      [dumpi_cv_have_mpi_lock_shared],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_LOCK_SHARED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_lock_shared="yes"
        ], [
          dumpi_cv_have_mpi_lock_shared="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_lock_shared" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LOCK_SHARED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_LOCK_EXCLUSIVE], [True if MPI_LOCK_EXCLUSIVE is defined])
    AC_CACHE_CHECK([whether the constant MPI_LOCK_EXCLUSIVE is defined],
      [dumpi_cv_have_mpi_lock_exclusive],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_LOCK_EXCLUSIVE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_lock_exclusive="yes"
        ], [
          dumpi_cv_have_mpi_lock_exclusive="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_lock_exclusive" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LOCK_EXCLUSIVE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_ORDER_C], [True if MPI_ORDER_C is defined])
    AC_CACHE_CHECK([whether the constant MPI_ORDER_C is defined],
      [dumpi_cv_have_mpi_order_c],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_ORDER_C;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_order_c="yes"
        ], [
          dumpi_cv_have_mpi_order_c="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_order_c" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_ORDER_C, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_ORDER_FORTRAN], [True if MPI_ORDER_FORTRAN is defined])
    AC_CACHE_CHECK([whether the constant MPI_ORDER_FORTRAN is defined],
      [dumpi_cv_have_mpi_order_fortran],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_ORDER_FORTRAN;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_order_fortran="yes"
        ], [
          dumpi_cv_have_mpi_order_fortran="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_order_fortran" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_ORDER_FORTRAN, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_THREAD_SINGLE], [True if MPI_THREAD_SINGLE is defined])
    AC_CACHE_CHECK([whether the constant MPI_THREAD_SINGLE is defined],
      [dumpi_cv_have_mpi_thread_single],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_THREAD_SINGLE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_thread_single="yes"
        ], [
          dumpi_cv_have_mpi_thread_single="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_thread_single" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_THREAD_SINGLE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_THREAD_FUNNELED], [True if MPI_THREAD_FUNNELED is defined])
    AC_CACHE_CHECK([whether the constant MPI_THREAD_FUNNELED is defined],
      [dumpi_cv_have_mpi_thread_funneled],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_THREAD_FUNNELED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_thread_funneled="yes"
        ], [
          dumpi_cv_have_mpi_thread_funneled="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_thread_funneled" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_THREAD_FUNNELED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_THREAD_SERIALIZED], [True if MPI_THREAD_SERIALIZED is defined])
    AC_CACHE_CHECK([whether the constant MPI_THREAD_SERIALIZED is defined],
      [dumpi_cv_have_mpi_thread_serialized],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_THREAD_SERIALIZED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_thread_serialized="yes"
        ], [
          dumpi_cv_have_mpi_thread_serialized="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_thread_serialized" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_THREAD_SERIALIZED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_THREAD_MULTIPLE], [True if MPI_THREAD_MULTIPLE is defined])
    AC_CACHE_CHECK([whether the constant MPI_THREAD_MULTIPLE is defined],
      [dumpi_cv_have_mpi_thread_multiple],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_THREAD_MULTIPLE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_thread_multiple="yes"
        ], [
          dumpi_cv_have_mpi_thread_multiple="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_thread_multiple" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_THREAD_MULTIPLE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_GRAPH], [True if MPI_GRAPH is defined])
    AC_CACHE_CHECK([whether the constant MPI_GRAPH is defined],
      [dumpi_cv_have_mpi_graph],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_GRAPH;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_graph="yes"
        ], [
          dumpi_cv_have_mpi_graph="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_graph" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_GRAPH, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_CART], [True if MPI_CART is defined])
    AC_CACHE_CHECK([whether the constant MPI_CART is defined],
      [dumpi_cv_have_mpi_cart],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_CART;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_cart="yes"
        ], [
          dumpi_cv_have_mpi_cart="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cart" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_CART, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_UNDEFINED], [True if MPI_UNDEFINED is defined])
    AC_CACHE_CHECK([whether the constant MPI_UNDEFINED is defined],
      [dumpi_cv_have_mpi_undefined],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_UNDEFINED;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_undefined="yes"
        ], [
          dumpi_cv_have_mpi_undefined="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_undefined" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UNDEFINED, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_TYPECLASS_REAL], [True if MPI_TYPECLASS_REAL is defined])
    AC_CACHE_CHECK([whether the constant MPI_TYPECLASS_REAL is defined],
      [dumpi_cv_have_mpi_typeclass_real],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_TYPECLASS_REAL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_typeclass_real="yes"
        ], [
          dumpi_cv_have_mpi_typeclass_real="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_typeclass_real" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_TYPECLASS_REAL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_TYPECLASS_INTEGER], [True if MPI_TYPECLASS_INTEGER is defined])
    AC_CACHE_CHECK([whether the constant MPI_TYPECLASS_INTEGER is defined],
      [dumpi_cv_have_mpi_typeclass_integer],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_TYPECLASS_INTEGER;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_typeclass_integer="yes"
        ], [
          dumpi_cv_have_mpi_typeclass_integer="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_typeclass_integer" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_TYPECLASS_INTEGER, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_TYPECLASS_COMPLEX], [True if MPI_TYPECLASS_COMPLEX is defined])
    AC_CACHE_CHECK([whether the constant MPI_TYPECLASS_COMPLEX is defined],
      [dumpi_cv_have_mpi_typeclass_complex],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_TYPECLASS_COMPLEX;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_typeclass_complex="yes"
        ], [
          dumpi_cv_have_mpi_typeclass_complex="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_typeclass_complex" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_TYPECLASS_COMPLEX, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_TAG_UB], [True if MPI_TAG_UB is defined])
    AC_CACHE_CHECK([whether the constant MPI_TAG_UB is defined],
      [dumpi_cv_have_mpi_tag_ub],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_TAG_UB;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_tag_ub="yes"
        ], [
          dumpi_cv_have_mpi_tag_ub="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_tag_ub" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_TAG_UB, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_HOST], [True if MPI_HOST is defined])
    AC_CACHE_CHECK([whether the constant MPI_HOST is defined],
      [dumpi_cv_have_mpi_host],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_HOST;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_host="yes"
        ], [
          dumpi_cv_have_mpi_host="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_host" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_HOST, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_IO], [True if MPI_IO is defined])
    AC_CACHE_CHECK([whether the constant MPI_IO is defined],
      [dumpi_cv_have_mpi_io],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_IO;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_io="yes"
        ], [
          dumpi_cv_have_mpi_io="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_io" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_IO, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_WTIME_IS_GLOBAL], [True if MPI_WTIME_IS_GLOBAL is defined])
    AC_CACHE_CHECK([whether the constant MPI_WTIME_IS_GLOBAL is defined],
      [dumpi_cv_have_mpi_wtime_is_global],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_WTIME_IS_GLOBAL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_wtime_is_global="yes"
        ], [
          dumpi_cv_have_mpi_wtime_is_global="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_wtime_is_global" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_WTIME_IS_GLOBAL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_UNIVERSE_SIZE], [True if MPI_UNIVERSE_SIZE is defined])
    AC_CACHE_CHECK([whether the constant MPI_UNIVERSE_SIZE is defined],
      [dumpi_cv_have_mpi_universe_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_UNIVERSE_SIZE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_universe_size="yes"
        ], [
          dumpi_cv_have_mpi_universe_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_universe_size" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UNIVERSE_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_LASTUSEDCODE], [True if MPI_LASTUSEDCODE is defined])
    AC_CACHE_CHECK([whether the constant MPI_LASTUSEDCODE is defined],
      [dumpi_cv_have_mpi_lastusedcode],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_LASTUSEDCODE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_lastusedcode="yes"
        ], [
          dumpi_cv_have_mpi_lastusedcode="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_lastusedcode" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LASTUSEDCODE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_APPNUM], [True if MPI_APPNUM is defined])
    AC_CACHE_CHECK([whether the constant MPI_APPNUM is defined],
      [dumpi_cv_have_mpi_appnum],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_APPNUM;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_appnum="yes"
        ], [
          dumpi_cv_have_mpi_appnum="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_appnum" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_APPNUM, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_WIN_BASE], [True if MPI_WIN_BASE is defined])
    AC_CACHE_CHECK([whether the constant MPI_WIN_BASE is defined],
      [dumpi_cv_have_mpi_win_base],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_WIN_BASE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_win_base="yes"
        ], [
          dumpi_cv_have_mpi_win_base="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_base" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_WIN_BASE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_WIN_SIZE], [True if MPI_WIN_SIZE is defined])
    AC_CACHE_CHECK([whether the constant MPI_WIN_SIZE is defined],
      [dumpi_cv_have_mpi_win_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_WIN_SIZE;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_win_size="yes"
        ], [
          dumpi_cv_have_mpi_win_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_size" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_WIN_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_WIN_DISP_UNIT], [True if MPI_WIN_DISP_UNIT is defined])
    AC_CACHE_CHECK([whether the constant MPI_WIN_DISP_UNIT is defined],
      [dumpi_cv_have_mpi_win_disp_unit],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      int value = MPI_WIN_DISP_UNIT;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_win_disp_unit="yes"
        ], [
          dumpi_cv_have_mpi_win_disp_unit="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_disp_unit" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_WIN_DISP_UNIT, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMM_NULL], [True if MPI_COMM_NULL is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMM_NULL is defined],
      [dumpi_cv_have_mpi_comm_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Comm value = MPI_COMM_NULL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_comm_null="yes"
        ], [
          dumpi_cv_have_mpi_comm_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMM_NULL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMM_WORLD], [True if MPI_COMM_WORLD is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMM_WORLD is defined],
      [dumpi_cv_have_mpi_comm_world],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Comm value = MPI_COMM_WORLD;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_comm_world="yes"
        ], [
          dumpi_cv_have_mpi_comm_world="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_world" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMM_WORLD, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_COMM_SELF], [True if MPI_COMM_SELF is defined])
    AC_CACHE_CHECK([whether the constant MPI_COMM_SELF is defined],
      [dumpi_cv_have_mpi_comm_self],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Comm value = MPI_COMM_SELF;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_comm_self="yes"
        ], [
          dumpi_cv_have_mpi_comm_self="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_self" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_COMM_SELF, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_GROUP_NULL], [True if MPI_GROUP_NULL is defined])
    AC_CACHE_CHECK([whether the constant MPI_GROUP_NULL is defined],
      [dumpi_cv_have_mpi_group_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Group value = MPI_GROUP_NULL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_group_null="yes"
        ], [
          dumpi_cv_have_mpi_group_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_GROUP_NULL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_GROUP_EMPTY], [True if MPI_GROUP_EMPTY is defined])
    AC_CACHE_CHECK([whether the constant MPI_GROUP_EMPTY is defined],
      [dumpi_cv_have_mpi_group_empty],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Group value = MPI_GROUP_EMPTY;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_group_empty="yes"
        ], [
          dumpi_cv_have_mpi_group_empty="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_empty" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_GROUP_EMPTY, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_ERRHANDLER_NULL], [True if MPI_ERRHANDLER_NULL is defined])
    AC_CACHE_CHECK([whether the constant MPI_ERRHANDLER_NULL is defined],
      [dumpi_cv_have_mpi_errhandler_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Errhandler value = MPI_ERRHANDLER_NULL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_errhandler_null="yes"
        ], [
          dumpi_cv_have_mpi_errhandler_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_errhandler_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_ERRHANDLER_NULL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_REQUEST_NULL], [True if MPI_REQUEST_NULL is defined])
    AC_CACHE_CHECK([whether the constant MPI_REQUEST_NULL is defined],
      [dumpi_cv_have_mpi_request_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Request value = MPI_REQUEST_NULL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_request_null="yes"
        ], [
          dumpi_cv_have_mpi_request_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_request_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_REQUEST_NULL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_FILE_NULL], [True if MPI_FILE_NULL is defined])
    AC_CACHE_CHECK([whether the constant MPI_FILE_NULL is defined],
      [dumpi_cv_have_mpi_file_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_File value = MPI_FILE_NULL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_file_null="yes"
        ], [
          dumpi_cv_have_mpi_file_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_FILE_NULL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_INFO_NULL], [True if MPI_INFO_NULL is defined])
    AC_CACHE_CHECK([whether the constant MPI_INFO_NULL is defined],
      [dumpi_cv_have_mpi_info_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Info value = MPI_INFO_NULL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_info_null="yes"
        ], [
          dumpi_cv_have_mpi_info_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_INFO_NULL, [1])
    fi

    AH_TEMPLATE([DUMPI_HAVE_MPI_WIN_NULL], [True if MPI_WIN_NULL is defined])
    AC_CACHE_CHECK([whether the constant MPI_WIN_NULL is defined],
      [dumpi_cv_have_mpi_win_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
	      int *argc;
	      char ***argv;
	      MPI_Win value = MPI_WIN_NULL;
	      MPI_Init(argc, argv);
	      MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_win_null="yes"
        ], [
          dumpi_cv_have_mpi_win_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_WIN_NULL, [1])
    fi

    AC_LANG_POP
  ]
)
