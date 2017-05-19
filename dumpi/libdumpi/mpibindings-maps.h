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

#ifndef DUMPI_LIBDUMPI_MPIBINDINGS_MAPS_H
#define DUMPI_LIBDUMPI_MPIBINDINGS_MAPS_H

#include <dumpi/common/hashmap.h>
#include <dumpi/common/types.h>
#include <dumpi/libdumpi/data.h>
#include <dumpi/common/constants.h>
#include <dumpi/common/debugflags.h>
#include <dumpi/dumpiconfig.h>
#include <mpi.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \ingroup libdumpi_internal
   * \defgroup libdumpi_internal_maps libdumpi/internal/maps \
   *    Hash maps for storing compact info about objects with built-in MPI types
   */
  /*@{*/ /* define scope for doxygen comments */

  /**
   * Initialize all hash maps.
   */
  void libdumpi_init_maps(void);

  /**
   * Free dynamically allocated memory from the hash maps.
   */
  void libdumpi_free_maps(void);

  /**
   * Get a dumpi communicator index corresponding to the given MPI_Comm.
   */
  dumpi_comm libdumpi_get_comm(MPI_Comm comm);

  /**
   * Get a dumpi datatype index corresponding to the given MPI_Datatype.
   */
  dumpi_datatype libdumpi_get_datatype(MPI_Datatype dtype);

  /**
   * Get a dumpi errhandler index corresponding to the given MPI_Errhandler
   */
  dumpi_datatype libdumpi_get_errhandler(MPI_Errhandler ehandler);

  /**
   * Get a dumpi file index corresponding to the given MPI_File.
   */
  dumpi_datatype libdumpi_get_file(MPI_File mfile);

  /**
   * Get a dumpi group index corresponding to the given MPI_Group.
   */
  dumpi_datatype libdumpi_get_group(MPI_Group mgroup);

  /**
   * Get a dumpi info index corresponding to the given MPI_Info.
   */
  dumpi_datatype libdumpi_get_info(MPI_Info minfo);

  /**
   * Get a dumpi op index corresponding to the given MPI_Op.
   */
  dumpi_datatype libdumpi_get_op(MPI_Op mop);

  /*
   * Get a dumpi request index corresponding to the given MPIO_Request.
   */
  dumpi_datatype libdumpi_get_iorequest(MPIO_Request mrequest);

  /**
   * Get a dumpi request index corresponding to the given MPI_Request.
   */
  dumpi_datatype libdumpi_get_request(MPI_Request mrequest);
  
  /**
   * Get a dumpi keyval index corresponding to the given MPI key (integer).
   */
  dumpi_datatype libdumpi_get_keyval(int mkeyval);

#if DUMPI_HAVE_MPI_WIN

  /**
   * Get a dumpi win index corresponding to the given MPI_Win.
   */
  dumpi_datatype libdumpi_get_win(MPI_Win mwin);

#endif /* ! DUMPI_HAVE_MPI_WIN */

  /*@}*/ /* close scope of doxygen comments */

#ifdef __cplusplus
} /* End of extern C block */
#endif /* ! __cplusplus */

#endif /* ! DUMPI_LIBDUMPI_MPIBINDINGS_MAPS_H */