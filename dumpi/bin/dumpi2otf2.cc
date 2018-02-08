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


/*
 * Program options struct.
 */
typedef struct d2o2opt {
  int verbose, help;
  const char *file;
} d2o2opt;

static int parse_options(int argc, char **argv, d2o2opt *opt);
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
 * - Parse dumpi datatypes into correct byte sizes (see dumpi2asci -X)
 * - Implement wait functions
 * - Implement collectives and pt2pt callbacks
 */
int main(int argc, char **argv) {

  dumpi_profile *profile;
  libundumpi_callbacks cback;
  d2o2opt opt;
  dumpfh = stdout;

  if(parse_options(argc, argv, &opt) == 0) {
      return 1;
    }
  libundumpi_clear_callbacks(&cback);
  set_callbacks(&cback);

  OTF2_Archive* archive = init_otf2(1);
  if((profile = undumpi_open(opt.file)) == NULL) {
      return 2;
  }

  //Loop over ranks
  int num_ranks = 1;
  for(int rank = 0; rank < num_ranks; rank++) {
    OTF2_EvtWriter* evt_writer = OTF2_Archive_GetEvtWriter(archive, rank);

    dumpi_header *head = undumpi_read_header(profile);
    read_header(head);
    dumpi_free_header(head);

    d2o2_addr = (d2o2_addrmap*)calloc(1, sizeof(d2o2_addrmap));
    assert(d2o2_addr != NULL);
    dumpi_read_function_addresses(profile, &(d2o2_addr->count),
                                  &(d2o2_addr->address), &(d2o2_addr->name));
    undumpi_read_stream(profile, &cback, (void*)&otf2_defs);
  }

  write_def_table(&otf2_defs, archive);

  for(int i = 0; i < d2o2_addr->count; ++i)
    free(d2o2_addr->name[i]);
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
  while((opt = getopt(argc, argv, "vhf:")) != -1) {
      switch(opt) {
        case 'v':
          // set verbose
          fprintf(stdout, "Setting output to verbose.\n");
          settings->verbose = 1;
          break;
        case 'h':
          fprintf(stderr,
                  "Usage:  %s [-h] [-v] [-f] filename\n"
                  "   Options:\n"
                  "        -h               Print this help\n"
                  "        -v               Verbose status output\n"
                  "        -f  filename     Read the given binary tracefile\n",
                  argv[0]);
          std::exit(0);
        case 'f':
          settings->file = strdup(optarg);
          break;
        default:
          fprintf(stderr, "Invalid argument %c.\n", opt);
          break;
        }
    }
  return 1;
}

void read_header(const dumpi_header *head) {
  /*
    char      version[3];
    uint64_t  starttime;
    char     *hostname;
    char     *username;
    int       meshdim;
    int      *meshcrd;
    int      *meshsize;
  */
  int i;
  time_t timev = (time_t)head->starttime;
  printf("version=%d.%d.%d\n"
          "starttime=%s"
          "hostname=%s\n"
          "username=%s\n"
          "meshdim=%d\n",
          head->version[0], head->version[1], head->version[2],
          ctime(&timev), head->hostname, head->username, head->meshdim);
  printf("meshsize=[");
  for(i = 0; i < head->meshdim; ++i) {
    printf("%d", head->meshsize[i]);
    if(i < (head->meshdim-1)) fprintf(dumpfh, ", ");
  }
  printf("]\n");
  printf("meshcrd=[");
  for(i = 0; i < head->meshdim; ++i) {
    printf("%d", head->meshcrd[i]);
    if(i < (head->meshdim-1)) fprintf(dumpfh, ", ");
  }
  printf("]\n");
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
  OTF2_Archive* archive = OTF2_Archive_Open( "ArchivePath",
                                             "ArchiveName",
                                             OTF2_FILEMODE_WRITE,
                                             1024 * 1024 /* event chunk size */,
                                             4 * 1024 * 1024 /* def chunk size */,
                                             OTF2_SUBSTRATE_POSIX,
                                             OTF2_COMPRESSION_NONE );

  OTF2_Archive_SetFlushCallbacks( archive, &flush_callbacks, NULL );
  OTF2_Archive_OpenEvtFiles(archive);

  return archive;
}

void write_def_table(DumpiArgs* dargs, OTF2_Archive* archive)
{
  OTF2_GlobalDefWriter* gdefwriter = OTF2_Archive_GetGlobalDefWriter(archive);
  mpi_call_map.clear();

  //PARADIGM
  OTF2_GlobalDefWriter_WriteParadigm(gdefwriter,
                                     OTF2_PARADIGM_MPI,
                                     otf2_defs.string[std::string("MPI")],
                                     OTF2_PARADIGM_CLASS_PROCESS);

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

  // SYSTEM_TREE_NODE
  OTF2_GlobalDefWriter_WriteSystemTreeNode( gdefwriter,
                                            0 /* id */,
                                            0 /* name */,
                                            0 /* class */,
                                            OTF2_UNDEFINED_SYSTEM_TREE_NODE /* parent */ );

  //COMM
  //CLOCK_PROPERTIES
  //LocationGroup?
  //Location?

  // STRINGS
  for(int i = 0; i < otf2_defs.string.size(); i++) {
    OTF2_GlobalDefWriter_WriteString(gdefwriter, i, otf2_defs.string[i].c_str());
  }
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
