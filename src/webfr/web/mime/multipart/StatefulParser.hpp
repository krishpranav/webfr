#ifndef webfr_web_mime_multipart_StatefulParser_hpp
#define webfr_web_mime_multipart_StatefulParser_hpp

#include "webfr/core/data/stream/ChunkedBuffer.hpp"
#include "webfr/core/data/share/LazyStringMap.hpp"
#include "webfr/core/Types.hpp"
#include <unordered_map>

namespace webfr { namespace web { namespace mime { namespace multipart {

class StatefulParser {
private:
  static constexpr v_int32 STATE_BOUNDARY = 0;
  static constexpr v_int32 STATE_AFTER_BOUNDARY = 1;
  static constexpr v_int32 STATE_HEADERS = 2;
  static constexpr v_int32 STATE_DATA = 3;
  static constexpr v_int32 STATE_DONE = 4;
private:
  static constexpr v_uint32 HEADERS_SECTION_END = ('\r' << 24) | ('\n' << 16) | ('\r' << 8) | ('\n');
private:
  
  typedef webfr::data::share::LazyStringMultimap<webfr::data::share::StringKeyLabelCI_FAST> Headers;
public:

  class Listener {
  public:

    typedef webfr::data::share::LazyStringMultimap<webfr::data::share::StringKeyLabelCI_FAST> Headers;
  public:

    virtual ~Listener() = default;

    virtual void onPartHeaders(const Headers& partHeaders) = 0;

    virtual void onPartData(p_char8 data, v_buff_size size) = 0;

  };

public:

  class AsyncListener {
  public:

    typedef webfr::data::share::LazyStringMultimap<webfr::data::share::StringKeyLabelCI_FAST> Headers;
  public:

    virtual ~AsyncListener() = default;

    virtual async::CoroutineStarter onPartHeadersAsync(const Headers& partHeaders) = 0;

    virtual async::CoroutineStarter onPartDataAsync(p_char8 data, v_buff_size size) = 0;

  };

private:

  class ListenerCall {
  public:

    static constexpr v_int32 CALL_NONE = 0;
    static constexpr v_int32 CALL_ON_HEADERS = 1;
    static constexpr v_int32 CALL_ON_DATA = 2;

  public:

    ListenerCall()
      : callType(CALL_NONE)
      , data(nullptr)
      , size(0)
    {}

    v_int32 callType;
    p_char8 data;
    v_io_size size;

    void setOnHeadersCall();
    void setOnDataCall(p_char8 pData, v_buff_size pSize);

    void call(StatefulParser* parser);
    async::CoroutineStarter callAsync(StatefulParser* parser);

    explicit operator bool() const;

  };

private:

  v_int32 m_state;
  v_int64 m_currPartIndex;
  v_buff_size m_currBoundaryCharIndex;
  bool m_checkForBoundary;
  bool m_finishingBoundary;
  bool m_readingBody;

  v_uint32 m_headerSectionEndAccumulator;

  webfr::String m_firstBoundarySample;
  webfr::String m_nextBoundarySample;

  webfr::data::stream::ChunkedBuffer m_headersBuffer;

  v_buff_size m_maxPartHeadersSize;

  std::shared_ptr<Listener> m_listener;
  std::shared_ptr<AsyncListener> m_asyncListener;

private:

  void parseHeaders(Headers& headers);

private:

  ListenerCall parseNext_Boundary(data::buffer::InlineWriteData& inlineData);
  void         parseNext_AfterBoundary(data::buffer::InlineWriteData& inlineData);
  ListenerCall parseNext_Headers(data::buffer::InlineWriteData& inlineData);
  ListenerCall parseNext_Data(data::buffer::InlineWriteData& inlineData);

public:

  StatefulParser(const webfr::String& boundary,
                 const std::shared_ptr<Listener>& listener,
                 const std::shared_ptr<AsyncListener>& asyncListener);


  void parseNext(data::buffer::InlineWriteData& inlineData, async::Action& action);

  bool finished();

};

}}}}

#endif 