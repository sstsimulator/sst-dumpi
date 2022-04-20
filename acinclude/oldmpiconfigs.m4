AC_DEFUN([DUMPI_TEST_MPI_OLD],
  [
    dnl Test whether mpi.h is found
    AC_LANG_PUSH(C)
    AC_MSG_CHECKING([whether mpi.h is found])
    AC_LINK_IFELSE(
      [AC_LANG_PROGRAM(
        [
          #include <mpi.h>
          #include <stdlib.h>
        ], [
          int *ac; char ***av;
          MPI_Init(ac, av);
          MPI_Finalize();
          return EXIT_SUCCESS;
        ]
      )],
      AC_MSG_RESULT([yes]),
      AC_MSG_FAILURE([no])
    )

    dnl Is the MPI_Win type recognized?
    AH_TEMPLATE([DUMPI_HAVE_MPI_WIN], [if MPI_Win is a recognized type])
    AC_CACHE_CHECK([whether MPI_Win is a valid type], [dumpi_cv_have_mpi_win],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *argc;
            char ***argv;
            MPI_Win mpiwin;
            MPI_Init(argc, argv);
            PMPI_Init(argc, argv);
            MPI_Finalize();
          ])],
        [
          dumpi_cv_have_mpi_win="yes"
        ], [
          dumpi_cv_have_mpi_win="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win" = "yes"; then
      AC_DEFINE(DUMPI_HAVE_MPI_WIN, [1])
    fi
  
    AH_TEMPLATE([DUMPI_SKIP_MPI_PCONTROL], [if MPI_Pcontrol is missing])
    AC_CACHE_CHECK([whether MPI_Pcontrol is present], [dumpi_cv_have_mpi_pcontrol],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            const int level;
            //
            MPI_Init(aargc, aargv);
            MPI_Pcontrol(level);
            PMPI_Pcontrol(level);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_pcontrol="yes"
        ], [
          dumpi_cv_have_mpi_pcontrol="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_pcontrol" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_PCONTROL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SEND], [if MPI_Send is missing])
    AC_CACHE_CHECK([whether MPI_Send is present], [dumpi_cv_have_mpi_send],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Send(buf, count, datatype, dest, tag, comm);
            PMPI_Send(buf, count, datatype, dest, tag, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_send="yes"
        ], [
          dumpi_cv_have_mpi_send="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_send" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SEND, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_RECV], [if MPI_Recv is missing])
    AC_CACHE_CHECK([whether MPI_Recv is present], [dumpi_cv_have_mpi_recv],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int source;
            int tag;
            MPI_Comm comm;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Recv(buf, count, datatype, source, tag, comm, status);
            PMPI_Recv(buf, count, datatype, source, tag, comm, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_recv="yes"
        ], [
          dumpi_cv_have_mpi_recv="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_recv" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_RECV, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GET_COUNT], [if MPI_Get_count is missing])
    AC_CACHE_CHECK([whether MPI_Get_count is present], [dumpi_cv_have_mpi_get_count],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Status *status;
            MPI_Datatype datatype;
            int *count;
            //
            MPI_Init(aargc, aargv);
            MPI_Get_count(status, datatype, count);
            PMPI_Get_count(status, datatype, count);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_get_count="yes"
        ], [
          dumpi_cv_have_mpi_get_count="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_get_count" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GET_COUNT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_BSEND], [if MPI_Bsend is missing])
    AC_CACHE_CHECK([whether MPI_Bsend is present], [dumpi_cv_have_mpi_bsend],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Bsend(buf, count, datatype, dest, tag, comm);
            PMPI_Bsend(buf, count, datatype, dest, tag, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_bsend="yes"
        ], [
          dumpi_cv_have_mpi_bsend="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_bsend" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_BSEND, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SSEND], [if MPI_Ssend is missing])
    AC_CACHE_CHECK([whether MPI_Ssend is present], [dumpi_cv_have_mpi_ssend],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Ssend(buf, count, datatype, dest, tag, comm);
            PMPI_Ssend(buf, count, datatype, dest, tag, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_ssend="yes"
        ], [
          dumpi_cv_have_mpi_ssend="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_ssend" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SSEND, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_RSEND], [if MPI_Rsend is missing])
    AC_CACHE_CHECK([whether MPI_Rsend is present], [dumpi_cv_have_mpi_rsend],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Rsend(buf, count, datatype, dest, tag, comm);
            PMPI_Rsend(buf, count, datatype, dest, tag, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_rsend="yes"
        ], [
          dumpi_cv_have_mpi_rsend="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_rsend" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_RSEND, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_BUFFER_ATTACH], [if MPI_Buffer_attach is missing])
    AC_CACHE_CHECK([whether MPI_Buffer_attach is present], [dumpi_cv_have_mpi_buffer_attach],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int size;
            //
            MPI_Init(aargc, aargv);
            MPI_Buffer_attach(buf, size);
            PMPI_Buffer_attach(buf, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_buffer_attach="yes"
        ], [
          dumpi_cv_have_mpi_buffer_attach="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_buffer_attach" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_BUFFER_ATTACH, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_BUFFER_DETACH], [if MPI_Buffer_detach is missing])
    AC_CACHE_CHECK([whether MPI_Buffer_detach is present], [dumpi_cv_have_mpi_buffer_detach],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int *size;
            //
            MPI_Init(aargc, aargv);
            MPI_Buffer_detach(buf, size);
            PMPI_Buffer_detach(buf, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_buffer_detach="yes"
        ], [
          dumpi_cv_have_mpi_buffer_detach="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_buffer_detach" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_BUFFER_DETACH, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ISEND], [if MPI_Isend is missing])
    AC_CACHE_CHECK([whether MPI_Isend is present], [dumpi_cv_have_mpi_isend],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Isend(buf, count, datatype, dest, tag, comm, request);
            PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_isend="yes"
        ], [
          dumpi_cv_have_mpi_isend="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_isend" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ISEND, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_IBSEND], [if MPI_Ibsend is missing])
    AC_CACHE_CHECK([whether MPI_Ibsend is present], [dumpi_cv_have_mpi_ibsend],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
            PMPI_Ibsend(buf, count, datatype, dest, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_ibsend="yes"
        ], [
          dumpi_cv_have_mpi_ibsend="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_ibsend" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_IBSEND, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ISSEND], [if MPI_Issend is missing])
    AC_CACHE_CHECK([whether MPI_Issend is present], [dumpi_cv_have_mpi_issend],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Issend(buf, count, datatype, dest, tag, comm, request);
            PMPI_Issend(buf, count, datatype, dest, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_issend="yes"
        ], [
          dumpi_cv_have_mpi_issend="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_issend" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ISSEND, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_IRSEND], [if MPI_Irsend is missing])
    AC_CACHE_CHECK([whether MPI_Irsend is present], [dumpi_cv_have_mpi_irsend],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Irsend(buf, count, datatype, dest, tag, comm, request);
            PMPI_Irsend(buf, count, datatype, dest, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_irsend="yes"
        ], [
          dumpi_cv_have_mpi_irsend="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_irsend" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_IRSEND, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_IRECV], [if MPI_Irecv is missing])
    AC_CACHE_CHECK([whether MPI_Irecv is present], [dumpi_cv_have_mpi_irecv],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int source;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Irecv(buf, count, datatype, source, tag, comm, request);
            PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_irecv="yes"
        ], [
          dumpi_cv_have_mpi_irecv="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_irecv" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_IRECV, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WAIT], [if MPI_Wait is missing])
    AC_CACHE_CHECK([whether MPI_Wait is present], [dumpi_cv_have_mpi_wait],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Request *request;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Wait(request, status);
            PMPI_Wait(request, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_wait="yes"
        ], [
          dumpi_cv_have_mpi_wait="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_wait" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WAIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TEST], [if MPI_Test is missing])
    AC_CACHE_CHECK([whether MPI_Test is present], [dumpi_cv_have_mpi_test],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Request *request;
            int *flag;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Test(request, flag, status);
            PMPI_Test(request, flag, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_test="yes"
        ], [
          dumpi_cv_have_mpi_test="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_test" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TEST, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_REQUEST_FREE], [if MPI_Request_free is missing])
    AC_CACHE_CHECK([whether MPI_Request_free is present], [dumpi_cv_have_mpi_request_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Request_free(request);
            PMPI_Request_free(request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_request_free="yes"
        ], [
          dumpi_cv_have_mpi_request_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_request_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_REQUEST_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WAITANY], [if MPI_Waitany is missing])
    AC_CACHE_CHECK([whether MPI_Waitany is present], [dumpi_cv_have_mpi_waitany],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPI_Request *requests;
            int *index;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Waitany(count, requests, index, status);
            PMPI_Waitany(count, requests, index, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_waitany="yes"
        ], [
          dumpi_cv_have_mpi_waitany="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_waitany" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WAITANY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TESTANY], [if MPI_Testany is missing])
    AC_CACHE_CHECK([whether MPI_Testany is present], [dumpi_cv_have_mpi_testany],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPI_Request *requests;
            int *index;
            int *flag;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Testany(count, requests, index, flag, status);
            PMPI_Testany(count, requests, index, flag, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_testany="yes"
        ], [
          dumpi_cv_have_mpi_testany="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_testany" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TESTANY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WAITALL], [if MPI_Waitall is missing])
    AC_CACHE_CHECK([whether MPI_Waitall is present], [dumpi_cv_have_mpi_waitall],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPI_Request *requests;
            MPI_Status *statuses;
            //
            MPI_Init(aargc, aargv);
            MPI_Waitall(count, requests, statuses);
            PMPI_Waitall(count, requests, statuses);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_waitall="yes"
        ], [
          dumpi_cv_have_mpi_waitall="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_waitall" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WAITALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TESTALL], [if MPI_Testall is missing])
    AC_CACHE_CHECK([whether MPI_Testall is present], [dumpi_cv_have_mpi_testall],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPI_Request *requests;
            int *flag;
            MPI_Status *statuses;
            //
            MPI_Init(aargc, aargv);
            MPI_Testall(count, requests, flag, statuses);
            PMPI_Testall(count, requests, flag, statuses);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_testall="yes"
        ], [
          dumpi_cv_have_mpi_testall="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_testall" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TESTALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WAITSOME], [if MPI_Waitsome is missing])
    AC_CACHE_CHECK([whether MPI_Waitsome is present], [dumpi_cv_have_mpi_waitsome],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPI_Request *requests;
            int *outcount;
            int *indices;
            MPI_Status *statuses;
            //
            MPI_Init(aargc, aargv);
            MPI_Waitsome(count, requests, outcount, indices, statuses);
            PMPI_Waitsome(count, requests, outcount, indices, statuses);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_waitsome="yes"
        ], [
          dumpi_cv_have_mpi_waitsome="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_waitsome" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WAITSOME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TESTSOME], [if MPI_Testsome is missing])
    AC_CACHE_CHECK([whether MPI_Testsome is present], [dumpi_cv_have_mpi_testsome],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPI_Request *requests;
            int *outcount;
            int *indices;
            MPI_Status *statuses;
            //
            MPI_Init(aargc, aargv);
            MPI_Testsome(count, requests, outcount, indices, statuses);
            PMPI_Testsome(count, requests, outcount, indices, statuses);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_testsome="yes"
        ], [
          dumpi_cv_have_mpi_testsome="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_testsome" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TESTSOME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_IPROBE], [if MPI_Iprobe is missing])
    AC_CACHE_CHECK([whether MPI_Iprobe is present], [dumpi_cv_have_mpi_iprobe],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int source;
            int tag;
            MPI_Comm comm;
            int *flag;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Iprobe(source, tag, comm, flag, status);
            PMPI_Iprobe(source, tag, comm, flag, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_iprobe="yes"
        ], [
          dumpi_cv_have_mpi_iprobe="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_iprobe" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_IPROBE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_PROBE], [if MPI_Probe is missing])
    AC_CACHE_CHECK([whether MPI_Probe is present], [dumpi_cv_have_mpi_probe],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int source;
            int tag;
            MPI_Comm comm;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Probe(source, tag, comm, status);
            PMPI_Probe(source, tag, comm, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_probe="yes"
        ], [
          dumpi_cv_have_mpi_probe="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_probe" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_PROBE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CANCEL], [if MPI_Cancel is missing])
    AC_CACHE_CHECK([whether MPI_Cancel is present], [dumpi_cv_have_mpi_cancel],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Cancel(request);
            PMPI_Cancel(request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cancel="yes"
        ], [
          dumpi_cv_have_mpi_cancel="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cancel" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CANCEL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TEST_CANCELLED], [if MPI_Test_cancelled is missing])
    AC_CACHE_CHECK([whether MPI_Test_cancelled is present], [dumpi_cv_have_mpi_test_cancelled],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Status *status;
            int *cancelled;
            //
            MPI_Init(aargc, aargv);
            MPI_Test_cancelled(status, cancelled);
            PMPI_Test_cancelled(status, cancelled);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_test_cancelled="yes"
        ], [
          dumpi_cv_have_mpi_test_cancelled="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_test_cancelled" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TEST_CANCELLED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SEND_INIT], [if MPI_Send_init is missing])
    AC_CACHE_CHECK([whether MPI_Send_init is present], [dumpi_cv_have_mpi_send_init],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Send_init(buf, count, datatype, dest, tag, comm, request);
            PMPI_Send_init(buf, count, datatype, dest, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_send_init="yes"
        ], [
          dumpi_cv_have_mpi_send_init="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_send_init" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SEND_INIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_BSEND_INIT], [if MPI_Bsend_init is missing])
    AC_CACHE_CHECK([whether MPI_Bsend_init is present], [dumpi_cv_have_mpi_bsend_init],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);
            PMPI_Bsend_init(buf, count, datatype, dest, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_bsend_init="yes"
        ], [
          dumpi_cv_have_mpi_bsend_init="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_bsend_init" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_BSEND_INIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SSEND_INIT], [if MPI_Ssend_init is missing])
    AC_CACHE_CHECK([whether MPI_Ssend_init is present], [dumpi_cv_have_mpi_ssend_init],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Ssend_init(buf, count, datatype, dest, tag, comm, request);
            PMPI_Ssend_init(buf, count, datatype, dest, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_ssend_init="yes"
        ], [
          dumpi_cv_have_mpi_ssend_init="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_ssend_init" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SSEND_INIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_RSEND_INIT], [if MPI_Rsend_init is missing])
    AC_CACHE_CHECK([whether MPI_Rsend_init is present], [dumpi_cv_have_mpi_rsend_init],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Rsend_init(buf, count, datatype, dest, tag, comm, request);
            PMPI_Rsend_init(buf, count, datatype, dest, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_rsend_init="yes"
        ], [
          dumpi_cv_have_mpi_rsend_init="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_rsend_init" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_RSEND_INIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_RECV_INIT], [if MPI_Recv_init is missing])
    AC_CACHE_CHECK([whether MPI_Recv_init is present], [dumpi_cv_have_mpi_recv_init],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int source;
            int tag;
            MPI_Comm comm;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Recv_init(buf, count, datatype, source, tag, comm, request);
            PMPI_Recv_init(buf, count, datatype, source, tag, comm, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_recv_init="yes"
        ], [
          dumpi_cv_have_mpi_recv_init="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_recv_init" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_RECV_INIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_START], [if MPI_Start is missing])
    AC_CACHE_CHECK([whether MPI_Start is present], [dumpi_cv_have_mpi_start],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Start(request);
            PMPI_Start(request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_start="yes"
        ], [
          dumpi_cv_have_mpi_start="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_start" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_START, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_STARTALL], [if MPI_Startall is missing])
    AC_CACHE_CHECK([whether MPI_Startall is present], [dumpi_cv_have_mpi_startall],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPI_Request *requests;
            //
            MPI_Init(aargc, aargv);
            MPI_Startall(count, requests);
            PMPI_Startall(count, requests);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_startall="yes"
        ], [
          dumpi_cv_have_mpi_startall="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_startall" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_STARTALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SENDRECV], [if MPI_Sendrecv is missing])
    AC_CACHE_CHECK([whether MPI_Sendrecv is present], [dumpi_cv_have_mpi_sendrecv],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int sendcount;
            MPI_Datatype sendtype;
            int dest;
            int sendtag;
            void *recvbuf;
            int recvcount;
            MPI_Datatype recvtype;
            int source;
            int recvtag;
            MPI_Comm comm;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);
            PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_sendrecv="yes"
        ], [
          dumpi_cv_have_mpi_sendrecv="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_sendrecv" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SENDRECV, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SENDRECV_REPLACE], [if MPI_Sendrecv_replace is missing])
    AC_CACHE_CHECK([whether MPI_Sendrecv_replace is present], [dumpi_cv_have_mpi_sendrecv_replace],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int count;
            MPI_Datatype datatype;
            int dest;
            int sendtag;
            int source;
            int recvtag;
            MPI_Comm comm;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Sendrecv_replace(buf, count, datatype, dest, sendtag, source, recvtag, comm, status);
            PMPI_Sendrecv_replace(buf, count, datatype, dest, sendtag, source, recvtag, comm, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_sendrecv_replace="yes"
        ], [
          dumpi_cv_have_mpi_sendrecv_replace="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_sendrecv_replace" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SENDRECV_REPLACE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CONTIGUOUS], [if MPI_Type_contiguous is missing])
    AC_CACHE_CHECK([whether MPI_Type_contiguous is present], [dumpi_cv_have_mpi_type_contiguous],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_contiguous(count, oldtype, newtype);
            PMPI_Type_contiguous(count, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_contiguous="yes"
        ], [
          dumpi_cv_have_mpi_type_contiguous="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_contiguous" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CONTIGUOUS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_VECTOR], [if MPI_Type_vector is missing])
    AC_CACHE_CHECK([whether MPI_Type_vector is present], [dumpi_cv_have_mpi_type_vector],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int blocklength;
            int stride;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_vector(count, blocklength, stride, oldtype, newtype);
            PMPI_Type_vector(count, blocklength, stride, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_vector="yes"
        ], [
          dumpi_cv_have_mpi_type_vector="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_vector" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_VECTOR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_HVECTOR], [if MPI_Type_hvector is missing])
    AC_CACHE_CHECK([whether MPI_Type_hvector is present], [dumpi_cv_have_mpi_type_hvector],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int blocklength;
            MPI_Aint stride;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_hvector(count, blocklength, stride, oldtype, newtype);
            PMPI_Type_hvector(count, blocklength, stride, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_hvector="yes"
        ], [
          dumpi_cv_have_mpi_type_hvector="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_hvector" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_HVECTOR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_INDEXED], [if MPI_Type_indexed is missing])
    AC_CACHE_CHECK([whether MPI_Type_indexed is present], [dumpi_cv_have_mpi_type_indexed],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int *lengths;
            int *indices;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_indexed(count, lengths, indices, oldtype, newtype);
            PMPI_Type_indexed(count, lengths, indices, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_indexed="yes"
        ], [
          dumpi_cv_have_mpi_type_indexed="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_indexed" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_INDEXED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_HINDEXED], [if MPI_Type_hindexed is missing])
    AC_CACHE_CHECK([whether MPI_Type_hindexed is present], [dumpi_cv_have_mpi_type_hindexed],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int *lengths;
            MPI_Aint *indices;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_hindexed(count, lengths, indices, oldtype, newtype);
            PMPI_Type_hindexed(count, lengths, indices, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_hindexed="yes"
        ], [
          dumpi_cv_have_mpi_type_hindexed="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_hindexed" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_HINDEXED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_STRUCT], [if MPI_Type_struct is missing])
    AC_CACHE_CHECK([whether MPI_Type_struct is present], [dumpi_cv_have_mpi_type_struct],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int *lengths;
            MPI_Aint *indices;
            MPI_Datatype *oldtypes;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_struct(count, lengths, indices, oldtypes, newtype);
            PMPI_Type_struct(count, lengths, indices, oldtypes, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_struct="yes"
        ], [
          dumpi_cv_have_mpi_type_struct="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_struct" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_STRUCT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ADDRESS], [if MPI_Address is missing])
    AC_CACHE_CHECK([whether MPI_Address is present], [dumpi_cv_have_mpi_address],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            MPI_Aint *address;
            //
            MPI_Init(aargc, aargv);
            MPI_Address(buf, address);
            PMPI_Address(buf, address);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_address="yes"
        ], [
          dumpi_cv_have_mpi_address="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_address" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ADDRESS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_EXTENT], [if MPI_Type_extent is missing])
    AC_CACHE_CHECK([whether MPI_Type_extent is present], [dumpi_cv_have_mpi_type_extent],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            MPI_Aint *extent;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_extent(datatype, extent);
            PMPI_Type_extent(datatype, extent);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_extent="yes"
        ], [
          dumpi_cv_have_mpi_type_extent="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_extent" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_EXTENT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_SIZE], [if MPI_Type_size is missing])
    AC_CACHE_CHECK([whether MPI_Type_size is present], [dumpi_cv_have_mpi_type_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            int *size;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_size(datatype, size);
            PMPI_Type_size(datatype, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_size="yes"
        ], [
          dumpi_cv_have_mpi_type_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_LB], [if MPI_Type_lb is missing])
    AC_CACHE_CHECK([whether MPI_Type_lb is present], [dumpi_cv_have_mpi_type_lb],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            MPI_Aint *lb;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_lb(datatype, lb);
            PMPI_Type_lb(datatype, lb);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_lb="yes"
        ], [
          dumpi_cv_have_mpi_type_lb="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_lb" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_LB, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_UB], [if MPI_Type_ub is missing])
    AC_CACHE_CHECK([whether MPI_Type_ub is present], [dumpi_cv_have_mpi_type_ub],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            MPI_Aint *ub;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_ub(datatype, ub);
            PMPI_Type_ub(datatype, ub);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_ub="yes"
        ], [
          dumpi_cv_have_mpi_type_ub="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_ub" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_UB, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_COMMIT], [if MPI_Type_commit is missing])
    AC_CACHE_CHECK([whether MPI_Type_commit is present], [dumpi_cv_have_mpi_type_commit],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype *datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_commit(datatype);
            PMPI_Type_commit(datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_commit="yes"
        ], [
          dumpi_cv_have_mpi_type_commit="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_commit" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_COMMIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_FREE], [if MPI_Type_free is missing])
    AC_CACHE_CHECK([whether MPI_Type_free is present], [dumpi_cv_have_mpi_type_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype *datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_free(datatype);
            PMPI_Type_free(datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_free="yes"
        ], [
          dumpi_cv_have_mpi_type_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GET_ELEMENTS], [if MPI_Get_elements is missing])
    AC_CACHE_CHECK([whether MPI_Get_elements is present], [dumpi_cv_have_mpi_get_elements],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Status *status;
            MPI_Datatype datatype;
            int *elements;
            //
            MPI_Init(aargc, aargv);
            MPI_Get_elements(status, datatype, elements);
            PMPI_Get_elements(status, datatype, elements);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_get_elements="yes"
        ], [
          dumpi_cv_have_mpi_get_elements="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_get_elements" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GET_ELEMENTS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_PACK], [if MPI_Pack is missing])
    AC_CACHE_CHECK([whether MPI_Pack is present], [dumpi_cv_have_mpi_pack],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *inbuf;
            int incount;
            MPI_Datatype datatype;
            void *outbuf;
            int outcount;
            int *position;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Pack(inbuf, incount, datatype, outbuf, outcount, position, comm);
            PMPI_Pack(inbuf, incount, datatype, outbuf, outcount, position, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_pack="yes"
        ], [
          dumpi_cv_have_mpi_pack="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_pack" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_PACK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_UNPACK], [if MPI_Unpack is missing])
    AC_CACHE_CHECK([whether MPI_Unpack is present], [dumpi_cv_have_mpi_unpack],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *inbuf;
            int incount;
            int *position;
            void *outbuf;
            int outcount;
            MPI_Datatype datatype;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Unpack(inbuf, incount, position, outbuf, outcount, datatype, comm);
            PMPI_Unpack(inbuf, incount, position, outbuf, outcount, datatype, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_unpack="yes"
        ], [
          dumpi_cv_have_mpi_unpack="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unpack" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_UNPACK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_PACK_SIZE], [if MPI_Pack_size is missing])
    AC_CACHE_CHECK([whether MPI_Pack_size is present], [dumpi_cv_have_mpi_pack_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int incount;
            MPI_Datatype datatype;
            MPI_Comm comm;
            int *size;
            //
            MPI_Init(aargc, aargv);
            MPI_Pack_size(incount, datatype, comm, size);
            PMPI_Pack_size(incount, datatype, comm, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_pack_size="yes"
        ], [
          dumpi_cv_have_mpi_pack_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_pack_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_PACK_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_BARRIER], [if MPI_Barrier is missing])
    AC_CACHE_CHECK([whether MPI_Barrier is present], [dumpi_cv_have_mpi_barrier],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Barrier(comm);
            PMPI_Barrier(comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_barrier="yes"
        ], [
          dumpi_cv_have_mpi_barrier="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_barrier" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_BARRIER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_BCAST], [if MPI_Bcast is missing])
    AC_CACHE_CHECK([whether MPI_Bcast is present], [dumpi_cv_have_mpi_bcast],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int count;
            MPI_Datatype datatype;
            int root;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Bcast(sendbuf, count, datatype, root, comm);
            PMPI_Bcast(sendbuf, count, datatype, root, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_bcast="yes"
        ], [
          dumpi_cv_have_mpi_bcast="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_bcast" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_BCAST, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GATHER], [if MPI_Gather is missing])
    AC_CACHE_CHECK([whether MPI_Gather is present], [dumpi_cv_have_mpi_gather],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *inbuf;
            int sendcount;
            MPI_Datatype sendtype;
            void *recvbuf;
            int recvcount;
            MPI_Datatype recvtype;
            int root;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Gather(inbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
            PMPI_Gather(inbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_gather="yes"
        ], [
          dumpi_cv_have_mpi_gather="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_gather" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GATHER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GATHERV], [if MPI_Gatherv is missing])
    AC_CACHE_CHECK([whether MPI_Gatherv is present], [dumpi_cv_have_mpi_gatherv],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int sendcount;
            MPI_Datatype sendtype;
            void *recvbuf;
            int *recvcounts;
            int *displs;
            MPI_Datatype recvtype;
            int root;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm);
            PMPI_Gatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, root, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_gatherv="yes"
        ], [
          dumpi_cv_have_mpi_gatherv="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_gatherv" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GATHERV, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SCATTER], [if MPI_Scatter is missing])
    AC_CACHE_CHECK([whether MPI_Scatter is present], [dumpi_cv_have_mpi_scatter],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int sendcount;
            MPI_Datatype sendtype;
            void *recvbuf;
            int recvcount;
            MPI_Datatype recvtype;
            int root;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
            PMPI_Scatter(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, root, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_scatter="yes"
        ], [
          dumpi_cv_have_mpi_scatter="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_scatter" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SCATTER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SCATTERV], [if MPI_Scatterv is missing])
    AC_CACHE_CHECK([whether MPI_Scatterv is present], [dumpi_cv_have_mpi_scatterv],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int *sendcounts;
            int *displs;
            MPI_Datatype sendtype;
            void *recvbuf;
            int recvcount;
            MPI_Datatype recvtype;
            int root;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm);
            PMPI_Scatterv(sendbuf, sendcounts, displs, sendtype, recvbuf, recvcount, recvtype, root, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_scatterv="yes"
        ], [
          dumpi_cv_have_mpi_scatterv="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_scatterv" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SCATTERV, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ALLGATHER], [if MPI_Allgather is missing])
    AC_CACHE_CHECK([whether MPI_Allgather is present], [dumpi_cv_have_mpi_allgather],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int sendcount;
            MPI_Datatype sendtype;
            void *recvbuf;
            int recvcount;
            MPI_Datatype recvtype;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
            PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_allgather="yes"
        ], [
          dumpi_cv_have_mpi_allgather="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_allgather" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ALLGATHER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ALLGATHERV], [if MPI_Allgatherv is missing])
    AC_CACHE_CHECK([whether MPI_Allgatherv is present], [dumpi_cv_have_mpi_allgatherv],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int sendcount;
            MPI_Datatype sendtype;
            void *recvbuf;
            int *recvcounts;
            int *displs;
            MPI_Datatype recvtype;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
            PMPI_Allgatherv(sendbuf, sendcount, sendtype, recvbuf, recvcounts, displs, recvtype, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_allgatherv="yes"
        ], [
          dumpi_cv_have_mpi_allgatherv="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_allgatherv" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ALLGATHERV, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ALLTOALL], [if MPI_Alltoall is missing])
    AC_CACHE_CHECK([whether MPI_Alltoall is present], [dumpi_cv_have_mpi_alltoall],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int sendcount;
            MPI_Datatype sendtype;
            void *recvbuf;
            int recvcount;
            MPI_Datatype recvtype;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
            PMPI_Alltoall(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_alltoall="yes"
        ], [
          dumpi_cv_have_mpi_alltoall="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_alltoall" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ALLTOALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ALLTOALLV], [if MPI_Alltoallv is missing])
    AC_CACHE_CHECK([whether MPI_Alltoallv is present], [dumpi_cv_have_mpi_alltoallv],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int *sendcounts;
            int *senddispls;
            MPI_Datatype sendtype;
            void *recvbuf;
            int *recvcounts;
            int *recvdispls;
            MPI_Datatype recvtype;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Alltoallv(sendbuf, sendcounts, senddispls, sendtype, recvbuf, recvcounts, recvdispls, recvtype, comm);
            PMPI_Alltoallv(sendbuf, sendcounts, senddispls, sendtype, recvbuf, recvcounts, recvdispls, recvtype, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_alltoallv="yes"
        ], [
          dumpi_cv_have_mpi_alltoallv="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_alltoallv" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ALLTOALLV, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_REDUCE], [if MPI_Reduce is missing])
    AC_CACHE_CHECK([whether MPI_Reduce is present], [dumpi_cv_have_mpi_reduce],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            void *recvbuf;
            int count;
            MPI_Datatype datatype;
            MPI_Op op;
            int root;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
            PMPI_Reduce(sendbuf, recvbuf, count, datatype, op, root, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_reduce="yes"
        ], [
          dumpi_cv_have_mpi_reduce="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_reduce" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_REDUCE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_OP_CREATE], [if MPI_Op_create is missing])
    AC_CACHE_CHECK([whether MPI_Op_create is present], [dumpi_cv_have_mpi_op_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_User_function *fptr;
            int commute;
            MPI_Op *op;
            //
            MPI_Init(aargc, aargv);
            MPI_Op_create(fptr, commute, op);
            PMPI_Op_create(fptr, commute, op);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_op_create="yes"
        ], [
          dumpi_cv_have_mpi_op_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_op_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_OP_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_OP_FREE], [if MPI_Op_free is missing])
    AC_CACHE_CHECK([whether MPI_Op_free is present], [dumpi_cv_have_mpi_op_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Op *op;
            //
            MPI_Init(aargc, aargv);
            MPI_Op_free(op);
            PMPI_Op_free(op);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_op_free="yes"
        ], [
          dumpi_cv_have_mpi_op_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_op_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_OP_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ALLREDUCE], [if MPI_Allreduce is missing])
    AC_CACHE_CHECK([whether MPI_Allreduce is present], [dumpi_cv_have_mpi_allreduce],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            void *recvbuf;
            int count;
            MPI_Datatype datatype;
            MPI_Op op;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
            PMPI_Allreduce(sendbuf, recvbuf, count, datatype, op, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_allreduce="yes"
        ], [
          dumpi_cv_have_mpi_allreduce="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_allreduce" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ALLREDUCE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_REDUCE_SCATTER], [if MPI_Reduce_scatter is missing])
    AC_CACHE_CHECK([whether MPI_Reduce_scatter is present], [dumpi_cv_have_mpi_reduce_scatter],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            void *recvbuf;
            int *recvcounts;
            MPI_Datatype datatype;
            MPI_Op op;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);
            PMPI_Reduce_scatter(sendbuf, recvbuf, recvcounts, datatype, op, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_reduce_scatter="yes"
        ], [
          dumpi_cv_have_mpi_reduce_scatter="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_reduce_scatter" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_REDUCE_SCATTER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_SCAN], [if MPI_Scan is missing])
    AC_CACHE_CHECK([whether MPI_Scan is present], [dumpi_cv_have_mpi_scan],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            void *recvbuf;
            int count;
            MPI_Datatype datatype;
            MPI_Op op;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Scan(sendbuf, recvbuf, count, datatype, op, comm);
            PMPI_Scan(sendbuf, recvbuf, count, datatype, op, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_scan="yes"
        ], [
          dumpi_cv_have_mpi_scan="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_scan" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_SCAN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_SIZE], [if MPI_Group_size is missing])
    AC_CACHE_CHECK([whether MPI_Group_size is present], [dumpi_cv_have_mpi_group_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group;
            int *size;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_size(group, size);
            PMPI_Group_size(group, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_size="yes"
        ], [
          dumpi_cv_have_mpi_group_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_RANK], [if MPI_Group_rank is missing])
    AC_CACHE_CHECK([whether MPI_Group_rank is present], [dumpi_cv_have_mpi_group_rank],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group;
            int *rank;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_rank(group, rank);
            PMPI_Group_rank(group, rank);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_rank="yes"
        ], [
          dumpi_cv_have_mpi_group_rank="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_rank" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_RANK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_TRANSLATE_RANKS], [if MPI_Group_translate_ranks is missing])
    AC_CACHE_CHECK([whether MPI_Group_translate_ranks is present], [dumpi_cv_have_mpi_group_translate_ranks],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group1;
            int count;
            int *ranks1;
            MPI_Group group2;
            int *ranks2;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_translate_ranks(group1, count, ranks1, group2, ranks2);
            PMPI_Group_translate_ranks(group1, count, ranks1, group2, ranks2);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_translate_ranks="yes"
        ], [
          dumpi_cv_have_mpi_group_translate_ranks="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_translate_ranks" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_TRANSLATE_RANKS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_COMPARE], [if MPI_Group_compare is missing])
    AC_CACHE_CHECK([whether MPI_Group_compare is present], [dumpi_cv_have_mpi_group_compare],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group1;
            MPI_Group group2;
            int *result;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_compare(group1, group2, result);
            PMPI_Group_compare(group1, group2, result);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_compare="yes"
        ], [
          dumpi_cv_have_mpi_group_compare="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_compare" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_COMPARE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_GROUP], [if MPI_Comm_group is missing])
    AC_CACHE_CHECK([whether MPI_Comm_group is present], [dumpi_cv_have_mpi_comm_group],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            MPI_Group *group;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_group(comm, group);
            PMPI_Comm_group(comm, group);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_group="yes"
        ], [
          dumpi_cv_have_mpi_comm_group="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_group" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_GROUP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_UNION], [if MPI_Group_union is missing])
    AC_CACHE_CHECK([whether MPI_Group_union is present], [dumpi_cv_have_mpi_group_union],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group1;
            MPI_Group group2;
            MPI_Group *newgroup;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_union(group1, group2, newgroup);
            PMPI_Group_union(group1, group2, newgroup);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_union="yes"
        ], [
          dumpi_cv_have_mpi_group_union="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_union" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_UNION, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_INTERSECTION], [if MPI_Group_intersection is missing])
    AC_CACHE_CHECK([whether MPI_Group_intersection is present], [dumpi_cv_have_mpi_group_intersection],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group1;
            MPI_Group group2;
            MPI_Group *newgroup;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_intersection(group1, group2, newgroup);
            PMPI_Group_intersection(group1, group2, newgroup);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_intersection="yes"
        ], [
          dumpi_cv_have_mpi_group_intersection="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_intersection" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_INTERSECTION, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_DIFFERENCE], [if MPI_Group_difference is missing])
    AC_CACHE_CHECK([whether MPI_Group_difference is present], [dumpi_cv_have_mpi_group_difference],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group1;
            MPI_Group group2;
            MPI_Group *newgroup;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_difference(group1, group2, newgroup);
            PMPI_Group_difference(group1, group2, newgroup);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_difference="yes"
        ], [
          dumpi_cv_have_mpi_group_difference="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_difference" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_DIFFERENCE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_INCL], [if MPI_Group_incl is missing])
    AC_CACHE_CHECK([whether MPI_Group_incl is present], [dumpi_cv_have_mpi_group_incl],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group;
            int count;
            int *ranks;
            MPI_Group *newgroup;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_incl(group, count, ranks, newgroup);
            PMPI_Group_incl(group, count, ranks, newgroup);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_incl="yes"
        ], [
          dumpi_cv_have_mpi_group_incl="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_incl" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_INCL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_EXCL], [if MPI_Group_excl is missing])
    AC_CACHE_CHECK([whether MPI_Group_excl is present], [dumpi_cv_have_mpi_group_excl],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group;
            int count;
            int *ranks;
            MPI_Group *newgroup;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_excl(group, count, ranks, newgroup);
            PMPI_Group_excl(group, count, ranks, newgroup);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_excl="yes"
        ], [
          dumpi_cv_have_mpi_group_excl="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_excl" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_EXCL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_RANGE_INCL], [if MPI_Group_range_incl is missing])
    AC_CACHE_CHECK([whether MPI_Group_range_incl is present], [dumpi_cv_have_mpi_group_range_incl],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group;
            int count;
            int ranges[][3];
            MPI_Group *newgroup;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_range_incl(group, count, ranges3, newgroup);
            PMPI_Group_range_incl(group, count, ranges3, newgroup);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_range_incl="yes"
        ], [
          dumpi_cv_have_mpi_group_range_incl="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_range_incl" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_RANGE_INCL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_RANGE_EXCL], [if MPI_Group_range_excl is missing])
    AC_CACHE_CHECK([whether MPI_Group_range_excl is present], [dumpi_cv_have_mpi_group_range_excl],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group;
            int count;
            int ranges[][3];
            MPI_Group *newgroup;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_range_excl(group, count, ranges3, newgroup);
            PMPI_Group_range_excl(group, count, ranges3, newgroup);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_range_excl="yes"
        ], [
          dumpi_cv_have_mpi_group_range_excl="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_range_excl" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_RANGE_EXCL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GROUP_FREE], [if MPI_Group_free is missing])
    AC_CACHE_CHECK([whether MPI_Group_free is present], [dumpi_cv_have_mpi_group_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group *group;
            //
            MPI_Init(aargc, aargv);
            MPI_Group_free(group);
            PMPI_Group_free(group);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_group_free="yes"
        ], [
          dumpi_cv_have_mpi_group_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_group_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GROUP_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_SIZE], [if MPI_Comm_size is missing])
    AC_CACHE_CHECK([whether MPI_Comm_size is present], [dumpi_cv_have_mpi_comm_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int *size;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_size(comm, size);
            PMPI_Comm_size(comm, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_size="yes"
        ], [
          dumpi_cv_have_mpi_comm_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_RANK], [if MPI_Comm_rank is missing])
    AC_CACHE_CHECK([whether MPI_Comm_rank is present], [dumpi_cv_have_mpi_comm_rank],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int *rank;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_rank(comm, rank);
            PMPI_Comm_rank(comm, rank);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_rank="yes"
        ], [
          dumpi_cv_have_mpi_comm_rank="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_rank" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_RANK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_COMPARE], [if MPI_Comm_compare is missing])
    AC_CACHE_CHECK([whether MPI_Comm_compare is present], [dumpi_cv_have_mpi_comm_compare],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm1;
            MPI_Comm comm2;
            int *result;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_compare(comm1, comm2, result);
            PMPI_Comm_compare(comm1, comm2, result);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_compare="yes"
        ], [
          dumpi_cv_have_mpi_comm_compare="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_compare" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_COMPARE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_DUP], [if MPI_Comm_dup is missing])
    AC_CACHE_CHECK([whether MPI_Comm_dup is present], [dumpi_cv_have_mpi_comm_dup],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm oldcomm;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_dup(oldcomm, newcomm);
            PMPI_Comm_dup(oldcomm, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_dup="yes"
        ], [
          dumpi_cv_have_mpi_comm_dup="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_dup" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_DUP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_CREATE], [if MPI_Comm_create is missing])
    AC_CACHE_CHECK([whether MPI_Comm_create is present], [dumpi_cv_have_mpi_comm_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm oldcomm;
            MPI_Group group;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_create(oldcomm, group, newcomm);
            PMPI_Comm_create(oldcomm, group, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_create="yes"
        ], [
          dumpi_cv_have_mpi_comm_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_SPLIT], [if MPI_Comm_split is missing])
    AC_CACHE_CHECK([whether MPI_Comm_split is present], [dumpi_cv_have_mpi_comm_split],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm oldcomm;
            int color;
            int key;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_split(oldcomm, color, key, newcomm);
            PMPI_Comm_split(oldcomm, color, key, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_split="yes"
        ], [
          dumpi_cv_have_mpi_comm_split="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_split" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_SPLIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_FREE], [if MPI_Comm_free is missing])
    AC_CACHE_CHECK([whether MPI_Comm_free is present], [dumpi_cv_have_mpi_comm_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm *comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_free(comm);
            PMPI_Comm_free(comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_free="yes"
        ], [
          dumpi_cv_have_mpi_comm_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_TEST_INTER], [if MPI_Comm_test_inter is missing])
    AC_CACHE_CHECK([whether MPI_Comm_test_inter is present], [dumpi_cv_have_mpi_comm_test_inter],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int *inter;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_test_inter(comm, inter);
            PMPI_Comm_test_inter(comm, inter);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_test_inter="yes"
        ], [
          dumpi_cv_have_mpi_comm_test_inter="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_test_inter" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_TEST_INTER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_REMOTE_SIZE], [if MPI_Comm_remote_size is missing])
    AC_CACHE_CHECK([whether MPI_Comm_remote_size is present], [dumpi_cv_have_mpi_comm_remote_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int *size;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_remote_size(comm, size);
            PMPI_Comm_remote_size(comm, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_remote_size="yes"
        ], [
          dumpi_cv_have_mpi_comm_remote_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_remote_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_REMOTE_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_REMOTE_GROUP], [if MPI_Comm_remote_group is missing])
    AC_CACHE_CHECK([whether MPI_Comm_remote_group is present], [dumpi_cv_have_mpi_comm_remote_group],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            MPI_Group *group;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_remote_group(comm, group);
            PMPI_Comm_remote_group(comm, group);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_remote_group="yes"
        ], [
          dumpi_cv_have_mpi_comm_remote_group="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_remote_group" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_REMOTE_GROUP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INTERCOMM_CREATE], [if MPI_Intercomm_create is missing])
    AC_CACHE_CHECK([whether MPI_Intercomm_create is present], [dumpi_cv_have_mpi_intercomm_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm localcomm;
            int localleader;
            MPI_Comm remotecomm;
            int remoteleader;
            int tag;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Intercomm_create(localcomm, localleader, remotecomm, remoteleader, tag, newcomm);
            PMPI_Intercomm_create(localcomm, localleader, remotecomm, remoteleader, tag, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_intercomm_create="yes"
        ], [
          dumpi_cv_have_mpi_intercomm_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_intercomm_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INTERCOMM_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INTERCOMM_MERGE], [if MPI_Intercomm_merge is missing])
    AC_CACHE_CHECK([whether MPI_Intercomm_merge is present], [dumpi_cv_have_mpi_intercomm_merge],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int high;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Intercomm_merge(comm, high, newcomm);
            PMPI_Intercomm_merge(comm, high, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_intercomm_merge="yes"
        ], [
          dumpi_cv_have_mpi_intercomm_merge="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_intercomm_merge" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INTERCOMM_MERGE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_KEYVAL_CREATE], [if MPI_Keyval_create is missing])
    AC_CACHE_CHECK([whether MPI_Keyval_create is present], [dumpi_cv_have_mpi_keyval_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Copy_function *copyfunc;
            MPI_Delete_function *delfunc;
            int *key;
            void *state;
            //
            MPI_Init(aargc, aargv);
            MPI_Keyval_create(copyfunc, delfunc, key, state);
            PMPI_Keyval_create(copyfunc, delfunc, key, state);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_keyval_create="yes"
        ], [
          dumpi_cv_have_mpi_keyval_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_keyval_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_KEYVAL_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_KEYVAL_FREE], [if MPI_Keyval_free is missing])
    AC_CACHE_CHECK([whether MPI_Keyval_free is present], [dumpi_cv_have_mpi_keyval_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *key;
            //
            MPI_Init(aargc, aargv);
            MPI_Keyval_free(key);
            PMPI_Keyval_free(key);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_keyval_free="yes"
        ], [
          dumpi_cv_have_mpi_keyval_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_keyval_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_KEYVAL_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ATTR_PUT], [if MPI_Attr_put is missing])
    AC_CACHE_CHECK([whether MPI_Attr_put is present], [dumpi_cv_have_mpi_attr_put],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int key;
            void *attr;
            //
            MPI_Init(aargc, aargv);
            MPI_Attr_put(comm, key, attr);
            PMPI_Attr_put(comm, key, attr);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_attr_put="yes"
        ], [
          dumpi_cv_have_mpi_attr_put="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_attr_put" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ATTR_PUT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ATTR_GET], [if MPI_Attr_get is missing])
    AC_CACHE_CHECK([whether MPI_Attr_get is present], [dumpi_cv_have_mpi_attr_get],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int key;
            void *attr;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Attr_get(comm, key, attr, flag);
            PMPI_Attr_get(comm, key, attr, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_attr_get="yes"
        ], [
          dumpi_cv_have_mpi_attr_get="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_attr_get" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ATTR_GET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ATTR_DELETE], [if MPI_Attr_delete is missing])
    AC_CACHE_CHECK([whether MPI_Attr_delete is present], [dumpi_cv_have_mpi_attr_delete],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int key;
            //
            MPI_Init(aargc, aargv);
            MPI_Attr_delete(comm, key);
            PMPI_Attr_delete(comm, key);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_attr_delete="yes"
        ], [
          dumpi_cv_have_mpi_attr_delete="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_attr_delete" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ATTR_DELETE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TOPO_TEST], [if MPI_Topo_test is missing])
    AC_CACHE_CHECK([whether MPI_Topo_test is present], [dumpi_cv_have_mpi_topo_test],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int *topo;
            //
            MPI_Init(aargc, aargv);
            MPI_Topo_test(comm, topo);
            PMPI_Topo_test(comm, topo);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_topo_test="yes"
        ], [
          dumpi_cv_have_mpi_topo_test="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_topo_test" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TOPO_TEST, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CART_CREATE], [if MPI_Cart_create is missing])
    AC_CACHE_CHECK([whether MPI_Cart_create is present], [dumpi_cv_have_mpi_cart_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm oldcomm;
            int ndim;
            int *dims;
            int *periods;
            int reorder;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Cart_create(oldcomm, ndim, dims, periods, reorder, newcomm);
            PMPI_Cart_create(oldcomm, ndim, dims, periods, reorder, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cart_create="yes"
        ], [
          dumpi_cv_have_mpi_cart_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cart_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CART_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_DIMS_CREATE], [if MPI_Dims_create is missing])
    AC_CACHE_CHECK([whether MPI_Dims_create is present], [dumpi_cv_have_mpi_dims_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int nodes;
            int ndim;
            int *dims;
            //
            MPI_Init(aargc, aargv);
            MPI_Dims_create(nodes, ndim, dims);
            PMPI_Dims_create(nodes, ndim, dims);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_dims_create="yes"
        ], [
          dumpi_cv_have_mpi_dims_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_dims_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_DIMS_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GRAPH_CREATE], [if MPI_Graph_create is missing])
    AC_CACHE_CHECK([whether MPI_Graph_create is present], [dumpi_cv_have_mpi_graph_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm oldcomm;
            int nodes;
            int *index;
            int *edges;
            int reorder;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Graph_create(oldcomm, nodes, index, edges, reorder, newcomm);
            PMPI_Graph_create(oldcomm, nodes, index, edges, reorder, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_graph_create="yes"
        ], [
          dumpi_cv_have_mpi_graph_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_graph_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GRAPH_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GRAPHDIMS_GET], [if MPI_Graphdims_get is missing])
    AC_CACHE_CHECK([whether MPI_Graphdims_get is present], [dumpi_cv_have_mpi_graphdims_get],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int *nodes;
            int *edges;
            //
            MPI_Init(aargc, aargv);
            MPI_Graphdims_get(comm, nodes, edges);
            PMPI_Graphdims_get(comm, nodes, edges);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_graphdims_get="yes"
        ], [
          dumpi_cv_have_mpi_graphdims_get="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_graphdims_get" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GRAPHDIMS_GET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GRAPH_GET], [if MPI_Graph_get is missing])
    AC_CACHE_CHECK([whether MPI_Graph_get is present], [dumpi_cv_have_mpi_graph_get],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int maxindex;
            int maxedges;
            int *index;
            int *edges;
            //
            MPI_Init(aargc, aargv);
            MPI_Graph_get(comm, maxindex, maxedges, index, edges);
            PMPI_Graph_get(comm, maxindex, maxedges, index, edges);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_graph_get="yes"
        ], [
          dumpi_cv_have_mpi_graph_get="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_graph_get" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GRAPH_GET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CARTDIM_GET], [if MPI_Cartdim_get is missing])
    AC_CACHE_CHECK([whether MPI_Cartdim_get is present], [dumpi_cv_have_mpi_cartdim_get],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int *ndim;
            //
            MPI_Init(aargc, aargv);
            MPI_Cartdim_get(comm, ndim);
            PMPI_Cartdim_get(comm, ndim);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cartdim_get="yes"
        ], [
          dumpi_cv_have_mpi_cartdim_get="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cartdim_get" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CARTDIM_GET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CART_GET], [if MPI_Cart_get is missing])
    AC_CACHE_CHECK([whether MPI_Cart_get is present], [dumpi_cv_have_mpi_cart_get],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int maxdims;
            int *dims;
            int *periods;
            int *coords;
            //
            MPI_Init(aargc, aargv);
            MPI_Cart_get(comm, maxdims, dims, periods, coords);
            PMPI_Cart_get(comm, maxdims, dims, periods, coords);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cart_get="yes"
        ], [
          dumpi_cv_have_mpi_cart_get="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cart_get" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CART_GET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CART_RANK], [if MPI_Cart_rank is missing])
    AC_CACHE_CHECK([whether MPI_Cart_rank is present], [dumpi_cv_have_mpi_cart_rank],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int *coords;
            int *rank;
            //
            MPI_Init(aargc, aargv);
            MPI_Cart_rank(comm, coords, rank);
            PMPI_Cart_rank(comm, coords, rank);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cart_rank="yes"
        ], [
          dumpi_cv_have_mpi_cart_rank="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cart_rank" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CART_RANK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CART_COORDS], [if MPI_Cart_coords is missing])
    AC_CACHE_CHECK([whether MPI_Cart_coords is present], [dumpi_cv_have_mpi_cart_coords],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int rank;
            int maxdims;
            int *coords;
            //
            MPI_Init(aargc, aargv);
            MPI_Cart_coords(comm, rank, maxdims, coords);
            PMPI_Cart_coords(comm, rank, maxdims, coords);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cart_coords="yes"
        ], [
          dumpi_cv_have_mpi_cart_coords="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cart_coords" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CART_COORDS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GRAPH_NEIGHBORS_COUNT], [if MPI_Graph_neighbors_count is missing])
    AC_CACHE_CHECK([whether MPI_Graph_neighbors_count is present], [dumpi_cv_have_mpi_graph_neighbors_count],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int rank;
            int *nneigh;
            //
            MPI_Init(aargc, aargv);
            MPI_Graph_neighbors_count(comm, rank, nneigh);
            PMPI_Graph_neighbors_count(comm, rank, nneigh);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_graph_neighbors_count="yes"
        ], [
          dumpi_cv_have_mpi_graph_neighbors_count="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_graph_neighbors_count" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GRAPH_NEIGHBORS_COUNT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GRAPH_NEIGHBORS], [if MPI_Graph_neighbors is missing])
    AC_CACHE_CHECK([whether MPI_Graph_neighbors is present], [dumpi_cv_have_mpi_graph_neighbors],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int rank;
            int maxneighbors;
            int *neighbors;
            //
            MPI_Init(aargc, aargv);
            MPI_Graph_neighbors(comm, rank, maxneighbors, neighbors);
            PMPI_Graph_neighbors(comm, rank, maxneighbors, neighbors);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_graph_neighbors="yes"
        ], [
          dumpi_cv_have_mpi_graph_neighbors="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_graph_neighbors" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GRAPH_NEIGHBORS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CART_SHIFT], [if MPI_Cart_shift is missing])
    AC_CACHE_CHECK([whether MPI_Cart_shift is present], [dumpi_cv_have_mpi_cart_shift],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int direction;
            int displ;
            int *source;
            int *dest;
            //
            MPI_Init(aargc, aargv);
            MPI_Cart_shift(comm, direction, displ, source, dest);
            PMPI_Cart_shift(comm, direction, displ, source, dest);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cart_shift="yes"
        ], [
          dumpi_cv_have_mpi_cart_shift="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cart_shift" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CART_SHIFT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CART_SUB], [if MPI_Cart_sub is missing])
    AC_CACHE_CHECK([whether MPI_Cart_sub is present], [dumpi_cv_have_mpi_cart_sub],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm oldcomm;
            int *remain_dims;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Cart_sub(oldcomm, remain_dims, newcomm);
            PMPI_Cart_sub(oldcomm, remain_dims, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cart_sub="yes"
        ], [
          dumpi_cv_have_mpi_cart_sub="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cart_sub" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CART_SUB, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CART_MAP], [if MPI_Cart_map is missing])
    AC_CACHE_CHECK([whether MPI_Cart_map is present], [dumpi_cv_have_mpi_cart_map],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int ndim;
            int *dims;
            int *period;
            int *newrank;
            //
            MPI_Init(aargc, aargv);
            MPI_Cart_map(comm, ndim, dims, period, newrank);
            PMPI_Cart_map(comm, ndim, dims, period, newrank);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_cart_map="yes"
        ], [
          dumpi_cv_have_mpi_cart_map="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_cart_map" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CART_MAP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GRAPH_MAP], [if MPI_Graph_map is missing])
    AC_CACHE_CHECK([whether MPI_Graph_map is present], [dumpi_cv_have_mpi_graph_map],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int nodes;
            int *index;
            int *edges;
            int *newrank;
            //
            MPI_Init(aargc, aargv);
            MPI_Graph_map(comm, nodes, index, edges, newrank);
            PMPI_Graph_map(comm, nodes, index, edges, newrank);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_graph_map="yes"
        ], [
          dumpi_cv_have_mpi_graph_map="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_graph_map" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GRAPH_MAP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GET_PROCESSOR_NAME], [if MPI_Get_processor_name is missing])
    AC_CACHE_CHECK([whether MPI_Get_processor_name is present], [dumpi_cv_have_mpi_get_processor_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *name;
            int *resultlen;
            //
            MPI_Init(aargc, aargv);
            MPI_Get_processor_name(name, resultlen);
            PMPI_Get_processor_name(name, resultlen);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_get_processor_name="yes"
        ], [
          dumpi_cv_have_mpi_get_processor_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_get_processor_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GET_PROCESSOR_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GET_VERSION], [if MPI_Get_version is missing])
    AC_CACHE_CHECK([whether MPI_Get_version is present], [dumpi_cv_have_mpi_get_version],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *version;
            int *subversion;
            //
            MPI_Init(aargc, aargv);
            MPI_Get_version(version, subversion);
            PMPI_Get_version(version, subversion);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_get_version="yes"
        ], [
          dumpi_cv_have_mpi_get_version="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_get_version" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GET_VERSION, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ERRHANDLER_CREATE], [if MPI_Errhandler_create is missing])
    AC_CACHE_CHECK([whether MPI_Errhandler_create is present], [dumpi_cv_have_mpi_errhandler_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Handler_function *function;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Errhandler_create(function, errhandler);
            PMPI_Errhandler_create(function, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_errhandler_create="yes"
        ], [
          dumpi_cv_have_mpi_errhandler_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_errhandler_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ERRHANDLER_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ERRHANDLER_SET], [if MPI_Errhandler_set is missing])
    AC_CACHE_CHECK([whether MPI_Errhandler_set is present], [dumpi_cv_have_mpi_errhandler_set],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            MPI_Errhandler errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Errhandler_set(comm, errhandler);
            PMPI_Errhandler_set(comm, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_errhandler_set="yes"
        ], [
          dumpi_cv_have_mpi_errhandler_set="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_errhandler_set" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ERRHANDLER_SET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ERRHANDLER_GET], [if MPI_Errhandler_get is missing])
    AC_CACHE_CHECK([whether MPI_Errhandler_get is present], [dumpi_cv_have_mpi_errhandler_get],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Errhandler_get(comm, errhandler);
            PMPI_Errhandler_get(comm, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_errhandler_get="yes"
        ], [
          dumpi_cv_have_mpi_errhandler_get="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_errhandler_get" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ERRHANDLER_GET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ERRHANDLER_FREE], [if MPI_Errhandler_free is missing])
    AC_CACHE_CHECK([whether MPI_Errhandler_free is present], [dumpi_cv_have_mpi_errhandler_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Errhandler_free(errhandler);
            PMPI_Errhandler_free(errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_errhandler_free="yes"
        ], [
          dumpi_cv_have_mpi_errhandler_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_errhandler_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ERRHANDLER_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ERROR_STRING], [if MPI_Error_string is missing])
    AC_CACHE_CHECK([whether MPI_Error_string is present], [dumpi_cv_have_mpi_error_string],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int errorcode;
            char *errorstring;
            int *resultlen;
            //
            MPI_Init(aargc, aargv);
            MPI_Error_string(errorcode, errorstring, resultlen);
            PMPI_Error_string(errorcode, errorstring, resultlen);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_error_string="yes"
        ], [
          dumpi_cv_have_mpi_error_string="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_error_string" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ERROR_STRING, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ERROR_CLASS], [if MPI_Error_class is missing])
    AC_CACHE_CHECK([whether MPI_Error_class is present], [dumpi_cv_have_mpi_error_class],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int errorcode;
            int *errorclass;
            //
            MPI_Init(aargc, aargv);
            MPI_Error_class(errorcode, errorclass);
            PMPI_Error_class(errorcode, errorclass);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_error_class="yes"
        ], [
          dumpi_cv_have_mpi_error_class="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_error_class" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ERROR_CLASS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WTIME], [if MPI_Wtime is missing])
    AC_CACHE_CHECK([whether MPI_Wtime is present], [dumpi_cv_have_mpi_wtime],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            //
            MPI_Init(aargc, aargv);
            MPI_Wtime();
            PMPI_Wtime();
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_wtime="yes"
        ], [
          dumpi_cv_have_mpi_wtime="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_wtime" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WTIME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WTICK], [if MPI_Wtick is missing])
    AC_CACHE_CHECK([whether MPI_Wtick is present], [dumpi_cv_have_mpi_wtick],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            //
            MPI_Init(aargc, aargv);
            MPI_Wtick();
            PMPI_Wtick();
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_wtick="yes"
        ], [
          dumpi_cv_have_mpi_wtick="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_wtick" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WTICK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INIT], [if MPI_Init is missing])
    AC_CACHE_CHECK([whether MPI_Init is present], [dumpi_cv_have_mpi_init],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            //
            MPI_Init(aargc, aargv);
            PMPI_Init(aargc, aargv);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_init="yes"
        ], [
          dumpi_cv_have_mpi_init="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_init" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FINALIZE], [if MPI_Finalize is missing])
    AC_CACHE_CHECK([whether MPI_Finalize is present], [dumpi_cv_have_mpi_finalize],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            //
            MPI_Init(aargc, aargv);
            MPI_Finalize();
            PMPI_Finalize;
          ])],
        [ 
          dumpi_cv_have_mpi_finalize="yes"
        ], [
          dumpi_cv_have_mpi_finalize="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_finalize" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FINALIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INITIALIZED], [if MPI_Initialized is missing])
    AC_CACHE_CHECK([whether MPI_Initialized is present], [dumpi_cv_have_mpi_initialized],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *result;
            //
            MPI_Init(aargc, aargv);
            MPI_Initialized(result);
            PMPI_Initialized(result);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_initialized="yes"
        ], [
          dumpi_cv_have_mpi_initialized="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_initialized" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INITIALIZED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ABORT], [if MPI_Abort is missing])
    AC_CACHE_CHECK([whether MPI_Abort is present], [dumpi_cv_have_mpi_abort],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int errorcode;
            //
            MPI_Init(aargc, aargv);
            MPI_Abort(comm, errorcode);
            PMPI_Abort(comm, errorcode);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_abort="yes"
        ], [
          dumpi_cv_have_mpi_abort="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_abort" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ABORT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_CLOSE_PORT], [if MPI_Close_port is missing])
    AC_CACHE_CHECK([whether MPI_Close_port is present], [dumpi_cv_have_mpi_close_port],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *portname;
            //
            MPI_Init(aargc, aargv);
            MPI_Close_port(portname);
            PMPI_Close_port(portname);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_close_port="yes"
        ], [
          dumpi_cv_have_mpi_close_port="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_close_port" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_CLOSE_PORT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_ACCEPT], [if MPI_Comm_accept is missing])
    AC_CACHE_CHECK([whether MPI_Comm_accept is present], [dumpi_cv_have_mpi_comm_accept],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *portname;
            MPI_Info info;
            int root;
            MPI_Comm oldcomm;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_accept(portname, info, root, oldcomm, newcomm);
            PMPI_Comm_accept(portname, info, root, oldcomm, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_accept="yes"
        ], [
          dumpi_cv_have_mpi_comm_accept="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_accept" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_ACCEPT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_CONNECT], [if MPI_Comm_connect is missing])
    AC_CACHE_CHECK([whether MPI_Comm_connect is present], [dumpi_cv_have_mpi_comm_connect],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *portname;
            MPI_Info info;
            int root;
            MPI_Comm oldcomm;
            MPI_Comm *newcomm;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_connect(portname, info, root, oldcomm, newcomm);
            PMPI_Comm_connect(portname, info, root, oldcomm, newcomm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_connect="yes"
        ], [
          dumpi_cv_have_mpi_comm_connect="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_connect" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_CONNECT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_DISCONNECT], [if MPI_Comm_disconnect is missing])
    AC_CACHE_CHECK([whether MPI_Comm_disconnect is present], [dumpi_cv_have_mpi_comm_disconnect],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm *comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_disconnect(comm);
            PMPI_Comm_disconnect(comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_disconnect="yes"
        ], [
          dumpi_cv_have_mpi_comm_disconnect="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_disconnect" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_DISCONNECT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_GET_PARENT], [if MPI_Comm_get_parent is missing])
    AC_CACHE_CHECK([whether MPI_Comm_get_parent is present], [dumpi_cv_have_mpi_comm_get_parent],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm *parent;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_get_parent(parent);
            PMPI_Comm_get_parent(parent);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_get_parent="yes"
        ], [
          dumpi_cv_have_mpi_comm_get_parent="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_get_parent" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_GET_PARENT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_JOIN], [if MPI_Comm_join is missing])
    AC_CACHE_CHECK([whether MPI_Comm_join is present], [dumpi_cv_have_mpi_comm_join],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int fd;
            MPI_Comm *comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_join(fd, comm);
            PMPI_Comm_join(fd, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_join="yes"
        ], [
          dumpi_cv_have_mpi_comm_join="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_join" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_JOIN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_SPAWN], [if MPI_Comm_spawn is missing])
    AC_CACHE_CHECK([whether MPI_Comm_spawn is present], [dumpi_cv_have_mpi_comm_spawn],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *command;
            char **argv;
            int maxprocs;
            MPI_Info info;
            int root;
            MPI_Comm oldcomm;
            MPI_Comm *newcomm;
            int *errcodes;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_spawn(command, argv, maxprocs, info, root, oldcomm, newcomm, errcodes);
            PMPI_Comm_spawn(command, argv, maxprocs, info, root, oldcomm, newcomm, errcodes);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_spawn="yes"
        ], [
          dumpi_cv_have_mpi_comm_spawn="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_spawn" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_SPAWN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_SPAWN_MULTIPLE], [if MPI_Comm_spawn_multiple is missing])
    AC_CACHE_CHECK([whether MPI_Comm_spawn_multiple is present], [dumpi_cv_have_mpi_comm_spawn_multiple],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            char **commands;
            char ***argvs;
            int *maxprocs;
            MPI_Info *info;
            int root;
            MPI_Comm oldcomm;
            MPI_Comm *newcomm;
            int *errcodes;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_spawn_multiple(count, commands, argvs, maxprocs, info, root, oldcomm, newcomm, errcodes);
            PMPI_Comm_spawn_multiple(count, commands, argvs, maxprocs, info, root, oldcomm, newcomm, errcodes);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_spawn_multiple="yes"
        ], [
          dumpi_cv_have_mpi_comm_spawn_multiple="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_spawn_multiple" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_SPAWN_MULTIPLE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_LOOKUP_NAME], [if MPI_Lookup_name is missing])
    AC_CACHE_CHECK([whether MPI_Lookup_name is present], [dumpi_cv_have_mpi_lookup_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *servicename;
            MPI_Info info;
            char *portname;
            //
            MPI_Init(aargc, aargv);
            MPI_Lookup_name(servicename, info, portname);
            PMPI_Lookup_name(servicename, info, portname);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_lookup_name="yes"
        ], [
          dumpi_cv_have_mpi_lookup_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_lookup_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_LOOKUP_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_OPEN_PORT], [if MPI_Open_port is missing])
    AC_CACHE_CHECK([whether MPI_Open_port is present], [dumpi_cv_have_mpi_open_port],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info info;
            char *portname;
            //
            MPI_Init(aargc, aargv);
            MPI_Open_port(info, portname);
            PMPI_Open_port(info, portname);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_open_port="yes"
        ], [
          dumpi_cv_have_mpi_open_port="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_open_port" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_OPEN_PORT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_PUBLISH_NAME], [if MPI_Publish_name is missing])
    AC_CACHE_CHECK([whether MPI_Publish_name is present], [dumpi_cv_have_mpi_publish_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *servicename;
            MPI_Info info;
            char *portname;
            //
            MPI_Init(aargc, aargv);
            MPI_Publish_name(servicename, info, portname);
            PMPI_Publish_name(servicename, info, portname);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_publish_name="yes"
        ], [
          dumpi_cv_have_mpi_publish_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_publish_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_PUBLISH_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_UNPUBLISH_NAME], [if MPI_Unpublish_name is missing])
    AC_CACHE_CHECK([whether MPI_Unpublish_name is present], [dumpi_cv_have_mpi_unpublish_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *servicename;
            MPI_Info info;
            char *portname;
            //
            MPI_Init(aargc, aargv);
            MPI_Unpublish_name(servicename, info, portname);
            PMPI_Unpublish_name(servicename, info, portname);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_unpublish_name="yes"
        ], [
          dumpi_cv_have_mpi_unpublish_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unpublish_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_UNPUBLISH_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ACCUMULATE], [if MPI_Accumulate is missing])
    AC_CACHE_CHECK([whether MPI_Accumulate is present], [dumpi_cv_have_mpi_accumulate],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int origincount;
            MPI_Datatype origintype;
            int targetrank;
            MPI_Aint targetdisp;
            int targetcount;
            MPI_Datatype targettype;
            MPI_Op op;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Accumulate(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, op, win);
            PMPI_Accumulate(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, op, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_accumulate="yes"
        ], [
          dumpi_cv_have_mpi_accumulate="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_accumulate" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ACCUMULATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GET], [if MPI_Get is missing])
    AC_CACHE_CHECK([whether MPI_Get is present], [dumpi_cv_have_mpi_get],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int origincount;
            MPI_Datatype origintype;
            int targetrank;
            MPI_Aint targetdisp;
            int targetcount;
            MPI_Datatype targettype;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Get(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, win);
            PMPI_Get(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_get="yes"
        ], [
          dumpi_cv_have_mpi_get="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_get" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_PUT], [if MPI_Put is missing])
    AC_CACHE_CHECK([whether MPI_Put is present], [dumpi_cv_have_mpi_put],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            int origincount;
            MPI_Datatype origintype;
            int targetrank;
            MPI_Aint targetdisp;
            int targetcount;
            MPI_Datatype targettype;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Put(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, win);
            PMPI_Put(buf, origincount, origintype, targetrank, targetdisp, targetcount, targettype, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_put="yes"
        ], [
          dumpi_cv_have_mpi_put="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_put" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_PUT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_COMPLETE], [if MPI_Win_complete is missing])
    AC_CACHE_CHECK([whether MPI_Win_complete is present], [dumpi_cv_have_mpi_win_complete],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_complete(win);
            PMPI_Win_complete(win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_complete="yes"
        ], [
          dumpi_cv_have_mpi_win_complete="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_complete" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_COMPLETE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_CREATE], [if MPI_Win_create is missing])
    AC_CACHE_CHECK([whether MPI_Win_create is present], [dumpi_cv_have_mpi_win_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *addr;
            MPI_Aint size;
            int dispunit;
            MPI_Info info;
            MPI_Comm comm;
            MPI_Win *win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_create(addr, size, dispunit, info, comm, win);
            PMPI_Win_create(addr, size, dispunit, info, comm, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_create="yes"
        ], [
          dumpi_cv_have_mpi_win_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_FENCE], [if MPI_Win_fence is missing])
    AC_CACHE_CHECK([whether MPI_Win_fence is present], [dumpi_cv_have_mpi_win_fence],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int assertion;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_fence(assertion, win);
            PMPI_Win_fence(assertion, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_fence="yes"
        ], [
          dumpi_cv_have_mpi_win_fence="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_fence" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_FENCE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_FREE], [if MPI_Win_free is missing])
    AC_CACHE_CHECK([whether MPI_Win_free is present], [dumpi_cv_have_mpi_win_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win *win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_free(win);
            PMPI_Win_free(win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_free="yes"
        ], [
          dumpi_cv_have_mpi_win_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_GET_GROUP], [if MPI_Win_get_group is missing])
    AC_CACHE_CHECK([whether MPI_Win_get_group is present], [dumpi_cv_have_mpi_win_get_group],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            MPI_Group *group;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_get_group(win, group);
            PMPI_Win_get_group(win, group);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_get_group="yes"
        ], [
          dumpi_cv_have_mpi_win_get_group="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_get_group" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_GET_GROUP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_LOCK], [if MPI_Win_lock is missing])
    AC_CACHE_CHECK([whether MPI_Win_lock is present], [dumpi_cv_have_mpi_win_lock],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int locktype;
            int winrank;
            int assertion;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_lock(locktype, winrank, assertion, win);
            PMPI_Win_lock(locktype, winrank, assertion, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_lock="yes"
        ], [
          dumpi_cv_have_mpi_win_lock="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_lock" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_LOCK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_POST], [if MPI_Win_post is missing])
    AC_CACHE_CHECK([whether MPI_Win_post is present], [dumpi_cv_have_mpi_win_post],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group;
            int assertion;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_post(group, assertion, win);
            PMPI_Win_post(group, assertion, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_post="yes"
        ], [
          dumpi_cv_have_mpi_win_post="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_post" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_POST, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_START], [if MPI_Win_start is missing])
    AC_CACHE_CHECK([whether MPI_Win_start is present], [dumpi_cv_have_mpi_win_start],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Group group;
            int assertion;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_start(group, assertion, win);
            PMPI_Win_start(group, assertion, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_start="yes"
        ], [
          dumpi_cv_have_mpi_win_start="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_start" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_START, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_TEST], [if MPI_Win_test is missing])
    AC_CACHE_CHECK([whether MPI_Win_test is present], [dumpi_cv_have_mpi_win_test],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_test(win, flag);
            PMPI_Win_test(win, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_test="yes"
        ], [
          dumpi_cv_have_mpi_win_test="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_test" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_TEST, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_UNLOCK], [if MPI_Win_unlock is missing])
    AC_CACHE_CHECK([whether MPI_Win_unlock is present], [dumpi_cv_have_mpi_win_unlock],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int winrank;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_unlock(winrank, win);
            PMPI_Win_unlock(winrank, win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_unlock="yes"
        ], [
          dumpi_cv_have_mpi_win_unlock="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_unlock" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_UNLOCK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_WAIT], [if MPI_Win_wait is missing])
    AC_CACHE_CHECK([whether MPI_Win_wait is present], [dumpi_cv_have_mpi_win_wait],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_wait(win);
            PMPI_Win_wait(win);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_wait="yes"
        ], [
          dumpi_cv_have_mpi_win_wait="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_wait" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_WAIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ALLTOALLW], [if MPI_Alltoallw is missing])
    AC_CACHE_CHECK([whether MPI_Alltoallw is present], [dumpi_cv_have_mpi_alltoallw],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            int *sendcounts;
            int *senddispls;
            MPI_Datatype *sendtypes;
            void *recvbuf;
            int *recvcounts;
            int *recvdispls;
            MPI_Datatype *recvtypes;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Alltoallw(sendbuf, sendcounts, senddispls, sendtypes, recvbuf, recvcounts, recvdispls, recvtypes, comm);
            PMPI_Alltoallw(sendbuf, sendcounts, senddispls, sendtypes, recvbuf, recvcounts, recvdispls, recvtypes, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_alltoallw="yes"
        ], [
          dumpi_cv_have_mpi_alltoallw="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_alltoallw" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ALLTOALLW, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_EXSCAN], [if MPI_Exscan is missing])
    AC_CACHE_CHECK([whether MPI_Exscan is present], [dumpi_cv_have_mpi_exscan],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *sendbuf;
            void *recvbuf;
            int count;
            MPI_Datatype datatype;
            MPI_Op op;
            MPI_Comm comm;
            //
            MPI_Init(aargc, aargv);
            MPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm);
            PMPI_Exscan(sendbuf, recvbuf, count, datatype, op, comm);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_exscan="yes"
        ], [
          dumpi_cv_have_mpi_exscan="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_exscan" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_EXSCAN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ADD_ERROR_CLASS], [if MPI_Add_error_class is missing])
    AC_CACHE_CHECK([whether MPI_Add_error_class is present], [dumpi_cv_have_mpi_add_error_class],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *errorclass;
            //
            MPI_Init(aargc, aargv);
            MPI_Add_error_class(errorclass);
            PMPI_Add_error_class(errorclass);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_add_error_class="yes"
        ], [
          dumpi_cv_have_mpi_add_error_class="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_add_error_class" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ADD_ERROR_CLASS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ADD_ERROR_CODE], [if MPI_Add_error_code is missing])
    AC_CACHE_CHECK([whether MPI_Add_error_code is present], [dumpi_cv_have_mpi_add_error_code],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int errorclass;
            int *errorcode;
            //
            MPI_Init(aargc, aargv);
            MPI_Add_error_code(errorclass, errorcode);
            PMPI_Add_error_code(errorclass, errorcode);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_add_error_code="yes"
        ], [
          dumpi_cv_have_mpi_add_error_code="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_add_error_code" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ADD_ERROR_CODE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ADD_ERROR_STRING], [if MPI_Add_error_string is missing])
    AC_CACHE_CHECK([whether MPI_Add_error_string is present], [dumpi_cv_have_mpi_add_error_string],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int errorcode;
            char *errorstring;
            //
            MPI_Init(aargc, aargv);
            MPI_Add_error_string(errorcode, errorstring);
            PMPI_Add_error_string(errorcode, errorstring);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_add_error_string="yes"
        ], [
          dumpi_cv_have_mpi_add_error_string="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_add_error_string" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ADD_ERROR_STRING, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_CALL_ERRHANDLER], [if MPI_Comm_call_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_Comm_call_errhandler is present], [dumpi_cv_have_mpi_comm_call_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int errorcode;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_call_errhandler(comm, errorcode);
            PMPI_Comm_call_errhandler(comm, errorcode);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_call_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_comm_call_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_call_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_CALL_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_CREATE_KEYVAL], [if MPI_Comm_create_keyval is missing])
    AC_CACHE_CHECK([whether MPI_Comm_create_keyval is present], [dumpi_cv_have_mpi_comm_create_keyval],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm_copy_attr_function *copyfunc;
            MPI_Comm_delete_attr_function *deletefunc;
            int *keyval;
            void *extrastate;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_create_keyval(copyfunc, deletefunc, keyval, extrastate);
            PMPI_Comm_create_keyval(copyfunc, deletefunc, keyval, extrastate);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_create_keyval="yes"
        ], [
          dumpi_cv_have_mpi_comm_create_keyval="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_create_keyval" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_CREATE_KEYVAL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_DELETE_ATTR], [if MPI_Comm_delete_attr is missing])
    AC_CACHE_CHECK([whether MPI_Comm_delete_attr is present], [dumpi_cv_have_mpi_comm_delete_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int keyval;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_delete_attr(comm, keyval);
            PMPI_Comm_delete_attr(comm, keyval);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_delete_attr="yes"
        ], [
          dumpi_cv_have_mpi_comm_delete_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_delete_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_DELETE_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_FREE_KEYVAL], [if MPI_Comm_free_keyval is missing])
    AC_CACHE_CHECK([whether MPI_Comm_free_keyval is present], [dumpi_cv_have_mpi_comm_free_keyval],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *keyval;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_free_keyval(keyval);
            PMPI_Comm_free_keyval(keyval);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_free_keyval="yes"
        ], [
          dumpi_cv_have_mpi_comm_free_keyval="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_free_keyval" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_FREE_KEYVAL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_GET_ATTR], [if MPI_Comm_get_attr is missing])
    AC_CACHE_CHECK([whether MPI_Comm_get_attr is present], [dumpi_cv_have_mpi_comm_get_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int keyval;
            void *value;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_get_attr(comm, keyval, value, flag);
            PMPI_Comm_get_attr(comm, keyval, value, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_get_attr="yes"
        ], [
          dumpi_cv_have_mpi_comm_get_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_get_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_GET_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_GET_NAME], [if MPI_Comm_get_name is missing])
    AC_CACHE_CHECK([whether MPI_Comm_get_name is present], [dumpi_cv_have_mpi_comm_get_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            char *name;
            int *resultlen;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_get_name(comm, name, resultlen);
            PMPI_Comm_get_name(comm, name, resultlen);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_get_name="yes"
        ], [
          dumpi_cv_have_mpi_comm_get_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_get_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_GET_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_SET_ATTR], [if MPI_Comm_set_attr is missing])
    AC_CACHE_CHECK([whether MPI_Comm_set_attr is present], [dumpi_cv_have_mpi_comm_set_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            int keyval;
            void *value;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_set_attr(comm, keyval, value);
            PMPI_Comm_set_attr(comm, keyval, value);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_set_attr="yes"
        ], [
          dumpi_cv_have_mpi_comm_set_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_set_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_SET_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_SET_NAME], [if MPI_Comm_set_name is missing])
    AC_CACHE_CHECK([whether MPI_Comm_set_name is present], [dumpi_cv_have_mpi_comm_set_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            char *name;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_set_name(comm, name);
            PMPI_Comm_set_name(comm, name);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_set_name="yes"
        ], [
          dumpi_cv_have_mpi_comm_set_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_set_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_SET_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_CALL_ERRHANDLER], [if MPI_File_call_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_File_call_errhandler is present], [dumpi_cv_have_mpi_file_call_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            int errorcode;
            //
            MPI_Init(aargc, aargv);
            MPI_File_call_errhandler(file, errorcode);
            PMPI_File_call_errhandler(file, errorcode);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_call_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_file_call_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_call_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_CALL_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GREQUEST_COMPLETE], [if MPI_Grequest_complete is missing])
    AC_CACHE_CHECK([whether MPI_Grequest_complete is present], [dumpi_cv_have_mpi_grequest_complete],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Request request;
            //
            MPI_Init(aargc, aargv);
            MPI_Grequest_complete(request);
            PMPI_Grequest_complete(request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_grequest_complete="yes"
        ], [
          dumpi_cv_have_mpi_grequest_complete="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_grequest_complete" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GREQUEST_COMPLETE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GREQUEST_START], [if MPI_Grequest_start is missing])
    AC_CACHE_CHECK([whether MPI_Grequest_start is present], [dumpi_cv_have_mpi_grequest_start],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Grequest_query_function *queryfunc;
            MPI_Grequest_free_function *freefunc;
            MPI_Grequest_cancel_function *cancelfunc;
            void *state;
            MPI_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_Grequest_start(queryfunc, freefunc, cancelfunc, state, request);
            PMPI_Grequest_start(queryfunc, freefunc, cancelfunc, state, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_grequest_start="yes"
        ], [
          dumpi_cv_have_mpi_grequest_start="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_grequest_start" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GREQUEST_START, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INIT_THREAD], [if MPI_Init_thread is missing])
    AC_CACHE_CHECK([whether MPI_Init_thread is present], [dumpi_cv_have_mpi_init_thread],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int required;
            int *provided;
            //
            MPI_Init_thread(aargc, aargv, required, provided);
            PMPI_Init_thread(aargc, aargv, required, provided);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_init_thread="yes"
        ], [
          dumpi_cv_have_mpi_init_thread="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_init_thread" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INIT_THREAD, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_IS_THREAD_MAIN], [if MPI_Is_thread_main is missing])
    AC_CACHE_CHECK([whether MPI_Is_thread_main is present], [dumpi_cv_have_mpi_is_thread_main],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Is_thread_main(flag);
            PMPI_Is_thread_main(flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_is_thread_main="yes"
        ], [
          dumpi_cv_have_mpi_is_thread_main="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_is_thread_main" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_IS_THREAD_MAIN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_QUERY_THREAD], [if MPI_Query_thread is missing])
    AC_CACHE_CHECK([whether MPI_Query_thread is present], [dumpi_cv_have_mpi_query_thread],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *supported;
            //
            MPI_Init(aargc, aargv);
            MPI_Query_thread(supported);
            PMPI_Query_thread(supported);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_query_thread="yes"
        ], [
          dumpi_cv_have_mpi_query_thread="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_query_thread" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_QUERY_THREAD, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_STATUS_SET_CANCELLED], [if MPI_Status_set_cancelled is missing])
    AC_CACHE_CHECK([whether MPI_Status_set_cancelled is present], [dumpi_cv_have_mpi_status_set_cancelled],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Status *status;
            int flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Status_set_cancelled(status, flag);
            PMPI_Status_set_cancelled(status, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_status_set_cancelled="yes"
        ], [
          dumpi_cv_have_mpi_status_set_cancelled="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_status_set_cancelled" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_STATUS_SET_CANCELLED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_STATUS_SET_ELEMENTS], [if MPI_Status_set_elements is missing])
    AC_CACHE_CHECK([whether MPI_Status_set_elements is present], [dumpi_cv_have_mpi_status_set_elements],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Status *status;
            MPI_Datatype datatype;
            int count;
            //
            MPI_Init(aargc, aargv);
            MPI_Status_set_elements(status, datatype, count);
            PMPI_Status_set_elements(status, datatype, count);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_status_set_elements="yes"
        ], [
          dumpi_cv_have_mpi_status_set_elements="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_status_set_elements" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_STATUS_SET_ELEMENTS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CREATE_KEYVAL], [if MPI_Type_create_keyval is missing])
    AC_CACHE_CHECK([whether MPI_Type_create_keyval is present], [dumpi_cv_have_mpi_type_create_keyval],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Type_copy_attr_function *copyfunc;
            MPI_Type_delete_attr_function *deletefunc;
            int *keyval;
            void *state;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_create_keyval(copyfunc, deletefunc, keyval, state);
            PMPI_Type_create_keyval(copyfunc, deletefunc, keyval, state);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_create_keyval="yes"
        ], [
          dumpi_cv_have_mpi_type_create_keyval="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_create_keyval" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CREATE_KEYVAL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_DELETE_ATTR], [if MPI_Type_delete_attr is missing])
    AC_CACHE_CHECK([whether MPI_Type_delete_attr is present], [dumpi_cv_have_mpi_type_delete_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            int keyval;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_delete_attr(datatype, keyval);
            PMPI_Type_delete_attr(datatype, keyval);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_delete_attr="yes"
        ], [
          dumpi_cv_have_mpi_type_delete_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_delete_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_DELETE_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_DUP], [if MPI_Type_dup is missing])
    AC_CACHE_CHECK([whether MPI_Type_dup is present], [dumpi_cv_have_mpi_type_dup],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_dup(oldtype, newtype);
            PMPI_Type_dup(oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_dup="yes"
        ], [
          dumpi_cv_have_mpi_type_dup="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_dup" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_DUP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_FREE_KEYVAL], [if MPI_Type_free_keyval is missing])
    AC_CACHE_CHECK([whether MPI_Type_free_keyval is present], [dumpi_cv_have_mpi_type_free_keyval],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *keyval;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_free_keyval(keyval);
            PMPI_Type_free_keyval(keyval);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_free_keyval="yes"
        ], [
          dumpi_cv_have_mpi_type_free_keyval="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_free_keyval" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_FREE_KEYVAL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_GET_ATTR], [if MPI_Type_get_attr is missing])
    AC_CACHE_CHECK([whether MPI_Type_get_attr is present], [dumpi_cv_have_mpi_type_get_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            int keyval;
            void *value;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_get_attr(datatype, keyval, value, flag);
            PMPI_Type_get_attr(datatype, keyval, value, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_get_attr="yes"
        ], [
          dumpi_cv_have_mpi_type_get_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_get_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_GET_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_GET_CONTENTS], [if MPI_Type_get_contents is missing])
    AC_CACHE_CHECK([whether MPI_Type_get_contents is present], [dumpi_cv_have_mpi_type_get_contents],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            int maxintegers;
            int maxaddresses;
            int maxdatatypes;
            int *arrintegers;
            MPI_Aint *arraddresses;
            MPI_Datatype *arrdatatypes;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_get_contents(datatype, maxintegers, maxaddresses, maxdatatypes, arrintegers, arraddresses, arrdatatypes);
            PMPI_Type_get_contents(datatype, maxintegers, maxaddresses, maxdatatypes, arrintegers, arraddresses, arrdatatypes);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_get_contents="yes"
        ], [
          dumpi_cv_have_mpi_type_get_contents="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_get_contents" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_GET_CONTENTS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_GET_ENVELOPE], [if MPI_Type_get_envelope is missing])
    AC_CACHE_CHECK([whether MPI_Type_get_envelope is present], [dumpi_cv_have_mpi_type_get_envelope],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            int *numintegers;
            int *numaddresses;
            int *numdatatypes;
            int *combiner;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_get_envelope(datatype, numintegers, numaddresses, numdatatypes, combiner);
            PMPI_Type_get_envelope(datatype, numintegers, numaddresses, numdatatypes, combiner);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_get_envelope="yes"
        ], [
          dumpi_cv_have_mpi_type_get_envelope="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_get_envelope" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_GET_ENVELOPE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_GET_NAME], [if MPI_Type_get_name is missing])
    AC_CACHE_CHECK([whether MPI_Type_get_name is present], [dumpi_cv_have_mpi_type_get_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            char *name;
            int *resultlen;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_get_name(datatype, name, resultlen);
            PMPI_Type_get_name(datatype, name, resultlen);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_get_name="yes"
        ], [
          dumpi_cv_have_mpi_type_get_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_get_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_GET_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_SET_ATTR], [if MPI_Type_set_attr is missing])
    AC_CACHE_CHECK([whether MPI_Type_set_attr is present], [dumpi_cv_have_mpi_type_set_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            int keyval;
            void *state;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_set_attr(datatype, keyval, state);
            PMPI_Type_set_attr(datatype, keyval, state);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_set_attr="yes"
        ], [
          dumpi_cv_have_mpi_type_set_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_set_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_SET_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_SET_NAME], [if MPI_Type_set_name is missing])
    AC_CACHE_CHECK([whether MPI_Type_set_name is present], [dumpi_cv_have_mpi_type_set_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            char *name;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_set_name(datatype, name);
            PMPI_Type_set_name(datatype, name);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_set_name="yes"
        ], [
          dumpi_cv_have_mpi_type_set_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_set_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_SET_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_MATCH_SIZE], [if MPI_Type_match_size is missing])
    AC_CACHE_CHECK([whether MPI_Type_match_size is present], [dumpi_cv_have_mpi_type_match_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int typeclass;
            int size;
            MPI_Datatype *datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_match_size(typeclass, size, datatype);
            PMPI_Type_match_size(typeclass, size, datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_match_size="yes"
        ], [
          dumpi_cv_have_mpi_type_match_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_match_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_MATCH_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_CALL_ERRHANDLER], [if MPI_Win_call_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_Win_call_errhandler is present], [dumpi_cv_have_mpi_win_call_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            int errorcode;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_call_errhandler(win, errorcode);
            PMPI_Win_call_errhandler(win, errorcode);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_call_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_win_call_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_call_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_CALL_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_CREATE_KEYVAL], [if MPI_Win_create_keyval is missing])
    AC_CACHE_CHECK([whether MPI_Win_create_keyval is present], [dumpi_cv_have_mpi_win_create_keyval],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win_copy_attr_function *copyfun;
            MPI_Win_delete_attr_function *deletefun;
            int *keyval;
            void *state;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_create_keyval(copyfun, deletefun, keyval, state);
            PMPI_Win_create_keyval(copyfun, deletefun, keyval, state);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_create_keyval="yes"
        ], [
          dumpi_cv_have_mpi_win_create_keyval="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_create_keyval" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_CREATE_KEYVAL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_DELETE_ATTR], [if MPI_Win_delete_attr is missing])
    AC_CACHE_CHECK([whether MPI_Win_delete_attr is present], [dumpi_cv_have_mpi_win_delete_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            int keyval;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_delete_attr(win, keyval);
            PMPI_Win_delete_attr(win, keyval);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_delete_attr="yes"
        ], [
          dumpi_cv_have_mpi_win_delete_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_delete_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_DELETE_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_FREE_KEYVAL], [if MPI_Win_free_keyval is missing])
    AC_CACHE_CHECK([whether MPI_Win_free_keyval is present], [dumpi_cv_have_mpi_win_free_keyval],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *keyval;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_free_keyval(keyval);
            PMPI_Win_free_keyval(keyval);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_free_keyval="yes"
        ], [
          dumpi_cv_have_mpi_win_free_keyval="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_free_keyval" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_FREE_KEYVAL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_GET_ATTR], [if MPI_Win_get_attr is missing])
    AC_CACHE_CHECK([whether MPI_Win_get_attr is present], [dumpi_cv_have_mpi_win_get_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            int keyval;
            void *state;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_get_attr(win, keyval, state, flag);
            PMPI_Win_get_attr(win, keyval, state, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_get_attr="yes"
        ], [
          dumpi_cv_have_mpi_win_get_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_get_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_GET_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_GET_NAME], [if MPI_Win_get_name is missing])
    AC_CACHE_CHECK([whether MPI_Win_get_name is present], [dumpi_cv_have_mpi_win_get_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            char *name;
            int *resultlen;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_get_name(win, name, resultlen);
            PMPI_Win_get_name(win, name, resultlen);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_get_name="yes"
        ], [
          dumpi_cv_have_mpi_win_get_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_get_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_GET_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_SET_ATTR], [if MPI_Win_set_attr is missing])
    AC_CACHE_CHECK([whether MPI_Win_set_attr is present], [dumpi_cv_have_mpi_win_set_attr],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            int keyval;
            void *state;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_set_attr(win, keyval, state);
            PMPI_Win_set_attr(win, keyval, state);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_set_attr="yes"
        ], [
          dumpi_cv_have_mpi_win_set_attr="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_set_attr" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_SET_ATTR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_SET_NAME], [if MPI_Win_set_name is missing])
    AC_CACHE_CHECK([whether MPI_Win_set_name is present], [dumpi_cv_have_mpi_win_set_name],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            char *name;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_set_name(win, name);
            PMPI_Win_set_name(win, name);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_set_name="yes"
        ], [
          dumpi_cv_have_mpi_win_set_name="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_set_name" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_SET_NAME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_ALLOC_MEM], [if MPI_Alloc_mem is missing])
    AC_CACHE_CHECK([whether MPI_Alloc_mem is present], [dumpi_cv_have_mpi_alloc_mem],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Aint size;
            MPI_Info info;
            void *baseptr;
            //
            MPI_Init(aargc, aargv);
            MPI_Alloc_mem(size, info, baseptr);
            PMPI_Alloc_mem(size, info, baseptr);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_alloc_mem="yes"
        ], [
          dumpi_cv_have_mpi_alloc_mem="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_alloc_mem" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_ALLOC_MEM, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_CREATE_ERRHANDLER], [if MPI_Comm_create_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_Comm_create_errhandler is present], [dumpi_cv_have_mpi_comm_create_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm_errhandler_fn *errfun;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_create_errhandler(errfun, errhandler);
            PMPI_Comm_create_errhandler(errfun, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_create_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_comm_create_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_create_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_CREATE_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_GET_ERRHANDLER], [if MPI_Comm_get_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_Comm_get_errhandler is present], [dumpi_cv_have_mpi_comm_get_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_get_errhandler(comm, errhandler);
            PMPI_Comm_get_errhandler(comm, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_get_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_comm_get_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_get_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_GET_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_COMM_SET_ERRHANDLER], [if MPI_Comm_set_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_Comm_set_errhandler is present], [dumpi_cv_have_mpi_comm_set_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            MPI_Errhandler errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Comm_set_errhandler(comm, errhandler);
            PMPI_Comm_set_errhandler(comm, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_comm_set_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_comm_set_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_comm_set_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_COMM_SET_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_CREATE_ERRHANDLER], [if MPI_File_create_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_File_create_errhandler is present], [dumpi_cv_have_mpi_file_create_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File_errhandler_fn *errfun;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_File_create_errhandler(errfun, errhandler);
            PMPI_File_create_errhandler(errfun, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_create_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_file_create_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_create_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_CREATE_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_ERRHANDLER], [if MPI_File_get_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_File_get_errhandler is present], [dumpi_cv_have_mpi_file_get_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_errhandler(file, errhandler);
            PMPI_File_get_errhandler(file, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_file_get_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_SET_ERRHANDLER], [if MPI_File_set_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_File_set_errhandler is present], [dumpi_cv_have_mpi_file_set_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Errhandler errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_File_set_errhandler(file, errhandler);
            PMPI_File_set_errhandler(file, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_set_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_file_set_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_set_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_SET_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FINALIZED], [if MPI_Finalized is missing])
    AC_CACHE_CHECK([whether MPI_Finalized is present], [dumpi_cv_have_mpi_finalized],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Finalized(flag);
            PMPI_Finalized(flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_finalized="yes"
        ], [
          dumpi_cv_have_mpi_finalized="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_finalized" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FINALIZED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FREE_MEM], [if MPI_Free_mem is missing])
    AC_CACHE_CHECK([whether MPI_Free_mem is present], [dumpi_cv_have_mpi_free_mem],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            //
            MPI_Init(aargc, aargv);
            MPI_Free_mem(buf);
            PMPI_Free_mem(buf);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_free_mem="yes"
        ], [
          dumpi_cv_have_mpi_free_mem="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_free_mem" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FREE_MEM, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_GET_ADDRESS], [if MPI_Get_address is missing])
    AC_CACHE_CHECK([whether MPI_Get_address is present], [dumpi_cv_have_mpi_get_address],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            void *buf;
            MPI_Aint *address;
            //
            MPI_Init(aargc, aargv);
            MPI_Get_address(buf, address);
            PMPI_Get_address(buf, address);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_get_address="yes"
        ], [
          dumpi_cv_have_mpi_get_address="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_get_address" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_GET_ADDRESS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_CREATE], [if MPI_Info_create is missing])
    AC_CACHE_CHECK([whether MPI_Info_create is present], [dumpi_cv_have_mpi_info_create],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info *info;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_create(info);
            PMPI_Info_create(info);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_create="yes"
        ], [
          dumpi_cv_have_mpi_info_create="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_create" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_CREATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_DELETE], [if MPI_Info_delete is missing])
    AC_CACHE_CHECK([whether MPI_Info_delete is present], [dumpi_cv_have_mpi_info_delete],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info info;
            char *key;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_delete(info, key);
            PMPI_Info_delete(info, key);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_delete="yes"
        ], [
          dumpi_cv_have_mpi_info_delete="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_delete" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_DELETE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_DUP], [if MPI_Info_dup is missing])
    AC_CACHE_CHECK([whether MPI_Info_dup is present], [dumpi_cv_have_mpi_info_dup],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info oldinfo;
            MPI_Info *newinfo;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_dup(oldinfo, newinfo);
            PMPI_Info_dup(oldinfo, newinfo);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_dup="yes"
        ], [
          dumpi_cv_have_mpi_info_dup="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_dup" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_DUP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_FREE], [if MPI_Info_free is missing])
    AC_CACHE_CHECK([whether MPI_Info_free is present], [dumpi_cv_have_mpi_info_free],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info *info;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_free(info);
            PMPI_Info_free(info);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_free="yes"
        ], [
          dumpi_cv_have_mpi_info_free="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_free" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_FREE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_GET], [if MPI_Info_get is missing])
    AC_CACHE_CHECK([whether MPI_Info_get is present], [dumpi_cv_have_mpi_info_get],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info info;
            char *key;
            int valuelength;
            char *value;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_get(info, key, valuelength, value, flag);
            PMPI_Info_get(info, key, valuelength, value, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_get="yes"
        ], [
          dumpi_cv_have_mpi_info_get="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_get" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_GET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_GET_NKEYS], [if MPI_Info_get_nkeys is missing])
    AC_CACHE_CHECK([whether MPI_Info_get_nkeys is present], [dumpi_cv_have_mpi_info_get_nkeys],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info info;
            int *nkeys;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_get_nkeys(info, nkeys);
            PMPI_Info_get_nkeys(info, nkeys);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_get_nkeys="yes"
        ], [
          dumpi_cv_have_mpi_info_get_nkeys="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_get_nkeys" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_GET_NKEYS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_GET_NTHKEY], [if MPI_Info_get_nthkey is missing])
    AC_CACHE_CHECK([whether MPI_Info_get_nthkey is present], [dumpi_cv_have_mpi_info_get_nthkey],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info info;
            int n;
            char *key;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_get_nthkey(info, n, key);
            PMPI_Info_get_nthkey(info, n, key);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_get_nthkey="yes"
        ], [
          dumpi_cv_have_mpi_info_get_nthkey="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_get_nthkey" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_GET_NTHKEY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_GET_VALUELEN], [if MPI_Info_get_valuelen is missing])
    AC_CACHE_CHECK([whether MPI_Info_get_valuelen is present], [dumpi_cv_have_mpi_info_get_valuelen],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info info;
            char *key;
            int *valuelen;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_get_valuelen(info, key, valuelen, flag);
            PMPI_Info_get_valuelen(info, key, valuelen, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_get_valuelen="yes"
        ], [
          dumpi_cv_have_mpi_info_get_valuelen="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_get_valuelen" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_GET_VALUELEN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_INFO_SET], [if MPI_Info_set is missing])
    AC_CACHE_CHECK([whether MPI_Info_set is present], [dumpi_cv_have_mpi_info_set],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Info info;
            char *key;
            char *value;
            //
            MPI_Init(aargc, aargv);
            MPI_Info_set(info, key, value);
            PMPI_Info_set(info, key, value);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_info_set="yes"
        ], [
          dumpi_cv_have_mpi_info_set="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_info_set" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_INFO_SET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_PACK_EXTERNAL], [if MPI_Pack_external is missing])
    AC_CACHE_CHECK([whether MPI_Pack_external is present], [dumpi_cv_have_mpi_pack_external],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *datarep;
            void *inbuf;
            int incount;
            MPI_Datatype intype;
            void *outbuf;
            MPI_Aint outcount;
            MPI_Aint *position;
            //
            MPI_Init(aargc, aargv);
            MPI_Pack_external(datarep, inbuf, incount, intype, outbuf, outcount, position);
            PMPI_Pack_external(datarep, inbuf, incount, intype, outbuf, outcount, position);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_pack_external="yes"
        ], [
          dumpi_cv_have_mpi_pack_external="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_pack_external" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_PACK_EXTERNAL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_PACK_EXTERNAL_SIZE], [if MPI_Pack_external_size is missing])
    AC_CACHE_CHECK([whether MPI_Pack_external_size is present], [dumpi_cv_have_mpi_pack_external_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *datarep;
            int incount;
            MPI_Datatype datatype;
            MPI_Aint *size;
            //
            MPI_Init(aargc, aargv);
            MPI_Pack_external_size(datarep, incount, datatype, size);
            PMPI_Pack_external_size(datarep, incount, datatype, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_pack_external_size="yes"
        ], [
          dumpi_cv_have_mpi_pack_external_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_pack_external_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_PACK_EXTERNAL_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_REQUEST_GET_STATUS], [if MPI_Request_get_status is missing])
    AC_CACHE_CHECK([whether MPI_Request_get_status is present], [dumpi_cv_have_mpi_request_get_status],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Request request;
            int *flag;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_Request_get_status(request, flag, status);
            PMPI_Request_get_status(request, flag, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_request_get_status="yes"
        ], [
          dumpi_cv_have_mpi_request_get_status="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_request_get_status" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_REQUEST_GET_STATUS, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CREATE_DARRAY], [if MPI_Type_create_darray is missing])
    AC_CACHE_CHECK([whether MPI_Type_create_darray is present], [dumpi_cv_have_mpi_type_create_darray],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int size;
            int rank;
            int ndims;
            int *gsizes;
            int *distribs;
            int *dargs;
            int *psizes;
            int order;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_create_darray(size, rank, ndims, gsizes, distribs, dargs, psizes, order, oldtype, newtype);
            PMPI_Type_create_darray(size, rank, ndims, gsizes, distribs, dargs, psizes, order, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_create_darray="yes"
        ], [
          dumpi_cv_have_mpi_type_create_darray="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_create_darray" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CREATE_DARRAY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CREATE_HINDEXED], [if MPI_Type_create_hindexed is missing])
    AC_CACHE_CHECK([whether MPI_Type_create_hindexed is present], [dumpi_cv_have_mpi_type_create_hindexed],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int *blocklengths;
            MPI_Aint *displacements;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_create_hindexed(count, blocklengths, displacements, oldtype, newtype);
            PMPI_Type_create_hindexed(count, blocklengths, displacements, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_create_hindexed="yes"
        ], [
          dumpi_cv_have_mpi_type_create_hindexed="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_create_hindexed" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CREATE_HINDEXED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CREATE_HVECTOR], [if MPI_Type_create_hvector is missing])
    AC_CACHE_CHECK([whether MPI_Type_create_hvector is present], [dumpi_cv_have_mpi_type_create_hvector],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int blocklength;
            MPI_Aint stride;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype);
            PMPI_Type_create_hvector(count, blocklength, stride, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_create_hvector="yes"
        ], [
          dumpi_cv_have_mpi_type_create_hvector="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_create_hvector" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CREATE_HVECTOR, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CREATE_INDEXED_BLOCK], [if MPI_Type_create_indexed_block is missing])
    AC_CACHE_CHECK([whether MPI_Type_create_indexed_block is present], [dumpi_cv_have_mpi_type_create_indexed_block],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int blocklength;
            int *displacments;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_create_indexed_block(count, blocklength, displacments, oldtype, newtype);
            PMPI_Type_create_indexed_block(count, blocklength, displacments, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_create_indexed_block="yes"
        ], [
          dumpi_cv_have_mpi_type_create_indexed_block="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_create_indexed_block" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CREATE_INDEXED_BLOCK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CREATE_RESIZED], [if MPI_Type_create_resized is missing])
    AC_CACHE_CHECK([whether MPI_Type_create_resized is present], [dumpi_cv_have_mpi_type_create_resized],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype oldtype;
            MPI_Aint lb;
            MPI_Aint extent;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_create_resized(oldtype, lb, extent, newtype);
            PMPI_Type_create_resized(oldtype, lb, extent, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_create_resized="yes"
        ], [
          dumpi_cv_have_mpi_type_create_resized="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_create_resized" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CREATE_RESIZED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CREATE_STRUCT], [if MPI_Type_create_struct is missing])
    AC_CACHE_CHECK([whether MPI_Type_create_struct is present], [dumpi_cv_have_mpi_type_create_struct],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            int *blocklengths;
            MPI_Aint *displacements;
            MPI_Datatype *oldtypes;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_create_struct(count, blocklengths, displacements, oldtypes, newtype);
            PMPI_Type_create_struct(count, blocklengths, displacements, oldtypes, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_create_struct="yes"
        ], [
          dumpi_cv_have_mpi_type_create_struct="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_create_struct" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CREATE_STRUCT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_CREATE_SUBARRAY], [if MPI_Type_create_subarray is missing])
    AC_CACHE_CHECK([whether MPI_Type_create_subarray is present], [dumpi_cv_have_mpi_type_create_subarray],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int ndims;
            int *sizes;
            int *subsizes;
            int *starts;
            int order;
            MPI_Datatype oldtype;
            MPI_Datatype *newtype;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_create_subarray(ndims, sizes, subsizes, starts, order, oldtype, newtype);
            PMPI_Type_create_subarray(ndims, sizes, subsizes, starts, order, oldtype, newtype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_create_subarray="yes"
        ], [
          dumpi_cv_have_mpi_type_create_subarray="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_create_subarray" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_CREATE_SUBARRAY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_GET_EXTENT], [if MPI_Type_get_extent is missing])
    AC_CACHE_CHECK([whether MPI_Type_get_extent is present], [dumpi_cv_have_mpi_type_get_extent],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            MPI_Aint *lb;
            MPI_Aint *extent;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_get_extent(datatype, lb, extent);
            PMPI_Type_get_extent(datatype, lb, extent);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_get_extent="yes"
        ], [
          dumpi_cv_have_mpi_type_get_extent="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_get_extent" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_GET_EXTENT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_TYPE_GET_TRUE_EXTENT], [if MPI_Type_get_true_extent is missing])
    AC_CACHE_CHECK([whether MPI_Type_get_true_extent is present], [dumpi_cv_have_mpi_type_get_true_extent],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Datatype datatype;
            MPI_Aint *lb;
            MPI_Aint *extent;
            //
            MPI_Init(aargc, aargv);
            MPI_Type_get_true_extent(datatype, lb, extent);
            PMPI_Type_get_true_extent(datatype, lb, extent);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_type_get_true_extent="yes"
        ], [
          dumpi_cv_have_mpi_type_get_true_extent="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_type_get_true_extent" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_TYPE_GET_TRUE_EXTENT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_UNPACK_EXTERNAL], [if MPI_Unpack_external is missing])
    AC_CACHE_CHECK([whether MPI_Unpack_external is present], [dumpi_cv_have_mpi_unpack_external],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *datarep;
            void *inbuf;
            MPI_Aint insize;
            MPI_Aint *position;
            void *outbuf;
            int outcount;
            MPI_Datatype datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype);
            PMPI_Unpack_external(datarep, inbuf, insize, position, outbuf, outcount, datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_unpack_external="yes"
        ], [
          dumpi_cv_have_mpi_unpack_external="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_unpack_external" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_UNPACK_EXTERNAL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_CREATE_ERRHANDLER], [if MPI_Win_create_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_Win_create_errhandler is present], [dumpi_cv_have_mpi_win_create_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win_errhandler_fn *errfun;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_create_errhandler(errfun, errhandler);
            PMPI_Win_create_errhandler(errfun, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_create_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_win_create_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_create_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_CREATE_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_GET_ERRHANDLER], [if MPI_Win_get_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_Win_get_errhandler is present], [dumpi_cv_have_mpi_win_get_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            MPI_Errhandler *errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_get_errhandler(win, errhandler);
            PMPI_Win_get_errhandler(win, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_get_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_win_get_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_get_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_GET_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_WIN_SET_ERRHANDLER], [if MPI_Win_set_errhandler is missing])
    AC_CACHE_CHECK([whether MPI_Win_set_errhandler is present], [dumpi_cv_have_mpi_win_set_errhandler],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Win win;
            MPI_Errhandler errhandler;
            //
            MPI_Init(aargc, aargv);
            MPI_Win_set_errhandler(win, errhandler);
            PMPI_Win_set_errhandler(win, errhandler);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_win_set_errhandler="yes"
        ], [
          dumpi_cv_have_mpi_win_set_errhandler="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_win_set_errhandler" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_WIN_SET_ERRHANDLER, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_OPEN], [if MPI_File_open is missing])
    AC_CACHE_CHECK([whether MPI_File_open is present], [dumpi_cv_have_mpi_file_open],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_Comm comm;
            char *filename;
            int amode;
            MPI_Info info;
            MPI_File *file;
            //
            MPI_Init(aargc, aargv);
            MPI_File_open(comm, filename, amode, info, file);
            PMPI_File_open(comm, filename, amode, info, file);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_open="yes"
        ], [
          dumpi_cv_have_mpi_file_open="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_open" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_OPEN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_CLOSE], [if MPI_File_close is missing])
    AC_CACHE_CHECK([whether MPI_File_close is present], [dumpi_cv_have_mpi_file_close],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File *file;
            //
            MPI_Init(aargc, aargv);
            MPI_File_close(file);
            PMPI_File_close(file);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_close="yes"
        ], [
          dumpi_cv_have_mpi_file_close="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_close" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_CLOSE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_DELETE], [if MPI_File_delete is missing])
    AC_CACHE_CHECK([whether MPI_File_delete is present], [dumpi_cv_have_mpi_file_delete],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *filename;
            MPI_Info info;
            //
            MPI_Init(aargc, aargv);
            MPI_File_delete(filename, info);
            PMPI_File_delete(filename, info);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_delete="yes"
        ], [
          dumpi_cv_have_mpi_file_delete="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_delete" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_DELETE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_SET_SIZE], [if MPI_File_set_size is missing])
    AC_CACHE_CHECK([whether MPI_File_set_size is present], [dumpi_cv_have_mpi_file_set_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset size;
            //
            MPI_Init(aargc, aargv);
            MPI_File_set_size(file, size);
            PMPI_File_set_size(file, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_set_size="yes"
        ], [
          dumpi_cv_have_mpi_file_set_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_set_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_SET_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_PREALLOCATE], [if MPI_File_preallocate is missing])
    AC_CACHE_CHECK([whether MPI_File_preallocate is present], [dumpi_cv_have_mpi_file_preallocate],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset size;
            //
            MPI_Init(aargc, aargv);
            MPI_File_preallocate(file, size);
            PMPI_File_preallocate(file, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_preallocate="yes"
        ], [
          dumpi_cv_have_mpi_file_preallocate="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_preallocate" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_PREALLOCATE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_SIZE], [if MPI_File_get_size is missing])
    AC_CACHE_CHECK([whether MPI_File_get_size is present], [dumpi_cv_have_mpi_file_get_size],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset *size;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_size(file, size);
            PMPI_File_get_size(file, size);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_size="yes"
        ], [
          dumpi_cv_have_mpi_file_get_size="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_size" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_SIZE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_GROUP], [if MPI_File_get_group is missing])
    AC_CACHE_CHECK([whether MPI_File_get_group is present], [dumpi_cv_have_mpi_file_get_group],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Group *group;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_group(file, group);
            PMPI_File_get_group(file, group);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_group="yes"
        ], [
          dumpi_cv_have_mpi_file_get_group="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_group" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_GROUP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_AMODE], [if MPI_File_get_amode is missing])
    AC_CACHE_CHECK([whether MPI_File_get_amode is present], [dumpi_cv_have_mpi_file_get_amode],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            int *amode;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_amode(file, amode);
            PMPI_File_get_amode(file, amode);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_amode="yes"
        ], [
          dumpi_cv_have_mpi_file_get_amode="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_amode" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_AMODE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_SET_INFO], [if MPI_File_set_info is missing])
    AC_CACHE_CHECK([whether MPI_File_set_info is present], [dumpi_cv_have_mpi_file_set_info],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Info info;
            //
            MPI_Init(aargc, aargv);
            MPI_File_set_info(file, info);
            PMPI_File_set_info(file, info);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_set_info="yes"
        ], [
          dumpi_cv_have_mpi_file_set_info="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_set_info" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_SET_INFO, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_INFO], [if MPI_File_get_info is missing])
    AC_CACHE_CHECK([whether MPI_File_get_info is present], [dumpi_cv_have_mpi_file_get_info],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Info *info;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_info(file, info);
            PMPI_File_get_info(file, info);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_info="yes"
        ], [
          dumpi_cv_have_mpi_file_get_info="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_info" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_INFO, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_SET_VIEW], [if MPI_File_set_view is missing])
    AC_CACHE_CHECK([whether MPI_File_set_view is present], [dumpi_cv_have_mpi_file_set_view],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            MPI_Datatype hosttype;
            MPI_Datatype filetype;
            char *datarep;
            MPI_Info info;
            //
            MPI_Init(aargc, aargv);
            MPI_File_set_view(file, offset, hosttype, filetype, datarep, info);
            PMPI_File_set_view(file, offset, hosttype, filetype, datarep, info);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_set_view="yes"
        ], [
          dumpi_cv_have_mpi_file_set_view="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_set_view" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_SET_VIEW, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_VIEW], [if MPI_File_get_view is missing])
    AC_CACHE_CHECK([whether MPI_File_get_view is present], [dumpi_cv_have_mpi_file_get_view],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset *offset;
            MPI_Datatype *hosttype;
            MPI_Datatype *filetype;
            char *datarep;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_view(file, offset, hosttype, filetype, datarep);
            PMPI_File_get_view(file, offset, hosttype, filetype, datarep);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_view="yes"
        ], [
          dumpi_cv_have_mpi_file_get_view="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_view" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_VIEW, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_AT], [if MPI_File_read_at is missing])
    AC_CACHE_CHECK([whether MPI_File_read_at is present], [dumpi_cv_have_mpi_file_read_at],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_at(file, offset, buf, count, datatype, status);
            PMPI_File_read_at(file, offset, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_at="yes"
        ], [
          dumpi_cv_have_mpi_file_read_at="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_at" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_AT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_AT_ALL], [if MPI_File_read_at_all is missing])
    AC_CACHE_CHECK([whether MPI_File_read_at_all is present], [dumpi_cv_have_mpi_file_read_at_all],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_at_all(file, offset, buf, count, datatype, status);
            PMPI_File_read_at_all(file, offset, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_at_all="yes"
        ], [
          dumpi_cv_have_mpi_file_read_at_all="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_at_all" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_AT_ALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_AT], [if MPI_File_write_at is missing])
    AC_CACHE_CHECK([whether MPI_File_write_at is present], [dumpi_cv_have_mpi_file_write_at],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_at(file, offset, buf, count, datatype, status);
            PMPI_File_write_at(file, offset, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_at="yes"
        ], [
          dumpi_cv_have_mpi_file_write_at="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_at" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_AT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL], [if MPI_File_write_at_all is missing])
    AC_CACHE_CHECK([whether MPI_File_write_at_all is present], [dumpi_cv_have_mpi_file_write_at_all],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_at_all(file, offset, buf, count, datatype, status);
            PMPI_File_write_at_all(file, offset, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_at_all="yes"
        ], [
          dumpi_cv_have_mpi_file_write_at_all="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_at_all" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_IREAD_AT], [if MPI_File_iread_at is missing])
    AC_CACHE_CHECK([whether MPI_File_iread_at is present], [dumpi_cv_have_mpi_file_iread_at],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPIO_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_File_iread_at(file, offset, buf, count, datatype, request);
            PMPI_File_iread_at(file, offset, buf, count, datatype, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_iread_at="yes"
        ], [
          dumpi_cv_have_mpi_file_iread_at="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_iread_at" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_IREAD_AT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_IWRITE_AT], [if MPI_File_iwrite_at is missing])
    AC_CACHE_CHECK([whether MPI_File_iwrite_at is present], [dumpi_cv_have_mpi_file_iwrite_at],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPIO_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_File_iwrite_at(file, offset, buf, count, datatype, request);
            PMPI_File_iwrite_at(file, offset, buf, count, datatype, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_iwrite_at="yes"
        ], [
          dumpi_cv_have_mpi_file_iwrite_at="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_iwrite_at" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_IWRITE_AT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ], [if MPI_File_read is missing])
    AC_CACHE_CHECK([whether MPI_File_read is present], [dumpi_cv_have_mpi_file_read],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read(file, buf, count, datatype, status);
            PMPI_File_read(file, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read="yes"
        ], [
          dumpi_cv_have_mpi_file_read="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_ALL], [if MPI_File_read_all is missing])
    AC_CACHE_CHECK([whether MPI_File_read_all is present], [dumpi_cv_have_mpi_file_read_all],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_all(file, buf, count, datatype, status);
            PMPI_File_read_all(file, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_all="yes"
        ], [
          dumpi_cv_have_mpi_file_read_all="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_all" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_ALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE], [if MPI_File_write is missing])
    AC_CACHE_CHECK([whether MPI_File_write is present], [dumpi_cv_have_mpi_file_write],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write(file, buf, count, datatype, status);
            PMPI_File_write(file, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write="yes"
        ], [
          dumpi_cv_have_mpi_file_write="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_ALL], [if MPI_File_write_all is missing])
    AC_CACHE_CHECK([whether MPI_File_write_all is present], [dumpi_cv_have_mpi_file_write_all],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_all(file, buf, count, datatype, status);
            PMPI_File_write_all(file, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_all="yes"
        ], [
          dumpi_cv_have_mpi_file_write_all="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_all" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_ALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_IREAD], [if MPI_File_iread is missing])
    AC_CACHE_CHECK([whether MPI_File_iread is present], [dumpi_cv_have_mpi_file_iread],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPIO_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_File_iread(file, buf, count, datatype, request);
            PMPI_File_iread(file, buf, count, datatype, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_iread="yes"
        ], [
          dumpi_cv_have_mpi_file_iread="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_iread" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_IREAD, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_IWRITE], [if MPI_File_iwrite is missing])
    AC_CACHE_CHECK([whether MPI_File_iwrite is present], [dumpi_cv_have_mpi_file_iwrite],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPIO_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_File_iwrite(file, buf, count, datatype, request);
            PMPI_File_iwrite(file, buf, count, datatype, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_iwrite="yes"
        ], [
          dumpi_cv_have_mpi_file_iwrite="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_iwrite" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_IWRITE, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_SEEK], [if MPI_File_seek is missing])
    AC_CACHE_CHECK([whether MPI_File_seek is present], [dumpi_cv_have_mpi_file_seek],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            int whence;
            //
            MPI_Init(aargc, aargv);
            MPI_File_seek(file, offset, whence);
            PMPI_File_seek(file, offset, whence);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_seek="yes"
        ], [
          dumpi_cv_have_mpi_file_seek="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_seek" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_SEEK, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_POSITION], [if MPI_File_get_position is missing])
    AC_CACHE_CHECK([whether MPI_File_get_position is present], [dumpi_cv_have_mpi_file_get_position],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset *offset;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_position(file, offset);
            PMPI_File_get_position(file, offset);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_position="yes"
        ], [
          dumpi_cv_have_mpi_file_get_position="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_position" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_POSITION, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_BYTE_OFFSET], [if MPI_File_get_byte_offset is missing])
    AC_CACHE_CHECK([whether MPI_File_get_byte_offset is present], [dumpi_cv_have_mpi_file_get_byte_offset],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            MPI_Offset *bytes;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_byte_offset(file, offset, bytes);
            PMPI_File_get_byte_offset(file, offset, bytes);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_byte_offset="yes"
        ], [
          dumpi_cv_have_mpi_file_get_byte_offset="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_byte_offset" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_BYTE_OFFSET, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_SHARED], [if MPI_File_read_shared is missing])
    AC_CACHE_CHECK([whether MPI_File_read_shared is present], [dumpi_cv_have_mpi_file_read_shared],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_shared(file, buf, count, datatype, status);
            PMPI_File_read_shared(file, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_shared="yes"
        ], [
          dumpi_cv_have_mpi_file_read_shared="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_shared" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_SHARED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_SHARED], [if MPI_File_write_shared is missing])
    AC_CACHE_CHECK([whether MPI_File_write_shared is present], [dumpi_cv_have_mpi_file_write_shared],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_shared(file, buf, count, datatype, status);
            PMPI_File_write_shared(file, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_shared="yes"
        ], [
          dumpi_cv_have_mpi_file_write_shared="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_shared" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_SHARED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_IREAD_SHARED], [if MPI_File_iread_shared is missing])
    AC_CACHE_CHECK([whether MPI_File_iread_shared is present], [dumpi_cv_have_mpi_file_iread_shared],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPIO_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_File_iread_shared(file, buf, count, datatype, request);
            PMPI_File_iread_shared(file, buf, count, datatype, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_iread_shared="yes"
        ], [
          dumpi_cv_have_mpi_file_iread_shared="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_iread_shared" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_IREAD_SHARED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_IWRITE_SHARED], [if MPI_File_iwrite_shared is missing])
    AC_CACHE_CHECK([whether MPI_File_iwrite_shared is present], [dumpi_cv_have_mpi_file_iwrite_shared],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPIO_Request *request;
            //
            MPI_Init(aargc, aargv);
            MPI_File_iwrite_shared(file, buf, count, datatype, request);
            PMPI_File_iwrite_shared(file, buf, count, datatype, request);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_iwrite_shared="yes"
        ], [
          dumpi_cv_have_mpi_file_iwrite_shared="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_iwrite_shared" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_IWRITE_SHARED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_ORDERED], [if MPI_File_read_ordered is missing])
    AC_CACHE_CHECK([whether MPI_File_read_ordered is present], [dumpi_cv_have_mpi_file_read_ordered],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_ordered(file, buf, count, datatype, status);
            PMPI_File_read_ordered(file, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_ordered="yes"
        ], [
          dumpi_cv_have_mpi_file_read_ordered="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_ordered" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_ORDERED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_ORDERED], [if MPI_File_write_ordered is missing])
    AC_CACHE_CHECK([whether MPI_File_write_ordered is present], [dumpi_cv_have_mpi_file_write_ordered],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_ordered(file, buf, count, datatype, status);
            PMPI_File_write_ordered(file, buf, count, datatype, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_ordered="yes"
        ], [
          dumpi_cv_have_mpi_file_write_ordered="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_ordered" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_ORDERED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_SEEK_SHARED], [if MPI_File_seek_shared is missing])
    AC_CACHE_CHECK([whether MPI_File_seek_shared is present], [dumpi_cv_have_mpi_file_seek_shared],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            int whence;
            //
            MPI_Init(aargc, aargv);
            MPI_File_seek_shared(file, offset, whence);
            PMPI_File_seek_shared(file, offset, whence);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_seek_shared="yes"
        ], [
          dumpi_cv_have_mpi_file_seek_shared="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_seek_shared" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_SEEK_SHARED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_POSITION_SHARED], [if MPI_File_get_position_shared is missing])
    AC_CACHE_CHECK([whether MPI_File_get_position_shared is present], [dumpi_cv_have_mpi_file_get_position_shared],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset *offset;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_position_shared(file, offset);
            PMPI_File_get_position_shared(file, offset);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_position_shared="yes"
        ], [
          dumpi_cv_have_mpi_file_get_position_shared="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_position_shared" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_POSITION_SHARED, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_AT_ALL_BEGIN], [if MPI_File_read_at_all_begin is missing])
    AC_CACHE_CHECK([whether MPI_File_read_at_all_begin is present], [dumpi_cv_have_mpi_file_read_at_all_begin],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            void *buf;
            int count;
            MPI_Datatype datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_at_all_begin(file, offset, buf, count, datatype);
            PMPI_File_read_at_all_begin(file, offset, buf, count, datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_at_all_begin="yes"
        ], [
          dumpi_cv_have_mpi_file_read_at_all_begin="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_at_all_begin" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_AT_ALL_BEGIN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_AT_ALL_END], [if MPI_File_read_at_all_end is missing])
    AC_CACHE_CHECK([whether MPI_File_read_at_all_end is present], [dumpi_cv_have_mpi_file_read_at_all_end],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_at_all_end(file, buf, status);
            PMPI_File_read_at_all_end(file, buf, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_at_all_end="yes"
        ], [
          dumpi_cv_have_mpi_file_read_at_all_end="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_at_all_end" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_AT_ALL_END, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL_BEGIN], [if MPI_File_write_at_all_begin is missing])
    AC_CACHE_CHECK([whether MPI_File_write_at_all_begin is present], [dumpi_cv_have_mpi_file_write_at_all_begin],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Offset offset;
            void *buf;
            int count;
            MPI_Datatype datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_at_all_begin(file, offset, buf, count, datatype);
            PMPI_File_write_at_all_begin(file, offset, buf, count, datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_at_all_begin="yes"
        ], [
          dumpi_cv_have_mpi_file_write_at_all_begin="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_at_all_begin" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL_BEGIN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL_END], [if MPI_File_write_at_all_end is missing])
    AC_CACHE_CHECK([whether MPI_File_write_at_all_end is present], [dumpi_cv_have_mpi_file_write_at_all_end],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_at_all_end(file, buf, status);
            PMPI_File_write_at_all_end(file, buf, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_at_all_end="yes"
        ], [
          dumpi_cv_have_mpi_file_write_at_all_end="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_at_all_end" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_AT_ALL_END, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_ALL_BEGIN], [if MPI_File_read_all_begin is missing])
    AC_CACHE_CHECK([whether MPI_File_read_all_begin is present], [dumpi_cv_have_mpi_file_read_all_begin],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_all_begin(file, buf, count, datatype);
            PMPI_File_read_all_begin(file, buf, count, datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_all_begin="yes"
        ], [
          dumpi_cv_have_mpi_file_read_all_begin="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_all_begin" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_ALL_BEGIN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_ALL_END], [if MPI_File_read_all_end is missing])
    AC_CACHE_CHECK([whether MPI_File_read_all_end is present], [dumpi_cv_have_mpi_file_read_all_end],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_all_end(file, buf, status);
            PMPI_File_read_all_end(file, buf, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_all_end="yes"
        ], [
          dumpi_cv_have_mpi_file_read_all_end="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_all_end" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_ALL_END, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_ALL_BEGIN], [if MPI_File_write_all_begin is missing])
    AC_CACHE_CHECK([whether MPI_File_write_all_begin is present], [dumpi_cv_have_mpi_file_write_all_begin],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_all_begin(file, buf, count, datatype);
            PMPI_File_write_all_begin(file, buf, count, datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_all_begin="yes"
        ], [
          dumpi_cv_have_mpi_file_write_all_begin="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_all_begin" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_ALL_BEGIN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_ALL_END], [if MPI_File_write_all_end is missing])
    AC_CACHE_CHECK([whether MPI_File_write_all_end is present], [dumpi_cv_have_mpi_file_write_all_end],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_all_end(file, buf, status);
            PMPI_File_write_all_end(file, buf, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_all_end="yes"
        ], [
          dumpi_cv_have_mpi_file_write_all_end="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_all_end" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_ALL_END, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_ORDERED_BEGIN], [if MPI_File_read_ordered_begin is missing])
    AC_CACHE_CHECK([whether MPI_File_read_ordered_begin is present], [dumpi_cv_have_mpi_file_read_ordered_begin],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_ordered_begin(file, buf, count, datatype);
            PMPI_File_read_ordered_begin(file, buf, count, datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_ordered_begin="yes"
        ], [
          dumpi_cv_have_mpi_file_read_ordered_begin="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_ordered_begin" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_ORDERED_BEGIN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_READ_ORDERED_END], [if MPI_File_read_ordered_end is missing])
    AC_CACHE_CHECK([whether MPI_File_read_ordered_end is present], [dumpi_cv_have_mpi_file_read_ordered_end],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_read_ordered_end(file, buf, status);
            PMPI_File_read_ordered_end(file, buf, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_read_ordered_end="yes"
        ], [
          dumpi_cv_have_mpi_file_read_ordered_end="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_read_ordered_end" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_READ_ORDERED_END, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_ORDERED_BEGIN], [if MPI_File_write_ordered_begin is missing])
    AC_CACHE_CHECK([whether MPI_File_write_ordered_begin is present], [dumpi_cv_have_mpi_file_write_ordered_begin],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            int count;
            MPI_Datatype datatype;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_ordered_begin(file, buf, count, datatype);
            PMPI_File_write_ordered_begin(file, buf, count, datatype);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_ordered_begin="yes"
        ], [
          dumpi_cv_have_mpi_file_write_ordered_begin="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_ordered_begin" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_ORDERED_BEGIN, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_WRITE_ORDERED_END], [if MPI_File_write_ordered_end is missing])
    AC_CACHE_CHECK([whether MPI_File_write_ordered_end is present], [dumpi_cv_have_mpi_file_write_ordered_end],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            void *buf;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPI_File_write_ordered_end(file, buf, status);
            PMPI_File_write_ordered_end(file, buf, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_write_ordered_end="yes"
        ], [
          dumpi_cv_have_mpi_file_write_ordered_end="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_write_ordered_end" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_WRITE_ORDERED_END, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_TYPE_EXTENT], [if MPI_File_get_type_extent is missing])
    AC_CACHE_CHECK([whether MPI_File_get_type_extent is present], [dumpi_cv_have_mpi_file_get_type_extent],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            MPI_Datatype datatype;
            MPI_Aint *extent;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_type_extent(file, datatype, extent);
            PMPI_File_get_type_extent(file, datatype, extent);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_type_extent="yes"
        ], [
          dumpi_cv_have_mpi_file_get_type_extent="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_type_extent" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_TYPE_EXTENT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_REGISTER_DATAREP], [if MPI_Register_datarep is missing])
    AC_CACHE_CHECK([whether MPI_Register_datarep is present], [dumpi_cv_have_mpi_register_datarep],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            char *name;
            MPI_Datarep_conversion_function *readfun;
            MPI_Datarep_conversion_function *writefun;
            MPI_Datarep_extent_function *extentfun;
            void *state;
            //
            MPI_Init(aargc, aargv);
            MPI_Register_datarep(name, readfun, writefun, extentfun, state);
            PMPI_Register_datarep(name, readfun, writefun, extentfun, state);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_register_datarep="yes"
        ], [
          dumpi_cv_have_mpi_register_datarep="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_register_datarep" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_REGISTER_DATAREP, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_SET_ATOMICITY], [if MPI_File_set_atomicity is missing])
    AC_CACHE_CHECK([whether MPI_File_set_atomicity is present], [dumpi_cv_have_mpi_file_set_atomicity],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            int flag;
            //
            MPI_Init(aargc, aargv);
            MPI_File_set_atomicity(file, flag);
            PMPI_File_set_atomicity(file, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_set_atomicity="yes"
        ], [
          dumpi_cv_have_mpi_file_set_atomicity="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_set_atomicity" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_SET_ATOMICITY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_GET_ATOMICITY], [if MPI_File_get_atomicity is missing])
    AC_CACHE_CHECK([whether MPI_File_get_atomicity is present], [dumpi_cv_have_mpi_file_get_atomicity],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            int *flag;
            //
            MPI_Init(aargc, aargv);
            MPI_File_get_atomicity(file, flag);
            PMPI_File_get_atomicity(file, flag);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_get_atomicity="yes"
        ], [
          dumpi_cv_have_mpi_file_get_atomicity="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_get_atomicity" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_GET_ATOMICITY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPI_FILE_SYNC], [if MPI_File_sync is missing])
    AC_CACHE_CHECK([whether MPI_File_sync is present], [dumpi_cv_have_mpi_file_sync],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPI_File file;
            //
            MPI_Init(aargc, aargv);
            MPI_File_sync(file);
            PMPI_File_sync(file);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpi_file_sync="yes"
        ], [
          dumpi_cv_have_mpi_file_sync="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpi_file_sync" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPI_FILE_SYNC, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPIO_TEST], [if MPIO_Test is missing])
    AC_CACHE_CHECK([whether MPIO_Test is present], [dumpi_cv_have_mpio_test],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPIO_Request *request;
            int *flag;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPIO_Test(request, flag, status);
            PMPIO_Test(request, flag, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpio_test="yes"
        ], [
          dumpi_cv_have_mpio_test="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpio_test" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPIO_TEST, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPIO_TESTANY], [if MPIO_Testany is missing])
    AC_CACHE_CHECK([whether MPIO_Testany is present], [dumpi_cv_have_mpio_testany],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPIO_Request *requests;
            int *index;
            int *flag;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPIO_Testany(count, requests, index, flag, status);
            PMPIO_Testany(count, requests, index, flag, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpio_testany="yes"
        ], [
          dumpi_cv_have_mpio_testany="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpio_testany" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPIO_TESTANY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPIO_TESTSOME], [if MPIO_Testsome is missing])
    AC_CACHE_CHECK([whether MPIO_Testsome is present], [dumpi_cv_have_mpio_testsome],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPIO_Request *requests;
            int *outcount;
            int *indices;
            MPI_Status *statuses;
            //
            MPI_Init(aargc, aargv);
            MPIO_Testsome(count, requests, outcount, indices, statuses);
            PMPIO_Testsome(count, requests, outcount, indices, statuses);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpio_testsome="yes"
        ], [
          dumpi_cv_have_mpio_testsome="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpio_testsome" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPIO_TESTSOME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPIO_TESTALL], [if MPIO_Testall is missing])
    AC_CACHE_CHECK([whether MPIO_Testall is present], [dumpi_cv_have_mpio_testall],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPIO_Request *requests;
            int *flag;
            MPI_Status *statuses;
            //
            MPI_Init(aargc, aargv);
            MPIO_Testall(count, requests, flag, statuses);
            PMPIO_Testall(count, requests, flag, statuses);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpio_testall="yes"
        ], [
          dumpi_cv_have_mpio_testall="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpio_testall" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPIO_TESTALL, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPIO_WAIT], [if MPIO_Wait is missing])
    AC_CACHE_CHECK([whether MPIO_Wait is present], [dumpi_cv_have_mpio_wait],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            MPIO_Request *request;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPIO_Wait(request, status);
            PMPIO_Wait(request, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpio_wait="yes"
        ], [
          dumpi_cv_have_mpio_wait="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpio_wait" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPIO_WAIT, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPIO_WAITANY], [if MPIO_Waitany is missing])
    AC_CACHE_CHECK([whether MPIO_Waitany is present], [dumpi_cv_have_mpio_waitany],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPIO_Request *requests;
            int *index;
            MPI_Status *status;
            //
            MPI_Init(aargc, aargv);
            MPIO_Waitany(count, requests, index, status);
            PMPIO_Waitany(count, requests, index, status);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpio_waitany="yes"
        ], [
          dumpi_cv_have_mpio_waitany="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpio_waitany" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPIO_WAITANY, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPIO_WAITSOME], [if MPIO_Waitsome is missing])
    AC_CACHE_CHECK([whether MPIO_Waitsome is present], [dumpi_cv_have_mpio_waitsome],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPIO_Request *requests;
            int *outcount;
            int *indices;
            MPI_Status *statuses;
            //
            MPI_Init(aargc, aargv);
            MPIO_Waitsome(count, requests, outcount, indices, statuses);
            PMPIO_Waitsome(count, requests, outcount, indices, statuses);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpio_waitsome="yes"
        ], [
          dumpi_cv_have_mpio_waitsome="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpio_waitsome" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPIO_WAITSOME, [1])
    fi

    AH_TEMPLATE([DUMPI_SKIP_MPIO_WAITALL], [if MPIO_Waitall is missing])
    AC_CACHE_CHECK([whether MPIO_Waitall is present], [dumpi_cv_have_mpio_waitall],
      [AC_LINK_IFELSE(
        [AC_LANG_PROGRAM(
          [
            #include <mpi.h>
          ], [
            int *aargc;
            char ***aargv;
            int count;
            MPIO_Request *requests;
            MPI_Status *statuses;
            //
            MPI_Init(aargc, aargv);
            MPIO_Waitall(count, requests, statuses);
            PMPIO_Waitall(count, requests, statuses);
            MPI_Finalize();
          ])],
        [ 
          dumpi_cv_have_mpio_waitall="yes"
        ], [
          dumpi_cv_have_mpio_waitall="no"
        ]
      )]
    )
    if test "$dumpi_cv_have_mpio_waitall" = "no"; then
      AC_DEFINE(DUMPI_SKIP_MPIO_WAITALL, [1])
    fi

    AC_LANG_POP
  ]
)

# END OF ORIGINAL DATA
