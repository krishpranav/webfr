#ifndef webfr_test_UnitTest_hpp
#define webfr_test_UnitTest_hpp

// includes
#include <functional>
#include "webfr/core/base/Environment.hpp"

namespace webfr { namespace test {

class UnitTest{
protected:
  const char* const TAG;
public:

  UnitTest(const char* testTAG)
    : TAG(testTAG)
  {}

  virtual ~UnitTest() = default;

  void run(v_int32 times);

  void run(){
    run(1);
  }

  virtual void onRun() = 0;

  template<class T>
  static void runTest(v_int32 times){
    T test;
    test.run(times);
  }
  
};

#define WEBFR_RUN_TEST(TEST) \
webfr::test::UnitTest::runTest<TEST>(1)
  
}}

#endif 