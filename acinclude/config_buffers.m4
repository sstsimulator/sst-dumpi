AC_DEFUN([CONFIG_BUFFER_SIZES], [

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
])

