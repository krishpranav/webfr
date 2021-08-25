#define AUTHORIZATION(TYPE, ...) \
WEBFR_MACRO_API_CONTROLLER_PARAM(WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION, WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION_INFO, TYPE, (__VA_ARGS__))

#define WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION_1(TYPE, NAME) \
auto __param_str_val_##NAME = __request->getHeader(webfr::web::protocol::http::Header::AUTHORIZATION); \
std::shared_ptr<webfr::web::server::handler::AuthorizationObject> __param_aosp_val_##NAME = ApiController::handleDefaultAuthorization(__param_str_val_##NAME); \
TYPE NAME = std::static_pointer_cast<TYPE::element_type>(__param_aosp_val_##NAME);

#define WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION_2(TYPE, NAME, AUTH_HANDLER) \
auto __param_str_val_##NAME = __request->getHeader(webfr::web::protocol::http::Header::AUTHORIZATION); \
std::shared_ptr<webfr::web::server::handler::AuthorizationHandler> __auth_handler_##NAME = AUTH_HANDLER; \
std::shared_ptr<webfr::web::server::handler::AuthorizationObject> __param_aosp_val_##NAME = __auth_handler_##NAME->handleAuthorization(__param_str_val_##NAME); \
TYPE NAME = std::static_pointer_cast<TYPE::element_type>(__param_aosp_val_##NAME);

#define WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION(TYPE, PARAM_LIST) \
WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION_, TYPE, WEBFR_MACRO_UNFOLD_VA_ARGS PARAM_LIST)

#define WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION_INFO_1(TYPE, NAME) \
auto __param_obj_##NAME = ApiController::getDefaultAuthorizationHandler(); \
if(__param_obj_##NAME) { \
  info->headers.add(webfr::web::protocol::http::Header::AUTHORIZATION, webfr::String::Class::getType()); \
  info->headers[webfr::web::protocol::http::Header::AUTHORIZATION].description = __param_obj_##NAME ->getScheme(); \
  info->authorization = __param_obj_##NAME ->getScheme(); \
} else { \
  throw webfr::web::protocol::http::HttpError(Status::CODE_500, "No authorization handler set up in controller before controller was added to router or swagger-doc."); \
}

#define WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION_INFO_2(TYPE, NAME, AUTH_HANDLER) \
std::shared_ptr<webfr::web::server::handler::AuthorizationHandler> __auth_handler_##NAME = AUTH_HANDLER; \
if(__auth_handler_##NAME) { \
  info->headers.add(webfr::web::protocol::http::Header::AUTHORIZATION, webfr::String::Class::getType()); \
  info->headers[webfr::web::protocol::http::Header::AUTHORIZATION].description = __auth_handler_##NAME->getScheme(); \
  info->authorization = __auth_handler_##NAME->getScheme(); \
} else { \
  throw webfr::web::protocol::http::HttpError(Status::CODE_500, "Invalid authorization handler given (or not set up) in AUTHORIZATION(TYPE, NAME, AUTH_HANDLER) before controller was added to router or swagger-doc."); \
}

#define WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION_INFO(TYPE, PARAM_LIST) \
WEBFR_MACRO_API_CONTROLLER_MACRO_SELECTOR(WEBFR_MACRO_API_CONTROLLER_AUTHORIZATION_INFO_, TYPE, WEBFR_MACRO_UNFOLD_VA_ARGS PARAM_LIST)