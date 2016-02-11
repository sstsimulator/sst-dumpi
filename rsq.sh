#!/bin/bash

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

CMD=`getopt -o cs --long libdumpi,libundumpi,dumpi2ascii,mpitest,clean -n 'rsq.sh' -- "$@"`
if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi
eval set -- "$CMD"

while true; do
   case "$1" in
      --libdumpi) build_libdumpi="yes" ; shift ;;
      --libundumpi) build_libundumpi="yes"; shift ;;
      --mpitest) build_mpitest="yes"; shift ;;
      --dumpi2ascii) build_dumpi2ascii="yes"; shift ;;
       -c) build_libdumpi="yes"; build_mpitest="yes"; shift ;;
       -s) build_libundumpi="yes"; build_dumpi2ascii="yes"; shift ;;
       --clean) rm -rf libdumpi-rsq.a libundumpi-rsq.a mpitest dumpi2ascii build-rsq core ; exit 0 ;;
       --) shift; break ;;
       *) echo "parse error." ; exit 1 ;;
   esac
done

echo -e "build_libdumpi=$build_libdumpi\nbuild_libundumpi=$build_libundumpi\nbuild_mpitest=$build_mpitest\n"

# Hack for RSQ until we figure out a better way to
# get autotools to recognize it.

if test "$CC" = ""; then
  CC=cc
fi
if test "$AR" = ""; then
  AR=ar
fi

if test ! -d build-rsq; then
  mkdir build-rsq
fi

# Remove this definition (comment out) if you 
# don't want to write to a memory buffer.
membuffer="-DDUMPI_WRITE_TO_MEMORY_BUFFER"

OPT="-O3"
#OPT="-g"
COMMON="-DDUMPI_HOST_NAME_MAX=64"
CFLAGS="$CFLAGS_COMMON $OPT -DDUMPI_MPIO_USES_MPI_REQUESTS -DDUMPI_SKIP_MPIO_WAITALL -DDUMPI_SKIP_MPIO_TESTALL -DDUMPI_SKIP_MPIO_WAITANY -DDUMPI_SKIP_MPIO_TESTANY -DDUMPI_SKIP_MPIO_WAITSOME -DDUMPI_SKIP_MPIO_TESTSOME -DDUMPI_ON_REDSTORM -DDUMPI_NO_COMPRESSION $membuffer $COMMON"
cd build-rsq
touch dumpiconfig.h

if [ "X$build_libdumpi" = "Xyes" ]; then
for file in ../src/common/*.c ../src/libdumpi/*.c; do
  echo $CC $CFLAGS -I. -I../src -c $file
  $CC $CFLAGS -I. -I../src -c $file
done
$AR cru ../libdumpi-rsq.a *.o
echo "Built libdumpi-rsq.a.  You may need to link using -rt"
rm *.o
fi

if [ "X$build_mpitest" = "Xyes" ]; then
for file in ../src/test/*.c; do
  echo $CC $CFLAGS -I. -I../src -c $file
  $CC $CFLAGS -I. -I../src -c $file
done
echo $CC $CFLAGS -I. -I../src -o ../mpitest *.o ../libdumpi-rsq.a
$CC $CFLAGS -I. -I../src -o ../mpitest *.o ../libdumpi-rsq.a ../linkins/*.o  /opt/xt-service/default/lib/cnos64/librca.a
rm *.o
fi

if [ "X$build_libundumpi" = "Xyes" ]; then
CC=gcc
CFLAGS="$OPT -I/opt/xt-mpt/2.0.62/mpich2-64/P2/include $COMMON"
for file in ../src/common/*.c ../src/libundumpi/*.c; do
  echo $CC $CFLAGS -I. -I../src -c $file
  $CC $CFLAGS -I. -I../src -c $file
done
$AR cru ../libundumpi-rsq.a *.o
rm *.o
fi

if [ "X$build_dumpi2ascii" = "Xyes" ]; then
CC=gcc
CFLAGS="-I/opt/xt-mpt/2.0.62/mpich2-64/P2/include"
for file in ../src/bin/*.c; do
  echo $CC $CFLAGS -I. -I../src -c $file
  $CC $CFLAGS -I. -I../src -c $file
done
$CC $CFLAGS -I. -I../src -o ../dumpi2ascii dumpi2ascii*.o ../libundumpi-rsq.a -lrt
rm *.o
fi
