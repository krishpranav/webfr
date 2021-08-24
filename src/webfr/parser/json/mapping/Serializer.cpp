#include "Serializer.hpp"
#include "webfr/parser/json/Utils.hpp"
#include "webfr/core/data/mapping/type/Any.hpp"

namespace webfr { namespace parser { namespace json { namespace mapping {

Serializer::Serializer(const std::shared_ptr<Config>& config)
  : m_config(config)
{

  m_methods.resize(data::mapping::type::ClassId::getClassCount(), nullptr);

  setSerializerMethod(data::mapping::type::__class::String::CLASS_ID, &Serializer::serializeString);
  setSerializerMethod(data::mapping::type::__class::Any::CLASS_ID, &Serializer::serializeAny);

  setSerializerMethod(data::mapping::type::__class::Int8::CLASS_ID, &Serializer::serializePrimitive<webfr::Int8>);
  setSerializerMethod(data::mapping::type::__class::UInt8::CLASS_ID, &Serializer::serializePrimitive<webfr::UInt8>);

  setSerializerMethod(data::mapping::type::__class::Int16::CLASS_ID, &Serializer::serializePrimitive<webfr::Int16>);
  setSerializerMethod(data::mapping::type::__class::UInt16::CLASS_ID, &Serializer::serializePrimitive<webfr::UInt16>);

  setSerializerMethod(data::mapping::type::__class::Int32::CLASS_ID, &Serializer::serializePrimitive<webfr::Int32>);
  setSerializerMethod(data::mapping::type::__class::UInt32::CLASS_ID, &Serializer::serializePrimitive<webfr::UInt32>);

  setSerializerMethod(data::mapping::type::__class::Int64::CLASS_ID, &Serializer::serializePrimitive<webfr::Int64>);
  setSerializerMethod(data::mapping::type::__class::UInt64::CLASS_ID, &Serializer::serializePrimitive<webfr::UInt64>);

  setSerializerMethod(data::mapping::type::__class::Float32::CLASS_ID, &Serializer::serializePrimitive<webfr::Float32>);
  setSerializerMethod(data::mapping::type::__class::Float64::CLASS_ID, &Serializer::serializePrimitive<webfr::Float64>);
  setSerializerMethod(data::mapping::type::__class::Boolean::CLASS_ID, &Serializer::serializePrimitive<webfr::Boolean>);

  setSerializerMethod(data::mapping::type::__class::AbstractObject::CLASS_ID, &Serializer::serializeObject);
  setSerializerMethod(data::mapping::type::__class::AbstractEnum::CLASS_ID, &Serializer::serializeEnum);

  setSerializerMethod(data::mapping::type::__class::AbstractVector::CLASS_ID, &Serializer::serializeList<webfr::AbstractVector>);
  setSerializerMethod(data::mapping::type::__class::AbstractList::CLASS_ID, &Serializer::serializeList<webfr::AbstractList>);
  setSerializerMethod(data::mapping::type::__class::AbstractUnorderedSet::CLASS_ID, &Serializer::serializeList<webfr::AbstractUnorderedSet>);

  setSerializerMethod(data::mapping::type::__class::AbstractPairList::CLASS_ID, &Serializer::serializeKeyValue<webfr::AbstractFields>);
  setSerializerMethod(data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID, &Serializer::serializeKeyValue<webfr::AbstractUnorderedFields>);

}

void Serializer::setSerializerMethod(const data::mapping::type::ClassId& classId, SerializerMethod method) {
  const v_uint32 id = classId.id;
  if(id < m_methods.size()) {
    m_methods[id] = method;
  } else {
    throw std::runtime_error("[webfr::parser::json::mapping::Serializer::setSerializerMethod()]: Error. Unknown classId");
  }
}

void Serializer::serializeString(data::stream::ConsistentOutputStream* stream, p_char8 data, v_buff_size size) {
  auto encodedValue = Utils::escapeString(data, size, false);
  stream->writeCharSimple('\"');
  stream->writeSimple(encodedValue);
  stream->writeCharSimple('\"');
}

void Serializer::serializeString(Serializer* serializer,
                                  data::stream::ConsistentOutputStream* stream,
                                  const webfr::Void& polymorph)
{

  (void) serializer;

  if(!polymorph) {
    stream->writeSimple("null", 4);
    return;
  }

  auto str = static_cast<webfr::base::StrBuffer*>(polymorph.get());

  serializeString(stream, str->getData(), str->getSize());

}

void Serializer::serializeAny(Serializer* serializer,
                              data::stream::ConsistentOutputStream* stream,
                              const webfr::Void& polymorph)
{

  if(!polymorph) {
    stream->writeSimple("null", 4);
    return;
  }

  auto anyHandle = static_cast<data::mapping::type::AnyHandle*>(polymorph.get());
  serializer->serialize(stream, webfr::Void(anyHandle->ptr, anyHandle->type));

}

void Serializer::serializeEnum(Serializer* serializer,
                               data::stream::ConsistentOutputStream* stream,
                               const webfr::Void& polymorph)
{
  auto polymorphicDispatcher = static_cast<const data::mapping::type::__class::AbstractEnum::PolymorphicDispatcher*>(
    polymorph.valueType->polymorphicDispatcher
  );

  data::mapping::type::EnumInterpreterError e = data::mapping::type::EnumInterpreterError::OK;
  serializer->serialize(stream, polymorphicDispatcher->toInterpretation(polymorph, e));

  if(e == data::mapping::type::EnumInterpreterError::OK) {
    return;
  }

  switch(e) {
    case data::mapping::type::EnumInterpreterError::CONSTRAINT_NOT_NULL:
      throw std::runtime_error("[webfr::parser::json::mapping::Serializer::serializeEnum()]: Error. Enum constraint violated - 'NotNull'.");
    default:
      throw std::runtime_error("[webfr::parser::json::mapping::Serializer::serializeEnum()]: Error. Can't serialize Enum.");
  }

}

void Serializer::serializeObject(Serializer* serializer,
                                  data::stream::ConsistentOutputStream* stream,
                                  const webfr::Void& polymorph)
{

  if(!polymorph) {
    stream->writeSimple("null", 4);
    return;
  }

  stream->writeCharSimple('{');

  bool first = true;
  auto dispatcher = static_cast<const webfr::data::mapping::type::__class::AbstractObject::PolymorphicDispatcher*>(polymorph.valueType->polymorphicDispatcher);
  auto fields = dispatcher->getProperties()->getList();
  auto object = static_cast<webfr::BaseObject*>(polymorph.get());

  for (auto const& field : fields) {

    auto value = field->get(object);
    if(value || serializer->getConfig()->includeNullFields) {
      (first) ? first = false : stream->writeSimple(",", 1);
      serializeString(stream, (p_char8)field->name, std::strlen(field->name));
      stream->writeSimple(":", 1);
      serializer->serialize(stream, value);
    }

  }

  stream->writeCharSimple('}');

}

void Serializer::serialize(data::stream::ConsistentOutputStream* stream,
                            const webfr::Void& polymorph)
{
  auto id = polymorph.valueType->classId.id;
  auto& method = m_methods[id];
  if(method) {
    (*method)(this, stream, polymorph);
  } else {

    auto* interpretation = polymorph.valueType->findInterpretation(m_config->enabledInterpretations);
    if(interpretation) {
      serialize(stream, interpretation->toInterpretation(polymorph));
    } else {
      throw std::runtime_error("[webfr::parser::json::mapping::Serializer::serialize()]: "
                               "Error. No serialize method for type '" +
                               std::string(polymorph.valueType->classId.name) + "'");
    }

  }
}

void Serializer::serializeToStream(data::stream::ConsistentOutputStream* stream,
                                   const webfr::Void& polymorph)
{
  if(m_config->useBeautifier) {
    json::Beautifier beautifier(stream, "  ", "\n");
    serialize(&beautifier, polymorph);
  } else {
    serialize(stream, polymorph);
  }
}

const std::shared_ptr<Serializer::Config>& Serializer::getConfig() {
  return m_config;
}

}}}}