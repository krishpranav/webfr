#include "./ErrorHandler.hpp"
#include "webfr/web/protocol/http/outgoing/BufferBody.hpp"

namespace webfr { namespace web { namespace server { namespace handler {

std::shared_ptr<protocol::http::outgoing::Response> ErrorHandler::handleError(const protocol::http::Status& status, const webfr::String& message) {
  Headers headers;
  return handleError(status, message, headers);
}

std::shared_ptr<protocol::http::outgoing::Response>
DefaultErrorHandler::handleError(const webfr::web::protocol::http::Status &status, const webfr::String &message, const Headers& headers){

  data::stream::BufferOutputStream stream;
  stream << "server=" << protocol::http::Header::Value::SERVER << "\n";
  stream << "code=" << status.code << "\n";
  stream << "description=" << status.description << "\n";
  stream << "message=" << message << "\n";
  auto response = protocol::http::outgoing::Response::createShared
      (status, protocol::http::outgoing::BufferBody::createShared(stream.toString()));

  response->putHeader(protocol::http::Header::SERVER, protocol::http::Header::Value::SERVER);
  response->putHeader(protocol::http::Header::CONNECTION, protocol::http::Header::Value::CONNECTION_CLOSE);

  for(const auto& pair : headers.getAll()) {
    response->putHeader_Unsafe(pair.first, pair.second);
  }

  return response;

}

}}}}