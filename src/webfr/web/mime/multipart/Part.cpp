#include "Part.hpp"
#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/core/parser/Caret.hpp"

#include <cstring>

namespace webfr { namespace web { namespace mime { namespace multipart {

Part::Part(const Headers &headers,
           const std::shared_ptr<data::stream::InputStream> &inputStream,
           const webfr::String& inMemoryData,
           v_int64 knownSize)
  : m_headers(headers)
  , m_inputStream(inputStream)
  , m_inMemoryData(inMemoryData)
  , m_knownSize(knownSize)
{

  auto contentDisposition = m_headers.getAsMemoryLabel<webfr::data::share::StringKeyLabel>("Content-Disposition");
  if(contentDisposition) {

    webfr::web::protocol::http::HeaderValueData valueData;
    webfr::web::protocol::http::Parser::parseHeaderValueData(valueData, contentDisposition, ';');

    m_name = valueData.getTitleParamValue("name");
    m_filename = valueData.getTitleParamValue("filename");

  }

}


Part::Part(const Headers& headers) : Part(headers, nullptr, nullptr, -1) {}

Part::Part() : Part(Headers(), nullptr, nullptr, -1) {}

void Part::setDataInfo(const std::shared_ptr<data::stream::InputStream>& inputStream,
                       const webfr::String& inMemoryData,
                       v_int64 knownSize)
{
  m_inputStream = inputStream;
  m_inMemoryData = inMemoryData;
  m_knownSize = knownSize;
}

void Part::setDataInfo(const std::shared_ptr<data::stream::InputStream>& inputStream) {
  m_inputStream = inputStream;
  m_inMemoryData = nullptr;
  m_knownSize = -1;
}

webfr::String Part::getName() const {
  return m_name;
}


webfr::String Part::getFilename() const {
  return m_filename;
}


const Part::Headers& Part::getHeaders() const {
  return m_headers;
}

webfr::String Part::getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const {
  return m_headers.get(headerName);
}

void Part::putHeader(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  m_headers.put(key, value);
}

bool Part::putHeaderIfNotExists(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  return m_headers.putIfNotExists(key, value);
}

std::shared_ptr<data::stream::InputStream> Part::getInputStream() const {
  return m_inputStream;
}

webfr::String Part::getInMemoryData() const {
  return m_inMemoryData;
}

v_int64 Part::getKnownSize() const {
  return m_knownSize;
}

void Part::setTag(const char* tagName, const std::shared_ptr<webfr::base::Countable>& tagObject) {
  m_tagName = tagName;
  m_tagObject = tagObject;
}

const char* Part::getTagName() {
  return m_tagName;
}

std::shared_ptr<webfr::base::Countable> Part::getTagObject() {
  return m_tagObject;
}

void Part::clearTag() {
  m_tagName = nullptr;
  m_tagObject.reset();
}

}}}}