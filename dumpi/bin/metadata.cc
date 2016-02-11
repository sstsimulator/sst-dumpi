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
