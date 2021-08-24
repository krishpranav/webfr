#ifndef webfr_parser_json_mapping_ObjectMapper_hpp
#define webfr_parser_json_mapping_ObjectMapper_hpp

#include "./Serializer.hpp"
#include "./Deserializer.hpp"
#include "webfr/core/data/mapping/ObjectMapper.hpp"

namespace webfr { namespace parser { namespace json { namespace mapping {

class ObjectMapper : public webfr::base::Countable, public webfr::data::mapping::ObjectMapper {
private:
  static Info& getMapperInfo() {
    static Info info("application/json");
    return info;
  }
private:
  std::shared_ptr<Serializer> m_serializer;
  std::shared_ptr<Deserializer> m_deserializer;
public:

  ObjectMapper(const std::shared_ptr<Serializer::Config>& serializerConfig,
               const std::shared_ptr<Deserializer::Config>& deserializerConfig);

  /**
   * Constructor.
   * @param serializer
   * @param deserializer
   */
  ObjectMapper(const std::shared_ptr<Serializer>& serializer = std::make_shared<Serializer>(),
               const std::shared_ptr<Deserializer>& deserializer = std::make_shared<Deserializer>());
public:

  static std::shared_ptr<ObjectMapper>
  createShared(const std::shared_ptr<Serializer::Config>& serializerConfig,
               const std::shared_ptr<Deserializer::Config>& deserializerConfig);

  /**
   * Create shared ObjectMapper.
   * @param serializer
   * @param deserializer
   * @return
   */
  static std::shared_ptr<ObjectMapper>
  createShared(const std::shared_ptr<Serializer>& serializer = std::make_shared<Serializer>(),
               const std::shared_ptr<Deserializer>& deserializer = std::make_shared<Deserializer>());

  void write(data::stream::ConsistentOutputStream* stream, const webfr::Void& variant) const override;

  webfr::Void read(webfr::parser::Caret& caret, const webfr::data::mapping::type::Type* const type) const override;


  std::shared_ptr<Serializer> getSerializer();

  std::shared_ptr<Deserializer> getDeserializer();
  
};
  
}}}}

#endif 