![SST](http://sst-simulator.org/img/sst-logo-small.png)

# Structural Simulation Toolkit (SST) DUMPI Trace Library

#### Copyright (c) 2009-2017, Sandia National Laboratories
Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly 
owned subsidiary of Honeywell International, Inc., for the U.S. Department of 
Energy's National Nuclear Security Administration under contract DE-NA0003525.

---
The DUMPI library provides trace collection and trace replay tools for MPI applications.
The collection (dumpi) and replay (undumpi) tools can be enable in configure using:
````
--enable-libdumpi
--enable-libundumpi
````

To get started from a release, simply configure DUMPI using an MPI compiler.

To get started from a git download, run 
````
./bootstrap.sh
````
You can then configure using any MPI compiler.

Once installed, simply add 
````
-L$(PATH_TO_DUMPI) -ldumpi
````
to the LDFLAGS for your project. This will automatically embed DUMPI hooks into every MPI call.
Run your application as normal, and DUMPI will generate a file for each MPI rank containing the trace
in that folder. Trace collection is simple!

For trace replay, you will require the SST/macro tool. See the sst-macro repo and PDF manual for details on trace replay.

##### [LICENSE](https://github.com/sstsimulator/sst-dumpi/blob/devel/LICENSE)

[![License](https://img.shields.io/badge/License-BSD%203--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

Under the terms of Contract DE-NA0003525 with Sandia Corporation, 
the U.S. Government retains certain rights in this software.

