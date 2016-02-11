AC_DEFUN([DUMPI_TEST_MPI_TYPES],
  [
    dnl Test whether MPI datatypes are defined properly
    dnl The MPI implementation on Hera at LLNL fails to define some of these
    AC_LANG_PUSH(C)

    AH_TEMPLATE([DUMPI_HAVE_MPI_DATATYPE_NULL], [defined if MPI_DATATYPE_NULL is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_DATATYPE_NULL], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_DATATYPE_NULL is present], [dumpi_cv_have_mpi_datatype_null],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_DATATYPE_NULL;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_datatype_null="yes"
        ], [
          dumpi_cv_have_mpi_datatype_null="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_datatype_null" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_DATATYPE_NULL, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_DATATYPE_NULL, [MPI_DATATYPE_NULL])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_DATATYPE_NULL, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_CHAR], [defined if MPI_CHAR is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_CHAR], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_CHAR is present], [dumpi_cv_have_mpi_char],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_CHAR;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_char="yes"
        ], [
          dumpi_cv_have_mpi_char="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_char" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_CHAR, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_CHAR, [MPI_CHAR])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_CHAR, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_SIGNED_CHAR], [defined if MPI_SIGNED_CHAR is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_SIGNED_CHAR], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_SIGNED_CHAR is present], [dumpi_cv_have_mpi_signed_char],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_SIGNED_CHAR;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_signed_char="yes"
        ], [
          dumpi_cv_have_mpi_signed_char="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_signed_char" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SIGNED_CHAR, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_SIGNED_CHAR, [MPI_SIGNED_CHAR])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_SIGNED_CHAR, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_UNSIGNED_CHAR], [defined if MPI_UNSIGNED_CHAR is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_UNSIGNED_CHAR], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_UNSIGNED_CHAR is present], [dumpi_cv_have_mpi_unsigned_char],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_UNSIGNED_CHAR;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_unsigned_char="yes"
        ], [
          dumpi_cv_have_mpi_unsigned_char="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unsigned_char" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UNSIGNED_CHAR, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED_CHAR, [MPI_UNSIGNED_CHAR])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED_CHAR, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_BYTE], [defined if MPI_BYTE is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_BYTE], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_BYTE is present], [dumpi_cv_have_mpi_byte],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_BYTE;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_byte="yes"
        ], [
          dumpi_cv_have_mpi_byte="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_byte" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_BYTE, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_BYTE, [MPI_BYTE])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_BYTE, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_WCHAR], [defined if MPI_WCHAR is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_WCHAR], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_WCHAR is present], [dumpi_cv_have_mpi_wchar],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_WCHAR;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_wchar="yes"
        ], [
          dumpi_cv_have_mpi_wchar="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_wchar" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_WCHAR, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_WCHAR, [MPI_WCHAR])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_WCHAR, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_SHORT], [defined if MPI_SHORT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_SHORT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_SHORT is present], [dumpi_cv_have_mpi_short],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_SHORT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_short="yes"
        ], [
          dumpi_cv_have_mpi_short="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_short" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SHORT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_SHORT, [MPI_SHORT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_SHORT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_UNSIGNED_SHORT], [defined if MPI_UNSIGNED_SHORT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_UNSIGNED_SHORT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_UNSIGNED_SHORT is present], [dumpi_cv_have_mpi_unsigned_short],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_UNSIGNED_SHORT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_unsigned_short="yes"
        ], [
          dumpi_cv_have_mpi_unsigned_short="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unsigned_short" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UNSIGNED_SHORT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED_SHORT, [MPI_UNSIGNED_SHORT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED_SHORT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_INT], [defined if MPI_INT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_INT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_INT is present], [dumpi_cv_have_mpi_int],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_INT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_int="yes"
        ], [
          dumpi_cv_have_mpi_int="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_int" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_INT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_INT, [MPI_INT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_INT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_UNSIGNED], [defined if MPI_UNSIGNED is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_UNSIGNED], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_UNSIGNED is present], [dumpi_cv_have_mpi_unsigned],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_UNSIGNED;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_unsigned="yes"
        ], [
          dumpi_cv_have_mpi_unsigned="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unsigned" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UNSIGNED, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED, [MPI_UNSIGNED])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LONG], [defined if MPI_LONG is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_LONG], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_LONG is present], [dumpi_cv_have_mpi_long],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_LONG;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_long="yes"
        ], [
          dumpi_cv_have_mpi_long="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_long" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LONG, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG, [MPI_LONG])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_UNSIGNED_LONG], [defined if MPI_UNSIGNED_LONG is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_UNSIGNED_LONG], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_UNSIGNED_LONG is present], [dumpi_cv_have_mpi_unsigned_long],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_UNSIGNED_LONG;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_unsigned_long="yes"
        ], [
          dumpi_cv_have_mpi_unsigned_long="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unsigned_long" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UNSIGNED_LONG, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED_LONG, [MPI_UNSIGNED_LONG])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED_LONG, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_FLOAT], [defined if MPI_FLOAT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_FLOAT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_FLOAT is present], [dumpi_cv_have_mpi_float],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_FLOAT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_float="yes"
        ], [
          dumpi_cv_have_mpi_float="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_float" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_FLOAT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_FLOAT, [MPI_FLOAT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_FLOAT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_DOUBLE], [defined if MPI_DOUBLE is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_DOUBLE], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_DOUBLE is present], [dumpi_cv_have_mpi_double],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_DOUBLE;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_double="yes"
        ], [
          dumpi_cv_have_mpi_double="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_double" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_DOUBLE, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_DOUBLE, [MPI_DOUBLE])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_DOUBLE, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LONG_DOUBLE], [defined if MPI_LONG_DOUBLE is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_LONG_DOUBLE], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_LONG_DOUBLE is present], [dumpi_cv_have_mpi_long_double],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_LONG_DOUBLE;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_long_double="yes"
        ], [
          dumpi_cv_have_mpi_long_double="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_long_double" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LONG_DOUBLE, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_DOUBLE, [MPI_LONG_DOUBLE])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_DOUBLE, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LONG_LONG_INT], [defined if MPI_LONG_LONG_INT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_LONG_LONG_INT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_LONG_LONG_INT is present], [dumpi_cv_have_mpi_long_long_int],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_LONG_LONG_INT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_long_long_int="yes"
        ], [
          dumpi_cv_have_mpi_long_long_int="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_long_long_int" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LONG_LONG_INT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_LONG_INT, [MPI_LONG_LONG_INT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_LONG_INT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG], [defined if MPI_UNSIGNED_LONG_LONG is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_UNSIGNED_LONG_LONG], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_UNSIGNED_LONG_LONG is present], [dumpi_cv_have_mpi_unsigned_long_long],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_UNSIGNED_LONG_LONG;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_unsigned_long_long="yes"
        ], [
          dumpi_cv_have_mpi_unsigned_long_long="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unsigned_long_long" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED_LONG_LONG, [MPI_UNSIGNED_LONG_LONG])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_UNSIGNED_LONG_LONG, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LONG_LONG], [defined if MPI_LONG_LONG is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_LONG_LONG], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_LONG_LONG is present], [dumpi_cv_have_mpi_long_long],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_LONG_LONG;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_long_long="yes"
        ], [
          dumpi_cv_have_mpi_long_long="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_long_long" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LONG_LONG, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_LONG, [MPI_LONG_LONG])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_LONG, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_PACKED], [defined if MPI_PACKED is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_PACKED], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_PACKED is present], [dumpi_cv_have_mpi_packed],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_PACKED;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_packed="yes"
        ], [
          dumpi_cv_have_mpi_packed="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_packed" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_PACKED, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_PACKED, [MPI_PACKED])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_PACKED, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LB], [defined if MPI_LB is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_LB], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_LB is present], [dumpi_cv_have_mpi_lb],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_LB;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_lb="yes"
        ], [
          dumpi_cv_have_mpi_lb="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_lb" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LB, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_LB, [MPI_LB])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_LB, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_UB], [defined if MPI_UB is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_UB], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_UB is present], [dumpi_cv_have_mpi_ub],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_UB;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_ub="yes"
        ], [
          dumpi_cv_have_mpi_ub="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_ub" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_UB, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_UB, [MPI_UB])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_UB, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_FLOAT_INT], [defined if MPI_FLOAT_INT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_FLOAT_INT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_FLOAT_INT is present], [dumpi_cv_have_mpi_float_int],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_FLOAT_INT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_float_int="yes"
        ], [
          dumpi_cv_have_mpi_float_int="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_float_int" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_FLOAT_INT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_FLOAT_INT, [MPI_FLOAT_INT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_FLOAT_INT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_DOUBLE_INT], [defined if MPI_DOUBLE_INT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_DOUBLE_INT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_DOUBLE_INT is present], [dumpi_cv_have_mpi_double_int],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_DOUBLE_INT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_double_int="yes"
        ], [
          dumpi_cv_have_mpi_double_int="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_double_int" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_DOUBLE_INT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_DOUBLE_INT, [MPI_DOUBLE_INT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_DOUBLE_INT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LONG_INT], [defined if MPI_LONG_INT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_LONG_INT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_LONG_INT is present], [dumpi_cv_have_mpi_long_int],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_LONG_INT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_long_int="yes"
        ], [
          dumpi_cv_have_mpi_long_int="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_long_int" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LONG_INT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_INT, [MPI_LONG_INT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_INT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_SHORT_INT], [defined if MPI_SHORT_INT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_SHORT_INT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_SHORT_INT is present], [dumpi_cv_have_mpi_short_int],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_SHORT_INT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_short_int="yes"
        ], [
          dumpi_cv_have_mpi_short_int="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_short_int" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_SHORT_INT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_SHORT_INT, [MPI_SHORT_INT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_SHORT_INT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_2INT], [defined if MPI_2INT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_2INT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_2INT is present], [dumpi_cv_have_mpi_2int],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_2INT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_2int="yes"
        ], [
          dumpi_cv_have_mpi_2int="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_2int" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_2INT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_2INT, [MPI_2INT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_2INT, [MPI_DATATYPE_NULL])
    fi


    AH_TEMPLATE([DUMPI_HAVE_MPI_LONG_DOUBLE_INT], [defined if MPI_LONG_DOUBLE_INT is available])
    AH_TEMPLATE([DUMPI_DATATYPE_MPI_LONG_DOUBLE_INT], [Map DUMPI datatypes to MPI types])
    AC_CACHE_CHECK([whether the datatype MPI_LONG_DOUBLE_INT is present], [dumpi_cv_have_mpi_long_double_int],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Datatype dt = MPI_LONG_DOUBLE_INT;
            MPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_long_double_int="yes"
        ], [
          dumpi_cv_have_mpi_long_double_int="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_long_double_int" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_LONG_DOUBLE_INT, [1])
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_DOUBLE_INT, [MPI_LONG_DOUBLE_INT])
    else
      AC_DEFINE(DUMPI_DATATYPE_MPI_LONG_DOUBLE_INT, [MPI_DATATYPE_NULL])
    fi

    AC_LANG_POP
  ]
)


