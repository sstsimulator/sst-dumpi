/**
Copyright 2009-2022 National Technology and Engineering Solutions of Sandia,
LLC (NTESS).  Under the terms of Contract DE-NA-0003525, the U.S. Government
retains certain rights in this software.

Sandia National Laboratories is a multimission laboratory managed and operated
by National Technology and Engineering Solutions of Sandia, LLC., a wholly
owned subsidiary of Honeywell International, Inc., for the U.S. Department of
Energy's National Nuclear Security Administration under contract DE-NA0003525.

Copyright (c) 2009-2022, NTESS

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

#ifndef DUMPI_BIN_METADATA_H
#define DUMPI_BIN_METADATA_H

#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdexcept>

namespace dumpi {

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  /**
   * Maintain metadata from a dumpi metafile.
   * Intended for C++-based utilities for parsing DUMPI information.
   */
  class metadata {
    int numprocs_;
    std::string metafile_;
    std::string folder_;
    std::string fileprefix_;
    /// This gets expanded to be the sprintf-format needed to open trace files.
    std::string tracefmt_;
    /// The number of digit in the filename.
    int width_;

  public:
    metadata(const std::string& metafile);

    /// Number of traces.
    int numTraces() const {
      return numprocs_;
    }

    std::string folder() const {
      return folder_;
    }

    std::string filePrefix() const {
      return fileprefix_;
    }

    /// Get the full filename corresponding to the given trace index.
    std::string tracename(int index) const {
      static char buf[1024];
      if (index >= numprocs_){
        throw std::runtime_error("Requested trace index is too large");
      }
      if (snprintf(buf, 1024, tracefmt_.c_str(), index) > 1023){
        throw std::runtime_error("snprintf failed to generate trace name");
      }
      return std::string(buf);
    }
  };

  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_METADATA_H
