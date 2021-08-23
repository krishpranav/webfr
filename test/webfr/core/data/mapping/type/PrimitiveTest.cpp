// includes
#include "PrimitiveTest.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

namespace {

  template<class T>
  void checkHash(const T& val) {
    auto h = std::hash<T>{}(val);
    WEBFR_LOGI("HASH", "type='%s', hash=%llu", val.valueType->classId.name, h);
  }

}

void PrimitiveTest::onRun() {

  {
    checkHash(webfr::Boolean(true));
    checkHash(webfr::Int8(0x7F));
    checkHash(webfr::UInt8(0xFF));
    checkHash(webfr::Int16(0x7FFF));
    checkHash(webfr::UInt16(0xFFFF));
    checkHash(webfr::Int32(0x7FFFFFFF));
    checkHash(webfr::UInt32(0xFFFFFFFF));
    checkHash(webfr::Int64(0x7FFFFFFFFFFFFFFF));
    checkHash(webfr::UInt64(0xFFFFFFFFFFFFFFFF));
    checkHash(webfr::Float32(0.2f));
    checkHash(webfr::Float64(0.2));
  }

  {
    WEBFR_LOGI(TAG, "test default constructor");
    webfr::Int32 i;
    WEBFR_ASSERT(!i);
    WEBFR_ASSERT(i == nullptr);
    WEBFR_ASSERT(i.valueType == webfr::Int32::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test value constructor");
    webfr::Int32 i(0);
    WEBFR_ASSERT(i);
    WEBFR_ASSERT(i != nullptr);
    WEBFR_ASSERT(i == 0);
    WEBFR_ASSERT(i.valueType == webfr::Int32::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test implicit value constructor");
    webfr::Int32 i = 0;
    WEBFR_ASSERT(i);
    WEBFR_ASSERT(i != nullptr);
    WEBFR_ASSERT(i == 0);
    WEBFR_ASSERT(i.valueType == webfr::Int32::Class::getType());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test '==' and '!=' operators");
    webfr::Int32 i1 = 0;
    webfr::Int32 i2;

    WEBFR_ASSERT(i1);
    WEBFR_ASSERT(i1 != nullptr);
    WEBFR_ASSERT(i1 == 0);
    WEBFR_ASSERT(i1 != 1);

    WEBFR_ASSERT(!i2);
    WEBFR_ASSERT(i2 == nullptr)

    WEBFR_ASSERT(i1 != i2);
    WEBFR_ASSERT(i2 != i1);

    i2 = 0;

    WEBFR_ASSERT(i1 == i2);
    WEBFR_ASSERT(i2 == i1);

    i1 = nullptr;
    i2 = nullptr;

    WEBFR_ASSERT(i1 == i2);

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "test copy-assign operator");
    webfr::Int32 i1 = 0;
    webfr::Int32 i2;

    WEBFR_ASSERT(i1 != i2);

    i2 = i1;

    WEBFR_ASSERT(i1 == i2);
    WEBFR_ASSERT(i1.get() == i2.get());

    i2 = 1;

    WEBFR_ASSERT(i1 != i2);
    WEBFR_ASSERT(i1.get() != i2.get());

    WEBFR_ASSERT(i1 == 0);
    WEBFR_ASSERT(i2 == 1);
  }

  {
    WEBFR_LOGI(TAG, "test move-assign operator");
    webfr::Int32 i1 = 0;
    webfr::Int32 i2;

    WEBFR_ASSERT(i1 != i2);

    i2 = std::move(i1);

    WEBFR_ASSERT(i1 == nullptr);
    WEBFR_ASSERT(i2 != nullptr);

    WEBFR_ASSERT(i2 == 0);
  }

  {
    WEBFR_LOGI(TAG, "test move-assign operator");
    webfr::Int32 i = 0;
    v_int32 v = i;
    WEBFR_ASSERT(v == i);
  }

  {
    WEBFR_LOGI(TAG, "Test Boolean [nullptr]");
    webfr::Boolean b;

    WEBFR_ASSERT(!b);
    WEBFR_ASSERT(b == nullptr);
    WEBFR_ASSERT(b != false);
    WEBFR_ASSERT(b != true);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test Boolean nullptr constructor");
    webfr::Boolean b = nullptr;

    WEBFR_ASSERT(!b);
    WEBFR_ASSERT(b == nullptr);
    WEBFR_ASSERT(b != false);
    WEBFR_ASSERT(b != true);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test Boolean [false]");
    webfr::Boolean b = false;

    WEBFR_ASSERT(!b); // <--- still !b
    WEBFR_ASSERT(b != nullptr);
    WEBFR_ASSERT(b == false);
    WEBFR_ASSERT(b != true);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test Boolean [true]");
    webfr::Boolean b = true;

    WEBFR_ASSERT(b);
    WEBFR_ASSERT(b != nullptr);
    WEBFR_ASSERT(b != false);
    WEBFR_ASSERT(b == true);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test Boolean copy-assign operator");
    webfr::Boolean b1 = true;
    webfr::Boolean b2;

    b2 = b1;

    WEBFR_ASSERT(b2);
    WEBFR_ASSERT(b1);
    WEBFR_ASSERT(b1 == b2);
    WEBFR_ASSERT(b1.get() == b2.get());

    b2 = false;

    WEBFR_ASSERT(b1.get() != b2.get());
    WEBFR_ASSERT(b1 != b2);
    WEBFR_ASSERT(b2 != b1);

    b1 = false;
    b2 = nullptr;

    WEBFR_ASSERT(b1 != b2);
    WEBFR_ASSERT(b2 != b1);

    b1 = nullptr;
    b2 = nullptr;

    WEBFR_ASSERT(b1 == b2);

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test Boolean move-assign operator");
    webfr::Boolean b1 = true;
    webfr::Boolean b2;

    b2 = std::move(b1);

    WEBFR_ASSERT(b2 != nullptr);
    WEBFR_ASSERT(b1 == nullptr);

    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}