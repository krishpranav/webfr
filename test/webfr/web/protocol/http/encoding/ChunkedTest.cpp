#include "ChunkedTest.hpp"
#include "webfr/web/protocol/http/encoding/Chunked.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"

namespace webfr { namespace test { namespace web { namespace protocol { namespace http { namespace encoding {

void ChunkedTest::onRun() {

  webfr::String data = "Hello World!!!";
  webfr::String encoded;
  webfr::String decoded;

  { // Empty string
    webfr::data::stream::BufferInputStream inStream(webfr::String(""));
    webfr::data::stream::BufferOutputStream outStream;

    webfr::web::protocol::http::encoding::EncoderChunked encoder;

    const v_int32 bufferSize = 5;
    v_char8 buffer[bufferSize];

    auto count = webfr::data::stream::transfer(&inStream, &outStream, 0, buffer, bufferSize, &encoder);
    encoded = outStream.toString();

    WEBFR_ASSERT(count == 0);
    WEBFR_ASSERT(encoded == "0\r\n\r\n");
  }

  { // Empty string
    webfr::data::stream::BufferInputStream inStream(encoded);
    webfr::data::stream::BufferOutputStream outStream;

    webfr::web::protocol::http::encoding::DecoderChunked decoder;

    const v_int32 bufferSize = 5;
    v_char8 buffer[bufferSize];

    auto count = webfr::data::stream::transfer(&inStream, &outStream, 0, buffer, bufferSize, &decoder);
    decoded = outStream.toString();
    WEBFR_ASSERT(count == encoded->getSize());
    WEBFR_ASSERT(decoded == "");
  }

  {
    webfr::data::stream::BufferInputStream inStream(data);
    webfr::data::stream::BufferOutputStream outStream;

    webfr::web::protocol::http::encoding::EncoderChunked encoder;

    const v_int32 bufferSize = 5;
    v_char8 buffer[bufferSize];

    auto count = webfr::data::stream::transfer(&inStream, &outStream, 0, buffer, bufferSize, &encoder);
    encoded = outStream.toString();

    WEBFR_ASSERT(count == data->getSize());
    WEBFR_ASSERT(encoded == "5\r\nHello\r\n5\r\n Worl\r\n4\r\nd!!!\r\n0\r\n\r\n");
  }

  {
    webfr::data::stream::BufferInputStream inStream(encoded);
    webfr::data::stream::BufferOutputStream outStream;

    webfr::web::protocol::http::encoding::DecoderChunked decoder;

    const v_int32 bufferSize = 5;
    v_char8 buffer[bufferSize];

    auto count = webfr::data::stream::transfer(&inStream, &outStream, 0, buffer, bufferSize, &decoder);
    decoded = outStream.toString();
    WEBFR_ASSERT(count == encoded->getSize());
    WEBFR_LOGD(TAG, "decoded='%s'", decoded->getData());
    WEBFR_ASSERT(decoded == data);
  }

  {
    webfr::data::stream::BufferInputStream inStream(data);
    webfr::data::stream::BufferOutputStream outStream;

    webfr::web::protocol::http::encoding::EncoderChunked encoder;
    webfr::web::protocol::http::encoding::DecoderChunked decoder;
    webfr::data::buffer::ProcessingPipeline pipeline({
      &encoder,
      &decoder
    });

    const v_int32 bufferSize = 5;
    v_char8 buffer[bufferSize];

    auto count = webfr::data::stream::transfer(&inStream, &outStream, 0, buffer, bufferSize, &pipeline);
    auto result = outStream.toString();
    WEBFR_ASSERT(count == data->getSize());
    WEBFR_LOGD(TAG, "result='%s'", result->getData());
    WEBFR_ASSERT(result == data);
  }


}

}}}}}}