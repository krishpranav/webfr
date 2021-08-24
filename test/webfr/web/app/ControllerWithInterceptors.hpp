#ifndef webfr_test_web_app_ControllerWithInterceptors_hpp
#define webfr_test_web_app_ControllerWithInterceptors_hpp

#include "webfr/web/server/api/ApiController.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/macro/component.hpp"

#include <sstream>

namespace webfr { namespace test { namespace web { namespace app {

namespace multipart = webfr::web::mime::multipart;

class ControllerWithInterceptors : public webfr::web::server::api::ApiController {
private:
  static constexpr const char* TAG = "test::web::app::ControllerWithInterceptors";
public:
  ControllerWithInterceptors(const std::shared_ptr<ObjectMapper>& objectMapper)
    : webfr::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<ControllerWithInterceptors> createShared(const std::shared_ptr<ObjectMapper>& objectMapper = WEBFR_GET_COMPONENT(std::shared_ptr<ObjectMapper>)){
    return std::make_shared<ControllerWithInterceptors>(objectMapper);
  }

#include WEBFR_CODEGEN_BEGIN(ApiController)

  ENDPOINT_INTERCEPTOR(interceptor, inter1) {

    /* assert order of interception */
    WEBFR_ASSERT(request->getHeader("header-in-inter2") == "inter2");
    WEBFR_ASSERT(request->getHeader("header-in-inter3") == "inter3");
    /********************************/

    request->putHeader("header-in-inter1", "inter1");
    auto response = (this->*intercepted)(request);
    response->putHeader("header-out-inter1", "inter1");
    return response;

  }
  ENDPOINT_INTERCEPTOR(interceptor, inter2) {

    /* assert order of interception */
    WEBFR_ASSERT(request->getHeader("header-in-inter3") == "inter3");
    /********************************/

    request->putHeader("header-in-inter2", "inter2");
    auto response = (this->*intercepted)(request);
    response->putHeader("header-out-inter2", "inter2");
    return response;

  }
  ENDPOINT_INTERCEPTOR(interceptor, inter3) {
    request->putHeader("header-in-inter3", "inter3");
    auto response = (this->*intercepted)(request);
    response->putHeader("header-out-inter3", "inter3");
    return response;
  }
  ENDPOINT_INTERCEPTOR(interceptor, asserter) {
    auto response = (this->*intercepted)(request);

    WEBFR_ASSERT(response->getHeader("header-out-inter1") == "inter1");
    WEBFR_ASSERT(response->getHeader("header-out-inter2") == "inter2");
    WEBFR_ASSERT(response->getHeader("header-out-inter3") == "inter3");

    return response;
  }
  ENDPOINT("GET", "test/interceptors", interceptor,
           REQUEST(std::shared_ptr<IncomingRequest>, request))
  {

    WEBFR_ASSERT(request->getHeader("header-in-inter1") == "inter1");
    WEBFR_ASSERT(request->getHeader("header-in-inter2") == "inter2");
    WEBFR_ASSERT(request->getHeader("header-in-inter3") == "inter3");

    return createResponse(Status::CODE_200, "Hello World!!!");

  }


#include WEBFR_CODEGEN_END(ApiController)

};

}}}}

#endif /* WEBFR_test_web_app_ControllerWithInterceptors_hpp */