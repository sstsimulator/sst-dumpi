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

#include <dumpi/bin/metadata.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

namespace dumpi {

  //
  // Construct a new
  metadata::metadata(const std::string &metafile) {
    std::ifstream in(metafile.c_str());
    if(! in) {
      FILE *fin = fopen(metafile.c_str(), "r");
      if(! fin) {
	std::cerr << "\"" << metafile << "\": " << strerror(errno) << "\n";
      }
      else {
	std::cerr << "Major weirdness:  failed to open ifstream for \""
		  << metafile << "\" but succeeded using fopen.\n";
      }
      throw "Failed to open DUMPI metafile.";
    }
    // The file search prefix should be the same leading path as the metafile.
    std::string::size_type slash = metafile.find_last_of('/');
    std::string pathprefix = "";
    if(slash != std::string::npos) {
      pathprefix = metafile.substr(0, slash+1);
    }
    // N
    while(in.good()) {
      std::string line;
      std::getline(in, line);
      std::string::size_type eq = line.find_first_of('=');
      if(eq != std::string::npos) {
	std::string key = line.substr(0, eq), val = line.substr(eq+1);
	if(std::string("numprocs") == key) {
          std::istringstream ss(val);
          ss >> numprocs_;
        }
	if(std::string("fileprefix") == key)
	  fileprefix_ = pathprefix + val;
      }
    }
    //std::cerr << "numprocs: " << numprocs_ << "\n"
    //    << "fileprefix: " << fileprefix_ << "\n";
    if(numprocs_ <= 0 || fileprefix_ == "") {
      throw "metadata:  Invalid metafile.";
    }
    // Now expand the fileprefix to have the "right" number of zeros.
    // Start with a %04f format, and move on from there.
    static const int maxwidth=10;
    std::stringstream ss;
    int files_found = 0;
    for(int i = 0; i < maxwidth; ++i) {
      files_found = 0;
      width_ = (4 + i) % maxwidth;
      for(int file = 0; file < numprocs_; ++file) {
	ss.clear(); // Clear any error flags
	ss.str(""); // Initalize stream.
	ss << fileprefix_ << "-" << std::setfill('0') << std::setw(width_)
	   << file << ".bin";
	FILE *exists = fopen(ss.str().c_str(), "r");
	if(exists) {
	  ++files_found;
	  fclose(exists);
	}
      }
      if(files_found == numprocs_) {
	// Create a printf-style format.
	ss.clear();
	ss.str("");
	ss << fileprefix_ << "-%0" << width_ << "d.bin";
	tracefmt_ = ss.str();
	break;
      }
    }
    if(files_found != numprocs_) {
      throw "metadata:  Failed to find binary trace files.";
    }
    else {
      ; //std::cerr << "File prefix: " << fileprefix_ << "\n";
    }
  }

} // end of namespace dumpi