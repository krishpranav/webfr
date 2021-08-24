#ifndef webfr_web_client_RequestExecutor_hpp
#define webfr_web_client_RequestExecutor_hpp

#include "RetryPolicy.hpp"
#include "webfr/web/protocol/http/incoming/Response.hpp"
#include "webfr/web/protocol/http/outgoing/Body.hpp"
#include "webfr/web/protocol/http/Http.hpp"

namespace webfr { namespace web { namespace client {

class RequestExecutor {
public:

  typedef webfr::String String;

  typedef webfr::async::Action Action;
public:

  typedef webfr::web::protocol::http::Headers Headers;

  typedef webfr::web::protocol::http::incoming::Response Response;

  typedef webfr::web::protocol::http::outgoing::Body Body;
public:
  
  class ConnectionHandle {
  public:
    virtual ~ConnectionHandle() {}
  };
  
public:
  typedef Action (webfr::async::AbstractCoroutine::*AsyncCallback)(const std::shared_ptr<Response>&);
  typedef Action (webfr::async::AbstractCoroutine::*AsyncConnectionCallback)(const std::shared_ptr<ConnectionHandle>&);
public:

  class RequestExecutionError : public std::runtime_error {
  public:

    constexpr static const v_int32 ERROR_CODE_CANT_CONNECT = 1;

    constexpr static const v_int32 ERROR_CODE_CANT_PARSE_STARTING_LINE = 2;

    constexpr static const v_int32 ERROR_CODE_CANT_PARSE_HEADERS = 3;

    constexpr static const v_int32 ERROR_CODE_CANT_READ_RESPONSE = 4;

    constexpr static const v_int32 ERROR_CODE_NO_RESPONSE = 5;
  private:
    v_int32 m_errorCode;
    const char* m_message;
    v_int32 m_readErrorCode;
  public:

    RequestExecutionError(v_int32 errorCode, const char* message, v_int32 readErrorCode = 0);

    v_int32 getErrorCode() const;

    const char* getMessage() const;
    
    v_int32 getReadErrorCode() const;
    
  };

private:
  std::shared_ptr<RetryPolicy> m_retryPolicy;
public:

  RequestExecutor(const std::shared_ptr<RetryPolicy>& retryPolicy);

  virtual ~RequestExecutor() = default;

  virtual std::shared_ptr<ConnectionHandle> getConnection() = 0;

  virtual webfr::async::CoroutineStarterForResult<const std::shared_ptr<ConnectionHandle>&> getConnectionAsync() = 0;

  virtual void invalidateConnection(const std::shared_ptr<ConnectionHandle>& connectionHandle) = 0;

  virtual std::shared_ptr<Response> executeOnce(const String& method,
                                                const String& path,
                                                const Headers& headers,
                                                const std::shared_ptr<Body>& body,
                                                const std::shared_ptr<ConnectionHandle>& connectionHandle) = 0;

  virtual webfr::async::CoroutineStarterForResult<const std::shared_ptr<Response>&>
  executeOnceAsync(const String& method,
                   const String& path,
                   const Headers& headers,
                   const std::shared_ptr<Body>& body,
                   const std::shared_ptr<ConnectionHandle>& connectionHandle) = 0;

  virtual std::shared_ptr<Response> execute(const String& method,
                                            const String& path,
                                            const Headers& headers,
                                            const std::shared_ptr<Body>& body,
                                            const std::shared_ptr<ConnectionHandle>& connectionHandle);

  webfr::async::CoroutineStarterForResult<const std::shared_ptr<Response>&>
  virtual executeAsync(const String& method,
                       const String& path,
                       const Headers& headers,
                       const std::shared_ptr<Body>& body,
                       const std::shared_ptr<ConnectionHandle>& connectionHandle);

};
  
}}}

#endif 