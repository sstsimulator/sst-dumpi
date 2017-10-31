AC_DEFUN([CHECK_CONST_QUALIFIERS], [

AH_TEMPLATE([DUMPI_CONST], [whether MPI 2 functions are const])
AH_TEMPLATE([DUMPI_DEPRECATED_CONST], [whether deprecated MPI functions are const])

if test "$enable_libdumpi" = "yes"; then

constType=`$srcdir/bin/check_const $CXX`


if test "X$constType" = "Xnone"; then
AC_MSG_CHECKING([whether MPI uses const qualifier])
AC_MSG_RESULT([no])
AC_DEFINE(DUMPI_CONST, []) 
AC_DEFINE(DUMPI_DEPRECATED_CONST, []) 
else
if test "X$constType" = "XnoDeprecated"; then
AC_MSG_CHECKING([whether MPI uses const qualifier])
AC_MSG_RESULT([not for deprecated])
AC_DEFINE(DUMPI_CONST, [const]) 
AC_DEFINE(DUMPI_DEPRECATED_CONST, []) 
else
#all have const
AC_MSG_CHECKING([whether MPI uses const qualifier])
AC_MSG_RESULT([yes])
AC_DEFINE(DUMPI_CONST, [const]) 
AC_DEFINE(DUMPI_DEPRECATED_CONST, [const]) 
fi
fi

fi #enable libdumpi


])


