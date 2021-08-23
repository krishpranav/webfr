#ifndef webfr_test_core_data_share_LazyStringMapTest_hpp
#define webfr_test_core_data_share_LazyStringMapTest_hpp

// includes
#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace share {

class LazyStringMapTest : public UnitTest{
public:

  LazyStringMapTest():UnitTest("TEST[core::data::share::LazyStringMapTest]"){}
  void onRun() override;

};

}}}}}

#endif 