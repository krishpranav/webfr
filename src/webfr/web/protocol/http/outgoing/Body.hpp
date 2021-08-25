#ifndef webfr_web_protocol_http_outgoing_Body_hpp
#define webfr_web_protocol_http_outgoing_Body_hpp

#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/core/data/stream/Stream.hpp"
#include "webfr/core/collection/ListMap.hpp"
#include "webfr/core/async/Coroutine.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

class Body : public data::stream::ReadCallback {
protected:
  typedef http::Headers Headers;
public:

  virtual ~Body() = default;

  virtual void declareHeaders(Headers& headers) = 0;

  virtual p_char8 getKnownData() = 0;

  virtual v_int64 getKnownSize() = 0;
  
};
  
}}}}}

#endif 