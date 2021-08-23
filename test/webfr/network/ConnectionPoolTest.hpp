#ifndef webfr_test_network_ConnectionPoolTest_hpp
#define webfr_test_network_ConnectionPoolTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace network {

class ConnectionPoolTest : public UnitTest {
public:

  ConnectionPoolTest():UnitTest("TEST[network::ConnectionPoolTest]"){}
  void onRun() override;

};

}}}


#endif 