#ifndef webfr_test_parser_json_mapping_DTOMapperTest_hpp
#define webfr_test_parser_json_mapping_DTOMapperTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {
  
class DTOMapperTest : public UnitTest{
public:
  
  DTOMapperTest():UnitTest("TEST[parser::json::mapping::DTOMapperTest]"){}
  void onRun() override;
  
};
  
}}}}}

#endif 