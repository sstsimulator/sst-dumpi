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
#include <dumpi/bin/trace.h>
#include <dumpi/bin/dumpistats-timebin.h>
#include <dumpi/bin/dumpistats-gatherbin.h>
#include <dumpi/bin/dumpistats-handlers.h>
#include <dumpi/bin/dumpistats-callbacks.h>
#include <sstream>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

using namespace dumpi;

static const struct option longopts[] = {
  {"help", no_argument, NULL, 'h'},
  {"verbose", no_argument, NULL, 'v'},
  {"bin", required_argument, NULL, 'b'},
  {"mark", required_argument, NULL, 'm'},
  {"gather", required_argument, NULL, 'g'},
  {"count", required_argument, NULL, 'c'},
  {"time", required_argument, NULL, 't'},
  {"sent", required_argument, NULL, 's'},
  {"recvd", required_argument, NULL, 'r'},
  {"exchange", required_argument, NULL, 'x'},
  {"lump", required_argument, NULL, 'l'},
  {"perfctr", required_argument, NULL, 'p'},
  {"in", required_argument, NULL, 'i'},
  {"out", required_argument, NULL, 'o'},
  {NULL, 0, NULL, 0}
};

void print_help(const std::string &name) {
  std::cerr << name << ":  Extract statistics from DUMPI trace files\n"
            << "Options:\n"
            << "   (-h|--help)                Print help screen and exit\n"
            << "   (-v|--verbose)             Verbose status output\n"
            << "   (-b|--bin)      timerange  Define a collection time range\n"
            << "   (-m|--mark)     /crt/end/  Create/end a bin at annotations\n"
            << "   (-g|--gather)   /crt/end/  Accumulate to an annotated bin\n"
            << "   (-c|--count)    funcname   Count entries into a function\n"
            << "   (-t|--time)     funcname   Accumulate time in a function\n"
            << "   (-s|--sent)     funcname   Count bytes sent by a function\n"
            << "   (-r|--recvd)    funcname   Count bytes recvd by function\n"
            << "   (-x|--exchange) funcname   Full send/recv exchange info\n"
            << "   (-l|--lump)     funcname   Lump (bin) messages by size\n"
            << "   (-p|--perfctr)  funcname   PAPI perfcounter info\n"
            << "   (-i|--in)       metafile   DUMPI metafile (required)\n"
            << "   (-o|--out)      fileroot   Output file root (required)\n"
            << "\n"
            << "The timerange has the form:\n"
            << "  (all | mpi | BOUND to BOUND) [by TIME]\n"
            << "    where BOUND the form:\n"
            << "      (IDENTITY | TIME) [(+|-) TIME]\n"
            << "    and IDENTITY is:\n"
            << "      start | end | init | finalize | TIME\n"
            << "    and TIME is:\n"
            << "      (hh:)?(mm:)?[0-9]0-9+(.[0-9]*)?\n"
            << "\n"
            << "The funcname has the form:\n"
            << "  all | mpi | non-mpi | sends | recvs | coll | wait | io | RE\n"
            << "    where RE is a case-insensitive regular expression\n"
            << "    (e.g. \"MPI_Wait.*\" or \"mpio?_(wait.*|test.*))\"\n"
            << "  All regular expressions are implicitly flanked by ^ and $,\n"
            << "  so \"mpi_.?send\" does not match MPI_Sendrecv\n"
            << "\n"
            << "The annotated bins (-m and -g) use regular expressions\n"
            << "with a format similar to sed basic regular expressions, so:"
            << "    -m '/begin bin/end bin/'\n"
            << "    -m '!begin bin!end bin!i'\n"
            << "    -g '| +start loop [0-9]+ *$|end loop [0-9]+$|'\n"
            << "are all valid annotations (note that backreferences are \n"
            << "not allowed).  Escaped characters other than the delimiter\n"
            << "are passed directly to the regular expression parser.\n"
            << "\n"
            << "Example 1:\n"
            << "  " << name << " --bin=all --time=mpi --time=other \\\n"
            << "        -i dumpi.meta -o stats\n"
            << "     Writes a file called stats-0.dat with three columns:\n"
            << "       1:  The rank of each MPI node\n"
            << "       2:  Aggregate time spent in MPI calls\n"
            << "       3:  Aggregate time spent outside MPI calls\n"
            << "\n"
            << "Example 2\n"
            << "  " << name << " --bin='init to finalize by 1:00' \\ \n"
            << "        --count=mpi_.*send -i dumpi.meta -o stats\n"
            << "     Writes files with send counts binned into 1 minute\n"
            << "     intervals.\n"
            << "\n"
            << "Example 3\n"
            << "  " << name << "  --bin='begin+10 to end-10' -s all -r all \\\n"
            << "         -i dumpi.meta -o stats \\\n"
            << "      Writes a new file containing bytes sent and received\n"
            << "      starting 10 seconds after first and ending 10 seconds\n"
            << "      before last simulation timestamp\n";
}

struct options {
  bool verbose;
  std::string infile, outroot;
  std::vector<binbase*> bin;
  std::vector<handlerbase*> handlers;
  options() : verbose(false) {}
};

int main(int argc, char **argv) {
  std::string shortopts;
  for(int optid = 0; longopts[optid].name != NULL; ++optid) {
    if(longopts[optid].val) {
      shortopts += char(longopts[optid].val);
      if(longopts[optid].has_arg != no_argument) shortopts += ":";
    }
  }
  options opt;
  int ch;
  /*
  {"help", no_argument, NULL, 'h'},
  {"verbose", no_argument, NULL, 'v'},
  {"bin", required_argument, NULL, 'b'},
  {"count", required_argument, NULL, 'c'},
  {"time", required_argument, NULL, 't'},
  {"sent", required_argument, NULL, 's'},
  {"recvd", required_argument, NULL, 'r'},
  {"exchange", required_argument, NULL, 'x'},
  {"in", required_argument, NULL, 'i'},
  {"out", required_argument, NULL, 'o'},
  */
  while((ch=getopt_long(argc, argv, shortopts.c_str(), longopts, NULL)) != -1) {
    switch(ch) {
    case 'h':
      print_help(argv[0]);
      return 1;
    case 'v':
      opt.verbose = true;
      break;
    case 'b':
      opt.bin.push_back(new timebin(optarg));
      break;
    case 'm':
      opt.bin.push_back(new gatherbin(optarg, false));
      break;
    case 'g':
      opt.bin.push_back(new gatherbin(optarg, true));
      break;
    case 'c':
      opt.handlers.push_back(new counter(optarg));
      break;
    case 't':
      opt.handlers.push_back(new timer(optarg));
      break;
    case 's':
      opt.handlers.push_back(new sender(optarg));
      break;
    case 'r':
      opt.handlers.push_back(new recver(optarg));
      break;
    case 'x':
      opt.handlers.push_back(new exchanger(optarg));
      break;
    case 'l':
      opt.handlers.push_back(new lumper(optarg));
      break;
    case 'p':
      opt.handlers.push_back(new perfcounter(optarg));
      break;
    case 'i':
      opt.infile = optarg;
      break;
    case 'o':
      opt.outroot = optarg;
      break;
    default:
      std::cerr << "Invalid argument: " << char(ch) << "\n";
      return 2;
    }    
  }

  if(opt.infile == "" || opt.outroot == "") {
    std::cerr << "Usage: " << argv[0] << " [options] -i infile -o outroot\n";
    return 3;
  }
  FILE *ff = fopen(opt.infile.c_str(), "r");
  if(! ff) {
    std::cerr << opt.infile << ":  " << strerror(errno) << "\n";
    return 4;
  }

  try {
    // Provide some sensible defaults (time in MPI and non-MPI functions).
    if(opt.bin.empty())
      opt.bin.push_back(new timebin("all"));
    if(opt.handlers.empty()) {
      opt.handlers.push_back(new timer("mpi"));
    }

    // Preparse the streams to get data types etc. correct.
    if(opt.verbose) std::cerr << "Parsing metafile\n";
    metadata meta(opt.infile);

    // Open traces.
    if(opt.verbose) std::cout << "Pre-parsing traces.\n";
    sharedstate shared(meta.traces());
    std::vector<trace> traces;
    preparse_traces(meta, &shared, traces);

    // Tell the handlers about world size.
    if(opt.verbose) std::cerr << "Setting up handlers\n";
    for(size_t i = 0; i < opt.handlers.size(); ++i)
      opt.handlers.at(i)->set_world_size(meta.traces());
    // Set up.  Each bin gets a copy of all the handlers.
    for(size_t i = 0; i < opt.bin.size(); ++i) {
      std::stringstream ss;
      ss << opt.outroot << "-bin" << i;
      std::string name = ss.str();
      opt.bin.at(i)->init(name, &traces, opt.handlers);
    }
    callbacks cb;

    if(opt.verbose) std::cerr << "Re-parsing files and building tables\n";
    cb.go(meta, traces, opt.bin);
    // Clean up.
    for(size_t i = 0; i < opt.bin.size(); ++i)
      delete opt.bin.at(i);
  } catch(const char *desc) {
    std::cerr << "Error exit: " << desc << "\n";
    return 10;
  }
}