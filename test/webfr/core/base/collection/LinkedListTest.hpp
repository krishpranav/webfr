#ifndef webfr_test_base_collection_LinkedListTest_hpp
#define webfr_test_base_collection_LinkedListTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace collection {
  
class LinkedListTest : public UnitTest{
public:
  
  LinkedListTest():UnitTest("TEST[webfr::collection::LinkedListTest]"){}
  void onRun() override;
  
};
  
}}}

#endif 