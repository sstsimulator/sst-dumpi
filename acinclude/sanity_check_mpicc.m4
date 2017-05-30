AC_DEFUN([SANITY_CHECK_MPI], [

if test "$enable_libdumpi" = "yes"; then
  # We'd better have an MPI-capable C compiler.
  AC_MSG_CHECKING([whether $CC can compile and link an MPI program])
  AC_LANG_PUSH([C])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [#include <mpi.h>],
      [
        int aargc; char **aargv;
        MPI_Init(&aargc, &aargv);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Finalize();
      ]
    )],
    [ AC_MSG_RESULT([yes]) ],
    [
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([Building libdumpi requires an MPI-capable C compiler.])
    ]
  )
  AC_LANG_POP([C])


fi # end of 'if test "$enable_libdumpi" = "yes"' block

])

