#!/bin/sh

#
#  This file is part of DUMPI: 
#               The MPI profiling library from the SST suite.
#  Copyright (c) 2009-2022 NTESS.
#  This software is distributed under the BSD License.
#  Under the terms of Contract DE-NA0003525 with NTESS,
#  the U.S. Government retains certain rights in this software.
#  For more information, see the LICENSE file in the top 
#  SST/macroscale directory.
#

(glibtoolize || libtoolize) \
  && autoreconf --force --install

