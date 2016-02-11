/*
 *  This file is part of DUMPI: 
 *               The MPI profiling library from the SST suite.
 *  Copyright (c) 2009 Sandia Corporation.
 *  This software is distributed under the BSD License.
 *  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
 *  the U.S. Government retains certain rights in this software.
 *  For more information, see the LICENSE file in the top 
 *  SST/macroscale directory.
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
