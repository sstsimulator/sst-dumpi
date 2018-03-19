#include <dumpi/bin/dumpi2otf2-defs.h>
#include <dumpi/libundumpi/callbacks.h>
#include <dumpi/common/argtypes.h>
#include <dumpi/common/constants.h>
#include <assert.h>
#include <otf2/otf2.h>
#include <algorithm>

#include <unordered_map>

d2o2_addrmap *d2o2_addr = NULL;

enum REQUEST_TYPE {
  REQUEST_TYPE_ISEND = 0,
  REQUEST_TYPE_IRECV
};

std::unordered_map<int, dumpi_irecv> irecv_requests;
std::unordered_map<int, REQUEST_TYPE> request_type;

static void incomplete_call(int request_id, REQUEST_TYPE type) {
  request_type[request_id] = type;
}

/*
 * ISends and IRecvs begin when invoked, but important information about
 * them are not recorded by OTF2 until they are completed, which occurs inside of Waits, Tests, etc.
 * Returns the number of events generated
 */
static int complete_call(OTF2_EvtWriter* writer, int request_id, OTF2_TimeStamp timestamp) {
  auto t = request_type.find(request_id);
  int events = 0;
  if (t == request_type.end()) {
    printf("Error: request id (%i) not found\n", request_id);
  }
  else if (t->second == REQUEST_TYPE_ISEND) {
    OTF2_EvtWriter_MpiIsendComplete(writer, nullptr, timestamp, request_id);
    events++;
  }
  else if (t->second == REQUEST_TYPE_IRECV) {
    auto dumpi_irecv_it = irecv_requests.find(request_id);
    dumpi_irecv irecv = dumpi_irecv_it->second;
    if(dumpi_irecv_it == irecv_requests.end()) {
      printf("Error: Request #(%i) not found while trying to complete MPI_IRecv\n", request_id);
    } else {
      OTF2_EvtWriter_MpiIrecv(writer, nullptr, timestamp, irecv.source, irecv.comm, irecv.tag, irecv.count, request_id);
      events++;
    }
  }
  return events;
}

int report_MPI_Send(const dumpi_send *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  OTF2_EvtWriter_MpiSend(args->writer, nullptr, DUPMI_TO_OTF2_TIMESTAMP(wall->start), prm->dest, prm->comm, prm->tag, prm->count);
  args->event_count[args->rank]++;
  DUMPI_RETURNING();
}

int report_MPI_Recv(const dumpi_recv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  OTF2_EvtWriter_MpiRecv(args->writer, nullptr, DUPMI_TO_OTF2_TIMESTAMP(wall->start), prm->source, prm->comm, prm->tag, prm->count);
  args->event_count[args->rank]++;
  DUMPI_RETURNING();
}

int report_MPI_Get_count(const dumpi_get_count *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Bsend(const dumpi_bsend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Ssend(const dumpi_ssend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Rsend(const dumpi_rsend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Buffer_attach(const dumpi_buffer_attach *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Buffer_detach(const dumpi_buffer_detach *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Isend(const dumpi_isend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  incomplete_call(prm->request, REQUEST_TYPE_ISEND);
  OTF2_EvtWriter_MpiIsend(args->writer, nullptr, DUPMI_TO_OTF2_TIMESTAMP(wall->start), prm->dest, prm->comm, prm->tag, prm->count, prm->request);
  args->event_count[args->rank]++;
  DUMPI_RETURNING();
}

int report_MPI_Ibsend(const dumpi_ibsend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Issend(const dumpi_issend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Irsend(const dumpi_irsend *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Irecv(const dumpi_irecv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  irecv_requests[prm->request] = *prm;
  incomplete_call(prm->request, REQUEST_TYPE_IRECV);
  OTF2_EvtWriter_MpiIrecvRequest(args->writer, nullptr, DUPMI_TO_OTF2_TIMESTAMP(wall->start), prm->request);
  args->event_count[args->rank]++;
  DUMPI_RETURNING();
}

int report_MPI_Wait(const dumpi_wait *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  args->event_count[args->rank] += complete_call(args->writer, prm->request, DUPMI_TO_OTF2_TIMESTAMP(wall->start));
  DUMPI_RETURNING();
}

int report_MPI_Test(const dumpi_test *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  if (prm->flag)
    args->event_count[args->rank] += complete_call(args->writer, prm->request, DUPMI_TO_OTF2_TIMESTAMP(wall->start));
  DUMPI_RETURNING();
}

int report_MPI_Request_free(const dumpi_request_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Waitany(const dumpi_waitany *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  args->event_count[args->rank] += complete_call(args->writer, prm->requests[prm->index], DUPMI_TO_OTF2_TIMESTAMP(wall->start));
  DUMPI_RETURNING();
}

int report_MPI_Testany(const dumpi_testany *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  if (prm->flag)
    args->event_count[args->rank] += complete_call(args->writer, prm->requests[prm->index], DUPMI_TO_OTF2_TIMESTAMP(wall->start));
  DUMPI_RETURNING();
}

int report_MPI_Waitall(const dumpi_waitall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  int events = 0;
  for (int i = 0; i < prm->count; i++)
    events += complete_call(args->writer, prm->requests[i], DUPMI_TO_OTF2_TIMESTAMP(wall->start));
  args->event_count[args->rank] += events;
  DUMPI_RETURNING();
}

int report_MPI_Testall(const dumpi_testall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  if (prm->flag) {
    int events = 0;
    for (int i = 0; i < prm->count; i++)
      events += complete_call(args->writer, prm->requests[i], DUPMI_TO_OTF2_TIMESTAMP(wall->start));
    args->event_count[args->rank] += events;
  }
  DUMPI_RETURNING();
}

int report_MPI_Waitsome(const dumpi_waitsome *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  int events = 0;
  for (int i = 0; i < prm->outcount; i++) {
      events += complete_call(args->writer, prm->requests[prm->indices[i]], DUPMI_TO_OTF2_TIMESTAMP(wall->start));
  }
  args->event_count[args->rank] += events;
  DUMPI_RETURNING();
}

int report_MPI_Testsome(const dumpi_testsome *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  int events = 0;
  for (int i = 0; i < prm->outcount; i++) {
      events += complete_call(args->writer, prm->requests[prm->indices[i]], DUPMI_TO_OTF2_TIMESTAMP(wall->start));
  }
  args->event_count[args->rank] += events;
  DUMPI_RETURNING();
}

int report_MPI_Iprobe(const dumpi_iprobe *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Probe(const dumpi_probe *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cancel(const dumpi_cancel *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Test_cancelled(const dumpi_test_cancelled *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Send_init(const dumpi_send_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Bsend_init(const dumpi_bsend_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Ssend_init(const dumpi_ssend_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Rsend_init(const dumpi_rsend_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Recv_init(const dumpi_recv_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Start(const dumpi_start *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Startall(const dumpi_startall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Sendrecv(const dumpi_sendrecv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Sendrecv_replace(const dumpi_sendrecv_replace *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_contiguous(const dumpi_type_contiguous *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_vector(const dumpi_type_vector *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_hvector(const dumpi_type_hvector *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_indexed(const dumpi_type_indexed *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_hindexed(const dumpi_type_hindexed *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_struct(const dumpi_type_struct *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Address(const dumpi_address *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_extent(const dumpi_type_extent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_size(const dumpi_type_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_lb(const dumpi_type_lb *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_ub(const dumpi_type_ub *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_commit(const dumpi_type_commit *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_free(const dumpi_type_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Get_elements(const dumpi_get_elements *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Pack(const dumpi_pack *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Unpack(const dumpi_unpack *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Pack_size(const dumpi_pack_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Barrier(const dumpi_barrier *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_BARRIER, 0, 0, 0);
  DUMPI_RETURNING();
}

int report_MPI_Bcast(const dumpi_bcast *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_BCAST, prm->root, prm->count, prm->count);
  DUMPI_RETURNING();
}

int report_MPI_Gather(const dumpi_gather *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_GATHER,
                     prm->root,
                     BYTE_COUNT(prm->sendtype, prm->sendcount),
                     (args->rank == prm->root) ? BYTE_COUNT(prm->recvtype, prm->recvcount) : 0);
  DUMPI_RETURNING();
}

int report_MPI_Gatherv(const dumpi_gatherv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Scatter(const dumpi_scatter *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_SCATTER,
                     prm->root,
                     BYTE_COUNT(prm->sendtype, prm->sendcount),
                     (args->rank == prm->root) ? BYTE_COUNT(prm->recvtype, prm->recvcount) : 0);
  DUMPI_RETURNING();
}

int report_MPI_Scatterv(const dumpi_scatterv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Allgather(const dumpi_allgather *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLGATHER,
                     0, // root is not used for this collective
                     BYTE_COUNT(prm->sendtype, prm->sendcount),
                     BYTE_COUNT(prm->recvtype, prm->recvcount));
  DUMPI_RETURNING();
}

int report_MPI_Allgatherv(const dumpi_allgatherv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Alltoall(const dumpi_alltoall *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLTOALL,
                     0, // root is not used in this collective
                     BYTE_COUNT(prm->sendtype, prm->sendcount),
                     BYTE_COUNT(prm->recvtype, prm->recvcount));
  DUMPI_RETURNING();
}

int report_MPI_Alltoallv(const dumpi_alltoallv *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Reduce(const dumpi_reduce *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();

  auto bytes = BYTE_COUNT(prm->datatype, prm->count);
  bool is_root = args->rank == prm->root;

  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_REDUCE,
                     prm->root,
                     is_root ? 0 : bytes,
                     is_root ? bytes : 0);

  DUMPI_RETURNING();
}

int report_MPI_Op_create(const dumpi_op_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Op_free(const dumpi_op_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Allreduce(const dumpi_allreduce *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_ALLREDUCE,
                     0, // root is not used in this collective
                     BYTE_COUNT(prm->datatype, prm->count),
                     BYTE_COUNT(prm->datatype, prm->count));
  DUMPI_RETURNING();
}

int report_MPI_Reduce_scatter(const dumpi_reduce_scatter *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  int recv_sum = 0;
  for (int i = 0; i < prm->commsize; i++) recv_sum += prm->recvcounts[i];

  COLLECTIVE_WRAPPER(OTF2_COLLECTIVE_OP_REDUCE_SCATTER,
                     0,
                     BYTE_COUNT(prm->datatype, prm->recvcounts[args->rank]), //TODO, get correct rank, instead of global rank.
                     BYTE_COUNT(prm->datatype, recv_sum));
  DUMPI_RETURNING();
}

int report_MPI_Scan(const dumpi_scan *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Group_size(const dumpi_group_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  auto dumpi_size = prm->size;
  auto otf2_size = args->mpi_group[prm->group].size();
  if (dumpi_size != otf2_size) printf("Error: MPI_Group_rank() for ID %i was %i, but expected %i.\n", prm->group, dumpi_size, otf2_size);
  DUMPI_RETURNING();
}

int report_MPI_Group_rank(const dumpi_group_rank *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Group_translate_ranks(const dumpi_group_translate_ranks *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Group_compare(const dumpi_group_compare *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_group(const dumpi_comm_group *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  // NULL indicates invalid parameters
  if (prm->group != DUMPI_COMM_NULL) {
    // Useful for settings MPI_COMM_WORLD's group ID to the implementation's default, otherwise it overwrites known parameters.
    args->mpi_comm[prm->comm].group = prm->group;
  }
  DUMPI_RETURNING();
}

// C++17/20 have nifty STL set operations that operate on arbitrary iterators (std::set_union, std::set_intersection, etc...)
// Unfortunately, this code targets C++11

int report_MPI_Group_union(const dumpi_group_union *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  auto g1_it = args->mpi_group[prm->group1].begin();
  auto g2_it = args->mpi_group[prm->group2].begin();
  auto g1_end = args->mpi_group[prm->group1].end();
  auto g2_end = args->mpi_group[prm->group2].end();
  auto dest = std::back_inserter(args->mpi_group[prm->newgroup]);

  // Set Union
  while (true)
  {
      if (g1_it==g1_end) { std::copy(g2_it,g2_end,dest); break; }
      if (g2_it==g2_end) { std::copy(g1_it,g1_end,dest); break; }

      if (*g1_it<*g2_it) { *dest = *g1_it; ++g1_it; }
      else if (*g2_it<*g1_it) { *dest = *g2_it; ++g2_it; }
      else { *dest = *g1_it; ++g1_it; ++g2_it; }
      ++dest;
  }

  DUMPI_RETURNING();
}

int report_MPI_Group_intersection(const dumpi_group_intersection *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  auto g1_it = args->mpi_group[prm->group1].begin();
  auto g2_it = args->mpi_group[prm->group2].begin();
  auto g1_end = args->mpi_group[prm->group1].end();
  auto g2_end = args->mpi_group[prm->group2].end();
  auto dest = std::back_inserter(args->mpi_group[prm->newgroup]);

  // Set Intersection
  while (g1_it!=g1_end && g2_it!=g2_end)
  {
    if (*g1_it<*g2_it) ++g1_it;
    else if (*g2_it<*g1_it) ++g2_it;
    else {
      *dest = *g1_it;
      ++dest; ++g1_it; ++g2_it;
    }
  }

  DUMPI_RETURNING();
}

int report_MPI_Group_difference(const dumpi_group_difference *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();

  auto g1_it = args->mpi_group[prm->group1].begin();
  auto g2_it = args->mpi_group[prm->group2].begin();
  auto g1_end = args->mpi_group[prm->group1].end();
  auto g2_end = args->mpi_group[prm->group2].end();
  auto dest = std::back_inserter(args->mpi_group[prm->newgroup]);

  // Set Difference
  while (g1_it != g1_end) {
    if (g2_it == g2_end) { std::copy(g1_it, g1_end, dest); break; }

    if (*g1_it < *g2_it) {
      *dest++ = *g1_it++;
    } else {
      if (! (*g2_it < *g1_it)) {
        ++g1_it;
      }
      ++g2_it;
    }
  }

  DUMPI_RETURNING();
}

int report_MPI_Group_incl(const dumpi_group_incl *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();

  // Creates a new group that includes the listed ranks
  auto new_group = args->mpi_group[prm->newgroup];

  for (int i = 0; i < prm->count; i++)
    new_group.push_back(prm->ranks[i]);

  DUMPI_RETURNING();
}

int report_MPI_Group_excl(const dumpi_group_excl *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();

  // Creates a new group that excludes the listed ranks
  auto new_group = args->mpi_group[prm->newgroup];
  auto old_group = args->mpi_group[prm->group];

  for (int i = 0; i < old_group.size(); i++) {
    bool exclude = false;
    for (int j = 0; j < prm->count; j++) {
      if (prm->ranks[j] == i) {
        exclude = true;
        break;
      }
    }
    if (!exclude) new_group.push_back(old_group[i]);
  }

  DUMPI_RETURNING();
}

int report_MPI_Group_range_incl(const dumpi_group_range_incl *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  args->mpi_group[prm->newgroup] = args->mpi_group[prm->group];
  for (int i = 0; i < prm->count; i++)
    for(int j = prm->ranges[i][0]; j < prm->ranges[i][1]; j+=prm->ranges[i][2])
      args->mpi_group[prm->newgroup].push_back(j);
  DUMPI_RETURNING();
}

int report_MPI_Group_range_excl(const dumpi_group_range_excl *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Group_free(const dumpi_group_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  // TODO: What happens if the MPI implementation re-uses the group's id?
  printf("Warning: Group Freed\n");
  DUMPI_RETURNING();
}

int report_MPI_Comm_size(const dumpi_comm_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_rank(const dumpi_comm_rank *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_compare(const dumpi_comm_compare *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_dup(const dumpi_comm_dup *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  args->mpi_comm[prm->newcomm] = args->mpi_comm[prm->oldcomm];
  DUMPI_RETURNING();
}

int report_MPI_Comm_create(const dumpi_comm_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  MPI_comm new_comm = MPI_comm("", prm->oldcomm, prm->newcomm, prm->group);
  args->mpi_comm[prm->newcomm] = new_comm;
  DUMPI_RETURNING();
}

int report_MPI_Comm_split(const dumpi_comm_split *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  // TODO This is an architectural issue. We can't know the result of a comm split until
  // after the def file would have been written. We should not use global def files because of this
  // One fix would be to write each def file individually
  WARN_UNUSED(true);
  DUMPI_RETURNING();
}

int report_MPI_Comm_free(const dumpi_comm_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  // TODO: What happens after a comm is freed? The implementation could reuse the id and overwrite its value in the final Def file.
  WARN_UNUSED(true);
  DUMPI_RETURNING();
}

int report_MPI_Comm_test_inter(const dumpi_comm_test_inter *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_remote_size(const dumpi_comm_remote_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_remote_group(const dumpi_comm_remote_group *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Intercomm_create(const dumpi_intercomm_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Intercomm_merge(const dumpi_intercomm_merge *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Keyval_create(const dumpi_keyval_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Keyval_free(const dumpi_keyval_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Attr_put(const dumpi_attr_put *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Attr_get(const dumpi_attr_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Attr_delete(const dumpi_attr_delete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Topo_test(const dumpi_topo_test *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cart_create(const dumpi_cart_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Dims_create(const dumpi_dims_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Graph_create(const dumpi_graph_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Graphdims_get(const dumpi_graphdims_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Graph_get(const dumpi_graph_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cartdim_get(const dumpi_cartdim_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cart_get(const dumpi_cart_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cart_rank(const dumpi_cart_rank *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cart_coords(const dumpi_cart_coords *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Graph_neighbors_count(const dumpi_graph_neighbors_count *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Graph_neighbors(const dumpi_graph_neighbors *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cart_shift(const dumpi_cart_shift *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cart_sub(const dumpi_cart_sub *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Cart_map(const dumpi_cart_map *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Graph_map(const dumpi_graph_map *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Get_processor_name(const dumpi_get_processor_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Get_version(const dumpi_get_version *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Errhandler_create(const dumpi_errhandler_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Errhandler_set(const dumpi_errhandler_set *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Errhandler_get(const dumpi_errhandler_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Errhandler_free(const dumpi_errhandler_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Error_string(const dumpi_error_string *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Error_class(const dumpi_error_class *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Wtime(const dumpi_wtime *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
  (void)prm;
}

int report_MPI_Wtick(const dumpi_wtick *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
  (void)prm;
}

int report_MPI_Init(const dumpi_init *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  // Record the earliest start time
  args->start_time = std::min(args->start_time, DUPMI_TO_OTF2_TIMESTAMP(wall->start));
  DUMPI_RETURNING();
}

int report_MPI_Finalize(const dumpi_finalize *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  // Record the latest end time
  args->stop_time = std::max(args->stop_time, DUPMI_TO_OTF2_TIMESTAMP(wall->stop));
  DUMPI_RETURNING();
}

int report_MPI_Initialized(const dumpi_initialized *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Abort(const dumpi_abort *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Close_port(const dumpi_close_port *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_accept(const dumpi_comm_accept *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_connect(const dumpi_comm_connect *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_disconnect(const dumpi_comm_disconnect *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_get_parent(const dumpi_comm_get_parent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_join(const dumpi_comm_join *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_spawn(const dumpi_comm_spawn *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_spawn_multiple(const dumpi_comm_spawn_multiple *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Lookup_name(const dumpi_lookup_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Open_port(const dumpi_open_port *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Publish_name(const dumpi_publish_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Unpublish_name(const dumpi_unpublish_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Accumulate(const dumpi_accumulate *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Get(const dumpi_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Put(const dumpi_put *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_complete(const dumpi_win_complete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_create(const dumpi_win_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_fence(const dumpi_win_fence *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_free(const dumpi_win_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_get_group(const dumpi_win_get_group *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_lock(const dumpi_win_lock *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_post(const dumpi_win_post *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_start(const dumpi_win_start *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_test(const dumpi_win_test *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_unlock(const dumpi_win_unlock *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_wait(const dumpi_win_wait *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Alltoallw(const dumpi_alltoallw *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  DUMPI_ENTERING();
  //TODO
  WARN_UNUSED(true);
  DUMPI_RETURNING();
}

int report_MPI_Exscan(const dumpi_exscan *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Add_error_class(const dumpi_add_error_class *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Add_error_code(const dumpi_add_error_code *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Add_error_string(const dumpi_add_error_string *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_call_errhandler(const dumpi_comm_call_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_create_keyval(const dumpi_comm_create_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_delete_attr(const dumpi_comm_delete_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_free_keyval(const dumpi_comm_free_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_get_attr(const dumpi_comm_get_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_get_name(const dumpi_comm_get_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_set_attr(const dumpi_comm_set_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_set_name(const dumpi_comm_set_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_call_errhandler(const dumpi_file_call_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Grequest_complete(const dumpi_grequest_complete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Grequest_start(const dumpi_grequest_start *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Init_thread(const dumpi_init_thread *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Is_thread_main(const dumpi_is_thread_main *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Query_thread(const dumpi_query_thread *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Status_set_cancelled(const dumpi_status_set_cancelled *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Status_set_elements(const dumpi_status_set_elements *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_create_keyval(const dumpi_type_create_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_delete_attr(const dumpi_type_delete_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_dup(const dumpi_type_dup *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_free_keyval(const dumpi_type_free_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_get_attr(const dumpi_type_get_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_get_contents(const dumpi_type_get_contents *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_get_envelope(const dumpi_type_get_envelope *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_get_name(const dumpi_type_get_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_set_attr(const dumpi_type_set_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_set_name(const dumpi_type_set_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_match_size(const dumpi_type_match_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_call_errhandler(const dumpi_win_call_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_create_keyval(const dumpi_win_create_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_delete_attr(const dumpi_win_delete_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_free_keyval(const dumpi_win_free_keyval *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_get_attr(const dumpi_win_get_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_get_name(const dumpi_win_get_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_set_attr(const dumpi_win_set_attr *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_set_name(const dumpi_win_set_name *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Alloc_mem(const dumpi_alloc_mem *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_create_errhandler(const dumpi_comm_create_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_get_errhandler(const dumpi_comm_get_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Comm_set_errhandler(const dumpi_comm_set_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_create_errhandler(const dumpi_file_create_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_errhandler(const dumpi_file_get_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_set_errhandler(const dumpi_file_set_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Finalized(const dumpi_finalized *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Free_mem(const dumpi_free_mem *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Get_address(const dumpi_get_address *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_create(const dumpi_info_create *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_delete(const dumpi_info_delete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_dup(const dumpi_info_dup *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_free(const dumpi_info_free *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_get(const dumpi_info_get *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_get_nkeys(const dumpi_info_get_nkeys *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_get_nthkey(const dumpi_info_get_nthkey *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_get_valuelen(const dumpi_info_get_valuelen *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Info_set(const dumpi_info_set *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Pack_external(const dumpi_pack_external *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Pack_external_size(const dumpi_pack_external_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Request_get_status(const dumpi_request_get_status *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_create_darray(const dumpi_type_create_darray *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_create_hindexed(const dumpi_type_create_hindexed *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_create_hvector(const dumpi_type_create_hvector *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_create_indexed_block(const dumpi_type_create_indexed_block *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_create_resized(const dumpi_type_create_resized *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_create_struct(const dumpi_type_create_struct *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_create_subarray(const dumpi_type_create_subarray *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_get_extent(const dumpi_type_get_extent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Type_get_true_extent(const dumpi_type_get_true_extent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Unpack_external(const dumpi_unpack_external *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_create_errhandler(const dumpi_win_create_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_get_errhandler(const dumpi_win_get_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Win_set_errhandler(const dumpi_win_set_errhandler *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_open(const dumpi_file_open *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_close(const dumpi_file_close *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_delete(const dumpi_file_delete *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_set_size(const dumpi_file_set_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_preallocate(const dumpi_file_preallocate *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_size(const dumpi_file_get_size *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_group(const dumpi_file_get_group *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_amode(const dumpi_file_get_amode *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_set_info(const dumpi_file_set_info *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_info(const dumpi_file_get_info *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_set_view(const dumpi_file_set_view *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_view(const dumpi_file_get_view *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_at(const dumpi_file_read_at *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_at_all(const dumpi_file_read_at_all *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_at(const dumpi_file_write_at *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_at_all(const dumpi_file_write_at_all *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_iread_at(const dumpi_file_iread_at *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_iwrite_at(const dumpi_file_iwrite_at *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read(const dumpi_file_read *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_all(const dumpi_file_read_all *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write(const dumpi_file_write *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_all(const dumpi_file_write_all *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_iread(const dumpi_file_iread *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_iwrite(const dumpi_file_iwrite *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_seek(const dumpi_file_seek *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_position(const dumpi_file_get_position *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_byte_offset(const dumpi_file_get_byte_offset *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_shared(const dumpi_file_read_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_shared(const dumpi_file_write_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_iread_shared(const dumpi_file_iread_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_iwrite_shared(const dumpi_file_iwrite_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_ordered(const dumpi_file_read_ordered *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_ordered(const dumpi_file_write_ordered *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_seek_shared(const dumpi_file_seek_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_position_shared(const dumpi_file_get_position_shared *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_at_all_begin(const dumpi_file_read_at_all_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_at_all_end(const dumpi_file_read_at_all_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_at_all_begin(const dumpi_file_write_at_all_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_at_all_end(const dumpi_file_write_at_all_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_all_begin(const dumpi_file_read_all_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_all_end(const dumpi_file_read_all_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_all_begin(const dumpi_file_write_all_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_all_end(const dumpi_file_write_all_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_ordered_begin(const dumpi_file_read_ordered_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_read_ordered_end(const dumpi_file_read_ordered_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_ordered_begin(const dumpi_file_write_ordered_begin *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_write_ordered_end(const dumpi_file_write_ordered_end *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_type_extent(const dumpi_file_get_type_extent *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_Register_datarep(const dumpi_register_datarep *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_set_atomicity(const dumpi_file_set_atomicity *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_get_atomicity(const dumpi_file_get_atomicity *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

int report_MPI_File_sync(const dumpi_file_sync *prm, uint16_t thread, const dumpi_time *cpu, const dumpi_time *wall, const dumpi_perfinfo *perf, void *uarg) {
  IF_SKIP_UNUSED_BEGIN(uarg);
  DUMPI_ENTERING();
  DUMPI_RETURNING();
  IF_SKIP_UNUSED_END();
}

void set_callbacks(libundumpi_callbacks *cbacks) {
  assert(cbacks != NULL);
  cbacks->on_send                      = report_MPI_Send                     ;
  cbacks->on_recv                      = report_MPI_Recv                     ;
  cbacks->on_get_count                 = report_MPI_Get_count                ;
  cbacks->on_bsend                     = report_MPI_Bsend                    ;
  cbacks->on_ssend                     = report_MPI_Ssend                    ;
  cbacks->on_rsend                     = report_MPI_Rsend                    ;
  cbacks->on_buffer_attach             = report_MPI_Buffer_attach            ;
  cbacks->on_buffer_detach             = report_MPI_Buffer_detach            ;
  cbacks->on_isend                     = report_MPI_Isend                    ;
  cbacks->on_ibsend                    = report_MPI_Ibsend                   ;
  cbacks->on_issend                    = report_MPI_Issend                   ;
  cbacks->on_irsend                    = report_MPI_Irsend                   ;
  cbacks->on_irecv                     = report_MPI_Irecv                    ;
  cbacks->on_wait                      = report_MPI_Wait                     ;
  cbacks->on_test                      = report_MPI_Test                     ;
  cbacks->on_request_free              = report_MPI_Request_free             ;
  cbacks->on_waitany                   = report_MPI_Waitany                  ;
  cbacks->on_testany                   = report_MPI_Testany                  ;
  cbacks->on_waitall                   = report_MPI_Waitall                  ;
  cbacks->on_testall                   = report_MPI_Testall                  ;
  cbacks->on_waitsome                  = report_MPI_Waitsome                 ;
  cbacks->on_testsome                  = report_MPI_Testsome                 ;
  cbacks->on_iprobe                    = report_MPI_Iprobe                   ;
  cbacks->on_probe                     = report_MPI_Probe                    ;
  cbacks->on_cancel                    = report_MPI_Cancel                   ;
  cbacks->on_test_cancelled            = report_MPI_Test_cancelled           ;
  cbacks->on_send_init                 = report_MPI_Send_init                ;
  cbacks->on_bsend_init                = report_MPI_Bsend_init               ;
  cbacks->on_ssend_init                = report_MPI_Ssend_init               ;
  cbacks->on_rsend_init                = report_MPI_Rsend_init               ;
  cbacks->on_recv_init                 = report_MPI_Recv_init                ;
  cbacks->on_start                     = report_MPI_Start                    ;
  cbacks->on_startall                  = report_MPI_Startall                 ;
  cbacks->on_sendrecv                  = report_MPI_Sendrecv                 ;
  cbacks->on_sendrecv_replace          = report_MPI_Sendrecv_replace         ;
  cbacks->on_type_contiguous           = report_MPI_Type_contiguous          ;
  cbacks->on_type_vector               = report_MPI_Type_vector              ;
  cbacks->on_type_hvector              = report_MPI_Type_hvector             ;
  cbacks->on_type_indexed              = report_MPI_Type_indexed             ;
  cbacks->on_type_hindexed             = report_MPI_Type_hindexed            ;
  cbacks->on_type_struct               = report_MPI_Type_struct              ;
  cbacks->on_address                   = report_MPI_Address                  ;
  cbacks->on_type_extent               = report_MPI_Type_extent              ;
  cbacks->on_type_size                 = report_MPI_Type_size                ;
  cbacks->on_type_lb                   = report_MPI_Type_lb                  ;
  cbacks->on_type_ub                   = report_MPI_Type_ub                  ;
  cbacks->on_type_commit               = report_MPI_Type_commit              ;
  cbacks->on_type_free                 = report_MPI_Type_free                ;
  cbacks->on_get_elements              = report_MPI_Get_elements             ;
  cbacks->on_pack                      = report_MPI_Pack                     ;
  cbacks->on_unpack                    = report_MPI_Unpack                   ;
  cbacks->on_pack_size                 = report_MPI_Pack_size                ;
  cbacks->on_barrier                   = report_MPI_Barrier                  ;
  cbacks->on_bcast                     = report_MPI_Bcast                    ;
  cbacks->on_gather                    = report_MPI_Gather                   ;
  cbacks->on_gatherv                   = report_MPI_Gatherv                  ;
  cbacks->on_scatter                   = report_MPI_Scatter                  ;
  cbacks->on_scatterv                  = report_MPI_Scatterv                 ;
  cbacks->on_allgather                 = report_MPI_Allgather                ;
  cbacks->on_allgatherv                = report_MPI_Allgatherv               ;
  cbacks->on_alltoall                  = report_MPI_Alltoall                 ;
  cbacks->on_alltoallv                 = report_MPI_Alltoallv                ;
  cbacks->on_reduce                    = report_MPI_Reduce                   ;
  cbacks->on_op_create                 = report_MPI_Op_create                ;
  cbacks->on_op_free                   = report_MPI_Op_free                  ;
  cbacks->on_allreduce                 = report_MPI_Allreduce                ;
  cbacks->on_reduce_scatter            = report_MPI_Reduce_scatter           ;
  cbacks->on_scan                      = report_MPI_Scan                     ;
  cbacks->on_group_size                = report_MPI_Group_size               ;
  cbacks->on_group_rank                = report_MPI_Group_rank               ;
  cbacks->on_group_translate_ranks     = report_MPI_Group_translate_ranks    ;
  cbacks->on_group_compare             = report_MPI_Group_compare            ;
  cbacks->on_comm_group                = report_MPI_Comm_group               ;
  cbacks->on_group_union               = report_MPI_Group_union              ;
  cbacks->on_group_intersection        = report_MPI_Group_intersection       ;
  cbacks->on_group_difference          = report_MPI_Group_difference         ;
  cbacks->on_group_incl                = report_MPI_Group_incl               ;
  cbacks->on_group_excl                = report_MPI_Group_excl               ;
  cbacks->on_group_range_incl          = report_MPI_Group_range_incl         ;
  cbacks->on_group_range_excl          = report_MPI_Group_range_excl         ;
  cbacks->on_group_free                = report_MPI_Group_free               ;
  cbacks->on_comm_size                 = report_MPI_Comm_size                ;
  cbacks->on_comm_rank                 = report_MPI_Comm_rank                ;
  cbacks->on_comm_compare              = report_MPI_Comm_compare             ;
  cbacks->on_comm_dup                  = report_MPI_Comm_dup                 ;
  cbacks->on_comm_create               = report_MPI_Comm_create              ;
  cbacks->on_comm_split                = report_MPI_Comm_split               ;
  cbacks->on_comm_free                 = report_MPI_Comm_free                ;
  cbacks->on_comm_test_inter           = report_MPI_Comm_test_inter          ;
  cbacks->on_comm_remote_size          = report_MPI_Comm_remote_size         ;
  cbacks->on_comm_remote_group         = report_MPI_Comm_remote_group        ;
  cbacks->on_intercomm_create          = report_MPI_Intercomm_create         ;
  cbacks->on_intercomm_merge           = report_MPI_Intercomm_merge          ;
  cbacks->on_keyval_create             = report_MPI_Keyval_create            ;
  cbacks->on_keyval_free               = report_MPI_Keyval_free              ;
  cbacks->on_attr_put                  = report_MPI_Attr_put                 ;
  cbacks->on_attr_get                  = report_MPI_Attr_get                 ;
  cbacks->on_attr_delete               = report_MPI_Attr_delete              ;
  cbacks->on_topo_test                 = report_MPI_Topo_test                ;
  cbacks->on_cart_create               = report_MPI_Cart_create              ;
  cbacks->on_dims_create               = report_MPI_Dims_create              ;
  cbacks->on_graph_create              = report_MPI_Graph_create             ;
  cbacks->on_graphdims_get             = report_MPI_Graphdims_get            ;
  cbacks->on_graph_get                 = report_MPI_Graph_get                ;
  cbacks->on_cartdim_get               = report_MPI_Cartdim_get              ;
  cbacks->on_cart_get                  = report_MPI_Cart_get                 ;
  cbacks->on_cart_rank                 = report_MPI_Cart_rank                ;
  cbacks->on_cart_coords               = report_MPI_Cart_coords              ;
  cbacks->on_graph_neighbors_count     = report_MPI_Graph_neighbors_count    ;
  cbacks->on_graph_neighbors           = report_MPI_Graph_neighbors          ;
  cbacks->on_cart_shift                = report_MPI_Cart_shift               ;
  cbacks->on_cart_sub                  = report_MPI_Cart_sub                 ;
  cbacks->on_cart_map                  = report_MPI_Cart_map                 ;
  cbacks->on_graph_map                 = report_MPI_Graph_map                ;
  cbacks->on_get_processor_name        = report_MPI_Get_processor_name       ;
  cbacks->on_get_version               = report_MPI_Get_version              ;
  cbacks->on_errhandler_create         = report_MPI_Errhandler_create        ;
  cbacks->on_errhandler_set            = report_MPI_Errhandler_set           ;
  cbacks->on_errhandler_get            = report_MPI_Errhandler_get           ;
  cbacks->on_errhandler_free           = report_MPI_Errhandler_free          ;
  cbacks->on_error_string              = report_MPI_Error_string             ;
  cbacks->on_error_class               = report_MPI_Error_class              ;
  cbacks->on_wtime                     = report_MPI_Wtime                    ;
  cbacks->on_wtick                     = report_MPI_Wtick                    ;
  cbacks->on_init                      = report_MPI_Init                     ;
  cbacks->on_finalize                  = report_MPI_Finalize                 ;
  cbacks->on_initialized               = report_MPI_Initialized              ;
  cbacks->on_abort                     = report_MPI_Abort                    ;
  cbacks->on_close_port                = report_MPI_Close_port               ;
  cbacks->on_comm_accept               = report_MPI_Comm_accept              ;
  cbacks->on_comm_connect              = report_MPI_Comm_connect             ;
  cbacks->on_comm_disconnect           = report_MPI_Comm_disconnect          ;
  cbacks->on_comm_get_parent           = report_MPI_Comm_get_parent          ;
  cbacks->on_comm_join                 = report_MPI_Comm_join                ;
  cbacks->on_comm_spawn                = report_MPI_Comm_spawn               ;
  cbacks->on_comm_spawn_multiple       = report_MPI_Comm_spawn_multiple      ;
  cbacks->on_lookup_name               = report_MPI_Lookup_name              ;
  cbacks->on_open_port                 = report_MPI_Open_port                ;
  cbacks->on_publish_name              = report_MPI_Publish_name             ;
  cbacks->on_unpublish_name            = report_MPI_Unpublish_name           ;
  cbacks->on_accumulate                = report_MPI_Accumulate               ;
  cbacks->on_get                       = report_MPI_Get                      ;
  cbacks->on_put                       = report_MPI_Put                      ;
  cbacks->on_win_complete              = report_MPI_Win_complete             ;
  cbacks->on_win_create                = report_MPI_Win_create               ;
  cbacks->on_win_fence                 = report_MPI_Win_fence                ;
  cbacks->on_win_free                  = report_MPI_Win_free                 ;
  cbacks->on_win_get_group             = report_MPI_Win_get_group            ;
  cbacks->on_win_lock                  = report_MPI_Win_lock                 ;
  cbacks->on_win_post                  = report_MPI_Win_post                 ;
  cbacks->on_win_start                 = report_MPI_Win_start                ;
  cbacks->on_win_test                  = report_MPI_Win_test                 ;
  cbacks->on_win_unlock                = report_MPI_Win_unlock               ;
  cbacks->on_win_wait                  = report_MPI_Win_wait                 ;
  cbacks->on_alltoallw                 = report_MPI_Alltoallw                ;
  cbacks->on_exscan                    = report_MPI_Exscan                   ;
  cbacks->on_add_error_class           = report_MPI_Add_error_class          ;
  cbacks->on_add_error_code            = report_MPI_Add_error_code           ;
  cbacks->on_add_error_string          = report_MPI_Add_error_string         ;
  cbacks->on_comm_call_errhandler      = report_MPI_Comm_call_errhandler     ;
  cbacks->on_comm_create_keyval        = report_MPI_Comm_create_keyval       ;
  cbacks->on_comm_delete_attr          = report_MPI_Comm_delete_attr         ;
  cbacks->on_comm_free_keyval          = report_MPI_Comm_free_keyval         ;
  cbacks->on_comm_get_attr             = report_MPI_Comm_get_attr            ;
  cbacks->on_comm_get_name             = report_MPI_Comm_get_name            ;
  cbacks->on_comm_set_attr             = report_MPI_Comm_set_attr            ;
  cbacks->on_comm_set_name             = report_MPI_Comm_set_name            ;
  cbacks->on_file_call_errhandler      = report_MPI_File_call_errhandler     ;
  cbacks->on_grequest_complete         = report_MPI_Grequest_complete        ;
  cbacks->on_grequest_start            = report_MPI_Grequest_start           ;
  cbacks->on_init_thread               = report_MPI_Init_thread              ;
  cbacks->on_is_thread_main            = report_MPI_Is_thread_main           ;
  cbacks->on_query_thread              = report_MPI_Query_thread             ;
  cbacks->on_status_set_cancelled      = report_MPI_Status_set_cancelled     ;
  cbacks->on_status_set_elements       = report_MPI_Status_set_elements      ;
  cbacks->on_type_create_keyval        = report_MPI_Type_create_keyval       ;
  cbacks->on_type_delete_attr          = report_MPI_Type_delete_attr         ;
  cbacks->on_type_dup                  = report_MPI_Type_dup                 ;
  cbacks->on_type_free_keyval          = report_MPI_Type_free_keyval         ;
  cbacks->on_type_get_attr             = report_MPI_Type_get_attr            ;
  cbacks->on_type_get_contents         = report_MPI_Type_get_contents        ;
  cbacks->on_type_get_envelope         = report_MPI_Type_get_envelope        ;
  cbacks->on_type_get_name             = report_MPI_Type_get_name            ;
  cbacks->on_type_set_attr             = report_MPI_Type_set_attr            ;
  cbacks->on_type_set_name             = report_MPI_Type_set_name            ;
  cbacks->on_type_match_size           = report_MPI_Type_match_size          ;
  cbacks->on_win_call_errhandler       = report_MPI_Win_call_errhandler      ;
  cbacks->on_win_create_keyval         = report_MPI_Win_create_keyval        ;
  cbacks->on_win_delete_attr           = report_MPI_Win_delete_attr          ;
  cbacks->on_win_free_keyval           = report_MPI_Win_free_keyval          ;
  cbacks->on_win_get_attr              = report_MPI_Win_get_attr             ;
  cbacks->on_win_get_name              = report_MPI_Win_get_name             ;
  cbacks->on_win_set_attr              = report_MPI_Win_set_attr             ;
  cbacks->on_win_set_name              = report_MPI_Win_set_name             ;
  cbacks->on_alloc_mem                 = report_MPI_Alloc_mem                ;
  cbacks->on_comm_create_errhandler    = report_MPI_Comm_create_errhandler   ;
  cbacks->on_comm_get_errhandler       = report_MPI_Comm_get_errhandler      ;
  cbacks->on_comm_set_errhandler       = report_MPI_Comm_set_errhandler      ;
  cbacks->on_file_create_errhandler    = report_MPI_File_create_errhandler   ;
  cbacks->on_file_get_errhandler       = report_MPI_File_get_errhandler      ;
  cbacks->on_file_set_errhandler       = report_MPI_File_set_errhandler      ;
  cbacks->on_finalized                 = report_MPI_Finalized                ;
  cbacks->on_free_mem                  = report_MPI_Free_mem                 ;
  cbacks->on_get_address               = report_MPI_Get_address              ;
  cbacks->on_info_create               = report_MPI_Info_create              ;
  cbacks->on_info_delete               = report_MPI_Info_delete              ;
  cbacks->on_info_dup                  = report_MPI_Info_dup                 ;
  cbacks->on_info_free                 = report_MPI_Info_free                ;
  cbacks->on_info_get                  = report_MPI_Info_get                 ;
  cbacks->on_info_get_nkeys            = report_MPI_Info_get_nkeys           ;
  cbacks->on_info_get_nthkey           = report_MPI_Info_get_nthkey          ;
  cbacks->on_info_get_valuelen         = report_MPI_Info_get_valuelen        ;
  cbacks->on_info_set                  = report_MPI_Info_set                 ;
  cbacks->on_pack_external             = report_MPI_Pack_external            ;
  cbacks->on_pack_external_size        = report_MPI_Pack_external_size       ;
  cbacks->on_request_get_status        = report_MPI_Request_get_status       ;
  cbacks->on_type_create_darray        = report_MPI_Type_create_darray       ;
  cbacks->on_type_create_hindexed      = report_MPI_Type_create_hindexed     ;
  cbacks->on_type_create_hvector       = report_MPI_Type_create_hvector      ;
  cbacks->on_type_create_indexed_block = report_MPI_Type_create_indexed_block;
  cbacks->on_type_create_resized       = report_MPI_Type_create_resized      ;
  cbacks->on_type_create_struct        = report_MPI_Type_create_struct       ;
  cbacks->on_type_create_subarray      = report_MPI_Type_create_subarray     ;
  cbacks->on_type_get_extent           = report_MPI_Type_get_extent          ;
  cbacks->on_type_get_true_extent      = report_MPI_Type_get_true_extent     ;
  cbacks->on_unpack_external           = report_MPI_Unpack_external          ;
  cbacks->on_win_create_errhandler     = report_MPI_Win_create_errhandler    ;
  cbacks->on_win_get_errhandler        = report_MPI_Win_get_errhandler       ;
  cbacks->on_win_set_errhandler        = report_MPI_Win_set_errhandler       ;
  cbacks->on_file_open                 = report_MPI_File_open                ;
  cbacks->on_file_close                = report_MPI_File_close               ;
  cbacks->on_file_delete               = report_MPI_File_delete              ;
  cbacks->on_file_set_size             = report_MPI_File_set_size            ;
  cbacks->on_file_preallocate          = report_MPI_File_preallocate         ;
  cbacks->on_file_get_size             = report_MPI_File_get_size            ;
  cbacks->on_file_get_group            = report_MPI_File_get_group           ;
  cbacks->on_file_get_amode            = report_MPI_File_get_amode           ;
  cbacks->on_file_set_info             = report_MPI_File_set_info            ;
  cbacks->on_file_get_info             = report_MPI_File_get_info            ;
  cbacks->on_file_set_view             = report_MPI_File_set_view            ;
  cbacks->on_file_get_view             = report_MPI_File_get_view            ;
  cbacks->on_file_read_at              = report_MPI_File_read_at             ;
  cbacks->on_file_read_at_all          = report_MPI_File_read_at_all         ;
  cbacks->on_file_write_at             = report_MPI_File_write_at            ;
  cbacks->on_file_write_at_all         = report_MPI_File_write_at_all        ;
  cbacks->on_file_iread_at             = report_MPI_File_iread_at            ;
  cbacks->on_file_iwrite_at            = report_MPI_File_iwrite_at           ;
  cbacks->on_file_read                 = report_MPI_File_read                ;
  cbacks->on_file_read_all             = report_MPI_File_read_all            ;
  cbacks->on_file_write                = report_MPI_File_write               ;
  cbacks->on_file_write_all            = report_MPI_File_write_all           ;
  cbacks->on_file_iread                = report_MPI_File_iread               ;
  cbacks->on_file_iwrite               = report_MPI_File_iwrite              ;
  cbacks->on_file_seek                 = report_MPI_File_seek                ;
  cbacks->on_file_get_position         = report_MPI_File_get_position        ;
  cbacks->on_file_get_byte_offset      = report_MPI_File_get_byte_offset     ;
  cbacks->on_file_read_shared          = report_MPI_File_read_shared         ;
  cbacks->on_file_write_shared         = report_MPI_File_write_shared        ;
  cbacks->on_file_iread_shared         = report_MPI_File_iread_shared        ;
  cbacks->on_file_iwrite_shared        = report_MPI_File_iwrite_shared       ;
  cbacks->on_file_read_ordered         = report_MPI_File_read_ordered        ;
  cbacks->on_file_write_ordered        = report_MPI_File_write_ordered       ;
  cbacks->on_file_seek_shared          = report_MPI_File_seek_shared         ;
  cbacks->on_file_get_position_shared  = report_MPI_File_get_position_shared ;
  cbacks->on_file_read_at_all_begin    = report_MPI_File_read_at_all_begin   ;
  cbacks->on_file_read_at_all_end      = report_MPI_File_read_at_all_end     ;
  cbacks->on_file_write_at_all_begin   = report_MPI_File_write_at_all_begin  ;
  cbacks->on_file_write_at_all_end     = report_MPI_File_write_at_all_end    ;
  cbacks->on_file_read_all_begin       = report_MPI_File_read_all_begin      ;
  cbacks->on_file_read_all_end         = report_MPI_File_read_all_end        ;
  cbacks->on_file_write_all_begin      = report_MPI_File_write_all_begin     ;
  cbacks->on_file_write_all_end        = report_MPI_File_write_all_end       ;
  cbacks->on_file_read_ordered_begin   = report_MPI_File_read_ordered_begin  ;
  cbacks->on_file_read_ordered_end     = report_MPI_File_read_ordered_end    ;
  cbacks->on_file_write_ordered_begin  = report_MPI_File_write_ordered_begin ;
  cbacks->on_file_write_ordered_end    = report_MPI_File_write_ordered_end   ;
  cbacks->on_file_get_type_extent      = report_MPI_File_get_type_extent     ;
  cbacks->on_register_datarep          = report_MPI_Register_datarep         ;
  cbacks->on_file_set_atomicity        = report_MPI_File_set_atomicity       ;
  cbacks->on_file_get_atomicity        = report_MPI_File_get_atomicity       ;
  cbacks->on_file_sync                 = report_MPI_File_sync                ;
}
