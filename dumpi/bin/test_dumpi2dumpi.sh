#!/bin/sh

#
#   This file is part of DUMPI: 
#                The MPI profiling library from the SST suite.
#   Copyright (c) 2009 Sandia Corporation.
#   This software is distributed under the BSD License.
#   Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
#   the U.S. Government retains certain rights in this software.
#   For more information, see the LICENSE file in the top 
#   SST/macroscale directory.
#

./dumpi2dumpi -M MPI_Comm_rank -M MPI_Comm_size             \
         -i $srcdir/../../tests/traces/testtrace-0000.bin \
         -o d2d.bin

./dumpi2ascii -F d2d.bin > callcounts.txt
diff -q callcounts.txt $srcdir/../../tests/traces/callcounts-single.txt
good="$?"
rm -f d2d.bin

./dumpi2dumpi -F -m MPI_Allreduce                           \
         -I $srcdir/../../tests/traces/testtrace.meta     \
         -o d2d-all

for file in d2d-all-*.bin; do
  ./dumpi2ascii -F $file > callcounts-$file.txt
  diff -I '^starttime=' -q callcounts-$file.txt $srcdir/../../tests/traces/callcounts-batch.txt
  current=$?
  #let good=good+current
  good=`awk "BEGIN{print $good+$current}"`
done

rm -f d2d-all* callcounts*.txt

exit $good
