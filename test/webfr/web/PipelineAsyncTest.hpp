#ifndef webfr_test_web_PipelineAsyncTest_hpp
#define webfr_test_web_PipelineAsyncTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web {

class PipelineAsyncTest : public UnitTest {
private:
  v_uint16 m_port;
  v_int32 m_pipelineSize;
public:

  PipelineAsyncTest(v_uint16 port, v_int32 pipelineSize)
    : UnitTest("TEST[web::PipelineAsyncTest]")
    , m_port(port)
    , m_pipelineSize(pipelineSize)
  {}

  void onRun() override;

};

}}}

#endif 