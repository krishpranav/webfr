#ifndef webfr_test_parser_json_mapping_DeserializerTest_hpp
#define webfr_test_parser_json_mapping_DeserializerTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {
  
class DeserializerTest : public UnitTest{
public:
  
  DeserializerTest():UnitTest("TEST[parser::json::mapping::DeserializerTest]"){}
  void onRun() override;
  
};
  
}}}}}

#endif 