#ifndef webfr_web_server_interceptor_AllowCorsGlobal_hpp
#define webfr_web_server_interceptor_AllowCorsGlobal_hpp

#include "webfr/web/server/interceptor/ResponseInterceptor.hpp"
#include "webfr/web/server/interceptor/RequestInterceptor.hpp"

namespace webfr { namespace web { namespace server { namespace interceptor {

class AllowOptionsGlobal : public RequestInterceptor {
public:
  std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request) override;
};

class AllowCorsGlobal : public ResponseInterceptor {
private:
  webfr::String m_origin;
  webfr::String m_methods;
  webfr::String m_headers;
  webfr::String m_maxAge;
public:

  AllowCorsGlobal(const webfr::String &origin = "*",
                  const webfr::String &methods = "GET, POST, OPTIONS",
                  const webfr::String &headers = "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization",
                  const webfr::String &maxAge = "1728000");

  std::shared_ptr<OutgoingResponse> intercept(const std::shared_ptr<IncomingRequest>& request,
                                              const std::shared_ptr<OutgoingResponse>& response) override;

};

}}}}

#endif 