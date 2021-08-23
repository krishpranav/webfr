#ifndef webfr_test_parser_json_mapping_UnorderedSetTest_hpp
#define webfr_test_parser_json_mapping_UnorderedSetTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {

class UnorderedSetTest : public UnitTest{
public:

  UnorderedSetTest():UnitTest("TEST[parser::json::mapping::UnorderedSetTest]"){}
  void onRun() override;

};

}}}}}

#endif