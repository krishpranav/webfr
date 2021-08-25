#ifndef webfr_web_protocol_http_incoming_RequestHeadersReader_hpp
#define webfr_web_protocol_http_incoming_RequestHeadersReader_hpp

#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/core/async/Coroutine.hpp"
#include "webfr/core/data/stream/StreamBufferedProxy.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

class RequestHeadersReader {
public:

  typedef webfr::async::Action Action;
private:
  static constexpr v_uint32 SECTION_END = ('\r' << 24) | ('\n' << 16) | ('\r' << 8) | ('\n');
public:

  struct Result {

    http::RequestStartingLine startingLine;
    http::Headers headers;

  };

private:

  struct ReadHeadersIteration {
    v_uint32 accumulator = 0;
    bool done = false;
  };

private:
  v_io_size readHeadersSectionIterative(ReadHeadersIteration& iteration,
                                              data::stream::InputStreamBufferedProxy* stream,
                                              async::Action& action);
private:
  webfr::data::stream::BufferOutputStream* m_bufferStream;
  v_buff_size m_readChunkSize;
  v_buff_size m_maxHeadersSize;
public:

  RequestHeadersReader(webfr::data::stream::BufferOutputStream* bufferStream,
                       v_buff_size readChunkSize = 2048,
                       v_buff_size maxHeadersSize = 4096)
    : m_bufferStream(bufferStream)
    , m_readChunkSize(readChunkSize)
    , m_maxHeadersSize(maxHeadersSize)
  {}

  Result readHeaders(data::stream::InputStreamBufferedProxy* stream, http::HttpError::Info& error);

  webfr::async::CoroutineStarterForResult<const RequestHeadersReader::Result&> readHeadersAsync(const std::shared_ptr<data::stream::InputStreamBufferedProxy>& stream);
  
};
  
}}}}}

#endif 