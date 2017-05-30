AC_DEFUN([CHECK_MPIIO], [

AC_ARG_ENABLE(mpio,
  [  --disable-mpio   Disable support for MPIO],
  [
    if test "$enableval" = "no"; then
      DUMPI_DISABLE_MPIO
    fi
  ], [
    DUMPI_DISABLE_MPIO
  ]
)

])

