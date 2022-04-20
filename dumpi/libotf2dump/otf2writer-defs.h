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

#ifndef OTF2WRITERDEFS_H
#define OTF2WRITERDEFS_H

#include <otf2/otf2.h>
#include <map>
#include <unordered_map>
#include <vector>
#include <functional>
#include <set>
#include <list>
#include <string>


namespace dumpi {

typedef int32_t mpi_request_t;
typedef int32_t mpi_group_t;
typedef int32_t mpi_comm_t;
typedef int16_t mpi_type_t;
typedef uint64_t otf2_time_t;

/**
 * @brief The OTF2_WRITER_RESULT enum
 * Used to classify errors
 */
enum OTF2_WRITER_RESULT {
  OTF2_WRITER_SUCCESS = 0,
  OTF2_WRITER_ERROR_DIRECTORY_ALREADY_EXISTS,
  OTF2_WRITER_ERROR_DIRECTORY_PATH_DOES_NOT_EXIST,
  OTF2_WRITER_ERROR_NOT_IMPLEMENTED,
  OTF2_WRITER_ERROR_OUT_OF_RANGE,
  OTF2_WRITER_ERROR_NO_RANK_SET,
  OTF2_WRITER_ERROR_WRITER_NOT_SET,
  OTF2_WRITER_ERROR_ARCHIVE_ALREADY_OPEN,
  OTF2_WRITER_ERROR_UKNOWN_MPI_GROUP,
  OTF2_WRITER_ERROR_UKNOWN_MPI_COMM,
  OTF2_WRITER_ERROR_UKNOWN_MPI_TYPE
};

/**
 * @brief The OTF2_WRITER_VERBOSITY enum
 * Determines output verbosity. All output is written to standard out.
 */
enum OTF2_WRITER_VERBOSITY {
  OWV_NONE = 0,
  OWV_ABORT,
  OWV_ERROR,
  OWV_WARN,
  OWV_INFO
};

/**
 * @brief The REQUEST_TYPE enum
 * Internal structure for async MPI calls
 */
enum REQUEST_TYPE {
  REQUEST_TYPE_ISEND = 0,
  REQUEST_TYPE_IRECV
};

/**
 * @brief The COMM_EVENT_TYPE enum
 * Internal structure for MPI_Comm_split and MPI_Comm_create collectives
 */
enum COMM_EVENT_TYPE {
  CET_COMM_SPLIT = 0,
  CET_COMM_CREATE
};

/**
 * @brief The irecv_capture struct
 * Internal structure for async MPI calls
 */
struct irecv_capture {
  uint64_t bytes_sent;
  uint32_t source;
  uint32_t tag;
  int comm;
  mpi_request_t request;
};

/**
 * @brief The OTF2DefTable class
 * Maps an incrementing id to a string. Helps def file creation, which
 * requires a mapping
 */
class OTF2DefTable {
 public:
  OTF2DefTable();

  int insert(const std::string& str);

  std::string get(int index) const;

  int get(const std::string& str) const;

  int size() const {
    return counter_;
  }

  std::unordered_map<std::string,int>::const_iterator
  begin() const {
    return map_.begin();
  }

  std::unordered_map<std::string,int>::const_iterator
  end() const {
    return map_.end();
  }

 private:
  std::unordered_map<std::string, int> map_;

  int counter_ = 0;
};

}

#endif // OTF2WRITERDEFS_H
