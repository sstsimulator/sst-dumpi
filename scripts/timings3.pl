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

my $binsize=180;

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

my $first_timestamp = 1e100;
my $wall_last_in = -1;
my $wall_last_out = -1;
my @wall_in_mpi = ();
my @wall_in_coll = ();
my @wall_in_wait = ();
my @wall_outside_mpi = ();
while(<>) {
    if(/(.*?) (entering|returning) at walltime (.*?), cputime (.*?) seconds/) {
	$first_timestamp = $3 if($3 < $first_timestamp);
	my $bin = int($3 - $first_timestamp) / int($binsize);
	if($2 eq 'entering') {
	    $wall_last_in = $3;
	    if($wall_last_out >= 0) {
		$wall_outside_mpi[$bin] += $wall_last_in - $wall_last_out;
	    }
	}
	elsif($2 eq 'returning') {
	    $wall_last_out = $3;
	    if($wall_last_in >= 0) {
		my $deltat = $wall_last_out - $wall_last_in;
		if(defined $collmap{$1}) {
		    $wall_in_coll[$bin] += $deltat;
		}
		elsif(defined $waitmap{$1}) {
		    $wall_in_wait[$bin] += $deltat;
		}
		else {
		    $wall_in_mpi[$bin] += $deltat;
		}
	    }
	}
	else {
	    die("Bogus input line: $_");
	}
    }
}

my $maxsize = &maxval(scalar(@wall_in_mpi),
		      scalar(@wall_in_coll),
		      scalar(@wall_in_wait),
		      scalar(@wall_outside_mpi));

printf "# %8s    %10s    %10s    %10s    %10s\n", "START", "MPI", "COLL", "WAIT", "NOMPI";
for(my $i = 0; $i < $maxsize; ++$i) {
    printf("  %8.2f    %10.4f    %10.4f    %10.4f    %10.4f\n",
	   $i*$binsize,
	   defined($wall_in_mpi[$i]) ? $wall_in_mpi[$i] : 0,
	   defined($wall_in_coll[$i]) ? $wall_in_coll[$i] : 0,
	   defined($wall_in_wait[$i]) ? $wall_in_wait[$i] : 0,
	   defined($wall_outside_mpi[$i]) ? $wall_outside_mpi[$i] : 0);
}

sub maxval {
    my @v = sort {$b <=> $a} @_;
    return $v[0];
}
