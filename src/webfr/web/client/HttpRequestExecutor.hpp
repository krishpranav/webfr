#ifndef webfr_web_client_HttpRequestExecutor_hpp
#define webfr_web_client_HttpRequestExecutor_hpp

#include "./RequestExecutor.hpp"
#include "webfr/web/protocol/http/incoming/SimpleBodyDecoder.hpp"
#include "webfr/network/ConnectionPool.hpp"
#include "webfr/network/ConnectionProvider.hpp"

namespace webfr { namespace web { namespace client {

class HttpRequestExecutor : public webfr::base::Countable, public RequestExecutor {
private:
  typedef webfr::web::protocol::http::Header Header;
  typedef webfr::network::ClientConnectionProvider ClientConnectionProvider;
  typedef webfr::web::protocol::http::incoming::BodyDecoder BodyDecoder;
protected:
  std::shared_ptr<ClientConnectionProvider> m_connectionProvider;
  std::shared_ptr<const BodyDecoder> m_bodyDecoder;
public:

  class ConnectionProxy : public data::stream::IOStream {
  private:
    std::shared_ptr<ClientConnectionProvider> m_connectionProvider;
    std::shared_ptr<data::stream::IOStream> m_connection;
    bool m_valid;
    bool m_invalidateOnDestroy;
  public:

    ConnectionProxy(const std::shared_ptr<ClientConnectionProvider>& connectionProvider,
                    const std::shared_ptr<data::stream::IOStream>& connection);

    ~ConnectionProxy() override;

    v_io_size read(void *buffer, v_buff_size count, async::Action& action) override;
    v_io_size write(const void *data, v_buff_size count, async::Action& action) override;

    void setInputStreamIOMode(data::stream::IOMode ioMode) override;
    data::stream::IOMode getInputStreamIOMode() override;
    data::stream::Context& getInputStreamContext() override;

    void setOutputStreamIOMode(data::stream::IOMode ioMode) override;
    data::stream::IOMode getOutputStreamIOMode() override;
    data::stream::Context& getOutputStreamContext() override;

    void invalidate();
    void setInvalidateOnDestroy(bool invalidateOnDestroy);

  };

public:

  /**
   * Connection handle for &l:HttpRequestExecutor; <br>
   * For more details see &id:webfr::web::client::RequestExecutor::ConnectionHandle;.
   */
  class HttpConnectionHandle : public ConnectionHandle {
  private:
    std::shared_ptr<ConnectionProxy> m_connectionProxy;
  public:

    HttpConnectionHandle(const std::shared_ptr<ConnectionProxy>& connectionProxy);

    std::shared_ptr<ConnectionProxy> getConnection();

    void invalidate();

  };
public:

  HttpRequestExecutor(const std::shared_ptr<ClientConnectionProvider>& connectionProvider,
                      const std::shared_ptr<RetryPolicy>& retryPolicy = nullptr,
                      const std::shared_ptr<const BodyDecoder>& bodyDecoder =
                      std::make_shared<webfr::web::protocol::http::incoming::SimpleBodyDecoder>());
public:

  static std::shared_ptr<HttpRequestExecutor>
  createShared(const std::shared_ptr<ClientConnectionProvider>& connectionProvider,
               const std::shared_ptr<RetryPolicy>& retryPolicy = nullptr,
               const std::shared_ptr<const BodyDecoder>& bodyDecoder =
               std::make_shared<webfr::web::protocol::http::incoming::SimpleBodyDecoder>());

  std::shared_ptr<ConnectionHandle> getConnection() override;

  webfr::async::CoroutineStarterForResult<const std::shared_ptr<HttpRequestExecutor::ConnectionHandle>&> getConnectionAsync() override;

  void invalidateConnection(const std::shared_ptr<ConnectionHandle>& connectionHandle) override;

  std::shared_ptr<Response> executeOnce(const String& method,
                                        const String& path,
                                        const Headers& headers,
                                        const std::shared_ptr<Body>& body,
                                        const std::shared_ptr<ConnectionHandle>& connectionHandle = nullptr) override;

  webfr::async::CoroutineStarterForResult<const std::shared_ptr<Response>&>
  executeOnceAsync(const String& method,
                   const String& path,
                   const Headers& headers,
                   const std::shared_ptr<Body>& body,
                   const std::shared_ptr<ConnectionHandle>& connectionHandle = nullptr) override;
  
};
  
}}}

#endif 