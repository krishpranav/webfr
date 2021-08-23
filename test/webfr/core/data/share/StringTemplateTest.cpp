// includes
#include "StringTemplateTest.hpp"
#include "webfr/core/data/share/StringTemplate.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace share {

void StringTemplateTest::onRun() {

  typedef webfr::data::share::StringTemplate StringTemplate;

  {
    WEBFR_LOGI(TAG, "Case1 ...");
    StringTemplate t("{} World!", {{0, 1, "p1"}});
    auto result = t.format(std::vector<webfr::String>({"Hello"}));
    WEBFR_ASSERT(result == "Hello World!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case2 ...");
    StringTemplate t("{} World!", {{0, 1, "p1"}});
    auto result = t.format(std::unordered_map<webfr::String, webfr::String>({{"p1", "Hello"}}));
    WEBFR_ASSERT(result == "Hello World!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case3 ...");
    StringTemplate t("Hello {}", {{6, 7, "p1"}});
    auto result = t.format(std::vector<webfr::String>({"World!"}));
    WEBFR_ASSERT(result == "Hello World!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case4 ...");
    StringTemplate t("Hello {}", {{6, 7, "p1"}});
    auto result = t.format(std::unordered_map<webfr::String, webfr::String>({{"p1", "World!"}}));
    WEBFR_ASSERT(result == "Hello World!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case5 ...");
    StringTemplate t("Hello {} World!", {{6, 7, "p1"}});
    auto result = t.format(std::vector<webfr::String>({"My"}));
    WEBFR_ASSERT(result == "Hello My World!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case6 ...");
    StringTemplate t("Hello {} World!", {{6, 7, "p1"}});
    auto result = t.format(std::unordered_map<webfr::String, webfr::String>({{"p1", "My"}}));
    WEBFR_ASSERT(result == "Hello My World!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case7 ...");
    StringTemplate t("? ? ?", {{0, 0, "p1"}, {2, 2, "p2"}, {4, 4, "p3"}});
    auto result = t.format(std::vector<webfr::String>({"Hello", "World", "WebFramework++!"}));
    WEBFR_ASSERT(result == "Hello World WebFramework++!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case8 ...");
    StringTemplate t("? ? ?", {{0, 0, "p1"}, {2, 2, "p2"}, {4, 4, "p3"}});
    auto result = t.format(std::unordered_map<webfr::String, webfr::String>({{"p3", "Hello"}, {"p2", "World"}, {"p1", "WebFramework++!"}}));
    WEBFR_ASSERT(result == "WebFramework++! World Hello");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case9 ...");
    StringTemplate t("? ? ?", {{0, 0, "p1"}, {2, 2, "p2"}, {4, 4, "p3"}});
    auto result = t.format("A");
    WEBFR_ASSERT(result == "A A A");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Case10 ...");
    StringTemplate t("? ? ?",
      {
        {0, 0, "p1", std::make_shared<webfr::base::Countable>()},
        {2, 2, "p2", std::make_shared<webfr::base::Countable>()},
        {4, 4, "p3", std::make_shared<webfr::base::Countable>()}
      }
    );
    auto result = t.format("(A)");
    WEBFR_ASSERT(result == "(A) (A) (A)");
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}