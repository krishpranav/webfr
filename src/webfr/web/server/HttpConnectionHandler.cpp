#include "./HttpConnectionHandler.hpp"
#include "webfr/web/protocol/http/incoming/Request.hpp"
#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/core/concurrency/Thread.hpp"
#include "webfr/core/data/buffer/IOBuffer.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"
#include "webfr/core/data/stream/StreamBufferedProxy.hpp"


namespace webfr { namespace web { namespace server {

HttpConnectionHandler::HttpConnectionHandler(const std::shared_ptr<HttpProcessor::Components>& components)
  : m_components(components)
{}

std::shared_ptr<HttpConnectionHandler> HttpConnectionHandler::createShared(const std::shared_ptr<HttpRouter>& router){
  return std::make_shared<HttpConnectionHandler>(router);
}

void HttpConnectionHandler::setErrorHandler(const std::shared_ptr<handler::ErrorHandler>& errorHandler){
  m_components->errorHandler = errorHandler;
  if(!m_components->errorHandler) {
    m_components->errorHandler = handler::DefaultErrorHandler::createShared();
  }
}

void HttpConnectionHandler::addRequestInterceptor(const std::shared_ptr<interceptor::RequestInterceptor>& interceptor) {
  m_components->requestInterceptors.push_back(interceptor);
}

void HttpConnectionHandler::addResponseInterceptor(const std::shared_ptr<interceptor::ResponseInterceptor>& interceptor) {
  m_components->responseInterceptors.push_back(interceptor);
}
  
void HttpConnectionHandler::handleConnection(const std::shared_ptr<webfr::data::stream::IOStream>& connection,
                                             const std::shared_ptr<const ParameterMap>& params)
{

  (void)params;

  connection->setOutputStreamIOMode(webfr::data::stream::IOMode::BLOCKING);
  connection->setInputStreamIOMode(webfr::data::stream::IOMode::BLOCKING);

  /* Create working thread */
  std::thread thread(&HttpProcessor::Task::run, HttpProcessor::Task(m_components, connection));
  
  /* Get hardware concurrency -1 in order to have 1cpu free of workers. */
  v_int32 concurrency = webfr::concurrency::getHardwareConcurrency();
  if(concurrency > 1) {
    concurrency -= 1;
  }
  
  /* Set thread affinity group CPUs [0..cpu_count - 1]. Leave one cpu free of workers */
  webfr::concurrency::setThreadAffinityToCpuRange(thread.native_handle(), 0, concurrency - 1 /* -1 because 0-based index */);
  
  thread.detach();

}

void HttpConnectionHandler::stop() {
  // DO NOTHING
}

}}}