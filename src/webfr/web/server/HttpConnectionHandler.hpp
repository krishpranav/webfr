#ifndef webfr_web_server_HttpConnectionHandler_hpp
#define webfr_web_server_HttpConnectionHandler_hpp

#include "webfr/web/server/HttpProcessor.hpp"
#include "webfr/network/ConnectionHandler.hpp"

namespace webfr { namespace web { namespace server {

class HttpConnectionHandler : public base::Countable, public network::ConnectionHandler {
private:
  std::shared_ptr<HttpProcessor::Components> m_components;
public:

  HttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components);

  HttpConnectionHandler(const std::shared_ptr<HttpRouter>& router)
    : HttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router))
  {}

  HttpConnectionHandler(const std::shared_ptr<HttpRouter>& router,
                        const std::shared_ptr<HttpProcessor::Config>& config)
    : HttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router, config))
  {}

public:

  static std::shared_ptr<HttpConnectionHandler> createShared(const std::shared_ptr<HttpRouter>& router);

  void setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler);

  void addRequestInterceptor(const std::shared_ptr<interceptor::RequestInterceptor>& interceptor);

  void addResponseInterceptor(const std::shared_ptr<interceptor::ResponseInterceptor>& interceptor);

  void handleConnection(const std::shared_ptr<IOStream>& connection, const std::shared_ptr<const ParameterMap>& params) override;

  void stop() override;
  
};
  
}}}

#endif 