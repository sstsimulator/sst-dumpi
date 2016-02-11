AC_DEFUN([AX_MPIVERSION], [
  AC_MSG_CHECKING(mpi version)
  AC_CACHE_VAL(ac_cv_mpi_version, [
        AC_RUN_IFELSE( 
          [AC_LANG_PROGRAM([#include <mpi.h>],
                           [return MPI_VERSION == 2;]
           )],
          [ac_cv_mpi_version=3],
          [ac_cv_mpi_version=2], 
          []
        ) ]
  )
          
   
  AC_MSG_RESULT($ac_cv_mpi_version)

  AS_IF([test "X$ac_cv_mpi_version" == "X2"],
    [$1],
    [$2]
  )
  
])
