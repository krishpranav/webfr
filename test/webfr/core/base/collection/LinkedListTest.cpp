#include "LinkedListTest.hpp"
#include "webfr/core/Types.hpp"
#include "webfr/core/collection/LinkedList.hpp"
#include "webfr-test/Checker.hpp"
#include <list>

namespace webfr { namespace test { namespace collection {
  
namespace {
  
class TestObject : public webfr::base::Countable {
public:
  SHARED_OBJECT_POOL(TestObject_Pool2, TestObject, 32)
public:
  TestObject()
  {}
public:
  
  static std::shared_ptr<TestObject> createShared2(){
    return std::make_shared<TestObject>();
  }
  
  static std::shared_ptr<TestObject> createShared(){
    return TestObject_Pool2::allocateShared();
  }
  
};
  
void testListPerformance(v_int32 iterationsCount){
  
  auto list = webfr::collection::LinkedList<std::shared_ptr<TestObject>>::createShared();
  
  for(v_int32 i = 0; i < iterationsCount; i++){
    list->pushBack(TestObject::createShared());
  }
  
  auto curr = list->getFirstNode();
  while (curr != nullptr) {
    auto data = curr->getData();
    curr = curr->getNext();
  }
  
}
  
void testStdListPerformance(v_int32 iterationsCount){
  
  std::list<std::shared_ptr<TestObject>> list;
  
  for(v_int32 i = 0; i < iterationsCount; i++){
    list.push_back(TestObject::createShared());
  }
  
  auto it = list.begin();
  while (it != list.end()) {
    auto data = *it;
    it++;
  }
  
}
  
}
  
void LinkedListTest::onRun() {
  
  
  v_int32 iterationsCount = 100000;
  
  {
    PerformanceChecker checker("webfr::collection::LinkedList pushBack time");
    for(v_int32 i = 0; i < 10; i ++) {
      testListPerformance(iterationsCount);
    }
  }
  
  {
    PerformanceChecker checker("std::list pushBack time");
    for(v_int32 i = 0; i < 10; i ++) {
      testStdListPerformance(iterationsCount);
    }
  }

}
  
}}}