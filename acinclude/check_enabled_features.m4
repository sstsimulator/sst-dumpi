AC_DEFUN([CHECK_ENABLED_FEATURES], [

# Options to turn off building different parts.
AC_ARG_ENABLE(bin,
  [  --disable-bin           Don't build dumpi2ascii (trace file converter)],
  [
    if test "$enableval" = "no"; then
      disable_bin="yes"
    fi
  ],
  disable_bin="" 
)

AC_ARG_ENABLE(libdumpi,
  [  --enable-libdumpi       Build libdumpi (the trace library)],
  [
    enable_libdumpi="$enableval"
  ],
  disable_libdumpi=""
)

AC_ARG_ENABLE(test,
  [  --enable-test           Build testmpi (coverage testing)],
  [
    enable_test="$enableval"
  ],
  enable_test="no"
)

AC_ARG_ENABLE(libundumpi,
  [  --disable-libundumpi    Don't build libundumpi (the trace file reader)],
  [
    if test "$enableval" = "no"; then
      disable_libundumpi="yes"
      disable_bin="yes"
    fi
  ],
  disable_libundumpi=""
)

# If we are building binary utlities, test wheter we can build the
# dumpi-to-otf converter.
if test "$disable_bin" != "yes"; then
   oldlibs="$LIBS"
   LIBS="$LIBS -lotf -lz"
   AC_MSG_CHECKING([wheter OTF library and header file can be found])
   AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [
	  #include <otf.h>
	  #include <stdlib.h>
        ],[
	  OTF_FileManager *mgr = NULL;
	  mgr = OTF_FileManager_open(10);
	  if(!mgr) {
	    return EXIT_FAILURE;
	  }
	  OTF_FileManager_close(mgr);
	  return EXIT_SUCCESS;
        ]
      )],
      [
        AC_MSG_RESULT([yes])
	enable_otf="yes"	
      ],[
        AC_MSG_RESULT([no])
	LIBS="$oldlibs"
      ]
    )
fi

AM_CONDITIONAL([WITH_BIN], [test "$disable_bin" != "yes"])
AM_CONDITIONAL([WITH_TEST], [test "$enable_test" = "yes"])
AM_CONDITIONAL([WITH_LIBDUMPI], [test "$enable_libdumpi" = "yes"])
AM_CONDITIONAL([WITH_LIBUNDUMPI], [test "$disable_libundumpi" != "yes"])
AM_CONDITIONAL([WITH_OTF], [test "$enable_otf" = "yes"])

if test "$enable_libdumpi" = "yes"; then
  DUMPI_FUNCTIONS
  DUMPI_TYPES
  DUMPI_OPS
  DUMPI_CONSTANTS
fi

])
