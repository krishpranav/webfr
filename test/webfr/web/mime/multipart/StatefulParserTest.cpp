#include "StatefulParserTest.hpp"
#include "webfr/web/mime/multipart/PartList.hpp"
#include "webfr/web/mime/multipart/InMemoryPartReader.hpp"
#include "webfr/web/mime/multipart/Reader.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"
#include <unordered_map>

namespace webfr { namespace test { namespace web { namespace mime { namespace multipart {

namespace {

  typedef webfr::web::mime::multipart::Part Part;

  static const char* TEST_DATA_1 =
    "--12345\r\n"
    "Content-Disposition: form-data; name=\"part1\"\r\n"
    "\r\n"
    "part1-value\r\n"
    "--12345\r\n"
    "Content-Disposition: form-data; name='part2' filename=\"filename.txt\"\r\n"
    "\r\n"
    "--part2-file-content-line1\r\n"
    "--1234part2-file-content-line2\r\n"
    "--12345\r\n"
    "Content-Disposition: form-data; name=part3 filename=\"filename.jpg\"\r\n"
    "\r\n"
    "part3-file-binary-data\r\n"
    "--12345--\r\n"
    ;


  void parseStepByStep(const webfr::String& text,
                       const webfr::String& boundary,
                       const std::shared_ptr<webfr::web::mime::multipart::StatefulParser::Listener>& listener,
                       const v_int32 step)
  {

    webfr::web::mime::multipart::StatefulParser parser(boundary, listener, nullptr);

    webfr::data::stream::BufferInputStream stream(text.getPtr(), text->getData(), text->getSize());
    std::unique_ptr<v_char8[]> buffer(new v_char8[step]);
    v_io_size size;
    while((size = stream.readSimple(buffer.get(), step)) != 0) {
      webfr::data::buffer::InlineWriteData inlineData(buffer.get(), size);
      while(inlineData.bytesLeft > 0 && !parser.finished()) {
        webfr::async::Action action;
        parser.parseNext(inlineData, action);
      }
    }
    WEBFR_ASSERT(parser.finished());

  }

  void assertPartData(const std::shared_ptr<Part>& part, const webfr::String& value) {

    WEBFR_ASSERT(part->getInMemoryData());
    WEBFR_ASSERT(part->getInMemoryData() == value);

    v_int64 bufferSize = 16;
    std::unique_ptr<v_char8[]> buffer(new v_char8[bufferSize]);

    webfr::data::stream::ChunkedBuffer stream;
    webfr::data::stream::transfer(part->getInputStream().get(), &stream, 0, buffer.get(), bufferSize);

    webfr::String readData = stream.toString();

    WEBFR_ASSERT(readData == part->getInMemoryData());

  }

}

void StatefulParserTest::onRun() {

  webfr::String text = TEST_DATA_1;

  for(v_int32 i = 1; i < text->getSize(); i++) {

    webfr::web::mime::multipart::PartList multipart("12345");

    auto listener = std::make_shared<webfr::web::mime::multipart::PartsParser>(&multipart);
    listener->setDefaultPartReader(std::make_shared<webfr::web::mime::multipart::InMemoryPartReader>(128));

    parseStepByStep(text, "12345", listener, i);

    if(multipart.count() != 3) {
      WEBFR_LOGD(TAG, "TEST_DATA_1 itearation %d", i);
    }

    WEBFR_ASSERT(multipart.count() == 3);

    auto part1 = multipart.getNamedPart("part1");
    auto part2 = multipart.getNamedPart("part2");
    auto part3 = multipart.getNamedPart("part3");

    WEBFR_ASSERT(part1);
    WEBFR_ASSERT(part2);
    WEBFR_ASSERT(part3);

    WEBFR_ASSERT(part1->getFilename().get() == nullptr);
    WEBFR_ASSERT(part2->getFilename() == "filename.txt");
    WEBFR_ASSERT(part3->getFilename() == "filename.jpg");

    assertPartData(part1, "part1-value");
    assertPartData(part2, "--part2-file-content-line1\r\n--1234part2-file-content-line2");
    assertPartData(part3, "part3-file-binary-data");

  }

}

}}}}}