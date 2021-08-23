// includes
#include "BufferStreamTest.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include "webfr/core/utils/Binary.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace stream {

void BufferStreamTest::onRun() {

  typedef webfr::data::stream::BufferOutputStream BufferOutputStream;

  {
    BufferOutputStream stream;

    stream  << "int=" << 1 << ", float=" << 1.1 << ", "
            << "bool=" << true << " or " << false;

    WEBFR_LOGV(TAG, "str='%s'", stream.toString()->c_str());

    stream.setCurrentPosition(0);
    stream << 101;
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(101));

    stream.setCurrentPosition(0);
    stream << (v_float32)101.1;
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::float32ToStr(101.1f));

    stream.setCurrentPosition(0);
    stream << (v_float64)101.1;
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::float64ToStr(101.1));

    stream.setCurrentPosition(0);
    stream << true;
    WEBFR_ASSERT(stream.toString() == "true");

    stream.setCurrentPosition(0);
    stream << false;
    WEBFR_ASSERT(stream.toString() == "false");

    stream.setCurrentPosition(0);
    stream << webfr::String("oat++");
    WEBFR_ASSERT(stream.toString() == "oat++");

    stream.setCurrentPosition(0);
    stream << webfr::Int8(8);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(8));

    stream.setCurrentPosition(0);
    stream << webfr::Int16(16);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(16));

    stream.setCurrentPosition(0);
    stream << webfr::Int32(32);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(32));

    stream.setCurrentPosition(0);
    stream << webfr::Int64(64);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::int32ToStr(64));

    stream.setCurrentPosition(0);
    stream << webfr::Float32(0.32f);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::float32ToStr(0.32f));

    stream.setCurrentPosition(0);
    stream << webfr::Float64(0.64);
    WEBFR_ASSERT(stream.toString() == webfr::utils::conversion::float64ToStr(0.64));

    stream.setCurrentPosition(0);
    stream << webfr::Boolean(true);
    WEBFR_ASSERT(stream.toString() == "true");

    stream.setCurrentPosition(0);
    stream << webfr::Boolean(false);
    WEBFR_ASSERT(stream.toString() == "false");

  }

  {

    BufferOutputStream stream;
    v_int32 fragmentsCount = 1024 * 10;

    for(v_int32 i = 0; i < fragmentsCount; i++) {
      stream.writeSimple("0123456789", 10);
    }

    auto wholeText = stream.toString();

    WEBFR_ASSERT(wholeText->getSize() == fragmentsCount * 10);

    v_int32 substringSize = 10;
    for(v_int32 i = 0; i < wholeText->getSize() - substringSize; i ++) {
      WEBFR_ASSERT(webfr::String((const char*)&wholeText->getData()[i], substringSize, false) == stream.getSubstring(i, substringSize));
    }

  }

  {

    webfr::String sample = "0123456789";
    webfr::String text = "";

    for(v_int32 i = 0; i < 1024; i++ ) {
      text = text + sample;
    }

    BufferOutputStream stream(0);

    for(v_int32 i = 0; i < 1024; i++ ) {
      stream << sample;

      WEBFR_ASSERT(stream.getCapacity() >= stream.getCurrentPosition());

    }

    WEBFR_ASSERT(text == stream.toString());

    WEBFR_ASSERT(stream.getCapacity() == webfr::utils::Binary::nextP2(1024 * (10)));

  }

}

}}}}}