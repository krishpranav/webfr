#ifndef webfr_test_core_data_mapping_type_PrimitiveTest_hpp
#define webfr_test_core_data_mapping_type_PrimitiveTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

class PrimitiveTest : public UnitTest{
public:

  PrimitiveTest():UnitTest("TEST[core::data::mapping::type::PrimitiveTest]"){}
  void onRun() override;

};

}}}}}}

#endif 
