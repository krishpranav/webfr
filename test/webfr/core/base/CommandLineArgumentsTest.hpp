#ifndef webfr_test_base_CommandLineArgumentsTest_hpp
#define webfr_test_base_CommandLineArgumentsTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace base {
  
class CommandLineArgumentsTest : public UnitTest{
public:
  
  CommandLineArgumentsTest():UnitTest("TEST[base::CommandLineArgumentsTest]"){}
  void onRun() override;
  
};
  
}}}

#endif 