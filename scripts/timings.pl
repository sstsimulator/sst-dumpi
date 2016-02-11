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

my $wall_last_in = -1;
my $wall_last_out = -1;
my $cpu_last_in = -1;
my $cpu_last_out = -1;
my $wall_inside_mpi = 0.0;
my $wall_outside_mpi = 0.0;
my $cpu_inside_mpi = 0.0;
my $cpu_outside_mpi = 0.0;
while(<>) {
    if(/(.*?) (entering|returning) at walltime (.*?), cputime (.*?) seconds/) {
	if($2 eq 'entering') {
	    $wall_last_in = $3;
	    $cpu_last_in = $4;
	    if($wall_last_out >= 0) {
		$wall_outside_mpi += $wall_last_in - $wall_last_out;
		$cpu_outside_mpi += $cpu_last_in - $cpu_last_out;
	    }
	}
	elsif($2 eq 'returning') {
	    $wall_last_out = $3;
	    $cpu_last_out = $4;
	    if($wall_last_in >= 0) {
		$wall_inside_mpi += $wall_last_out - $wall_last_in;
		$cpu_inside_mpi += $cpu_last_out - $cpu_last_in;
	    }
	}
	else {
	    die("Bogus input line: $_");
	}
    }
}

print "$wall_inside_mpi $wall_outside_mpi $cpu_inside_mpi $cpu_outside_mpi\n";
