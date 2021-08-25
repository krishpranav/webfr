#include "./ApiController.hpp"
#include <webfr/web/server/handler/ErrorHandler.hpp>

namespace webfr { namespace web { namespace server { namespace api {
  
void ApiController::addEndpointsToRouter(const std::shared_ptr<Router>& router){
  auto node = m_endpoints->getFirstNode();
  while (node != nullptr) {
    auto endpoint = node->getData();
    router->route(endpoint->info()->method, endpoint->info()->path, endpoint->handler);
    node = node->getNext();
  }
}

std::shared_ptr<ApiController::Endpoints> ApiController::getEndpoints() {
  return m_endpoints;
}

void ApiController::setEndpointInfo(const std::string& endpointName, const std::shared_ptr<Endpoint::Info>& info){
  m_endpointInfo[endpointName] = info;
}

std::shared_ptr<ApiController::Endpoint::Info> ApiController::getEndpointInfo(const std::string& endpointName) {
  return m_endpointInfo[endpointName];
}

void ApiController::setEndpointHandler(const std::string& endpointName, const std::shared_ptr<RequestHandler>& handler) {
  m_endpointHandlers[endpointName] = handler;
}

std::shared_ptr<ApiController::RequestHandler> ApiController::getEndpointHandler(const std::string& endpointName) {
  return m_endpointHandlers[endpointName];
}

void ApiController::setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler){
  m_errorHandler = errorHandler;
}

std::shared_ptr<ApiController::OutgoingResponse> ApiController::handleError(const Status& status, const webfr::String& message) const {
  if(m_errorHandler) {
    return m_errorHandler->handleError(status, message);
  }
  throw webfr::web::protocol::http::HttpError(status, message);
}

void ApiController::setDefaultAuthorizationHandler(const std::shared_ptr<handler::AuthorizationHandler>& authorizationHandler){
  m_defaultAuthorizationHandler = authorizationHandler;
}

std::shared_ptr<handler::AuthorizationHandler> ApiController::getDefaultAuthorizationHandler() {
  return m_defaultAuthorizationHandler;
}

std::shared_ptr<handler::AuthorizationObject> ApiController::handleDefaultAuthorization(const String &authHeader) const {
  if(m_defaultAuthorizationHandler) {
    return m_defaultAuthorizationHandler->handleAuthorization(authHeader);
  }

  throw webfr::web::protocol::http::HttpError(Status::CODE_500, "Authorization is not setup.");
}

const std::shared_ptr<webfr::data::mapping::ObjectMapper>& ApiController::getDefaultObjectMapper() const {
  return m_defaultObjectMapper;
}

std::shared_ptr<ApiController::OutgoingResponse> ApiController::createResponse(const Status& status,
                                                                               const webfr::String& str) const {
  return ResponseFactory::createResponse(status, str);
}

std::shared_ptr<ApiController::OutgoingResponse> ApiController::createDtoResponse(const Status& status,
                                                                                  const webfr::Void& dto,
                                                                                  const std::shared_ptr<webfr::data::mapping::ObjectMapper>& objectMapper) const {
  return ResponseFactory::createResponse(status, dto, objectMapper);
}

std::shared_ptr<ApiController::OutgoingResponse> ApiController::createDtoResponse(const Status& status,
                                                                                  const webfr::Void& dto) const {
  return ResponseFactory::createResponse(status, dto, m_defaultObjectMapper);
}

}}}}