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

#include <dumpi/bin/dumpi2otf2-defs.h>
#include <dumpi/libundumpi/libundumpi.h>

#include <glob.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

DumpiArgs otf2_defs;

static int parse_cli_options(int argc, char **argv, d2o2opt *opt);
static std::vector<std::string> glob_files(const char* path);
static void read_header(const dumpi_header *head);
static void register_type_sizes(dumpi_profile *profile, dumpi::OTF2_Writer* writer);
static void mk_archive_dir(const char* path);
static void print_usage();
static std::vector<int> get_type_sizes(dumpi_profile*);

int main(int argc, char **argv) {
  dumpi_profile *profile;
  libundumpi_callbacks cback;
  d2o2opt& opt = otf2_defs.program_options;

  if(parse_cli_options(argc, argv, &opt) == 0) return 1;

  libundumpi_clear_callbacks(&cback);
  set_callbacks(&cback);

  auto dumpi_bin_files = glob_files((std::string(opt.dumpi_archive) + "*.bin").c_str());
  if (dumpi_bin_files.size() == 0) {
    printf("Error: could not open dumpi archive\n");
    return 2;
  }

  auto& writer = otf2_defs.otf2_writer;
  if (opt.verbose == 1)
    writer.set_verbosity(dumpi::OWV_INFO);
  writer.open_archive(opt.output_archive, dumpi_bin_files.size(), true);
  writer.register_comm_world(DUMPI_COMM_WORLD);

  // Loop over ranks
  for(int rank = 0; rank < dumpi_bin_files.size(); rank++) {
    profile = undumpi_open(dumpi_bin_files[rank].c_str());
    writer.set_rank(rank);
    register_type_sizes(profile, &writer);

    undumpi_read_stream(profile, &cback, (void*)&otf2_defs);
    undumpi_close(profile);
  }
  writer.close_archive();
  return 0;
}

int parse_cli_options(int argc, char **argv, d2o2opt* settings) {
  int opt;
  bool output_set = false;
  bool input_set = false;

  assert(settings != NULL);
  memset(settings, 0, sizeof(d2o2opt));
  while((opt = getopt(argc, argv, "vhsi:o:")) != -1) {
      switch(opt) {
        case 'v':
          fprintf(stdout, "Setting output to verbose.\n");
          settings->verbose = 1;
          break;
        case 'h':
          print_usage();
          std::exit(0);
          break;
        case 'i':
          settings->dumpi_archive = strdup(optarg);
          input_set = true;
          break;
        case 'o':
          settings->output_archive = strdup(optarg);
          output_set = true;
          break;
        case 's':
          settings->skip_unused_calls = true;
          break;
        default:
          fprintf(stderr, "Invalid argument %c.\n", opt);
          break;
        }
    }

  if (!output_set) {
    printf("%s", "Error: no OTF2 archive destination set. Use '-o'\n");
    print_usage();
    return 0;
  }

  if (!input_set) {
    printf("%s", "Error: No path Dumpi trace path set. Use '-i'\n");
    print_usage();
    return 0;
  }

  return 1;
}

void print_usage() {
  printf("%s",
        "Usage:  %s [-h] [-v] [-i] archive [-o] archive\n"
        "   Options:\n"
        "        -h               Print this help\n"
        "        -v               Verbose status output\n"
        "        -s               Skip callbacks that will not be used by SST/macro (untested)"
        "        -i  archive      Path to Dumpi tracefile\n"
        "        -o  archive      Output OTF2 archive name\n");
}

static void register_type_sizes(dumpi_profile *profile, dumpi::OTF2_Writer* writer) {
  dumpi_sizeof sizes = undumpi_read_datatype_sizes(profile);
  for(int i = 0; i < sizes.count; i++)
    writer->register_type(i, sizes.size[i]);

  delete[] sizes.size;
}

// Returns a list of files that match a given pattern
static std::vector<std::string> glob_files(const char* path)
{
  glob_t g_res;
  glob(path, GLOB_TILDE, nullptr, &g_res);
  std::vector<std::string> result;
  for(int i = 0; i < g_res.gl_pathc; i++)
    result.push_back(std::string(g_res.gl_pathv[i]));
  globfree(&g_res);
  return result;
}
