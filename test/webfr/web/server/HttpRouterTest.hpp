#ifndef webfr_test_web_server_HttpRouterTest_hpp
#define webfr_test_web_server_HttpRouterTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web { namespace server {

class HttpRouterTest : public UnitTest {
public:

  HttpRouterTest():UnitTest("TEST[web::server::HttpRouterTest]"){}
  void onRun() override;

};

}}}}

#endif 