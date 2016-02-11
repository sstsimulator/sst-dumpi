#!/usr/bin/perl -w

#
#  This file is part of DUMPI: 
#               The MPI profiling library from the SST suite.
#  Copyright (c) 2009 Sandia Corporation.  
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the LICENSE file in the top 
#  SST/macroscale directory.
#

use strict;

my @colls = qw(
  MPI_Barrier          MPI_Bcast         MPI_Gather           MPI_Gatherv
  MPI_Scatter          MPI_Scatterv      MPI_Allgather        MPI_Allgatherv
  MPI_Alltoall         MPI_Alltoallv     MPI_Reduce           MPI_Allreduce
  MPI_Reduce_scatter   MPI_Scan          MPI_Comm_dup         MPI_Comm_create          
  MPI_Comm_split       MPI_Comm_free     MPI_Intercomm_create MPI_Init
  MPI_Finalize         MPI_Comm_spawn    MPI_Comm_spawn_multiple 
  MPI_Alltoallw        MPI_Exscan        MPI_Init_thread      MPI_File_open
  MPI_File_close       MPI_File_set_size MPI_File_preallocate MPI_File_set_info 
  MPI_File_set_view    MPI_File_read_at_all        MPI_File_write_at_all
  MPI_File_read_all    MPI_File_write_all          MPI_File_read_ordered   
  MPI_File_seek_shared MPI_File_read_at_all_begin  MPI_File_read_at_all_end
  MPI_File_write_at_all_begin            MPI_File_write_at_all_end
  MPI_File_read_all_begin                MPI_File_read_all_end
  MPI_File_write_all_begin               MPI_File_write_all_end
  MPI_File_read_ordered_begin            MPI_File_read_ordered_end
  MPI_File_write_ordered_begin           MPI_File_write_ordered_end 
  MPI_File_set_atomicity                 MPI_File_sync            
);

my @waits = qw(MPI_Wait MPI_Waitany MPI_Waitsome MPI_Waitall MPI_Probe
               MPI_Start MPI_Startall MPI_Win_wait
               MPIO_Wait MPIO_Waitany MPIO_Waitsome MPIO_Waitall);

my(%collmap, %waitmap);
for my $c (@colls) { $collmap{$c} = 1; }
for my $w (@waits) { $waitmap{$w} = 1; }

my $wall_last_in = -1;
my $wall_last_out = -1;
my $wall_in_mpi = 0.0;
my $wall_in_coll = 0.0;
my $wall_in_wait = 0.0;
my $wall_outside_mpi = 0.0;
while(<>) {
    if(/(.*?) (entering|returning) at walltime (.*?), cputime (.*?) seconds/) {
	if($2 eq 'entering') {
	    $wall_last_in = $3;
	    if($wall_last_out >= 0) {
		$wall_outside_mpi += $wall_last_in - $wall_last_out;
	    }
	}
	elsif($2 eq 'returning') {
	    $wall_last_out = $3;
	    if($wall_last_in >= 0) {
		my $deltat = $wall_last_out - $wall_last_in;
		if(defined $collmap{$1}) {
		    $wall_in_coll += $deltat;
		}
		elsif(defined $waitmap{$1}) {
		    $wall_in_wait += $deltat;
		}
		else {
		    $wall_in_mpi += $deltat;
		}
	    }
	}
	else {
	    die("Bogus input line: $_");
	}
    }
}

print "$wall_in_mpi $wall_in_coll $wall_in_wait $wall_outside_mpi\n";
