#ifndef webfr_web_server_interceptor_RequestInterceptor_hpp
#define webfr_web_server_interceptor_RequestInterceptor_hpp

#include "webfr/web/protocol/http/outgoing/Response.hpp"
#include "webfr/web/protocol/http/incoming/Request.hpp"
#include "webfr/web/protocol/http/Http.hpp"

namespace webfr { namespace web { namespace server { namespace interceptor {

class RequestInterceptor {
public:

  typedef webfr::web::protocol::http::incoming::Request IncomingRequest;

  typedef webfr::web::protocol::http::outgoing::Response OutgoingResponse;
public:
  
  virtual ~RequestInterceptor() = default;
  
  virtual std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request) = 0;
  
};
  
}}}}

#endif