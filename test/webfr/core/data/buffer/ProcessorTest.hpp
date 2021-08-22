#ifndef webfr_test_core_data_buffer_ProcessorTest_hpp
#define webfr_test_core_data_buffer_ProcessorTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace buffer {

class ProcessorTest : public UnitTest{
public:

  ProcessorTest():UnitTest("TEST[core::data::buffer::ProcessorTest]"){}
  void onRun() override;

};

}}}}}

#endif 