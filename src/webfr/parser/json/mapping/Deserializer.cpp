#include "Deserializer.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"

namespace webfr { namespace parser { namespace json { namespace mapping {

Deserializer::Deserializer(const std::shared_ptr<Config>& config)
  : m_config(config)
{

  m_methods.resize(data::mapping::type::ClassId::getClassCount(), nullptr);

  setDeserializerMethod(data::mapping::type::__class::String::CLASS_ID, &Deserializer::deserializeString);
  setDeserializerMethod(data::mapping::type::__class::Any::CLASS_ID, &Deserializer::deserializeAny);

  setDeserializerMethod(data::mapping::type::__class::Int8::CLASS_ID, &Deserializer::deserializeInt<webfr::Int8>);
  setDeserializerMethod(data::mapping::type::__class::UInt8::CLASS_ID, &Deserializer::deserializeUInt<webfr::UInt8>);

  setDeserializerMethod(data::mapping::type::__class::Int16::CLASS_ID, &Deserializer::deserializeInt<webfr::Int16>);
  setDeserializerMethod(data::mapping::type::__class::UInt16::CLASS_ID, &Deserializer::deserializeUInt<webfr::UInt16>);

  setDeserializerMethod(data::mapping::type::__class::Int32::CLASS_ID, &Deserializer::deserializeInt<webfr::Int32>);
  setDeserializerMethod(data::mapping::type::__class::UInt32::CLASS_ID, &Deserializer::deserializeUInt<webfr::UInt32>);

  setDeserializerMethod(data::mapping::type::__class::Int64::CLASS_ID, &Deserializer::deserializeInt<webfr::Int64>);
  setDeserializerMethod(data::mapping::type::__class::UInt64::CLASS_ID, &Deserializer::deserializeUInt<webfr::UInt64>);

  setDeserializerMethod(data::mapping::type::__class::Float32::CLASS_ID, &Deserializer::deserializeFloat32);
  setDeserializerMethod(data::mapping::type::__class::Float64::CLASS_ID, &Deserializer::deserializeFloat64);
  setDeserializerMethod(data::mapping::type::__class::Boolean::CLASS_ID, &Deserializer::deserializeBoolean);

  setDeserializerMethod(data::mapping::type::__class::AbstractObject::CLASS_ID, &Deserializer::deserializeObject);
  setDeserializerMethod(data::mapping::type::__class::AbstractEnum::CLASS_ID, &Deserializer::deserializeEnum);

  setDeserializerMethod(data::mapping::type::__class::AbstractVector::CLASS_ID, &Deserializer::deserializeList<webfr::AbstractVector>);
  setDeserializerMethod(data::mapping::type::__class::AbstractList::CLASS_ID, &Deserializer::deserializeList<webfr::AbstractList>);
  setDeserializerMethod(data::mapping::type::__class::AbstractUnorderedSet::CLASS_ID, &Deserializer::deserializeList<webfr::AbstractUnorderedSet>);

  setDeserializerMethod(data::mapping::type::__class::AbstractPairList::CLASS_ID, &Deserializer::deserializeKeyValue<webfr::AbstractFields>);
  setDeserializerMethod(data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID, &Deserializer::deserializeKeyValue<webfr::AbstractUnorderedFields>);

}

void Deserializer::setDeserializerMethod(const data::mapping::type::ClassId& classId, DeserializerMethod method) {
  const v_uint32 id = classId.id;
  if(id < m_methods.size()) {
    m_methods[id] = method;
  } else {
    throw std::runtime_error("[webfr::parser::json::mapping::Deserializer::setDeserializerMethod()]: Error. Unknown classId");
  }
}

void Deserializer::skipScope(webfr::parser::Caret& caret, v_char8 charOpen, v_char8 charClose){

  p_char8 data = caret.getData();
  v_buff_size size = caret.getDataSize();
  v_buff_size pos = caret.getPosition();
  v_int32 scopeCounter = 0;

  bool isInString = false;

  while(pos < size){
    v_char8 a = data[pos];
    if(a == charOpen){
      if(!isInString){
        scopeCounter ++;
      }
    } else if(a == charClose){
      if(!isInString){
        scopeCounter --;
        if(scopeCounter == 0){
          caret.setPosition(pos + 1);
          return;
        }
      }
    } else if(a == '"') {
      isInString = !isInString;
    } else if(a == '\\'){
      pos ++;
    }

    pos ++;

  }
}

void Deserializer::skipString(webfr::parser::Caret& caret){
  p_char8 data = caret.getData();
  v_buff_size size = caret.getDataSize();
  v_buff_size pos = caret.getPosition();
  v_int32 scopeCounter = 0;
  while(pos < size){
    v_char8 a = data[pos];
    if(a == '"'){
      scopeCounter ++;
      if(scopeCounter == 2) {
        caret.setPosition(pos + 1);
        return;
      }
    } else if(a == '\\'){
      pos ++;
    }
    pos ++;
  }
}

void Deserializer::skipToken(webfr::parser::Caret& caret){
  p_char8 data = caret.getData();
  v_buff_size size = caret.getDataSize();
  v_buff_size pos = caret.getPosition();
  while(pos < size){
    v_char8 a = data[pos];
    if(a == ' ' || a == '\t' || a == '\n' || a == '\r' || a == '\b' || a == '\f' ||
       a == '}' || a == ',' || a == ']') {
      caret.setPosition(pos);
      return;
    }
    pos ++;
  }
}

void Deserializer::skipValue(webfr::parser::Caret& caret){
  if(caret.isAtChar('{')){
    skipScope(caret, '{', '}');
  } else if(caret.isAtChar('[')){
    skipScope(caret, '[', ']');
  } else if(caret.isAtChar('"')){
    skipString(caret);
  } else {
    skipToken(caret);
  }
}

webfr::Void Deserializer::deserializeFloat32(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {

  (void) deserializer;
  (void) type;

  if(caret.isAtText("null", true)){
    return webfr::Void(Float32::Class::getType());
  } else {
    return Float32(caret.parseFloat32());
  }
}

webfr::Void Deserializer::deserializeFloat64(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {

  (void) deserializer;
  (void) type;

  if(caret.isAtText("null", true)){
    return webfr::Void(Float64::Class::getType());
  } else {
    return Float64(caret.parseFloat64());
  }

}

webfr::Void Deserializer::deserializeBoolean(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {

  (void) deserializer;
  (void) type;

  if(caret.isAtText("null", true)){
    return webfr::Void(Boolean::Class::getType());
  } else {
    if(caret.isAtText("true", true)) {
      return Boolean(true);
    } else if(caret.isAtText("false", true)) {
      return Boolean(false);
    } else {
      caret.setError("[webfr::parser::json::mapping::Deserializer::readBooleanValue()]: Error. 'true' or 'false' - expected.", ERROR_CODE_VALUE_BOOLEAN);
      return webfr::Void(Boolean::Class::getType());
    }
  }

}

webfr::Void Deserializer::deserializeString(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {

  (void) deserializer;
  (void) type;

  if(caret.isAtText("null", true)){
    return webfr::Void(String::Class::getType());
  } else {
    return webfr::Void(webfr::parser::json::Utils::parseString(caret).getPtr(), String::Class::getType());
  }
}

const data::mapping::type::Type* Deserializer::guessNumberType(webfr::parser::Caret& caret) {
  (void)caret;
  return Float64::Class::getType();
}

const data::mapping::type::Type* Deserializer::guessType(webfr::parser::Caret& caret) {
  {
    parser::Caret::StateSaveGuard stateGuard(caret);
    v_char8 c = *caret.getCurrData();
    switch (c) {
      case '"':
        return String::Class::getType();
      case '{':
        return webfr::Fields<Any>::Class::getType();
      case '[':
        return webfr::List<Any>::Class::getType();
      case 't':
        if(caret.isAtText("true")) return Boolean::Class::getType();
        break;
      case 'f':
        if(caret.isAtText("false")) return Boolean::Class::getType();
        break;
      default:
        if (c == '-' || caret.isAtDigitChar()) {
          return guessNumberType(caret);
        }
    }
  }
  caret.setError("[webfr::parser::json::mapping::Deserializer::guessType()]: Error. Can't guess type for webfr::Any.");
  return nullptr;
}

webfr::Void Deserializer::deserializeAny(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {
  (void) type;
  if(caret.isAtText("null", true)){
    return webfr::Void(Any::Class::getType());
  } else {
    const Type* const fieldType = guessType(caret);
    if(fieldType != nullptr) {
      auto fieldValue = deserializer->deserialize(caret, fieldType);
      auto anyHandle = std::make_shared<data::mapping::type::AnyHandle>(fieldValue.getPtr(), fieldValue.valueType);
      return webfr::Void(anyHandle, Any::Class::getType());
    }
  }
  return webfr::Void(Any::Class::getType());
}

webfr::Void Deserializer::deserializeEnum(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {

  auto polymorphicDispatcher = static_cast<const data::mapping::type::__class::AbstractEnum::PolymorphicDispatcher*>(
    type->polymorphicDispatcher
  );

  data::mapping::type::EnumInterpreterError e = data::mapping::type::EnumInterpreterError::OK;
  const auto& value = deserializer->deserialize(caret, polymorphicDispatcher->getInterpretationType());
  if(caret.hasError()) {
    return nullptr;
  }
  const auto& result = polymorphicDispatcher->fromInterpretation(value, e);

  if(e == data::mapping::type::EnumInterpreterError::OK) {
    return result;
  }

  switch(e) {
    case data::mapping::type::EnumInterpreterError::CONSTRAINT_NOT_NULL:
      caret.setError("[webfr::parser::json::mapping::Deserializer::deserializeEnum()]: Error. Enum constraint violated - 'NotNull'.");
      break;
    default:
      caret.setError("[webfr::parser::json::mapping::Deserializer::deserializeEnum()]: Error. Can't deserialize Enum.");
  }

  return nullptr;

}

webfr::Void Deserializer::deserializeObject(Deserializer* deserializer, parser::Caret& caret, const Type* const type) {

  if(caret.isAtText("null", true)){
    return webfr::Void(type);
  }

  if(caret.canContinueAtChar('{', 1)) {

    auto dispatcher = static_cast<const webfr::data::mapping::type::__class::AbstractObject::PolymorphicDispatcher*>(type->polymorphicDispatcher);
    auto object = dispatcher->createObject();
    const auto& fieldsMap = dispatcher->getProperties()->getMap();

    caret.skipBlankChars();

    while (!caret.isAtChar('}') && caret.canContinue()) {

      caret.skipBlankChars();
      auto key = Utils::parseStringToStdString(caret);
      if(caret.hasError()){
        return nullptr;
      }

      auto fieldIterator = fieldsMap.find(key);
      if(fieldIterator != fieldsMap.end()){

        caret.skipBlankChars();
        if(!caret.canContinueAtChar(':', 1)){
          caret.setError("[webfr::parser::json::mapping::Deserializer::readObject()]: Error. ':' - expected", ERROR_CODE_OBJECT_SCOPE_COLON_MISSING);
          return nullptr;
        }

        caret.skipBlankChars();

        auto field = fieldIterator->second;
        field->set(static_cast<webfr::BaseObject*>(object.get()), deserializer->deserialize(caret, field->type));

      } else if (deserializer->getConfig()->allowUnknownFields) {
        caret.skipBlankChars();
        if(!caret.canContinueAtChar(':', 1)){
          caret.setError("[webfr::parser::json::mapping::Deserializer::readObject()/if(config->allowUnknownFields){}]: Error. ':' - expected", ERROR_CODE_OBJECT_SCOPE_COLON_MISSING);
          return nullptr;
        }
        caret.skipBlankChars();
        skipValue(caret);
      } else {
        caret.setError("[webfr::parser::json::mapping::Deserializer::readObject()]: Error. Unknown field", ERROR_CODE_OBJECT_SCOPE_UNKNOWN_FIELD);
        return nullptr;
      }

      caret.skipBlankChars();
      caret.canContinueAtChar(',', 1);

    }

    if(!caret.canContinueAtChar('}', 1)){
      if(!caret.hasError()){
        caret.setError("[webfr::parser::json::mapping::Deserializer::readObject()]: Error. '}' - expected", ERROR_CODE_OBJECT_SCOPE_CLOSE);
      }
      return nullptr;
    }

    return object;

  } else {
    caret.setError("[webfr::parser::json::mapping::Deserializer::readObject()]: Error. '{' - expected", ERROR_CODE_OBJECT_SCOPE_OPEN);
  }

  return nullptr;

}

webfr::Void Deserializer::deserialize(parser::Caret& caret, const Type* const type) {
  auto id = type->classId.id;
  auto& method = m_methods[id];
  if(method) {
    return (*method)(this, caret, type);
  } else {

    auto* interpretation = type->findInterpretation(m_config->enabledInterpretations);
    if(interpretation) {
      return interpretation->fromInterpretation(deserialize(caret, interpretation->getInterpretationType()));
    }

    throw std::runtime_error("[webfr::parser::json::mapping::Deserializer::deserialize()]: "
                             "Error. No deserialize method for type '" + std::string(type->classId.name) + "'");
  }
}

const std::shared_ptr<Deserializer::Config>& Deserializer::getConfig() {
  return m_config;
}

}}}}