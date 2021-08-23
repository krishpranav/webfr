#ifndef webfr_test_core_data_stream_ChunkedBufferTest_hpp
#define webfr_test_core_data_stream_ChunkedBufferTest_hpp

#include "webfr-test/UnitTest.hpp"

namespace oatpp { namespace test { namespace core { namespace data { namespace stream {

class ChunkedBufferTest : public UnitTest{
public:

  ChunkedBufferTest():UnitTest("TEST[core::data::stream::ChunkedBufferTest]"){}
  void onRun() override;

};

}}}}}


#endif //webfr_test_core_data_stream_ChunkedBufferTest_hpp
