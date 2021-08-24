#ifndef webfr_test_encoding_AuthorizationHandlerTest_hpp
#define webfr_test_encoding_AuthorizationHandlerTest_hpp

#include <webfr/web/server/handler/AuthorizationHandler.hpp>
#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web { namespace server { namespace handler {

class AuthorizationHandlerTest : public UnitTest{
 public:
  AuthorizationHandlerTest():UnitTest("TEST[web::server::handler::AuthorizationHandlerTest]"){}
  void onRun() override;
};

}}}}}

#endif 