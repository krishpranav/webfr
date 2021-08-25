#ifndef webfr_web_protocol_http_incoming_Response_hpp
#define webfr_web_protocol_http_incoming_Response_hpp

#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/web/protocol/http/incoming/BodyDecoder.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

class Response : public webfr::base::Countable {
public:
  OBJECT_POOL(Incoming_Response_Pool, Response, 32)
  SHARED_OBJECT_POOL(Shared_Incoming_Response_Pool, Response, 32)
private:
  v_int32 m_statusCode;
  webfr::String m_statusDescription;
  http::Headers m_headers;
  std::shared_ptr<webfr::data::stream::InputStream> m_bodyStream;

  std::shared_ptr<const http::incoming::BodyDecoder> m_bodyDecoder;
  
  std::shared_ptr<webfr::data::stream::IOStream> m_connection;
  
public:

  Response(v_int32 statusCode,
           const webfr::String& statusDescription,
           const http::Headers& headers,
           const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
           const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);
public:

  static std::shared_ptr<Response> createShared(v_int32 statusCode,
                                                const webfr::String& statusDescription,
                                                const http::Headers& headers,
                                                const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
                                                const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);

  v_int32 getStatusCode() const;

  webfr::String getStatusDescription() const;

  const http::Headers& getHeaders() const;

  void putHeader(const webfr::String& key, const webfr::String& value);

  bool putHeaderIfNotExists(const webfr::String& key, const webfr::String& value);

  void putHeader_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value);

  bool putHeaderIfNotExists_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value);

  webfr::String getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const;

  std::shared_ptr<webfr::data::stream::InputStream> getBodyStream() const;

  std::shared_ptr<const http::incoming::BodyDecoder> getBodyDecoder() const;

  void transferBody(data::stream::WriteCallback* writeCallback) const;

  /**
   * Decode and transfer body to toStream.
   * Use case example - stream huge body directly to file using relatively small buffer.
   * @param toStream - pointer to &id:webfr::data::stream::OutputStream;.
   */
  void transferBodyToStream(webfr::data::stream::OutputStream* toStream) const;

  webfr::String readBodyToString() const;

  template<class Wrapper>
  Wrapper readBodyToDto(webfr::data::mapping::ObjectMapper* objectMapper) const {
    return m_bodyDecoder->decodeToDto<Wrapper>(m_headers, m_bodyStream.get(), objectMapper);
  }
  
  async::CoroutineStarter transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const;

  webfr::async::CoroutineStarter transferBodyToStreamAsync(const std::shared_ptr<webfr::data::stream::OutputStream>& toStream) const;

  webfr::async::CoroutineStarterForResult<const webfr::String&> readBodyToStringAsync() const {
    return m_bodyDecoder->decodeToStringAsync(m_headers, m_bodyStream);
  }

  template<class Wrapper>
  webfr::async::CoroutineStarterForResult<const Wrapper&>
  readBodyToDtoAsync(const std::shared_ptr<webfr::data::mapping::ObjectMapper>& objectMapper) const {
    return m_bodyDecoder->decodeToDtoAsync<Wrapper>(m_headers, m_bodyStream, objectMapper);
  }
  
};
  
}}}}}

#endif