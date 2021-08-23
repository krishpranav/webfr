// includes
#include "LazyStringMapTest.hpp"
#include "webfr/core/data/share/LazyStringMap.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace share {

namespace {

  typedef webfr::data::share::StringKeyLabel StringKeyLabel;
  typedef webfr::data::share::StringKeyLabelCI StringKeyLabelCI;

  template<class T>
  using LazyStringMap = webfr::data::share::LazyStringMap<T>;

}

void LazyStringMapTest::onRun() {

  p_char8 text = (p_char8) "Hello World!";

  {

    LazyStringMap<StringKeyLabel> map;

    map.put("key1", StringKeyLabel(nullptr, text, 5));
    map.put("key2", StringKeyLabel(nullptr, text + 6, 6));

    webfr::String s1 = map.get("key1");
    webfr::String s2 = map.get("key2");

    WEBFR_ASSERT(s1 == "Hello");
    WEBFR_ASSERT(s2 == "World!");

    webfr::String s12 = map.get("key1");
    webfr::String s22 = map.get("key2");

    WEBFR_ASSERT(s1.get() == s12.get());
    WEBFR_ASSERT(s2.get() == s22.get());

    WEBFR_ASSERT(map.get("KEY1") == nullptr);
    WEBFR_ASSERT(map.get("KEY2") == nullptr);

    auto all = map.getAll();

    auto s13 = all["key1"];
    auto s23 = all["key2"];

    WEBFR_ASSERT(s13.getData() == s1->getData() && s13.getSize() == s1->getSize());
    WEBFR_ASSERT(s23.getData() == s2->getData() && s23.getSize() == s2->getSize());
    WEBFR_ASSERT(s1.get() == s13.getMemoryHandle().get());
    WEBFR_ASSERT(s2.get() == s23.getMemoryHandle().get());

    WEBFR_ASSERT(map.getSize() == 2);

  }

  {

    LazyStringMap<StringKeyLabelCI> map;

    map.put("key1", StringKeyLabel(nullptr, text, 5));
    map.put("key2", StringKeyLabel(nullptr, text + 6, 6));

    auto s01 = map.getAsMemoryLabel_Unsafe<StringKeyLabel>("key1");
    auto s02 = map.getAsMemoryLabel_Unsafe<StringKeyLabel>("key2");

    WEBFR_ASSERT(s01 == "Hello");
    WEBFR_ASSERT(s02 == "World!");

    WEBFR_ASSERT(s01.getMemoryHandle() == nullptr);
    WEBFR_ASSERT(s02.getMemoryHandle() == nullptr);

    auto s1 = map.getAsMemoryLabel<StringKeyLabel>("key1");
    auto s2 = map.getAsMemoryLabel<StringKeyLabel>("key2");

    WEBFR_ASSERT(s1 == "Hello");
    WEBFR_ASSERT(s2 == "World!");

    webfr::String s12 = map.get("key1");
    webfr::String s22 = map.get("key2");

    WEBFR_ASSERT(s1.getMemoryHandle().get() == s12.get());
    WEBFR_ASSERT(s2.getMemoryHandle().get() == s22.get());

    WEBFR_ASSERT(map.getAsMemoryLabel<StringKeyLabel>("KEY1") == s1);
    WEBFR_ASSERT(map.getAsMemoryLabel<StringKeyLabel>("KEY2") == s2);

  }

  {

    LazyStringMap<StringKeyLabelCI> map1;
    LazyStringMap<StringKeyLabelCI> map2;

    map1.put("key1", StringKeyLabel(nullptr, text, 5));
    map1.put("key2", StringKeyLabel(nullptr, text + 6, 6));

    WEBFR_ASSERT(map1.getSize() == 2);
    WEBFR_ASSERT(map2.getSize() == 0);

    map2 = std::move(map1);

    WEBFR_ASSERT(map1.getSize() == 0);
    WEBFR_ASSERT(map2.getSize() == 2);

    {

      auto all = map2.getAll_Unsafe();

      auto s1 = all["key1"];
      auto s2 = all["key2"];

      WEBFR_ASSERT(s1.getMemoryHandle() == nullptr);
      WEBFR_ASSERT(s2.getMemoryHandle() == nullptr);

      WEBFR_ASSERT(s1 == "Hello");
      WEBFR_ASSERT(s2 == "World!");

    }

    {

      auto all = map2.getAll();

      auto s1 = all["key1"];
      auto s2 = all["key2"];

      WEBFR_ASSERT(s1.getMemoryHandle());
      WEBFR_ASSERT(s2.getMemoryHandle());

      WEBFR_ASSERT(s1 == "Hello");
      WEBFR_ASSERT(s2 == "World!");

      auto s12 = map2.get("key1");
      auto s22 = map2.get("key2");

      WEBFR_ASSERT(s1.getMemoryHandle().get() == s12.get());
      WEBFR_ASSERT(s2.getMemoryHandle().get() == s22.get());

    }

  }


}

}}}}}