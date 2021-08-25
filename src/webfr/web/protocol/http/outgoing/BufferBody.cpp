#include "BufferBody.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

BufferBody::BufferBody(const webfr::String& buffer, const data::share::StringKeyLabel& contentType)
  : m_buffer(buffer)
  , m_contentType(contentType)
  , m_inlineData(m_buffer->getData(), m_buffer->getSize())
{}

std::shared_ptr<BufferBody> BufferBody::createShared(const webfr::String& buffer, const data::share::StringKeyLabel& contentType) {
  return Shared_Http_Outgoing_BufferBody_Pool::allocateShared(buffer, contentType);
}

v_io_size BufferBody::read(void *buffer, v_buff_size count, async::Action& action) {

  (void) action;

  v_buff_size desiredToRead = m_inlineData.bytesLeft;

  if(desiredToRead > 0) {

    if (desiredToRead > count) {
      desiredToRead = count;
    }

    std::memcpy(buffer, m_inlineData.currBufferPtr, desiredToRead);
    m_inlineData.inc(desiredToRead);

    return desiredToRead;

  }

  return 0;

}

void BufferBody::declareHeaders(Headers& headers) {
  if(m_contentType) {
    headers.put(Header::CONTENT_TYPE, m_contentType);
  }
}

p_char8 BufferBody::getKnownData() {
  return m_buffer->getData();
}

v_int64 BufferBody::getKnownSize() {
  return m_buffer->getSize();
}

}}}}}