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
#include <dumpi/bin/metadata.h>
#include <dumpi/libundumpi/bindings.h>

#include <glob.h>
#include <string.h>
#include <getopt.h>
#include <assert.h>
#include <cstdlib>
#include <algorithm>

static int parse_cli_options(int argc, char **argv, d2o2opt *opt);
static std::vector<std::string> glob_files(const char* path);
static void read_header(const dumpi_header *head);
static void register_type_sizes(dumpi_profile *profile, dumpi::OTF2_Writer* writer);
static void mk_archive_dir(const char* path);
static void print_usage();
static std::vector<int> get_type_sizes(dumpi_profile*);

/** These are stateless lambdas and can be global variables */
static libundumpi_callbacks first_pass_cback, second_cback;

int run_second_pass(dumpi::OTF2_Writer& writer, int rank, dumpi::metadata& md,
                    int terminate_percent)
{
  auto profile = undumpi_open(md.tracename(rank).c_str());

  std::cout << "Terminate at " << terminate_percent << std::endl;
  if (terminate_percent < 100){
    profile->terminate_pos = (profile->total_file_size * terminate_percent) / 100;
  }
  undumpi_read_stream(profile, &second_cback, (void*)&writer, true);
  undumpi_close(profile);
  return 0;
}

struct active_profile {
  dumpi_profile* profile;
  dumpi::OTF2_Writer* writer;
};

static int add_new_comm(int global_id_offset, std::vector<active_profile>& creators)
{
  if (creators.empty()){
    return 0;
  }
  dumpi::OTF2_MPI_Comm::shared_ptr first = creators[0].writer->pending_comm();
  if (first->parent->group->size() > creators.size()){
    return 0; //this is not ready yet
  }

  int next_id = global_id_offset;
  if (first->is_split){
    std::map<int, std::vector<dumpi::OTF2_Writer*>> color_map;
    for (active_profile& prof : creators){
      dumpi::OTF2_MPI_Comm::shared_ptr comm = prof.writer->pending_comm();
      if (comm->color != DUMPI_UNDEFINED)
        color_map[comm->color].emplace_back(prof.writer);
    }

    auto sorter = [](const dumpi::OTF2_Writer* lw, const dumpi::OTF2_Writer* rw){
      dumpi::OTF2_MPI_Comm* l = lw->pending_comm().get();
      dumpi::OTF2_MPI_Comm* r = rw->pending_comm().get();
      if (l->key != r->key) return l->key < r->key;
      else return l->parent->local_rank < r->parent->local_rank;
    };

    for (auto& pair : color_map){
      int color = pair.first;
      auto& vec = pair.second;
      std::sort(vec.begin(), vec.end(), sorter);
      int new_rank = 0;
      std::vector<int> global_ranks(vec.size());
      for (dumpi::OTF2_Writer* writer : vec){
        dumpi::OTF2_MPI_Comm::shared_ptr comm = writer->pending_comm();
        global_ranks[new_rank] = comm->world_rank;
        comm->local_rank = new_rank++;
        comm->global_id = next_id;
      }

      for (dumpi::OTF2_Writer* writer : vec){
        dumpi::OTF2_MPI_Comm::shared_ptr comm = writer->pending_comm();
        dumpi::OTF2_MPI_Group::shared_ptr group = writer->make_comm_split(comm, global_ranks);
        comm->group = group;
        group->global_id = writer->global_group_id_from_comm_id(comm->global_id);
        writer->clear_pending_comm();
      }
      ++next_id;
    }
    return next_id - global_id_offset;
  } else {
    //okay - groups already created and everything is good
    //just need to assign the global id
    for (active_profile& prof : creators){
      prof.writer->pending_comm()->global_id = next_id;
      prof.writer->pending_comm()->group->global_id =
            prof.writer->global_group_id_from_comm_id(next_id);
      prof.writer->clear_pending_comm();
    }
    return 1;
  }
}

int main(int argc, char **argv)
{
  d2o2opt opt;
  dumpi::OTF2_Writer writer;

  if(parse_cli_options(argc, argv, &opt) != 0) return 1;

  libundumpi_clear_callbacks(&first_pass_cback);
  libundumpi_clear_callbacks(&second_cback);
  set_first_pass_callbacks(&first_pass_cback);
  set_callbacks(&second_cback);

  if (opt.dumpi_meta.empty()){
    fprintf(stderr, "Must specify input DUMPI meta file (-i)\n");
    return 1;
  }

  dumpi::metadata md(opt.dumpi_meta);

  std::vector<dumpi::OTF2_Writer> writers(md.numTraces());

  // Initialize the writer
  writer.set_verbosity(opt.verbose ? dumpi::OWV_INFO : dumpi::OWV_WARN);


  // Loop over trace files. Dumpi creates one trace file per MPI rank
  // The first pass constructs information about Communicators, Groups, and type sizes.
  if (opt.print_progress) printf("Identifying Communicators, Groups, and types\n");

  std::vector<active_profile> active_profiles(md.numTraces());
  for (int rank=0; rank < md.numTraces(); ++rank){
    dumpi::OTF2_Writer& writer = writers[rank];
    writer.set_write_global_comms(true); //remapping will not work - ids get reused
    writer.register_comm_world(DUMPI_COMM_WORLD, md.numTraces(), rank);
    writer.register_comm_self(DUMPI_COMM_SELF);
    writer.register_comm_null(DUMPI_COMM_NULL);
    writer.register_comm_error(DUMPI_COMM_ERROR);
    writer.register_null_request(DUMPI_REQUEST_NULL);
    writer.set_clock_resolution(1E9);

    active_profile& active = active_profiles[rank];
    std::string trace_file = md.tracename(rank);
    active.profile = undumpi_open(trace_file.c_str());
    if (!active.profile){
      throw std::runtime_error("failed opening trace " + trace_file);
    }
    active.writer = &writer;

    // this has to come here after undumpi_open
    register_type_sizes(active.profile, &writer);

    dumpi_start_stream_read(active.profile);
  }

  std::map<int, std::vector<active_profile>> pending_comm_creates;

  int num_finished = 0;
  int finalized;
  int comm_id_counter = dumpi::OTF2_Writer::MPI_COMM_USER_ID_OFFSET;
  libundumpi_cbpair first_pass_callarr[DUMPI_END_OF_STREAM] = {{NULL, NULL}};
  libundumpi_populate_callbacks(&first_pass_cback, first_pass_callarr);

  std::cout << "Executing first pass to construct communicators" << std::endl;

  while(num_finished < md.numTraces()){
    if (active_profiles.empty()){
      for (auto iter = pending_comm_creates.begin(); iter != pending_comm_creates.end(); ++iter){
        int num_created = add_new_comm(comm_id_counter, iter->second);
        if (num_created > 0){
          auto& collective_profiles = iter->second;
          active_profiles.insert(active_profiles.end(),
                                 collective_profiles.begin(), collective_profiles.end());
          pending_comm_creates.erase(iter);
          comm_id_counter += num_created;
          std::cout << comm_id_counter << " communicators constructed " << std::endl;
          break;
        }
      }
    } else {
      active_profile active = active_profiles.back();
      active_profiles.pop_back();

      if (active.writer->pending_comm()){
        std::cerr << "Rank " << active.writer->world_rank()
              << " has pending comm but is still in active list " << std::endl;
        abort();
      }

      int stream_active = 1;
      if (opt.percent < 100){
        active.profile->terminate_pos = (active.profile->terminate_pos * opt.percent) / 100;
      }
      while (!active.writer->pending_comm()
             && stream_active
             && active.profile->pos < active.profile->terminate_pos){
        stream_active = undumpi_read_single_call(active.profile, first_pass_callarr, active.writer,
                                            &finalized);
      }

      if (stream_active && active.profile->pos < active.profile->terminate_pos){
        //we have more calls, but progress has stalled on a collective
        int global_id = active.writer->pending_comm()->parent->global_id;
        pending_comm_creates[global_id].push_back(active);
      } else {
        //nope, stream is over
        undumpi_close(active.profile);
        ++num_finished;
      }
    }
  }


  std::cout << "Executing second pass to build traces" << std::endl;

  std::string traceFolder = opt.output_archive.empty() ? md.filePrefix() + "-otf2" : opt.output_archive;
  std::vector<int> event_counts(md.numTraces());

  uint64_t min_start_time = std::numeric_limits<uint64_t>::max();
  uint64_t max_stop_time = std::numeric_limits<uint64_t>::min();

  std::vector<dumpi::OTF2_MPI_Comm::shared_ptr> unique_comms;
  int old_percent = 0;
  for (int rank = 0; rank < md.numTraces(); rank++) {
    dumpi::OTF2_Writer& writer = writers[rank];

    auto& writer_unique_comms = writer.unique_comms();
    unique_comms.insert(unique_comms.end(), writer_unique_comms.begin(), writer_unique_comms.end());
    writer.reverse_versions();

    if (writer.open_archive(traceFolder) != dumpi::OTF2_WRITER_SUCCESS) {
      fprintf(stderr, "Error opening the archive for rank %d\n", rank);
      return 1;
    }

    int rc = run_second_pass(writer, rank, md, opt.percent);
    if (rc != 0){
      fprintf(stderr, "Error writing DUMPI rank %d into OTF2 archive\n", rank);
      return 1;
    }

    int new_percent = ((1 + rank)*100)/md.numTraces();
    if (new_percent != old_percent){
      std::cout << "Pass " << new_percent << "% complete" << std::endl;
      old_percent = new_percent;
    }

    event_counts[rank] = writer.event_count();
    writer.write_local_def_file();
    if (rank > 0) writer.close_archive(); //rank 0 is special

    min_start_time = std::min(min_start_time, writer.start_time());
    max_stop_time = std::max(max_stop_time, writer.stop_time());
  }

  std::cout << "Writing definition files" << std::endl;

  writers[0].write_global_def_file(event_counts, unique_comms,
                                   min_start_time, max_stop_time);
  writers[0].close_archive();
  return 0;
}

int parse_cli_options(int argc, char **argv, d2o2opt* settings) {
  int opt;
  bool output_set = false;
  bool input_set = false;

  //set defaults
  settings->percent = 100;
  settings->help = 0;
  settings->verbose = 0;
  settings->print_progress = false;


  assert(settings != NULL);
  while((opt = getopt(argc, argv, "vhpi:o:t:")) != -1) {
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
          settings->dumpi_meta = optarg;
          input_set = true;
          break;
        case 'o':
          settings->output_archive = optarg;
          output_set = true;
          break;
        case 'p':
          settings->print_progress = true;
          break;
        case 't':
          settings->percent = atoi(optarg);
          break;
        default:
          fprintf(stderr, "Invalid argument %c.\n", opt);
          break;
        }
    }

  if (!input_set) {
    printf("Error: No Dumpi trace path set. Use '-i'\n\n");
    print_usage();
    return 1;
  }

  /** We can sensible choose a default
  if (!output_set) {
    printf("%s", "Error: no OTF2 archive destination set. Use '-o'\n\n");
    print_usage();
    return 0;
  }
  */

  return 0;
}

void print_usage() {
  printf("Usage:  dumpi2otf2 [-h] [-v] [-p] [-i] archive [-o] archive\n"
        "   Options:\n"
        "        -h               Print this help\n"
        "        -v               Verbose status output\n"
        "        -p               Print progress\n"
        "        -i  archive      Path to Dumpi tracefile\n"
        "        -o  archive      Output OTF2 archive name\n");
}

static void
register_type_sizes(dumpi_profile *profile, dumpi::OTF2_Writer* writer)
{
  dumpi_sizeof sizes = undumpi_read_datatype_sizes(profile);
  for(int i = 0; i < sizes.count; i++){
    writer->register_type(i, sizes.size[i]);
  }
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
