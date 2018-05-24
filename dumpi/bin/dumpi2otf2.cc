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
#include <dumpi/libotf2dump/otf2writer.h>

#include <glob.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>

static int parse_cli_options(int argc, char **argv, d2o2opt *opt);
static std::vector<std::string> glob_files(const char* path);
static void read_header(const dumpi_header *head);
static void register_type_sizes(dumpi_profile *profile, dumpi::OTF2_Writer* writer);
static void mk_archive_dir(const char* path);
static void print_usage();
static std::vector<int> get_type_sizes(dumpi_profile*);

int main(int argc, char **argv) {
  dumpi_profile *profile;
  libundumpi_callbacks first_pass_cback, second_cback;
  d2o2opt opt;
  dumpi::OTF2_Writer writer;

  if(parse_cli_options(argc, argv, &opt) == 0) return 1;

  libundumpi_clear_callbacks(&first_pass_cback);
  libundumpi_clear_callbacks(&second_cback);
  set_first_pass_callbacks(&first_pass_cback);
  set_callbacks(&second_cback);

  auto dumpi_bin_files = glob_files((std::string(opt.dumpi_archive.c_str()) + "/*.bin").c_str());
  int num_ranks = dumpi_bin_files.size();
  if (num_ranks == 0) {
    printf("Error: could not open dumpi archive\n");
    return 2;
  }

  // Initialize the writer
  writer.set_verbosity(opt.verbose ? dumpi::OWV_INFO : dumpi::OWV_WARN);

  if (writer.open_archive(opt.output_archive, num_ranks, true) != dumpi::OTF2_WRITER_SUCCESS) {
    printf("Error opening the archive");
    return 1;
  }

  writer.register_comm_world(DUMPI_COMM_WORLD);
  writer.register_comm_self(DUMPI_COMM_SELF);
  writer.register_comm_null(DUMPI_COMM_NULL);
  writer.register_comm_error(DUMPI_COMM_ERROR);
  writer.register_null_request(DUMPI_REQUEST_NULL);
  writer.set_clock_resolution(1E9);

  // Loop over trace files. Dumpi creates one trace file per MPI rank
  // The first pass constructs information about Communicators, Groups, and type sizes.
  writer.set_comm_mode(dumpi::COMM_MODE_BUILD_COMM);
  if (opt.print_progress) printf("Identifying Communicators, Groups, and types\n");
  for(int rank = 0; rank < num_ranks; rank++) {
    profile = undumpi_open(dumpi_bin_files[rank].c_str());
    writer.set_rank(rank);
    // TODO, types are rank-specific, easiest to put them into a hash_map
    register_type_sizes(profile, &writer);
    undumpi_read_stream(profile, &first_pass_cback, (void*)&writer);
    undumpi_close(profile);

    if (opt.print_progress) printf("%.2f%% complete\n", ((1 + rank)*100.0)/num_ranks);
    fflush(stdout);
  }

  // The second pass records MPI events and their parameters
  if (opt.print_progress) printf("\nWriting event files\n");
  for(int rank = 0; rank < num_ranks; rank++) {
    writer.set_comm_mode(dumpi::COMM_MODE_BUILD_COMM_COMPLETE);
    profile = undumpi_open(dumpi_bin_files[rank].c_str());
    writer.set_rank(rank);
    undumpi_read_stream(profile, &second_cback, (void*)&writer);
    undumpi_close(profile);

    if (opt.print_progress) printf("%.2f%% complete\n", ((1 + rank)*100.0)/num_ranks);
    fflush(stdout);
  }
  if (opt.print_progress) printf("\nWriting definition files\n");
  fflush(stdout);
  writer.close_archive();
  return 0;
}

int parse_cli_options(int argc, char **argv, d2o2opt* settings) {
  int opt;
  bool output_set = false;
  bool input_set = false;

  assert(settings != NULL);
  memset(settings, 0, sizeof(d2o2opt));
  while((opt = getopt(argc, argv, "vhpi:o:")) != -1) {
      char* tmp_str = nullptr;
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
          tmp_str = strdup(optarg);
          settings->dumpi_archive = std::string(tmp_str);
          input_set = true;
          break;
        case 'o':
          tmp_str = strdup(optarg);
          settings->output_archive = tmp_str;
          output_set = true;
          break;
        case 'p':
          settings->print_progress = true;
          break;
        default:
          fprintf(stderr, "Invalid argument %c.\n", opt);
          break;
        }
      if(tmp_str != nullptr) free(tmp_str);
    }

  if (!input_set) {
    printf("%s", "Error: No Dumpi trace path set. Use '-i'\n\n");
    print_usage();
    return 0;
  }

  if (!output_set) {
    printf("%s", "Error: no OTF2 archive destination set. Use '-o'\n\n");
    print_usage();
    return 0;
  }

  return 1;
}

void print_usage() {
  printf("%s",
        "Usage:  %s [-h] [-v] [-p] [-i] archive [-o] archive\n"
        "   Options:\n"
        "        -h               Print this help\n"
        "        -v               Verbose status output\n"
        "        -p               Print progress\n"
        "        -i  archive      Path to Dumpi tracefile\n"
        "        -o  archive      Output OTF2 archive name\n");
}

static void register_type_sizes(dumpi_profile *profile, dumpi::OTF2_Writer* writer) {
  dumpi_sizeof sizes = undumpi_read_datatype_sizes(profile);
  for(int i = 0; i < sizes.count; i++)
    writer->register_type(i, sizes.size[i]);

  free(sizes.size);
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
