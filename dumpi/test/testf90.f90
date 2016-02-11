!
!  This file is part of DUMPI: 
!               The MPI profiling library from the SST suite.
!  Copyright (c) 2009 Sandia Corporation.
!  This software is distributed under the BSD License.
!  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
!  the U.S. Government retains certain rights in this software.
!  For more information, see the LICENSE file in the top 
!  SST/macroscale directory.
!
program testf90
  use mpi
  implicit none
  integer ierr, myid, numprocs, status, v
  call mpi_init(ierr)
  call mpi_comm_rank(MPI_COMM_WORLD, myid, ierr)
  call mpi_comm_size(MPI_COMM_WORLD, numprocs, ierr)
  call mpi_barrier(MPI_COMM_WORLD, ierr)
  if(myid == 0) then
     write(*,*)'World size:',numprocs
  endif
  if(numprocs > 1) then
     if(myid == 0) then
        v = 10
        call mpi_send(v, 1, MPI_INTEGER, 1, 0, MPI_COMM_WORLD, ierr)
     else if(myid == 1) then
        call mpi_recv(v, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, status, ierr)
     end if
  end if
  call mpi_finalize(ierr)  
  stop
end program testf90
