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

#ifndef DUMPI_BIN_TRACE_H
#define DUMPI_BIN_TRACE_H

#include <dumpi/bin/sharedstate.h>
#include <dumpi/bin/metadata.h>
#include <dumpi/bin/group.h>
#include <dumpi/bin/comm.h>
#include <dumpi/bin/type.h>
#include <dumpi/bin/timeutils.h>
#include <dumpi/libundumpi/libundumpi.h>
#include <dumpi/libundumpi/bindings.h>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>  // off_t

namespace dumpi {

  /**
   * \ingroup dumpi_utilities
   */
  /*@{*/

  /**
   * Maintain state during pre-parsing of DUMPI trace files
   * and allow C++-based analysis utilities to query for various
   * state information during trace file analysis.
   */
  class trace {
  public:
    /// Track the state of preparsing.
    enum state {PREPARSE_FRESH=0,    PREPARSE_BLOCKED,
		PREPARSE_ADVANCED, PREPARSE_READY, PREPARSE_DONE};

  private:
    /// Our global index.
    int index_;
    /// The full (qualified) filename for this trace.
    std::string filename_;
    /// The profile we're working from.
    dumpi_profile *profile_;
    /// The current offset in the profile.
    off_t off_;
    /// Our state of preparsing.
    state state_;
    /// The state we share with all other trace instances.
    sharedstate *shared_;
    /// For each communicator, we store the creation and free time.
    struct commentry {
      dumpi_clock created;
      dumpi_clock freed;
      comm the_comm;
      commentry() {}
    commentry(const comm& cc) : the_comm(cc) {
      created.sec = created.nsec = freed.sec = freed.nsec = 0;
    }
    commentry(const comm& cc, const dumpi_clock &cr) : created(cr), the_comm(cc)
      {
	freed.sec = freed.nsec = 0;
      }
    };
    /// Our communicators (populated during preparse).
    typedef std::multimap<int, commentry> commmap_t;
    commmap_t comms_;
    /// Like for comms, store creation and free time for each comm.
    struct groupentry {
      dumpi_clock created;
      dumpi_clock freed;
      group the_group;
      groupentry() {}
    groupentry(const group& gg) : the_group(gg) {
      created.sec = created.nsec = freed.sec = freed.nsec = 0;
    }
    groupentry(const group& gg, const dumpi_clock &cr) :
      created(cr), the_group(gg)
      {
	freed.sec = freed.nsec = 0;
      }
    };
    /// Our groups (populated during preparse).
    typedef std::multimap<int, groupentry> groupmap_t;
    groupmap_t groups_;
    /// Our types, stored in an analogous way to comms and groups.
    struct typeentry {
      dumpi_clock created;
      dumpi_clock committed;
      dumpi_clock freed;
      type the_type;
      typeentry() {}
    typeentry(const type &tt) : the_type(tt) {
      created.sec = created.nsec = freed.sec = freed.nsec = 0;
      committed.sec = committed.nsec = 0;
    }
    typeentry(const type &tt, const dumpi_clock &cr) :
      created(cr), the_type(tt)
      {
	freed.sec = freed.nsec = 0;
	committed.sec = committed.nsec = 0;
      }
    };
    /// Our types (populated during preparse).
    typedef std::multimap<int, typeentry> typemap_t;
    typemap_t types_;
    /// The callbacks we use during preparsing
    /// (array must be at least of lenght DUMPI_END_OF_STREAM))
    libundumpi_cbpair callarr_[DUMPI_END_OF_STREAM];
    /// Keep track of whether we've hit a blocking call or not.
    bool blocked_;
    /// This is part of the reentry state required for the callback parsing
    /// (maybe I should group this with the cbpair array in dumpi).
    int mpi_finalized_;
    /// The index of any pending communicator (corresponding to a collective op).
    /// Indexed by thread id.
    struct {
      int id;
      commhandle handle;
      dumpi_clock created;
    } pending_communicator_;

    /// Key time stamps.
    dumpi_clock start_time_, stop_time_, init_time_, finalize_time_;

    /// Private function to set up callbacks.
    void setup_callbacks();

    /// Private function to handle state checks before comm creation routines.
    /// Returns the "old" communicator handle.
    inline const comm& comm_creation_precheck(int index);

    // Private functions to handle callbacks.
    static int handle_generic
      (const void *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_init
      (const void *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_finalize
      (const void *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_comm_dup
      (const dumpi_comm_dup *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_comm_create
      (const dumpi_comm_create *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_comm_split
      (const dumpi_comm_split *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_comm_group
      (const dumpi_comm_group *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_comm_free
      (const dumpi_comm_free *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_group_union
      (const dumpi_group_union *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_group_intersection
      (const dumpi_group_intersection *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_group_difference
      (const dumpi_group_difference *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_group_incl
      (const dumpi_group_incl *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_group_excl
      (const dumpi_group_excl *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_group_range_incl
      (const dumpi_group_range_incl *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_group_range_excl
      (const dumpi_group_range_excl *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_group_free
      (const dumpi_group_free *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_contiguous
      (const dumpi_type_contiguous *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_vector
      (const dumpi_type_vector *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_hvector
      (const dumpi_type_hvector *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_indexed
      (const dumpi_type_indexed *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_hindexed
      (const dumpi_type_hindexed *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_struct
      (const dumpi_type_struct *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_dup
      (const dumpi_type_dup *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_create_darray
      (const dumpi_type_create_darray *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_create_hindexed
      (const dumpi_type_create_hindexed *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_create_hvector
      (const dumpi_type_create_hvector *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_create_indexed_block
      (const dumpi_type_create_indexed_block *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_create_resized
      (const dumpi_type_create_resized *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_create_struct
      (const dumpi_type_create_struct *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_create_subarray
      (const dumpi_type_create_subarray *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_commit
      (const dumpi_type_commit *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);
    static int handle_type_free
      (const dumpi_type_free *prm, uint16_t thread,
       const dumpi_time *cpu, const dumpi_time *wall,
       const dumpi_perfinfo *perf, void *userarg);

    /// Initialize type sizes for older trace files (before type size info
    /// was added).
    void assign_default_sizes();

    /// Create a groupentry.  This will fail if there is currently another
    /// active groupunicator with the same index.
    void add_groupentry(int grouphandle, const group &the_group);

    /// Create a commentry.  This will fail if there is currently another
    /// active communicator with the same index.
    void add_commentry(int commhandle, const comm &the_comm);

    /// Create a typeentry.  This will fail if there is currently another
    /// active typeunicator with the same index.
    void add_typeentry(int typehandle, const type &the_type);

    /// Get a groupentry.
    groupentry& get_groupentry(int commhandle);

    /// Get a commentry.
    commentry& get_commentry(int commhandle);

    /// Get a commentry.
    typeentry& get_typeentry(int commhandle);
    
    /// Test whether we have hit a new high or low in timestamps.
    inline void check_time(const dumpi_time *wall);

    /// Mark init time.  Calls check_time.
    inline void check_init(const dumpi_time *wall);

    /// Mark finalize time.  Calls check_time.
    inline void check_finalize(const dumpi_time *wall);

  public:
    /// Creation.
    trace();

    /// Global rank for this trace.
    int rank() const { return index_; }

    /// Initialize the trace.
    void init(sharedstate *shared, const std::string &filename, int index);

    /// Preparse until we hit a blocking call or finish the trace stream.
    state preparse();

    /// Test whether there is a communiator with the given index.
    bool has_comm(int commhandle) const;

    /// Get the newest communicator with the given index.
    const comm& get_comm(int commhandle, bool must_be_active=true) const;

    /// Get a communicator with the given index that was valid at the
    /// given time stamp.
    const comm& get_comm(int commhandle, const dumpi_clock &at_time) const;

    /// Test whether there is a group with the given index.
    bool has_group(int grouphandle) const;

    /// Get the newest group with the given index.
    const group& get_group(int grouphandle, bool must_be_active=true) const;

    /// Get a group with the given index that was valid at the
    /// given time stamp.
    const group& get_group(int grouphandle, const dumpi_clock &at_time) const;

    /// Test whether there is a type with the given index.
    bool has_type(int typehandle) const;

    /// Get the newest type with the given index.
    const type& get_type(int typehandle, bool must_be_active=true) const;

    /// Get a type with the given index that was valid at the
    /// given time stamp.
    const type& get_type(int typehandle, const dumpi_clock &at_time) const;

    /// The first timestamp encountered in the trace.
    const dumpi_clock& start_time() const { return start_time_; }

    /// The last timestamp encountered in the trace.
    const dumpi_clock& stop_time() const { return stop_time_; }

    /// The time at which MPI_Init completed.
    const dumpi_clock& init_time() const { return init_time_; }

    /// The time at which MPI_Finalize entered.
    const dumpi_clock& finalize_time() const { return finalize_time_; }
  };

  /// Utility function to populate a list of trace objects.
  inline void preparse_traces(const metadata &meta, sharedstate *shared,
                              std::vector<trace> &traces)
  {
    traces.clear();
    traces.resize(meta.traces());
    for(size_t i = 0; i < traces.size(); ++i)
      traces.at(i).init(shared, meta.tracename(i), i);
    int active_traces=traces.size();
    // Parse traces until everybody is done.
    while(active_traces) {
      //std::cerr << "TOP OF WHILE LOOP\n";
      int made_progress = 0;
      active_traces = traces.size();
      for(size_t currtrace = 0; currtrace < traces.size(); ++currtrace) {
        //std::cerr << "Preparsing trace " << currtrace << "\n";
        trace::state state = traces.at(currtrace).preparse();
        if(state == trace::PREPARSE_ADVANCED) ++made_progress;
        if(state == trace::PREPARSE_DONE) {
          //std::cerr << "Completed\n";
          --active_traces;
        }
        //std::cerr << "After preparse: state is " << state
        //        << " -- active traces " << active_traces << "\n";
      }
      if(active_traces > 0 && made_progress <= 0) {
        std::cerr << "Error: Still have active traces but all are blocked\n";
        throw "realmain: Deadlocked\n";
      }
    }
  }
  
  /*@}*/

} // end of namespace dumpi

#endif // ! DUMPI_BIN_TRACE_H