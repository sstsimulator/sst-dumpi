
dnl
dnl   This file is part of DUMPI: 
dnl                The MPI profiling library from the SST suite.
dnl   Copyright (c) 2009 Sandia Corporation.
dnl   This software is distributed under the BSD License.
dnl   Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
dnl   the U.S. Government retains certain rights in this software.
dnl   For more information, see the LICENSE file in the top 
dnl   SST/macroscale directory.
dnl

dnl Version info, used both in library versioning and inside dumpi.
m4_define([DUMPI_VERSION_TAG], 1)
m4_define([DUMPI_SUBVERSION_TAG], 1)
m4_define([DUMPI_SUBSUBVERSION_TAG], 0)
# Enable this for releases
dnl m4_define([DUMPI_SNAPSHOT_TAG])
# Enable this for development snapshots (should generally be enabled)
m4_define([DUMPI_SNAPSHOT_TAG], -snapshot)
m4_define([DUMPI_ACVERSION_TAG], 
          [DUMPI_VERSION_TAG.DUMPI_SUBVERSION_TAG.DUMPI_SUBSUBVERSION_TAG''DUMPI_SNAPSHOT_TAG])

dnl Libtool library versioning is
dnl a useful idea implemented in a confusing way.
dnl -version-info A:B:C means:
dnl     Major version A
dnl     subversion B (backward compatible for all A:X where X <= B)
dnl     and A is backward compatible to major version A-C
dnl which means that C <= A,
dnl and C == 0 means no major-version backward compatibility.
dnl First use of library versioning implemented in version 0.6.4,
dnl labelled as library version 1.0.0 
dnl (because library versioning has to start somewhere).
dnl Dumpi version 0.7.0 given libversion 2.0.0 because call order has
dnl been changed for many common IO routines (now consistently passing
dnl dumpi_profile as the first argument for IO calls).
dnl Dumpi version 1.0.0 given libversion 3.0.0 because of several new 
dnl API calls, bug fixes, and renamings.
m4_define([DUMPI_MAJOR_LIBVERS], 3)
m4_define([DUMPI_MINOR_LIBVERS], 0)
m4_define([DUMPI_BACKW_LIBVERS], 0)
m4_define([DUMPI_LIBVERSION_TAG],
          [DUMPI_MAJOR_LIBVERS:DUMPI_MINOR_LIBVERS:DUMPI_BACKW_LIBVERS])

AC_INIT([dumpi], [DUMPI_ACVERSION_TAG], [hadalst@sandia.gov])
AC_CONFIG_MACRO_DIR([acinclude])
AC_CONFIG_AUX_DIR(bin)
AC_CONFIG_HEADER(dumpi/dumpiconfig-generated.h)

AC_CANONICAL_SYSTEM
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

# AC_PROG_RANLIB
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LIBTOOL
AC_LANG_C
AC_PROG_LIBTOOL

# Disable shared libraries by default.
#AC_DISABLE_SHARED
#AM_DISABLE_SHARED
LT_INIT

# Don't reorder options.
# AC_PRESERVE_HELP_ORDER

# Version info.
AH_TEMPLATE([DUMPI_VERSION], [Major version number])
AH_TEMPLATE([DUMPI_SUBVERSION], [Major version number])
AH_TEMPLATE([DUMPI_SUBSUBVERSION], [Major version number])
AC_DEFINE_UNQUOTED(DUMPI_VERSION, [DUMPI_VERSION_TAG])
AC_DEFINE_UNQUOTED(DUMPI_SUBVERSION, [DUMPI_SUBVERSION_TAG])
AC_DEFINE_UNQUOTED(DUMPI_SUBSUBVERSION, [DUMPI_SUBSUBVERSION_TAG])
AC_SUBST(DUMPI_LIBVERSION, [DUMPI_LIBVERSION_TAG])

# Catamount (redstorm) -- this platform presents a number of challenges
# because of some of the things not provided (threads, ntohl, etc.).
AH_TEMPLATE([DUMPI_ON_REDSTORM], [Compile for a compute node on catamount])
AC_ARG_ENABLE(catamount,
  [  --enable-catamount      Compile for a compute node on catamount],
  [
    if test "$enableval" = "yes"; then
      cross_compiling=yes
      dumpi_on_catamount=yes
      AC_DEFINE(DUMPI_ON_REDSTORM, [1])
      LDFLAGS="$LDFLAGS -lrca"
    fi
  ]
)

AC_ARG_WITH([arch],
    [AS_HELP_STRING([--with-arch], [Give an architecture hint to help compilation. Currently supported are bgp and bgq.])],
    [
	arch_hint=$withval
    ],
    [
	arch_hint="none"
    ]
)

AH_TEMPLATE([DUMPI_ON_BGQ],
      [BGQ does odd stuff with grid position and timers])
if test "X$arch_hint" = "Xbgq"; then
AC_DEFINE(DUMPI_ON_BGQ, [1])
CPPFLAGS="$CPPFLAGS -I/bgsys/drivers/ppcfloor"
CPPFLAGS="$CPPFLAGS -I/bgsys/drivers/ppcfloor/firmware/include "
CPPFLAGS="$CPPFLAGS -I/bgsys/drivers/ppcfloor/spi/include/kernel"
CPPFLAGS="$CPPFLAGS -I/bgsys/drivers/ppcfloor/spi/include/kernel/cnk"
CPPFLAGS="$CPPFLAGS -I/bgsys/drivers/ppcfloor/hwi/include"
AC_MSG_RESULT([Adding extra options for BGQ system])
fi

AH_TEMPLATE([DUMPI_ON_BGP],
      [BGP does odd stuff with grid position and timers])
if test "X$arch_hint" = "Xbgp"; then
AC_DEFINE(DUMPI_ON_BGP, [1])
CPPFLAGS="$CPPFLAGS -I/bgsys/drivers/ppcfloor/arch/include"
AC_MSG_RESULT([Adding extra options for BGP system])
fi

# We allow the caller to suppress high-resoultion timers.
# The reason is that librt sometimes (GNU) brings in pthreads, and certain
# MPI implementations (e.g. MPICH-1) cannot work correctly with pthreads.
AH_TEMPLATE([DUMPI_DISABLE_POSIX_TIMERS], [Skip high resolution timers])
AC_MSG_CHECKING([whether POSIX timers should be used])
AC_ARG_ENABLE(posix-timers,
  [  --disable-posix-timers  Disable POSIX high-resolution timers)],
  [
    if test "$enableval" = "no"; then
      disable_posix_timers="yes",
      AC_DEFINE(DUMPI_DISABLE_POSIX_TIMERS, [1])
      AC_MSG_RESULT([POSIX timers turned off])
    fi
  ],[
    disable_posix_timers="no"
    # Test whether librt is found (hack hack).
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
         [#include <time.h>],
         [clock_getres((clockid_t)0, NULL);]
      )],
      [
        AC_MSG_RESULT([available without linking -lrt])
      ],[
        oldlibs="$LIBS"
        LIBS="$LIBS -lrt"
        AC_LINK_IFELSE(
          [AC_LANG_PROGRAM(
            [#include <time.h>],
            [clock_getres((clockid_t)0, NULL);]
          )],
          [
            AC_MSG_RESULT([available when linked with -lrt])
          ],[
            LIBS="$oldlibs"
            AC_DEFINE(DUMPI_DISABLE_POSIX_TIMERS, [1])
            AC_MSG_RESULT([POSIX timers not available])
          ]
        )
      ]
    )
  ]
)

# Options to constify MPI 2 interface for annoying compilers
AC_ARG_ENABLE(const-buffers,
  [  --enable-const-buffers         Don't build dumpi2ascii (trace file converter)],
  [ enable_const_buffers=$enableval ],
  enable_const_buffers=no
)
AH_TEMPLATE([DUMPI_CONST], [whether MPI 2 functions are const])
if test "X$enable_const_buffers" = "Xyes"; then
   AC_DEFINE(DUMPI_CONST, [const]) 
else
   AC_DEFINE(DUMPI_CONST, []) 
fi


# Options to turn off building different parts.
AC_ARG_ENABLE(bin,
  [  --disable-bin           Don't build dumpi2ascii (trace file converter)],
  [
    if test "$enableval" = "no"; then
      disable_bin="yes"
    fi
  ],
  disable_bin="" 
)

AC_ARG_ENABLE(libdumpi,
  [  --enable-libdumpi       Build libdumpi (the trace library)],
  [
    enable_libdumpi="$enableval"
  ],
  disable_libdumpi=""
)
AC_ARG_ENABLE(test,
  [  --enable-test           Build testmpi (coverage testing)],
  [
    enable_test="$enableval"
  ],
  enable_test="no"
)
AC_ARG_ENABLE(libundumpi,
  [  --disable-libundumpi    Don't build libundumpi (the trace file reader)],
  [
    if test "$enableval" = "no"; then
      disable_libundumpi="yes"
      disable_bin="yes"
    fi
  ],
  disable_libundumpi=""
)
AC_MSG_RESULT(["enable_test=$enable_test"])

# If we are building binary utlities, test wheter we can build the
# dumpi-to-otf converter.
if test "$disable_bin" != "yes"; then
   oldlibs="$LIBS"
   LIBS="$LIBS -lotf -lz"
   AC_MSG_CHECKING([wheter OTF library and header file can be found])
   AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [
	  #include <otf.h>
	  #include <stdlib.h>
        ],[
	  OTF_FileManager *mgr = NULL;
	  mgr = OTF_FileManager_open(10);
	  if(!mgr) {
	    return EXIT_FAILURE;
	  }
	  OTF_FileManager_close(mgr);
	  return EXIT_SUCCESS;
        ]
      )],
      [
        AC_MSG_RESULT([yes])
	enable_otf="yes"	
      ],[
        AC_MSG_RESULT([no])
	LIBS="$oldlibs"
      ]
    )
fi

AM_CONDITIONAL([WITH_BIN], [test "$disable_bin" != "yes"])
AM_CONDITIONAL([WITH_TEST], [test "$enable_test" = "yes"])
AM_CONDITIONAL([WITH_LIBDUMPI], [test "$enable_libdumpi" = "yes"])
AM_CONDITIONAL([WITH_LIBUNDUMPI], [test "$disable_libundumpi" != "yes"])
AM_CONDITIONAL([WITH_OTF], [test "$enable_otf" = "yes"])

AC_ARG_ENABLE(f77,
  [  --enable-f77[=path]     Build fortran77 bindings for libdumpi],
  [
    if test "$enableval" != "no"; then
      if test "$enableval" != "yes"; then
        F77="$enableval"
      fi
      enable_f77="yes"
    fi
  ],
  enable_f77="no"
)

AC_ARG_ENABLE(f90,
  [  --enable-f90[=path]     Build fortran90 bindings for libdumpi],
  [
    if test "$enableval" != "no"; then
      if test "$enableval" != "yes"; then
        FC="$enableval"
        F77="$enableval"
        #use for both f90 and f77
      fi
      enable_f90="yes"
      enable_f77="yes"
    fi
  ],
  enable_f90="no"
)

if test "$enable_libdumpi" = "yes"; then
  # We'd better have an MPI-capable C compiler.
  AC_MSG_CHECKING([whether $CC can compile and link an MPI program])
  AC_LANG_PUSH([C])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM(
      [#include <mpi.h>],
      [
        int aargc; char **aargv;
        MPI_Init(&aargc, &aargv);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Finalize();
      ]
    )],
    [ AC_MSG_RESULT([yes]) ],
    [
      AC_MSG_RESULT([no])
      AC_MSG_ERROR([Building libdumpi requires an MPI-capable C compiler.])
    ]
  )
  AC_LANG_POP([C])


  # Test whether we have an MPI-capable F90 compiler
  if test "$enable_f90" = "yes"; then
     AC_MSG_CHECKING([Fortran 90 compiler])
     AC_PROG_FC
     if test "$FC" == ""; then
        AC_MSG_ERROR([no F90 compiler found])
     else
        AC_MSG_RESULT([found $FC])
        F77="$FC"
     fi
     AC_FC_WRAPPERS
     AC_MSG_CHECKING([whether $FC can compile and link an MPI program])
     AC_LANG_PUSH([Fortran])
      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [],
          [
          use mpi
          implicit none
          integer ierr
          call mpi_init(ierr)
          call mpi_barrier(mpi_comm_world, ierr)
          call mpi_finalize(ierr)
          ]
        )],
        [
          dnl Well, at least it compiles and links
          AC_MSG_RESULT([yes])
          MPIF90="$FC"
        ],[
          AC_MSG_RESULT([no])
          AC_MSG_ERROR([Please ensure that the FC variable points to an MPI-enabled Fortran90 compiler])
        ]
      )
     AC_LANG_POP([Fortran])   
  fi

  if test "$enable_f77" = "yes"; then
     AC_MSG_CHECKING([Fortran77 compiler])
     AC_PROG_F77
     if test "$F77" == ""; then
        AC_MSG_ERROR([no F77 compiler found])
     else
        AC_MSG_RESULT([found $F77])
     fi
     AC_F77_WRAPPERS
     AC_MSG_CHECKING([whether $F77 can compile and link an MPI program])
     AC_LANG_PUSH([Fortran 77])
      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [],
          [
      implicit none
      include 'mpif.h'
      integer ierr
      call mpi_init(ierr)
      call mpi_barrier(mpi_comm_world, ierr)
      call mpi_finalize(ierr)
          ]
        )],
        [
          dnl Well, at least it compiles and links
          AC_MSG_RESULT([yes])
          MPIF77="$F77"
        ],[
          AC_MSG_RESULT([no.])
          AC_MSG_ERROR([Please ensure that the F77 variable points to an MPI-enabled Fortran77 compiler])
        ]
      )
     AC_LANG_POP([Fortran 77])   
  fi

fi # end of 'if test "$enable_libdumpi" = "yes"' block

AM_CONDITIONAL([WITH_MPIF77], [test "$MPIF77" != ""])
AM_CONDITIONAL([WITH_MPIF90], [test "$MPIF90" != ""])


AC_ARG_WITH(mpi-version,
  [  --with-mpi-version[=2,3,etc]   The MPI verstion to compile for],
  [
    mpiversion="$withval" 
  ],[
    mpiversion=2 
  ]
)

AH_TEMPLATE([IS_MPI_TWO],
            [Define to indicate which mpibindings to use])
if test "X$mpiversion" = "X2"; then
AC_DEFINE(IS_MPI_TWO, [1])
AM_CONDITIONAL([WITH_MPI_TWO], true)
AM_CONDITIONAL([WITH_MPI_THREE], false)
fi
if test "X$mpiversion" = "X3"; then
AM_CONDITIONAL([WITH_MPI_TWO], false)
AM_CONDITIONAL([WITH_MPI_THREE], true)
fi


# Add an option for writing to a memory buffer (instead of direct to file).
minimum_memcache=1048568   dnl One megabyte minus 8 bytes
default_memcache=134217720 dnl 128 megabytes minus 8 bytes
AH_TEMPLATE([DUMPI_MEMBUF_SIZE], [buffer size if DUMPI_WRITE_TO_MEMORY_BUFFER])
AC_ARG_WITH(trace-buffer-size,
  [  --with-trace-buffer[=SIZE]
                          Default size of a a (large) memory used to cache
                          output.  This cache allows profiling of calls made
			  outside MPI_Init* and MPI_Finalize.
                          The size of this buffer can  make a significant
                          performance difference on HPC systems that route
                          file writes over RPC or RDMA],
  [
   # the only allowed values are positive integers.
   if ! `echo $enableval | grep -qE '^([0-9]+)$'`; then
     AC_MSG_ERROR([--enable-trace-buffer:  Invalid buffer size $withval])
   elif test "$enableval" -ge "$minimum_memcache"; then
     AC_DEFINE_UNQUOTED(DUMPI_MEMBUF_SIZE, $enableval)
   elif test "$enableval" -lt "$minimum_memcache"; then
     AC_DEFINE_UNQUOTED(DUMPI_MEMBUF_SIZE, $minimum_memcache)
   else
     AC_DEFINE_UNQUOTED(DUMPI_MEMBUF_SIZE, $default_memcache)
   fi
  ],[
     AC_DEFINE_UNQUOTED(DUMPI_MEMBUF_SIZE, $default_memcache)
  ]
)

AH_TEMPLATE([DUMPI_USE_PTHREADS],
	    [Put a unique index in thread-local storage.])

if test "$enable_libdumpi" = "yes"; then
  DUMPI_FUNCTIONS
  DUMPI_TYPES
  DUMPI_OPS
  DUMPI_CONSTANTS
fi

AC_ARG_ENABLE(mpio,
  [  --disable-mpio   Disable support for MPIO],
  [
    if test "$enableval" = "no"; then
      DUMPI_DISABLE_MPIO
    fi
  ], [
    DUMPI_DISABLE_MPIO
  ]
)

AC_ARG_ENABLE(pthreads,
  [  --disable-pthreads   Disable support for multithreaded programs],
  [
    if test "$enableval" = "no"; then
      enable_pthreads=no
    else
      enable_pthreads=yes
    fi
  ], [
    enable_pthreads=yes
  ]
)

if test "$enable_pthreads" = "yes"; then
  if test "$enable_libdumpi" = "yes"; then 
    # Test whether the system supports pthread thread-local storage.
    AC_MSG_CHECKING([whether pthreads provide thread-local storage])
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [
          #include <stdlib.h>
          #include <pthread.h>
        ],[
          pthread_key_t the_key;
          void *the_value = NULL;
          pthread_key_create(&the_key, NULL);
          pthread_setspecific(the_key, the_value);
          return EXIT_SUCCESS;
        ]
      )
     ], [
       AC_MSG_RESULT([yes])
       AC_DEFINE(DUMPI_USE_PTHREADS)
     ],[
       AC_MSG_RESULT([no])
     ])
  fi
fi

# See if HOST_NAME_MAX is declared
AC_CHECK_DECL(HOST_NAME_MAX,
              [dumpi_hnm=HOST_NAME_MAX],
              [
	      dumpi_hnm=`getconf HOST_NAME_MAX`
              ],
              [#include <unistd.h>]
)
AC_DEFINE_UNQUOTED(DUMPI_HOST_NAME_MAX, $dumpi_hnm, [The maximum length of a host name string.])

# Check for non-POSIX timers
AC_CHECK_FUNCS([gettimeofday getrusage])

# Check whether we have PAPI installed.
AH_TEMPLATE([DUMPI_HAVE_PAPI], [PAPI support is provided.])
AC_MSG_CHECKING([papi support])
AC_ARG_ENABLE(papi,
  [  --enable-papi[=PATH]      Grab PAPI files relative to PATH.],
  [
    if test "$enableval" = "no"; then
      AC_MSG_RESULT([papi support disabled.])
    else
      AC_MSG_RESULT([papi support enabled.])
      AC_DEFINE(DUMPI_HAVE_PAPI)
      if test "$enableval" != "yes"; then
        CFLAGS="$CFLAGS -I$enableval/include"
	LDFLAGS="$LDFLAGS -L$enableval/lib"
      fi
      LDFLAGS="$LDFLAGS -lpapi"
   fi
  ], [
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [
	  #include <papi.h>
	  #include <stdlib.h>
        ],[
	  char *errstr = (char*)malloc(PAPI_MAX_STR_LEN);
	  int ret = PAPI_library_init(PAPI_VER_CURRENT);
	  if(ret != PAPI_VER_CURRENT) {
            PAPI_perror(ret, errstr, PAPI_MAX_STR_LEN);
	    return EXIT_FAILURE;
	  }
	  return EXIT_SUCCESS;
        ]
      )],
      [
        AC_MSG_RESULT([yes])
	LDFLAGS="$LDFLAGS -lpapi"
	AC_DEFINE(DUMPI_HAVE_PAPI)
      ],[
        AC_MSG_RESULT([no])
      ]
    )
  ]
)

# Instrumentation functions.
AH_TEMPLATE([DUMPI_ENABLE_INSTRUMENTATION], [Use instrumentation functions])
AH_TEMPLATE([DUMPI_SKIP_INSTRUMENTING], [Don't instrument this function])
AH_TEMPLATE([DUMPI_WEAK_SYMBOL], [Declare function as a weak symbol])
AH_TEMPLATE([DUMPI_DLADDR_WORKS], [We can use dladdr to get function names])

AC_DEFUN([AC_TEST_INSTRUMENTATION],
  [
    dnl First, test whether the basic pattern and attributes compile.
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [
	  #ifdef __cplusplus
	  extern "C" {
	  #endif /* ! __cplusplus */
	  void __attribute__((__no_instrument_function__))
	  __cyg_profile_func_enter(void *fn, void *site);	  
	  void __attribute__((__no_instrument__function__))
	  __cyg_profile_func_exit(void *fn, void *site);
	  #ifdef __cplusplus
	  } /* end of extern "C" block */
	  #endif /* ! __cplusplus */
	  void __cyg_profile_func_enter(void *fn, void *site) {
	    (void)fn;
	    (void)site;
	  }
	  void __cyg_profile_func_exit(void *fn, void *site) {
	    (void)fn;
	    (void)site;
	  }
	  int func() { return 0; }
        ],[
	  return func();
        ]
      )],
      [
        dnl Well, at least it compiles.
	instrumentation_works="yes"
	AC_DEFINE(DUMPI_ENABLE_INSTRUMENTATION)
	AC_DEFINE_UNQUOTED(DUMPI_SKIP_INSTRUMENTING,
	                   [__attribute__((__no_instrument_function__))])
      ],[
        instrumentation_works="no"
	AC_DEFINE_UNQUOTED(DUMPI_SKIP_INSTRUMENTING, [])
      ]
    )
    dnl Next, test whether we can safely request weak symbols
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [
	  int __attribute__((__weak__)) weakfun();
	  int weakfun() { return 0; }
	],[
	  if(weakfun) {
	    return weakfun();
	  }
	  else {
	    return 0;
	  }
	]
      )],
      [
        # That compiles.
	attribute_weak_works="yes"
	AC_DEFINE_UNQUOTED(DUMPI_WEAK_SYMBOL, [__attribute__((__weak__))])
      ],[
        # no compile.
	attribute_weak_works="no"
      ]
    )
    dnl Now, test whether we can use libdl to extract function names.
    if test "$instrumentation_works" = "yes"; then
      old_cflags="$CFLAGS"
      old_ldflags="$LDFLAGS"
      CFLAGS="$CFLAGS -rdynamic"
      LDFLAGS="$LDFLAGS -ldl"
      AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
	    #include <assert.h>
	    #define _GNU_SOURCE 1
	    #define __USE_GNU 1
	    #include <dlfcn.h>
	    #ifdef __cplusplus
	    extern "C" {
	    #endif /* ! __cplusplus */
	    void __attribute__((__no_instrument_function__))
	    __cyg_profile_func_enter(void *fn, void *site);
	    #ifdef __cplusplus
	    } /* end of extern "C" block */
	    #endif /* ! __cplusplus */
	    void __cyg_profile_func_enter(void *fn, void *site) {
	      Dl_info din;
	      assert(dladdr(fn, &din) != 0);
	    }
	    int func() { return 0; }
	  ],[
	    return func();
	  ]
        )],
        [
          # that compiles
	  AC_DEFINE([DUMPI_DLADDR_WORKS])
	  dladdr_works="yes"
        ],[
          # no compilo
	  dladdr_works="no"
	  CFLAGS="$old_cflags"
	  LDFLAGS="$old_ldflags"
        ]
      ),
    fi
  ]
)

AC_MSG_CHECKING([instrumentation settings])
AC_ARG_ENABLE(instrumenting, 
  [ -enable-instrumenting Provide instrumentation functions], 
  [ 
    if test "$enableval" = "yes"; then
      AC_MSG_RESULT([instrumentation enabled])
      AC_TEST_INSTRUMENTATION 
      if test "$instrumentation_works" = "yes"; then
        AC_MSG_RESULT([enabled]) 
      else 
        AC_MSG_ERROR([instrumentation requested but not working.])  
      fi 
    fi 
  ], [ 
    AC_MSG_RESULT([instrumentation not enabled])
    AC_DEFINE_UNQUOTED(DUMPI_SKIP_INSTRUMENTING, [])
  ]
)

# dot (from graphviz) is optional but can really help with doxygen documentation
AC_PATH_PROG([DOT], [dot])
if test -z "$DOT"; then
  HAVE_DOT=NO
else
  HAVE_DOT=YES
fi
AC_SUBST([HAVE_DOT])

AC_CONFIG_FILES([
 Makefile
 dumpi/Makefile
 dumpi/common/Makefile
 dumpi/libdumpi/Makefile
 dumpi/libundumpi/Makefile
 dumpi/bin/Makefile
 dumpi/test/Makefile
 tests/Makefile
 docs/doxygen.cfg
])
AC_OUTPUT

