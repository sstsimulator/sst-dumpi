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

# Compute lowest and highest cost associated with each MPI function

use strict;
my %cpu = (
 'last-in' => -1,
 'maxcost' => {},
 'mincost' => {},
 'sumcost' => {},
 'rawcost' => {},
 'count' => {}
);
my %wall = (
 'last-in' => -1,
 'maxcost' => {},
 'mincost' => {},
 'sumcost' => {},
 'rawcost' => {},
 'count' => {}
);

while(<>) {
    if(/(.*?) (entering|returning) at walltime (.*?), cputime (.*?) seconds/) {
	my $mpi = $1;
	if($2 eq 'entering') {
	    $wall{'last-in'} = $3;
	    $cpu{'last-in'} = $4;
	}
	elsif($2 eq 'returning') {
	    my $wall_last_out = $3;
	    my $cpu_last_out = $4;
            unless(defined $wall{'maxcost'}->{$mpi}) {
                $wall{'maxcost'}->{$mpi} = 0;
                $wall{'mincost'}->{$mpi} = 1e100;
                $wall{'sumcost'}->{$mpi} = 0;
                $wall{'rawcost'}->{$mpi} = [];
                $wall{'count'}->{$mpi} = 0;
                $cpu{'maxcost'}->{$mpi} = 0;
                $cpu{'mincost'}->{$mpi} = 1e100;
                $cpu{'sumcost'}->{$mpi} = 0;
                $cpu{'rawcost'}->{$mpi} = [];
                $cpu{'count'}->{$mpi} = 0;
            }
	    my $wall_inside_mpi = $wall_last_out - $wall{'last-in'};
            my $cpu_inside_mpi = $cpu_last_out - $cpu{'last-in'};
	    $wall{'maxcost'}->{$mpi} = $wall_inside_mpi if $wall_inside_mpi > $wall{'maxcost'}->{$mpi};
	    $wall{'mincost'}->{$mpi} = $wall_inside_mpi if $wall_inside_mpi < $wall{'mincost'}->{$mpi};
            $wall{'sumcost'}->{$mpi} += $wall_inside_mpi;
            push @{$wall{'rawcost'}->{$mpi}}, $wall_inside_mpi;
            $wall{'count'}{$mpi} += 1;
	    $cpu{'maxcost'}->{$mpi} = $cpu_inside_mpi if $cpu_inside_mpi > $cpu{'maxcost'}->{$mpi};
	    $cpu{'mincost'}->{$mpi} = $cpu_inside_mpi if $cpu_inside_mpi < $cpu{'mincost'}->{$mpi};
            $cpu{'sumcost'}->{$mpi} += $cpu_inside_mpi;
            push @{$cpu{'rawcost'}->{$mpi}}, $cpu_inside_mpi;
            $cpu{'count'}{$mpi} += 1;
	}
	else {
	    die("Bogus input line: $_");
	}
    }

}

print "#Call\ttotal-count\twall-mincost\twall-maxcost\twall-average\twall-mean\tcpu-mincost\tcpu-maxcost\tcpu-average\tcpu-mean\n";
for my $key (keys %{$wall{'mincost'}}) {
    my $wallave = $wall{'sumcost'}->{$key} / $wall{'count'}->{$key};
    my $cpuave = $cpu{'sumcost'}->{$key} / $cpu{'count'}->{$key};
    my $mid = scalar(@{$wall{'rawcost'}->{$key}}) / 2;
    my $wallmean = $wall{'rawcost'}->{$key}->[$mid];
    my $cpumean = $cpu{'rawcost'}->{$key}->[$mid];
    if($mid != int($mid) and $mid > 0) {
        $wallmean = ($wallmean + $wall{'rawcost'}->{$key}->[$mid-1]) / 2;
        $cpumean = ($cpumean + $cpu{'rawcost'}->{$key}->[$mid-1]) / 2;
    }
    print "$key\t$wall{count}->{$key}\t$wall{mincost}->{$key}\t$wall{maxcost}->{$key}\t$wallave\t$wallmean\t$cpu{mincost}->{$key}\t$cpu{maxcost}->{$key}\t$cpuave\t$cpumean\n";
}
