#ifndef webfr_test_core_data_share_MemoryLabelTest_hpp
#define webfr_test_core_data_share_MemoryLabelTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace share {
  
class MemoryLabelTest : public UnitTest{
public:
  
  MemoryLabelTest():UnitTest("TEST[core::data::share::MemoryLabelTest]"){}
  void onRun() override;
  
};
  
}}}}}

#endif 