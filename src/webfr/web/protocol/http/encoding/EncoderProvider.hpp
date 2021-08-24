#ifndef webfr_web_protocol_http_encoding_EncoderProvider_hpp
#define webfr_web_protocol_http_encoding_EncoderProvider_hpp

#include "webfr/core/data/buffer/Processor.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace encoding {

class EncoderProvider {
public:

  virtual ~EncoderProvider() = default;

  virtual webfr::String getEncodingName() = 0;

  virtual std::shared_ptr<data::buffer::Processor> getProcessor() = 0;

};

}}}}}

#endif 