#include "HttpRequestExecutor.hpp"
#include "webfr/web/protocol/http/incoming/ResponseHeadersReader.hpp"
#include "webfr/web/protocol/http/outgoing/Request.hpp"
#include "webfr/network/tcp/Connection.hpp"
#include "webfr/core/data/stream/StreamBufferedProxy.hpp"

#if defined(WIN32) || defined(_WIN32)
#include <io.h>
#endif

namespace webfr { namespace web { namespace client {

HttpRequestExecutor::ConnectionProxy::ConnectionProxy(const std::shared_ptr<ClientConnectionProvider>& connectionProvider,
                                                      const std::shared_ptr<data::stream::IOStream>& connection)
  : m_connectionProvider(connectionProvider)
  , m_connection(connection)
  , m_valid(true)
  , m_invalidateOnDestroy(false)
{}

HttpRequestExecutor::ConnectionProxy::~ConnectionProxy() {
  if(m_invalidateOnDestroy) {
    invalidate();
  }
}

v_io_size HttpRequestExecutor::ConnectionProxy::read(void *buffer, v_buff_size count, async::Action& action) {
  return m_connection->read(buffer, count, action);
}

v_io_size HttpRequestExecutor::ConnectionProxy::write(const void *data, v_buff_size count, async::Action& action) {
  return m_connection->write(data,count, action);
}

void HttpRequestExecutor::ConnectionProxy::setInputStreamIOMode(data::stream::IOMode ioMode) {
  m_connection->setInputStreamIOMode(ioMode);
}

data::stream::IOMode HttpRequestExecutor::ConnectionProxy::getInputStreamIOMode() {
  return m_connection->getInputStreamIOMode();
}

data::stream::Context& HttpRequestExecutor::ConnectionProxy::getInputStreamContext() {
  return m_connection->getInputStreamContext();
}

void HttpRequestExecutor::ConnectionProxy::setOutputStreamIOMode(data::stream::IOMode ioMode) {
  return m_connection->setOutputStreamIOMode(ioMode);
}

data::stream::IOMode HttpRequestExecutor::ConnectionProxy::getOutputStreamIOMode() {
  return m_connection->getOutputStreamIOMode();
}

data::stream::Context& HttpRequestExecutor::ConnectionProxy::getOutputStreamContext() {
  return m_connection->getOutputStreamContext();
}

void HttpRequestExecutor::ConnectionProxy::invalidate() {
  if(m_valid) {
    m_connectionProvider->invalidate(m_connection);
    m_valid = false;
  }
}

void HttpRequestExecutor::ConnectionProxy::setInvalidateOnDestroy(bool invalidateOnDestroy) {
  m_invalidateOnDestroy = invalidateOnDestroy;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HttpRequestExecutor::HttpConnectionHandle

HttpRequestExecutor::HttpConnectionHandle::HttpConnectionHandle(const std::shared_ptr<ConnectionProxy>& connectionProxy)
  : m_connectionProxy(connectionProxy)
{}


std::shared_ptr<HttpRequestExecutor::ConnectionProxy> HttpRequestExecutor::HttpConnectionHandle::getConnection() {
  return m_connectionProxy;
}

void HttpRequestExecutor::HttpConnectionHandle::invalidate() {
  m_connectionProxy->invalidate();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HttpRequestExecutor

HttpRequestExecutor::HttpRequestExecutor(const std::shared_ptr<ClientConnectionProvider>& connectionProvider,
                                         const std::shared_ptr<RetryPolicy>& retryPolicy,
                                         const std::shared_ptr<const BodyDecoder>& bodyDecoder)
  : RequestExecutor(retryPolicy)
  , m_connectionProvider(connectionProvider)
  , m_bodyDecoder(bodyDecoder)
{}

std::shared_ptr<HttpRequestExecutor>
HttpRequestExecutor::createShared(const std::shared_ptr<ClientConnectionProvider>& connectionProvider,
                                  const std::shared_ptr<RetryPolicy>& retryPolicy,
                                  const std::shared_ptr<const BodyDecoder>& bodyDecoder)
{
  return std::make_shared<HttpRequestExecutor>(connectionProvider, retryPolicy, bodyDecoder);
}

std::shared_ptr<HttpRequestExecutor::ConnectionHandle> HttpRequestExecutor::getConnection() {
  auto connection = m_connectionProvider->get();
  if(!connection){
    throw RequestExecutionError(RequestExecutionError::ERROR_CODE_CANT_CONNECT,
                                "[webfr::web::client::HttpRequestExecutor::getConnection()]: ConnectionProvider failed to provide Connection");
  }
  auto connectionProxy = std::make_shared<ConnectionProxy>(m_connectionProvider, connection);
  return std::make_shared<HttpConnectionHandle>(connectionProxy);
}

webfr::async::CoroutineStarterForResult<const std::shared_ptr<HttpRequestExecutor::ConnectionHandle>&>
HttpRequestExecutor::getConnectionAsync() {
  
  class GetConnectionCoroutine : public webfr::async::CoroutineWithResult<GetConnectionCoroutine, const std::shared_ptr<ConnectionHandle>&> {
  private:
    std::shared_ptr<ClientConnectionProvider> m_connectionProvider;
  public:
    
    GetConnectionCoroutine(const std::shared_ptr<ClientConnectionProvider>& connectionProvider)
      : m_connectionProvider(connectionProvider)
    {}
    
    Action act() override {
      return m_connectionProvider->getAsync().callbackTo(&GetConnectionCoroutine::onConnectionReady);
    }
    
    Action onConnectionReady(const std::shared_ptr<webfr::data::stream::IOStream>& connection) {
      auto connectionProxy = std::make_shared<ConnectionProxy>(m_connectionProvider, connection);
      return _return(std::make_shared<HttpConnectionHandle>(connectionProxy));
    }
    
  };
  
  return GetConnectionCoroutine::startForResult(m_connectionProvider);
  
}

void HttpRequestExecutor::invalidateConnection(const std::shared_ptr<ConnectionHandle>& connectionHandle) {

  if(connectionHandle) {
    auto handle = static_cast<HttpConnectionHandle*>(connectionHandle.get());
    handle->invalidate();
  }

}
  
std::shared_ptr<HttpRequestExecutor::Response>
HttpRequestExecutor::executeOnce(const String& method,
                                 const String& path,
                                 const Headers& headers,
                                 const std::shared_ptr<Body>& body,
                                 const std::shared_ptr<ConnectionHandle>& connectionHandle) {
  
  std::shared_ptr<ConnectionProxy> connection;
  std::shared_ptr<HttpConnectionHandle> httpCH = std::static_pointer_cast<HttpConnectionHandle>(connectionHandle);
  if(httpCH) {
    connection = httpCH->getConnection();
  }

  if(!connection){
    throw RequestExecutionError(RequestExecutionError::ERROR_CODE_CANT_CONNECT,
                                "[webfr::web::client::HttpRequestExecutor::executeOnce()]: Connection is null");
  }

  connection->setInputStreamIOMode(data::stream::IOMode::BLOCKING);
  connection->setOutputStreamIOMode(data::stream::IOMode::BLOCKING);
  
  auto request = webfr::web::protocol::http::outgoing::Request::createShared(method, path, headers, body);
  request->putHeaderIfNotExists_Unsafe(webfr::web::protocol::http::Header::HOST, m_connectionProvider->getProperty("host"));
  request->putHeaderIfNotExists_Unsafe(webfr::web::protocol::http::Header::CONNECTION, webfr::web::protocol::http::Header::Value::CONNECTION_KEEP_ALIVE);

  webfr::data::share::MemoryLabel buffer(webfr::base::StrBuffer::createShared(webfr::data::buffer::IOBuffer::BUFFER_SIZE));

  webfr::data::stream::OutputStreamBufferedProxy upStream(connection, buffer);
  request->send(&upStream);
  upStream.flush();
  
  webfr::web::protocol::http::incoming::ResponseHeadersReader headerReader(buffer, 4096);
  webfr::web::protocol::http::HttpError::Info error;
  const auto& result = headerReader.readHeaders(connection, error);
  
  if(error.status.code != 0) {
    connection->invalidate();
    throw RequestExecutionError(RequestExecutionError::ERROR_CODE_CANT_PARSE_STARTING_LINE,
                                "[webfr::web::client::HttpRequestExecutor::executeOnce()]: Failed to parse response. Invalid response headers");
  }
  
  if(error.ioStatus < 0) {
    connection->invalidate();
    throw RequestExecutionError(RequestExecutionError::ERROR_CODE_CANT_PARSE_STARTING_LINE,
                                "[webfr::web::client::HttpRequestExecutor::executeOnce()]: Failed to read response.");
  }
                                                                                
  auto connectionHeader = result.headers.getAsMemoryLabel<webfr::data::share::StringKeyLabelCI>(Header::CONNECTION);
  if (connectionHeader == "close") {
    connection->setInvalidateOnDestroy(true);
  }
  
  auto bodyStream = webfr::data::stream::InputStreamBufferedProxy::createShared(connection,
                                                                                buffer,
                                                                                result.bufferPosStart,
                                                                                result.bufferPosEnd,
                                                                                result.bufferPosStart != result.bufferPosEnd);

  return Response::createShared(result.startingLine.statusCode,
                                result.startingLine.description.toString(),
                                result.headers, bodyStream, m_bodyDecoder);
  
}

webfr::async::CoroutineStarterForResult<const std::shared_ptr<HttpRequestExecutor::Response>&>
HttpRequestExecutor::executeOnceAsync(const String& method,
                                      const String& path,
                                      const Headers& headers,
                                      const std::shared_ptr<Body>& body,
                                      const std::shared_ptr<ConnectionHandle>& connectionHandle) {
  
  typedef protocol::http::incoming::ResponseHeadersReader ResponseHeadersReader;
  
  class ExecutorCoroutine : public webfr::async::CoroutineWithResult<ExecutorCoroutine, const std::shared_ptr<HttpRequestExecutor::Response>&> {
  private:
    typedef webfr::web::protocol::http::outgoing::Request OutgoingRequest;
  private:
    HttpRequestExecutor* m_this;
    String m_method;
    String m_path;
    Headers m_headers;
    std::shared_ptr<Body> m_body;
    std::shared_ptr<const BodyDecoder> m_bodyDecoder;
    std::shared_ptr<HttpConnectionHandle> m_connectionHandle;
    webfr::data::share::MemoryLabel m_buffer;
    ResponseHeadersReader m_headersReader;
    std::shared_ptr<webfr::data::stream::OutputStreamBufferedProxy> m_upstream;
  private:
    std::shared_ptr<ConnectionProxy> m_connection;
  public:
    
    ExecutorCoroutine(HttpRequestExecutor* _this,
                      const String& method,
                      const String& path,
                      const Headers& headers,
                      const std::shared_ptr<Body>& body,
                      const std::shared_ptr<const BodyDecoder>& bodyDecoder,
                      const std::shared_ptr<HttpConnectionHandle>& connectionHandle)
      : m_this(_this)
      , m_method(method)
      , m_path(path)
      , m_headers(headers)
      , m_body(body)
      , m_bodyDecoder(bodyDecoder)
      , m_connectionHandle(connectionHandle)
      , m_buffer(base::StrBuffer::createShared(webfr::data::buffer::IOBuffer::BUFFER_SIZE))
      , m_headersReader(m_buffer, 4096)
    {}
    
    Action act() override {

      if(m_connectionHandle) {
        m_connection = m_connectionHandle->getConnection();
      }

      if(!m_connection) {
        throw RequestExecutionError(RequestExecutionError::ERROR_CODE_CANT_CONNECT,
                                    "[webfr::web::client::HttpRequestExecutor::executeOnceAsync::ExecutorCoroutine{act()}]: Connection is null");
      }

      m_connection->setInputStreamIOMode(data::stream::IOMode::ASYNCHRONOUS);
      m_connection->setOutputStreamIOMode(data::stream::IOMode::ASYNCHRONOUS);

      auto request = OutgoingRequest::createShared(m_method, m_path, m_headers, m_body);
      request->putHeaderIfNotExists_Unsafe(Header::HOST, m_this->m_connectionProvider->getProperty("host"));
      request->putHeaderIfNotExists_Unsafe(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
      m_upstream = webfr::data::stream::OutputStreamBufferedProxy::createShared(m_connection, m_buffer);
      return OutgoingRequest::sendAsync(request, m_upstream).next(m_upstream->flushAsync()).next(yieldTo(&ExecutorCoroutine::readResponse));

    }
    
    Action readResponse() {
      return m_headersReader.readHeadersAsync(m_connection).callbackTo(&ExecutorCoroutine::onHeadersParsed);
    }
    
    Action onHeadersParsed(const ResponseHeadersReader::Result& result) {

      auto connectionHeader = result.headers.getAsMemoryLabel<webfr::data::share::StringKeyLabelCI>(Header::CONNECTION);
      if (connectionHeader == "close") {
        m_connection->setInvalidateOnDestroy(true);
      }

      auto bodyStream = webfr::data::stream::InputStreamBufferedProxy::createShared(m_connection,
                                                                                    m_buffer,
                                                                                    result.bufferPosStart,
                                                                                    result.bufferPosEnd,
                                                                                    result.bufferPosStart != result.bufferPosEnd);
      
      return _return(Response::createShared(result.startingLine.statusCode,
                                            result.startingLine.description.toString(),
                                            result.headers, bodyStream, m_bodyDecoder));
      
    }

    Action handleError(webfr::async::Error* error) override {

      if(m_connection) {
        m_connection->invalidate();
      }

      return error;

    }
    
  };

  auto httpCH = std::static_pointer_cast<HttpConnectionHandle>(connectionHandle);
  return ExecutorCoroutine::startForResult(this, method, path, headers, body, m_bodyDecoder, httpCH);
  
}
  
}}}