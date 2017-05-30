AC_DEFUN([CHECK_INSTRUMENTATION], [

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

])

