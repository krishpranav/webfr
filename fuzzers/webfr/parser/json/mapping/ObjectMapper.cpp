#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/macro/codegen.hpp"

typedef webfr::parser::Caret ParsingCaret;
typedef webfr::parser::json::mapping::Serializer Serializer;
typedef webfr::parser::json::mapping::Deserializer Deserializer;

#include WEBFR_CODEGEN_BEGIN(DTO)

class EmptyDto : public webfr::DTO {
  DTO_INIT(EmptyDto, DTO)
};

class Test1 : public webfr::DTO {
  DTO_INIT(Test1, DTO)
  DTO_FIELD(String, strF);
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  webfr::String input(reinterpret_cast<const char*>(data), size, true);
  webfr::parser::json::mapping::ObjectMapper mapper;
  try {
    mapper.readFromString<webfr::Object<Test1>>(input);
  } catch(...) {}

  return 0;
}