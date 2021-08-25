#ifndef webfr_web_protocol_http_outgoing_StreamingBody_hpp
#define webfr_web_protocol_http_outgoing_StreamingBody_hpp

#include "./Body.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

class StreamingBody : public Body {
private:
  std::shared_ptr<data::stream::ReadCallback> m_readCallback;
public:

  StreamingBody(const std::shared_ptr<data::stream::ReadCallback>& readCallback);

  v_io_size read(void *buffer, v_buff_size count, async::Action& action) override;

  void declareHeaders(Headers& headers) override;

  p_char8 getKnownData() override;

  v_int64 getKnownSize() override;

};

}}}}}

#endif 