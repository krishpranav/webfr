#ifndef webfr_test_base_CaretTest_hpp
#define webfr_test_base_CaretTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace parser {

class CaretTest : public UnitTest{
public:

  CaretTest():UnitTest("TEST[parser::CaretTest]"){}
  void onRun() override;

};

}}}


#endif 