#ifndef webfr_test_web_protocol_http_encoding_ChunkedTest_hpp
#define webfr_test_web_protocol_http_encoding_ChunkedTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace web { namespace protocol { namespace http { namespace encoding {

class ChunkedTest : public UnitTest {
public:

  ChunkedTest():UnitTest("TEST[web::protocol::http::encoding::ChunkedTest]"){}
  void onRun() override;

};

}}}}}}

#endif 