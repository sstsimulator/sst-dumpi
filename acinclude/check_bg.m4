AC_DEFUN([CHECK_BG], [

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

])
