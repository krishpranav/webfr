#ifndef webfr_web_server_rest_Controller_hpp
#define webfr_web_server_rest_Controller_hpp

#include "./Endpoint.hpp"
#include "webfr/web/server/handler/AuthorizationHandler.hpp"
#include "webfr/web/server/handler/ErrorHandler.hpp"
#include "webfr/web/server/handler/AuthorizationHandler.hpp"
#include "webfr/web/server/HttpConnectionHandler.hpp"
#include "webfr/web/url/mapping/Router.hpp"
#include "webfr/web/protocol/http/incoming/Response.hpp"
#include "webfr/web/protocol/http/outgoing/Request.hpp"
#include "webfr/web/protocol/http/outgoing/ResponseFactory.hpp"
#include "webfr/core/collection/LinkedList.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include <unordered_map>

namespace webfr { namespace web { namespace server { namespace api {

class ApiController : public webfr::base::Countable {
protected:
  typedef ApiController __ControllerType;
public:
  
  typedef webfr::web::server::HttpRouter Router;

  typedef webfr::web::protocol::http::outgoing::ResponseFactory ResponseFactory;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::incoming::Request;.
   */
  typedef webfr::web::protocol::http::incoming::Request IncomingRequest;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::outgoing::Request;.
   */
  typedef webfr::web::protocol::http::outgoing::Request OutgoingRequest;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::incoming::Response;.
   */
  typedef webfr::web::protocol::http::incoming::Response IncomingResponse;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::outgoing::Response;.
   */
  typedef webfr::web::protocol::http::outgoing::Response OutgoingResponse;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::Status;.
   */
  typedef webfr::web::protocol::http::Status Status;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::Header;.
   */
  typedef webfr::web::protocol::http::Header Header;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::QueryParams;.
   */
  typedef webfr::web::protocol::http::QueryParams QueryParams;

  /**
   * Convenience typedef for &id:webfr::web::server::api::Endpoint;.
   */
  typedef webfr::web::server::api::Endpoint Endpoint;

  /**
   * Convenience typedef for list of &id:webfr::web::server::api::Endpoint;.
   */
  typedef webfr::collection::LinkedList<std::shared_ptr<Endpoint>> Endpoints;

  /**
   * Convenience typedef for &id:webfr::web::server::HttpRequestHandler;.
   */
  typedef webfr::web::server::HttpRequestHandler RequestHandler;

  /**
   * Convenience typedef for &id:webfr::web::server::handler::AuthorizationHandler;.
   */
  typedef webfr::web::server::handler::AuthorizationHandler AuthorizationHandler;
  
public:

  /**
   * Convenience typedef for &id:webfr::data::mapping::ObjectMapper;.
   */
  typedef webfr::data::mapping::ObjectMapper ObjectMapper;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::String;.
   */
  typedef webfr::String String;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Int8;.
   */
  typedef webfr::Int8 Int8;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::UInt8;.
   */
  typedef webfr::UInt8 UInt8;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Int16;.
   */
  typedef webfr::Int16 Int16;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::UInt16;.
   */
  typedef webfr::UInt16 UInt16;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Int32;.
   */
  typedef webfr::Int32 Int32;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::UInt32;.
   */
  typedef webfr::UInt32 UInt32;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Int64;.
   */
  typedef webfr::Int64 Int64;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::UInt64;.
   */
  typedef webfr::UInt64 UInt64;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Float32;.
   */
  typedef webfr::Float32 Float32;

  /**
   * Convenience typedef for &id:atpp::data::mapping::type::Float64;.
   */
  typedef webfr::Float64 Float64;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Boolean;.
   */
  typedef webfr::Boolean Boolean;

  /*
   * Convenience typedef for std::function<std::shared_ptr<Endpoint::Info>()>.
   */
  typedef std::function<std::shared_ptr<Endpoint::Info>()> EndpointInfoBuilder;

  template <class T>
  using Object = webfr::Object<T>;

  template <class T>
  using List = webfr::List<T>;

  template <class Value>
  using Fields = webfr::Fields<Value>;

  template <class T>
  using Enum = webfr::data::mapping::type::Enum<T>;

protected:
  
  /*
   * Endpoint Coroutine base class
   */
  template<class CoroutineT, class ControllerT>
  class HandlerCoroutine : public webfr::async::CoroutineWithResult<CoroutineT, const std::shared_ptr<OutgoingResponse>&> {
  public:
    
    HandlerCoroutine(ControllerT* pController, const std::shared_ptr<IncomingRequest>& pRequest)
      : controller(pController)
      , request(pRequest)
    {}
    
    const ControllerT* controller;
    std::shared_ptr<IncomingRequest> request;
    
  };
  
  /*
   * Handler which subscribes to specific URL in Router and delegates calls endpoints 
   */
  template<class T>
  class Handler : public RequestHandler {
  public:
    typedef std::shared_ptr<OutgoingResponse> (T::*Method)(const std::shared_ptr<IncomingRequest>&);
    typedef webfr::async::CoroutineStarterForResult<const std::shared_ptr<OutgoingResponse>&>
            (T::*MethodAsync)(const std::shared_ptr<IncomingRequest>&);

  private:

    class ErrorHandlingCoroutine : public webfr::async::CoroutineWithResult<ErrorHandlingCoroutine, const std::shared_ptr<OutgoingResponse>&> {
    private:
      Handler* m_handler;
      std::shared_ptr<IncomingRequest> m_request;
    public:

      ErrorHandlingCoroutine(Handler* handler, const std::shared_ptr<IncomingRequest>& request)
        : m_handler(handler)
        , m_request(request)
      {}

      async::Action act() override {
        return (m_handler->m_controller->*m_handler->m_methodAsync)(m_request)
          .callbackTo(&ErrorHandlingCoroutine::onResponse);
      }

      async::Action onResponse(const std::shared_ptr<OutgoingResponse>& response) {
        return this->_return(response);
      }

      async::Action handleError(async::Error* error) override {
        auto response = m_handler->m_controller->m_errorHandler->handleError(protocol::http::Status::CODE_500, error->what());
        return this->_return(response);
      }

    };

  private:
    T* m_controller;
    Method m_method;
    MethodAsync m_methodAsync;
  public:
    Handler(T* controller, Method method, MethodAsync methodAsync)
      : m_controller(controller)
      , m_method(method)
      , m_methodAsync(methodAsync)
    {}
  public:
    
    static std::shared_ptr<Handler> createShared(T* controller, Method method, MethodAsync methodAsync){
      return std::make_shared<Handler>(controller, method, methodAsync);
    }
    
    std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) override {

      if(m_method == nullptr) {
        if(m_methodAsync == nullptr) {
          return m_controller->handleError(Status::CODE_500, "[ApiController]: Error. Handler method is nullptr.");
        }
        return m_controller->handleError(Status::CODE_500, "[ApiController]: Error. Non-async call to async endpoint.");
      }

      if(m_controller->m_errorHandler) {

        try {
          return (m_controller->*m_method)(request);
        } catch (webfr::web::protocol::http::HttpError& error) {
          return m_controller->m_errorHandler->handleError(error.getInfo().status, error.getMessage(), error.getHeaders());
        } catch (std::exception& error) {
          return m_controller->m_errorHandler->handleError(protocol::http::Status::CODE_500, error.what());
        } catch (...) {
          return m_controller->m_errorHandler->handleError(protocol::http::Status::CODE_500, "Unknown error");
        }

      }

      return (m_controller->*m_method)(request);

    }
    
    webfr::async::CoroutineStarterForResult<const std::shared_ptr<OutgoingResponse>&>
    handleAsync(const std::shared_ptr<protocol::http::incoming::Request>& request) override {

      if(m_methodAsync == nullptr) {
        if(m_method == nullptr) {
          throw webfr::web::protocol::http::HttpError(Status::CODE_500, "[ApiController]: Error. Handler method is nullptr.");
        }
        throw webfr::web::protocol::http::HttpError(Status::CODE_500, "[ApiController]: Error. Async call to non-async endpoint.");
      }

      if(m_controller->m_errorHandler) {
        return ErrorHandlingCoroutine::startForResult(this, request);
      }

      return (m_controller->*m_methodAsync)(request);

    }

    Method setMethod(Method method) {
      auto prev = m_method;
      m_method = method;
      return prev;
    }

    Method getMethod() {
      return m_method;
    }

    MethodAsync setMethodAsync(MethodAsync methodAsync) {
      auto prev = m_methodAsync;
      m_methodAsync = methodAsync;
      return prev;
    }

    MethodAsync getMethodAsync() {
      return m_methodAsync;
    }
    
  };

protected:

  void setEndpointInfo(const std::string& endpointName, const std::shared_ptr<Endpoint::Info>& info);

  std::shared_ptr<Endpoint::Info> getEndpointInfo(const std::string& endpointName);

  void setEndpointHandler(const std::string& endpointName, const std::shared_ptr<RequestHandler>& handler);

  std::shared_ptr<RequestHandler> getEndpointHandler(const std::string& endpointName);
  
protected:
  std::shared_ptr<Endpoints> m_endpoints;
  std::shared_ptr<handler::ErrorHandler> m_errorHandler;
  std::shared_ptr<handler::AuthorizationHandler> m_defaultAuthorizationHandler;
  std::shared_ptr<webfr::data::mapping::ObjectMapper> m_defaultObjectMapper;
  std::unordered_map<std::string, std::shared_ptr<Endpoint::Info>> m_endpointInfo;
  std::unordered_map<std::string, std::shared_ptr<RequestHandler>> m_endpointHandlers;
  const webfr::String m_routerPrefix;
public:
  ApiController(const std::shared_ptr<webfr::data::mapping::ObjectMapper>& defaultObjectMapper, const webfr::String &routerPrefix = nullptr)
    : m_endpoints(Endpoints::createShared())
    , m_errorHandler(nullptr)
    , m_defaultObjectMapper(defaultObjectMapper)
    , m_routerPrefix(routerPrefix)
  {}
public:
  
  template<class T>
  static std::shared_ptr<Endpoint> createEndpoint(const std::shared_ptr<Endpoints>& endpoints,
                                                  const std::shared_ptr<Handler<T>>& handler,
                                                  const EndpointInfoBuilder& infoBuilder)
  {
    auto endpoint = Endpoint::createShared(handler, infoBuilder);
    endpoints->pushBack(endpoint);
    return endpoint;
  }

  /**
   * Subscribes all created endpoint-handlers to corresponding URLs in Router
   */
  void addEndpointsToRouter(const std::shared_ptr<Router>& router);
  
  /**
   * Get list of Endpoints created via ENDPOINT macro
   */
  std::shared_ptr<Endpoints> getEndpoints();

  /**
   * [under discussion]
   * Set error handler to handle calls to handleError
   */
  void setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler);

  /**
   * [under discussion]
   * Do not use it directly. This method is under discussion.
   * Currently returns Response created by registered ErrorHandler or returns Response created by DefaultErrorHandler::handleDefaultError
   * Notice: Does not throw the Error anymore, error-response has to be returned by the caller!
   */
  std::shared_ptr<OutgoingResponse> handleError(const Status& status, const webfr::String& message) const;

  /**
   * [under discussion]
   * Set authorization handler to handle calls to handleAuthorization.
   * Must be called before controller is added to a router or swagger-doc if an endpoint uses the AUTHORIZATION macro
   */
  void setDefaultAuthorizationHandler(const std::shared_ptr<handler::AuthorizationHandler>& authorizationHandler);

  /**
   * Get authorization handler.
   * @return
   */
  std::shared_ptr<handler::AuthorizationHandler> getDefaultAuthorizationHandler();

  /**
   * [under discussion]
   * Do not use it directly. This method is under discussion.
   * Currently returns AuthorizationObject created by AuthorizationHandler or return DefaultAuthorizationObject by DefaultAuthorizationHandler if AuthorizationHandler is null
   */
  std::shared_ptr<handler::AuthorizationObject> handleDefaultAuthorization(const String &authHeader) const;
  
  const std::shared_ptr<webfr::data::mapping::ObjectMapper>& getDefaultObjectMapper() const;
  
  
  std::shared_ptr<OutgoingResponse> createResponse(const Status& status,
                                                   const webfr::String& str) const;
  
  std::shared_ptr<OutgoingResponse> createDtoResponse(const Status& status,
                                                      const webfr::Void& dto,
                                                      const std::shared_ptr<webfr::data::mapping::ObjectMapper>& objectMapper) const;
  
  std::shared_ptr<OutgoingResponse> createDtoResponse(const Status& status,
                                                      const webfr::Void& dto) const;

public:

  template<typename T>
  struct TypeInterpretation {

    static T fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
      (void) text;
      success = false;
      WEBFR_LOGE("[webfr::web::server::api::ApiController::TypeInterpretation::fromString()]",
                 "Error. No conversion from '%s' to '%s' is defined.", "webfr::String", typeName->getData());
      throw std::runtime_error("[webfr::web::server::api::ApiController::TypeInterpretation::fromString()]: Error. "
                               "No conversion from 'webfr::String' to '" + typeName->std_str() + "' is defined. "
                               "Please define type conversion.");
    }

  };

};

template<>
struct ApiController::TypeInterpretation <webfr::String> {
  static webfr::String fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    success = true;
    return text;
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::Int8> {
  static webfr::Int8 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;

    return static_cast<Int8::UnderlyingType>(utils::conversion::strToInt32(text, success));
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::UInt8> {
  static webfr::UInt8 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return static_cast<UInt8::UnderlyingType>(utils::conversion::strToUInt32(text, success));
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::Int16> {
  static webfr::Int16 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return static_cast<Int16::UnderlyingType>(utils::conversion::strToInt32(text, success));
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::UInt16> {
  static webfr::UInt16 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return static_cast<UInt16::UnderlyingType>(utils::conversion::strToUInt32(text, success));
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::Int32> {
  static webfr::Int32 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return utils::conversion::strToInt32(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::UInt32> {
  static webfr::UInt32 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return utils::conversion::strToUInt32(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::Int64> {
  static webfr::Int64 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return utils::conversion::strToInt64(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::UInt64> {
  static webfr::UInt64 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return utils::conversion::strToUInt64(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::Float32> {
  static webfr::Float32 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return utils::conversion::strToFloat32(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::Float64> {
  static webfr::Float64 fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return utils::conversion::strToFloat64(text, success);
  }
};

template<>
struct ApiController::TypeInterpretation <webfr::Boolean> {
  static webfr::Boolean fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    (void) typeName;
    return utils::conversion::strToBool(text, success);
  }
};

template<class T, class I>
struct ApiController::TypeInterpretation <data::mapping::type::EnumObjectWrapper<T, I>> {

  typedef data::mapping::type::EnumObjectWrapper<T, I> EnumOW;
  typedef typename I::UnderlyingTypeObjectWrapper UTOW;

  static EnumOW fromString(const webfr::String& typeName, const webfr::String& text, bool& success) {
    const auto& parsedValue = ApiController::TypeInterpretation<UTOW>::fromString(typeName, text, success);
    if(success) {
      data::mapping::type::EnumInterpreterError error = data::mapping::type::EnumInterpreterError::OK;
      const auto& result = I::fromInterpretation(parsedValue, error);
      if(error == data::mapping::type::EnumInterpreterError::OK) {
        return result.template staticCast<EnumOW>();
      }
      success = false;
    }
    return nullptr;
  }

};

}}}}

#endif 