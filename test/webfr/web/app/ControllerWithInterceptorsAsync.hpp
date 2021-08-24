#ifndef webfr_test_web_app_ControllerWithInterceptorsAsync_hpp
#define webfr_test_web_app_ControllerWithInterceptorsAsync_hpp

#include "webfr/web/server/api/ApiController.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/macro/component.hpp"
#include <sstream>

namespace webfr { namespace test { namespace web { namespace app {

namespace multipart = webfr::web::mime::multipart;

class ControllerWithInterceptorsAsync : public webfr::web::server::api::ApiController {
private:
  static constexpr const char* TAG = "test::web::app::ControllerWithInterceptorsAsync";
public:
  ControllerWithInterceptorsAsync(const std::shared_ptr<ObjectMapper>& objectMapper)
    : webfr::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<ControllerWithInterceptorsAsync> createShared(const std::shared_ptr<ObjectMapper>& objectMapper = WEBFR_GET_COMPONENT(std::shared_ptr<ObjectMapper>)){
    return std::make_shared<ControllerWithInterceptorsAsync>(objectMapper);
  }

#include WEBFR_CODEGEN_BEGIN(ApiController)

  ENDPOINT_INTERCEPTOR_ASYNC(Interceptor, inter1) {

    WEBFR_ASSERT(request->getHeader("header-in-inter2") == "inter2");
    WEBFR_ASSERT(request->getHeader("header-in-inter3") == "inter3");

    request->putHeader("header-in-inter1", "inter1");
    return (this->*intercepted)(request);

  }
  ENDPOINT_INTERCEPTOR_ASYNC(Interceptor, inter2) {

    WEBFR_ASSERT(request->getHeader("header-in-inter3") == "inter3");

    request->putHeader("header-in-inter2", "inter2");
    return (this->*intercepted)(request);

  }
  ENDPOINT_INTERCEPTOR_ASYNC(Interceptor, inter3) {

    class IterceptorCoroutine : public webfr::async::CoroutineWithResult<IterceptorCoroutine, const std::shared_ptr<OutgoingResponse>&> {
    private:
      ControllerWithInterceptorsAsync* m_this;
      Handler<ControllerWithInterceptorsAsync>::MethodAsync m_intercepted;
      std::shared_ptr<IncomingRequest> m_request;
    public:

      IterceptorCoroutine(ControllerWithInterceptorsAsync* _this,
                          const Handler<ControllerWithInterceptorsAsync>::MethodAsync& intercepted,
                          const std::shared_ptr<IncomingRequest>& request)
        : m_this(_this)
        , m_intercepted(intercepted)
        , m_request(request)
      {}

      webfr::async::Action act() override {
        m_request->putHeader("header-in-inter3", "inter3");
        return (m_this->*m_intercepted)(m_request).callbackTo(&IterceptorCoroutine::onResponse);
      }

      webfr::async::Action onResponse(const std::shared_ptr<OutgoingResponse>& response) {
        response->putHeader("header-out-inter3", "inter3");
        return this->_return(response);
      }

    };

    return IterceptorCoroutine::startForResult(this, intercepted, request);

  }
  ENDPOINT_INTERCEPTOR_ASYNC(Interceptor, asserter) {

    class IterceptorCoroutine : public webfr::async::CoroutineWithResult<IterceptorCoroutine, const std::shared_ptr<OutgoingResponse>&> {
    private:
      ControllerWithInterceptorsAsync* m_this;
      Handler<ControllerWithInterceptorsAsync>::MethodAsync m_intercepted;
      std::shared_ptr<IncomingRequest> m_request;
    public:

      IterceptorCoroutine(ControllerWithInterceptorsAsync* _this,
                          const Handler<ControllerWithInterceptorsAsync>::MethodAsync& intercepted,
                          const std::shared_ptr<IncomingRequest>& request)
        : m_this(_this)
        , m_intercepted(intercepted)
        , m_request(request)
      {}

      webfr::async::Action act() override {
        return (m_this->*m_intercepted)(m_request).callbackTo(&IterceptorCoroutine::onResponse);
      }

      webfr::async::Action onResponse(const std::shared_ptr<OutgoingResponse>& response) {
        WEBFR_ASSERT(response->getHeader("header-out-inter3") == "inter3");
        return this->_return(response);
      }

    };

    return IterceptorCoroutine::startForResult(this, intercepted, request);

  }
  ENDPOINT_ASYNC("GET", "test/interceptors", Interceptor) {

    ENDPOINT_ASYNC_INIT(Interceptor)

    Action act() {

      WEBFR_ASSERT(request->getHeader("header-in-inter1") == "inter1");
      WEBFR_ASSERT(request->getHeader("header-in-inter2") == "inter2");
      WEBFR_ASSERT(request->getHeader("header-in-inter3") == "inter3");

      return _return(controller->createResponse(Status::CODE_200, "Hello World Async!!!"));
    }

  };

#include WEBFR_CODEGEN_END(ApiController)

};

}}}}

#endif 