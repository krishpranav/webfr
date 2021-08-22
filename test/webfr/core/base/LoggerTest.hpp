#ifndef webfr_test_base_LoggerTest_hpp
#define webfr_test_base_LoggerTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace base {

class LoggerTest : public UnitTest{
 public:

  LoggerTest():UnitTest("TEST[base::LoggerTest]"){}
  void onRun() override;

  WEBFR_DECLARE_LOG_CATEGORY(TESTCATEGORY);
};

}}}

#endif 