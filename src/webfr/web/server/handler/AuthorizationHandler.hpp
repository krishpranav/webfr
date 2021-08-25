#ifndef webfr_web_server_handler_AuthorizationHandler_hpp
#define webfr_web_server_handler_AuthorizationHandler_hpp

#include <webfr/web/protocol/http/incoming/Request.hpp>
#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/data/mapping/type/Type.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"


namespace webfr { namespace web { namespace server { namespace handler {

class AuthorizationObject : public webfr::base::Countable {
protected:
  AuthorizationObject() = default;
};

/**
 * Abstract Authorization Handler.
 */
class AuthorizationHandler {
public:
  /**
   * Convenience typedef for &l:AuthorizationObject;.
   */
  typedef webfr::web::server::handler::AuthorizationObject AuthorizationObject;

  /**
   * Convenience typedef for &id:webfr::data::stream::ChunkedBuffer;.
   */
  typedef webfr::data::stream::ChunkedBuffer ChunkedBuffer;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::Headers;.
   */
  typedef webfr::web::protocol::http::Headers Headers;
private:
  webfr::String m_scheme;
  webfr::String m_realm;
public:

  /**
   * Constructor.
   * @param scheme - authorization type scheme. &id:webfr::String;.
   * @param realm - realm. &id:webfr::String;.
   */
  AuthorizationHandler(const webfr::String& scheme, const webfr::String& realm);

  /**
   * Default virtual destructor.
   */
  virtual ~AuthorizationHandler() = default;

  /**
   * Implement this method! Return nullptr if authorization should be denied.
   * @param header - `Authorization` header. &id:webfr::String;.
   * @return - `std::shared_ptr` to &id:webfr::web::server::handler::AuthorizationObject;.
   */
  virtual std::shared_ptr<AuthorizationObject> handleAuthorization(const webfr::String& authorizationHeader) = 0;

  /**
   * Render WWW-Authenicate header value. <br>
   * Custom Authorization handlers may override this method in order to provide additional information.
   * @param stream - &id:webfr::data::stream::ChunkedBuffer;.
   */
  virtual void renderAuthenticateHeaderValue(ChunkedBuffer& stream);

  /**
   * Add authorization error headers to the headers map. <br>
   * @param headers - &id:webfr::web::protocol::http::Headers;.
   */
  virtual void addErrorResponseHeaders(Headers& headers);

  /**
   * Get authorization scheme.
   * @return
   */
  webfr::String getScheme();

  /**
   * Get authorization realm.
   * @return
   */
  webfr::String getRealm();

};

/**
 * Default Basic AuthorizationObject - Convenience object to enable Basic-Authorization without the need to implement anything.
 */
class DefaultBasicAuthorizationObject : public AuthorizationObject {
public:

  /**
   * User-Id. &id:webfr::String;.
   */
  webfr::String userId;

  /**
   * Password. &id:webfr::String;.
   */
  webfr::String password;

};

/**
 * AuthorizationHandler for Authorization Type `Basic`. <br>
 * See [RFC 7617](https://tools.ietf.org/html/rfc7617). <br>
 * Extend this class to implement Custom Basic Authorization.
 */
class BasicAuthorizationHandler : public AuthorizationHandler {
public:

  /**
   * Constructor.
   * @param realm
   */
  BasicAuthorizationHandler(const webfr::String& realm = "API");

  /**
   * Implementation of &l:AuthorizationHandler::handleAuthorization ();
   * @param header - &id:webfr::String;.
   * @return - std::shared_ptr to &id:webfr::web::server::handler::AuthorizationObject;.
   */
  std::shared_ptr<AuthorizationObject> handleAuthorization(const webfr::String &header) override;

  /**
   * Implement this method! Do the actual authorization here. When not implemented returns &l:DefaultBasicAuthorizationObject;.
   * @param userId - user id. &id:webfr::String;.
   * @param password - password. &id:webfr::String;.
   * @return - `std::shared_ptr` to &l:AuthorizationObject;. `nullptr` - for "Unauthorized".
   */
  virtual std::shared_ptr<AuthorizationObject> authorize(const webfr::String& userId, const webfr::String& password);

};

/**
 * Default Bearer AuthorizationObject - Convenience object to enable Bearer-Authorization without the need to implement anything.
 */
class DefaultBearerAuthorizationObject : public AuthorizationObject {
public:

  /**
   * Token. &id:webfr::String;.
   */
  webfr::String token;

};

/**
 * AuthorizationHandler for Authorization Type `Bearer`. <br>
 * See [RFC 6750](https://tools.ietf.org/html/rfc6750). <br>
 * Extend this class to implement Custom Bearer Authorization.
 */
class BearerAuthorizationHandler : public AuthorizationHandler {
public:

  /**
   * Constructor.
   * @param realm
   */
  BearerAuthorizationHandler(const webfr::String& realm = "API");

  /**
   * Implementation of &l:AuthorizationHandler::handleAuthorization ();
   * @param header - &id:webfr::String;.
   * @return - std::shared_ptr to &id:webfr::web::server::handler::AuthorizationObject;.
   */
  std::shared_ptr<AuthorizationObject> handleAuthorization(const webfr::String &header) override;

  /**
   * Implement this method! Do the actual authorization here. When not implemented returns &l:DefaultBearerAuthorizationObject;.
   * @param token - access token.
   * @return - `std::shared_ptr` to &l:AuthorizationObject;. `nullptr` - for "Unauthorized".
   */
  virtual std::shared_ptr<AuthorizationObject> authorize(const webfr::String& token);

};

}}}}

#endif 