// includes
#include "EnumTest.hpp"
#include "webfr/core/Types.hpp"
#include "webfr/core/macro/codegen.hpp"
#include <unordered_map>

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

#include WEBFR_CODEGEN_BEGIN(DTO)

ENUM(Enum0, v_int32)

ENUM(Enum1, v_int32,
     VALUE(V1, 1),
     VALUE(V2, 2),
     VALUE(V3, 3)
)

ENUM(Enum2, v_int32,
  VALUE(NAME_1, 1, "name-1"),
  VALUE(NAME_2, 2, "name-2"),
  VALUE(NAME_3, 3, "name-3")
)

ENUM(Enum3, v_int32,
   VALUE(V_1, 1, "v-1", "description_1"),
   VALUE(V_2, 2, "v-2", "description_2"),
   VALUE(V_3, 3, "v-3", "description_3")
)

#include WEBFR_CODEGEN_END(DTO)

void EnumTest::onRun() {

  {
    WEBFR_LOGI(TAG, "Check Hash...");

    {
      auto v = std::hash<webfr::Enum<Enum1>>{}(webfr::Enum<Enum1>());
      WEBFR_ASSERT(v == 0);
    }

    {
      auto v = std::hash<webfr::Enum<Enum1>>{}(webfr::Enum<Enum1>(Enum1::V1));
      WEBFR_ASSERT(v == 1);
    }

    {
      auto v = std::hash<webfr::Enum<Enum1>>{}(webfr::Enum<Enum1>(Enum1::V2));
      WEBFR_ASSERT(v == 2);
    }

    {
      auto v = std::hash<webfr::Enum<Enum1>>{}(webfr::Enum<Enum1>(Enum1::V3));
      WEBFR_ASSERT(v == 3);
    }

    std::unordered_map<webfr::Enum<Enum1>, webfr::String> map({
      {Enum1::V1, "v1"},
      {Enum1::V2, "v2"},
      {Enum1::V3, "v3"},
    });

    WEBFR_ASSERT(map.size() == 3);
    WEBFR_ASSERT(map[Enum1::V1] == "v1");
    WEBFR_ASSERT(map[Enum1::V2] == "v2");
    WEBFR_ASSERT(map[Enum1::V3] == "v3");

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Check Meta...");
    {
      const auto &entries = webfr::Enum<Enum0>::getEntries();
      WEBFR_ASSERT(entries.size() == 0);
    }

    {
      const auto &v = webfr::Enum<Enum1>::getEntries();
      WEBFR_ASSERT(v.size() == 3);
      WEBFR_ASSERT(v[0].index == 0 && v[0].name == "V1" && v[0].value == Enum1::V1 && v[0].description == nullptr);
      WEBFR_ASSERT(v[1].index == 1 && v[1].name == "V2" && v[1].value == Enum1::V2 && v[1].description == nullptr);
      WEBFR_ASSERT(v[2].index == 2 && v[2].name == "V3" && v[2].value == Enum1::V3 && v[2].description == nullptr);
    }

    {
      const auto &v = webfr::Enum<Enum2>::getEntries();
      WEBFR_ASSERT(v.size() == 3);
      WEBFR_ASSERT(v[0].index == 0 && v[0].name == "name-1" && v[0].value == Enum2::NAME_1 && v[0].description == nullptr);
      WEBFR_ASSERT(v[1].index == 1 && v[1].name == "name-2" && v[1].value == Enum2::NAME_2 && v[1].description == nullptr);
      WEBFR_ASSERT(v[2].index == 2 && v[2].name == "name-3" && v[2].value == Enum2::NAME_3 && v[2].description == nullptr);
    }

    {
      const auto &v = webfr::Enum<Enum3>::getEntries();
      WEBFR_ASSERT(v.size() == 3);
      WEBFR_ASSERT(v[0].index == 0 && v[0].name == "v-1" && v[0].value == Enum3::V_1 && v[0].description == "description_1");
      WEBFR_ASSERT(v[1].index == 1 && v[1].name == "v-2" && v[1].value == Enum3::V_2 && v[1].description == "description_2");
      WEBFR_ASSERT(v[2].index == 2 && v[2].name == "v-3" && v[2].value == Enum3::V_3 && v[2].description == "description_3");
    }

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Declaration...");
    WEBFR_ASSERT(webfr::Enum<Enum2>::Interpreter::notNull == false);
    WEBFR_ASSERT(webfr::Enum<Enum2>::NotNull::Interpreter::notNull == true);

    WEBFR_ASSERT(webfr::Enum<Enum2>::AsString::Interpreter::notNull == false);
    WEBFR_ASSERT(webfr::Enum<Enum2>::AsString::NotNull::Interpreter::notNull == true);

    WEBFR_ASSERT(webfr::Enum<Enum2>::AsNumber::Interpreter::notNull == false);
    WEBFR_ASSERT(webfr::Enum<Enum2>::AsNumber::NotNull::Interpreter::notNull == true);

    WEBFR_ASSERT(webfr::Enum<Enum2>::NotNull::AsString::Interpreter::notNull == true);
    WEBFR_ASSERT(webfr::Enum<Enum2>::NotNull::AsNumber::Interpreter::notNull == true);

    auto pd1 = static_cast<const webfr::data::mapping::type::__class::AbstractEnum::PolymorphicDispatcher*>(
      webfr::Enum<Enum2>::Class::getType()->polymorphicDispatcher
    );

    auto pd2 = static_cast<const webfr::data::mapping::type::__class::AbstractEnum::PolymorphicDispatcher*>(
      webfr::Enum<Enum2>::NotNull::Class::getType()->polymorphicDispatcher
    );

    WEBFR_ASSERT(pd1->notNull == false);
    WEBFR_ASSERT(pd2->notNull == true);

    {
      auto interEnum = pd1->getInterpretedEnum();
      WEBFR_ASSERT(interEnum.size() == 3);
      WEBFR_ASSERT(interEnum[0].getStoredType() == webfr::String::Class::getType());
    }

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test Interpreter AsString...");
    webfr::data::mapping::type::EnumInterpreterError e = webfr::data::mapping::type::EnumInterpreterError::OK;
    auto inter = webfr::Enum<Enum2>::AsString::Interpreter::toInterpretation(webfr::Enum<Enum2>::AsString(Enum2::NAME_1), e);
    WEBFR_ASSERT(inter.valueType == webfr::String::Class::getType());
    WEBFR_ASSERT(e == webfr::data::mapping::type::EnumInterpreterError::OK);

    auto interValue = inter.staticCast<webfr::String>();
    WEBFR_ASSERT(interValue == "name-1");

    webfr::Void voidValue = webfr::Enum<Enum2>::AsString::Interpreter::fromInterpretation(interValue, e);
    WEBFR_ASSERT(voidValue.valueType == webfr::Enum<Enum2>::AsString::Class::getType());
    WEBFR_ASSERT(e == webfr::data::mapping::type::EnumInterpreterError::OK);

    auto value = voidValue.staticCast<webfr::Enum<Enum2>::AsString>();
    WEBFR_ASSERT(value == Enum2::NAME_1);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test Interpreter AsNumber...");
    webfr::data::mapping::type::EnumInterpreterError e = webfr::data::mapping::type::EnumInterpreterError::OK;
    auto inter = webfr::Enum<Enum2>::AsNumber::Interpreter::toInterpretation(webfr::Enum<Enum2>::AsNumber(Enum2::NAME_1), e);
    WEBFR_ASSERT(inter.valueType == webfr::Int32::Class::getType());
    WEBFR_ASSERT(e == webfr::data::mapping::type::EnumInterpreterError::OK);

    auto interValue = inter.staticCast<webfr::Int32>();
    WEBFR_ASSERT(interValue == static_cast<v_int32>(Enum2::NAME_1));

    webfr::Void voidValue = webfr::Enum<Enum2>::AsNumber::Interpreter::fromInterpretation(interValue, e);
    WEBFR_ASSERT(voidValue.valueType == webfr::Enum<Enum2>::AsNumber::Class::getType());
    WEBFR_ASSERT(e == webfr::data::mapping::type::EnumInterpreterError::OK);

    auto value = voidValue.staticCast<webfr::Enum<Enum2>::AsNumber>();
    WEBFR_ASSERT(value == Enum2::NAME_1);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test default constructors and == operators...");
    webfr::Enum<Enum2>::AsString e1;
    webfr::Enum<Enum2>::AsString e2;

    WEBFR_ASSERT(!e1);
    WEBFR_ASSERT(e1 == nullptr);
    WEBFR_ASSERT(e1 == e2);
    WEBFR_ASSERT(e2 == e1);

    webfr::Enum<Enum2>::NotNull e3;

    WEBFR_ASSERT(e1 == e3);
    WEBFR_ASSERT(e3 == e1);

    webfr::Enum<Enum2>::AsNumber::NotNull e4;

    WEBFR_ASSERT(e1 == e4);
    WEBFR_ASSERT(e4 == e1);

    WEBFR_ASSERT(e1.valueType != e4.valueType); // Types are not equal because interpreters are different
    WEBFR_ASSERT(e1.valueType->classId.id == e4.valueType->classId.id); // But classId is the same

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test value constructor and == operators...");
    webfr::Enum<Enum2> e1(Enum2::NAME_1);
    webfr::Enum<Enum2> e2(Enum2::NAME_1);
    webfr::Enum<Enum2> e3;

    WEBFR_ASSERT(e1);
    WEBFR_ASSERT(e1 != nullptr);
    WEBFR_ASSERT(e1 == e2);
    WEBFR_ASSERT(e1 != e3);
    WEBFR_ASSERT(e3 != e1);

    WEBFR_ASSERT(e1 == Enum2::NAME_1);
    WEBFR_ASSERT(e1 != Enum2::NAME_2);
    WEBFR_ASSERT(e3 != Enum2::NAME_1);

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test copy-assignment operator...");
    webfr::Enum<Enum2>::AsString e1;
    webfr::Enum<Enum2>::AsNumber e2(Enum2::NAME_1);
    Enum2 ve;

    WEBFR_ASSERT(e1.valueType == webfr::Enum<Enum2>::AsString::Class::getType());
    WEBFR_ASSERT(e2.valueType == webfr::Enum<Enum2>::AsNumber::Class::getType());
    WEBFR_ASSERT(e1.valueType != e2.valueType);

    e1 = e2;

    WEBFR_ASSERT(e1.valueType == webfr::Enum<Enum2>::AsString::Class::getType());
    WEBFR_ASSERT(e2.valueType == webfr::Enum<Enum2>::AsNumber::Class::getType());
    WEBFR_ASSERT(e1.valueType != e2.valueType);

    WEBFR_ASSERT(e1 == e2);
    WEBFR_ASSERT(e2 == e1);
    WEBFR_ASSERT(e1.get() == e2.get());

    e1 = Enum2::NAME_2;

    WEBFR_ASSERT(e1 != e2);
    WEBFR_ASSERT(e2 != e1);
    WEBFR_ASSERT(e1.get() != e2.get());

    WEBFR_ASSERT(e1 == Enum2::NAME_2);
    WEBFR_ASSERT(e2 == Enum2::NAME_1);

    ve = e1;

    WEBFR_ASSERT(ve == Enum2::NAME_2);

    ve = e2;

    WEBFR_ASSERT(ve == Enum2::NAME_1);

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test move-assignment operator...");
    webfr::Enum<Enum2>::AsString e1;
    webfr::Enum<Enum2>::AsNumber e2(Enum2::NAME_1);

    e1 = std::move(e2);

    WEBFR_ASSERT(e1);
    WEBFR_ASSERT(!e2);

    WEBFR_ASSERT(e1 == Enum2::NAME_1);

    WEBFR_LOGI(TAG, "OK");
  }


}

}}}}}}
