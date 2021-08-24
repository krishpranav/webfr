#include "CommunicationUtils.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace utils {
  
bool CommunicationUtils::headerEqualsCI_FAST(const webfr::data::share::MemoryLabel& headerValue, const char* value) {
  v_int32 size = (v_int32) std::strlen(value);
  return size == headerValue.getSize() && webfr::base::StrBuffer::equalsCI_FAST(headerValue.getData(), value, size);
}
  
void CommunicationUtils::considerConnectionState(const std::shared_ptr<protocol::http::incoming::Request>& request,
                                                 const std::shared_ptr<protocol::http::outgoing::Response>& response,
                                                 ConnectionState& connectionState)
{

  if(connectionState != ConnectionState::ALIVE) {
    return;
  }

  auto outState = response->getHeaders().getAsMemoryLabel<webfr::data::share::StringKeyLabelCI_FAST>(Header::CONNECTION);
  if(outState && outState == Header::Value::CONNECTION_UPGRADE) {
    connectionState = ConnectionState::DELEGATED;
    return;
  }
  
  if(request) {
    /* If the connection header is present in the request and its value isn't keep-alive, then close */
    auto connection = request->getHeaders().getAsMemoryLabel<webfr::data::share::StringKeyLabelCI_FAST>(Header::CONNECTION);
    if(connection) {
      if(connection != Header::Value::CONNECTION_KEEP_ALIVE) {
        connectionState = ConnectionState::CLOSING;
      }
      return;
    }
    
    /* If protocol == HTTP/1.1 */
    /* Set HTTP/1.1 default Connection header value (Keep-Alive), if no Connection header present in response. */
    /* Set keep-alive to value specified in response otherwise */
    auto& protocol = request->getStartingLine().protocol;
    if(protocol && headerEqualsCI_FAST(protocol, "HTTP/1.1")) {
      if(outState && outState != Header::Value::CONNECTION_KEEP_ALIVE) {
        connectionState = ConnectionState::CLOSING;
      }
      return;
    }
  }
  
  /* If protocol != HTTP/1.1 */
  /* Set default Connection header value (Close), if no Connection header present in response. */
  /* Set keep-alive to value specified in response otherwise */
  if(!outState || outState != Header::Value::CONNECTION_KEEP_ALIVE) {
    connectionState = ConnectionState::CLOSING;
  }

  return;
  
}

std::shared_ptr<encoding::EncoderProvider>
CommunicationUtils::selectEncoder(const std::shared_ptr<http::incoming::Request>& request,
                                  const std::shared_ptr<http::encoding::ProviderCollection>& providers)
{
  if(providers && request) {

    auto suggested = request->getHeaders().getAsMemoryLabel<webfr::data::share::StringKeyLabel>(Header::ACCEPT_ENCODING);

    if(suggested) {

      http::HeaderValueData valueData;
      http::Parser::parseHeaderValueData(valueData, suggested, ',');

      return providers->get(valueData.tokens);

    }

  }

  return nullptr;

}
  
}}}}}
