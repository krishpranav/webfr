#ifndef webfr_test_core_data_mapping_type_ObjectTest_hpp
#define webfr_test_core_data_mapping_type_ObjectTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

class ObjectTest : public UnitTest{
public:

  ObjectTest():UnitTest("TEST[core::data::mapping::type::ObjectTest]"){}
  void onRun() override;

};

}}}}}}

#endif 
