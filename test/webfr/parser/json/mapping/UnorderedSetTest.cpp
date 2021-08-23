#include "UnorderedSetTest.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"

namespace webfr { namespace test { namespace parser { namespace json { namespace mapping {

void UnorderedSetTest::onRun() {

  webfr::parser::json::mapping::ObjectMapper mapper;

  {
    webfr::UnorderedSet<webfr::String> set = {"Hello", "World", "!"};
    auto json = mapper.writeToString(set);
    WEBFR_LOGD(TAG, "json='%s'", json->c_str());
  }

  {
    webfr::String json = "[\"Hello\",\"World\",\"!\",\"Hello\",\"World\",\"!\"]";
    auto set = mapper.readFromString<webfr::UnorderedSet<webfr::String>>(json);
    WEBFR_ASSERT(set);
    WEBFR_ASSERT(set->size() == 3);
    for(auto& item : *set) {
      WEBFR_LOGD(TAG, "item='%s'", item->c_str());
    }
  }
  
}

}}}}}