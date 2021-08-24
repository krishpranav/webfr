#ifndef webfr_web_protocol_http_utils_CommunicationUtils_hpp
#define webfr_web_protocol_http_utils_CommunicationUtils_hpp

#include "webfr/web/protocol/http/incoming/Request.hpp"
#include "webfr/web/protocol/http/outgoing/Response.hpp"
#include "webfr/web/protocol/http/encoding/ProviderCollection.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace utils {

class CommunicationUtils {
public:

  enum class ConnectionState : int {
    ALIVE = 0, 
    DELEGATED = 1, 
    CLOSING = 2, 
    DEAD = 3 
  };

private:
  static bool headerEqualsCI_FAST(const webfr::data::share::MemoryLabel& headerValue, const char* value);
public:

  static void considerConnectionState(const std::shared_ptr<protocol::http::incoming::Request>& request,
                                      const std::shared_ptr<protocol::http::outgoing::Response>& response,
                                      ConnectionState& connectionState);

  static std::shared_ptr<encoding::EncoderProvider> selectEncoder(const std::shared_ptr<http::incoming::Request>& request,
                                                                  const std::shared_ptr<http::encoding::ProviderCollection>& providers);
  
};
  
}}}}}

#endif 