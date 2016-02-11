C
C     This file is part of DUMPI: 
C         The MPI profiling library from the SST suite.
C     Copyright (c) 2009 Sandia Corporation.
C     This software is distributed under the BSD License.
C     Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
C     the U.S. Government retains certain rights in this software.
C     For more information, see the LICENSE file in the top 
C     SST/macroscale directory.
C
      subroutine libdumpi_get_f77_constants
      implicit none
      include 'mpif.h'
      integer status_size
      integer compare_ident, compare_similar, compare_unequal
      status_size = mpi_status_size
      compare_ident = mpi_ident
      compare_similar = mpi_similar
      compare_unequal = mpi_unequal
C     Call back the C code to define status info (yuck)
      call libdumpi_f77_put_status_info(mpi_status_ignore,
     &     mpi_statuses_ignore)
      return
      end
