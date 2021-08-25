#include "Request.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

Request::Request(const std::shared_ptr<webfr::data::stream::IOStream>& connection,
                 const http::RequestStartingLine& startingLine,
                 const http::Headers& headers,
                 const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
                 const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder)
  : m_connection(connection)
  , m_startingLine(startingLine)
  , m_headers(headers)
  , m_bodyStream(bodyStream)
  , m_bodyDecoder(bodyDecoder)
  , m_queryParamsParsed(false)
{}

std::shared_ptr<Request> Request::createShared(const std::shared_ptr<webfr::data::stream::IOStream>& connection,
                                               const http::RequestStartingLine& startingLine,
                                               const http::Headers& headers,
                                               const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
                                               const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder)
{
  return Shared_Incoming_Request_Pool::allocateShared(connection, startingLine, headers, bodyStream, bodyDecoder);
}

std::shared_ptr<webfr::data::stream::IOStream> Request::getConnection() {
  return m_connection;
}

const http::RequestStartingLine& Request::getStartingLine() const {
  return m_startingLine;
}

void Request::setPathVariables(const url::mapping::Pattern::MatchMap& pathVariables) {
  m_pathVariables = pathVariables;
}

const url::mapping::Pattern::MatchMap& Request::getPathVariables() const {
  return m_pathVariables;
}

const http::Headers& Request::getHeaders() const {
  return m_headers;
}

const http::QueryParams& Request::getQueryParameters() const {
  if(!m_queryParamsParsed) {
    webfr::network::Url::Parser::parseQueryParams(m_queryParams, m_pathVariables.getTail());
    m_queryParamsParsed = true;
  }
  return m_queryParams;
}

webfr::String Request::getQueryParameter(const webfr::data::share::StringKeyLabel& name) const {
  return getQueryParameters().get(name);
}

webfr::String Request::getQueryParameter(const webfr::data::share::StringKeyLabel& name, const webfr::String& defaultValue) const {
  auto value = getQueryParameter(name);
  return value ? value : defaultValue;
}

std::shared_ptr<webfr::data::stream::InputStream> Request::getBodyStream() const {
  return m_bodyStream;
}

std::shared_ptr<const http::incoming::BodyDecoder> Request::getBodyDecoder() const {
  return m_bodyDecoder;
}

void Request::putHeader(const webfr::String& key, const webfr::String& value) {
  m_headers.put(key, value);
}

bool Request::putHeaderIfNotExists(const webfr::String& key, const webfr::String& value) {
  return m_headers.putIfNotExists(key, value);
}

void Request::putHeader_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  m_headers.put(key, value);
}

bool Request::putHeaderIfNotExists_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  return m_headers.putIfNotExists(key, value);
}

webfr::String Request::getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const{
  return m_headers.get(headerName);
}

webfr::String Request::getPathVariable(const webfr::data::share::StringKeyLabel& name) const {
  return m_pathVariables.getVariable(name);
}

webfr::String Request::getPathTail() const {
  return m_pathVariables.getTail();
}

void Request::transferBody(data::stream::WriteCallback* writeCallback) const {
  m_bodyDecoder->decode(m_headers, m_bodyStream.get(), writeCallback);
}

void Request::transferBodyToStream(webfr::data::stream::OutputStream* toStream) const {
  m_bodyDecoder->decode(m_headers, m_bodyStream.get(), toStream);
}

webfr::String Request::readBodyToString() const {
  return m_bodyDecoder->decodeToString(m_headers, m_bodyStream.get());
}

async::CoroutineStarter Request::transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const {
  return m_bodyDecoder->decodeAsync(m_headers, m_bodyStream, writeCallback);
}

async::CoroutineStarter Request::transferBodyToStreamAsync(const std::shared_ptr<webfr::data::stream::OutputStream>& toStream) const {
  return m_bodyDecoder->decodeAsync(m_headers, m_bodyStream, toStream);
}

async::CoroutineStarterForResult<const webfr::String&> Request::readBodyToStringAsync() const {
  return m_bodyDecoder->decodeToStringAsync(m_headers, m_bodyStream);
}

}}}}}