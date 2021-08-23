#include "DTOMapperPerfTest.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/parser/json/mapping/Serializer.hpp"
#include "webfr/parser/json/mapping/Deserializer.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"
#include "webfr/core/macro/basic.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr-test/Checker.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {
  
namespace {

typedef webfr::parser::json::mapping::Serializer Serializer;
typedef webfr::parser::json::mapping::Deserializer Deserializer;

#include WEBFR_CODEGEN_BEGIN(DTO)
  
  class Test1 : public webfr::DTO {
    
    DTO_INIT(Test1, DTO)
    
    DTO_FIELD(String, field_string);
    DTO_FIELD(Int32, field_int32);
    DTO_FIELD(List<Int32>, field_list);
    
    static Wrapper createTestInstance(){
      auto result = Test1::createShared();
      result->field_string = "String Field";
      result->field_int32 = 5;
      result->field_list = List<Int32>::createShared();
      result->field_list->push_back(1);
      result->field_list->push_back(2);
      result->field_list->push_back(3);
      return result;
    }
    
  };
  
#include WEBFR_CODEGEN_END(DTO)
  
}
  
void DTOMapperPerfTest::onRun() {
  
  v_int32 numIterations = 1000000;

  auto serializer2 = std::make_shared<webfr::parser::json::mapping::Serializer>();
  auto mapper = webfr::parser::json::mapping::ObjectMapper::createShared();
  
  auto test1 = Test1::createTestInstance();
  auto test1_Text = mapper->writeToString(test1);
  WEBFR_LOGV(TAG, "json='%s'", (const char*) test1_Text->getData());

  {
    PerformanceChecker checker("Serializer");
    for(v_int32 i = 0; i < numIterations; i ++) {
      mapper->writeToString(test1);
    }
  }
  
  {
    PerformanceChecker checker("Deserializer");
    webfr::parser::Caret caret(test1_Text);
    for(v_int32 i = 0; i < numIterations; i ++) {
      caret.setPosition(0);
      mapper->readFromCaret<webfr::Object<Test1>>(caret);
    }
  }

}
  
}}}}}