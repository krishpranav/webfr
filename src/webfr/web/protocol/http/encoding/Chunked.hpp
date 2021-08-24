#ifndef webfr_web_protocol_http_encoding_Chunked_hpp
#define webfr_web_protocol_http_encoding_Chunked_hpp

#include "EncoderProvider.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace encoding {

class EncoderChunked : public data::buffer::Processor {
private:
  webfr::String m_chunkHeader;
  bool m_writeChunkHeader = true;
  bool m_firstChunk = true;
  bool m_finished = false;
  v_io_size m_lastFlush = 0;
public:

  v_io_size suggestInputStreamReadSize() override;

  v_int32 iterate(data::buffer::InlineReadData& dataIn, data::buffer::InlineReadData& dataOut) override;

};

class DecoderChunked : public data::buffer::Processor {
public:
  static constexpr v_int32 ERROR_CHUNK_HEADER_TOO_LONG = 100;
private:
  data::stream::BufferOutputStream m_chunkHeaderBuffer;
  v_io_size m_currentChunkSize;
  bool m_firstChunk;
  bool m_finished;
  v_io_size m_lastFlush;
private:
  v_int32 readHeader(data::buffer::InlineReadData& dataIn);
public:

  DecoderChunked();

  v_io_size suggestInputStreamReadSize() override;

  v_int32 iterate(data::buffer::InlineReadData& dataIn, data::buffer::InlineReadData& dataOut) override;

};

class ChunkedEncoderProvider : public EncoderProvider {
public:

  webfr::String getEncodingName() override;

  std::shared_ptr<data::buffer::Processor> getProcessor() override;

};

class ChunkedDecoderProvider : public EncoderProvider {
public:

  webfr::String getEncodingName() override;

  std::shared_ptr<data::buffer::Processor> getProcessor() override;

};

}}}}}

#endif 