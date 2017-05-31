AC_DEFUN([CHECK_CONST_QUALIFIERS], [

AH_TEMPLATE([DUMPI_CONST], [whether MPI 2 functions are const])
AH_TEMPLATE([DUMPI_DEPRECATED_CONST], [whether deprecated MPI functions are const])

if test "$enable_libdumpi" = "yes"; then
AC_LANG_PUSH(C)

SAVE_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS -Werror=incompatible-pointer-types-discards-qualifiers -Werror=incompatible-pointer-types"

AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
    [
      #include <mpi.h>
    ],[
      const void* src = 0;
      MPI_Send(src, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    ])],
  [
   #if here, then MPI_Send has const buffers
   AC_MSG_CHECKING([whether MPI uses const qualifier])
   AC_MSG_RESULT([yes])
   AC_DEFINE(DUMPI_CONST, [const]) 
  ],[
   AC_MSG_CHECKING([whether MPI uses const qualifier])
   AC_MSG_RESULT([no])
   AC_DEFINE(DUMPI_CONST, []) 
  ])

AC_LINK_IFELSE(
  [AC_LANG_PROGRAM(
    [
      #include <mpi.h>
    ],[
      int count;
      const int* blocklens = 0;
      const MPI_Aint* displs = 0;
      const MPI_Datatype* oldtype = 0;
      MPI_Datatype* newtype = 0;
      MPI_Type_struct(count, blocklens, displs, oldtype, newtype);
    ])],
  [
   #if here, then MPI_Send has const buffers
   AC_MSG_CHECKING([whether deprecated MPI uses const qualifier])
   AC_MSG_RESULT([yes])
   AC_DEFINE(DUMPI_DEPRECATED_CONST, [const]) 
  ],[
   AC_MSG_CHECKING([whether deprecated MPI uses const qualifier])
   AC_MSG_RESULT([no])
   AC_DEFINE(DUMPI_DEPRECATED_CONST, []) 
  ])

CPPFLAGS="$SAVE_CPPFLAGS"
AC_LANG_POP
fi


])


