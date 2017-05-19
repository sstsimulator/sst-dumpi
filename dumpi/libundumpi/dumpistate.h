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

#ifndef DUMPI_LIBUNDUMPI_DUMPISTATE_H
#define DUMPI_LIBUNDUMPI_DUMPISTATE_H

#include <dumpi/common/types.h>
#include <dumpi/common/funclab>ls.h>
#include <dumpi/common/constants.h>
#include <stdint.h>

/**
 * \defgroup dumpistate dumpistate: Trace state information and parsing
 */
/*@{*/   /* this puts the following comments in doxygen group 'dumpistate' */

/**
 * Maintain the state of a DUMPI trace, including communicators created or
 * destroyed, mapping local indices to global indices, managing function
 * names, etc.
 *
 * This was written for the dumpi2otf converter, but should be of general
 * utility for a wider range of converters and trace analysis tools.
 */
typedef struct dumpistate dumpistate;

/**
 * Maintain MPI communicator information for dumpistate communicators.
 * Can be obtained from a properly initialized dumpistate object.
 */
typedef struct dumpistate_comm dumpistate_comm;

/**
 * Maintain MPI group information for dumpistate groups.
 * These objects are unique to each global node rank, since MPI_Group_incl
 * allows the construction of communicators where each node has a unique
 * mapping between local and global indices.
 * Can be obtained from a properly initialized dumpistate object.
 */
typedef struct dumpistate_group dumpistate_group;

/**
 * Maintain MPI type information for dumpistate groups.
 * Can be obtained from a properly initialized dumpistate object.
 */
typedef struct dumpistate_type dumpistate_type;
 
/**
 * Create a new dumpi state.
 * Key information (type sizes etc) are read from the given DUMPI input streams.
 * 
 * All the files are parsed to create a history of MPI communicators
 * and user-defined types created and destroyed during the run.
 * This is necessary (in the general case) because of strange things that can 
 * be done using MPI_Comm_create and MPI_Comm_split, and because an MPI
 * implementation may choose to reuse indices for user-defined types.
 *
 * A non-null object returned from this call can be used as an input argument
 * for all subsequent methods in this header file.
 *
 * \param metafile  The name of a DUMPI metafile describing a parallel run.
 * \param message   If an error occurs on dumpi_state_create and this pointer 
 *                  is non-null, it will be set to point to a dynamically
 *                  allocated buffer with an error message.
 * \return          Non-null object on success.  This object should be freed
 *                  using dumpi_state_free.  Returns NULL and sets message 
 *                  to an error message on failure.
 */
dumpistate* dumpistate_create(const char *metafile, char **message);

/**
 * Get the first timestamp during which this dumpistate was valid.
 * This is the time at which the node in question entered MPI_Init*
 */
const dumpi_clock* dumpistate_mpi_started(const dumpistate *ds, int global_rank);

/**
 * Get the last time at which this dumpistate was valid.
 * This is the time at which the node in question returned from MPI_Init.
 */
const dumpi_clock* dumpistate_mpi_stopped(const dumpistate *ds, int global_rank);

/**
 * Free all memory dynamically allocated to the given dumpi state.
 */
void dumpistate_free(dumpistate *state);

/**
 * Get COMM_WORLD for the given global rank.
 */
const dumpistate_comm* dumpistate_comm_world(const dumpistate *ds, int globalrank);

/**
 * Get the time at which the given communicator was first active.
 * For MPI_COMM_WORLD and MPI_COMM_SELF, time at return from MPI_Init*
 * For all other communicators, this is the time at return from the corresponding
 * create routine (MPI_Comm_create, MPI_Comm_split, MPI_Comm_dup, etc.)
 */
const dumpi_clock* dumpistate_comm_created(const dumpistate_comm *ds);

/**
 * Get the time at which the given communicator was last active.
 * For MPI_COMM_WORLD and MPI_COMM_SELF, time at entry into MPI_Finalize.
 * For all other communicators, this is the time of entry into MPI_Comm_free.
 */
const dumpi_clock* dumpistate_comm_destroyed(const dumpistate_comm *ds);

/**
 * The function call (dumpi identifier) that was used to generate this comm.
 * For MPI_COMM_WORLD and MPI_COMM_SELF, this is MPI_Init.
 * For all other communicators, it is the identifier of the function call
 * (DUMPI_Comm_create, DUMPI_Comm_split, DUMPI_Comm_dup, etc.).
 */
dumpi_function dumpistate_comm_creator(const dumpistate_comm *ds);

/**
 * Query the size of a dumpistate communicator.
 */
int dumpistate_comm_size(const dumpistate_comm *ds);

/**
 * Get the local rank of the host node for this communicator.
 */
int dumpistate_comm_rank(const dumpistate_comm *ds);

/**
 * Query the local rank of the the given global rank in the given communicator.
 * Returns -1 if the given global rank is not a member of the communicator.
 */
int dumpistate_comm_local_rank(const dumpistate_comm *ds, int global_rank);

/**
 * Query the global rank of the the given local rank in the given communicator.
 * Returns -1 if the given local rank is not valid in the the communicator.
 */
int dumpistate_comm_global_rank(const dumpistate_comm *ds, int local_rank);

/**
 * Get the group underlying the given communicator.
 */
dumpistate_group* dumpistate_comm_group(const dumpistate_comm *comm);

/**
 * Get the time at which the given group was first active.
 * For groups underlying MPI_COMM_WORLD and MPI_COMM_SELF, return from MPI_Init*
 * For all other communicators, this is the time at return from the corresponding
 * create routine (MPI_Group_create, ...) or creation time of the parent
 * communicator's group if the input group was not explicitly created
 * (e.g. for MPI_Comm_dup).
 */
const dumpi_clock* dumpistate_group_created(const dumpistate_group *ds);

/**
 * Get the time at which the given group was last active.
 * For groups underlying MPI_COMM_WORLD and MPI_COMM_SELF, entry into MPI_Finalize.
 * For all other communicators, this is the time of entry into MPI_Group_free
 * or MPI_Comm_free (for debugging clarity, this code does not use reference 
 * counting to share actual group objects).
 */
const dumpi_clock* dumpistate_group_destroyed(const dumpistate_group *ds);

/**
 * Query the size of a dumpistate group.
 */
int dumpistate_group_size(const dumpistate_group *ds);

/**
 * Get the local rank of the host node for this group.
 */
int dumpistate_group_rank(const dumpistate_group *ds);

/**
 * Query the local rank of the the given global rank in the given group.
 * Returns -1 if the given global rank is not a member of the group.
 */
int dumpistate_group_local_rank(const dumpistate_group *ds, int global_rank);

/**
 * Query the global rank of the the given local rank in the given group.
 * Returns -1 if the given local rank is not valid in the the group.
 */
int dumpistate_group_global_rank(const dumpistate_group *ds, int local_rank);

/**
 * The prototype for callback functions that report results from parsing
 * one or more streams.
 *
 * \param node_index the index of the current node in MPI_COMM_WORLD.
 * \param func       the dumpi_function index of the current function,
 *                   see common/funclabels.h
 * \param prm        the parameters for the current call,
 *                   see common/argtypes.h
 * \param thread     the locally unique thread index for the calling thread.
 * \param cpu        state of the cpu clock at the start and end of the call,
 *                   all zeroes if cpu timing was not enabled.
 * \param wall       state of the wall clock at the start and end of the call.
 * \param perf       state of performance counters before and after the call,
 *                   NULL if performance counters were not enabled.
 * \param userarg    user argument that was passed into the parse function.
 * \return           1 on success, 0 on failure (will will stop the parser).
 */
typedef int (*dumpistate_callback_fun_t)(int node_index,
					 dumpi_function func,
					 void *prm,
					 uint16_t thread,
					 const dumpi_time *cpu,
					 const dumpi_time *wall,
					 const dumpi_perfinfo *perf,
					 void *userarg);

/**
 * Parse all streams simultaneously and return a collection of callbacks 
 * in a roughly time-sequential manner.  Internally, all times are corrected
 * relative the completion time of MPI_Init to correct for clock skew 
 * between different nodes in the simulation.
 *
 * Because most parallel platforms don't have a globally synchronous clock,
 * the third parameter inserts an implicit synchronization between all streams
 * with the given interval.  Setting this parameter to zero will result in
 * parsing the streams in strictly time-ordered manner, but at the cost of
 * a significant performance hit.
 * 
 * \param state         a valid dumpistate created using dumpistate_create.
 * \param callb         a function called for each function encountered.
 * \param sync_interval interval between implicit synchronization barriers.
 * \param userarg       argument that gets passed to the callback function.
 * \return 1 on success, 0 on failure.
 */
int dumpistate_parse_all(const dumpistate *state,
			 dumpistate_callback_fun_t callb,
			 const dumpi_clock *sync_interval,
			 void *userarg);

/**
 * Parse only the stream for the given MPI world index and 
 * perform callback for each function entered.
 *
 * \param state         a valid dumpistate created using dumpistate_create.
 * \param mpi_index     the index of the node to be parsed.
 * \param callb         a function called for each function encountered.
 * \param userarg       argument that gets passed to the callback function.
 * \return 1 on success, 0 on failure.
 */
int dumpistate_parse_single(const dumpistate *state,
			    int mpi_index,
			    dumpistate_callback_fun_t callb,
			    void *userarg);

/**
 * Get the number of nodes in this state object.
 * Equivalent to calling dumpistate_comm_size(dumpistate_comm_world(0)).
 * \param state      a valid dumpistate object from dumpistate_create.
 * \return           the number of nodes in the trace run.
 */
int dumpistate_world_size(const dumpistate *state);

/**
 * Get the earliest timestamp encountered in the trace for the given
 * node index.  This is the entry time of the first profiled call 
 * (not neccessarily MPI_Init).
 *
 * \param state           a valid dumpistate object from dumpistate_create.
 * \param global_mpi_rank the rank of interest.
 * \return                the start time, or NULL if state or rank are invalid.
 */
const dumpi_clock* dumpistate_start_time(const dumpistate *state,
					 int global_mpi_rank);

/**
 * Get the last timestamp encountered in the trace for the given node index.
 * This is the time of the return time of the last profiled call
 * (not neccessarily MPI_Finalize).
 *
 * \param state           a valid dumpistate object from dumpistate_create.
 * \param global_mpi_rank the rank of interest.
 * \return                the start time, or NULL if state or rank are invalid.
 */
const dumpi_clock* dumpistate_end_time(const dumpistate *state,
					int global_mpi_rank);

/**
 * Access information about the given communicator index at the given time
 * in the trace.
 * 
 * \param state           a valid dumpistate created by dumpistate_create.
 * \param global_mpi_rank the rank of the node to be queried (communicator
 *                        indices may not be consistent across nodes).
 * \param comm            the communicator index as reported by DUMPI.
 * \param at_time         the walltime of interest.
 * \return                the target communicator if it is valid at the given
 *                        time in the trace, NULL if the index is not active.
 *                        This information is internal to the parser, and its
 *                        lifetime extends until dumpistate_free is called.
 */
const dumpistate_comm* dumpistate_get_comm(const dumpistate *state,
					   int global_mpi_rank,
					   dumpi_comm comm,
					   const dumpi_clock *at_time);

/**
 * Access information about the size of the given type in bytes on the
 * given target platform.
 * 
 * In DUMPI versions 0.7 and later, size of built-in types is contained in the
 * trace files.  For earlier versions (which did not have this information), 
 * we are extracting this information from the call trace for user-defined
 * types and relying on a reasonable guess for built-in types.
 *
 * \param state           a valid dumpistate created by dumpistate_create.
 * \param global_mpi_rank the rank of the node to be queried (communicator
 *                        indices may not be consistent across nodes).
 * \param type            the type index as reported by DUMPI.
 * \param at_time         the walltime of interest (MPI implementations may
 *                        re-use indices for user-defined types).
 * \return                the type size in bytes if it is valid at the given
 *                        time in the trace, -1 if the type index is not active.
 */
int dumpistate_get_type_size(const dumpistate *state,
			     int global_mpi_rank,
			     dumpi_datatype type,
			     const dumpi_clock *at_time);

/**
 * Access information about function names (if available) from a DUMPI
 * trace file with call tracing enabled.
 *
 * \param state           a valid dumpistate created by dumpistate_create.
 * \param global_mpi_rank the rank of the node to be queried (function addresses
 *                        may not be consistent across nodes).
 * \param address         function address as reported by DUMPI.
 * \return                the function name if available, blank std::string ("")
 *                        if the function address is valid but does not have
 *                        a name associated with it, and NULL if the function
 *                        address is not recognized.
 *                        This information is internal to the parser, and its
 *                        lifetime extends until dumpistate_free is called.
 */
const char* dumpistate_get_function_name(const dumpistate *state,
					 int global_mpi_rank,
					 uint64_t address);

/*@}*/ /* close doxygen group 'dumpistate' */

#endif /* DUMPI_LIBUNDUMPI_DUMPISTATE_H */