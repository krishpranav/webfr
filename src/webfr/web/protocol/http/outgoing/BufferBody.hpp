#ifndef webfr_web_protocol_http_outgoing_BufferBody_hpp
#define webfr_web_protocol_http_outgoing_BufferBody_hpp

#include "./Body.hpp"
#include "webfr/web/protocol/http/Http.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

class BufferBody : public webfr::base::Countable, public Body {
public:
  OBJECT_POOL(Http_Outgoing_BufferBody_Pool, BufferBody, 32)
  SHARED_OBJECT_POOL(Shared_Http_Outgoing_BufferBody_Pool, BufferBody, 32)
private:
  webfr::String m_buffer;
  webfr::data::share::StringKeyLabel m_contentType;
  data::buffer::InlineReadData m_inlineData;
public:
  BufferBody(const webfr::String& buffer, const data::share::StringKeyLabel& contentType);
public:

  static std::shared_ptr<BufferBody> createShared(const webfr::String& buffer,
                                                  const data::share::StringKeyLabel& contentType = data::share::StringKeyLabel());

  v_io_size read(void *buffer, v_buff_size count, async::Action& action) override;

  void declareHeaders(Headers& headers) override;

  p_char8 getKnownData() override;

  v_int64 getKnownSize() override;
  
};
  
}}}}}

#endif 