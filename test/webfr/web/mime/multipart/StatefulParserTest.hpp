#ifndef webfr_test_web_mime_multipart_StatefulParserTest_hpp
#define webfr_test_web_mime_multipart_StatefulParserTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web { namespace mime { namespace multipart {

class StatefulParserTest : public UnitTest {
public:

  StatefulParserTest():UnitTest("TEST[web::mime::multipart::StatefulParserTest]"){}
  void onRun() override;

};

}}}}}

#endif 