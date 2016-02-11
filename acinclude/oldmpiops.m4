AC_DEFUN([OLD_DUMPI_TEST_MPI_OPS],
  [
    dnl Test whether MPI operations are defined properly
    dnl The MPI implementation on Hera at LLNL fails to define some of these
    AC_LANG_PUSH(C)

    AH_TEMPLATE([DUMPI_HAVE_MPI_OP_NULL], [defined if MPI_OP_NULL is available])
    AC_CACHE_CHECK([whether the operation MPI_OP_NULL is present], [dumpi_cv_have_mpi_op_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_OP_NULL;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_op_null="yes"
        ], [
          dumpi_cv_have_mpi_op_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_op_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_OP_NULL, [1])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_MAX], [defined if MPI_MAX is available])
    AH_TEMPLATE([DUMPI_OP_MPI_MAX], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_MAX is present], [dumpi_cv_have_mpi_max],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_MAX;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_max="yes"
        ], [
          dumpi_cv_have_mpi_max="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_max" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MAX, [1])
      AC_DEFINE(DUMPI_OP_MPI_MAX, [MPI_MAX])
    else
      AC_DEFINE(DUMPI_OP_MPI_MAX, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_MIN], [defined if MPI_MIN is available])
    AH_TEMPLATE([DUMPI_OP_MPI_MIN], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_MIN is present], [dumpi_cv_have_mpi_min],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_MIN;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_min="yes"
        ], [
          dumpi_cv_have_mpi_min="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_min" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MIN, [1])
      AC_DEFINE(DUMPI_OP_MPI_MIN, [MPI_MIN])
    else
      AC_DEFINE(DUMPI_OP_MPI_MIN, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_SUM], [defined if MPI_SUM is available])
    AH_TEMPLATE([DUMPI_OP_MPI_SUM], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_SUM is present], [dumpi_cv_have_mpi_sum],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_SUM;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_sum="yes"
        ], [
          dumpi_cv_have_mpi_sum="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_sum" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SUM, [1])
      AC_DEFINE(DUMPI_OP_MPI_SUM, [MPI_SUM])
    else
      AC_DEFINE(DUMPI_OP_MPI_SUM, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_PROD], [defined if MPI_PROD is available])
    AH_TEMPLATE([DUMPI_OP_MPI_PROD], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_PROD is present], [dumpi_cv_have_mpi_prod],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_PROD;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_prod="yes"
        ], [
          dumpi_cv_have_mpi_prod="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_prod" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_PROD, [1])
      AC_DEFINE(DUMPI_OP_MPI_PROD, [MPI_PROD])
    else
      AC_DEFINE(DUMPI_OP_MPI_PROD, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LAND], [defined if MPI_LAND is available])
    AH_TEMPLATE([DUMPI_OP_MPI_LAND], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_LAND is present], [dumpi_cv_have_mpi_land],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_LAND;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_land="yes"
        ], [
          dumpi_cv_have_mpi_land="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_land" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LAND, [1])
      AC_DEFINE(DUMPI_OP_MPI_LAND, [MPI_LAND])
    else
      AC_DEFINE(DUMPI_OP_MPI_LAND, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_BAND], [defined if MPI_BAND is available])
    AH_TEMPLATE([DUMPI_OP_MPI_BAND], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_BAND is present], [dumpi_cv_have_mpi_band],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_BAND;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_band="yes"
        ], [
          dumpi_cv_have_mpi_band="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_band" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_BAND, [1])
      AC_DEFINE(DUMPI_OP_MPI_BAND, [MPI_BAND])
    else
      AC_DEFINE(DUMPI_OP_MPI_BAND, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LOR], [defined if MPI_LOR is available])
    AH_TEMPLATE([DUMPI_OP_MPI_LOR], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_LOR is present], [dumpi_cv_have_mpi_lor],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_LOR;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_lor="yes"
        ], [
          dumpi_cv_have_mpi_lor="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_lor" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LOR, [1])
      AC_DEFINE(DUMPI_OP_MPI_LOR, [MPI_LOR])
    else
      AC_DEFINE(DUMPI_OP_MPI_LOR, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_BOR], [defined if MPI_BOR is available])
    AH_TEMPLATE([DUMPI_OP_MPI_BOR], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_BOR is present], [dumpi_cv_have_mpi_bor],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_BOR;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_bor="yes"
        ], [
          dumpi_cv_have_mpi_bor="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_bor" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_BOR, [1])
      AC_DEFINE(DUMPI_OP_MPI_BOR, [MPI_BOR])
    else
      AC_DEFINE(DUMPI_OP_MPI_BOR, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LXOR], [defined if MPI_LXOR is available])
    AH_TEMPLATE([DUMPI_OP_MPI_LXOR], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_LXOR is present], [dumpi_cv_have_mpi_lxor],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_LXOR;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_lxor="yes"
        ], [
          dumpi_cv_have_mpi_lxor="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_lxor" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LXOR, [1])
      AC_DEFINE(DUMPI_OP_MPI_LXOR, [MPI_LXOR])
    else
      AC_DEFINE(DUMPI_OP_MPI_LXOR, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_BXOR], [defined if MPI_BXOR is available])
    AH_TEMPLATE([DUMPI_OP_MPI_BXOR], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_BXOR is present], [dumpi_cv_have_mpi_bxor],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_BXOR;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_bxor="yes"
        ], [
          dumpi_cv_have_mpi_bxor="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_bxor" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_BXOR, [1])
      AC_DEFINE(DUMPI_OP_MPI_BXOR, [MPI_BXOR])
    else
      AC_DEFINE(DUMPI_OP_MPI_BXOR, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_MINLOC], [defined if MPI_MINLOC is available])
    AH_TEMPLATE([DUMPI_OP_MPI_MINLOC], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_MINLOC is present], [dumpi_cv_have_mpi_minloc],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_MINLOC;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_minloc="yes"
        ], [
          dumpi_cv_have_mpi_minloc="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_minloc" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MINLOC, [1])
      AC_DEFINE(DUMPI_OP_MPI_MINLOC, [MPI_MINLOC])
    else
      AC_DEFINE(DUMPI_OP_MPI_MINLOC, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_MAXLOC], [defined if MPI_MAXLOC is available])
    AH_TEMPLATE([DUMPI_OP_MPI_MAXLOC], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_MAXLOC is present], [dumpi_cv_have_mpi_maxloc],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_MAXLOC;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_maxloc="yes"
        ], [
          dumpi_cv_have_mpi_maxloc="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_maxloc" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_MAXLOC, [1])
      AC_DEFINE(DUMPI_OP_MPI_MAXLOC, [MPI_MAXLOC])
    else
      AC_DEFINE(DUMPI_OP_MPI_MAXLOC, [MPI_OP_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_REPLACE], [defined if MPI_REPLACE is available])
    AH_TEMPLATE([DUMPI_OP_MPI_REPLACE], [Map DUMPI operations to MPI])
    AC_CACHE_CHECK([whether the operation MPI_REPLACE is present], [dumpi_cv_have_mpi_replace],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Op op = MPI_REPLACE;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_replace="yes"
        ], [
          dumpi_cv_have_mpi_replace="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_replace" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_REPLACE, [1])
      AC_DEFINE(DUMPI_OP_MPI_REPLACE, [MPI_REPLACE])
    else
      AC_DEFINE(DUMPI_OP_MPI_REPLACE, [MPI_OP_NULL])
    fi

    AC_LANG_POP
  ]
)

