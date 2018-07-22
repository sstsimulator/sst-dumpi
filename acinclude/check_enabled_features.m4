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

AM_CONDITIONAL([WITH_BIN], [test "$disable_bin" != "yes"])
AM_CONDITIONAL([WITH_TEST], [test "$enable_test" = "yes"])
AM_CONDITIONAL([WITH_LIBDUMPI], [test "$enable_libdumpi" = "yes"])
AM_CONDITIONAL([WITH_LIBUNDUMPI], [test "$disable_libundumpi" != "yes"])

if test "$enable_libdumpi" = "yes"; then
  DUMPI_FUNCTIONS
  DUMPI_TYPES
  DUMPI_OPS
  DUMPI_CONSTANTS
fi

])
