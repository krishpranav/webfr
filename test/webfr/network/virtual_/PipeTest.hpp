#ifndef webfr_test_network_virtual__PipeTest_hpp
#define webfr_test_network_virtual__PipeTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace network { namespace virtual_ {
  
class PipeTest : public UnitTest {
public:
  PipeTest():UnitTest("TEST[network::virtual_::PipeTest]"){}
  void onRun() override;
};
  
}}}}

#endif 