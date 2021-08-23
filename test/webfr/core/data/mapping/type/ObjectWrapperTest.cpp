#include "ObjectWrapperTest.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

namespace {

  template<class T, class Clazz = webfr::data::mapping::type::__class::Void>
  using ObjectWrapper = webfr::data::mapping::type::ObjectWrapper<T, Clazz>;

}

void ObjectWrapperTest::onRun() {

  {
    WEBFR_LOGI(TAG, "Check default valueType is assigned (default tparam Clazz)...");
    ObjectWrapper<base::StrBuffer> pw;
    WEBFR_ASSERT(!pw);
    WEBFR_ASSERT(pw == nullptr);
    WEBFR_ASSERT(pw.valueType == webfr::data::mapping::type::__class::Void::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check default valueType is assigned (specified tparam Clazz)...");
    ObjectWrapper<base::StrBuffer, webfr::data::mapping::type::__class::String> pw;
    WEBFR_ASSERT(!pw);
    WEBFR_ASSERT(pw == nullptr);
    WEBFR_ASSERT(pw.valueType == webfr::data::mapping::type::__class::String::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check valueType is assigned from constructor...");
    ObjectWrapper<base::StrBuffer> pw(webfr::data::mapping::type::__class::String::getType());
    WEBFR_ASSERT(!pw);
    WEBFR_ASSERT(pw == nullptr);
    WEBFR_ASSERT(pw.valueType == webfr::data::mapping::type::__class::String::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check valueType is assigned from copy constructor...");
    ObjectWrapper<base::StrBuffer> pw1(webfr::data::mapping::type::__class::String::getType());
    ObjectWrapper<base::StrBuffer> pw2(pw1);
    WEBFR_ASSERT(pw2.valueType == webfr::data::mapping::type::__class::String::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check valueType is assigned from move constructor...");
    ObjectWrapper<base::StrBuffer> pw1(webfr::data::mapping::type::__class::String::getType());
    ObjectWrapper<base::StrBuffer> pw2(std::move(pw1));
    WEBFR_ASSERT(pw2.valueType == webfr::data::mapping::type::__class::String::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check valueType is NOT assigned from copy-assign operator...");
    ObjectWrapper<base::StrBuffer> pw1(webfr::data::mapping::type::__class::String::getType());
    ObjectWrapper<base::StrBuffer> pw2;
    pw2 = pw1;
    WEBFR_ASSERT(pw2.valueType == webfr::data::mapping::type::__class::Void::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check valueType is NOT assigned from move-assign operator...");
    ObjectWrapper<base::StrBuffer> pw1(webfr::data::mapping::type::__class::String::getType());
    ObjectWrapper<base::StrBuffer> pw2;
    pw2 = std::move(pw1);
    WEBFR_ASSERT(pw2.valueType == webfr::data::mapping::type::__class::Void::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check copy-assign operator. Check == operator...");
    ObjectWrapper<base::StrBuffer> pw1;
    WEBFR_ASSERT(!pw1);
    WEBFR_ASSERT(pw1 == nullptr);
    WEBFR_ASSERT(pw1.valueType == webfr::data::mapping::type::__class::Void::getType());

    ObjectWrapper<base::StrBuffer> pw2 = base::StrBuffer::createShared("Hello!");
    WEBFR_ASSERT(pw2);
    WEBFR_ASSERT(pw2 != nullptr);
    WEBFR_ASSERT(pw2.valueType == webfr::data::mapping::type::__class::Void::getType());

    pw1 = pw2;

    WEBFR_ASSERT(pw1);
    WEBFR_ASSERT(pw1 != nullptr);

    WEBFR_ASSERT(pw2);
    WEBFR_ASSERT(pw2 != nullptr);

    WEBFR_ASSERT(pw1 == pw2);
    WEBFR_ASSERT(pw1.get() == pw2.get());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check != operator...");
    ObjectWrapper<base::StrBuffer, webfr::data::mapping::type::__class::String> pw1(base::StrBuffer::createShared("Hello!"));
    WEBFR_ASSERT(pw1);
    WEBFR_ASSERT(pw1 != nullptr);
    WEBFR_ASSERT(pw1.valueType == webfr::data::mapping::type::__class::String::getType());

    ObjectWrapper<base::StrBuffer, webfr::data::mapping::type::__class::String> pw2(base::StrBuffer::createShared("Hello!"));
    WEBFR_ASSERT(pw2);
    WEBFR_ASSERT(pw2 != nullptr);
    WEBFR_ASSERT(pw2.valueType == webfr::data::mapping::type::__class::String::getType());

    WEBFR_ASSERT(pw1 != pw2);
    WEBFR_ASSERT(pw1.get() != pw2.get());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check move-assign operator. Check != operator...");
    ObjectWrapper<base::StrBuffer> pw1;
    WEBFR_ASSERT(!pw1);
    WEBFR_ASSERT(pw1 == nullptr);
    WEBFR_ASSERT(pw1.valueType == webfr::data::mapping::type::__class::Void::getType());

    ObjectWrapper<base::StrBuffer> pw2 = base::StrBuffer::createShared("Hello!");
    WEBFR_ASSERT(pw2);
    WEBFR_ASSERT(pw2 != nullptr);
    WEBFR_ASSERT(pw2.valueType == webfr::data::mapping::type::__class::Void::getType());

    pw1 = std::move(pw2);

    WEBFR_ASSERT(pw1);
    WEBFR_ASSERT(pw1 != nullptr);

    WEBFR_ASSERT(!pw2);
    WEBFR_ASSERT(pw2 == nullptr);

    WEBFR_ASSERT(pw1 != pw2);
    WEBFR_ASSERT(pw1.get() != pw2.get());
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}
