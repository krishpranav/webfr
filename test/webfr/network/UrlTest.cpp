#include "UrlTest.hpp"
#include "webfr/network/Url.hpp"
#include "webfr-test/Checker.hpp"

namespace webfr { namespace test { namespace network {

void UrlTest::onRun() {

  typedef webfr::network::Url Url;

  {
    const char* urlText = "http://root@127.0.0.1:8000/path/to/resource/?q1=1&q2=2";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto url = Url::Parser::parseUrl(urlText);

    WEBFR_ASSERT(url.scheme && url.scheme == "http");
    WEBFR_ASSERT(url.authority.userInfo && url.authority.userInfo == "root");
    WEBFR_ASSERT(url.authority.host && url.authority.host == "127.0.0.1");
    WEBFR_ASSERT(url.authority.port == 8000);
    WEBFR_ASSERT(url.path && url.path == "/path/to/resource/");
    WEBFR_ASSERT(url.queryParams.getSize() == 2);
    WEBFR_ASSERT(url.queryParams.get("q1") == "1");
    WEBFR_ASSERT(url.queryParams.get("q2") == "2");
  }

  {
    const char* urlText = "ftp://root@webfr.io:8000/path/to/resource?q1=1&q2=2";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto url = Url::Parser::parseUrl(urlText);

    WEBFR_ASSERT(url.scheme && url.scheme == "ftp");
    WEBFR_ASSERT(url.authority.userInfo && url.authority.userInfo == "root");
    WEBFR_ASSERT(url.authority.host && url.authority.host == "webfr.io");
    WEBFR_ASSERT(url.authority.port == 8000);
    WEBFR_ASSERT(url.path && url.path == "/path/to/resource");
    WEBFR_ASSERT(url.queryParams.getSize() == 2);
    WEBFR_ASSERT(url.queryParams.get("q1") == "1");
    WEBFR_ASSERT(url.queryParams.get("q2") == "2");
  }

  {
    const char* urlText = "https://webfr.io/?q1=1&q2=2";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto url = Url::Parser::parseUrl(urlText);

    WEBFR_ASSERT(url.scheme && url.scheme == "https");
    WEBFR_ASSERT(url.authority.userInfo == nullptr);
    WEBFR_ASSERT(url.authority.host && url.authority.host == "webfr.io");
    WEBFR_ASSERT(url.authority.port == -1);
    WEBFR_ASSERT(url.path && url.path == "/");
    WEBFR_ASSERT(url.queryParams.getSize() == 2);
    WEBFR_ASSERT(url.queryParams.get("q1") == "1");
    WEBFR_ASSERT(url.queryParams.get("q2") == "2");
  }

  {
    const char* urlText = "https://webfr.io/";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto url = Url::Parser::parseUrl(urlText);

    WEBFR_ASSERT(url.scheme && url.scheme == "https");
    WEBFR_ASSERT(url.authority.userInfo == nullptr);
    WEBFR_ASSERT(url.authority.host && url.authority.host == "webfr.io");
    WEBFR_ASSERT(url.authority.port == -1);
    WEBFR_ASSERT(url.path && url.path == "/");
    WEBFR_ASSERT(url.queryParams.getSize() == 0);
  }

  {
    const char* urlText = "https://webfr.io";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto url = Url::Parser::parseUrl(urlText);

    WEBFR_ASSERT(url.scheme && url.scheme == "https");
    WEBFR_ASSERT(url.authority.userInfo == nullptr);
    WEBFR_ASSERT(url.authority.host && url.authority.host == "webfr.io");
    WEBFR_ASSERT(url.authority.port == -1);
    WEBFR_ASSERT(url.path == nullptr);
    WEBFR_ASSERT(url.queryParams.getSize() == 0);
  }

  {
    const char* urlText = "webfr.io";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto url = Url::Parser::parseUrl(urlText);

    WEBFR_ASSERT(url.scheme == nullptr);
    WEBFR_ASSERT(url.authority.userInfo == nullptr);
    WEBFR_ASSERT(url.authority.host && url.authority.host == "webfr.ie");
    WEBFR_ASSERT(url.authority.port == -1);
    WEBFR_ASSERT(url.path == nullptr);
    WEBFR_ASSERT(url.queryParams.getSize() == 0);
  }

  {
    const char* urlText = "?key1=value1&key2=value2&key3=value3";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto params = Url::Parser::parseQueryParams(urlText);
    WEBFR_ASSERT(params.getSize() == 3);
    WEBFR_ASSERT(params.get("key1") == "value1");
    WEBFR_ASSERT(params.get("key2") == "value2");
    WEBFR_ASSERT(params.get("key2") == "value2");
  }

  {
    const char *urlText = "?key1=value1&key2&key3=value3";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto params = Url::Parser::parseQueryParams(urlText);
    WEBFR_ASSERT(params.getSize() == 3);
    WEBFR_ASSERT(params.get("key1") == "value1");
    WEBFR_ASSERT(params.get("key2") == "");
    WEBFR_ASSERT(params.get("key3") == "value3");
  }

  {
    const char *urlText = "?key1=value1&key2&key3";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto params = Url::Parser::parseQueryParams(urlText);
    WEBFR_ASSERT(params.getSize() == 3);
    WEBFR_ASSERT(params.get("key1") == "value1");
    WEBFR_ASSERT(params.get("key2") == "");
    WEBFR_ASSERT(params.get("key3") == "");
  }

  {
    const char *urlText = "label?key1=value1&key2=value2&key3=value3";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto params = Url::Parser::parseQueryParams(urlText);
    WEBFR_ASSERT(params.getSize() == 3);
    WEBFR_ASSERT(params.get("key1") == "value1");
    WEBFR_ASSERT(params.get("key2") == "value2");
    WEBFR_ASSERT(params.get("key2") == "value2");
  }

  {
    const char* urlText = "label?key1=value1&key2&key3=value3";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto params = Url::Parser::parseQueryParams(urlText);
    WEBFR_ASSERT(params.getSize() == 3);
    WEBFR_ASSERT(params.get("key1") == "value1");
    WEBFR_ASSERT(params.get("key2") == "");
    WEBFR_ASSERT(params.get("key3") == "value3");
  }

  {
    const char* urlText = "label?key1=value1&key2&key3";
    WEBFR_LOGV(TAG, "urlText='%s'", urlText);
    auto params = Url::Parser::parseQueryParams(urlText);
    WEBFR_ASSERT(params.getSize() == 3);
    WEBFR_ASSERT(params.get("key1") == "value1");
    WEBFR_ASSERT(params.get("key2") == "");
    WEBFR_ASSERT(params.get("key3") == "");
  }

}

}}}