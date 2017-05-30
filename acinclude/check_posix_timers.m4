AC_DEFUN([CHECK_TIMERS], [

# We allow the caller to suppress high-resoultion timers.
# The reason is that librt sometimes (GNU) brings in pthreads, and certain
# MPI implementations (e.g. MPICH-1) cannot work correctly with pthreads.
AH_TEMPLATE([DUMPI_DISABLE_POSIX_TIMERS], [Skip high resolution timers])
AC_MSG_CHECKING([whether POSIX timers should be used])
AC_ARG_ENABLE(posix-timers,
  [  --disable-posix-timers  Disable POSIX high-resolution timers)],
  [
    if test "$enableval" = "no"; then
      disable_posix_timers="yes",
      AC_DEFINE(DUMPI_DISABLE_POSIX_TIMERS, [1])
      AC_MSG_RESULT([POSIX timers turned off])
    fi
  ],[
    disable_posix_timers="no"
    # Test whether librt is found (hack hack).
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
         [#include <time.h>],
         [clock_getres((clockid_t)0, NULL);]
      )],
      [
        AC_MSG_RESULT([available without linking -lrt])
      ],[
        oldlibs="$LIBS"
        LIBS="$LIBS -lrt"
        AC_LINK_IFELSE(
          [AC_LANG_PROGRAM(
            [#include <time.h>],
            [clock_getres((clockid_t)0, NULL);]
          )],
          [
            AC_MSG_RESULT([available when linked with -lrt])
          ],[
            LIBS="$oldlibs"
            AC_DEFINE(DUMPI_DISABLE_POSIX_TIMERS, [1])
            AC_MSG_RESULT([POSIX timers not available])
          ]
        )
      ]
    )
  ]
)

# Check for non-POSIX timers
AC_CHECK_FUNCS([gettimeofday getrusage])

])

