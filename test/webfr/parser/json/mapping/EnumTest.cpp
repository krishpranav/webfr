#include "EnumTest.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/macro/codegen.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {

namespace {

#include WEBFR_CODEGEN_BEGIN(DTO)

ENUM(Enum0, v_int32);

ENUM(Enum1, v_int32,
  VALUE(V1, 10, "enum1-v1"),
  VALUE(V2, 20, "enum1-v2"),
  VALUE(V3, 30, "enum1-v3")
);

class DTO1 : public webfr::DTO {

  DTO_INIT(DTO1, DTO)

  DTO_FIELD(Enum<Enum1>::AsString, enum1);

};

#include WEBFR_CODEGEN_END(DTO)

}

void EnumTest::onRun() {

  webfr::parser::json::mapping::ObjectMapper mapper;

  {
    WEBFR_LOGI(TAG, "Serializer as string...");
    webfr::Fields<webfr::Enum<Enum1>::AsString> map = {{"enum", Enum1::V1}};
    auto json = mapper.writeToString(map);
    WEBFR_LOGD(TAG, "json='%s'", json->c_str());
    WEBFR_ASSERT(json == "{\"enum\":\"enum1-v1\"}");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Serializer as string null...");
    webfr::Fields<webfr::Enum<Enum1>::AsString> map = {{"enum", nullptr}};
    auto json = mapper.writeToString(map);
    WEBFR_LOGD(TAG, "json='%s'", json->c_str());
    WEBFR_ASSERT(json == "{\"enum\":null}");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Serializer as string error on null...");
    bool error = false;
    webfr::Fields<webfr::Enum<Enum1>::AsString::NotNull> map = {{"enum", nullptr}};
    try {
      auto json = mapper.writeToString(map);
    } catch (const std::runtime_error& e) {
      WEBFR_LOGD(TAG, "error - %s", e.what());
      error = true;
    }
    WEBFR_ASSERT(error == true);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Serializer as int...");
    webfr::Fields<webfr::Enum<Enum1>::AsNumber> map = {{"enum", Enum1::V1}};
    auto json = mapper.writeToString(map);
    WEBFR_LOGD(TAG, "json='%s'", json->c_str());
    WEBFR_ASSERT(json == "{\"enum\":10}");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Serializer as int null...");
    webfr::Fields<webfr::Enum<Enum1>::AsNumber> map = {{"enum", nullptr}};
    auto json = mapper.writeToString(map);
    WEBFR_LOGD(TAG, "json='%s'", json->c_str());
    WEBFR_ASSERT(json == "{\"enum\":null}");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Serializer as int error on null...");
    bool error = false;
    webfr::Fields<webfr::Enum<Enum1>::AsNumber::NotNull> map = {{"enum", nullptr}};
    try {
      auto json = mapper.writeToString(map);
    } catch (const std::runtime_error& e) {
      WEBFR_LOGD(TAG, "error - %s", e.what());
      error = true;
    }
    WEBFR_ASSERT(error == true);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Deserializer as string...");
    webfr::String json = "{\"enum\":\"enum1-v2\"}";
    auto map = mapper.readFromString<webfr::Fields<webfr::Enum<Enum1>::AsString>>(json);
    WEBFR_ASSERT(map["enum"] == Enum1::V2);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Deserializer as string null...");
    webfr::String json = "{\"enum\":null}";
    auto map = mapper.readFromString<webfr::Fields<webfr::Enum<Enum1>::AsString>>(json);
    WEBFR_ASSERT(map["enum"] == nullptr);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Deserializer as string error on null...");
    bool error = false;
    webfr::String json = "{\"enum\":null}";
    try {
      auto map = mapper.readFromString<webfr::Fields<webfr::Enum<Enum1>::AsString::NotNull>>(json);
    } catch (const webfr::parser::ParsingError& e) {
      WEBFR_LOGD(TAG, "error - %s", e.what());
      error = true;
    }
    WEBFR_ASSERT(error == true);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Deserializer as int...");
    webfr::String json = "{\"enum\":20}";
    auto map = mapper.readFromString<webfr::Fields<webfr::Enum<Enum1>::AsNumber>>(json);
    WEBFR_ASSERT(map["enum"] == Enum1::V2);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Deserializer as int null...");
    webfr::String json = "{\"enum\":null}";
    auto map = mapper.readFromString<webfr::Fields<webfr::Enum<Enum1>::AsNumber>>(json);
    WEBFR_ASSERT(map["enum"] == nullptr);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Deserializer as int error on null...");
    bool error = false;
    webfr::String json = "{\"enum\":null}";
    try {
      auto map = mapper.readFromString<webfr::Fields<webfr::Enum<Enum1>::AsNumber::NotNull>>(json);
    } catch (const webfr::parser::ParsingError& e) {
      WEBFR_LOGD(TAG, "error - %s", e.what());
      error = true;
    }
    WEBFR_ASSERT(error == true);
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}