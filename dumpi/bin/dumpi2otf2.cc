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

#include <otf2/otf2.h>
#include <glob.h>
#include <string.h>
#include <libgen.h>

#include <algorithm>
#include <cstring>
#include <vector>
#include <string.h>
#include <getopt.h>
#include <assert.h>

#include <sys/stat.h>

#include <type_traits>

extern int optind;
FILE *dumpfh = NULL;
DumpiArgs otf2_defs;

static int parse_cli_options(int argc, char **argv, d2o2opt *opt);
static std::vector<std::string> glob_files(const char* path);
static void read_header(const dumpi_header *head);
static OTF2_Archive* open_archive();
static void check_status( OTF2_ErrorCode status, char* description);
static OTF2_FlushType pre_flush( void* userData, OTF2_FileType fileType, OTF2_LocationRef location, void* callerData, bool final);
static OTF2_TimeStamp post_flush(void* userData, OTF2_FileType fileType, OTF2_LocationRef location);
static OTF2_Archive* init_otf2(int ranks, const char* archive);
static void write_def_table(DumpiArgs*, OTF2_Archive*);


static void mk_archive_dir(const char* path);
static void print_usage();
static std::vector<int> get_type_sizes(dumpi_profile*);

extern std::vector<const char*> mpi_api_list;

OTF2_FlushCallbacks flush_callbacks =
{
    .otf2_pre_flush  = pre_flush,
    .otf2_post_flush = post_flush
};

/* TODO
 * - Handle V collectives
 */
int main(int argc, char **argv) {
  dumpi_profile *profile;
  libundumpi_callbacks cback;
  d2o2opt& opt = otf2_defs.program_options;
  dumpfh = stdout;

  if(parse_cli_options(argc, argv, &opt) == 0) return 1;

  libundumpi_clear_callbacks(&cback);
  set_callbacks(&cback);

  auto dumpi_bin_files = glob_files((std::string(opt.dumpi_archive) + "*.bin").c_str());
  if (dumpi_bin_files.size() == 0) {
    printf("Error: could not open dumpi archive\n");
    return 2;
  }

  otf2_defs.num_ranks = dumpi_bin_files.size();
  OTF2_Archive* archive = init_otf2(dumpi_bin_files.size(), opt.output_archive);

  // Loop over ranks
  for(int rank = 0; rank < dumpi_bin_files.size(); rank++) {
    profile = undumpi_open(dumpi_bin_files[rank].c_str());
    OTF2_EvtWriter* evt_writer = OTF2_Archive_GetEvtWriter(archive, rank);
    otf2_defs.rank = rank;
    otf2_defs.writer = evt_writer;

    // Get type sizes
    otf2_defs.type_sizes = get_type_sizes(profile);

    undumpi_read_stream(profile, &cback, (void*)&otf2_defs);
    undumpi_close(profile);
    OTF2_Archive_CloseEvtWriter(archive, evt_writer); 
  }
  //if (rank == 0) {
  //  // Write def files
    write_def_table(&otf2_defs, archive);
  //}

  // Close the Archive
  OTF2_Archive_CloseEvtFiles(archive);
  OTF2_Archive_Close(archive);
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

OTF2_FlushType pre_flush( void* userData, OTF2_FileType fileType, OTF2_LocationRef location, void* callerData, bool final)
{
    return OTF2_FLUSH;
}

OTF2_TimeStamp post_flush(void* userData, OTF2_FileType fileType, OTF2_LocationRef location)
{
    return 0;
}

OTF2_Archive* init_otf2(int num_ranks, const char* archive_dir) {
  mk_archive_dir(archive_dir);
  OTF2_Archive* archive = OTF2_Archive_Open( archive_dir,
                                             "traces",
                                             OTF2_FILEMODE_WRITE,
                                             1024 * 1024 /* event chunk size */,
                                             4 * 1024 * 1024 /* def chunk size */,
                                             OTF2_SUBSTRATE_POSIX,
                                             OTF2_COMPRESSION_NONE );

  OTF2_Archive_SetSerialCollectiveCallbacks( archive );
  OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
  OTF2_Archive_OpenEvtFiles(archive);
  OTF2_Archive_OpenDefFiles(archive);

  return archive;
}

std::vector<int> get_type_sizes(dumpi_profile* profile) {
  dumpi_sizeof sizes = undumpi_read_datatype_sizes(profile);
  std::vector<int> sz_vec;

  //printf("Reading Sizes:\n");
  for (int i = 0; i < sizes.count; i++) {
    //printf("  type id: %i, size: %i\n", i, sizes.size[i]);
    sz_vec.push_back(sizes.size[i]);
  }
  delete[] sizes.size;
  return sz_vec;
}

// Returns a list of files that match the pattern
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

static void check_status( OTF2_ErrorCode status, const char* description) {
  if ( status != OTF2_SUCCESS )
    printf("OTF2 Error: %s\n\t%s\n", description, OTF2_Error_GetDescription(status));
}

// See ScoreP source for local def writer examples
//  vendor/otf2/src/tools/otf2_trace_gen/otf2_trace_gen.c

void write_def_table(DumpiArgs* dargs, OTF2_Archive* archive)
{
  // Reference
  // https://silc.zih.tu-dresden.de/otf2-current/group__usage__writing__mpi.html
  OTF2_GlobalDefWriter* defwriter = OTF2_Archive_GetGlobalDefWriter(archive);

  OTF2_GlobalDefWriter_WriteClockProperties(defwriter,
                                            1E9,
                                            otf2_defs.start_time,
                                            otf2_defs.stop_time - otf2_defs.start_time
                                            );

  // Strings must come first in the def file
  // Otherwise tools like otf2_print will report errors, even though all of the information is available
  otf2_defs.string[""];
  otf2_defs.string["MPI_COMM_WORLD"];

//  for(int i = 0; i < otf2_defs.num_ranks; i++) {
//    char process_name[32];
//    sprintf(process_name, "MPI Rank %d", i);
//    otf2_defs.string[process_name];
//  }

//  for (int i = 0; i < otf2_defs.num_ranks; i++) {
//    char loc_name[32];
//    sprintf(loc_name, "Master Thread %d", i);

//  }
  for(int rank = 0; rank < otf2_defs.num_ranks; rank++) {
    otf2_defs.string[std::string("MPI Rank ") + std::to_string(rank)];
    otf2_defs.string[std::string("Master Thread ") + std::to_string(rank)];
  }



  // STRINGS
  for(int i = 0; i < otf2_defs.string.size(); i++) {
    OTF2_GlobalDefWriter_WriteString(defwriter, i, otf2_defs.string[i].c_str());
  }

  // REGIONS
  printf("%i\n", otf2_defs.region.size());
  for(int i = 0; i < otf2_defs.region.size(); i++) {
    std::string region_name = otf2_defs.region[i];
    auto val = otf2_defs.string[region_name];
    printf("REGION: %s\n", region_name.c_str());
    OTF2_GlobalDefWriter_WriteRegion(defwriter,
                                 i                             /* id */,
                                 otf2_defs.region[region_name] /* region name  */,
                                 otf2_defs.string[region_name] /* alternative name */,
                                 0                             /* description */,
                                 OTF2_REGION_ROLE_FUNCTION,
                                 OTF2_PARADIGM_MPI,
                                 OTF2_REGION_FLAG_NONE,
                                 0                             /* source file */,
                                 0                             /* begin lno */,
                                 0                             /* end lno */ );

  }

  // SYSTEM_TREE_NODE
  OTF2_GlobalDefWriter_WriteSystemTreeNode( defwriter,
                                            0 /* id */,
                                            0 /* name */,
                                            0 /* class */,
                                            OTF2_UNDEFINED_SYSTEM_TREE_NODE /* parent */ );

//  }
  // LOCATIONGROUP
  // locationgroup appears to be used to identify computation localities. For example, all of the threads on a given node.
  for (int i = 0; i < otf2_defs.num_ranks; i++) {
    char rank_name[32];
    sprintf(rank_name, "MPI Rank %d", i);
    OTF2_GlobalDefWriter_WriteLocationGroup(defwriter,
                                            i,
                                            otf2_defs.string[rank_name],
                                            OTF2_LOCATION_GROUP_TYPE_PROCESS,
                                            0 // TODO, This should point to the node this rank ran on. Not necessary for sst/macro trace replay.
                                            );
  }

  // LOCATION
  // In this context, each location will identifies a rank. Must come before groups.
  for (int i = 0; i < otf2_defs.num_ranks; i++) {
    char loc_name[32];
    sprintf(loc_name, "Master Thread %d", i);
    OTF2_GlobalDefWriter_WriteLocation(defwriter,
                                       i,
                                       otf2_defs.string[loc_name],
                                       OTF2_LOCATION_TYPE_CPU_THREAD,
                                       otf2_defs.event_count[i],
                                       0);
  }


  // GROUP
  for(auto group_it = otf2_defs.mpi_group.begin(); group_it != otf2_defs.mpi_group.end(); group_it++) {
    auto members = group_it->second;
    uint64_t* group_list = new uint64_t[members.size()];
    for (int i = 0; i < members.size(); i++)  group_list[i] = (uint64_t)members[i];

    OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                    group_it->first,
                                    otf2_defs.string[""],
                                    OTF2_GROUP_TYPE_COMM_GROUP,
                                    OTF2_PARADIGM_MPI,
                                    OTF2_GROUP_FLAG_NONE,
                                    members.size(),
                                    group_list);
    delete group_list;
  }

  // When no communicator manipulation occurs, mpi_group list will be empty because it is dynamically generated from MPI callbacks.
  // The trace needs to have at least one group for MPI_COMM_WORLD
  if (otf2_defs.mpi_group.size() == 0) {
    uint64_t* group_list = new uint64_t[otf2_defs.num_ranks];
    for (int i = 0; i < otf2_defs.num_ranks; i++)  group_list[i] = i;
    OTF2_GlobalDefWriter_WriteGroup(defwriter,
                                    0,
                                    otf2_defs.string[""],
                                    OTF2_GROUP_TYPE_COMM_GROUP,
                                    OTF2_PARADIGM_MPI,
                                    OTF2_GROUP_FLAG_NONE,
                                    otf2_defs.num_ranks,
                                    group_list);
    delete group_list;
  }

  // COMM
  for(auto comm_it = otf2_defs.mpi_comm.begin(); comm_it != otf2_defs.mpi_comm.end(); comm_it++) {
    MPI_comm& comm = comm_it->second;

    OTF2_GlobalDefWriter_WriteComm( defwriter,
                                    comm_it->first,
                                    comm.id == DUMPI_COMM_WORLD ? otf2_defs.string["MPI_COMM_WORLD"] : otf2_defs.string[""],
                                    comm.group,
                                    comm.parent);
  }

  // Register communicator mapping. Also essential for creating Local def files, which are not created automatically.
  for(int rank = 0; rank< otf2_defs.num_ranks; rank++) {
    OTF2_DefWriter* local_def_writer = OTF2_Archive_GetDefWriter(archive, rank);

    // local COMM mappings
    OTF2_IdMap* mpi_comm_map = OTF2_IdMap_Create( OTF2_ID_MAP_SPARSE, 2 );

    // Each location uses its rank as the communicator id which maps to the global 0
    check_status(OTF2_IdMap_AddIdPair( mpi_comm_map, rank, DUMPI_COMM_WORLD ), "Adding MPI_COMM_WORLD to a mapping table");
    check_status(OTF2_IdMap_AddIdPair( mpi_comm_map, rank + otf2_defs.num_ranks, DUMPI_COMM_SELF ), "Adding MPI_COMM_SELF to a mapping table");
    check_status(OTF2_DefWriter_WriteMappingTable( local_def_writer,
                                      OTF2_MAPPING_COMM,
                                      mpi_comm_map ), "Writing a communicator mapping table to the OTF2 archive");

    OTF2_IdMap_Free(mpi_comm_map);
    check_status(OTF2_Archive_CloseDefWriter( archive, local_def_writer), "Closing a local def writer");
  }
}

void mk_archive_dir(const char *path) {
  struct stat st = {0};

  if (stat(path, &st) == -1) {
    mkdir(path, 0755);
  }
}

/*
 * Implementation of OTF2 def table
 */
OTF2DefTable::OTF2DefTable() {}
int OTF2DefTable::map(std::string string)
{
  auto it = _map.find(string);
  _added_last_lookup = false;
  if ( it != _map.end()) return it->second;
  _map.insert(std::make_pair(string, _counter));
  _added_last_lookup = true;
  return _counter++;
}


const std::string OTF2DefTable::map(int index) {
  // Finds the string corresponding to the given int.
  auto out = std::find_if(_map.begin(), _map.end(), [index] (const auto& elem) { return elem.second == index; });
  return out == _map.end() ? nullptr : out->first;
}

const std::string OTF2DefTable::operator[] (int index) { return map(index); }
int OTF2DefTable::operator[] (const char* index) { return map(std::string(index));}
int OTF2DefTable::operator[] (std::string string) { return map(string); }
int OTF2DefTable::size() { return _counter; }
bool OTF2DefTable::added_last_lookup() {return _added_last_lookup;}
