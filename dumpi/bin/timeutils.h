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

#ifndef DUMPI_BIN_TIMEUTILS_H
#define DUMPI_BIN_TIMEUTILS_H

// Simple convenience wrappings around dumpi_clock.
#include <dumpi/common/types.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

namespace dumpi {

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  inline void clear(dumpi_clock &cc) {
    cc.sec = cc.nsec = 0;
  }

  inline std::ostream& operator<<(std::ostream &os, const dumpi_clock &cc) {
    // Need the std::stringstream here to make sure 'setw' works reasonably
    // for the caller.
    std::stringstream ss;
    ss << cc.sec << "." << std::setw(9) << std::setfill('0') << cc.nsec;
    os << ss.str();
    return os;
  }

  inline bool operator<(const dumpi_clock &a, const dumpi_clock &b) {
    if(a.sec != b.sec) return a.sec < b.sec;
    else return a.nsec < b.nsec;
  }

  inline bool operator<=(const dumpi_clock &a, const dumpi_clock &b) {
    if(a.sec != b.sec) return a.sec < b.sec;
    else return a.nsec <= b.nsec;
  }

  inline bool operator>(const dumpi_clock &a, const dumpi_clock &b) {
    if(a.sec != b.sec) return a.sec > b.sec;
    else return a.nsec > b.nsec;
  }

  inline bool operator>=(const dumpi_clock &a, const dumpi_clock &b) {
    if(a.sec != b.sec) return a.sec > b.sec;
    else return a.nsec >= b.nsec;
  }

  inline bool operator==(const dumpi_clock &a, const dumpi_clock &b) {
    return (a.sec == b.sec && a.nsec == b.nsec);
  }

  inline bool operator!=(const dumpi_clock &a, const dumpi_clock &b) {
    return (a.sec != b.sec && a.nsec != b.nsec);
  }

  inline dumpi_clock operator-(const dumpi_clock &a, const dumpi_clock &b) {
    static const int64_t scaling(1e9);
    int64_t aa = a.nsec + a.sec * scaling;
    int64_t bb = b.nsec + b.sec * scaling;
    int64_t cc = aa - bb;
    dumpi_clock rv = dumpi_clock_init_scale(cc, scaling);
    if(cc < 0) {
      if(rv.sec != 0) rv.nsec = abs(rv.nsec);
      else rv.nsec = -abs(rv.nsec);
    }
    return rv;
  }
  
  inline dumpi_clock operator+(const dumpi_clock &a, const dumpi_clock &b) {
    static const int64_t scaling(1e9);
    int64_t aa = a.nsec + a.sec * scaling;
    int64_t bb = b.nsec + b.sec * scaling;
    int64_t cc = aa + bb;
    dumpi_clock rv = dumpi_clock_init_scale(cc, scaling);
    if(cc < 0) {
      if(rv.sec != 0) rv.nsec = abs(rv.nsec);
      else rv.nsec = -abs(rv.nsec);
    }
    return rv;
  }
  
  inline dumpi_clock& operator+=(dumpi_clock &a, const dumpi_clock &b) {
    dumpi_clock tmp = a + b;
    a = tmp;
    return a;
  }

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_TIMEUTILS_H