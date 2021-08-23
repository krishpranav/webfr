#ifndef webfr_test_parser_json_mapping_DTOMapperPerfTest_hpp
#define webfr_test_parser_json_mapping_DTOMapperPerfTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {
  
class DTOMapperPerfTest : public UnitTest{
public:
  
  DTOMapperPerfTest():UnitTest("TEST[parser::json::mapping::DTOMapperPerfTest]"){}
  void onRun() override;
  
};
  
}}}}}

#endif 