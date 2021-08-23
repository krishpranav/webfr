// includes
#include "TypeTest.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {
  
namespace {
  
#include WEBFR_CODEGEN_BEGIN(DTO)
  
  class TestDto : public webfr::DTO {
    
    DTO_INIT(TestDto, DTO);

    DTO_FIELD(String, field_string);
    DTO_FIELD(Int8, field_int8);
    DTO_FIELD(Int16, field_int16);
    DTO_FIELD(Int32, field_int32);
    DTO_FIELD(Int64, field_int64);
    DTO_FIELD(Float32, field_float32);
    DTO_FIELD(Float64, field_float64);
    DTO_FIELD(Boolean, field_boolean);
    
    DTO_FIELD(List<String>, field_list_string);
    DTO_FIELD(List<Int32>, field_list_int32);
    DTO_FIELD(List<Int64>, field_list_int64);
    DTO_FIELD(List<Float32>, field_list_float32);
    DTO_FIELD(List<Float64>, field_list_float64);
    DTO_FIELD(List<Boolean>, field_list_boolean);
    
    DTO_FIELD(Fields<String>, field_map_string_string);
    
    DTO_FIELD(Object<TestDto>, obj1);
    
  };
  
#include WEBFR_CODEGEN_END(DTO)
  
}
  
void TypeTest::onRun() {
  
  auto obj = TestDto::createShared();
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_string.valueType->classId.name);
  WEBFR_ASSERT(obj->field_string.valueType->classId.id == webfr::data::mapping::type::__class::String::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_int8.valueType->classId.name);
  WEBFR_ASSERT(obj->field_int8.valueType->classId.id == webfr::data::mapping::type::__class::Int8::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_int16.valueType->classId.name);
  WEBFR_ASSERT(obj->field_int16.valueType->classId.id == webfr::data::mapping::type::__class::Int16::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_int32.valueType->classId.name);
  WEBFR_ASSERT(obj->field_int32.valueType->classId.id == webfr::data::mapping::type::__class::Int32::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_int64.valueType->classId.name);
  WEBFR_ASSERT(obj->field_int64.valueType->classId.id == webfr::data::mapping::type::__class::Int64::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_float32.valueType->classId.name);
  WEBFR_ASSERT(obj->field_float32.valueType->classId.id == webfr::data::mapping::type::__class::Float32::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_float64.valueType->classId.name);
  WEBFR_ASSERT(obj->field_float64.valueType->classId.id == webfr::data::mapping::type::__class::Float64::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_boolean.valueType->classId.name);
  WEBFR_ASSERT(obj->field_boolean.valueType->classId.id == webfr::data::mapping::type::__class::Boolean::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_list_string.valueType->classId.name);
  WEBFR_ASSERT(obj->field_list_string.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_list_int32.valueType->classId.name);
  WEBFR_ASSERT(obj->field_list_int32.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_list_int64.valueType->classId.name);
  WEBFR_ASSERT(obj->field_list_int64.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_list_float32.valueType->classId.name);
  WEBFR_ASSERT(obj->field_list_float32.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_list_float64.valueType->classId.name);
  WEBFR_ASSERT(obj->field_list_float64.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_list_boolean.valueType->classId.name);
  WEBFR_ASSERT(obj->field_list_boolean.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->field_map_string_string.valueType->classId.name);
  WEBFR_ASSERT(obj->field_map_string_string.valueType->classId.id == webfr::data::mapping::type::__class::AbstractPairList::CLASS_ID.id);
  
  WEBFR_LOGV(TAG, "type: '%s'", obj->obj1.valueType->classId.name);
  WEBFR_ASSERT(obj->obj1.valueType->classId.id == webfr::data::mapping::type::__class::AbstractObject::CLASS_ID.id);

}
  
}}}}}}
