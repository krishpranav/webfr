// includes
#include "MemoryLabelTest.hpp"
#include "webfr/core/data/share/MemoryLabel.hpp"
#include <unordered_map>
#include "webfr/web/protocol/http/Http.hpp"
#include "webfr-test/Checker.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace share {

namespace {
  typedef webfr::data::share::MemoryLabel MemoryLabel;
  typedef webfr::data::share::StringKeyLabel StringKeyLabel;
  typedef webfr::data::share::StringKeyLabelCI StringKeyLabelCI;
  typedef webfr::data::share::StringKeyLabelCI_FAST StringKeyLabelCI_FAST;
}
  
void MemoryLabelTest::onRun() {

  {
    WEBFR_LOGI(TAG, "StringKeyLabel default constructor...");
    StringKeyLabel s;
    StringKeyLabel s0;
    WEBFR_ASSERT(!s);
    WEBFR_ASSERT(s == nullptr);
    WEBFR_ASSERT(s == s0);
    WEBFR_ASSERT(s != "text");
    WEBFR_ASSERT(s != webfr::String("text"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabel nullptr constructor...");
    StringKeyLabel s(nullptr);
    WEBFR_ASSERT(!s);
    WEBFR_ASSERT(s == nullptr);
    WEBFR_ASSERT(s != "text");
    WEBFR_ASSERT(s != webfr::String("text"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabel const char* constructor...");
    StringKeyLabel s("hello");
    StringKeyLabel s0;
    WEBFR_ASSERT(s);
    WEBFR_ASSERT(s != nullptr);
    WEBFR_ASSERT(s != s0);
    WEBFR_ASSERT(s0 != s);
    WEBFR_ASSERT(s == "hello");
    WEBFR_ASSERT(s == webfr::String("hello"));
    WEBFR_ASSERT(s != "text");
    WEBFR_ASSERT(s != webfr::String("text"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabel webfr::String constructor...");
    StringKeyLabel s(webfr::String("hello"));
    WEBFR_ASSERT(s);
    WEBFR_ASSERT(s != nullptr);
    WEBFR_ASSERT(s == "hello");
    WEBFR_ASSERT(s == webfr::String("hello"));
    WEBFR_ASSERT(s != "text");
    WEBFR_ASSERT(s != webfr::String("text"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabelCI default constructor...");
    StringKeyLabelCI s;
    StringKeyLabelCI s0;
    WEBFR_ASSERT(!s);
    WEBFR_ASSERT(s == nullptr);
    WEBFR_ASSERT(s == s0);
    WEBFR_ASSERT(s != "teXt");
    WEBFR_ASSERT(s != webfr::String("teXt"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabelCI nullptr constructor...");
    StringKeyLabelCI s(nullptr);
    WEBFR_ASSERT(!s);
    WEBFR_ASSERT(s == nullptr);
    WEBFR_ASSERT(s != "teXt");
    WEBFR_ASSERT(s != webfr::String("teXt"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabelCI const char* constructor...");
    StringKeyLabelCI s("hello");
    StringKeyLabelCI s0;
    WEBFR_ASSERT(s);
    WEBFR_ASSERT(s != nullptr);
    WEBFR_ASSERT(s != s0);
    WEBFR_ASSERT(s0 != s);
    WEBFR_ASSERT(s == "helLO");
    WEBFR_ASSERT(s == webfr::String("helLO"));
    WEBFR_ASSERT(s != "text");
    WEBFR_ASSERT(s != webfr::String("teXt"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabelCI webfr::String constructor...");
    StringKeyLabelCI s(webfr::String("hello"));
    WEBFR_ASSERT(s);
    WEBFR_ASSERT(s != nullptr);
    WEBFR_ASSERT(s == "helLO");
    WEBFR_ASSERT(s == webfr::String("helLO"));
    WEBFR_ASSERT(s != "text");
    WEBFR_ASSERT(s != webfr::String("teXt"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabelCI_FAST default constructor...");
    StringKeyLabelCI_FAST s;
    StringKeyLabelCI_FAST s0;
    WEBFR_ASSERT(!s);
    WEBFR_ASSERT(s == nullptr);
    WEBFR_ASSERT(s == s0);
    WEBFR_ASSERT(s != "teXt");
    WEBFR_ASSERT(s != webfr::String("teXt"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabelCI_FAST nullptr constructor...");
    StringKeyLabelCI_FAST s(nullptr);
    WEBFR_ASSERT(!s);
    WEBFR_ASSERT(s == nullptr);
    WEBFR_ASSERT(s != "teXt");
    WEBFR_ASSERT(s != webfr::String("teXt"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabelCI_FAST const char* constructor...");
    StringKeyLabelCI_FAST s("hello");
    StringKeyLabelCI_FAST s0;
    WEBFR_ASSERT(s);
    WEBFR_ASSERT(s != nullptr);
    WEBFR_ASSERT(s != s0);
    WEBFR_ASSERT(s0 != s);
    WEBFR_ASSERT(s == "helLO");
    WEBFR_ASSERT(s == webfr::String("helLO"));
    WEBFR_ASSERT(s != "text");
    WEBFR_ASSERT(s != webfr::String("teXt"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "StringKeyLabelCI_FAST webfr::String constructor...");
    StringKeyLabelCI_FAST s(webfr::String("hello"));
    WEBFR_ASSERT(s);
    WEBFR_ASSERT(s != nullptr);
    WEBFR_ASSERT(s == "helLO");
    WEBFR_ASSERT(s == webfr::String("helLO"));
    WEBFR_ASSERT(s != "text");
    WEBFR_ASSERT(s != webfr::String("teXt"));
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "general test...");

    webfr::String sharedData = "big text goes here";
    webfr::String key1 = "key1";
    webfr::String key2 = "key2";
    webfr::String key3 = "key3";
    webfr::String key4 = "key4";

    std::unordered_map<StringKeyLabel, MemoryLabel> stringMap;
    std::unordered_map<StringKeyLabelCI, MemoryLabel> stringMapCI;
    std::unordered_map<StringKeyLabelCI_FAST, MemoryLabel> stringMapCI_FAST;

    // Case-Sensitive

    stringMap[key1] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[0], 3);
    stringMap[key2] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[4], 4);
    stringMap[key3] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[9], 4);
    stringMap[key4] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[14], 4);

    WEBFR_ASSERT(webfr::base::StrBuffer::equals("big", stringMap["key1"].getData(), 3));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("text", stringMap["key2"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("goes", stringMap["key3"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("here", stringMap["key4"].getData(), 4));

    WEBFR_ASSERT(stringMap.find("Key1") == stringMap.end());
    WEBFR_ASSERT(stringMap.find("Key2") == stringMap.end());
    WEBFR_ASSERT(stringMap.find("Key3") == stringMap.end());
    WEBFR_ASSERT(stringMap.find("Key4") == stringMap.end());


    // CI

    stringMapCI[key1] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[0], 3);
    stringMapCI[key2] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[4], 4);
    stringMapCI[key3] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[9], 4);
    stringMapCI[key4] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[14], 4);

    WEBFR_ASSERT(webfr::base::StrBuffer::equals("big", stringMapCI["key1"].getData(), 3));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("text", stringMapCI["key2"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("goes", stringMapCI["key3"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("here", stringMapCI["key4"].getData(), 4));

    WEBFR_ASSERT(webfr::base::StrBuffer::equals("big", stringMapCI["KEY1"].getData(), 3));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("text", stringMapCI["KEY2"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("goes", stringMapCI["KEY3"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("here", stringMapCI["KEY4"].getData(), 4));


    // CI_FAST

    stringMapCI_FAST[key1] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[0], 3);
    stringMapCI_FAST[key2] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[4], 4);
    stringMapCI_FAST[key3] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[9], 4);
    stringMapCI_FAST[key4] = MemoryLabel(sharedData.getPtr(), &sharedData->getData()[14], 4);

    WEBFR_ASSERT(webfr::base::StrBuffer::equals("big", stringMapCI_FAST["key1"].getData(), 3));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("text", stringMapCI_FAST["key2"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("goes", stringMapCI_FAST["key3"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("here", stringMapCI_FAST["key4"].getData(), 4));

    WEBFR_ASSERT(webfr::base::StrBuffer::equals("big", stringMapCI_FAST["KEY1"].getData(), 3));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("text", stringMapCI_FAST["KEY2"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("goes", stringMapCI_FAST["KEY3"].getData(), 4));
    WEBFR_ASSERT(webfr::base::StrBuffer::equals("here", stringMapCI_FAST["KEY4"].getData(), 4));

    {

      v_int32 iterationsCount = 100;

      webfr::String headersText =
        "header0: value0\r\n"
        "header1: value1\r\n"
        "header2: value2\r\n"
        "header3: value3\r\n"
        "header4: value4\r\n"
        "header5: value5\r\n"
        "header6: value6\r\n"
        "header7: value7\r\n"
        "header8: value8\r\n"
        "header9: value9\r\n"
        "\r\n";

      {

        webfr::test::PerformanceChecker timer("timer");

        for (v_int32 i = 0; i < iterationsCount; i++) {

          webfr::parser::Caret caret(headersText);
          webfr::web::protocol::http::Status status;
          webfr::web::protocol::http::Headers headers;
          webfr::web::protocol::http::Parser::parseHeaders(headers, headersText.getPtr(), caret, status);

          WEBFR_ASSERT(status.code == 0);
          WEBFR_ASSERT(headers.getSize() == 10);


          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header0").equals("value0", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header1").equals("value1", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header2").equals("value2", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header3").equals("value3", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header4").equals("value4", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header5").equals("value5", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header6").equals("value6", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header7").equals("value7", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header8").equals("value8", 6));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header9").equals("value9", 6));

          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header0").equals("value0"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header1").equals("value1"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header2").equals("value2"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header3").equals("value3"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header4").equals("value4"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header5").equals("value5"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header6").equals("value6"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header7").equals("value7"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header8").equals("value8"));
          WEBFR_ASSERT(headers.getAsMemoryLabel<StringKeyLabel>("header9").equals("value9"));

        }

      }

    }

    WEBFR_LOGI(TAG, "OK");

  }

  {

    v_int32 iterationsCount = 100;

    webfr::String headersText =
      "header0: value0\r\n"
      "header0: value1\r\n"
      "header1: value2\r\n"
      "header1: value3\r\n"
      "header2: value4\r\n"
      "header2: value5\r\n"
      "header3: value6\r\n"
      "header3: value7\r\n"
      "header4: value8\r\n"
      "header4: value9\r\n"
      "\r\n";

    webfr::parser::Caret caret(headersText);
    webfr::web::protocol::http::Status status;
    webfr::web::protocol::http::Headers headers;
    webfr::web::protocol::http::Parser::parseHeaders(headers, headersText.getPtr(), caret, status);

    WEBFR_ASSERT(status.code == 0);
    WEBFR_ASSERT(headers.getSize() == 10);

    for(auto& h : headers.getAll()) {
      auto key = h.first.toString();
      auto val = h.second.toString();
      WEBFR_LOGD(TAG, "'%s': '%s'", key->c_str(), val->c_str());
    }

  }

}
  
}}}}}