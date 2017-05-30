AC_DEFUN([CHECK_MPI_VERSION], [

AC_ARG_WITH(mpi-version,
  [  --with-mpi-version[=2,3,etc]   The MPI verstion to compile for],
  [
    mpiversion="$withval" 
  ],[
    mpiversion=2 
  ]
)

AH_TEMPLATE([IS_MPI_TWO],
            [Define to indicate which mpibindings to use])
if test "X$mpiversion" = "X2"; then
AC_DEFINE(IS_MPI_TWO, [1])
AM_CONDITIONAL([WITH_MPI_TWO], true)
AM_CONDITIONAL([WITH_MPI_THREE], false)
fi
if test "X$mpiversion" = "X3"; then
AM_CONDITIONAL([WITH_MPI_TWO], false)
AM_CONDITIONAL([WITH_MPI_THREE], true)
fi

])
