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

#ifndef DUMPI_COMMON_PERFCTRTAGS_H
#define DUMPI_COMMON_PERFCTRTAGS_H

#ifdef __cplusplus
extern "C" {
#endif /* ! __cplusplus */

  /**
   * \addtogroup common_perfctrs
   */
  /*@{*/

  /**
   * The performance counter tags.
   *
   * These correspond exactly to the PAPI performance counters,
   * but I can't rely on the PAPI counters have the same value on
   * different platforms (or across different versions) and we need to
   * be able to parse trace files on systems that don't have PAPI installed,
   * so we remap them to a common (zero-based) indexing scheme.
   */
  typedef enum dumpi_papi_tag {
   DUMPI_PAPI_L1_DCM=0,/*Level 1 data cache misses */
   DUMPI_PAPI_L1_ICM,  /*Level 1 instruction cache misses */
   DUMPI_PAPI_L2_DCM,  /*Level 2 data cache misses */
   DUMPI_PAPI_L2_ICM,  /*Level 2 instruction cache misses */
   DUMPI_PAPI_L3_DCM,  /*Level 3 data cache misses */
   DUMPI_PAPI_L3_ICM,  /*Level 3 instruction cache misses */
   DUMPI_PAPI_L1_TCM,  /*Level 1 total cache misses */
   DUMPI_PAPI_L2_TCM,  /*Level 2 total cache misses */
   DUMPI_PAPI_L3_TCM,  /*Level 3 total cache misses */
   DUMPI_PAPI_CA_SNP,  /*Snoops */
   DUMPI_PAPI_CA_SHR,  /*Request for shared cache line (SMP) */
   DUMPI_PAPI_CA_CLN,  /*Request for clean cache line (SMP) */
   DUMPI_PAPI_CA_INV,  /*Request for cache line Invalidation (SMP) */
   DUMPI_PAPI_CA_ITV,  /*Request for cache line Intervention (SMP) */
   DUMPI_PAPI_L3_LDM,  /*Level 3 load misses */
   DUMPI_PAPI_L3_STM,  /*Level 3 store misses */
   /* 0x10 */
   DUMPI_PAPI_BRU_IDL, /*Cycles branch units are idle */
   DUMPI_PAPI_FXU_IDL, /*Cycles integer units are idle */
   DUMPI_PAPI_FPU_IDL, /*Cycles floating point units are idle */
   DUMPI_PAPI_LSU_IDL, /*Cycles load/store units are idle */
   DUMPI_PAPI_TLB_DM,  /*Data translation lookaside buffer misses */
   DUMPI_PAPI_TLB_IM,  /*Instr translation lookaside buffer misses */
   DUMPI_PAPI_TLB_TL,  /*Total translation lookaside buffer misses */
   DUMPI_PAPI_L1_LDM,  /*Level 1 load misses */
   DUMPI_PAPI_L1_STM,  /*Level 1 store misses */
   DUMPI_PAPI_L2_LDM,  /*Level 2 load misses */
   DUMPI_PAPI_L2_STM,  /*Level 2 store misses */
   DUMPI_PAPI_BTAC_M,  /*BTAC miss */
   DUMPI_PAPI_PRF_DM,  /*Prefetch data instruction caused a miss */
   DUMPI_PAPI_L3_DCH,  /*Level 3 Data Cache Hit */
   DUMPI_PAPI_TLB_SD,  /*Xlation lookaside buffer shootdowns (SMP) */
   DUMPI_PAPI_CSR_FAL, /*Failed store conditional instructions */
   /* 0x20 */
   DUMPI_PAPI_CSR_SUC, /*Successful store conditional instructions */
   DUMPI_PAPI_CSR_TOT, /*Total store conditional instructions */
   DUMPI_PAPI_MEM_SCY, /*Cycles Stalled Waiting for Memory Access */
   DUMPI_PAPI_MEM_RCY, /*Cycles Stalled Waiting for Memory Read */
   DUMPI_PAPI_MEM_WCY, /*Cycles Stalled Waiting for Memory Write */
   DUMPI_PAPI_STL_ICY, /*Cycles with No Instruction Issue */
   DUMPI_PAPI_FUL_ICY, /*Cycles with Maximum Instruction Issue */
   DUMPI_PAPI_STL_CCY, /*Cycles with No Instruction Completion */
   DUMPI_PAPI_FUL_CCY, /*Cycles with Maximum Instruction Completion */
   DUMPI_PAPI_HW_INT,  /*Hardware interrupts */
   DUMPI_PAPI_BR_UCN,  /*Unconditional branch instructions executed */
   DUMPI_PAPI_BR_CN,   /*Conditional branch instructions executed */
   DUMPI_PAPI_BR_TKN,  /*Conditional branch instructions taken */
   DUMPI_PAPI_BR_NTK,  /*Conditional branch instructions not taken */
   DUMPI_PAPI_BR_MSP,  /*Conditional branch instructions mispred */
   DUMPI_PAPI_BR_PRC,  /*Conditional branch instructions corr. pred */
   /* 0x30 */
   DUMPI_PAPI_FMA_INS, /*FMA instructions completed */
   DUMPI_PAPI_TOT_IIS, /*Total instructions issued */
   DUMPI_PAPI_TOT_INS, /*Total instructions executed */
   DUMPI_PAPI_INT_INS, /*Integer instructions executed */
   DUMPI_PAPI_FP_INS,  /*Floating point instructions executed */
   DUMPI_PAPI_LD_INS,  /*Load instructions executed */
   DUMPI_PAPI_SR_INS,  /*Store instructions executed */
   DUMPI_PAPI_BR_INS,  /*Total branch instructions executed */
   DUMPI_PAPI_VEC_INS, /*Vector/SIMD instructions executed */
   DUMPI_PAPI_RES_STL, /*Cycles processor is stalled on resource */
   DUMPI_PAPI_FP_STAL, /*Cycles any FP units are stalled */
   DUMPI_PAPI_TOT_CYC, /*Total cycles */
   DUMPI_PAPI_LST_INS, /*Total load/store inst. executed */
   DUMPI_PAPI_SYC_INS, /*Sync. inst. executed */
   DUMPI_PAPI_L1_DCH,  /*L1 D Cache Hit */
   DUMPI_PAPI_L2_DCH,  /*L2 D Cache Hit */
   /* 0x40 */
   DUMPI_PAPI_L1_DCA,  /*L1 D Cache Access */
   DUMPI_PAPI_L2_DCA,  /*L2 D Cache Access */
   DUMPI_PAPI_L3_DCA,  /*L3 D Cache Access */
   DUMPI_PAPI_L1_DCR,  /*L1 D Cache Read */
   DUMPI_PAPI_L2_DCR,  /*L2 D Cache Read */
   DUMPI_PAPI_L3_DCR,  /*L3 D Cache Read */
   DUMPI_PAPI_L1_DCW,  /*L1 D Cache Write */
   DUMPI_PAPI_L2_DCW,  /*L2 D Cache Write */
   DUMPI_PAPI_L3_DCW,  /*L3 D Cache Write */
   DUMPI_PAPI_L1_ICH,  /*L1 instruction cache hits */
   DUMPI_PAPI_L2_ICH,  /*L2 instruction cache hits */
   DUMPI_PAPI_L3_ICH,  /*L3 instruction cache hits */
   DUMPI_PAPI_L1_ICA,  /*L1 instruction cache accesses */
   DUMPI_PAPI_L2_ICA,  /*L2 instruction cache accesses */
   DUMPI_PAPI_L3_ICA,  /*L3 instruction cache accesses */
   DUMPI_PAPI_L1_ICR,  /*L1 instruction cache reads */
   /* 0x50 */
   DUMPI_PAPI_L2_ICR,  /*L2 instruction cache reads */
   DUMPI_PAPI_L3_ICR,  /*L3 instruction cache reads */
   DUMPI_PAPI_L1_ICW,  /*L1 instruction cache writes */
   DUMPI_PAPI_L2_ICW,  /*L2 instruction cache writes */
   DUMPI_PAPI_L3_ICW,  /*L3 instruction cache writes */
   DUMPI_PAPI_L1_TCH,  /*L1 total cache hits */
   DUMPI_PAPI_L2_TCH,  /*L2 total cache hits */
   DUMPI_PAPI_L3_TCH,  /*L3 total cache hits */
   DUMPI_PAPI_L1_TCA,  /*L1 total cache accesses */
   DUMPI_PAPI_L2_TCA,  /*L2 total cache accesses */
   DUMPI_PAPI_L3_TCA,  /*L3 total cache accesses */
   DUMPI_PAPI_L1_TCR,  /*L1 total cache reads */
   DUMPI_PAPI_L2_TCR,  /*L2 total cache reads */
   DUMPI_PAPI_L3_TCR,  /*L3 total cache reads */
   DUMPI_PAPI_L1_TCW,  /*L1 total cache writes */
   DUMPI_PAPI_L2_TCW,  /*L2 total cache writes */
   /* 0x60 */
   DUMPI_PAPI_L3_TCW,  /*L3 total cache writes */
   DUMPI_PAPI_FML_INS, /*FM ins */
   DUMPI_PAPI_FAD_INS, /*FA ins */
   DUMPI_PAPI_FDV_INS, /*FD ins */
   DUMPI_PAPI_FSQ_INS, /*FSq ins */
   DUMPI_PAPI_FNV_INS, /*Finv ins */
   DUMPI_PAPI_FP_OPS,  /*Floating point operations executed */
   /* Operations only supported on BGL */
   DUMPI_PAPI_BGL_OED,     /*Oedipus operations */
   DUMPI_PAPI_BGL_TS_32B , /*Torus 32B chunks sent */
   DUMPI_PAPI_BGL_TS_FULL, /*Torus no token UPC cycles */ 
   DUMPI_PAPI_BGL_TR_DPKT, /*Tree 256 byte packets */
   DUMPI_PAPI_BGL_TR_FULL, /*UPC cycles (CLOCKx2) tree rcv is full */
   /* Done */
   DUMPI_PAPI_END,          /*This should always be here! */
   DUMPI_FIRST_USER_PERFCTR /* Native performance counters numbered from here */
  } dumpi_papi_tag;

  /** This gets called automatically if needed */
  void dumpi_init_perfctr_tags(void);

  /** Get the std::string name of a perfcounter tag. */
  const char* dumpi_perfctr_name(int tag);

  /** Get the PAPI label corresponding to a DUMPI_PAPI label */
  int dumpi_get_event_code(dumpi_papi_tag tag);

  /*@}*/
  
#ifdef __cplusplus
} /* end of extern "C" block */
#endif /* ! __cplusplus */

#endif /* DUMPI_COMMON_PERFCTRTAGS_H */