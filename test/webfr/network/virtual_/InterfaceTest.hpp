#ifndef webfr_test_network_virtual__InterfaceTest_hpp
#define webfr_test_network_virtual__InterfaceTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace network { namespace virtual_ {
  
class InterfaceTest : public UnitTest {
public:
  InterfaceTest():UnitTest("TEST[network::virtual_::InterfaceTest]"){}
  void onRun() override;
};
  
}}}}

#endif 