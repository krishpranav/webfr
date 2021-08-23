#include "DeserializerTest.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/macro/codegen.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {

namespace {

#include WEBFR_CODEGEN_BEGIN(DTO)

typedef webfr::parser::Caret ParsingCaret;
typedef webfr::parser::json::mapping::Serializer Serializer;
typedef webfr::parser::json::mapping::Deserializer Deserializer;

class EmptyDto : public webfr::DTO {

  DTO_INIT(EmptyDto, DTO)

};

class Test1 : public webfr::DTO {
  
  DTO_INIT(Test1, DTO)
  
  DTO_FIELD(String, strF);
  
};
  
class Test2 : public webfr::DTO {
  
  DTO_INIT(Test2, DTO)
  
  DTO_FIELD(Int32, int32F);
  
};
  
class Test3 : public webfr::DTO {
  
  DTO_INIT(Test3, DTO)
  
  DTO_FIELD(Float32, float32F);
  
};

class Test4 : public webfr::DTO {

  DTO_INIT(Test4, DTO)

  DTO_FIELD(Object<EmptyDto>, object);
  DTO_FIELD(List<Object<EmptyDto>>, list);
  DTO_FIELD(Fields<Object<EmptyDto>>, map);

};
  
#include WEBFR_CODEGEN_END(DTO)
  
}
  
void DeserializerTest::onRun(){
  
  auto mapper = webfr::parser::json::mapping::ObjectMapper::createShared();
  
  auto obj1 = mapper->readFromString<webfr::Object<Test1>>("{}");
  
  WEBFR_ASSERT(obj1);
  WEBFR_ASSERT(!obj1->strF);
  
  obj1 = mapper->readFromString<webfr::Object<Test1>>("{\"strF\":\"value1\"}");
  
  WEBFR_ASSERT(obj1);
  WEBFR_ASSERT(obj1->strF);
  WEBFR_ASSERT(obj1->strF->equals("value1"));
  
  obj1 = mapper->readFromString<webfr::Object<Test1>>("{\n\r\t\f\"strF\"\n\r\t\f:\n\r\t\f\"value1\"\n\r\t\f}");
  
  WEBFR_ASSERT(obj1);
  WEBFR_ASSERT(obj1->strF);
  WEBFR_ASSERT(obj1->strF->equals("value1"));
  
  auto obj2 = mapper->readFromString<webfr::Object<Test2>>("{\"int32F\": null}");
  
  WEBFR_ASSERT(obj2);
  WEBFR_ASSERT(!obj2->int32F);
  
  obj2 = mapper->readFromString<webfr::Object<Test2>>("{\"int32F\": 32}");
  
  WEBFR_ASSERT(obj2);
  WEBFR_ASSERT(obj2->int32F == 32);
  
  obj2 = mapper->readFromString<webfr::Object<Test2>>("{\"int32F\":    -32}");
  
  WEBFR_ASSERT(obj2);
  WEBFR_ASSERT(obj2->int32F == -32);
  
  auto obj3 = mapper->readFromString<webfr::Object<Test3>>("{\"float32F\": null}");
  
  WEBFR_ASSERT(obj3);
  WEBFR_ASSERT(!obj3->float32F);
  
  obj3 = mapper->readFromString<webfr::Object<Test3>>("{\"float32F\": 32}");
  
  WEBFR_ASSERT(obj3);
  WEBFR_ASSERT(obj3->float32F == 32);
  
  obj3 = mapper->readFromString<webfr::Object<Test3>>("{\"float32F\": 1.32e1}");
  
  WEBFR_ASSERT(obj3);
  WEBFR_ASSERT(obj3->float32F);
  
  obj3 = mapper->readFromString<webfr::Object<Test3>>("{\"float32F\": 1.32e+1 }");
  
  WEBFR_ASSERT(obj3);
  WEBFR_ASSERT(obj3->float32F);
  
  obj3 = mapper->readFromString<webfr::Object<Test3>>("{\"float32F\": 1.32e-1 }");
  
  WEBFR_ASSERT(obj3);
  WEBFR_ASSERT(obj3->float32F);
  
  obj3 = mapper->readFromString<webfr::Object<Test3>>("{\"float32F\": -1.32E-1 }");
  
  WEBFR_ASSERT(obj3);
  WEBFR_ASSERT(obj3->float32F);
  
  obj3 = mapper->readFromString<webfr::Object<Test3>>("{\"float32F\": -1.32E1 }");
  
  WEBFR_ASSERT(obj3);
  WEBFR_ASSERT(obj3->float32F);
  
  auto list = mapper->readFromString<webfr::List<webfr::Int32>>("[1, 2, 3]");
  WEBFR_ASSERT(list);
  WEBFR_ASSERT(list->size() == 3);
  WEBFR_ASSERT(list[0] == 1);
  WEBFR_ASSERT(list[1] == 2);
  WEBFR_ASSERT(list[2] == 3);

  // Empty test

  auto obj4 = mapper->readFromString<webfr::Object<Test4>>("{\"object\": {}, \"list\": [], \"map\": {}}");
  WEBFR_ASSERT(obj4);
  WEBFR_ASSERT(obj4->object);
  WEBFR_ASSERT(obj4->list);
  WEBFR_ASSERT(obj4->list->size() == 0);
  WEBFR_ASSERT(obj4->map->size() == 0);

  obj4 = mapper->readFromString<webfr::Object<Test4>>("{\"object\": {\n\r\t}, \"list\": [\n\r\t], \"map\": {\n\r\t}}");
  WEBFR_ASSERT(obj4);
  WEBFR_ASSERT(obj4->object);
  WEBFR_ASSERT(obj4->list);
  WEBFR_ASSERT(obj4->list->size() == 0);
  WEBFR_ASSERT(obj4->map->size() == 0);

}
  
}}}}}