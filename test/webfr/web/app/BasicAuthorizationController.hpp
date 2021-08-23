#ifndef webfr_test_web_app_AuthorizationController_hpp
#define webfr_test_web_app_AuthorizationController_hpp

#include "./DTOs.hpp"

#include "webfr/web/server/api/ApiController.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/macro/component.hpp"

#include <sstream>

namespace webfr { namespace test { namespace web { namespace app {


class DefaultBasicAuthorizationController : public webfr::web::server::api::ApiController {
private:
  static constexpr const char* TAG = "test::web::app::BasicAuthorizationController";
public:

  DefaultBasicAuthorizationController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : webfr::web::server::api::ApiController(objectMapper)
  {
    setDefaultAuthorizationHandler(std::make_shared<webfr::web::server::handler::BasicAuthorizationHandler>("default-test-realm"));
  }

public:

  static std::shared_ptr<DefaultBasicAuthorizationController> createShared(const std::shared_ptr<ObjectMapper>& objectMapper = WEBFR_GET_COMPONENT(std::shared_ptr<ObjectMapper>)){
    return std::make_shared<DefaultBasicAuthorizationController>(objectMapper);
  }

#include WEBFR_CODEGEN_BEGIN(ApiController)

  ENDPOINT("GET", "default-basic-authorization", basicAuthorization,
           AUTHORIZATION(std::shared_ptr<webfr::web::server::handler::DefaultBasicAuthorizationObject>, authObject)) {

    auto dto = TestDto::createShared();
    dto->testValue = authObject->userId + ":" + authObject->password;

    if(dto->testValue == "foo:bar") {
      return createDtoResponse(Status::CODE_200, dto);
    } else {
      return createDtoResponse(Status::CODE_401, dto);
    }

  }

#include WEBFR_CODEGEN_END(ApiController)

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Custom Basic Authorization

class MyAuthorizationObject : public webfr::web::server::handler::AuthorizationObject {
public:

  MyAuthorizationObject(v_int64 pId, const webfr::String& pAuthString)
    : id(pId)
    , authString(pAuthString)
  {}

  v_int64 id;
  webfr::String authString;

};

class MyBasicAuthorizationHandler : public webfr::web::server::handler::BasicAuthorizationHandler {
public:

  MyBasicAuthorizationHandler()
    : BasicAuthorizationHandler("custom-test-realm")
  {}

  std::shared_ptr<AuthorizationObject> authorize(const webfr::String& userId, const webfr::String& password) override {
    if(userId == "foo" && password == "bar") {
      return std::make_shared<MyAuthorizationObject>(1337, userId + ":" + password);
    }
    return nullptr;
  }

};

class BasicAuthorizationController : public webfr::web::server::api::ApiController {
private:
  static constexpr const char* TAG = "test::web::app::BasicAuthorizationController";
public:
  std::shared_ptr<AuthorizationHandler> m_authHandler = std::make_shared<MyBasicAuthorizationHandler>();
public:
  BasicAuthorizationController(const std::shared_ptr<ObjectMapper>& objectMapper)
    : webfr::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<BasicAuthorizationController> createShared(const std::shared_ptr<ObjectMapper>& objectMapper = WEBFR_GET_COMPONENT(std::shared_ptr<ObjectMapper>)){
    return std::make_shared<BasicAuthorizationController>(objectMapper);
  }

#include WEBFR_CODEGEN_BEGIN(ApiController)

  ENDPOINT("GET", "basic-authorization", basicAuthorization,
           AUTHORIZATION(std::shared_ptr<MyAuthorizationObject>, authObject, m_authHandler)) {

    auto dto = TestDto::createShared();
    dto->testValue = authObject->authString;

    if(dto->testValue == "foo:bar" && authObject->id == 1337) {
      return createDtoResponse(Status::CODE_200, dto);
    } else {
      return createDtoResponse(Status::CODE_401, dto);
    }

  }

#include WEBFR_CODEGEN_END(ApiController)

};

}}}}

#endif /* WEBFR_test_web_app_Controller_hpp */