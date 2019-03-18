/**
Copyright 2009-2018 National Technology and Engineering Solutions of Sandia, 
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S.  Government 
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly 
owned subsidiary of Honeywell International, Inc., for the U.S. Department of 
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2018, NTESS

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of the copyright holder nor the names of its
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

#ifndef DUMPI_BIN_DUMPI2ASCII_DEFS_H
#define DUMPI_BIN_DUMPI2ASCII_DEFS_H

#include <stdio.h>
#include <dumpi/common/perfctrtags.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \defgroup dumpi2ascii dumpi2ascii:  Convert binary DUMPI trace files
   *     to human-readable ASCII files.
   */

  /**
   * \ingroup dumpi2ascii
   * \defgroup dumpi2ascii_defs dumpi2ascii/defs:  Utility functions and \
   *    preprocessor definitions for reporting the contents of an MPI trace.
   */
  /*@{*/

  /**
   * The filehandle to which dumpi2ascii writes its stuff.
   */
  extern FILE *dumpfh;
  extern int rankProcessing;

  /** "Magic" value to indicate that we're dealing with a
   * NUL-terminated std::string */
#define DUMPI_CSTRING -1
#ifndef CSTRING
#define CSTRING DUMPI_CSTRING
#endif

  /** "Magic" value to indicate that we have a NULL-terminated 
   * array of pointers */
#define DUMPI_NULLTERM -2
#ifndef NULLTERM
#define NULLTERM DUMPI_NULLTERM
#endif

  /** Common preamble for profiling methods */
#define DUMPI_ENTERING(METHOD, PRM, THREAD, CPU, WALL, PERF) do { \
    (void)uarg;						    \
    fprintf(dumpfh, "T=  %d.%09d s: MPI Rank %d : " #METHOD  "()\n",       \
           (WALL)->start.sec, (WALL)->start.nsec, rankProcessing);				    \
} while(0)
#ifndef ENTERING
#define ENTERING DUMPI_ENTERING
#endif

  /** Common postamble for profiling methods */
#define DUMPI_RETURNING(METHOD, PRM, THREAD, CPU, WALL, PERF)	    \
  fprintf(dumpfh, "T=  %d.%09d s: MPI Rank %d : " #METHOD "() finished\n",       \
         (WALL)->stop.sec, (WALL)->stop.nsec, rankProcessing);				    			    \
  return 1
#ifndef RETURNING
#define RETURNING DUMPI_RETURNING
#endif

  /** Print PAPI perfcounter information at start of MPI call if available */
#define DUMPI_PUT_PERF_IN(PERF) do {			            \
  if(PERF != NULL && perf->count > 0) {			    \
    int i;						    \
    fprintf(dumpfh, "Perfcounters: [");				    \
    for(i = 0; i < perf->count; ++i) {			    \
      fprintf(dumpfh, "%s=%lld",					    \
       perf->counter_tag[i],                          \
       (long long)perf->invalue[i]);		    \
      if(i < (perf->count-1)) fprintf(dumpfh, ", ");		    \
    }							    \
    fprintf(dumpfh, "]\n");					    \
  }							    \
} while(0)
#ifndef PUT_PERF_IN
#define PUT_PERF_IN DUMPI_PUT_PERF_IN
#endif

  /** Print PAPI perfcounter information at completion of MPI call */
#define DUMPI_PUT_PERF_OUT(PERF) do {			            \
  if(PERF != NULL && perf->count > 0) {			    \
    int i;						    \
    fprintf(dumpfh, "Perfcounters: [");				    \
    for(i = 0; i < perf->count; ++i) {			    \
      fprintf(dumpfh, "%s=%lld",					    \
       perf->counter_tag[i],                          \
       (long long)perf->outvalue[i]);		    \
      if(i < (perf->count-1)) fprintf(dumpfh, ", ");		    \
    }							    \
    fprintf(dumpfh, "]\n");					    \
  }							    \
} while(0)
#ifndef PUT_PERF_OUT
#define PUT_PERF_OUT DUMPI_PUT_PERF_OUT
#endif

#define DUMPI_REPORT_INT(TTAG)

#define DUMPI_REPORT_INT_ARRAY_1(LEN, TTAG)

#define DUMPI_REPORT_INT64_T(TTAG)

#define DUMPI_REPORT_INT_ARRAY_2(X, Y, TTAG)

#define DUMPI_REPORT_CHAR_ARRAY_1(IGNORED, TTAG)

#define DUMPI_REPORT_CHAR_ARRAY_2(LEN, IGNORED, TTAG)

#define DUMPI_REPORT_CHAR_ARRAY_3(X, Y, IGNORED, TTAG)

#define DUMPI_REPORT_COMBINER(TTAG)

#define DUMPI_REPORT_DEST(TTAG)

#define DUMPI_REPORT_DISTRIBUTION_ARRAY_1(LEN, TTAG)

#define DUMPI_REPORT_ERRCODE_ARRAY_1(LEN, TTAG)

#define DUMPI_REPORT_FILE(TTAG)

#define DUMPI_REPORT_INFO(TTAG)

#define DUMPI_REPORT_INFO_ARRAY_1(LEN, TTAG)

#define DUMPI_REPORT_KEYVAL(TTAG)

#define DUMPI_REPORT_COMM_KEYVAL(TTAG)

#define DUMPI_REPORT_TYPE_KEYVAL(TTAG)

#define DUMPI_REPORT_WIN_KEYVAL(TTAG)

#define DUMPI_REPORT_LOCKTYPE(TTAG)

#define DUMPI_REPORT_ERRHANDLER(TTAG)

#define DUMPI_REPORT_FILEMODE(TTAG)

#define DUMPI_REPORT_ORDERING(TTAG)

#define DUMPI_REPORT_SOURCE(TTAG)

#define DUMPI_REPORT_TAG(TTAG)

#define DUMPI_REPORT_THREADLEVEL(TTAG)

#define DUMPI_REPORT_TOPOLOGY(TTAG)

#define DUMPI_REPORT_TYPECLASS(TTAG)

#define DUMPI_REPORT_WIN(TTAG)

#define DUMPI_REPORT_WIN_ASSERT(TTAG)

#define DUMPI_REPORT_STATUS_PTR(TTAG)

#define DUMPI_REPORT_STATUS_ARRAY_1(COUNT, TTAG)

#define DUMPI_STRVAL(TTAG)

#define DUMPI_STRARR(TTAG, LEN)

#define DUMPI_REPORT_COMM(TTAG)

#define DUMPI_REPORT_COMPARISON(TTAG)

#define DUMPI_REPORT_DATATYPE(TTAG)

#define DUMPI_REPORT_DATATYPE_ARRAY_1(COUNT, TTAG)

#define DUMPI_REPORT_GROUP(TTAG)

#define DUMPI_REPORT_OP(TTAG)

#define DUMPI_REPORT_REQUEST(TTAG)

#define DUMPI_REPORT_REQUEST_ARRAY_1(COUNT, TTAG)

#define DUMPIO_REPORT_REQUEST(TTAG)

#define DUMPIO_REPORT_REQUEST_ARRAY_1(COUNT, TTAG)

#define DUMPI_REPORT_WHENCE(TTAG)

#define DUMPI_REPORT_FUNC(FN)

  /*@}*/

#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_BIN_DUMPI2ASCII_DEFS_H */
