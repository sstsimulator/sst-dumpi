/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
 */

/* Undefine the non-unique preprocessor variables set by autotools.
 * Otherwise, external codes that use autoconf/automake/autoheader
 * can end up with garbage-definitions for these tokens.
 *
 * Unfortunately, autotools does not currently provide any way to 
 * specify unique names for these symbols */

#ifndef DUMPI_DUMPICONFIG_H
#define DUMPI_DUMPICONFIG_H

#undef LT_OBJDIR
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION
#undef VERSION

#include <dumpi/common/byteswap.h>

#include <dumpi/dumpiconfig-generated.h>

#ifdef DUMPI_ON_REDSTORM
#define htonl dumpi_swap32
#define htons dumpi_swap16
#define ntohl dumpi_swap32
#define ntohs dumpi_swap16
#endif

#undef LT_OBJDIR
#undef PACKAGE
#undef PACKAGE_BUGREPORT
#undef PACKAGE_NAME
#undef PACKAGE_STRING
#undef PACKAGE_TARNAME
#undef PACKAGE_URL
#undef PACKAGE_VERSION
#undef VERSION

#endif /* ! DUMPI_DUMPICONFIG_H */
