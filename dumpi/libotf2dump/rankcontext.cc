#include <dumpi/libotf2dump/otf2writer.h>

namespace dumpi {
  int RankContext::dispose(OTF2_Archive* archive) {
    // close existing OTF2 event file if it is open
    if (evt_writer != nullptr) {
      OTF2_Archive_CloseEvtWriter(archive, evt_writer);
      evt_writer = nullptr;
    }

    rank = -1;
    event_count = 0;
    int incomplete_count = irecv_requests.size();
    irecv_requests.clear();
    request_type.clear();
    return incomplete_count;
  }

  // I-event handling
  void RankContext::incomplete_call(int request_id, REQUEST_TYPE type) {
    request_type[request_id] = type;
  }

  /*
   * ISends and IRecvs begin when invoked, but important information about
   * them are not recorded by OTF2 until they are completed, which occurs inside of Waits, Tests, etc.
   * Returns the number of events generated
   */
  void RankContext::complete_call(request_t request_id, uint64_t timestamp) {
    auto t = request_type.find(request_id);
    if (t == request_type.end()) {
      printf("Error: request id (%i) not found\n", request_id);
      return;
    }
    else if (t->second == REQUEST_TYPE_ISEND) {
      OTF2_EvtWriter_MpiIsendComplete(evt_writer, nullptr, timestamp, request_id);
      event_count++;
    }
    else if (t->second == REQUEST_TYPE_IRECV) {
      auto irecv_it = irecv_requests.find(request_id);
      irecv_capture irecv = irecv_it->second;
      if(irecv_it == irecv_requests.end()) {
        printf("Error: Request #(%i) not found while trying to complete MPI_IRecv\n", request_id);
      } else {
        OTF2_EvtWriter_MpiIrecv(evt_writer, nullptr, timestamp, irecv.source, irecv.comm, irecv.tag, irecv.bytes_sent, request_id);
        irecv_requests.erase(irecv_it);
        event_count++;
      }
    }
    request_type.erase(t);
  }
}
