#include "DTOMapperTest.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/data/mapping/type/Object.hpp"
#include "webfr/core/data/mapping/type/List.hpp"
#include "webfr/core/data/mapping/type/Primitive.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include "webfr/core/macro/codegen.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {
  
namespace {
  
#include WEBFR_CODEGEN_BEGIN(DTO)
  
class TestChild : public webfr::DTO {
  
  DTO_INIT(TestChild, DTO)
  
  DTO_FIELD(String, name) = "Name";
  DTO_FIELD(String, secondName) = "Second Name";

public:

  TestChild(const char* pName, const char* pSecondName)
    : name(pName)
    , secondName(pSecondName)
  {}

};

class Test : public webfr::DTO {
  
  DTO_INIT(Test, DTO)
  
  DTO_FIELD(String, field_string, "string-field-name-qualifier");
  DTO_FIELD(Int32, field_int32, "int32-field-name-qualifier");
  DTO_FIELD(Int64, field_int64);
  DTO_FIELD(Float32, field_float32);
  DTO_FIELD(Float64, field_float64);
  DTO_FIELD(Boolean, field_boolean);
  
  DTO_FIELD(List<String>, field_list_string) = {};
  DTO_FIELD(List<Int32>, field_list_int32) = {};
  DTO_FIELD(List<Int64>, field_list_int64) = {};
  DTO_FIELD(List<Float32>, field_list_float32) = {};
  DTO_FIELD(List<Float64>, field_list_float64) = {};
  DTO_FIELD(List<Boolean>, field_list_boolean) = {};
  
  DTO_FIELD(List<Object<TestChild>>, field_list_object) = {};
  DTO_FIELD(List<List<Object<TestChild>>>, field_list_list_object) = {};

  DTO_FIELD(Vector<String>, field_vector);
  DTO_FIELD(Fields<String>, field_fields);
  DTO_FIELD(UnorderedFields<String>, field_unordered_fields);
  
  DTO_FIELD(Object<Test>, obj1);
  DTO_FIELD(Object<TestChild>, child1);
  
};

class TestAny : public webfr::DTO {

  DTO_INIT(TestAny, DTO)

  DTO_FIELD(List<Any>, anyList) = List<Any>::createShared();

};

class TestAnyNested : public webfr::DTO {

  DTO_INIT(TestAnyNested, DTO)

  DTO_FIELD(String, f1) = "Field_1";
  DTO_FIELD(String, f2) = "Field_2";

};
  
#include WEBFR_CODEGEN_END(DTO)
  
}
  
void DTOMapperTest::onRun(){
  
  auto mapper = webfr::parser::json::mapping::ObjectMapper::createShared();
  mapper->getSerializer()->getConfig()->useBeautifier = true;

  auto test1 = Test::createShared();
  
  test1->field_string = "string value";
  test1->field_int32 = 32;
  test1->field_int64 = 64;
  test1->field_float32 = 0.32f;
  test1->field_float64 = 0.64;
  test1->field_boolean = true;
  
  test1->obj1 = Test::createShared();
  test1->obj1->field_string = "inner string";
  test1->obj1->field_list_string->push_back("inner str_item_1");
  test1->obj1->field_list_string->push_back("inner str_item_2");
  test1->obj1->field_list_string->push_back("inner str_item_3");
  
  test1->child1 = TestChild::createShared();
  test1->child1->name = "child1_name";
  test1->child1->secondName = "child1_second_name";
  
  test1->field_list_string->push_back("str_item_1");
  test1->field_list_string->push_back("str_item_2");
  test1->field_list_string->push_back("str_item_3");
  
  test1->field_list_int32->push_back(321);
  test1->field_list_int32->push_back(322);
  test1->field_list_int32->push_back(323);
  
  test1->field_list_int64->push_back(641);
  test1->field_list_int64->push_back(642);
  test1->field_list_int64->push_back(643);
  
  test1->field_list_float32->push_back(0.321f);
  test1->field_list_float32->push_back(0.322f);
  test1->field_list_float32->push_back(0.323f);
  
  test1->field_list_float64->push_back(0.641);
  test1->field_list_float64->push_back(0.642);
  test1->field_list_float64->push_back(0.643);
  
  test1->field_list_boolean->push_back(true);
  test1->field_list_boolean->push_back(false);
  test1->field_list_boolean->push_back(true);
  
  test1->field_list_object->push_back(TestChild::createShared("child", "1"));
  test1->field_list_object->push_back(TestChild::createShared("child", "2"));
  test1->field_list_object->push_back(TestChild::createShared("child", "3"));
  
  auto l1 = webfr::List<webfr::Object<TestChild>>::createShared();
  auto l2 = webfr::List<webfr::Object<TestChild>>::createShared();
  auto l3 = webfr::List<webfr::Object<TestChild>>::createShared();
  
  l1->push_back(TestChild::createShared("list_1", "item_1"));
  l1->push_back(TestChild::createShared("list_1", "item_2"));
  l1->push_back(TestChild::createShared("list_1", "item_3"));
  
  l2->push_back(TestChild::createShared("list_2", "item_1"));
  l2->push_back(TestChild::createShared("list_2", "item_2"));
  l2->push_back(TestChild::createShared("list_2", "item_3"));
  
  l3->push_back(TestChild::createShared("list_3", "item_1"));
  l3->push_back(TestChild::createShared("list_3", "item_2"));
  l3->push_back(TestChild::createShared("list_3", "item_3"));
  
  test1->field_list_list_object->push_back(l1);
  test1->field_list_list_object->push_back(l2);
  test1->field_list_list_object->push_back(l3);

  test1->field_vector = {"vector_item1", "vector_item2", "vector_item3"};

  test1->field_fields = {
    {"key0", "pair_item0"},
    {"key1", "pair_item1"},
    {"key2", "pair_item2"},
    {"key3", "pair_item3"},
    {"key4", "pair_item4"},
    {"key5", "pair_item5"},
    {"key6", "pair_item6"},
    {"key7", "pair_item7"},
    {"key8", "pair_item8"},
    {"key9", "pair_item9"},
    {"key10", "pair_item10"},
    {"key11", "pair_item11"}
  };

  test1->field_unordered_fields = {
    {"key0", "map_item0"},
    {"key1", "map_item1"},
    {"key2", "map_item2"},
    {"key3", "map_item3"},
    {"key4", "map_item4"},
    {"key5", "map_item5"},
    {"key6", "map_item6"},
    {"key7", "map_item7"},
    {"key8", "map_item8"},
    {"key9", "map_item9"},
    {"key10", "map_item10"},
    {"key11", "map_item11"}
  };
  
  auto result = mapper->writeToString(test1);
  
  WEBFR_LOGV(TAG, "json='%s'", (const char*) result->getData());
  
  WEBFR_LOGV(TAG, "...");
  WEBFR_LOGV(TAG, "...");
  WEBFR_LOGV(TAG, "...");

  webfr::parser::Caret caret(result);
  auto obj = mapper->readFromCaret<webfr::Object<Test>>(caret);
  
  WEBFR_ASSERT(obj->field_string);
  WEBFR_ASSERT(obj->field_string == test1->field_string);
  
  WEBFR_ASSERT(obj->field_int32);
  WEBFR_ASSERT(obj->field_int32 == test1->field_int32);
  
  WEBFR_ASSERT(obj->field_int64);
  WEBFR_ASSERT(obj->field_int64 == test1->field_int64);
  
  WEBFR_ASSERT(obj->field_float32);
  WEBFR_ASSERT(obj->field_float32 == test1->field_float32);
  
  WEBFR_ASSERT(obj->field_float64);
  WEBFR_ASSERT(obj->field_float64 == test1->field_float64);
  
  WEBFR_ASSERT(obj->field_boolean);
  WEBFR_ASSERT(obj->field_boolean == test1->field_boolean);

  {
    auto c = obj->field_vector;
    WEBFR_ASSERT(c[0] == "vector_item1");
    WEBFR_ASSERT(c[1] == "vector_item2");
    WEBFR_ASSERT(c[2] == "vector_item3");
  }

  {
    auto c = obj->field_fields;
    v_int32 i = 0;
    for(auto& pair : *c) {
      WEBFR_ASSERT(pair.first == "key" + webfr::utils::conversion::int32ToStr(i));
      WEBFR_ASSERT(pair.second == "pair_item" + webfr::utils::conversion::int32ToStr(i));
      i++;
    }
  }

  {
    auto c = obj->field_unordered_fields;
    WEBFR_ASSERT(c["key1"] == "map_item1");
    WEBFR_ASSERT(c["key2"] == "map_item2");
    WEBFR_ASSERT(c["key3"] == "map_item3");
  }
  
  result = mapper->writeToString(obj);
  
  WEBFR_LOGV(TAG, "json='%s'", (const char*) result->getData());

  {

    auto obj = TestAny::createShared();
    obj->anyList = {
      webfr::String("Hello Any!!!"),
      webfr::Int32(32),
      webfr::Int64(64),
      webfr::Float64(0.64),
      webfr::Float64(0.64),
      TestAnyNested::createShared()
    };

    auto map = webfr::Fields<Any>::createShared();
    map["bool-field"] = webfr::Boolean(false);
    map["vector"] = webfr::Vector<webfr::String>({"vector_v1", "vector_v2", "vector_v3"});
    map["unordered_map"] = webfr::UnorderedFields<webfr::String>({{"key1", "value1"}, {"key2", "value2"}});

    obj->anyList->push_back(map);

    auto json = mapper->writeToString(obj);
    WEBFR_LOGV(TAG, "any json='%s'", (const char*) json->getData());

    auto deserializedAny = mapper->readFromString<webfr::Fields<webfr::Any>>(json);

    auto json2 = mapper->writeToString(deserializedAny);
    WEBFR_LOGV(TAG, "any json='%s'", (const char*) json2->getData());

  }

}
  
#include WEBFR_CODEGEN_END(DTO)

}}}}}