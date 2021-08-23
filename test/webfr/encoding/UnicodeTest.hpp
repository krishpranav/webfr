#ifndef webfr_test_encoding_UnicodeTest_hpp
#define webfr_test_encoding_UnicodeTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace encoding {
  
class UnicodeTest : public UnitTest{
public:
  UnicodeTest():UnitTest("TEST[encoding::UnicodeTest]"){}
  void onRun() override;
};
  
}}}


#endif 