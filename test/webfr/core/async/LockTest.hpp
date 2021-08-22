#ifndef webfr_test_async_LockTest_hpp
#define webfr_test_async_LockTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace async {

class LockTest : public UnitTest{
public:

  LockTest():UnitTest("TEST[async::LockTest]"){}
  void onRun() override;

};

}}}

#endif 