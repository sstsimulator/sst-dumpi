
AC_DEFUN([CHECK_FORTRAN_ENABLED], [

AC_ARG_ENABLE(f77,
  [  --enable-f77[=path]     Build fortran77 bindings for libdumpi],
  [
    if test "$enableval" != "no"; then
      if test "$enableval" != "yes"; then
        F77="$enableval"
      fi
      enable_f77="yes"
      MPIF77="$F77"
    fi
  ],
  enable_f77="no"
)

AC_ARG_ENABLE(f90,
  [  --enable-f90[=path]     Build fortran90 bindings for libdumpi],
  [
    if test "$enableval" != "no"; then
      if test "$enableval" != "yes"; then
        FC="$enableval"
        F77="$enableval"
        #use for both f90 and f77
        MPIF90="$FC"
      fi
      enable_f90="yes"
      enable_f77="yes"
    fi
  ],
  enable_f90="no"
)

AM_CONDITIONAL([WITH_MPIF77], [test "$MPIF77" != ""])
AM_CONDITIONAL([WITH_MPIF90], [test "$MPIF90" != ""])

])

AC_DEFUN([SANITY_CHECK_F90], [
 AC_MSG_CHECKING([Fortran 90 compiler])
 if test "$FC" == ""; then
    AC_MSG_ERROR([no F90 compiler found])
 else
    AC_MSG_RESULT([found $FC])
    F77="$FC"
 fi
 AC_FC_WRAPPERS
 AC_MSG_CHECKING([whether $FC can compile and link an MPI program])
 AC_LANG_PUSH([Fortran])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [],
      [
      use mpi
      implicit none
      integer ierr
      call mpi_init(ierr)
      call mpi_barrier(mpi_comm_world, ierr)
      call mpi_finalize(ierr)
      ]
    )],
    [
      dnl Well, at least it compiles and links
      AC_MSG_RESULT([yes])
    ],[
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([Please ensure that the FC variable points to an MPI-enabled Fortran90 compiler])
    ]
  )
 AC_LANG_POP([Fortran])   
])

AC_DEFUN([SANITY_CHECK_F77], [
 AC_MSG_CHECKING([Fortran77 compiler])
 if test "$F77" == ""; then
    AC_MSG_ERROR([no F77 compiler found])
 else
    AC_MSG_RESULT([found $F77])
 fi
 AC_F77_WRAPPERS
 AC_MSG_CHECKING([whether $F77 can compile and link an MPI program])
 AC_LANG_PUSH([Fortran 77])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [],
      [
  implicit none
  include 'mpif.h'
  integer ierr
  call mpi_init(ierr)
  call mpi_barrier(mpi_comm_world, ierr)
  call mpi_finalize(ierr)
      ]
    )],
    [
      dnl Well, at least it compiles and links
      AC_MSG_RESULT([yes])
    ],[
      AC_MSG_RESULT([no.])
      AC_MSG_ERROR([Please ensure that the F77 variable points to an MPI-enabled Fortran77 compiler])
    ]
  )
 AC_LANG_POP([Fortran 77])   
])

