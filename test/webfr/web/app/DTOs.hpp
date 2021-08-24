#ifndef webfr_test_web_app_DTOs_hpp
#define webfr_test_web_app_DTOs_hpp

#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace web { namespace app {

#include WEBFR_CODEGEN_BEGIN(DTO)

class TestDto : public webfr::DTO {
  
  DTO_INIT(TestDto, DTO)
  
  DTO_FIELD(String, testValue);
  DTO_FIELD(Int32, testValueInt);
  DTO_FIELD(Fields<String>, testMap);
  
};

ENUM(AllowedPathParams, v_int32,
  VALUE(HELLO, 100, "hello"),
  VALUE(WORLD, 200, "world")
)

#include WEBFR_CODEGEN_END(DTO)
  
}}}}

#endif 