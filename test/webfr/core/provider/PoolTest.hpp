#ifndef webfr_test_provider_PoolTest_hpp
#define webfr_test_provider_PoolTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace provider {

class PoolTest : public UnitTest{
public:

  PoolTest():UnitTest("TEST[provider::PoolTest]"){}
  void onRun() override;

};

}}}}


#endif 