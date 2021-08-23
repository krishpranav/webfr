// includes
#include "StringTest.hpp"
#include "webfr/core/Types.hpp"
#include <functional>

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

void StringTest::onRun() {

  {
    webfr::String s = "hello"; 
    std::hash<webfr::String>{}(s);
  }

  {
    WEBFR_LOGI(TAG, "test default constructor");
    webfr::String s;
    WEBFR_ASSERT(!s);
    WEBFR_ASSERT(s == nullptr);
    WEBFR_ASSERT(s == (const char*) nullptr);
    WEBFR_ASSERT(s.valueType == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test const char* constructor");
    webfr::String s("");
    WEBFR_ASSERT(s);
    WEBFR_ASSERT(s != nullptr);
    WEBFR_ASSERT(s != (const char*) nullptr)
    WEBFR_ASSERT(s->getSize() == 0);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test const char* implicit constructor");
    webfr::String s = "";
    WEBFR_ASSERT(s);
    WEBFR_ASSERT(s != nullptr);
    WEBFR_ASSERT(s != (const char*) nullptr)
    WEBFR_ASSERT(s->getSize() == 0);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test '==', '!=' operators");
    webfr::String s1 = "a";
    webfr::String s2;

    WEBFR_ASSERT(s1 != s2);
    WEBFR_ASSERT(s2 != s1);

    WEBFR_ASSERT(s1 == "a");
    WEBFR_ASSERT(s1 != "aa");
    WEBFR_ASSERT(s1 != "");

    s2 = "aa";

    WEBFR_ASSERT(s1 != s2);
    WEBFR_ASSERT(s2 != s1);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test copy-asssign operator");
    webfr::String s1 = "s1";
    webfr::String s2;

    s2 = s1;

    WEBFR_ASSERT(s1 == s2);
    WEBFR_ASSERT(s1.get() == s2.get());

    s1 = "s2";

    WEBFR_ASSERT(s1 != s2);
    WEBFR_ASSERT(s2 != s1);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test const char* assign operator");
    webfr::String s1 = "s1";
    webfr::String s2(s1);

    WEBFR_ASSERT(s1 == s2);
    WEBFR_ASSERT(s1.get() == s2.get());

    s1 = "s2";

    WEBFR_ASSERT(s1 != s2);
    WEBFR_ASSERT(s2 != s1);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test move assign operator");
    webfr::String s1 = "s1";
    webfr::String s2;

    s2 = std::move(s1);

    WEBFR_ASSERT(s1 == nullptr);
    WEBFR_ASSERT(s2 != nullptr);
    WEBFR_ASSERT(s2 == "s1");

    WEBFR_ASSERT(s1 != s2);
    WEBFR_ASSERT(s1.get() != s2.get());
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}
