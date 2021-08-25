#ifndef webfr_web_server_HttpProcessor_hpp
#define webfr_web_server_HttpProcessor_hpp

#include "./HttpRouter.hpp"
#include "./interceptor/RequestInterceptor.hpp"
#include "./interceptor/ResponseInterceptor.hpp"
#include "./handler/ErrorHandler.hpp"
#include "webfr/web/protocol/http/encoding/ProviderCollection.hpp"
#include "webfr/web/protocol/http/incoming/RequestHeadersReader.hpp"
#include "webfr/web/protocol/http/incoming/Request.hpp"
#include "webfr/web/protocol/http/outgoing/Response.hpp"
#include "webfr/web/protocol/http/utils/CommunicationUtils.hpp"
#include "webfr/core/data/stream/StreamBufferedProxy.hpp"
#include "webfr/core/async/Processor.hpp"

namespace webfr { namespace web { namespace server {

class HttpProcessor {
public:
  typedef std::list<std::shared_ptr<web::server::interceptor::RequestInterceptor>> RequestInterceptors;
  typedef std::list<std::shared_ptr<web::server::interceptor::ResponseInterceptor>> ResponseInterceptors;
  typedef web::protocol::http::incoming::RequestHeadersReader RequestHeadersReader;
  typedef protocol::http::utils::CommunicationUtils::ConnectionState ConnectionState;
public:

  struct Config {

    v_buff_size headersInBufferInitial = 2048;

    v_buff_size headersOutBufferInitial = 2048;

    v_buff_size headersReaderChunkSize = 2048;

    v_buff_size headersReaderMaxSize = 4096;

  };

public:

  struct Components {

    Components(const std::shared_ptr<HttpRouter>& pRouter,
               const std::shared_ptr<protocol::http::encoding::ProviderCollection>& pContentEncodingProviders,
               const std::shared_ptr<const webfr::web::protocol::http::incoming::BodyDecoder>& pBodyDecoder,
               const std::shared_ptr<handler::ErrorHandler>& pErrorHandler,
               const RequestInterceptors& pRequestInterceptors,
               const ResponseInterceptors& pResponseInterceptors,
               const std::shared_ptr<Config>& pConfig);

    Components(const std::shared_ptr<HttpRouter>& pRouter);

    Components(const std::shared_ptr<HttpRouter>& pRouter, const std::shared_ptr<Config>& pConfig);

    /**
     * Router to route incoming requests. &id:webfr::web::server::HttpRouter;.
     */
    std::shared_ptr<HttpRouter> router;

    /**
     * Content-encoding providers. &id:webfr::web::protocol::encoding::ProviderCollection;.
     */
    std::shared_ptr<protocol::http::encoding::ProviderCollection> contentEncodingProviders;

    /**
     * Body decoder. &id:webfr::web::protocol::http::incoming::BodyDecoder;.
     */
    std::shared_ptr<const webfr::web::protocol::http::incoming::BodyDecoder> bodyDecoder;

    /**
     * Error handler. &id:webfr::web::server::handler::ErrorHandler;.
     */
    std::shared_ptr<handler::ErrorHandler> errorHandler;

    /**
     * Collection of request interceptors. &id:webfr::web::server::interceptor::RequestInterceptor;.
     */
    RequestInterceptors requestInterceptors;

    /**
     * Collection of request interceptors. &id:webfr::web::server::interceptor::ResponseInterceptor;.
     */
    ResponseInterceptors responseInterceptors;

    /**
     * Resource allocation config. &l:HttpProcessor::Config;.
     */
    std::shared_ptr<Config> config;

  };

private:

  struct ProcessingResources {

    ProcessingResources(const std::shared_ptr<Components>& pComponents,
                        const std::shared_ptr<webfr::data::stream::IOStream>& pConnection);

    std::shared_ptr<Components> components;
    std::shared_ptr<webfr::data::stream::IOStream> connection;
    webfr::data::stream::BufferOutputStream headersInBuffer;
    webfr::data::stream::BufferOutputStream headersOutBuffer;
    RequestHeadersReader headersReader;
    std::shared_ptr<webfr::data::stream::InputStreamBufferedProxy> inStream;

  };

  static
  std::shared_ptr<protocol::http::outgoing::Response>
  processNextRequest(ProcessingResources& resources,
                     const std::shared_ptr<protocol::http::incoming::Request>& request,
                     ConnectionState& connectionState);
  static ConnectionState processNextRequest(ProcessingResources& resources);

public:

  class Task : public base::Countable {
  private:
    std::shared_ptr<Components> m_components;
    std::shared_ptr<webfr::data::stream::IOStream> m_connection;
  public:

    Task(const std::shared_ptr<Components>& components,
         const std::shared_ptr<webfr::data::stream::IOStream>& connection);
  public:

    void run();

  };
  
public:

  class Coroutine : public webfr::async::Coroutine<HttpProcessor::Coroutine> {
  private:
    std::shared_ptr<Components> m_components;
    std::shared_ptr<webfr::data::stream::IOStream> m_connection;
    webfr::data::stream::BufferOutputStream m_headersInBuffer;
    RequestHeadersReader m_headersReader;
    std::shared_ptr<webfr::data::stream::BufferOutputStream> m_headersOutBuffer;
    std::shared_ptr<webfr::data::stream::InputStreamBufferedProxy> m_inStream;
    ConnectionState m_connectionState;
  private:
    webfr::web::server::HttpRouter::BranchRouter::Route m_currentRoute;
    std::shared_ptr<protocol::http::incoming::Request> m_currentRequest;
    std::shared_ptr<protocol::http::outgoing::Response> m_currentResponse;
  public:


    Coroutine(const std::shared_ptr<Components>& components,
              const std::shared_ptr<webfr::data::stream::IOStream>& connection);
    
    Action act() override;

    Action parseHeaders();
    
    Action onHeadersParsed(const RequestHeadersReader::Result& headersReadResult);
    
    Action onRequestFormed();
    Action onResponse(const std::shared_ptr<protocol::http::outgoing::Response>& response);
    Action onResponseFormed();
    Action onRequestDone();
    
    Action handleError(Error* error) override;
    
  };
  
};
  
}}}

#endif 