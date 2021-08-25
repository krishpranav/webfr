#ifndef webfr_web_protocol_http_outgoing_Response_hpp
#define webfr_web_protocol_http_outgoing_Response_hpp

#include "webfr/web/protocol/http/outgoing/Body.hpp"
#include "webfr/web/protocol/http/encoding/EncoderProvider.hpp"
#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/network/ConnectionHandler.hpp"
#include "webfr/core/async/Coroutine.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

class Response : public webfr::base::Countable {
public:

  typedef http::Headers Headers;

  typedef webfr::network::ConnectionHandler ConnectionHandler;
public:
  OBJECT_POOL(Outgoing_Response_Pool, Response, 32)
  SHARED_OBJECT_POOL(Shared_Outgoing_Response_Pool, Response, 32)
private:
  Status m_status;
  Headers m_headers;
  std::shared_ptr<Body> m_body;
  std::shared_ptr<ConnectionHandler> m_connectionUpgradeHandler;
  std::shared_ptr<const ConnectionHandler::ParameterMap> m_connectionUpgradeParameters;
public:

  Response(const Status& status, const std::shared_ptr<Body>& body);
public:

  static std::shared_ptr<Response> createShared(const Status& status, const std::shared_ptr<Body>& body);

  const Status& getStatus() const;

  Headers& getHeaders();

  void putHeader(const webfr::String& key, const webfr::String& value);

  bool putHeaderIfNotExists(const webfr::String& key, const webfr::String& value);

  void putHeader_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value);

  bool putHeaderIfNotExists_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value);

  webfr::String getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const;

  void setConnectionUpgradeHandler(const std::shared_ptr<ConnectionHandler>& handler);

  std::shared_ptr<ConnectionHandler> getConnectionUpgradeHandler();

  void setConnectionUpgradeParameters(const std::shared_ptr<const ConnectionHandler::ParameterMap>& parameters);

  std::shared_ptr<const ConnectionHandler::ParameterMap> getConnectionUpgradeParameters();

  void send(data::stream::OutputStream* stream,
            data::stream::BufferOutputStream* headersWriteBuffer,
            http::encoding::EncoderProvider* contentEncoderProvider);

  static webfr::async::CoroutineStarter sendAsync(const std::shared_ptr<Response>& _this,
                                                  const std::shared_ptr<data::stream::OutputStream>& stream,
                                                  const std::shared_ptr<data::stream::BufferOutputStream>& headersWriteBuffer,
                                                  const std::shared_ptr<http::encoding::EncoderProvider>& contentEncoderProvider);
  
};
  
}}}}}

#endif 