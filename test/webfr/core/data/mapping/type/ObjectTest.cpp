// includes
#include "ObjectTest.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/Types.hpp"
#include "webfr-test/Checker.hpp"
#include <thread>

namespace webfr { namespace test { namespace core { namespace data { namespace mapping { namespace  type {

namespace {

#include WEBFR_CODEGEN_BEGIN(DTO)

class Dto0 : public webfr::DTO {
  DTO_INIT(Dto0, DTO)
};

class DtoA : public webfr::DTO {

  DTO_INIT(DtoA, DTO)

  DTO_FIELD_INFO(id) {
    info->description = "identifier";
    info->pattern = "^[a-z0-9]+$";
  }
  DTO_FIELD(String, id) = "Some default id";

  DTO_HC_EQ(id)

public:

  DtoA(const String& pId)
    : id(pId)
  {}

};

class DtoB : public DtoA {

  DTO_INIT(DtoB, DtoA)

  DTO_FIELD_INFO(a) {
    info->description = "some field with a qualified name";
  }
  DTO_FIELD(String, a, "field-a") = "default-value";

};

class DtoC : public DtoA {

  DTO_INIT(DtoC, DtoA)

  DTO_FIELD(String, a);
  DTO_FIELD(String, b);
  DTO_FIELD(String, c);

  DTO_HC_EQ(a, b, c);

};

#include WEBFR_CODEGEN_END(DTO)

void runDtoInitializations() {
  for(v_int32 i = 0; i < 1000; i ++) {
    auto dto = DtoB::createShared();
  }
}

void runDtoInitializetionsInThreads() {

  std::list<std::thread> threads;
  for(v_int32 i = 0; i < 500; i++) {
    threads.push_back(std::thread(runDtoInitializations));
  }

  for(auto& t : threads) {
    t.join();
  }

}

}

void ObjectTest::onRun() {

  {
    webfr::test::PerformanceChecker timer("DTO - Initializations.");
    runDtoInitializetionsInThreads();
  }

  {
    auto dto = DtoA::createShared("id1");
    WEBFR_ASSERT(dto->id == "id1");
  }

  {
    WEBFR_LOGI(TAG, "Test Meta 1...");

    auto type = Object<DtoA>::Class::getType();
    auto dispatcher = static_cast<const webfr::data::mapping::type::__class::AbstractObject::PolymorphicDispatcher*>(type->polymorphicDispatcher);
    const auto& propsMap = dispatcher->getProperties()->getMap();

    WEBFR_ASSERT(propsMap.size() == 1);

    auto it = propsMap.find("id");
    WEBFR_ASSERT(it != propsMap.end());
    WEBFR_ASSERT(it->second->info.description == "identifier");
    WEBFR_ASSERT(it->second->info.pattern == "^[a-z0-9]+$");

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test Meta 2...");

    auto type = Object<DtoB>::Class::getType();
    auto dispatcher = static_cast<const webfr::data::mapping::type::__class::AbstractObject::PolymorphicDispatcher*>(type->polymorphicDispatcher);
    const auto& propsMap = dispatcher->getProperties()->getMap();

    WEBFR_ASSERT(propsMap.size() == 2);

    {
      auto it = propsMap.find("id");
      WEBFR_ASSERT("id" && it != propsMap.end());
      WEBFR_ASSERT(it->second->info.description == "identifier");
    }

    {
      auto it = propsMap.find("field-a");
      WEBFR_ASSERT("field-a" && it != propsMap.end());
      WEBFR_ASSERT(it->second->info.description == "some field with a qualified name");
    }

    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test 1...");
    Object<DtoA> a;
    WEBFR_ASSERT(!a);
    WEBFR_ASSERT(a == nullptr);
    WEBFR_ASSERT(a.valueType->classId.id == webfr::data::mapping::type::__class::AbstractObject::CLASS_ID.id);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test 2...");
    Object<DtoA> a;
    Object<DtoA> b;
    WEBFR_ASSERT(a == b);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test 3...");
    auto a = DtoA::createShared();
    Object<DtoA> b;
    WEBFR_ASSERT(a != b);
    WEBFR_ASSERT(b != a);
    auto ohc = a->hashCode();
    auto whc = std::hash<webfr::Object<DtoA>>{}(a);
    WEBFR_ASSERT(ohc == whc);
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test 4...");
    auto a = Dto0::createShared();
    auto b = Dto0::createShared();
    WEBFR_ASSERT(a != b);
    WEBFR_ASSERT(a->hashCode() != b->hashCode());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test 5...");
    auto a = DtoA::createShared();
    auto b = DtoA::createShared();

    WEBFR_ASSERT(a == b);
    WEBFR_ASSERT(a->hashCode() == b->hashCode());

    a->id = "hello";

    WEBFR_ASSERT(a != b);
    WEBFR_ASSERT(a->hashCode() != b->hashCode());

    b->id = "hello";

    WEBFR_ASSERT(a == b);
    WEBFR_ASSERT(a->hashCode() == b->hashCode());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test 6...");
    auto a = DtoB::createShared();
    auto b = DtoB::createShared();

    WEBFR_ASSERT(a->a == "default-value");
    WEBFR_ASSERT(b->a == "default-value");

    a->a = "value1"; // value that is ignored in HC & EQ
    a->a = "value2"; // value that is ignored in HC & EQ

    WEBFR_ASSERT(a == b);
    WEBFR_ASSERT(a->hashCode() == b->hashCode());

    a->id = "hello";

    WEBFR_ASSERT(a != b);
    WEBFR_ASSERT(a->hashCode() != b->hashCode());

    b->id = "hello";

    WEBFR_ASSERT(a == b);
    WEBFR_ASSERT(a->hashCode() == b->hashCode());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test 7...");
    auto a = DtoC::createShared();
    auto b = DtoC::createShared();

    a->id = "1";
    b->id = "2";

    WEBFR_ASSERT(a != b);
    WEBFR_ASSERT(a->hashCode() != b->hashCode());

    a->id = "2";

    WEBFR_ASSERT(a == b);
    WEBFR_ASSERT(a->hashCode() == b->hashCode());

    a->c = "a";

    WEBFR_ASSERT(a != b);
    WEBFR_ASSERT(a->hashCode() != b->hashCode());

    b->c = "a";

    WEBFR_ASSERT(a == b);
    WEBFR_ASSERT(a->hashCode() == b->hashCode());
    WEBFR_LOGI(TAG, "OK");
  }

  {
    WEBFR_LOGI(TAG, "Test 8...");
    auto a = DtoB::createShared();
    auto b = DtoB::createShared();
    auto c = DtoB::createShared();
    auto d = DtoB::createShared();
    auto e = DtoB::createShared();

    a->a = "1";
    b->a = "2";
    c->a = "3";
    d->a = "4";
    e->a = "5";

    a->id = "1";
    e->id = "1";

    webfr::UnorderedSet<webfr::Object<DtoB>> set = {a, b, c, d, e};

    WEBFR_ASSERT(set->size() == 2);
    WEBFR_ASSERT(set[a] == true);
    WEBFR_ASSERT(set[b] == true);
    WEBFR_ASSERT(set[c] == true);
    WEBFR_ASSERT(set[d] == true);
    WEBFR_ASSERT(set[e] == true);
    WEBFR_LOGI(TAG, "OK");
  }

}

}}}}}}
