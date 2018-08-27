
AC_DEFUN([CHECK_OTF2], [

SAVE_CPPFLAGS="$CPPFLAGS" 
SAVE_LDFLAGS="$LDFLAGS"
SAVE_LIBS="$LIBS"

AC_ARG_ENABLE(otf2,
  [AS_HELP_STRING(
    [--(dis|en)able-otf2],
    [Enable otf2 supported trace replay],
    )],
  [ enable_otf2=$enableval ], 
  [ enable_otf2=no ]
)

#either $enableval is yes,no if yes, then system install of otf2
#if custom, folder specified then add to cppflags and ldflags 
if test "X$enable_otf2" = "Xyes"; then
  should_have_otf2=yes
  LIBS="-lotf2"
else
if test "X$enable_otf2" != "Xno"; then
  should_have_otf2=yes
  LDFLAGS="-L$enable_otf2/lib"
  LIBS="-lotf2"
  CPPFLAGS="-I$enable_otf2/include"
else
  should_have_otf2=no
fi
fi

#check header 
AC_CHECK_HEADER([otf2/otf2.h],
  [have_otf2=yes],
  [have_otf2=no]
)

# test for dumpi-to-otf2 converter
if test "$disable_bin" != "yes" -a "$have_otf2" = "yes"; then
   LIBS="-lotf2 -lz"
   AC_MSG_CHECKING([whether OTF2 library and header files can be found])
   AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [
	  #include <otf2/otf2.h>
	  #include <stdlib.h>
        ],[
	  return EXIT_SUCCESS;
        ]
      )],
      [
        AC_MSG_RESULT([yes])
	      have_otf2="yes"
      ],[
        AC_MSG_RESULT([no])
        have_otf2="no"
      ]
    )
fi

if test "X$should_have_otf2" = "Xyes" -a "X$have_otf2" != "Xyes"; then
  AC_MSG_ERROR([OTF2 libraries required by --enable-otf2 not found])
fi

if test "X$should_have_otf2" = "Xyes" -a "X$have_otf2" = "Xyes"; then
build_otf2=yes
AC_DEFINE([OTF2_ENABLED],,[Define OTF2 support as enabled])
else
build_otf2=no
fi

AM_CONDITIONAL([WITH_OTF2], [test "$build_otf2" = "yes"])

#check lib - try this later
OTF2_CPPFLAGS=$CPPFLAGS
OTF2_LDFLAGS=$LDFLAGS
OTF2_LIBS=$LIBS

AC_SUBST([OTF2_CPPFLAGS])
AC_SUBST([OTF2_LDFLAGS])
AC_SUBST([OTF2_LIBS])

CPPFLAGS="$SAVE_CPPFLAGS"
LDFLAGS="$SAVE_LDFLAGS" 
LIBS="$SAVE_LIBS"

])

