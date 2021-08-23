// includes
#include "VectorTest.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

void VectorTest::onRun() {

  {
    WEBFR_LOGI(TAG, "test default constructor...");
    webfr::Vector<webfr::String> vector;

    WEBFR_ASSERT(!vector);
    WEBFR_ASSERT(vector == nullptr);

    WEBFR_ASSERT(vector.get() == nullptr);
    WEBFR_ASSERT(vector.valueType->classId.id == webfr::data::mapping::type::__class::AbstractVector::CLASS_ID.id);
    WEBFR_ASSERT(vector.valueType->params.size() == 1);
    WEBFR_ASSERT(vector.valueType->params.front() == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test empty ilist constructor...");
    webfr::Vector<webfr::String> vector({});

    WEBFR_ASSERT(vector);
    WEBFR_ASSERT(vector != nullptr);
    WEBFR_ASSERT(vector->size() == 0);

    WEBFR_ASSERT(vector.get() != nullptr);
    WEBFR_ASSERT(vector.valueType->classId.id == webfr::data::mapping::type::__class::AbstractVector::CLASS_ID.id);
    WEBFR_ASSERT(vector.valueType->params.size() == 1);
    WEBFR_ASSERT(vector.valueType->params.front() == webfr::String::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test createShared()...");
    webfr::Vector<webfr::String> vector = webfr::Vector<webfr::String>::createShared();

    WEBFR_ASSERT(vector);
    WEBFR_ASSERT(vector != nullptr);
    WEBFR_ASSERT(vector->size() == 0);

    WEBFR_ASSERT(vector.get() != nullptr);
    WEBFR_ASSERT(vector.valueType->classId.id == webfr::data::mapping::type::__class::AbstractVector::CLASS_ID.id);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test copy-assignment operator...");
    webfr::Vector<webfr::String> vector1({});
    webfr::Vector<webfr::String> vector2;

    vector2 = vector1;

    WEBFR_ASSERT(vector1);
    WEBFR_ASSERT(vector2);

    WEBFR_ASSERT(vector1->size() == 0);
    WEBFR_ASSERT(vector2->size() == 0);

    WEBFR_ASSERT(vector1.get() == vector2.get());

    vector2->push_back("a");

    WEBFR_ASSERT(vector1->size() == 1);
    WEBFR_ASSERT(vector2->size() == 1);

    vector2 = {"b", "c"};

    WEBFR_ASSERT(vector1->size() == 1);
    WEBFR_ASSERT(vector2->size() == 2);

    WEBFR_ASSERT(vector2[0] == "b");
    WEBFR_ASSERT(vector2[1] == "c");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test move-assignment operator...");
    webfr::Vector<webfr::String> vector1({});
    webfr::Vector<webfr::String> vector2;

    vector2 = std::move(vector1);

    WEBFR_ASSERT(!vector1);
    WEBFR_ASSERT(vector2);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test get element by index...");
    webfr::Vector<webfr::String> vector = {"a", "b", "c"};

    WEBFR_ASSERT(vector);
    WEBFR_ASSERT(vector != nullptr);
    WEBFR_ASSERT(vector->size() == 3);

    WEBFR_ASSERT(vector[0] == "a");
    WEBFR_ASSERT(vector[1] == "b");
    WEBFR_ASSERT(vector[2] == "c");

    vector[1] = "Hello!";

    WEBFR_ASSERT(vector->size() == 3);

    WEBFR_ASSERT(vector[0] == "a");
    WEBFR_ASSERT(vector[1] == "Hello!");
    WEBFR_ASSERT(vector[2] == "c");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test polymorphicDispatcher...");
    webfr::Vector<webfr::String> vector = {"a", "b", "c"};

    auto polymorphicDispatcher = static_cast<const typename webfr::Vector<webfr::String>::Class::PolymorphicDispatcher*>(
      vector.valueType->polymorphicDispatcher
    );

    polymorphicDispatcher->addPolymorphicItem(vector, webfr::String("d"));

    WEBFR_ASSERT(vector->size() == 4);

    WEBFR_ASSERT(vector[0] == "a");
    WEBFR_ASSERT(vector[1] == "b");
    WEBFR_ASSERT(vector[2] == "c");
    WEBFR_ASSERT(vector[3] == "d");
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}
