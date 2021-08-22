#ifndef test_memory_MemoryPoolTest_hpp
#define test_memory_MemoryPoolTest_hpp

// includes
#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace memory {
  
class MemoryPoolTest : public UnitTest{
public:
  
  MemoryPoolTest():UnitTest("TEST[base::memory::MemoryPoolTest]"){}
  void onRun() override;
  
};
  
}}}

#endif /* test_memory_MemoryPoolTest_hpp */