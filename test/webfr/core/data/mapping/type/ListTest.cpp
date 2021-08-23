// includes
#include "ListTest.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

void ListTest::onRun() {

  {
    WEBFR_LOGI(TAG, "test default constructor...");
    webfr::List<webfr::String> list;

    WEBFR_ASSERT(!list);
    WEBFR_ASSERT(list == nullptr);

    WEBFR_ASSERT(list.get() == nullptr);
    WEBFR_ASSERT(list.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
    WEBFR_ASSERT(list.valueType->params.size() == 1);
    WEBFR_ASSERT(list.valueType->params.front() == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test empty ilist constructor...");
    webfr::List<webfr::String> list({});

    WEBFR_ASSERT(list);
    WEBFR_ASSERT(list != nullptr);
    WEBFR_ASSERT(list->size() == 0);

    WEBFR_ASSERT(list.get() != nullptr);
    WEBFR_ASSERT(list.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
    WEBFR_ASSERT(list.valueType->params.size() == 1);
    WEBFR_ASSERT(list.valueType->params.front() == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test createShared()...");
    webfr::List<webfr::String> list = webfr::List<webfr::String>::createShared();

    WEBFR_ASSERT(list);
    WEBFR_ASSERT(list != nullptr);
    WEBFR_ASSERT(list->size() == 0);

    WEBFR_ASSERT(list.get() != nullptr);
    WEBFR_ASSERT(list.valueType->classId.id == webfr::data::mapping::type::__class::AbstractList::CLASS_ID.id);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test copy-assignment operator...");
    webfr::List<webfr::String> list1({});
    webfr::List<webfr::String> list2;

    list2 = list1;

    WEBFR_ASSERT(list1);
    WEBFR_ASSERT(list2);

    WEBFR_ASSERT(list1->size() == 0);
    WEBFR_ASSERT(list2->size() == 0);

    WEBFR_ASSERT(list1.get() == list2.get());

    list2->push_back("a");

    WEBFR_ASSERT(list1->size() == 1);
    WEBFR_ASSERT(list2->size() == 1);

    list2 = {"b", "c"};

    WEBFR_ASSERT(list1->size() == 1);
    WEBFR_ASSERT(list2->size() == 2);

    WEBFR_ASSERT(list2[0] == "b");
    WEBFR_ASSERT(list2[1] == "c");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test move-assignment operator...");
    webfr::List<webfr::String> list1({});
    webfr::List<webfr::String> list2;

    list2 = std::move(list1);

    WEBFR_ASSERT(!list1);
    WEBFR_ASSERT(list2);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test get element by index...");
    webfr::List<webfr::String> list = {"a", "b", "c"};

    WEBFR_ASSERT(list);
    WEBFR_ASSERT(list != nullptr);
    WEBFR_ASSERT(list->size() == 3);

    WEBFR_ASSERT(list[0] == "a");
    WEBFR_ASSERT(list[1] == "b");
    WEBFR_ASSERT(list[2] == "c");

    list[1] = "Hello!";

    WEBFR_ASSERT(list->size() == 3);

    WEBFR_ASSERT(list[0] == "a");
    WEBFR_ASSERT(list[1] == "Hello!");
    WEBFR_ASSERT(list[2] == "c");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test polymorphicDispatcher...");
    webfr::List<webfr::String> list = {"a", "b", "c"};

    auto polymorphicDispatcher = static_cast<const typename webfr::List<webfr::String>::Class::PolymorphicDispatcher*>(
      list.valueType->polymorphicDispatcher
    );

    polymorphicDispatcher->addPolymorphicItem(list, webfr::String("d"));

    WEBFR_ASSERT(list->size() == 4);

    WEBFR_ASSERT(list[0] == "a");
    WEBFR_ASSERT(list[1] == "b");
    WEBFR_ASSERT(list[2] == "c");
    WEBFR_ASSERT(list[3] == "d");
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}
