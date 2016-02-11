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

EXTRA_DIST = LICENSE

SUBDIRS = dumpi tests
ACLOCAL_AMFLAGS = -I acinclude

.PHONEY: doc
doc:
	cd docs && doxygen doxygen.cfg

#doc-install: doc
#	$(INSTALL) -d docs/sst-macroscale @docdir@

if WITH_LIBDUMPI
libdumpi:
	cd dumpi/common && make
	cd dumpi/libdumpi && make

libdumpi-install: libdumpi
	cd dumpi/libdumpi && make install
endif

if WITH_LIBUNDUMPI
libundumpi:
	cd dumpi/common && make
	cd dumpi/libundumpi && make

libundumpi-install: libundumpi
	cd dumpi/libundumpi && make install
endif

if WITH_BIN
dumpi2ascii:
	cd dumpi/libundumpi && make
	cd dumpi/bin && make dumpi2ascii
endif

if WITH_TEST
testmpi:
	cd dumpi/libdumpi && make
	cd dumpi/bin && make testmpi
endif

superclean: distclean
	rm -rf configure autom4te.cache aclocal.m4
	rm -f acinclude/ltsugar.m4 acinclude/libtool.m4 acinclude/ltversion.m4 acinclude/lt~obsolete.m4 acinclude/ltoptions.m4 
	rm -f bin/depcomp bin/missing bin/config.guess bin/config.sub bin/ltmain.sh bin/install-sh
	find . -name Makefile.in | xargs rm -f
	rm -f dumpi/dumpiconfig.h.in
