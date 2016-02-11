!
!     This file is part of DUMPI: 
!         The MPI profiling library from the SST suite.
!     Copyright (c) 2009 Sandia Corporation.
!     This software is distributed under the BSD License.
!     Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
!     the U.S. Government retains certain rights in this software.
!     For more information, see the LICENSE file in the top 
!     SST/macroscale directory.
!
  subroutine libdumpi_get_f77_constants
    use mpi
    implicit none
    integer status_size
    integer compare_ident, compare_similar, compare_unequal
    status_size = mpi_status_size
    compare_ident = mpi_ident
    compare_similar = mpi_similar
    compare_unequal = mpi_unequal
    ! Call back the C code to define status info (yuck)
    call libdumpi_f77_put_status_info(mpi_status_ignore, mpi_statuses_ignore)
    return
    end
