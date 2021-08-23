#ifndef webfr_test_core_data_mapping_TypeResolverTest_hpp
#define webfr_test_core_data_mapping_TypeResolverTest_hpp

// includes
#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping {

class TypeResolverTest : public UnitTest{
public:

  TypeResolverTest():UnitTest("TEST[core::data::mapping::type::TypeResolverTest]"){}
  void onRun() override;

};

}}}}}

#endif 