#ifndef webfr_test_core_data_mapping_type_StringTest_hpp
#define webfr_test_core_data_mapping_type_StringTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

class StringTest : public UnitTest{
public:

  StringTest():UnitTest("TEST[core::data::mapping::type::StringTest]"){}
  void onRun() override;

};

}}}}}}

#endif 
