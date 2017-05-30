AC_DEFUN([CHECK_PTHREADS], [

AH_TEMPLATE([DUMPI_USE_PTHREADS],
	    [Put a unique index in thread-local storage.])

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

])

