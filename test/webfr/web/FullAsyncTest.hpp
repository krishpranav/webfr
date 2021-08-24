#ifndef webfr_test_web_FullAsyncTest_hpp
#define webfr_test_web_FullAsyncTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web {
  
class FullAsyncTest : public UnitTest {
private:
  v_uint16 m_port;
  v_int32 m_iterationsPerStep;
public:
  
  FullAsyncTest(v_uint16 port, v_int32 iterationsPerStep)
    : UnitTest("TEST[web::FullAsyncTest]")
    , m_port(port)
    , m_iterationsPerStep(iterationsPerStep)
  {}

  void onRun() override;
  
};
  
}}}

#endif 