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

#include <dumpi/libundumpi/dumpistate.h>
#include <dumpi/libundumpi/dumpistate-internal-types.h>
#include <dumpi/libundumpi/dumpistate-internal-setup.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Create a new dumpi state.
 */
dumpistate* dumpistate_create(const char *metafile, char **message)
{
  dumpistate *retval = (dumpistate*)calloc(1, sizeof(dumpistate));
  (void)metafile;
  (void)message;
  if(retval == NULL) {
    if(message != NULL) {
      *message = strdup("dumpistate_create:  Failed to allocate dumpistate.");
      assert(*message != NULL);
    }
  }
  else {
    /* This is a little ugly since we don't want to leak memory
     * (it would be a little prettier with goto, though */
    int scratch = dumpistate_read_metafile(metafile, &retval->meta, message);
    if(scratch) scratch = 
		  dumpistate_read_metafile(metafile, &retval->meta, message);
    if(scratch) scratch = dumpistate_alloc_locals(retval, message);
    if(scratch) scratch = dumpistate_preparse_streams(retval, message);
    if(scratch == 0) {
      dumpistate_free(retval);
      retval = NULL;
    }
  }
  return retval;
}

/*
 * Free all memory dynamically allocated to the given dumpi state.
 */
void dumpistate_free(dumpistate *state) {
  fprintf(stderr, "Unimplemented:  dumpistate_free\n");
}

/*
 * Parse all streams simultaneously and return a collection of callbacks 
 * in a roughly time-sequential manner.
 */
int dumpistate_parse_all(const dumpistate *state,
			 dumpistate_callback_fun_t callb,
			 const dumpi_clock *sync_interval,
			 void *userarg)
{
  fprintf(stderr, "Unimplemented:  dumpistate_parse_all\n");
  return 0;
}

/*
 * Parse only the stream for the given MPI world index and 
 * perform callback for each function entered.
 */

int dumpistate_parse_single(const dumpistate *state,
			    int mpi_index,
			    dumpistate_callback_fun_t callb,
			    void *userarg)
{
  fprintf(stderr, "Unimplemented:  dumpistate_parse_single\n");
  return 0;
}

/*
 * Get the number of nodes in this state object.
 * Equivalent to MPI_Comm_size(MPI_COMM_WORLD, &size).
 */
int dumpistate_world_size(const dumpistate *state)
{
  if(state == NULL) {
    fprintf(stderr, __FILE__ ":%d:  dumpistate_world_size:  NULL state pointer\n",
	    __LINE__);
    abort();
  }
  return state->meta.procs;
}

/*
 * Utility function to validate local state.
 */
#define DUMPI_VALIDATE_LOCAL_STATE(STATE, RANK, FUNCTION) do {		\
  if((STATE) == NULL) {							\
    fprintf(stderr, __FILE__ ":%d: " FUNCTION	": "			\
	    "NULL state pointer.\n", __LINE__);				\
    abort();								\
  }									\
  if(global_mpi_rank < 0 || global_mpi_rank >= state->meta.procs) {	\
    fprintf(stderr, __FILE__ ":%d: " FUNCTION ": "			\
	    "Invalid MPI rank %d\n", __LINE__, global_mpi_rank);	\
  }									\
  assert(state->state != NULL);						\
} while(0)

/*
 * Get the earliest timestamp encountered in the trace for the given
 * node index.
 */
const dumpi_clock* dumpistate_start_time(const dumpistate *state,
					 int global_mpi_rank)
{
  DUMPI_VALIDATE_LOCAL_STATE(state, global_mpi_rank, "dumpistate_start_time");
  return &state->state[global_mpi_rank].time_bounds.start;
}

/*
 * Get the last timestamp encountered in the trace for the given node index.
 */
const dumpi_clock* dumpistate_end_time(const dumpistate *state,
				       int global_mpi_rank)
{
  DUMPI_VALIDATE_LOCAL_STATE(state, global_mpi_rank, "dumpistate_end_time");
  return &state->state[global_mpi_rank].time_bounds.stop;
}

/*
 * Test whether one dumpi_clock is less or equal to another.
 */
static inline int time_le(const dumpi_clock *a, const dumpi_clock *b) {
  if(a->sec != b->sec)
    return a->sec < b->sec;
  else
    return a->nsec <= b->nsec;
}

/*
 * Test whether one dumpi_clock is greater or equal to another.
 */
static inline int time_ge(const dumpi_clock *a, const dumpi_clock *b) {
  if(a->sec != b->sec)
    return a->sec > b->sec;
  else
    return a->nsec >= b->nsec;
}

/*
 * Utility function to test whether a given dumpi_clock falls within a
 * dumpi_time range
 */
static inline int active_at_time(const dumpi_clock *begin,
				 const dumpi_clock *end,
				 const dumpi_clock *time)
{
  return (time_ge(time, begin) && (time_le(end, begin) || time_le(time, end)));
}

/*
 * Access information about the given communicator index at the given time
 * in the trace.
 */
const dumpistate_comm* dumpistate_get_comm(const dumpistate *state,
					   int global_mpi_rank,
					   dumpi_comm comm,
					   const dumpi_clock *at_time)
{
  const dumpistate_comm *retval = NULL;
  DUMPI_VALIDATE_LOCAL_STATE(state, global_mpi_rank, "dumpistate_get_comm");
  /* We could technically return NULL here, but this is almost
   * certainly a programming error or corrupt input */
  if(comm < 0 || comm >= state->state[global_mpi_rank].comms.length) {
    fprintf(stderr, __FILE__ ":%d: dumpistate_get_comm:  "
	    "Communicator %d was never encountered for node rank %d\n",
	    __LINE__, comm, global_mpi_rank);
    abort();
  }
  {
    /* Traverse the linked list for this node */
    const dumpistate_comm_ll *ll = state->state[global_mpi_rank].comms.entry[comm];
    while(ll != NULL) {
      /* Test whether this communiator was active during at_time */
      if(active_at_time(&ll->comm.committed, &ll->comm.freed, at_time)) {
	retval = &ll->comm;
	break;
      }
      ll = ll->next;
    }
  }
  return retval;
}

/*
 * Access information about the size of the given type in bytes on the
 * given target platform.
 */
int dumpistate_get_type_size(const dumpistate *state,
			     int global_mpi_rank,
			     dumpi_datatype type,
			     const dumpi_clock *at_time)
{
  const dumpistate_type *retval = NULL;
  (state, global_mpi_rank, "dumpistate_get_type_size");
  /* We could technically return NULL here, but this is almost
   * certainly a programming error or corrupt input */
  if(type < 0 || type >= state->state[global_mpi_rank].types.length) {
    fprintf(stderr, __FILE__ ":%d: dumpistate_get_type_size:  "
	    "Type %d was never encountered for node rank %d\n",
	    __LINE__, type, global_mpi_rank);
    abort();
  }
  {
    /* Traverse the linked list for this node */
    const dumpistate_type_ll *ll = state->state[global_mpi_rank].types.entry[type];
    while(ll != NULL) {
      /* Test whether this type was active during at_time */
      if(active_at_time(&ll->type.created, &ll->type.freed, at_time)) {
	retval = &ll->type;
	break;
      }
      ll = ll->next;
    }
  }
  if(retval) return retval->size;
  else return -1;
}

/*
 * Access information about function names (if available) from a DUMPI
 * trace file with call tracing enabled.
 */
const char* dumpistate_get_function_name(const dumpistate *state,
					  int global_mpi_rank,
					  uint64_t address)
{
  int i;
  const char *retval = NULL;
  DUMPI_VALIDATE_LOCAL_STATE(state, global_mpi_rank, "dumpistate_get_type_size");
  for(i = 0; i < state->state[global_mpi_rank].functions.length; ++i) {
    if(state->state[global_mpi_rank].functions.func[i].address == address) {
      retval = state->state[global_mpi_rank].functions.func[i].name;
      break;
    }
  }
  if(retval) return retval;
  else return NULL;
}
