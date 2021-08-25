#include "./Response.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

Response::Response(v_int32 statusCode,
                   const webfr::String& statusDescription,
                   const http::Headers& headers,
                   const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
                   const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder)
  : m_statusCode(statusCode)
  , m_statusDescription(statusDescription)
  , m_headers(headers)
  , m_bodyStream(bodyStream)
  , m_bodyDecoder(bodyDecoder)
{}

std::shared_ptr<Response> Response::createShared(v_int32 statusCode,
                                                 const webfr::String& statusDescription,
                                                 const http::Headers& headers,
                                                 const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
                                                 const std::shared_ptr<const http::incoming::BodyDecoder>& bodyDecoder) {
  return Shared_Incoming_Response_Pool::allocateShared(statusCode, statusDescription, headers, bodyStream, bodyDecoder);
}

v_int32 Response::getStatusCode() const {
  return m_statusCode;
}

webfr::String Response::getStatusDescription() const {
  return m_statusDescription;
}

const http::Headers& Response::getHeaders() const {
  return m_headers;
}

void Response::putHeader(const webfr::String& key, const webfr::String& value) {
  m_headers.put(key, value);
}

bool Response::putHeaderIfNotExists(const webfr::String& key, const webfr::String& value) {
  return m_headers.putIfNotExists(key, value);
}

void Response::putHeader_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  m_headers.put(key, value);
}

bool Response::putHeaderIfNotExists_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  return m_headers.putIfNotExists(key, value);
}

webfr::String Response::getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const{
  return m_headers.get(headerName);
}

std::shared_ptr<webfr::data::stream::InputStream> Response::getBodyStream() const {
  return m_bodyStream;
}

std::shared_ptr<const http::incoming::BodyDecoder> Response::getBodyDecoder() const {
  return m_bodyDecoder;
}

void Response::transferBody(data::stream::WriteCallback* writeCallback) const {
  m_bodyDecoder->decode(m_headers, m_bodyStream.get(), writeCallback);
}

void Response::transferBodyToStream(webfr::data::stream::OutputStream* toStream) const {
  m_bodyDecoder->decode(m_headers, m_bodyStream.get(), toStream);
}

webfr::String Response::readBodyToString() const {
  return m_bodyDecoder->decodeToString(m_headers, m_bodyStream.get());
}

async::CoroutineStarter Response::transferBodyAsync(const std::shared_ptr<data::stream::WriteCallback>& writeCallback) const {
  return m_bodyDecoder->decodeAsync(m_headers, m_bodyStream, writeCallback);
}

webfr::async::CoroutineStarter Response::transferBodyToStreamAsync(const std::shared_ptr<webfr::data::stream::OutputStream>& toStream) const {
  return m_bodyDecoder->decodeAsync(m_headers, m_bodyStream, toStream);
}

}}}}}