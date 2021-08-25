#include "./ResponseFactory.hpp"
#include "./BufferBody.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {
  
std::shared_ptr<Response>
ResponseFactory::createResponse(const Status& status, const webfr::String& text) {
  return Response::createShared(status, BufferBody::createShared(text));
}

std::shared_ptr<Response>
ResponseFactory::createResponse(const Status& status,
                                const webfr::Void& dto,
                                const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper) {
  return Response::createShared(status, BufferBody::createShared(
    objectMapper->writeToString(dto),
    objectMapper->getInfo().http_content_type
  ));
}

  
}}}}}