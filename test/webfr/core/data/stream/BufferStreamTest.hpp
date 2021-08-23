#ifndef webfr_test_core_data_stream_BufferStream_hpp
#define webfr_test_core_data_stream_BufferStream_hpp

#include "webfr-test/UnitTest.hpp"

namespace webfr { namespace test { namespace core { namespace data { namespace stream {

class BufferStreamTest : public UnitTest{
public:

  BufferStreamTest():UnitTest("TEST[core::data::stream::BufferStreamTest]"){}
  void onRun() override;

};

}}}}}


#endif 