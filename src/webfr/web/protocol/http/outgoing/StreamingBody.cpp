#include "StreamingBody.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

StreamingBody::StreamingBody(const std::shared_ptr<data::stream::ReadCallback>& readCallback)
  : m_readCallback(readCallback)
{}

v_io_size StreamingBody::read(void *buffer, v_buff_size count, async::Action& action) {
  return m_readCallback->read(buffer, count, action);
}

void StreamingBody::declareHeaders(Headers& headers) {
  (void) headers;
}

p_char8 StreamingBody::getKnownData() {
  return nullptr;
}


v_int64 StreamingBody::getKnownSize() {
  return -1;
}

}}}}}