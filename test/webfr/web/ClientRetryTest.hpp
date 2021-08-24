#ifndef webfr_test_web_ClientRetryTest_hpp
#define webfr_test_web_ClientRetryTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web {

class ClientRetryTest : public UnitTest {
private:
  v_uint16 m_port;
public:

  ClientRetryTest(v_uint16 port)
    : UnitTest("TEST[web::ClientRetryTest]")
    , m_port(port)
  {}

  void onRun() override;

};

}}}

#endif 