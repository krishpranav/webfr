#ifndef webfr_parser_json_mapping_Serializer_hpp
#define webfr_parser_json_mapping_Serializer_hpp

#include "webfr/parser/json/Beautifier.hpp"
#include "webfr/core/Types.hpp"
#include <vector>

namespace webfr { namespace parser { namespace json { namespace mapping {

class Serializer {
public:
  typedef webfr::data::mapping::type::Type Type;
  typedef webfr::data::mapping::type::BaseObject::Property Property;
  typedef webfr::data::mapping::type::BaseObject::Properties Properties;

  typedef webfr::String String;
public:

  class Config : public webfr::base::Countable {
  public:

    Config()
    {}
  public:

    static std::shared_ptr<Config> createShared(){
      return std::make_shared<Config>();
    }

    bool includeNullFields = true;

    bool throwOnUnknownTypes = true;

    bool useBeautifier = false;

    webfr::String beautifierIndent = "  ";

    webfr::String beautifierNewLine = "\n";

    std::vector<std::string> enabledInterpretations = {};

  };
public:
  typedef void (*SerializerMethod)(Serializer*,
                                   data::stream::ConsistentOutputStream*,
                                   const webfr::Void&);
private:

  template<class T>
  static void serializePrimitive(Serializer* serializer,
                                 data::stream::ConsistentOutputStream* stream,
                                 const webfr::Void& polymorph){
    (void) serializer;

    if(polymorph){
      stream->writeAsString(* static_cast<typename T::ObjectType*>(polymorph.get()));
    } else {
      stream->writeSimple("null", 4);
    }
  }

  template<class Collection>
  static void serializeList(Serializer* serializer, data::stream::ConsistentOutputStream* stream, const webfr::Void& polymorph) {

    if(!polymorph) {
      stream->writeSimple("null", 4);
      return;
    }

    const auto& list = polymorph.staticCast<Collection>();

    stream->writeCharSimple('[');
    bool first = true;

    for(auto& value : *list) {
      if(value || serializer->getConfig()->includeNullFields) {
        (first) ? first = false : stream->writeSimple(",", 1);
        serializer->serialize(stream, value);
      }
    }

    stream->writeCharSimple(']');

  }

  template<class Collection>
  static void serializeKeyValue(Serializer* serializer, data::stream::ConsistentOutputStream* stream, const webfr::Void& polymorph) {

    if(!polymorph) {
      stream->writeSimple("null", 4);
      return;
    }

    const auto& map = polymorph.staticCast<Collection>();

    stream->writeCharSimple('{');
    bool first = true;

    for(auto& pair : *map) {
      const auto& value = pair.second;
      if(value || serializer->getConfig()->includeNullFields) {
        (first) ? first = false : stream->writeSimple(",", 1);
        const auto& key = pair.first;
        serializeString(stream, key->getData(), key->getSize());
        stream->writeSimple(":", 1);
        serializer->serialize(stream, value);
      }
    }

    stream->writeCharSimple('}');

  }

  static void serializeString(webfr::data::stream::ConsistentOutputStream* stream, p_char8 data, v_buff_size size);
  static void serializeString(Serializer* serializer,
                              data::stream::ConsistentOutputStream* stream,
                              const webfr::Void& polymorph);

  static void serializeAny(Serializer* serializer,
                           data::stream::ConsistentOutputStream* stream,
                           const webfr::Void& polymorph);

  static void serializeEnum(Serializer* serializer,
                            data::stream::ConsistentOutputStream* stream,
                            const webfr::Void& polymorph);

  static void serializeObject(Serializer* serializer,
                              data::stream::ConsistentOutputStream* stream,
                              const webfr::Void& polymorph);

  void serialize(data::stream::ConsistentOutputStream* stream, const webfr::Void& polymorph);

private:
  std::shared_ptr<Config> m_config;
  std::vector<SerializerMethod> m_methods;
public:

  Serializer(const std::shared_ptr<Config>& config = std::make_shared<Config>());

  void setSerializerMethod(const data::mapping::type::ClassId& classId, SerializerMethod method);

  void serializeToStream(data::stream::ConsistentOutputStream* stream, const webfr::Void& polymorph);

  const std::shared_ptr<Config>& getConfig();

};

}}}}

#endif 