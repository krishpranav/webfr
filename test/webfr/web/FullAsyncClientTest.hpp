#ifndef webfr_test_web_FullAsyncClientTest_hpp
#define webfr_test_web_FullAsyncClientTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web {

class FullAsyncClientTest : public UnitTest {
private:
  v_uint16 m_port;
  v_int32 m_connectionsPerEndpoint;
public:

  FullAsyncClientTest(v_uint16 port, v_int32 connectionsPerEndpoint)
    : UnitTest("TEST[web::FullAsyncClientTest]")
    , m_port(port)
    , m_connectionsPerEndpoint(connectionsPerEndpoint)
  {}

  void onRun() override;

};

}}}

#endif 