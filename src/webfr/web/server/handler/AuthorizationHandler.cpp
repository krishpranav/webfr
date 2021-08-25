#include "AuthorizationHandler.hpp"
#include "webfr/encoding/Base64.hpp"
#include "webfr/core/parser/Caret.hpp"

namespace webfr { namespace web { namespace server { namespace handler {

AuthorizationHandler::AuthorizationHandler(const webfr::String& scheme, const webfr::String& realm)
  : m_scheme(scheme)
  , m_realm(realm)
{}

void AuthorizationHandler::renderAuthenticateHeaderValue(ChunkedBuffer& stream) {
  stream << m_scheme << " " << "realm=\"" << m_realm << "\"";
}

void AuthorizationHandler::addErrorResponseHeaders(Headers& headers) {
  ChunkedBuffer stream;
  renderAuthenticateHeaderValue(stream);
  headers.put_LockFree(protocol::http::Header::WWW_AUTHENTICATE, stream.toString());
}

webfr::String AuthorizationHandler::getScheme() {
  return m_scheme;
}

webfr::String AuthorizationHandler::getRealm() {
  return m_realm;
}


BasicAuthorizationHandler::BasicAuthorizationHandler(const webfr::String& realm)
  : AuthorizationHandler("Basic", realm)
{}

std::shared_ptr<handler::AuthorizationObject> BasicAuthorizationHandler::handleAuthorization(const webfr::String &header) {

  if(header && header->getSize() > 6 && header->startsWith("Basic ")) {

    webfr::String auth = webfr::encoding::Base64::decode(header->c_str() + 6, header->getSize() - 6);
    parser::Caret caret(auth);

    if (caret.findChar(':')) {
      webfr::String userId((const char *) &caret.getData()[0], caret.getPosition(), true);
      webfr::String password((const char *) &caret.getData()[caret.getPosition() + 1],
                             caret.getDataSize() - caret.getPosition() - 1, true);
      auto authResult = authorize(userId, password);
      if(authResult) {
        return authResult;
      }

      Headers responseHeaders;
      addErrorResponseHeaders(responseHeaders);
      throw protocol::http::HttpError(protocol::http::Status::CODE_401, "Unauthorized", responseHeaders);

    }

  }

  Headers responseHeaders;
  addErrorResponseHeaders(responseHeaders);
  throw protocol::http::HttpError(protocol::http::Status::CODE_401, "Authorization Required", responseHeaders);

}

std::shared_ptr<AuthorizationObject> BasicAuthorizationHandler::authorize(const webfr::String &userId,
                                                                          const webfr::String &password)
{
  auto authorizationObject = std::make_shared<DefaultBasicAuthorizationObject>();
  authorizationObject->userId = userId;
  authorizationObject->password = password;
  return authorizationObject;
}

BearerAuthorizationHandler::BearerAuthorizationHandler(const webfr::String& realm)
  : AuthorizationHandler("Bearer", realm)
{}

std::shared_ptr<AuthorizationObject> BearerAuthorizationHandler::handleAuthorization(const webfr::String &header) {

  if(header && header->getSize() > 7 && header->startsWith("Bearer ")) {

    webfr::String token = webfr::String(header->c_str() + 7, header->getSize() - 7, true);

    auto authResult = authorize(token);
    if(authResult) {
      return authResult;
    }

    Headers responseHeaders;
    addErrorResponseHeaders(responseHeaders);
    throw protocol::http::HttpError(protocol::http::Status::CODE_401, "Unauthorized", responseHeaders);

  }

  Headers responseHeaders;
  addErrorResponseHeaders(responseHeaders);
  throw protocol::http::HttpError(protocol::http::Status::CODE_401, "Authorization Required", responseHeaders);

}

std::shared_ptr<AuthorizationObject> BearerAuthorizationHandler::authorize(const webfr::String& token) {
  auto authObject = std::make_shared<DefaultBearerAuthorizationObject>();
  authObject->token = token;
  return authObject;
}

}}}}