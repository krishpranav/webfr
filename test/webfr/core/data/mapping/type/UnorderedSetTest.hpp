#ifndef webfr_test_core_data_mapping_type_UnorderedSetTest_hpp
#define webfr_test_core_data_mapping_type_UnorderedSetTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

class UnorderedSetTest : public UnitTest{
public:

  UnorderedSetTest():UnitTest("TEST[core::data::mapping::type::UnorderedSetTest]"){}
  void onRun() override;

};

}}}}}}

#endif 
