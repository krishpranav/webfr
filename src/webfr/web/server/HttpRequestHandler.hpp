#ifndef webfr_web_server_HttpRequestHandler_hpp
#define webfr_web_server_HttpRequestHandler_hpp

#include "webfr/web/protocol/http/outgoing/ResponseFactory.hpp"
#include "webfr/web/protocol/http/outgoing/Response.hpp"
#include "webfr/web/protocol/http/incoming/Request.hpp"

namespace webfr { namespace web { namespace server {

class HttpRequestHandler {
public:

  typedef webfr::web::protocol::http::Status Status;

  typedef webfr::web::protocol::http::Header Header;

  typedef webfr::web::protocol::http::Headers Headers;

  typedef webfr::web::protocol::http::QueryParams QueryParams;

  typedef webfr::web::protocol::http::incoming::Request IncomingRequest;

  typedef webfr::web::protocol::http::outgoing::Response OutgoingResponse;

  typedef webfr::web::protocol::http::outgoing::ResponseFactory ResponseFactory;

  typedef webfr::web::protocol::http::HttpError HttpError;

public:

  virtual std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest>& request) {
    (void)request;
    throw HttpError(Status::CODE_501, "Endpoint not implemented.");
  }

  virtual webfr::async::CoroutineStarterForResult<const std::shared_ptr<OutgoingResponse>&>
  handleAsync(const std::shared_ptr<IncomingRequest>& request) {
    (void)request;
    throw HttpError(Status::CODE_501, "Asynchronous endpoint not implemented.");
  }

  virtual ~HttpRequestHandler() = default;
};

}}}

#endif