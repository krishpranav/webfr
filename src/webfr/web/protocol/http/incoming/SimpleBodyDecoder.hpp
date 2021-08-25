#ifndef webfr_web_protocol_http_incoming_SimpleBodyDecoder_hpp
#define webfr_web_protocol_http_incoming_SimpleBodyDecoder_hpp

#include "BodyDecoder.hpp"
#include "webfr/web/protocol/http/encoding/ProviderCollection.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

class SimpleBodyDecoder : public BodyDecoder {
private:
  std::shared_ptr<encoding::ProviderCollection> m_contentDecoders;
private:
  base::ObjectHandle<data::buffer::Processor> getStreamProcessor(const data::share::StringKeyLabelCI& transferEncoding,
                                                                 const data::share::StringKeyLabelCI& contentEncoding) const;
public:

  SimpleBodyDecoder(const std::shared_ptr<encoding::ProviderCollection>& contentDecoders = nullptr);

  void decode(const Headers& headers, data::stream::InputStream* bodyStream, data::stream::WriteCallback* writeCallback) const override;

  webfr::async::CoroutineStarter decodeAsync(const Headers& headers,
                                             const std::shared_ptr<webfr::data::stream::InputStream>& bodyStream,
                                             const std::shared_ptr<webfr::data::stream::WriteCallback>& writeCallback) const override;
  
  
};
  
}}}}}

#endif 