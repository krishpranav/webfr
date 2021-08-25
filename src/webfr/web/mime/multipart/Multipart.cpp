#include "Multipart.hpp"
#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/encoding/Base64.hpp"
#include "webfr/core/utils/Random.hpp"

namespace webfr { namespace web { namespace mime { namespace multipart {

Multipart::Multipart(const webfr::String& boundary)
  : m_boundary(boundary)
{}

webfr::String Multipart::getBoundary() {
  return m_boundary;
}

std::shared_ptr<Part> Multipart::readNextPartSimple() {
  async::Action action;
  auto result = readNextPart(action);
  if(!action.isNone()) {
    throw std::runtime_error("[webfr::web::mime::multipart::Multipart::readNextPartSimple()]. Error."
                             "Async method is called for non-async API.");
  }
  return result;
}

void Multipart::writeNextPartSimple(const std::shared_ptr<Part>& part) {
  async::Action action;
  writeNextPart(part, action);
  if(!action.isNone()) {
    throw std::runtime_error("[webfr::web::mime::multipart::Multipart::writeNextPartSimple()]. Error."
                             "Async method is called for non-async API.");
  }
}

webfr::String Multipart::generateRandomBoundary(v_int32 boundarySize) {
  std::unique_ptr<v_char8[]> buffer(new v_char8[boundarySize]);
  utils::random::Random::randomBytes(buffer.get(), boundarySize);
  return encoding::Base64::encode(buffer.get(), boundarySize, encoding::Base64::ALPHABET_BASE64_URL_SAFE);
}

webfr::String Multipart::parseBoundaryFromHeaders(const Headers& requestHeaders) {

  webfr::String boundary;
  auto contentType = requestHeaders.getAsMemoryLabel<webfr::data::share::StringKeyLabel>("Content-Type");

  if(contentType) {
    webfr::web::protocol::http::HeaderValueData valueData;
    webfr::web::protocol::http::Parser::parseHeaderValueData(valueData, contentType, ';');
    boundary = valueData.getTitleParamValue("boundary");
  }

  return boundary;

}

}}}}