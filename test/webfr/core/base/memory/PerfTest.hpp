#ifndef PerfTest_hpp
#define PerfTest_hpp

// includes
#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace memory {
  
class PerfTest : public UnitTest{
public:
  
  PerfTest():UnitTest("TEST[base::memory::PerfTest]"){}
  void onRun() override;
  
};
  
}}}

#endif 