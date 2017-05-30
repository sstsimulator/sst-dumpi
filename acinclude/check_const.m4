AC_DEFUN([CHECK_CONST_QUALIFIERS], [

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

])
