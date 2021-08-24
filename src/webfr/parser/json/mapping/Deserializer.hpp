#ifndef webfr_parser_json_mapping_Deserializer_hpp
#define webfr_parser_json_mapping_Deserializer_hpp

#include "webfr/parser/json/Utils.hpp"
#include "webfr/core/parser/Caret.hpp"
#include "webfr/core/Types.hpp"

#include <vector>

namespace webfr { namespace parser { namespace json { namespace mapping {

class Deserializer {
public:
  typedef webfr::data::mapping::type::Type Type;
  typedef webfr::data::mapping::type::BaseObject::Property Property;
  typedef webfr::data::mapping::type::BaseObject::Properties Properties;

  typedef webfr::String String;

public:

  static constexpr v_int32 ERROR_CODE_OBJECT_SCOPE_OPEN = 1;

  static constexpr v_int32 ERROR_CODE_OBJECT_SCOPE_CLOSE = 2;

  static constexpr v_int32 ERROR_CODE_OBJECT_SCOPE_UNKNOWN_FIELD = 3;

  static constexpr v_int32 ERROR_CODE_OBJECT_SCOPE_COLON_MISSING = 4;

  static constexpr v_int32 ERROR_CODE_ARRAY_SCOPE_OPEN = 5;

  static constexpr v_int32 ERROR_CODE_ARRAY_SCOPE_CLOSE = 6;

  static constexpr v_int32 ERROR_CODE_VALUE_BOOLEAN = 7;

public:

  class Config : public webfr::base::Countable {
  public:

    Config()
    {}
  public:

    static std::shared_ptr<Config> createShared(){
      return std::make_shared<Config>();
    }

    bool allowUnknownFields = true;

    std::vector<std::string> enabledInterpretations = {};

  };

public:
  typedef webfr::Void (*DeserializerMethod)(Deserializer*, parser::Caret&, const Type* const);
private:
  static void skipScope(webfr::parser::Caret& caret, v_char8 charOpen, v_char8 charClose);
  static void skipString(webfr::parser::Caret& caret);
  static void skipToken(webfr::parser::Caret& caret);
  static void skipValue(webfr::parser::Caret& caret);
private:
  static const Type* guessNumberType(webfr::parser::Caret& caret);
  static const Type* guessType(webfr::parser::Caret& caret);
private:

  template<class T>
  static webfr::Void deserializeInt(Deserializer* deserializer, parser::Caret& caret, const Type* const type){

    (void) deserializer;
    (void) type;

    if(caret.isAtText("null", true)){
      return webfr::Void(T::Class::getType());
    } else {
      return T(static_cast<typename T::UnderlyingType>(caret.parseInt()));
    }

  }

  template<class T>
  static webfr::Void deserializeUInt(Deserializer* deserializer, parser::Caret& caret, const Type* const type){

    (void) deserializer;
    (void) type;

    if(caret.isAtText("null", true)){
      return webfr::Void(T::Class::getType());
    } else {

      return T(static_cast<typename T::UnderlyingType>(caret.parseUnsignedInt()));
    }

  }

  template<class Collection>
  static webfr::Void deserializeList(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {

    if(caret.isAtText("null", true)){
      return webfr::Void(type);
    }

    if(caret.canContinueAtChar('[', 1)) {

      auto polymorphicDispatcher = static_cast<const typename Collection::Class::PolymorphicDispatcher*>(type->polymorphicDispatcher);
      auto listWrapper = polymorphicDispatcher->createObject();
      const auto& list = listWrapper.template staticCast<Collection>();

      auto itemType = *type->params.begin();

      caret.skipBlankChars();

      while(!caret.isAtChar(']') && caret.canContinue()){

        caret.skipBlankChars();
        auto item = deserializer->deserialize(caret, itemType);
        if(caret.hasError()){
          return nullptr;
        }

        polymorphicDispatcher->addPolymorphicItem(listWrapper, item);
        caret.skipBlankChars();

        caret.canContinueAtChar(',', 1);

      }

      if(!caret.canContinueAtChar(']', 1)){
        if(!caret.hasError()){
          caret.setError("[webfr::parser::json::mapping::Deserializer::deserializeList()]: Error. ']' - expected", ERROR_CODE_ARRAY_SCOPE_CLOSE);
        }
        return nullptr;
      };

      return webfr::Void(list.getPtr(), list.valueType);
    } else {
      caret.setError("[webfr::parser::json::mapping::Deserializer::deserializeList()]: Error. '[' - expected", ERROR_CODE_ARRAY_SCOPE_OPEN);
      return nullptr;
    }

  }

  template<class Collection>
  static webfr::Void deserializeKeyValue(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {

    if(caret.isAtText("null", true)){
      return webfr::Void(type);
    }

    if(caret.canContinueAtChar('{', 1)) {

      auto polymorphicDispatcher = static_cast<const typename Collection::Class::PolymorphicDispatcher*>(type->polymorphicDispatcher);
      auto mapWrapper = polymorphicDispatcher->createObject();
      const auto& map = mapWrapper.template staticCast<Collection>();

      auto it = type->params.begin();
      auto keyType = *it ++;
      if(keyType->classId.id != webfr::data::mapping::type::__class::String::CLASS_ID.id){
        throw std::runtime_error("[webfr::parser::json::mapping::Deserializer::deserializeKeyValue()]: Invalid json map key. Key should be String");
      }
      auto valueType = *it;

      caret.skipBlankChars();

      while (!caret.isAtChar('}') && caret.canContinue()) {

        caret.skipBlankChars();
        auto key = Utils::parseString(caret);
        if(caret.hasError()){
          return nullptr;
        }

        caret.skipBlankChars();
        if(!caret.canContinueAtChar(':', 1)){
          caret.setError("[webfr::parser::json::mapping::Deserializer::deserializeKeyValue()]: Error. ':' - expected", ERROR_CODE_OBJECT_SCOPE_COLON_MISSING);
          return nullptr;
        }

        caret.skipBlankChars();

        auto item = deserializer->deserialize(caret, valueType);
        if(caret.hasError()){
          return nullptr;
        }
        polymorphicDispatcher->addPolymorphicItem(mapWrapper, key, item);

        caret.skipBlankChars();
        caret.canContinueAtChar(',', 1);

      }

      if(!caret.canContinueAtChar('}', 1)){
        if(!caret.hasError()){
          caret.setError("[webfr::parser::json::mapping::Deserializer::deserializeKeyValue()]: Error. '}' - expected", ERROR_CODE_OBJECT_SCOPE_CLOSE);
        }
        return nullptr;
      }

      return webfr::Void(map.getPtr(), map.valueType);

    } else {
      caret.setError("[webfr::parser::json::mapping::Deserializer::deserializeKeyValue()]: Error. '{' - expected", ERROR_CODE_OBJECT_SCOPE_OPEN);
    }

    return nullptr;

  }

  static webfr::Void deserializeFloat32(Deserializer* deserializer, parser::Caret& caret, const Type* const type);
  static webfr::Void deserializeFloat64(Deserializer* deserializer, parser::Caret& caret, const Type* const type);
  static webfr::Void deserializeBoolean(Deserializer* deserializer, parser::Caret& caret, const Type* const type);
  static webfr::Void deserializeString(Deserializer* deserializer, parser::Caret& caret, const Type* const type);
  static webfr::Void deserializeAny(Deserializer* deserializer, parser::Caret& caret, const Type* const type);
  static webfr::Void deserializeEnum(Deserializer* deserializer, parser::Caret& caret, const Type* const type);
  static webfr::Void deserializeObject(Deserializer* deserializer, parser::Caret& caret, const Type* const type);

private:
  std::shared_ptr<Config> m_config;
  std::vector<DeserializerMethod> m_methods;
public:

  Deserializer(const std::shared_ptr<Config>& config = std::make_shared<Config>());

  void setDeserializerMethod(const data::mapping::type::ClassId& classId, DeserializerMethod method);

  webfr::Void deserialize(parser::Caret& caret, const Type* const type);

  const std::shared_ptr<Config>& getConfig();

};

}}}}

#endif 