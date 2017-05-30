AC_DEFUN([CHECK_PAPI], [

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

])
