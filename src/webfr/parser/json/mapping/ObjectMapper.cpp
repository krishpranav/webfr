#include "ObjectMapper.hpp"

namespace webfr { namespace parser { namespace json { namespace mapping {

ObjectMapper::ObjectMapper(const std::shared_ptr<Serializer::Config>& serializerConfig,
                           const std::shared_ptr<Deserializer::Config>& deserializerConfig)
  : data::mapping::ObjectMapper(getMapperInfo())
  , m_serializer(std::make_shared<Serializer>(serializerConfig))
  , m_deserializer(std::make_shared<Deserializer>(deserializerConfig))
{}

ObjectMapper::ObjectMapper(const std::shared_ptr<Serializer>& serializer,
                           const std::shared_ptr<Deserializer>& deserializer)
  : data::mapping::ObjectMapper(getMapperInfo())
  , m_serializer(serializer)
  , m_deserializer(deserializer)
{}

std::shared_ptr<ObjectMapper> ObjectMapper::createShared(const std::shared_ptr<Serializer::Config>& serializerConfig,
                                                         const std::shared_ptr<Deserializer::Config>& deserializerConfig){
  return std::make_shared<ObjectMapper>(serializerConfig, deserializerConfig);
}

std::shared_ptr<ObjectMapper> ObjectMapper::createShared(const std::shared_ptr<Serializer>& serializer,
                                                         const std::shared_ptr<Deserializer>& deserializer){
  return std::make_shared<ObjectMapper>(serializer, deserializer);
}

void ObjectMapper::write(data::stream::ConsistentOutputStream* stream,
                         const webfr::Void& variant) const {
  m_serializer->serializeToStream(stream, variant);
}

webfr::Void ObjectMapper::read(webfr::parser::Caret& caret,
                                                                     const webfr::data::mapping::type::Type* const type) const {
  return m_deserializer->deserialize(caret, type);
}

std::shared_ptr<Serializer> ObjectMapper::getSerializer() {
  return m_serializer;
}

std::shared_ptr<Deserializer> ObjectMapper::getDeserializer() {
  return m_deserializer;
}

}}}}