// includes
#include "TypeResolverTest.hpp"
#include "webfr/core/data/mapping/TypeResolver.hpp"
#include "webfr/core/Types.hpp"
#include "webfr/core/macro/codegen.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping {

namespace {

#include WEBFR_CODEGEN_BEGIN(DTO)

class TestDto : public webfr::DTO {

  DTO_INIT(TestDto, DTO)

  DTO_FIELD(String, f_str);

  DTO_FIELD(Int8, f_int8);
  DTO_FIELD(UInt8, f_uint8);

  DTO_FIELD(Int16, f_int16);
  DTO_FIELD(UInt16, f_uint16);

  DTO_FIELD(Int32, f_int32);
  DTO_FIELD(UInt32, f_uint32);

  DTO_FIELD(Int64, f_int64);
  DTO_FIELD(UInt64, f_uint64);

  DTO_FIELD(Float32, f_float32);
  DTO_FIELD(Float64, f_float64);

  DTO_FIELD(Boolean, f_bool);

  DTO_FIELD(Vector<String>, f_vector);
  DTO_FIELD(List<String>, f_list);
  DTO_FIELD(UnorderedSet<String>, f_set);

  DTO_FIELD(Fields<String>, f_fields);
  DTO_FIELD(UnorderedFields<String>, f_unordered_fields);

  DTO_FIELD(Any, f_any);

  DTO_FIELD(Object<TestDto>, f_dto);

};

#include WEBFR_CODEGEN_END(DTO)

}

void TypeResolverTest::onRun() {

  webfr::data::mapping::TypeResolver::Cache cache;
  webfr::data::mapping::TypeResolver tr;

  auto dto1 = TestDto::createShared();

  dto1->f_str = "hello dto1";

  dto1->f_int8 = 8;
  dto1->f_uint8 = 88;

  dto1->f_int16 = 16;
  dto1->f_uint16 = 1616;

  dto1->f_int32 = 32;
  dto1->f_uint32 = 3232;

  dto1->f_int64 = 64;
  dto1->f_uint64 = 6464;

  dto1->f_float32 = 0.32f;
  dto1->f_float64 = 0.64;

  dto1->f_bool = true;

  dto1->f_vector = {"hello", "world"};
  dto1->f_list = {"hello", "world"};
  dto1->f_set = {"hello", "world"};

  dto1->f_fields = {{"hello", "world"}};
  dto1->f_unordered_fields = {{"hello", "world"}};

  dto1->f_any = webfr::String("hey ANY!");

  dto1->f_dto = TestDto::createShared();


  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_str"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::String::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_str"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::String::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_str.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_str"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::String::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_str"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::String::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_str.get());

  }


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Int8

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_int8"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Int8::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_int8"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Int8::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_int8.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_int8"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Int8::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_int8"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Int8::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_int8.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_uint8"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::UInt8::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_uint8"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::UInt8::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_uint8.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_uint8"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::UInt8::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_uint8"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::UInt8::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_uint8.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Int16

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_int16"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Int16::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_int16"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Int16::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_int16.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_int16"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Int16::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_int16"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Int16::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_int16.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_uint16"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::UInt16::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_uint16"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::UInt16::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_uint16.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_uint16"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::UInt16::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_uint16"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::UInt16::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_uint16.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Int32

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_int32"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Int32::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_int32"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Int32::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_int32.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_int32"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Int32::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_int32"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Int32::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_int32.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_uint32"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::UInt32::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_uint32"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::UInt32::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_uint32.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_uint32"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::UInt32::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_uint32"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::UInt32::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_uint32.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Int64

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_int64"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Int64::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_int64"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Int64::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_int64.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_int64"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Int64::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_int64"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Int64::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_int64.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_uint64"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::UInt64::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_uint64"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::UInt64::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_uint64.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_uint64"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::UInt64::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_uint64"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::UInt64::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_uint64.get());

  }



  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Float32

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_float32"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Float32::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_float32"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Float32::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_float32.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_float32"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Float32::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_float32"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Float32::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_float32.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Float64

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_float64"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Float64::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_float64"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Float64::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_float64.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_float64"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Float64::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_float64"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Float64::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_float64.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Boolean

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_bool"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Boolean::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_bool"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Boolean::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_bool.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_bool"}, cache);
    WEBFR_ASSERT(type != nullptr);
    WEBFR_ASSERT(type->classId.id == webfr::Boolean::Class::CLASS_ID.id);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_bool"}, cache);
    WEBFR_ASSERT(val.valueType->classId.id == webfr::Boolean::Class::CLASS_ID.id);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_bool.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Vector

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_vector"}, cache);
    WEBFR_ASSERT(type == dto1->f_vector.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_vector"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_vector.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_vector.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_vector"}, cache);
    WEBFR_ASSERT(type == dto1->f_dto->f_vector.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_vector"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_dto->f_vector.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_vector.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // List

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_list"}, cache);
    WEBFR_ASSERT(type == dto1->f_list.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_list"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_list.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_list.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_list"}, cache);
    WEBFR_ASSERT(type == dto1->f_dto->f_list.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_list"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_dto->f_list.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_list.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Set

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_set"}, cache);
    WEBFR_ASSERT(type == dto1->f_set.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_set"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_set.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_set.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_set"}, cache);
    WEBFR_ASSERT(type == dto1->f_dto->f_set.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_set"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_dto->f_set.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_set.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Fields

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_fields"}, cache);
    WEBFR_ASSERT(type == dto1->f_fields.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_fields"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_fields.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_fields.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_fields"}, cache);
    WEBFR_ASSERT(type == dto1->f_dto->f_fields.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_fields"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_dto->f_fields.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_fields.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // UnorderedFields

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_unordered_fields"}, cache);
    WEBFR_ASSERT(type == dto1->f_unordered_fields.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_unordered_fields"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_unordered_fields.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_unordered_fields.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_unordered_fields"}, cache);
    WEBFR_ASSERT(type == dto1->f_dto->f_unordered_fields.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_unordered_fields"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_dto->f_unordered_fields.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_unordered_fields.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Any

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_any"}, cache);
    WEBFR_ASSERT(type == dto1->f_any.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_any"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_any.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_any.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_any"}, cache);
    WEBFR_ASSERT(type == dto1->f_dto->f_any.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_any"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_dto->f_any.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_any.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Dto

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto"}, cache);
    WEBFR_ASSERT(type == dto1->f_dto.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_dto.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_dto.get());

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_dto"}, cache);
    WEBFR_ASSERT(type == dto1->f_dto->f_dto.valueType);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_dto"}, cache);
    WEBFR_ASSERT(val.valueType == dto1->f_dto->f_dto.valueType);
    WEBFR_ASSERT(val.get() == dto1->f_dto->f_dto.get());

  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Non-Existing

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_non_existing"}, cache);
    WEBFR_ASSERT(type == nullptr);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_non_existing"}, cache);
    WEBFR_ASSERT(val.valueType == webfr::Void::Class::getType());
    WEBFR_ASSERT(val == nullptr);

  }

  {

    auto type = tr.resolveObjectPropertyType(webfr::Object<TestDto>::Class::getType(), {"f_dto", "f_non_existing"}, cache);
    WEBFR_ASSERT(type == nullptr);

    auto val = tr.resolveObjectPropertyValue(dto1, {"f_dto", "f_non_existing"}, cache);
    WEBFR_ASSERT(val.valueType == webfr::Void::Class::getType());
    WEBFR_ASSERT(val == nullptr);

  }

}

}}}}}