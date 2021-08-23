#ifndef webfr_test_parser_json_mapping_EnumTest_hpp
#define webfr_test_parser_json_mapping_EnumTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {

class EnumTest : public UnitTest{
public:

  EnumTest():UnitTest("TEST[parser::json::mapping::EnumTest]"){}
  void onRun() override;

};

}}}}}

#endif 