#ifndef webfr_test_encoding_Base64Test_hpp
#define webfr_test_encoding_Base64Test_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace encoding {
  
class Base64Test : public UnitTest{
public:
  Base64Test():UnitTest("TEST[encoding::Base64Test]"){}
  void onRun() override;
};
  
}}}

#endif 