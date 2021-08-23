#ifndef webfr_test_network_UrlTest_hpp
#define webfr_test_network_UrlTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace network {

class UrlTest : public UnitTest {
public:

  UrlTest():UnitTest("TEST[network::UrlTest]"){}
  void onRun() override;

};

}}}


#endif 