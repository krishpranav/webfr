#ifndef webfr_test_core_data_share_StringTemplateTest_hpp
#define webfr_test_core_data_share_StringTemplateTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace share {

class StringTemplateTest : public UnitTest{
public:

  StringTemplateTest():UnitTest("TEST[core::data::share::StringTemplateTest]"){}
  void onRun() override;

};

}}}}}

#endif