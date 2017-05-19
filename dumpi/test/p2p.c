/**
Copyright 2009-2017 National Technology and Engineering Solutions of Sandia, 
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S.  Government 
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly 
owned subsidiary of Honeywell International, Inc., for the U.S. Department of 
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2017, NTESS

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of Sandia Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Questions? Contact sst-macro-help@sandia.gov
*/

#include <dumpi/test/p2p.h>
#include <dumpi/dumpiconfig.h>
#include <mpi.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Run through all p2p tests (sends, isends, deferred_sends, sendrecvs) */
void all_p2p(int rank, int size) {
  sends(rank, size);
  isends(rank, size);
  /*deferred_sends(rank, size);*/
  /*sendrecvs(rank, size);*/
}

#define DUMPI_TEST_SEND(rank, size, XC_TYPE, XMPI_TYPE) do {           \
  int i, bsize;							 \
  XC_TYPE sendarr[1024];					 \
  XC_TYPE *recvarr;						 \
  MPI_Request *req;						 \
  XC_TYPE *scratch;						 \
  bsize = 1024 * size * sizeof(XC_TYPE);			 \
  scratch = (XC_TYPE*)malloc(bsize);				 \
  assert(scratch != NULL);					 \
  MPI_Buffer_attach(scratch, bsize);				 \
  recvarr = (XC_TYPE*)malloc(bsize);				 \
  assert(recvarr != NULL);					 \
  for(i = 0; i < 1024; ++i) sendarr[i] = (XC_TYPE)i;		 \
  req = (MPI_Request*)malloc(size * sizeof(MPI_Request));	 \
  assert(req != NULL);						 \
  for(i = 0; i < size; ++i) req[i] = MPI_REQUEST_NULL;		 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Irecv(recvarr + 1024*i, 1024, XMPI_TYPE, i,		 \
	        1, MPI_COMM_WORLD, req+i);			 \
    }								 \
  }								 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {					         \
      MPI_Send(sendarr, 1024, XMPI_TYPE, i, 1, MPI_COMM_WORLD);  \
    }								 \
  }								 \
  MPI_Waitall(size, req, MPI_STATUSES_IGNORE);			 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Irecv(recvarr + 1024*i, 1024, XMPI_TYPE, i,		 \
	        1, MPI_COMM_WORLD, req+i);			 \
    }								 \
  }								 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {					         \
      MPI_Bsend(sendarr, 1024, XMPI_TYPE, i, 1, MPI_COMM_WORLD); \
    }								 \
  }								 \
  MPI_Waitall(size, req, MPI_STATUSES_IGNORE);			 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Irecv(recvarr + 1024*i, 1024, XMPI_TYPE, i,		 \
	        1, MPI_COMM_WORLD, req+i);			 \
    }								 \
  }								 \
  MPI_Barrier(MPI_COMM_WORLD);					 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {					         \
      MPI_Rsend(sendarr, 1024, XMPI_TYPE, i, 1, MPI_COMM_WORLD); \
    }								 \
  }								 \
  MPI_Waitall(size, req, MPI_STATUSES_IGNORE);			 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Irecv(recvarr + 1024*i, 1024, XMPI_TYPE, i,		 \
	        1, MPI_COMM_WORLD, req+i);			 \
    }								 \
  }								 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {					         \
      MPI_Ssend(sendarr, 1024, XMPI_TYPE, i, 1, MPI_COMM_WORLD); \
    }								 \
  }								 \
  MPI_Waitall(size, req, MPI_STATUSES_IGNORE);			 \
  MPI_Barrier(MPI_COMM_WORLD);					 \
  MPI_Buffer_detach(&scratch, &bsize);				 \
  for(i = 0; i < size; ++i)					 \
    if(req[i] != MPI_REQUEST_NULL)				 \
      MPI_Request_free(req+i);					 \
  free(scratch);		 				 \
  free(recvarr);						 \
  free(req);							 \
} while(0)

/* send, bsend, rsend, ssend, recv */
void sends(int rank, int size) {
#if DUMPI_HAVE_MPI_CHAR
  DUMPI_TEST_SEND(rank, size, char, MPI_CHAR);
#endif
#if DUMPI_HAVE_MPI_SIGNED_CHAR
  DUMPI_TEST_SEND(rank, size, signed char, MPI_SIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_CHAR
  DUMPI_TEST_SEND(rank, size, unsigned char, MPI_UNSIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_BYTE
  DUMPI_TEST_SEND(rank, size, unsigned char, MPI_BYTE);
#endif
#if DUMPI_HAVE_MPI_WCHAR
  DUMPI_TEST_SEND(rank, size, wchar_t, MPI_WCHAR);
#endif
#if DUMPI_HAVE_MPI_SHORT
  DUMPI_TEST_SEND(rank, size, short, MPI_SHORT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_SHORT
  DUMPI_TEST_SEND(rank, size, unsigned short, MPI_UNSIGNED_SHORT);
#endif
#if DUMPI_HAVE_MPI_INT
  DUMPI_TEST_SEND(rank, size, int, MPI_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED
  DUMPI_TEST_SEND(rank, size, unsigned, MPI_UNSIGNED);
#endif
#if DUMPI_HAVE_MPI_LONG
  DUMPI_TEST_SEND(rank, size, long, MPI_LONG);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG
  DUMPI_TEST_SEND(rank, size, unsigned long, MPI_UNSIGNED_LONG);
#endif
#if DUMPI_HAVE_MPI_FLOAT
  DUMPI_TEST_SEND(rank, size, float, MPI_FLOAT);
#endif
#if DUMPI_HAVE_MPI_DOUBLE
  DUMPI_TEST_SEND(rank, size, double, MPI_DOUBLE);
#endif
  if(MPI_LONG_DOUBLE != MPI_DATATYPE_NULL) {
#if DUMPI_HAVE_MPI_LONG_DOUBLE
    DUMPI_TEST_SEND(rank, size, long double, MPI_LONG_DOUBLE);
#endif
  }
#if DUMPI_HAVE_MPI_LONG_LONG_INT
  DUMPI_TEST_SEND(rank, size, long long, MPI_LONG_LONG_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG
  DUMPI_TEST_SEND(rank, size, unsigned long long, MPI_UNSIGNED_LONG_LONG);
#endif
}

#define DUMPI_TEST_ISEND(rank, size, XC_TYPE, XMPI_TYPE) do {          \
  int i,j, bsize;						 \
  XC_TYPE *sendarr;						 \
  XC_TYPE recvarr[1024];					 \
  MPI_Request *req;						 \
  XC_TYPE *scratch;						 \
  bsize   = size * 2048 * sizeof(XC_TYPE);			 \
  scratch = (XC_TYPE*)malloc(bsize);				 \
  assert(scratch != NULL);					 \
  MPI_Buffer_attach(scratch, bsize);				 \
  sendarr = (XC_TYPE*)malloc(1024 * size * sizeof(XC_TYPE));	 \
  assert(sendarr != NULL);					 \
  for(i = 0; i < size; ++i)					 \
    for(j = 0; j < 1024; ++j)					 \
      sendarr[i*size + j] = (XC_TYPE)i;				 \
  req = (MPI_Request*)malloc(2 * size * sizeof(MPI_Request));	 \
  assert(req != NULL);						 \
  for(i = 0; i < 2*size; ++i) req[i] = MPI_REQUEST_NULL;	 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Isend(sendarr + 1024*i, 1024, XMPI_TYPE, i,		 \
	        1, MPI_COMM_WORLD, req+i);			 \
    }								 \
  }								 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {					         \
      MPI_Recv(recvarr, 1024, XMPI_TYPE, i, 1, MPI_COMM_WORLD,	 \
	       MPI_STATUS_IGNORE);				 \
    }								 \
  }								 \
  MPI_Waitall(size, req, MPI_STATUSES_IGNORE);			 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Ibsend(sendarr + 1024*i, 1024, XMPI_TYPE, i,		 \
	         1, MPI_COMM_WORLD, req+i);			 \
    }								 \
  }								 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {					         \
      MPI_Recv(recvarr, 1024, XMPI_TYPE, i, 1, MPI_COMM_WORLD,	 \
	       MPI_STATUS_IGNORE);				 \
    }								 \
  }								 \
  MPI_Waitall(size, req, MPI_STATUSES_IGNORE);			 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {					         \
      MPI_Irecv(recvarr, 1024, XMPI_TYPE, i, 1, MPI_COMM_WORLD,	 \
	        req + (size+i));				 \
    }								 \
  }								 \
  MPI_Barrier(MPI_COMM_WORLD);					 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Irsend(sendarr + 1024*i, 1024, XMPI_TYPE, i,		 \
	         1, MPI_COMM_WORLD, req+i);			 \
    }								 \
  }								 \
  MPI_Waitall(2*size, req, MPI_STATUSES_IGNORE);		 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Issend(sendarr + 1024*i, 1024, XMPI_TYPE, i,		 \
	         1, MPI_COMM_WORLD, req+i);			 \
    }								 \
  }								 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {					         \
      MPI_Recv(recvarr, 1024, XMPI_TYPE, i, 1, MPI_COMM_WORLD,	 \
	       MPI_STATUS_IGNORE);				 \
    }								 \
  }								 \
  MPI_Waitall(size, req, MPI_STATUSES_IGNORE);			 \
  MPI_Barrier(MPI_COMM_WORLD);					 \
  MPI_Buffer_detach(&scratch, &bsize);				 \
  for(i = 0; i < size; ++i)					 \
    if(req[i] != MPI_REQUEST_NULL)				 \
      MPI_Request_free(req);					 \
  free(sendarr);						 \
  free(req);							 \
  free(scratch);						 \
} while(0)

/* isend, ibsend, irsend, issend, irecv */
void isends(int rank, int size) {
#if DUMPI_HAVE_MPI_CHAR
  DUMPI_TEST_ISEND(rank, size, char, MPI_CHAR);
#endif
#if DUMPI_HAVE_MPI_SIGNED_CHAR
  DUMPI_TEST_ISEND(rank, size, signed char, MPI_SIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_CHAR
  DUMPI_TEST_ISEND(rank, size, unsigned char, MPI_UNSIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_BYTE
  DUMPI_TEST_ISEND(rank, size, unsigned char, MPI_BYTE);
#endif
#if DUMPI_HAVE_MPI_WCHAR
  DUMPI_TEST_ISEND(rank, size, wchar_t, MPI_WCHAR);
#endif
#if DUMPI_HAVE_MPI_SHORT
  DUMPI_TEST_ISEND(rank, size, short, MPI_SHORT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_SHORT
  DUMPI_TEST_ISEND(rank, size, unsigned short, MPI_UNSIGNED_SHORT);
#endif
#if DUMPI_HAVE_MPI_INT
  DUMPI_TEST_ISEND(rank, size, int, MPI_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED
  DUMPI_TEST_ISEND(rank, size, unsigned, MPI_UNSIGNED);
#endif
#if DUMPI_HAVE_MPI_LONG
  DUMPI_TEST_ISEND(rank, size, long, MPI_LONG);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG
  DUMPI_TEST_ISEND(rank, size, unsigned long, MPI_UNSIGNED_LONG);
#endif
#if DUMPI_HAVE_MPI_FLOAT
  DUMPI_TEST_ISEND(rank, size, float, MPI_FLOAT);
#endif
#if DUMPI_HAVE_MPI_DOUBLE
  DUMPI_TEST_ISEND(rank, size, double, MPI_DOUBLE);
#endif
  if(MPI_LONG_DOUBLE != MPI_DATATYPE_NULL) {
#if DUMPI_HAVE_MPI_LONG_DOUBLE
    DUMPI_TEST_ISEND(rank, size, long double, MPI_LONG_DOUBLE);
#endif
  }
#if DUMPI_HAVE_MPI_LONG_LONG_INT
  DUMPI_TEST_ISEND(rank, size, long long, MPI_LONG_LONG_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG
  DUMPI_TEST_ISEND(rank, size, unsigned long long, MPI_UNSIGNED_LONG_LONG);
#endif
}

/* send_init, bsend_init, ssend_init, rsend_init, recv_init, start, startall */
#define DUMPI_TEST_DEFERRED_SEND(rank, size, XC_TYPE, XMPI_TYPE) do {  \
  int i,j, bsize, startid;					 \
  XC_TYPE *sendarr, *recvarr, *scratch;				 \
  MPI_Request *sendreq, *recvreq;				 \
  bsize   = size * 2048 * sizeof(XC_TYPE);			 \
  scratch = (XC_TYPE*)malloc(bsize);				 \
  assert(scratch != NULL);					 \
  MPI_Buffer_attach(scratch, bsize);				 \
  sendarr = (XC_TYPE*)malloc(size * 1024 * sizeof(XC_TYPE));	 \
  recvarr = (XC_TYPE*)malloc(size * 1024 * sizeof(XC_TYPE));	 \
  sendreq = (MPI_Request*)malloc(size * sizeof(MPI_Request));	 \
  recvreq = (MPI_Request*)malloc(size * sizeof(MPI_Request));	 \
  assert(sendarr != NULL && recvarr != NULL);			 \
  assert(sendreq != NULL && recvreq != NULL);			 \
  for(i = 0; i < size; ++i) {					 \
    sendreq[i] = recvreq[i] = MPI_REQUEST_NULL;			 \
    for(j = 0; j < 1024; ++j) {					 \
      sendarr[i*1024 + j] = (XC_TYPE)j;				 \
    }								 \
  }								 \
  /* Send_init */						 \
  startid = 0;							 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Send_init(sendarr, 1024, XMPI_TYPE, i,		 \
		    1, MPI_COMM_WORLD, sendreq+startid);	 \
      MPI_Irecv(recvarr + i*1024, 1024, XMPI_TYPE,		 \
		i, 1, MPI_COMM_WORLD,				 \
		recvreq+i);					 \
      ++startid;						 \
    }								 \
  }								 \
  MPI_Startall(startid, sendreq);				 \
  MPI_Waitall(size, recvreq, MPI_STATUSES_IGNORE);		 \
  /* Bsend_init */						 \
  startid = 0;							 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Bsend_init(sendarr, 1024, XMPI_TYPE, i,		 \
		     1, MPI_COMM_WORLD, sendreq+startid);	 \
      MPI_Irecv(recvarr + i*1024, 1024, XMPI_TYPE, i,		 \
		1, MPI_COMM_WORLD, recvreq+i);			 \
      ++startid;						 \
    }								 \
  }								 \
  MPI_Startall(startid, sendreq);				 \
  MPI_Waitall(size, recvreq, MPI_STATUSES_IGNORE);		 \
  /* Rsend_init */						 \
  startid = 0;							 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Rsend_init(sendarr, 1024, XMPI_TYPE, i,		 \
		     1, MPI_COMM_WORLD, sendreq+startid);	 \
      MPI_Irecv(recvarr + i*1024, 1024, XMPI_TYPE, i,		 \
		1, MPI_COMM_WORLD, recvreq+i);			 \
      ++startid;						 \
    }								 \
  }								 \
  MPI_Barrier(MPI_COMM_WORLD);					 \
  MPI_Startall(startid, sendreq);				 \
  MPI_Waitall(size, recvreq, MPI_STATUSES_IGNORE);		 \
  /* Ssend_init */						 \
  startid = 0;							 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Ssend_init(sendarr, 1024, XMPI_TYPE, i,		 \
		     1, MPI_COMM_WORLD, sendreq+startid);	 \
      MPI_Irecv(recvarr + i*1024, 1024, XMPI_TYPE, i,		 \
		1, MPI_COMM_WORLD, recvreq+i);			 \
      ++startid;						 \
    }								 \
  }								 \
  MPI_Startall(startid, sendreq);				 \
  MPI_Waitall(size, recvreq, MPI_STATUSES_IGNORE);		 \
  /* Recv_init */						 \
  startid = 0;							 \
  for(i = 0; i < size; ++i) {					 \
    if(i != rank) {						 \
      MPI_Isend(sendarr + i*1024, 1024, XMPI_TYPE,		 \
		i, 1, MPI_COMM_WORLD, sendreq+i);		 \
      MPI_Recv_init(recvarr + i*1024, 1024, XMPI_TYPE, i,	 \
		    1, MPI_COMM_WORLD, recvreq+startid);	 \
      ++startid;						 \
    }								 \
  }								 \
  /* Test MPI_Start here (to use the opportunity) */		 \
  for(i = 0; i < startid; ++i) {				 \
    MPI_Start(recvreq+i);					 \
  }								 \
  MPI_Waitall(size, sendreq, MPI_STATUSES_IGNORE);		 \
  MPI_Barrier(MPI_COMM_WORLD);					 \
  /* All done */						 \
  MPI_Buffer_detach(&scratch, &bsize);				 \
  for(i = 0; i < size; ++i) {					 \
    if(sendreq[i] != MPI_REQUEST_NULL)				 \
      MPI_Request_free(sendreq+i);				 \
    if(recvreq[i] != MPI_REQUEST_NULL)				 \
      MPI_Request_free(recvreq+i);				 \
  }								 \
  free(scratch);						 \
  free(sendarr);						 \
  free(recvarr);						 \
  free(sendreq);						 \
  free(recvreq);						 \
} while(0)

/* send_init, bsend_init, ssend_init, rsend_init, recv_init, start, startall */
void deferred_sends(int rank, int size) {
#if DUMPI_HAVE_MPI_CHAR
  DUMPI_TEST_DEFERRED_SEND(rank, size, char, MPI_CHAR);
#endif
#if DUMPI_HAVE_MPI_SIGNED_CHAR
  DUMPI_TEST_DEFERRED_SEND(rank, size, signed char, MPI_SIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_CHAR
  DUMPI_TEST_DEFERRED_SEND(rank, size, unsigned char, MPI_UNSIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_BYTE
  DUMPI_TEST_DEFERRED_SEND(rank, size, unsigned char, MPI_BYTE);
#endif
#if DUMPI_HAVE_MPI_WCHAR
  DUMPI_TEST_DEFERRED_SEND(rank, size, wchar_t, MPI_WCHAR);
#endif
#if DUMPI_HAVE_MPI_SHORT
  DUMPI_TEST_DEFERRED_SEND(rank, size, short, MPI_SHORT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_SHORT
  DUMPI_TEST_DEFERRED_SEND(rank, size, unsigned short, MPI_UNSIGNED_SHORT);
#endif
#if DUMPI_HAVE_MPI_INT
  DUMPI_TEST_DEFERRED_SEND(rank, size, int, MPI_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED
  DUMPI_TEST_DEFERRED_SEND(rank, size, unsigned, MPI_UNSIGNED);
#endif
#if DUMPI_HAVE_MPI_LONG
  DUMPI_TEST_DEFERRED_SEND(rank, size, long, MPI_LONG);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG
  DUMPI_TEST_DEFERRED_SEND(rank, size, unsigned long, MPI_UNSIGNED_LONG);
#endif
#if DUMPI_HAVE_MPI_FLOAT
  DUMPI_TEST_DEFERRED_SEND(rank, size, float, MPI_FLOAT);
#endif
#if DUMPI_HAVE_MPI_DOUBLE
  DUMPI_TEST_DEFERRED_SEND(rank, size, double, MPI_DOUBLE);
#endif
  if(MPI_LONG_DOUBLE != MPI_DATATYPE_NULL) {
  #if DUMPI_HAVE_MPI_LONG_DOUBLE
    DUMPI_TEST_DEFERRED_SEND(rank, size, long double, MPI_LONG_DOUBLE);
#endif
  }
#if DUMPI_HAVE_MPI_LONG_LONG_INT
  DUMPI_TEST_DEFERRED_SEND(rank, size, long long, MPI_LONG_LONG_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG
  DUMPI_TEST_DEFERRED_SEND(rank, size, unsigned long long, MPI_UNSIGNED_LONG_LONG);
#endif
}

#define DUMPI_TEST_SENDRECV(rank, size, XC_TYPE, XMPI_TYPE) do {              \
  int i, source, dest, sendcount, recvcount, sendtag, recvtag;		\
  XC_TYPE *sendbuf, *recvbuf;						\
  sendcount = recvcount = 1024;						\
  sendtag = recvtag = 10;						\
  sendbuf = (XC_TYPE*)malloc(sendcount * sizeof(XC_TYPE));		\
  recvbuf = (XC_TYPE*)malloc(recvcount * sizeof(XC_TYPE));		\
  assert(sendbuf != NULL && recvbuf != NULL);				\
  for(i = 0; i < sendcount; ++i) sendbuf[i] = (XC_TYPE)i;		\
  /* go */								\
  for(dest = 0; dest < size; ++dest) {					\
    if(dest != rank) {							\
      source = dest;							\
      MPI_Sendrecv(sendbuf, sendcount, XMPI_TYPE, dest, sendtag,	\
		   recvbuf, recvcount, XMPI_TYPE, source, recvtag,	\
		   MPI_COMM_WORLD, MPI_STATUS_IGNORE);			\
    }									\
  }									\
  free(sendbuf);							\
  free(recvbuf);							\
} while(0)

#define DUMPI_TEST_SENDRECV_REPLACE(rank, size, XC_TYPE, XMPI_TYPE) do {      \
  int i, source, dest, count, sendtag, recvtag;			        \
  XC_TYPE *buf;								\
  count = 1024;								\
  sendtag = recvtag = 20;						\
  buf = (XC_TYPE*)malloc(count * sizeof(XC_TYPE));			\
  assert(buf != NULL);							\
  for(i = 0; i < count; ++i) buf[i] = (XC_TYPE)(rank+i);		\
  /* go */								\
  for(dest = 0; dest < size; ++dest) {					\
    if(dest != rank) {							\
      source = dest;							\
      MPI_Sendrecv_replace(buf, count, XMPI_TYPE,			\
			   dest, sendtag, source, recvtag,		\
			   MPI_COMM_WORLD, MPI_STATUS_IGNORE);		\
    }									\
  }									\
  free(buf);								\
} while(0)

/* sendrecv, sendrecv_replace */
void sendrecvs(int rank, int size) {
#if DUMPI_HAVE_MPI_CHAR
  DUMPI_TEST_SENDRECV(rank, size, char, MPI_CHAR);
#endif
#if DUMPI_HAVE_MPI_SIGNED_CHAR
  DUMPI_TEST_SENDRECV(rank, size, signed char, MPI_SIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_CHAR
  DUMPI_TEST_SENDRECV(rank, size, unsigned char, MPI_UNSIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_BYTE
  DUMPI_TEST_SENDRECV(rank, size, unsigned char, MPI_BYTE);
#endif
#if DUMPI_HAVE_MPI_WCHAR
  DUMPI_TEST_SENDRECV(rank, size, wchar_t, MPI_WCHAR);
#endif
#if DUMPI_HAVE_MPI_SHORT
  DUMPI_TEST_SENDRECV(rank, size, short, MPI_SHORT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_SHORT
  DUMPI_TEST_SENDRECV(rank, size, unsigned short, MPI_UNSIGNED_SHORT);
#endif
#if DUMPI_HAVE_MPI_INT
  DUMPI_TEST_SENDRECV(rank, size, int, MPI_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED
  DUMPI_TEST_SENDRECV(rank, size, unsigned, MPI_UNSIGNED);
#endif
#if DUMPI_HAVE_MPI_LONG
  DUMPI_TEST_SENDRECV(rank, size, long, MPI_LONG);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG
  DUMPI_TEST_SENDRECV(rank, size, unsigned long, MPI_UNSIGNED_LONG);
#endif
#if DUMPI_HAVE_MPI_FLOAT
  DUMPI_TEST_SENDRECV(rank, size, float, MPI_FLOAT);
#endif
#if DUMPI_HAVE_MPI_DOUBLE
  DUMPI_TEST_SENDRECV(rank, size, double, MPI_DOUBLE);
#endif
  if(MPI_LONG_DOUBLE != MPI_DATATYPE_NULL) {
  #if DUMPI_HAVE_MPI_LONG_DOUBLE
  DUMPI_TEST_SENDRECV(rank, size, long double, MPI_LONG_DOUBLE);
#endif
  }
#if DUMPI_HAVE_MPI_LONG_LONG_INT
  DUMPI_TEST_SENDRECV(rank, size, long long, MPI_LONG_LONG_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG
  DUMPI_TEST_SENDRECV(rank, size, unsigned long long, MPI_UNSIGNED_LONG_LONG);
#endif

#if DUMPI_HAVE_MPI_CHAR
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, char, MPI_CHAR);
#endif
#if DUMPI_HAVE_MPI_SIGNED_CHAR
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, signed char, MPI_SIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_CHAR
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, unsigned char, MPI_UNSIGNED_CHAR);
#endif
#if DUMPI_HAVE_MPI_BYTE
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, unsigned char, MPI_BYTE);
#endif
#if DUMPI_HAVE_MPI_WCHAR
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, wchar_t, MPI_WCHAR);
#endif
#if DUMPI_HAVE_MPI_SHORT
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, short, MPI_SHORT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_SHORT
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, unsigned short, MPI_UNSIGNED_SHORT);
#endif
#if DUMPI_HAVE_MPI_INT
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, int, MPI_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, unsigned, MPI_UNSIGNED);
#endif
#if DUMPI_HAVE_MPI_LONG
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, long, MPI_LONG);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, unsigned long, MPI_UNSIGNED_LONG);
#endif
#if DUMPI_HAVE_MPI_FLOAT
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, float, MPI_FLOAT);
#endif
#if DUMPI_HAVE_MPI_DOUBLE
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, double, MPI_DOUBLE);
#endif
  if(MPI_LONG_DOUBLE != MPI_DATATYPE_NULL) {
  #if DUMPI_HAVE_MPI_LONG_DOUBLE
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, long double, MPI_LONG_DOUBLE);
#endif
  }
#if DUMPI_HAVE_MPI_LONG_LONG_INT
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, long long, MPI_LONG_LONG_INT);
#endif
#if DUMPI_HAVE_MPI_UNSIGNED_LONG_LONG
  DUMPI_TEST_SENDRECV_REPLACE(rank, size, unsigned long long, MPI_UNSIGNED_LONG_LONG);
#endif
}