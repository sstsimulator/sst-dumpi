#include <dumpi/libotf2dump/otf2writer.h>
#include <iostream>

/**
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



}
*/
