#ifndef webfr_test_web_PipelineTest_hpp
#define webfr_test_web_PipelineTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web {

class PipelineTest : public UnitTest {
private:
  v_uint16 m_port;
  v_int32 m_pipelineSize;
public:

  PipelineTest(v_uint16 port, v_int32 pipelineSize)
    : UnitTest("TEST[web::PipelineTest]")
    , m_port(port)
    , m_pipelineSize(pipelineSize)
  {}

  void onRun() override;

};

}}}

#endif 