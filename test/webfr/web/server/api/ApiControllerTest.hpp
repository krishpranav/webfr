#ifndef webfr_test_web_server_api_ApiControllerTest_hpp
#define webfr_test_web_server_api_ApiControllerTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web { namespace server { namespace api {

class ApiControllerTest : public UnitTest {
public:

  ApiControllerTest():UnitTest("TEST[web::server::api::ApiControllerTest]"){}
  void onRun() override;

};

}}}}}

#endif 