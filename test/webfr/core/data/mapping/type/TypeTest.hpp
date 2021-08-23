#ifndef webfr_test_core_data_mapping_type_TypeTest_hpp
#define webfr_test_core_data_mapping_type_TypeTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {
  
class TypeTest : public UnitTest{
public:
  
  TypeTest():UnitTest("TEST[core::data::mapping::type::TypeTest]"){}
  void onRun() override;
  
};
  
}}}}}}

#endif
