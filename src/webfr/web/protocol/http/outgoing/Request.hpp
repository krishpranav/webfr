#ifndef webfr_web_protocol_http_outgoing_Request_hpp
#define webfr_web_protocol_http_outgoing_Request_hpp

#include "webfr/web/protocol/http/outgoing/Body.hpp"
#include "webfr/web/protocol/http/Http.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

class Request : public webfr::base::Countable {
public:

  typedef protocol::http::Headers Headers;
public:
  OBJECT_POOL(Outgoing_Request_Pool, Request, 32)
  SHARED_OBJECT_POOL(Shared_Outgoing_Request_Pool, Request, 32)
private:
  webfr::data::share::StringKeyLabel m_method;
  webfr::data::share::StringKeyLabel m_path;
  Headers m_headers;
  std::shared_ptr<Body> m_body;
public:


  Request(const webfr::data::share::StringKeyLabel& method,
          const webfr::data::share::StringKeyLabel& path,
          const Headers& headers,
          const std::shared_ptr<Body>& body);
  
public:

  static std::shared_ptr<Request> createShared(const webfr::data::share::StringKeyLabel& method,
                                               const webfr::data::share::StringKeyLabel& path,
                                               const Headers& headers,
                                               const std::shared_ptr<Body>& body);

  const webfr::data::share::StringKeyLabel& getMethod() const;

  const webfr::data::share::StringKeyLabel& getPath() const;

  Headers& getHeaders();

  void putHeader(const webfr::String& key, const webfr::String& value);

  bool putHeaderIfNotExists(const webfr::String& key, const webfr::String& value);

  void putHeader_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value);

  bool putHeaderIfNotExists_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value);

  webfr::String getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const;

  std::shared_ptr<Body> getBody();

  void send(data::stream::OutputStream* stream);

  static webfr::async::CoroutineStarter sendAsync(std::shared_ptr<Request> _this,
                                                  const std::shared_ptr<data::stream::OutputStream>& stream);
  
};
  
}}}}}

#endif 