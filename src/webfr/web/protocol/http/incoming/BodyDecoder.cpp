#include "BodyDecoder.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

webfr::async::CoroutineStarterForResult<const webfr::String&>
BodyDecoder::decodeToStringAsync(const Headers& headers, const std::shared_ptr<data::stream::InputStream>& bodyStream) const {

  class ToStringDecoder : public webfr::async::CoroutineWithResult<ToStringDecoder, const webfr::String&> {
  private:
    const BodyDecoder* m_decoder;
    Headers m_headers;
    std::shared_ptr<webfr::data::stream::InputStream> m_bodyStream;
    std::shared_ptr<webfr::data::stream::ChunkedBuffer> m_outputStream;
  public:

    ToStringDecoder(const BodyDecoder* decoder,
                    const Headers& headers,
                    const std::shared_ptr<data::stream::InputStream>& bodyStream)
      : m_decoder(decoder)
      , m_headers(headers)
      , m_bodyStream(bodyStream)
      , m_outputStream(std::make_shared<data::stream::ChunkedBuffer>())
    {}

    Action act() override {
      return m_decoder->decodeAsync(m_headers, m_bodyStream, m_outputStream).next(yieldTo(&ToStringDecoder::onDecoded));
    }

    Action onDecoded() {
      return _return(m_outputStream->toString());
    }

  };

  return ToStringDecoder::startForResult(this, headers, bodyStream);

}

}}}}}