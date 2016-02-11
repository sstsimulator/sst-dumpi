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
#include <dumpi/bin/trace.h>
#include <dumpi/bin/otfwriter.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h> // ftello
#include <getopt.h>

namespace dumpi {

  // Options.
  struct d2oopt {
    d2oopt() : verbose(0), help(0) {}

    int verbose, help;
    std::string metafile, outroot;
  };

  static void parse_options(int argc, char **argv, d2oopt &opt);
  static void print_help(int argc, char **argv, d2oopt &opt);

  int realmain(int argc, char **argv) {
    if(argc < 2) {
      std::cerr << "Usage: " << argv[0] << " infile.meta\n";
      return 1;
    }
    d2oopt opt;
    parse_options(argc, argv, opt);
    if(opt.help) {
      print_help(argc, argv, opt);
      return 1;
    }
    if(opt.metafile == "" || opt.outroot == "") {
      std::cerr << "Error:  Metafile and outfile root must not be blank\n";
      return 2;
    }
    if(std::ifstream((opt.outroot + ".otf").c_str())) {
      std::cerr << "Error:  Refusing to overwrite OTF files with common root "
		<< opt.outroot << "\n";
      return 3;
    }

    // Read metadata (should I add facilities for this to dumpi?)\n";
    if(opt.verbose) std::cout << "Parsing metafile\n";
    metadata meta(opt.metafile);

    // This becomes the shared back-end info for the traces.
    // This also constructs COMM_WORLD for us.
    sharedstate shared(meta.traces());

    // Open traces.
    if(opt.verbose) std::cout << "Pre-parsing traces.\n";
    std::vector<trace> traces(meta.traces());
    preparse_traces(meta, &shared, traces);
    // and now we should have all the preparse state we could wish for
    // ...do whatever we want with the preparse data.
    if(opt.verbose) std::cout << "Converting DUMPI traces to OTF.\n";
    otfwriter writer(opt.outroot.c_str());
    writer.write(meta, traces);
    // Done.
    return 0;
  }

  void parse_options(int argc, char **argv, d2oopt &opt) {
    int optid;
    while((optid = getopt(argc, argv, "vhf:o:")) != -1) {
      switch(optid) {
      case 'v': opt.verbose = 1; break;
      case 'h': opt.help = 1; break;
      case 'f': opt.metafile = optarg; break;
      case 'o': opt.outroot = optarg; break;
      }
    }
    if(opt.metafile == "" && optind < argc) {
      opt.metafile = argv[optind];
      ++optind;
    }
    if(opt.outroot == "" && optind < argc) {
      opt.outroot = argv[optind];
    }
  }

  void print_help(int, char **argv, d2oopt &opt) {
    std::cout << "Usage:  " << argv[0] << " [-h] [-v] -f metafile -o outroot\n"
              << "   Options:\n"
              << "        -h             Print this help\n"
              << "        -v             Verbose status output\n"
              << "        -f  metafile   Read traces based on the given metafile\n"
              << "        -o  outroot    Write OTF files to the given fileroot\n";
  }

} // end of namespace dumpi

int main(int argc, char **argv) {
  try {
    return dumpi::realmain(argc, argv);
  } catch(std::exception &e) {
    std::cerr << "main:  Caught std::exception " << e.what() << "\n";
    return 1;
  }
  catch(const char *msg) {
    std::cerr << "Error: " << msg << "\n";
    return 2;
  }
  catch(char *msg) {
    std::cerr << "Error: " << msg << "\n";
    return 2;
  }
  catch(...) {
    std::cerr << "main:  Caught unknown error type\n";
    return 3;
  }
}
