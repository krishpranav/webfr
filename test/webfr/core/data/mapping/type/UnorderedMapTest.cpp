// includes
#include "UnorderedMapTest.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

void UnorderedMapTest::onRun() {

  {
    WEBFR_LOGI(TAG, "test default constructor...");
    webfr::UnorderedFields<String> map;

    WEBFR_ASSERT(!map);
    WEBFR_ASSERT(map == nullptr);

    WEBFR_ASSERT(map.get() == nullptr);
    WEBFR_ASSERT(map.valueType->classId.id == webfr::data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID.id);
    WEBFR_ASSERT(map.valueType->params.size() == 2);
    auto it = map.valueType->params.begin();
    WEBFR_ASSERT(*it++ == webfr::String::Class::getType());
    WEBFR_ASSERT(*it++ == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test empty ilist constructor...");
    webfr::UnorderedFields<String> map({});

    WEBFR_ASSERT(map);
    WEBFR_ASSERT(map != nullptr);
    WEBFR_ASSERT(map->size() == 0);

    WEBFR_ASSERT(map.get() != nullptr);
    WEBFR_ASSERT(map.valueType->classId.id == webfr::data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID.id);
    WEBFR_ASSERT(map.valueType->params.size() == 2);
    auto it = map.valueType->params.begin();
    WEBFR_ASSERT(*it++ == webfr::String::Class::getType());
    WEBFR_ASSERT(*it++ == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test createShared()...");
    webfr::UnorderedFields<String> map = webfr::UnorderedFields<String>::createShared();

    WEBFR_ASSERT(map);
    WEBFR_ASSERT(map != nullptr);
    WEBFR_ASSERT(map->size() == 0);

    WEBFR_ASSERT(map.get() != nullptr);
    WEBFR_ASSERT(map.valueType->classId.id == webfr::data::mapping::type::__class::AbstractUnorderedMap::CLASS_ID.id);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test copy-assignment operator...");
    webfr::UnorderedFields<String> map1({});
    webfr::UnorderedFields<String> map2;

    map2 = map1;

    WEBFR_ASSERT(map1);
    WEBFR_ASSERT(map2);

    WEBFR_ASSERT(map1->size() == 0);
    WEBFR_ASSERT(map2->size() == 0);

    WEBFR_ASSERT(map1.get() == map2.get());

    map2->insert({"key", "a"});

    WEBFR_ASSERT(map1->size() == 1);
    WEBFR_ASSERT(map2->size() == 1);

    map2 = {{"key1", "b"}, {"key2", "c"}};

    WEBFR_ASSERT(map1->size() == 1);
    WEBFR_ASSERT(map2->size() == 2);

    WEBFR_ASSERT(map2["key1"] == "b");
    WEBFR_ASSERT(map2["key2"] == "c");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test move-assignment operator...");
    webfr::UnorderedFields<String> map1({});
    webfr::UnorderedFields<String> map2;

    map2 = std::move(map1);

    WEBFR_ASSERT(!map1);
    WEBFR_ASSERT(map2);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test get element by index...");
    webfr::UnorderedFields<String> map = {{"key1", "a"}, {"key2", "b"}, {"key3", "c"}};

    WEBFR_ASSERT(map);
    WEBFR_ASSERT(map != nullptr);
    WEBFR_ASSERT(map->size() == 3);

    WEBFR_ASSERT(map["key1"] == "a");
    WEBFR_ASSERT(map["key2"] == "b");
    WEBFR_ASSERT(map["key3"] == "c");

    map["key2"] = "Hello!";

    WEBFR_ASSERT(map->size() == 3);

    WEBFR_ASSERT(map["key1"] == "a");
    WEBFR_ASSERT(map["key2"] == "Hello!");
    WEBFR_ASSERT(map["key3"] == "c");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test polymorphicDispatcher...");
    webfr::UnorderedFields<String> map = {{"key1", "a"}, {"key2", "b"}, {"key3", "c"}};

    auto polymorphicDispatcher = static_cast<const typename webfr::UnorderedFields<String>::Class::PolymorphicDispatcher*>(
      map.valueType->polymorphicDispatcher
    );

    polymorphicDispatcher->addPolymorphicItem(map, webfr::String("key1"), webfr::String("d"));

    WEBFR_ASSERT(map->size() == 3);

    WEBFR_ASSERT(map["key1"] == "d");
    WEBFR_ASSERT(map["key2"] == "b");
    WEBFR_ASSERT(map["key3"] == "c");
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}
