#ifndef webfr_web_protocol_http_outgoing_MultipartBody_hpp
#define webfr_web_protocol_http_outgoing_MultipartBody_hpp

#include "./Body.hpp"
#include "webfr/web/mime/multipart/Multipart.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

class MultipartBody : public webfr::base::Countable, public Body {
public:

  typedef webfr::web::mime::multipart::Multipart Multipart;

  typedef webfr::web::mime::multipart::Part Part;
private:

  typedef webfr::data::stream::ReadCallback ReadCallback;

private:

  static constexpr v_int32 STATE_BOUNDARY = 0;
  static constexpr v_int32 STATE_HEADERS = 1;
  static constexpr v_int32 STATE_BODY = 2;
  static constexpr v_int32 STATE_INC_PART = 3;
  static constexpr v_int32 STATE_ROUND = 4; // number of possible states. used to round the state.
  static constexpr v_int32 STATE_FINISHED = 5;

private:

  class PartIterator {
  private:
    std::shared_ptr<Multipart> m_multipart;
    std::shared_ptr<Part> m_part;
    bool m_isFirst;
    bool m_initialized;
  public:

    PartIterator(const std::shared_ptr<Multipart>& multipart)
      : m_multipart(multipart)
      , m_part(nullptr)
      , m_isFirst(true)
      , m_initialized(false)
    {}

    void init(async::Action& action) {
      if(!m_initialized) {
        m_part = m_multipart->readNextPart(action);
        m_initialized = true;
      }
    }

    void inc(async::Action& action) {
      m_part = m_multipart->readNextPart(action);
      m_isFirst = false;
    }

    bool finished() {
      return m_part == nullptr;
    }

    bool isFirst() {
      return m_isFirst;
    }

    std::shared_ptr<Part> get() {
      return m_part;
    }

  };

private:

  static v_io_size readBoundary(const std::shared_ptr<Multipart>& multipart,
                                PartIterator& iterator,
                                data::stream::BufferInputStream& readStream,
                                void *buffer,
                                v_buff_size count);

  static v_io_size readHeaders(const std::shared_ptr<Multipart>& multipart,
                               PartIterator& iterator,
                               data::stream::BufferInputStream& readStream,
                               void *buffer,
                               v_buff_size count);

private:
  std::shared_ptr<Multipart> m_multipart;
  webfr::String m_contentType;
private:
  PartIterator m_iterator;
  v_int32 m_state;
  webfr::data::stream::BufferInputStream m_readStream;
  bool m_flushParts;
private:
  v_io_size readBody(void *buffer, v_buff_size count, async::Action& action);
  v_io_size incPart(async::Action& action);
public:

  MultipartBody(const std::shared_ptr<Multipart>& multipart,
                const webfr::String& contentType = "multipart/form-data",
                bool flushParts = false);

  v_io_size read(void *buffer, v_buff_size count, async::Action& action) override;

  void declareHeaders(Headers& headers) override;

  p_char8 getKnownData() override;

  v_int64 getKnownSize() override;

};

}}}}}

#endif 