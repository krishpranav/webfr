#include "AuthorizationHandlerTest.hpp"
#include "webfr/web/server/handler/AuthorizationHandler.hpp"

namespace webfr { namespace test { namespace web { namespace server { namespace handler {

namespace {

class MyBasicAuthorizationObject : public webfr::web::server::handler::AuthorizationObject {
public:
  webfr::String userId;
  webfr::String password;
};

class MyBasicAuthorizationHandler : public webfr::web::server::handler::BasicAuthorizationHandler {
public:

  std::shared_ptr<AuthorizationObject> authorize(const webfr::String& userId, const webfr::String& password) {
    auto authObject = std::make_shared<MyBasicAuthorizationObject>();
    authObject->userId = userId;
    authObject->password = password;
    return authObject;
  }

};

}

void AuthorizationHandlerTest::onRun() {

  webfr::String user = "foo";
  webfr::String password = "bar";
  webfr::String header = "Basic Zm9vOmJhcg==";

  {
    MyBasicAuthorizationHandler basicAuthHandler;
    auto auth = std::static_pointer_cast<MyBasicAuthorizationObject>(basicAuthHandler.handleAuthorization(header));
    WEBFR_LOGV(TAG, "header=\"%s\" -> user=\"%s\" password=\"%s\"", header->c_str(), auth->userId->c_str(), auth->password->c_str());
    WEBFR_ASSERT(auth->userId->equals("foo"));
    WEBFR_ASSERT(auth->password->equals("bar"));
  }

  {
    webfr::web::server::handler::BasicAuthorizationHandler defaultBasicAuthHandler;
    auto auth = std::static_pointer_cast<webfr::web::server::handler::DefaultBasicAuthorizationObject>(defaultBasicAuthHandler.handleAuthorization(header));
    WEBFR_LOGV(TAG, "header=\"%s\" -> user=\"%s\" password=\"%s\"", header->c_str(), auth->userId->c_str(), auth->password->c_str());
    WEBFR_ASSERT(auth->userId->equals("foo"));
    WEBFR_ASSERT(auth->password->equals("bar"));
  }

}

}}}}}