#ifndef webfr_web_server_AsyncHttpConnectionHandler_hpp
#define webfr_web_server_AsyncHttpConnectionHandler_hpp

#include "webfr/web/server/HttpProcessor.hpp"
#include "webfr/network/ConnectionHandler.hpp"
#include "webfr/core/async/Executor.hpp"

namespace webfr { namespace web { namespace server {

class AsyncHttpConnectionHandler : public base::Countable, public network::ConnectionHandler {
private:
  std::shared_ptr<webfr::async::Executor> m_executor;
private:
  std::shared_ptr<HttpProcessor::Components> m_components;
public:

  AsyncHttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components, v_int32 threadCount = webfr::async::Executor::VALUE_SUGGESTED);

  AsyncHttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components, const std::shared_ptr<webfr::async::Executor>& executor);

  AsyncHttpConnectionHandler(const std::shared_ptr<HttpRouter>& router, v_int32 threadCount = webfr::async::Executor::VALUE_SUGGESTED)
    : AsyncHttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router), threadCount)
  {}

  AsyncHttpConnectionHandler(const std::shared_ptr<HttpRouter>& router, const std::shared_ptr<webfr::async::Executor>& executor)
    : AsyncHttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router), executor)
  {}

  AsyncHttpConnectionHandler(const std::shared_ptr<HttpRouter>& router,
                             const std::shared_ptr<HttpProcessor::Config>& config,
                             v_int32 threadCount = webfr::async::Executor::VALUE_SUGGESTED)
    : AsyncHttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router, config), threadCount)
  {}

  AsyncHttpConnectionHandler(const std::shared_ptr<HttpRouter>& router,
                             const std::shared_ptr<HttpProcessor::Config>& config,
                             const std::shared_ptr<webfr::async::Executor>& executor)
    : AsyncHttpConnectionHandler(std::make_shared<HttpProcessor::Components>(router, config), executor)
  {}

public:
  
  static std::shared_ptr<AsyncHttpConnectionHandler> createShared(const std::shared_ptr<HttpRouter>& router,
                                                                  v_int32 threadCount = webfr::async::Executor::VALUE_SUGGESTED);
  
  static std::shared_ptr<AsyncHttpConnectionHandler> createShared(const std::shared_ptr<HttpRouter>& router,
                                                                  const std::shared_ptr<webfr::async::Executor>& executor);
  
  void setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler);

  void addRequestInterceptor(const std::shared_ptr<interceptor::RequestInterceptor>& interceptor);

  void addResponseInterceptor(const std::shared_ptr<interceptor::ResponseInterceptor>& interceptor);

  
  void handleConnection(const std::shared_ptr<IOStream>& connection, const std::shared_ptr<const ParameterMap>& params) override;

  void stop() override;
  
};
  
}}}

#endif 
