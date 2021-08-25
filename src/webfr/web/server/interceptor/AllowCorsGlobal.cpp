#include "AllowCorsGlobal.hpp"

namespace webfr { namespace web { namespace server { namespace interceptor {

std::shared_ptr<protocol::http::outgoing::Response> AllowOptionsGlobal::intercept(const std::shared_ptr<IncomingRequest> &request) {

  const auto &line = request->getStartingLine();

  if (line.method == "OPTIONS") {
    return OutgoingResponse::createShared(protocol::http::Status::CODE_204, nullptr);
  }

  return nullptr;

}

AllowCorsGlobal::AllowCorsGlobal(const webfr::String &origin,
                                 const webfr::String &methods,
                                 const webfr::String &headers,
                                 const webfr::String &maxAge)
  : m_origin(origin)
  , m_methods(methods)
  , m_headers(headers)
  , m_maxAge(maxAge)
{}

std::shared_ptr<protocol::http::outgoing::Response> AllowCorsGlobal::intercept(const std::shared_ptr<IncomingRequest>& request,
                                                                               const std::shared_ptr<OutgoingResponse>& response)
{
  response->putHeaderIfNotExists(protocol::http::Header::CORS_ORIGIN, m_origin);
  response->putHeaderIfNotExists(protocol::http::Header::CORS_METHODS, m_methods);
  response->putHeaderIfNotExists(protocol::http::Header::CORS_HEADERS, m_headers);
  response->putHeaderIfNotExists(protocol::http::Header::CORS_MAX_AGE, m_maxAge);
  return response;
}

}}}}