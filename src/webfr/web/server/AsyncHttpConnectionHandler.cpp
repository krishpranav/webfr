#include "./AsyncHttpConnectionHandler.hpp"

namespace webfr { namespace web { namespace server {

AsyncHttpConnectionHandler::AsyncHttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components,
                                                       v_int32 threadCount)
  : m_executor(std::make_shared<webfr::async::Executor>(threadCount))
  , m_components(components)
{
  m_executor->detach();
}

AsyncHttpConnectionHandler::AsyncHttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components,
                                                       const std::shared_ptr<webfr::async::Executor>& executor)
  : m_executor(executor)
  , m_components(components)
{}

std::shared_ptr<AsyncHttpConnectionHandler> AsyncHttpConnectionHandler::createShared(const std::shared_ptr<HttpRouter>& router, v_int32 threadCount){
  return std::make_shared<AsyncHttpConnectionHandler>(router, threadCount);
}

std::shared_ptr<AsyncHttpConnectionHandler> AsyncHttpConnectionHandler::createShared(const std::shared_ptr<HttpRouter>& router, const std::shared_ptr<webfr::async::Executor>& executor){
  return std::make_shared<AsyncHttpConnectionHandler>(router, executor);
}

void AsyncHttpConnectionHandler::setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler){
  m_components->errorHandler = errorHandler;
  if(!m_components->errorHandler) {
    m_components->errorHandler = handler::DefaultErrorHandler::createShared();
  }
}

void AsyncHttpConnectionHandler::addRequestInterceptor(const std::shared_ptr<interceptor::RequestInterceptor>& interceptor) {
  m_components->requestInterceptors.push_back(interceptor);
}

void AsyncHttpConnectionHandler::addResponseInterceptor(const std::shared_ptr<interceptor::ResponseInterceptor>& interceptor) {
  m_components->responseInterceptors.push_back(interceptor);
}

void AsyncHttpConnectionHandler::handleConnection(const std::shared_ptr<IOStream>& connection,
                                                  const std::shared_ptr<const ParameterMap>& params)
{

  (void)params;

  connection->setOutputStreamIOMode(webfr::data::stream::IOMode::ASYNCHRONOUS);
  connection->setInputStreamIOMode(webfr::data::stream::IOMode::ASYNCHRONOUS);

  m_executor->execute<HttpProcessor::Coroutine>(m_components, connection);
  
}

void AsyncHttpConnectionHandler::stop() {
  // DO NOTHING
}
  
}}}