#ifndef webfr_web_protocol_http_incoming_BodyDecoder_hpp
#define webfr_web_protocol_http_incoming_BodyDecoder_hpp

#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/core/data/mapping/ObjectMapper.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"
#include "webfr/core/async/Coroutine.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

class BodyDecoder {
private:
  
  template<class Wrapper>
  class ToDtoDecoder : public webfr::async::CoroutineWithResult<ToDtoDecoder<Wrapper>, const Wrapper&> {
  private:
    const BodyDecoder* m_decoder;
    Headers m_headers;
    std::shared_ptr<webfr::data::stream::InputStream> m_bodyStream;
    std::shared_ptr<webfr::data::mapping::ObjectMapper> m_objectMapper;
    std::shared_ptr<webfr::data::stream::ChunkedBuffer> m_outputStream;
  public:
    
    ToDtoDecoder(const BodyDecoder* decoder,
                 Headers& headers,
                 const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
                 const std::shared_ptr<webfr::data::mapping::ObjectMapper>& objectMapper)
      : m_decoder(decoder)
      , m_headers(headers)
      , m_bodyStream(bodyStream)
      , m_objectMapper(objectMapper)
      , m_outputStream(std::make_shared<webfr::data::stream::ChunkedBuffer>())
    {}
    
    webfr::async::Action act() override {
      return m_decoder->decodeAsync(m_headers, m_bodyStream, m_outputStream).next(this->yieldTo(&ToDtoDecoder::onDecoded));
    }
    
    webfr::async::Action onDecoded() {
      auto body = m_outputStream->toString();
      webfr::parser::Caret caret(body);
      auto dto = m_objectMapper->readFromCaret<Wrapper>(caret);
      if(caret.hasError()) {
        return this->template error<webfr::async::Error>(caret.getErrorMessage());
      }
      return this->_return(dto);
    }
    
  };
  
public:

  virtual ~BodyDecoder() = default;

  virtual void decode(const Headers& headers, data::stream::InputStream* bodyStream, data::stream::WriteCallback* writeCallback) const = 0;

  virtual webfr::async::CoroutineStarter decodeAsync(const Headers& headers,
                                                     const std::shared_ptr<data::stream::InputStream>& bodyStream,
                                                     const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const = 0;


  webfr::String decodeToString(const Headers& headers, data::stream::InputStream* bodyStream) const {
    webfr::data::stream::ChunkedBuffer stream;
    decode(headers, bodyStream, &stream);
    return stream.toString();
  }

  template<class Wrapper>
  Wrapper decodeToDto(const Headers& headers,
                      data::stream::InputStream* bodyStream,
                      data::mapping::ObjectMapper* objectMapper) const {
    return objectMapper->readFromString<Wrapper>(decodeToString(headers, bodyStream));
  }

  webfr::async::CoroutineStarterForResult<const webfr::String&>
  decodeToStringAsync(const Headers& headers, const std::shared_ptr<data::stream::InputStream>& bodyStream) const;

  template<class Wrapper>
  webfr::async::CoroutineStarterForResult<const Wrapper&>
  decodeToDtoAsync(const Headers& headers,
                   const std::shared_ptr<data::stream::InputStream>& bodyStream,
                   const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper) const {
    return ToDtoDecoder<Wrapper>::startForResult(this, headers, bodyStream, objectMapper);
  }
  
};
  
}}}}}

#endif 