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


#include <dumpi/bin/dumpi2otf2-callbacks.h>
#include <dumpi/bin/dumpi2otf2-defs.h>
#include <dumpi/libundumpi/libundumpi.h>

//#include <dumpi/bin/dumpi2ascii-callbacks.h>
//#include <dumpi/common/funcs.h>
//#include <dumpi/common/perfctrs.h>
//#include <dumpi/common/debugflags.h>
#include <otf2/otf2.h>
#include <glob.h>

#include <algorithm>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <assert.h>

#include <type_traits>

extern int optind;
FILE *dumpfh = NULL;
DumpiArgs otf2_defs;

static int parse_options(int argc, char **argv, d2o2opt *opt);
static std::vector<std::string> glob_files(const char* path);
static void read_header(const dumpi_header *head);
static OTF2_Archive* open_archive();
static OTF2_FlushType pre_flush( void* userData, OTF2_FileType fileType, OTF2_LocationRef location, void* callerData, bool final);
static OTF2_TimeStamp post_flush(void* userData, OTF2_FileType fileType, OTF2_LocationRef location);
static OTF2_Archive* init_otf2(int i);
static void write_def_table(DumpiArgs*, OTF2_Archive*);

static std::unordered_map<const char*, int> mpi_call_map;
extern std::vector<const char*> mpi_api_list;

OTF2_FlushCallbacks flush_callbacks =
{
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
};

/* TODO
 * - handle data type creation
 * - Parse dumpi datatypes into correct byte sizes (see dumpi2asci -X)
 * - Handle V collectives
 */
int main(int argc, char **argv) {

  dumpi_profile *profile;
  libundumpi_callbacks cback;
  d2o2opt& opt = otf2_defs.program_options;
  dumpfh = stdout;

  if(parse_options(argc, argv, &opt) == 0) return 1;

  libundumpi_clear_callbacks(&cback);
  set_callbacks(&cback);

  OTF2_Archive* archive = init_otf2(1);
  //auto dumpi_meta_file = glob_files((std::string(opt.dumpi_archive) + "*.meta").c_str());


  auto dumpi_bin_files = glob_files((std::string(opt.dumpi_archive) + "*.bin").c_str());
  if (dumpi_bin_files.size() == 0) {
    printf("Error: could not open dumpi archive\n");
    return 2;
  }

  //Loop over ranks
  int num_ranks = 1;
  for(int rank = 0; rank < dumpi_bin_files.size(); rank++) {
    profile = undumpi_open(dumpi_bin_files[rank].c_str());
    OTF2_EvtWriter* evt_writer = OTF2_Archive_GetEvtWriter(archive, rank);
    otf2_defs.rank = rank;
    otf2_defs.writer = evt_writer;

    // Get type sizes
    dumpi_sizeof sizes;
    dumpi_read_datatype_sizes(profile, &sizes);
    for(int i = 0; i < sizes.count; i++)
      otf2_defs.mpi_type_to_size[i] = sizes.size[i];

    // Get start time from header
    dumpi_header *head = undumpi_read_header(profile);
    otf2_defs.start_time = head->starttime; // TODO: WRONG! this is a unix timestamp!
    dumpi_free_header(head);

    d2o2_addr = (d2o2_addrmap*)calloc(1, sizeof(d2o2_addrmap));
    assert(d2o2_addr != NULL);
    dumpi_read_function_addresses(profile, &(d2o2_addr->count),
                                  &(d2o2_addr->address), &(d2o2_addr->name));
    undumpi_read_stream(profile, &cback, (void*)&otf2_defs);
    undumpi_close(profile);
    OTF2_Archive_CloseEvtWriter(archive, evt_writer);
  }

  OTF2_Archive_CloseEvtFiles(archive);
  write_def_table(&otf2_defs, archive);
  OTF2_Archive_Close(archive);

  printf("What are these?\n");
  for(int i = 0; i < d2o2_addr->count; ++i) {
    printf("%s\n", d2o2_addr->name[i]);
    free(d2o2_addr->name[i]);
  }
  free(d2o2_addr->address);
  free(d2o2_addr->name);
  free(d2o2_addr);
    d2o2_addr = NULL;
  return 0;
}

int parse_options(int argc, char **argv, d2o2opt* settings) {
  int opt;
  assert(settings != NULL);
  memset(settings, 0, sizeof(d2o2opt));
  while((opt = getopt(argc, argv, "vhi:o:")) != -1) {
      switch(opt) {
        case 'v':
          // set verbose
          fprintf(stdout, "Setting output to verbose.\n");
          settings->verbose = 1;
          break;
        case 'h':
          fprintf(stderr,
                  "Usage:  %s [-h] [-v] [-i] archive [-o] archive\n"
                  "   Options:\n"
                  "        -h               Print this help\n"
                  "        -v               Verbose status output\n"
                  "        -i  archive      Path to Dumpi tracefile\n"
                  "        -o  archive      Output OTF2 archive name\n",
                  argv[0]);
          std::exit(0);
          break;
        case 'i':
          settings->dumpi_archive = strdup(optarg);
          break;
        case 'o':
          settings->output_archive = strdup(optarg);
          break;
        default:
          fprintf(stderr, "Invalid argument %c.\n", opt);
          break;
        }
    }
  return 1;
}

OTF2_FlushType pre_flush( void* userData, OTF2_FileType fileType, OTF2_LocationRef location, void* callerData, bool final)
{
    return OTF2_FLUSH;
}

OTF2_TimeStamp post_flush(void* userData, OTF2_FileType fileType, OTF2_LocationRef location)
{
    return 0;
}

OTF2_Archive* init_otf2(int num_ranks) {
  OTF2_Archive* archive = OTF2_Archive_Open( "/home/sknight/code/github/sst-dumpi-backup/build/df_AMG_n8_dumpi/",
                                             "ArchiveName",
                                             OTF2_FILEMODE_WRITE,
                                             1024 * 1024 /* event chunk size */,
                                             4 * 1024 * 1024 /* def chunk size */,
                                             OTF2_SUBSTRATE_POSIX,
                                             OTF2_COMPRESSION_NONE );

//  OTF2_MPI_Archive_SetCollectiveCallbacks( archive,
//                                           DUMPI_COMM_WORLD,
//                                           DUMPI_COMM_NULL );

  OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
  OTF2_Archive_OpenEvtFiles(archive);

  return archive;
}

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

void write_def_table(DumpiArgs* dargs, OTF2_Archive* archive)
{
  OTF2_GlobalDefWriter* gdefwriter = OTF2_Archive_GetGlobalDefWriter(archive);
  mpi_call_map.clear();

  // Reference
  // https://silc.zih.tu-dresden.de/otf2-current/group__usage__writing__mpi.html

  // CLOCK
  OTF2_GlobalDefWriter_WriteClockProperties(gdefwriter,
                                            1E9,        /* dumpi is accurate to the nanosecond */
                                            0,          /* TODO: find the correct offset, dumpi probably does not start at 0 */
                                            0);

  // REGIONS
  for (int i = 0; i < otf2_defs.region.size(); i++) {
    auto region_name = otf2_defs.region[i];
    OTF2_GlobalDefWriter_WriteRegion( gdefwriter,
                                      i                             /* id */,
                                      otf2_defs.string[region_name] /* region name  */,
                                      0                             /* alternative name */,
                                      0                             /* description */,
                                      OTF2_REGION_ROLE_FUNCTION,
                                      OTF2_PARADIGM_MPI,
                                      OTF2_REGION_FLAG_NONE,
                                      0                             /* source file */,
                                      0                             /* begin lno */,
                                      0                             /* end lno */ );
  }

//  TODO: find size
//  uint64_t comm_locations[ size ];
//  for ( int r = 0; r < size; r++ )
//  {
//      comm_locations[ r ] = r;
//  }
//  OTF2_GlobalDefWriter_WriteGroup( gdefwriter,
//                                   0 /* id */,
//                                   7 /* name */,
//                                   OTF2_GROUP_TYPE_COMM_LOCATIONS,
//                                   OTF2_PARADIGM_MPI,
//                                   OTF2_GROUP_FLAG_NONE,
//                                   size,
//                                   comm_locations );

//  //PARADIGM
//  OTF2_GlobalDefWriter_WriteParadigm(gdefwriter,
//                                     OTF2_PARADIGM_MPI,
//                                     otf2_defs.string[std::string("MPI")],
//                                     OTF2_PARADIGM_CLASS_PROCESS);

  // SYSTEM_TREE_NODE
  OTF2_GlobalDefWriter_WriteSystemTreeNode( gdefwriter,
                                            0 /* id */,
                                            0 /* name */,
                                            0 /* class */,
                                            OTF2_UNDEFINED_SYSTEM_TREE_NODE /* parent */ );

  // STRINGS
  for(int i = 0; i < otf2_defs.string.size(); i++) {
    OTF2_GlobalDefWriter_WriteString(gdefwriter, i, otf2_defs.string[i].c_str());
  }
  //COMM
  //LocationGroup?
  //Location
}

OTF2DefTable::OTF2DefTable() {}
int OTF2DefTable::map(std::string string)
{
  auto it = _map.find(string);
  if ( it != _map.end())
    return it->second;
  _map.insert(std::make_pair(string, _counter));
  return _counter++;
}

const std::string OTF2DefTable::map(int index) {
  // Finds the string corresponding to the given int.
  auto out = std::find_if(_map.begin(), _map.end(), [index] (const auto& elem) { return elem.second == index; });
  return out == _map.end() ? nullptr : out->first;
}

const std::string OTF2DefTable::operator[] (int index) {return map(index); }
int OTF2DefTable::operator[] (std::string string) { map(string); }
int OTF2DefTable::size() {return _counter;}
