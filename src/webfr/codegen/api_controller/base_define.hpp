#define WEBFR_MACRO_API_CONTROLLER_PARAM_MACRO(MACRO, INFO, TYPE, PARAM_LIST) MACRO(TYPE, PARAM_LIST)
#define WEBFR_MACRO_API_CONTROLLER_PARAM_INFO(MACRO, INFO, TYPE, PARAM_LIST) INFO(TYPE, PARAM_LIST)
#define WEBFR_MACRO_API_CONTROLLER_PARAM_TYPE(MACRO, INFO, TYPE, PARAM_LIST) const TYPE&
#define WEBFR_MACRO_API_CONTROLLER_PARAM_NAME(MACRO, INFO, TYPE, PARAM_LIST) WEBFR_MACRO_FIRSTARG PARAM_LIST
#define WEBFR_MACRO_API_CONTROLLER_PARAM_TYPE_STR(MACRO, INFO, TYPE, PARAM_LIST) #TYPE
#define WEBFR_MACRO_API_CONTROLLER_PARAM_NAME_STR(MACRO, INFO, TYPE, PARAM_LIST) WEBFR_MACRO_FIRSTARG_STR PARAM_LIST
#define WEBFR_MACRO_API_CONTROLLER_PARAM(MACRO, INFO, TYPE, PARAM_LIST) (MACRO, INFO, TYPE, PARAM_LIST)

#define REQUEST(TYPE, ...) \
WEBFR_MACRO_API_CONTROLLER_PARAM(WEBFR_MACRO_API_CONTROLLER_REQUEST, WEBFR_MACRO_API_CONTROLLER_REQUEST_INFO, TYPE, (__VA_ARGS__))

#define HEADER(TYPE, ...) \
WEBFR_MACRO_API_CONTROLLER_PARAM(WEBFR_MACRO_API_CONTROLLER_HEADER, WEBFR_MACRO_API_CONTROLLER_HEADER_INFO, TYPE, (__VA_ARGS__))

#define PATH(TYPE, ...) \
WEBFR_MACRO_API_CONTROLLER_PARAM(WEBFR_MACRO_API_CONTROLLER_PATH, WEBFR_MACRO_API_CONTROLLER_PATH_INFO, TYPE, (__VA_ARGS__))

#define QUERIES(TYPE, ...) \
WEBFR_MACRO_API_CONTROLLER_PARAM(WEBFR_MACRO_API_CONTROLLER_QUERIES, WEBFR_MACRO_API_CONTROLLER_QUERIES_INFO, TYPE, (__VA_ARGS__))

#define QUERY(TYPE, ...) \
WEBFR_MACRO_API_CONTROLLER_PARAM(WEBFR_MACRO_API_CONTROLLER_QUERY, WEBFR_MACRO_API_CONTROLLER_QUERY_INFO, TYPE, (__VA_ARGS__))

#define BODY_STRING(TYPE, ...) \
WEBFR_MACRO_API_CONTROLLER_PARAM(WEBFR_MACRO_API_CONTROLLER_BODY_STRING, WEBFR_MACRO_API_CONTROLLER_BODY_STRING_INFO, TYPE, (__VA_ARGS__))

#define BODY_DTO(TYPE, ...) \
WEBFR_MACRO_API_CONTROLLER_PARAM(WEBFR_MACRO_API_CONTROLLER_BODY_DTO, WEBFR_MACRO_API_CONTROLLER_BODY_DTO_INFO, TYPE, (__VA_ARGS__))

#define WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(MACRO, TYPE, ...) \
WEBFR_MACRO_EXPAND(WEBFR_MACRO_MACRO_SELECTOR(MACRO, (__VA_ARGS__)) (TYPE, __VA_ARGS__))


#define WEBFR_MACRO_API_CONTROLLER_REQUEST(TYPE, PARAM_LIST) \
const auto& WEBFR_MACRO_FIRSTARG PARAM_LIST = __request;

#define WEBFR_MACRO_API_CONTROLLER_REQUEST_INFO(TYPE, PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_HEADER_1(TYPE, NAME) \
const auto& __param_str_val_##NAME = __request->getHeader(#NAME); \
if(!__param_str_val_##NAME){ \
  return ApiController::handleError(Status::CODE_400, "Missing HEADER parameter '" #NAME "'"); \
} \
bool __param_validation_check_##NAME; \
const auto& NAME = ApiController::TypeInterpretation<TYPE>::fromString(#TYPE, __param_str_val_##NAME, __param_validation_check_##NAME); \
if(!__param_validation_check_##NAME){ \
  return ApiController::handleError(Status::CODE_400, "Invalid HEADER parameter '" #NAME "'. Expected type is '" #TYPE "'"); \
}

#define WEBFR_MACRO_API_CONTROLLER_HEADER_2(TYPE, NAME, QUALIFIER) \
const auto& __param_str_val_##NAME = __request->getHeader(QUALIFIER); \
if(!__param_str_val_##NAME){ \
  return ApiController::handleError(Status::CODE_400, \
  webfr::String("Missing HEADER parameter '") + QUALIFIER + "'"); \
} \
bool __param_validation_check_##NAME; \
const auto& NAME = ApiController::TypeInterpretation<TYPE>::fromString(#TYPE, __param_str_val_##NAME, __param_validation_check_##NAME); \
if(!__param_validation_check_##NAME){ \
  return ApiController::handleError(Status::CODE_400, \
                                    webfr::String("Invalid HEADER parameter '") + \
                                    QUALIFIER + \
                                    "'. Expected type is '" #TYPE "'"); \
}

#define WEBFR_MACRO_API_CONTROLLER_HEADER(TYPE, PARAM_LIST) \
WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(WEBFR_MACRO_API_CONTROLLER_HEADER_, TYPE, WEBFR_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_HEADER_INFO_1(TYPE, NAME) \
info->headers.add(#NAME, TYPE::Class::getType());

#define WEBFR_MACRO_API_CONTROLLER_HEADER_INFO_2(TYPE, NAME, QUALIFIER) \
info->headers.add(QUALIFIER, TYPE::Class::getType());

#define WEBFR_MACRO_API_CONTROLLER_HEADER_INFO(TYPE, PARAM_LIST) \
WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(WEBFR_MACRO_API_CONTROLLER_HEADER_INFO_, TYPE, WEBFR_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_PATH_1(TYPE, NAME) \
const auto& __param_str_val_##NAME = __request->getPathVariable(#NAME); \
if(!__param_str_val_##NAME){ \
  return ApiController::handleError(Status::CODE_400, "Missing PATH parameter '" #NAME "'"); \
} \
bool __param_validation_check_##NAME; \
const auto& NAME = ApiController::TypeInterpretation<TYPE>::fromString(#TYPE, __param_str_val_##NAME, __param_validation_check_##NAME); \
if(!__param_validation_check_##NAME){ \
  return ApiController::handleError(Status::CODE_400, "Invalid PATH parameter '" #NAME "'. Expected type is '" #TYPE "'"); \
}

#define WEBFR_MACRO_API_CONTROLLER_PATH_2(TYPE, NAME, QUALIFIER) \
const auto& __param_str_val_##NAME = __request->getPathVariable(QUALIFIER); \
if(!__param_str_val_##NAME){ \
  return ApiController::handleError(Status::CODE_400, \
  webfr::String("Missing PATH parameter '") + QUALIFIER + "'"); \
} \
bool __param_validation_check_##NAME; \
const auto NAME = ApiController::TypeInterpretation<TYPE>::fromString(#TYPE, __param_str_val_##NAME, __param_validation_check_##NAME); \
if(!__param_validation_check_##NAME){ \
  return ApiController::handleError(Status::CODE_400, \
                                    webfr::String("Invalid PATH parameter '") + \
                                    QUALIFIER + \
                                    "'. Expected type is '" #TYPE "'"); \
}

#define WEBFR_MACRO_API_CONTROLLER_PATH(TYPE, PARAM_LIST) \
WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(WEBFR_MACRO_API_CONTROLLER_PATH_, TYPE, WEBFR_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_PATH_INFO_1(TYPE, NAME) \
info->pathParams.add(#NAME, TYPE::Class::getType());

#define WEBFR_MACRO_API_CONTROLLER_PATH_INFO_2(TYPE, NAME, QUALIFIER) \
info->pathParams.add(QUALIFIER, TYPE::Class::getType());

#define WEBFR_MACRO_API_CONTROLLER_PATH_INFO(TYPE, PARAM_LIST) \
WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(WEBFR_MACRO_API_CONTROLLER_PATH_INFO_, TYPE, WEBFR_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_QUERIES(TYPE, PARAM_LIST) \
const auto& WEBFR_MACRO_FIRSTARG PARAM_LIST = __request->getQueryParameters();

#define WEBFR_MACRO_API_CONTROLLER_QUERIES_INFO(TYPE, PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_QUERY_1(TYPE, NAME) \
const auto& __param_str_val_##NAME = __request->getQueryParameter(#NAME); \
if(!__param_str_val_##NAME){ \
  return ApiController::handleError(Status::CODE_400, "Missing QUERY parameter '" #NAME "'"); \
} \
bool __param_validation_check_##NAME; \
const auto& NAME = ApiController::TypeInterpretation<TYPE>::fromString(#TYPE, __param_str_val_##NAME, __param_validation_check_##NAME); \
if(!__param_validation_check_##NAME){ \
  return ApiController::handleError(Status::CODE_400, "Invalid QUERY parameter '" #NAME "'. Expected type is '" #TYPE "'"); \
}

#define WEBFR_MACRO_API_CONTROLLER_QUERY_2(TYPE, NAME, QUALIFIER) \
const auto& __param_str_val_##NAME = __request->getQueryParameter(QUALIFIER); \
if(!__param_str_val_##NAME){ \
  return ApiController::handleError(Status::CODE_400, \
  webfr::String("Missing QUERY parameter '") + QUALIFIER + "'"); \
} \
bool __param_validation_check_##NAME; \
const auto& NAME = ApiController::TypeInterpretation<TYPE>::fromString(#TYPE, __param_str_val_##NAME, __param_validation_check_##NAME); \
if(!__param_validation_check_##NAME){ \
  return ApiController::handleError(Status::CODE_400, \
                                    webfr::String("Invalid QUERY parameter '") + \
                                    QUALIFIER + \
                                    "'. Expected type is '" #TYPE "'"); \
}

#define WEBFR_MACRO_API_CONTROLLER_QUERY_3(TYPE, NAME, QUALIFIER, DEFAULT) \
const auto& __param_str_val_##NAME = __request->getQueryParameter(QUALIFIER, DEFAULT); \
bool __param_validation_check_##NAME; \
const auto& NAME = ApiController::TypeInterpretation<TYPE>::fromString(#TYPE, __param_str_val_##NAME, __param_validation_check_##NAME); \
if(!__param_validation_check_##NAME){ \
  return ApiController::handleError(Status::CODE_400, \
                                    webfr::String("Invalid QUERY parameter '") + \
                                    QUALIFIER + \
                                    "'. Expected type is '" #TYPE "'"); \
}

#define WEBFR_MACRO_API_CONTROLLER_QUERY(TYPE, PARAM_LIST) \
WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(WEBFR_MACRO_API_CONTROLLER_QUERY_, TYPE, WEBFR_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_QUERY_INFO_1(TYPE, NAME) \
info->queryParams.add(#NAME, TYPE::Class::getType());

#define WEBFR_MACRO_API_CONTROLLER_QUERY_INFO_2(TYPE, NAME, QUALIFIER) \
info->queryParams.add(QUALIFIER, TYPE::Class::getType());

#define WEBFR_MACRO_API_CONTROLLER_QUERY_INFO_3(TYPE, NAME, QUALIFIER, DEFAULT) \
info->queryParams.add(QUALIFIER, TYPE::Class::getType());

#define WEBFR_MACRO_API_CONTROLLER_QUERY_INFO(TYPE, PARAM_LIST) \
WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(WEBFR_MACRO_API_CONTROLLER_QUERY_INFO_, TYPE, WEBFR_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_BODY_STRING(TYPE, PARAM_LIST) \
const auto& WEBFR_MACRO_FIRSTARG PARAM_LIST = __request->readBodyToString();

// __INFO

#define WEBFR_MACRO_API_CONTROLLER_BODY_STRING_INFO(TYPE, PARAM_LIST) \
info->body.name = WEBFR_MACRO_FIRSTARG_STR PARAM_LIST; \
info->body.required = true; \
info->body.type = webfr::data::mapping::type::__class::String::getType(); \
if(getDefaultObjectMapper()) { \
  info->bodyContentType = getDefaultObjectMapper()->getInfo().http_content_type; \
}

// BODY_DTO MACRO // ------------------------------------------------------

#define WEBFR_MACRO_API_CONTROLLER_BODY_DTO(TYPE, PARAM_LIST) \
if(!getDefaultObjectMapper()) { \
  return ApiController::handleError(Status::CODE_500, "ObjectMapper was NOT set. Can't deserialize the request body."); \
} \
const auto& WEBFR_MACRO_FIRSTARG PARAM_LIST = \
__request->readBodyToDto<TYPE>(getDefaultObjectMapper().get()); \
if(!WEBFR_MACRO_FIRSTARG PARAM_LIST) { \
  return ApiController::handleError(Status::CODE_400, "Missing valid body parameter '" WEBFR_MACRO_FIRSTARG_STR PARAM_LIST "'"); \
}

// __INFO

#define WEBFR_MACRO_API_CONTROLLER_BODY_DTO_INFO(TYPE, PARAM_LIST) \
info->body.name = WEBFR_MACRO_FIRSTARG_STR PARAM_LIST; \
info->body.required = true; \
info->body.type = TYPE::Class::getType(); \
if(getDefaultObjectMapper()) { \
  info->bodyContentType = getDefaultObjectMapper()->getInfo().http_content_type; \
}

// FOR EACH // ------------------------------------------------------

#define WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_DECL_FIRST(INDEX, COUNT, X) \
WEBFR_MACRO_API_CONTROLLER_PARAM_TYPE X WEBFR_MACRO_API_CONTROLLER_PARAM_NAME X

#define WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_DECL_REST(INDEX, COUNT, X) \
, WEBFR_MACRO_API_CONTROLLER_PARAM_TYPE X WEBFR_MACRO_API_CONTROLLER_PARAM_NAME X

#define WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_PUT(INDEX, COUNT, X) \
WEBFR_MACRO_API_CONTROLLER_PARAM_MACRO X

#define WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_CALL_FIRST(INDEX, COUNT, X) \
WEBFR_MACRO_API_CONTROLLER_PARAM_NAME X

#define WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_CALL_REST(INDEX, COUNT, X) \
, WEBFR_MACRO_API_CONTROLLER_PARAM_NAME X

#define WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_INFO(INDEX, COUNT, X) \
WEBFR_MACRO_API_CONTROLLER_PARAM_INFO X

// ENDPOINT_INFO MACRO // ------------------------------------------------------

#define ENDPOINT_INFO(NAME) \
\
std::shared_ptr<Endpoint::Info> Z__ENDPOINT_CREATE_ADDITIONAL_INFO_##NAME() { \
  auto info = Z__EDNPOINT_INFO_GET_INSTANCE_##NAME(); \
  Z__ENDPOINT_ADD_INFO_##NAME(info); \
  return info; \
} \
\
const std::shared_ptr<Endpoint::Info> Z__ENDPOINT_ADDITIONAL_INFO_##NAME = Z__ENDPOINT_CREATE_ADDITIONAL_INFO_##NAME(); \
\
void Z__ENDPOINT_ADD_INFO_##NAME(const std::shared_ptr<Endpoint::Info>& info)

// ENDPOINT MACRO // ------------------------------------------------------

#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_DECL_DEFAULTS(NAME, METHOD, PATH) \
\
template<class T> \
static typename std::shared_ptr<Handler<T>> Z__ENDPOINT_HANDLER_GET_INSTANCE_##NAME(T* controller) { \
  auto handler = std::static_pointer_cast<Handler<T>>(controller->getEndpointHandler(#NAME)); \
  if(!handler) { \
    handler = Handler<T>::createShared(controller, &T::Z__PROXY_METHOD_##NAME, nullptr); \
    controller->setEndpointHandler(#NAME, handler); \
  } \
  return handler; \
} \
\
std::shared_ptr<Endpoint::Info> Z__EDNPOINT_INFO_GET_INSTANCE_##NAME() { \
  std::shared_ptr<Endpoint::Info> info = getEndpointInfo(#NAME); \
  if(!info){ \
    info = Endpoint::Info::createShared(); \
    setEndpointInfo(#NAME, info); \
  } \
  return info; \
}

#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_DECL_0(NAME, METHOD, PATH)  \
\
EndpointInfoBuilder Z__CREATE_ENDPOINT_INFO_##NAME = [this](){ \
  auto info = Z__EDNPOINT_INFO_GET_INSTANCE_##NAME(); \
  info->name = #NAME; \
  info->path = ((m_routerPrefix != nullptr) ? m_routerPrefix + PATH : PATH); \
  info->method = METHOD; \
  if (info->path == "") { \
    info->path = "/"; \
  } \
  return info; \
}; \
\
const std::shared_ptr<Endpoint> Z__ENDPOINT_##NAME = createEndpoint(m_endpoints, \
                                                        Z__ENDPOINT_HANDLER_GET_INSTANCE_##NAME(this), \
                                                        Z__CREATE_ENDPOINT_INFO_##NAME);

#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_0(NAME, METHOD, PATH) \
WEBFR_MACRO_API_CONTROLLER_ENDPOINT_DECL_DEFAULTS(NAME, METHOD, PATH) \
WEBFR_MACRO_API_CONTROLLER_ENDPOINT_DECL_0(NAME, METHOD, PATH) \
\
std::shared_ptr<webfr::web::protocol::http::outgoing::Response> \
Z__PROXY_METHOD_##NAME(const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& __request) \
{ \
  (void)__request; \
  return NAME(); \
} \
\
std::shared_ptr<webfr::web::protocol::http::outgoing::Response> NAME()

////////////////////

#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_DECL_1(NAME, METHOD, PATH, ...)  \
\
EndpointInfoBuilder Z__CREATE_ENDPOINT_INFO_##NAME = [this](){ \
auto info = Z__EDNPOINT_INFO_GET_INSTANCE_##NAME(); \
  info->name = #NAME; \
  info->path = ((m_routerPrefix != nullptr) ? m_routerPrefix + PATH : PATH); \
  info->method = METHOD; \
  WEBFR_MACRO_FOREACH(WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_INFO, __VA_ARGS__) \
  return info; \
}; \
\
const std::shared_ptr<Endpoint> Z__ENDPOINT_##NAME = createEndpoint(m_endpoints, \
                                                        Z__ENDPOINT_HANDLER_GET_INSTANCE_##NAME(this), \
                                                        Z__CREATE_ENDPOINT_INFO_##NAME);

#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_1(NAME, METHOD, PATH, ...) \
WEBFR_MACRO_API_CONTROLLER_ENDPOINT_DECL_DEFAULTS(NAME, METHOD, PATH) \
WEBFR_MACRO_API_CONTROLLER_ENDPOINT_DECL_1(NAME, METHOD, PATH, __VA_ARGS__) \
\
std::shared_ptr<webfr::web::protocol::http::outgoing::Response> \
Z__PROXY_METHOD_##NAME(const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& __request) \
{ \
  WEBFR_MACRO_FOREACH(WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_PUT, __VA_ARGS__) \
  return NAME( \
    WEBFR_MACRO_FOREACH_FIRST_AND_REST( \
      WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_CALL_FIRST, \
      WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_CALL_REST, \
      __VA_ARGS__ \
    ) \
  ); \
} \
\
std::shared_ptr<webfr::web::protocol::http::outgoing::Response> NAME(\
  WEBFR_MACRO_FOREACH_FIRST_AND_REST( \
    WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_DECL_FIRST, \
    WEBFR_MACRO_API_CONTROLLER_FOR_EACH_PARAM_DECL_REST, \
    __VA_ARGS__ \
  ) \
)

// Chooser

#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_MACRO_0(METHOD, PATH, NAME) \
WEBFR_MACRO_EXPAND(WEBFR_MACRO_API_CONTROLLER_ENDPOINT_0(NAME, METHOD, PATH))

#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_MACRO_1(METHOD, PATH, NAME, ...) \
WEBFR_MACRO_EXPAND(WEBFR_MACRO_API_CONTROLLER_ENDPOINT_1(NAME, METHOD, PATH, __VA_ARGS__))

/**
 * Codegen macoro to be used in `webfr::web::server::api::ApiController` to generate Endpoint.
 * @param METHOD - Http method ("GET", "POST", "PUT", etc.).
 * @param PATH - Path to endpoint (without host).
 * @param NAME - Name of the generated method.
 * @return - std::shared_ptr to &id:webfr::web::protocol::http::outgoing::Response;.
 */
#define ENDPOINT(METHOD, PATH, ...) \
WEBFR_MACRO_EXPAND(WEBFR_MACRO_MACRO_BINARY_SELECTOR(WEBFR_MACRO_API_CONTROLLER_ENDPOINT_MACRO_, (__VA_ARGS__)) (METHOD, PATH, __VA_ARGS__))

/**
 * Endpoint interceptor
 */
#define ENDPOINT_INTERCEPTOR(ENDPOINT_NAME, NAME) \
\
Handler<webfr::web::server::api::ApiController>::Method \
  Z__INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME = Z__INTERCEPTOR_METHOD_SET_##ENDPOINT_NAME ##_ ##NAME(this); \
\
template<class T> \
Handler<webfr::web::server::api::ApiController>::Method Z__INTERCEPTOR_METHOD_SET_##ENDPOINT_NAME ##_ ##NAME (T* controller) { \
  return static_cast<Handler<webfr::web::server::api::ApiController>::Method>( \
    Z__ENDPOINT_HANDLER_GET_INSTANCE_##ENDPOINT_NAME(controller)->setMethod(&T::Z__PROXY_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME) \
  ); \
} \
\
std::shared_ptr<webfr::web::protocol::http::outgoing::Response> \
Z__PROXY_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME(const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& request) { \
  return Z__USER_PROXY_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME(this, request); \
} \
\
template<class T> \
std::shared_ptr<webfr::web::protocol::http::outgoing::Response> \
Z__USER_PROXY_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME( \
  T*, \
  const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& request \
) { \
  auto intercepted = static_cast<typename Handler<T>::Method>(Z__INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME); \
  return Z__USER_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME <T> (intercepted, request); \
} \
\
template<class T> \
std::shared_ptr<webfr::web::protocol::http::outgoing::Response> \
Z__USER_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME( \
  typename Handler<T>::Method intercepted, \
  const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& request \
)

// ENDPOINT ASYNC MACRO // ------------------------------------------------------

/*
 *  1 - Method to obtain endpoint call function ptr
 *  2 - Endpoint info singleton
 */
#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_ASYNC_DECL_DEFAULTS(NAME, METHOD, PATH) \
\
template<class T> \
static typename std::shared_ptr<Handler<T>> Z__ENDPOINT_HANDLER_GET_INSTANCE_##NAME(T* controller) { \
  auto handler = std::static_pointer_cast<Handler<T>>(controller->getEndpointHandler(#NAME)); \
  if(!handler) { \
    handler = Handler<T>::createShared(controller, nullptr, &T::Z__PROXY_METHOD_##NAME); \
    controller->setEndpointHandler(#NAME, handler); \
  } \
  return handler; \
} \
\
std::shared_ptr<Endpoint::Info> Z__EDNPOINT_INFO_GET_INSTANCE_##NAME() { \
  std::shared_ptr<Endpoint::Info> info = getEndpointInfo(#NAME); \
  if(!info){ \
    info = Endpoint::Info::createShared(); \
    setEndpointInfo(#NAME, info); \
  } \
  return info; \
}

/*
 *  1 - Endpoint info instance
 *  2 - Endpoint instance
 */
#define WEBFR_MACRO_API_CONTROLLER_ENDPOINT_ASYNC_DECL(NAME, METHOD, PATH)  \
\
EndpointInfoBuilder Z__CREATE_ENDPOINT_INFO_##NAME = [this](){ \
  auto info = Z__EDNPOINT_INFO_GET_INSTANCE_##NAME(); \
  info->name = #NAME; \
  info->path = PATH; \
  info->method = METHOD; \
  return info; \
}; \
\
const std::shared_ptr<Endpoint> Z__ENDPOINT_##NAME = createEndpoint(m_endpoints, \
                                                                    Z__ENDPOINT_HANDLER_GET_INSTANCE_##NAME(this), \
                                                                    Z__CREATE_ENDPOINT_INFO_##NAME);

/**
 * Codegen macoro to be used in `webfr::web::server::api::ApiController` to generate Asynchronous Endpoint.
 * @param METHOD - Http method ("GET", "POST", "PUT", etc.).
 * @param PATH - Path to endpoint (without host).
 * @param NAME - Name of the generated method.
 * @return - &id:webfr::async::Action;.
 */
#define ENDPOINT_ASYNC(METHOD, PATH, NAME) \
WEBFR_MACRO_API_CONTROLLER_ENDPOINT_ASYNC_DECL_DEFAULTS(NAME, METHOD, PATH) \
WEBFR_MACRO_API_CONTROLLER_ENDPOINT_ASYNC_DECL(NAME, METHOD, PATH) \
\
webfr::async::CoroutineStarterForResult<const std::shared_ptr<webfr::web::protocol::http::outgoing::Response>&> \
Z__PROXY_METHOD_##NAME(const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& __request) \
{ \
  return NAME::startForResult(this, __request); \
} \
\
class NAME : public HandlerCoroutine<NAME, __ControllerType>

/**
 * Auxiliary codegen macro for `ENDPOINT_ASYNC` to generate correct constructor for Asynchronous Endpoint Coroutine.
 * @NAME - Name of the endpoint. Exact the same name as was passed to `ENDPOINT_ASYNC` macro.
 */
#define ENDPOINT_ASYNC_INIT(NAME) \
public: \
\
  NAME(__ControllerType* pController, \
       const std::shared_ptr<IncomingRequest>& pRequest) \
    : HandlerCoroutine(pController, pRequest) \
  {}


/**
 * Endpoint interceptor
 */
#define ENDPOINT_INTERCEPTOR_ASYNC(ENDPOINT_NAME, NAME) \
\
Handler<webfr::web::server::api::ApiController>::MethodAsync \
  Z__INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME = Z__INTERCEPTOR_METHOD_SET_##ENDPOINT_NAME ##_ ##NAME(this); \
\
template<class T> \
Handler<webfr::web::server::api::ApiController>::MethodAsync Z__INTERCEPTOR_METHOD_SET_##ENDPOINT_NAME ##_ ##NAME (T* controller) { \
  return static_cast<Handler<webfr::web::server::api::ApiController>::MethodAsync>( \
    Z__ENDPOINT_HANDLER_GET_INSTANCE_##ENDPOINT_NAME(controller)->setMethodAsync(&T::Z__PROXY_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME) \
  ); \
} \
\
webfr::async::CoroutineStarterForResult<const std::shared_ptr<webfr::web::protocol::http::outgoing::Response>&> \
Z__PROXY_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME(const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& request) { \
  return Z__USER_PROXY_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME(this, request); \
} \
\
template<class T> \
webfr::async::CoroutineStarterForResult<const std::shared_ptr<webfr::web::protocol::http::outgoing::Response>&> \
Z__USER_PROXY_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME( \
  T*, \
  const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& request \
) { \
  auto intercepted = static_cast<typename Handler<T>::MethodAsync>(Z__INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME); \
  return Z__USER_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME <T> (intercepted, request); \
} \
\
template<class T> \
webfr::async::CoroutineStarterForResult<const std::shared_ptr<webfr::web::protocol::http::outgoing::Response>&> \
Z__USER_INTERCEPTOR_METHOD_##ENDPOINT_NAME ##_ ##NAME( \
  typename Handler<T>::MethodAsync intercepted, \
  const std::shared_ptr<webfr::web::protocol::http::incoming::Request>& request \
)