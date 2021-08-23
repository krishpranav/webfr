#ifndef webfr_test_web_app_BearerAuthorizationController_hpp
#define webfr_test_web_app_BearerAuthorizationController_hpp

#include "./DTOs.hpp"
#include "webfr/web/server/api/ApiController.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/macro/component.hpp"

#include <sstream>

namespace oatpp { namespace test { namespace web { namespace app {

class BearerAuthorizationObject : public webfr::web::server::handler::AuthorizationObject {
public:
  webfr::String user;
  webfr::String password;
  webfr::String token;
};

class MyBearerAuthorizationHandler : public webfr::web::server::handler::BearerAuthorizationHandler {
public:

  MyBearerAuthorizationHandler()
    : webfr::web::server::handler::BearerAuthorizationHandler("custom-bearer-realm")
  {}

  std::shared_ptr<AuthorizationObject> authorize(const webfr::String& token) override {

    if(token == "4e99e8c12de7e01535248d2bac85e732") {
      auto obj = std::make_shared<BearerAuthorizationObject>();
      obj->user = "foo";
      obj->password = "bar";
      obj->token = token;
      return obj;
    }

    return nullptr;
  }

};

class BearerAuthorizationController : public webfr::web::server::api::ApiController {
private:
  static constexpr const char* TAG = "test::web::app::BearerAuthorizationController";

private:
  std::shared_ptr<AuthorizationHandler> m_authHandler = std::make_shared<MyBearerAuthorizationHandler>();
public:

  BearerAuthorizationController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : webfr::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<BearerAuthorizationController> createShared(const std::shared_ptr<ObjectMapper>& objectMapper = WEBFR_GET_COMPONENT(std::shared_ptr<ObjectMapper>)){
    return std::make_shared<BearerAuthorizationController>(objectMapper);
  }

#include WEBFR_CODEGEN_BEGIN(ApiController)

  ENDPOINT("GET", "bearer-authorization", authorization,
           AUTHORIZATION(std::shared_ptr<BearerAuthorizationObject>, authorizatioBearer, m_authHandler)) {
    auto dto = TestDto::createShared();
    dto->testValue = authorizatioBearer->user + ":" + authorizatioBearer->password;
    if(dto->testValue == "foo:bar" && authorizatioBearer->token == "4e99e8c12de7e01535248d2bac85e732") {
      return createDtoResponse(Status::CODE_200, dto);
    } else {
      return createDtoResponse(Status::CODE_401, dto);
    }
  }

#include WEBFR_CODEGEN_END(ApiController)

};

}}}}

#endif /* WEBFR_test_web_app_Controller_hpp */