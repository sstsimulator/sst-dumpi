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

die("Usage $0 file1 file2\n") unless @ARGV == 2;

open A, "<$ARGV[0]" or die("Failed to open $ARGV[0] for reading: $!\n");
open B, "<$ARGV[1]" or die("Failed to open $ARGV[1] for reading: $!\n");

my @a = ();
my @b = ();
while(<A>) {
    my @t = split ' ', $_;
    push @a, \@t;
}
while(<B>) {
    my @t = split ' ', $_;
    push @b, \@t;
}

die("Files have different number of lines\n") unless @a == @b;

for my $i (0..$#a) {
    print($a[$i]->[0], " ",
	  ($a[$i]->[1] - $b[$i]->[1]), " ",
	  ($a[$i]->[2] - $b[$i]->[2]), " ",
	  $a[$i]->[3], " ",
	  $a[$i]->[4], "\n");
    die("MPI calls don't match\n") unless 
	($a[$i]->[3] eq $b[$i]->[3] and
	 $a[$i]->[4] eq $b[$i]->[4]);
}
