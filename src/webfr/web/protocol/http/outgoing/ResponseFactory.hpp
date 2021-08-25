#ifndef webfr_web_protocol_http_outgoing_ResponseFactory_hpp
#define webfr_web_protocol_http_outgoing_ResponseFactory_hpp

#include "./Response.hpp"
#include "webfr/core/data/mapping/ObjectMapper.hpp"
#include "webfr/core/data/mapping/type/Type.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

class ResponseFactory {
public:

  static std::shared_ptr<Response> createResponse(const Status& status, const webfr::String& text);

  static std::shared_ptr<Response> createResponse(const Status& status,
                                                  const webfr::Void& dto,
                                                  const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper);
  
};
  
}}}}}

#endif 