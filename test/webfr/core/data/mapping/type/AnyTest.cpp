// includes
#include "AnyTest.hpp"
#include "webfr/core/data/mapping/type/Any.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/macro/codegen.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

namespace {

#include WEBFR_CODEGEN_BEGIN(DTO)

class Dto1 : public webfr::DTO {
  DTO_INIT(Dto1, DTO);
};

class Dto2 : public webfr::DTO {
  DTO_INIT(Dto2, DTO);
};

class Test : public webfr::DTO {

  DTO_INIT(Test, DTO);

  DTO_FIELD(webfr::Any, any);

};

#include WEBFR_CODEGEN_END(DTO)

}

void AnyTest::onRun() {

  {
    WEBFR_LOGI(TAG, "Test default constructor...");
    webfr::Any any;
    WEBFR_ASSERT(!any);
    WEBFR_ASSERT(any.valueType == webfr::data::mapping::type::__class::Any::getType());
    WEBFR_ASSERT(any.getStoredType() == nullptr);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test nullptr constructor...");
    webfr::Any any(nullptr);
    WEBFR_ASSERT(!any);
    WEBFR_ASSERT(any.valueType == webfr::data::mapping::type::__class::Any::getType());
    WEBFR_ASSERT(any.getStoredType() == nullptr);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test retrieve()...");
    webfr::Any any(webfr::String("Hello Any!"));
    WEBFR_ASSERT(any);
    WEBFR_ASSERT(any.valueType == webfr::data::mapping::type::__class::Any::getType());
    WEBFR_ASSERT(any.getStoredType() == webfr::data::mapping::type::__class::String::getType());
    auto str = any.retrieve<webfr::String>();
    WEBFR_ASSERT(str == "Hello Any!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test store()...");
    webfr::Any any(webfr::Int32(32));

    WEBFR_ASSERT(any);
    WEBFR_ASSERT(any.valueType == webfr::data::mapping::type::__class::Any::getType());
    WEBFR_ASSERT(any.getStoredType() == webfr::data::mapping::type::__class::Int32::getType());

    any.store(webfr::String("Hello Any!"));

    WEBFR_ASSERT(any);
    WEBFR_ASSERT(any.valueType == webfr::data::mapping::type::__class::Any::getType());
    WEBFR_ASSERT(any.getStoredType() == webfr::data::mapping::type::__class::String::getType());

    auto str = any.retrieve<webfr::String>();
    WEBFR_ASSERT(str == "Hello Any!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test retrieve() class check...");
    webfr::Any any(Dto1::createShared());
    WEBFR_ASSERT(any);
    WEBFR_ASSERT(any.valueType == webfr::data::mapping::type::__class::Any::getType());
    WEBFR_ASSERT(any.getStoredType() == Object<Dto1>::Class::getType());

    bool wasError = false;

    try {
      auto obj = any.retrieve<webfr::Object<Dto2>>(); // wrong object
    } catch (std::runtime_error&) {
      wasError = true;
    }

    WEBFR_ASSERT(wasError);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test copy-assign operator...");
    webfr::Any any1(webfr::String("Hello!"));
    webfr::Any any2;

    any2 = any1;

    WEBFR_ASSERT(any1);
    WEBFR_ASSERT(any2);

    WEBFR_ASSERT(any1.valueType == webfr::data::mapping::type::__class::Any::getType());
    WEBFR_ASSERT(any2.valueType == webfr::data::mapping::type::__class::Any::getType());

    WEBFR_ASSERT(any1.getStoredType() == webfr::data::mapping::type::__class::String::getType());
    WEBFR_ASSERT(any2.getStoredType() == webfr::data::mapping::type::__class::String::getType());

    WEBFR_ASSERT(any1 == any2);
    WEBFR_ASSERT(any1.getPtr().get() != any2.getPtr().get());

    auto str1 = any1.retrieve<webfr::String>();
    auto str2 = any2.retrieve<webfr::String>();

    WEBFR_ASSERT(str1 == str2);
    WEBFR_ASSERT(str1.get() == str2.get() && str1 == "Hello!");
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test move-assign operator...");
    webfr::Any any1(webfr::String("Hello!"));
    webfr::Any any2;

    any2 = std::move(any1);

    WEBFR_ASSERT(!any1);
    WEBFR_ASSERT(any2);

    WEBFR_ASSERT(any1.valueType == webfr::data::mapping::type::__class::Any::getType());
    WEBFR_ASSERT(any2.valueType == webfr::data::mapping::type::__class::Any::getType());

    WEBFR_ASSERT(any1.getStoredType() == nullptr);
    WEBFR_ASSERT(any2.getStoredType() == webfr::data::mapping::type::__class::String::getType());

    WEBFR_ASSERT(any1 != any2);
    WEBFR_ASSERT(any1.getPtr().get() != any2.getPtr().get());

    auto str1 = any1.retrieve<webfr::String>();
    auto str2 = any2.retrieve<webfr::String>();

    WEBFR_ASSERT(str1 != str2);
    WEBFR_ASSERT(str2 == "Hello!");
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}