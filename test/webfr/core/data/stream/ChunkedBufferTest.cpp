#include "ChunkedBufferTest.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace stream {

void ChunkedBufferTest::onRun() {

  typedef webfr::data::stream::ChunkedBuffer ChunkedBuffer;

  {
    ChunkedBuffer stream;

    stream  << "int=" << 1 << ", float=" << 1.1 << ", "
            << "bool=" << true << " or " << false;

    WEBFR_LOGV(TAG, "str='%s'", stream.toString()->c_str());

    stream.clear();
    stream << 101;
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(101));

    stream.clear();
    stream << (v_float32)101.1;
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::float32ToStr(101.1f));

    stream.clear();
    stream << (v_float64)101.1;
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::float64ToStr(101.1));

    stream.clear();
    stream << true;
    WEBFR_ASSERT(stream.toString() == "true");

    stream.clear();
    stream << false;
    WEBFR_ASSERT(stream.toString() == "false");

    stream.clear();
    stream << webfr::String("oat++");
    WEBFR_ASSERT(stream.toString() == "oat++");

    stream.clear();
    stream << webfr::Int8(8);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(8));

    stream.clear();
    stream << webfr::Int16(16);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(16));

    stream.clear();
    stream << webfr::Int32(32);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(32));

    stream.clear();
    stream << webfr::Int64(64);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(64));

    stream.clear();
    stream << webfr::Float32(0.32f);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::float32ToStr(0.32f));

    stream.clear();
    stream << webfr::Float64(0.64);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::float64ToStr(0.64));

    stream.clear();
    stream << webfr::Boolean(true);
    WEBFR_ASSERT(stream.toString() == "true");

    stream.clear();
    stream << webfr::Boolean(false);
    WEBFR_ASSERT(stream.toString() == "false");

  }

  {

    ChunkedBuffer stream;

    for(v_int32 i = 0; i < ChunkedBuffer::CHUNK_ENTRY_SIZE * 10; i++) {
      stream.writeSimple("0123456789", 10);
    }

    auto wholeText = stream.toString();

    WEBFR_ASSERT(wholeText->getSize() == ChunkedBuffer::CHUNK_ENTRY_SIZE * 10 * 10);

    v_int32 substringSize = 10;
    for(v_int32 i = 0; i < wholeText->getSize() - substringSize; i ++) {
      WEBFR_ASSERT(webfr::String((const char*)&wholeText->getData()[i], substringSize, false) == stream.getSubstring(i, substringSize));
    }

    substringSize = (v_int32) ChunkedBuffer::CHUNK_ENTRY_SIZE * 2;
    for(v_int32 i = 0; i < wholeText->getSize() - substringSize; i ++) {
      WEBFR_ASSERT(webfr::String((const char*)&wholeText->getData()[i], substringSize, false) == stream.getSubstring(i, substringSize));
    }

  }


}

}}}}}
