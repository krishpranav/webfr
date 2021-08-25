#ifndef webfr_web_protocol_http_incoming_Request_hpp
#define webfr_web_protocol_http_incoming_Request_hpp

#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/web/protocol/http/incoming/BodyDecoder.hpp"
#include "webfr/web/url/mapping/Pattern.hpp"
#include "webfr/network/Url.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

class Request : public webfr::base::Countable {
public:
  OBJECT_POOL(Incoming_Request_Pool, Request, 32)
  SHARED_OBJECT_POOL(Shared_Incoming_Request_Pool, Request, 32)
private:

  std::shared_ptr<webfr::data::stream::IOStream> m_connection;
  http::RequestStartingLine m_startingLine;
  url::mapping::Pattern::MatchMap m_pathVariables;
  http::Headers m_headers;
  std::shared_ptr<webfr::data::stream::InputStream> m_bodyStream;
  
  std::shared_ptr<const http::incoming::BodyDecoder> m_bodyDecoder;

  mutable bool m_queryParamsParsed; 
  mutable http::QueryParams m_queryParams;

public:
  
  Request(const std::shared_ptr<webfr::data::stream::IOStream>& connection,
          const http::RequestStartingLine& startingLine,
          const http::Headers& headers,
          const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
          const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);
public:
  
  static std::shared_ptr<Request> createShared(const std::shared_ptr<webfr::data::stream::IOStream>& connection,
                                               const http::RequestStartingLine& startingLine,
                                               const http::Headers& headers,
                                               const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
                                               const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder);

  /**
   * Get raw connection stream.
   * @return - &id:std::shared_ptr<webfr::data::stream::IOStream> m_connection;.
   */
  std::shared_ptr<webfr::data::stream::IOStream> getConnection();

  const http::QueryParams& getQueryParameters() const;

  webfr::String getQueryParameter(const webfr::data::share::StringKeyLabel& name) const;

  webfr::String getQueryParameter(const webfr::data::share::StringKeyLabel& name, const webfr::String& defaultValue) const;

  const http::RequestStartingLine& getStartingLine() const;

  void setPathVariables(const url::mapping::Pattern::MatchMap& pathVariables);

  const url::mapping::Pattern::MatchMap& getPathVariables() const;

  const http::Headers& getHeaders() const;

  std::shared_ptr<webfr::data::stream::InputStream> getBodyStream() const;

  std::shared_ptr<const http::incoming::BodyDecoder> getBodyDecoder() const;

  void putHeader(const webfr::String& key, const webfr::String& value);

  bool putHeaderIfNotExists(const webfr::String& key, const webfr::String& value);

  void putHeader_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value);

  bool putHeaderIfNotExists_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value);

  webfr::String getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const;

  webfr::String getPathVariable(const webfr::data::share::StringKeyLabel& name) const;

  webfr::String getPathTail() const;

  void transferBody(data::stream::WriteCallback* writeCallback) const;

  void transferBodyToStream(webfr::data::stream::OutputStream* toStream) const;
   
  webfr::String readBodyToString() const;

  template<class Wrapper>
  Wrapper readBodyToDto(data::mapping::ObjectMapper* objectMapper) const {
    return objectMapper->readFromString<Wrapper>(m_bodyDecoder->decodeToString(m_headers, m_bodyStream.get()));
  }

  async::CoroutineStarter transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const;

  webfr::async::CoroutineStarter transferBodyToStreamAsync(const std::shared_ptr<webfr::data::stream::OutputStream>& toStream) const;

  async::CoroutineStarterForResult<const webfr::String&> readBodyToStringAsync() const;

  template<class Wrapper>
  webfr::async::CoroutineStarterForResult<const Wrapper&>
  readBodyToDtoAsync(const std::shared_ptr<webfr::data::mapping::ObjectMapper>& objectMapper) const {
    return m_bodyDecoder->decodeToDtoAsync<Wrapper>(m_headers, m_bodyStream, objectMapper);
  }
  
};
  
}}}}}

#endif 
