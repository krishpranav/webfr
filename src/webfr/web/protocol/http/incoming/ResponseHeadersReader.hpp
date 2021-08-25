#ifndef webfr_web_protocol_http_incoming_ResponseHeadersReader_hpp
#define webfr_web_protocol_http_incoming_ResponseHeadersReader_hpp

#include "webfr/web/protocol/http/Http.hpp"
#include "webfr/core/async/Coroutine.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace incoming {

class ResponseHeadersReader {
public:

  typedef webfr::async::Action Action;

private:
  static constexpr v_uint32 SECTION_END = ('\r' << 24) | ('\n' << 16) | ('\r' << 8) | ('\n');
public:

  struct Result {

    http::ResponseStartingLine startingLine;

    http::Headers headers;

    v_buff_size bufferPosStart;

    v_buff_size bufferPosEnd;
  };
private:

  struct ReadHeadersIteration {
    v_uint32 accumulator = 0;
    v_buff_size progress = 0;
    bool done = false;
  };

private:

  v_io_size readHeadersSectionIterative(ReadHeadersIteration& iteration,
                                              const std::shared_ptr<webfr::data::stream::IOStream>& connection,
                                              data::stream::ConsistentOutputStream* bufferStream,
                                              Result& result,
                                              async::Action& action);

private:
  webfr::data::share::MemoryLabel m_buffer;
  v_buff_size m_maxHeadersSize;
public:

  ResponseHeadersReader(const webfr::data::share::MemoryLabel& buffer, v_buff_size maxHeadersSize)
    : m_buffer(buffer)
    , m_maxHeadersSize(maxHeadersSize)
  {}

  Result readHeaders(const std::shared_ptr<webfr::data::stream::IOStream>& connection, http::HttpError::Info& error);

  webfr::async::CoroutineStarterForResult<const Result&> readHeadersAsync(const std::shared_ptr<webfr::data::stream::IOStream>& connection);
  
};
  
}}}}}

#endif 