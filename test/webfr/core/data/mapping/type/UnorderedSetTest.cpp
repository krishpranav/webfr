// includes
#include "UnorderedSetTest.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

void UnorderedSetTest::onRun() {


  {
    WEBFR_LOGI(TAG, "test default constructor...");
    webfr::UnorderedSet<webfr::String> set;

    WEBFR_ASSERT(!set);
    WEBFR_ASSERT(set == nullptr);

    WEBFR_ASSERT(set.get() == nullptr);
    WEBFR_ASSERT(set.valueType->classId.id == webfr::data::mapping::type::__class::AbstractUnorderedSet::CLASS_ID.id);
    WEBFR_ASSERT(set.valueType->params.size() == 1);
    WEBFR_ASSERT(set.valueType->params.front() == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test empty ilist constructor...");
    webfr::UnorderedSet<webfr::String> set({});

    WEBFR_ASSERT(set);
    WEBFR_ASSERT(set != nullptr);
    WEBFR_ASSERT(set->size() == 0);

    WEBFR_ASSERT(set.get() != nullptr);
    WEBFR_ASSERT(set.valueType->classId.id == webfr::data::mapping::type::__class::AbstractUnorderedSet::CLASS_ID.id);
    WEBFR_ASSERT(set.valueType->params.size() == 1);
    WEBFR_ASSERT(set.valueType->params.front() == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test createShared()...");
    webfr::UnorderedSet<webfr::String> set = webfr::UnorderedSet<webfr::String>::createShared();

    WEBFR_ASSERT(set);
    WEBFR_ASSERT(set != nullptr);
    WEBFR_ASSERT(set->size() == 0);

    WEBFR_ASSERT(set.get() != nullptr);
    WEBFR_ASSERT(set.valueType->classId.id == webfr::data::mapping::type::__class::AbstractUnorderedSet::CLASS_ID.id);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test copy-assignment operator...");
    webfr::UnorderedSet<webfr::String> set1({});
    webfr::UnorderedSet<webfr::String> set2;

    set2 = set1;

    WEBFR_ASSERT(set1);
    WEBFR_ASSERT(set2);

    WEBFR_ASSERT(set1->size() == 0);
    WEBFR_ASSERT(set2->size() == 0);

    WEBFR_ASSERT(set1.get() == set2.get());

    set2->insert("a");

    WEBFR_ASSERT(set1->size() == 1);
    WEBFR_ASSERT(set2->size() == 1);

    set2 = {"b", "c"};

    WEBFR_ASSERT(set1->size() == 1);
    WEBFR_ASSERT(set2->size() == 2);

    WEBFR_ASSERT(set2["b"] == true);
    WEBFR_ASSERT(set2["c"] == true);

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test move-assignment operator...");
    webfr::UnorderedSet<webfr::String> set1({});
    webfr::UnorderedSet<webfr::String> set2;

    set2 = std::move(set1);

    WEBFR_ASSERT(!set1);
    WEBFR_ASSERT(set2);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test polymorphicDispatcher...");
    webfr::UnorderedSet<webfr::String> set = {"a", "b", "c"};

    auto polymorphicDispatcher = static_cast<const typename webfr::UnorderedSet<webfr::String>::Class::PolymorphicDispatcher*>(
      set.valueType->polymorphicDispatcher
    );

    polymorphicDispatcher->addPolymorphicItem(set, webfr::String("a"));
    polymorphicDispatcher->addPolymorphicItem(set, webfr::String("b"));
    polymorphicDispatcher->addPolymorphicItem(set, webfr::String("c"));

    polymorphicDispatcher->addPolymorphicItem(set, webfr::String("d"));

    WEBFR_ASSERT(set->size() == 4);

    WEBFR_ASSERT(set["a"]);
    WEBFR_ASSERT(set["b"]);
    WEBFR_ASSERT(set["c"]);
    WEBFR_ASSERT(set["d"]);
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}
