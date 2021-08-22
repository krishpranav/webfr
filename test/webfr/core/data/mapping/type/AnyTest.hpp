#ifndef webfr_test_core_data_mapping_type_AnyTest_hpp
#define webfr_test_core_data_mapping_type_AnyTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

class AnyTest : public UnitTest{
public:

  AnyTest():UnitTest("TEST[core::data::mapping::type::AnyTest]"){}
  void onRun() override;

};

}}}}}}

#endif 
