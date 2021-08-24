#include "webfr/web/ClientRetryTest.hpp"
#include "webfr/web/FullTest.hpp"
#include "webfr/web/FullAsyncTest.hpp"
#include "webfr/web/FullAsyncClientTest.hpp"
#include "webfr/web/PipelineTest.hpp"
#include "webfr/web/PipelineAsyncTest.hpp"
#include "webfr/web/protocol/http/encoding/ChunkedTest.hpp"
#include "webfr/web/server/api/ApiControllerTest.hpp"
#include "webfr/web/server/handler/AuthorizationHandlerTest.hpp"
#include "webfr/web/server/HttpRouterTest.hpp"
#include "webfr/web/mime/multipart/StatefulParserTest.hpp"
#include "webfr/network/virtual_/PipeTest.hpp"
#include "webfr/network/virtual_/InterfaceTest.hpp"
#include "webfr/network/UrlTest.hpp"
#include "webfr/network/ConnectionPoolTest.hpp"
#include "webfr/parser/json/mapping/DeserializerTest.hpp"
#include "webfr/parser/json/mapping/DTOMapperPerfTest.hpp"
#include "webfr/parser/json/mapping/DTOMapperTest.hpp"
#include "webfr/parser/json/mapping/EnumTest.hpp"
#include "webfr/parser/json/mapping/UnorderedSetTest.hpp"
#include "webfr/encoding/UnicodeTest.hpp"
#include "webfr/encoding/Base64Test.hpp"
#include "webfr/core/parser/CaretTest.hpp"
#include "webfr/core/provider/PoolTest.hpp"
#include "webfr/core/async/LockTest.hpp"
#include "webfr/core/data/mapping/type/UnorderedMapTest.hpp"
#include "webfr/core/data/mapping/type/PairListTest.hpp"
#include "webfr/core/data/mapping/type/VectorTest.hpp"
#include "webfr/core/data/mapping/type/UnorderedSetTest.hpp"
#include "webfr/core/data/mapping/type/ListTest.hpp"
#include "webfr/core/data/mapping/type/ObjectTest.hpp"
#include "webfr/core/data/mapping/type/StringTest.hpp"
#include "webfr/core/data/mapping/type/PrimitiveTest.hpp"
#include "webfr/core/data/mapping/type/ObjectWrapperTest.hpp"
#include "webfr/core/data/mapping/type/TypeTest.hpp"
#include "webfr/core/data/mapping/type/AnyTest.hpp"
#include "webfr/core/data/mapping/type/EnumTest.hpp"
#include "webfr/core/data/mapping/type/InterpretationTest.hpp"
#include "webfr/core/data/mapping/TypeResolverTest.hpp"
#include "webfr/core/data/stream/BufferStreamTest.hpp"
#include "webfr/core/data/stream/ChunkedBufferTest.hpp"
#include "webfr/core/data/share/LazyStringMapTest.hpp"
#include "webfr/core/data/share/StringTemplateTest.hpp"
#include "webfr/core/data/share/MemoryLabelTest.hpp"
#include "webfr/core/data/buffer/ProcessorTest.hpp"
#include "webfr/core/base/collection/LinkedListTest.hpp"
#include "webfr/core/base/memory/MemoryPoolTest.hpp"
#include "webfr/core/base/memory/PerfTest.hpp"
#include "webfr/core/base/CommandLineArgumentsTest.hpp"
#include "webfr/core/base/LoggerTest.hpp"
#include "webfr/core/async/Coroutine.hpp"
#include "webfr/core/Types.hpp"
#include "webfr/core/concurrency/SpinLock.hpp"
#include "webfr/core/base/Environment.hpp"
#include <iostream>
#include <mutex>

namespace {

void runTests() {

  webfr::base::Environment::printCompilationConfig();

  WEBFR_LOGD("Tests", "coroutine size=%d", sizeof(webfr::async::AbstractCoroutine));
  WEBFR_LOGD("Tests", "action size=%d", sizeof(webfr::async::Action));
  WEBFR_LOGD("Tests", "class count=%d", webfr::data::mapping::type::ClassId::getClassCount());

  WEBFR_RUN_TEST(webfr::test::base::CommandLineArgumentsTest);
  WEBFR_RUN_TEST(webfr::test::base::LoggerTest);

  WEBFR_RUN_TEST(webfr::test::memory::MemoryPoolTest);
  WEBFR_RUN_TEST(webfr::test::memory::PerfTest);

  WEBFR_RUN_TEST(webfr::test::collection::LinkedListTest);

  WEBFR_RUN_TEST(webfr::test::core::data::share::MemoryLabelTest);
  WEBFR_RUN_TEST(webfr::test::core::data::share::LazyStringMapTest);
  WEBFR_RUN_TEST(webfr::test::core::data::share::StringTemplateTest);

  WEBFR_RUN_TEST(webfr::test::core::data::buffer::ProcessorTest);

  WEBFR_RUN_TEST(webfr::test::core::data::stream::ChunkedBufferTest);
  WEBFR_RUN_TEST(webfr::test::core::data::stream::BufferStreamTest);

  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::ObjectWrapperTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::TypeTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::StringTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::PrimitiveTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::ListTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::VectorTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::UnorderedSetTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::PairListTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::UnorderedMapTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::AnyTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::EnumTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::ObjectTest);

  WEBFR_RUN_TEST(webfr::test::core::data::mapping::type::InterpretationTest);
  WEBFR_RUN_TEST(webfr::test::core::data::mapping::TypeResolverTest);

  WEBFR_RUN_TEST(webfr::test::async::LockTest);
  WEBFR_RUN_TEST(webfr::test::parser::CaretTest);

  WEBFR_RUN_TEST(webfr::test::core::provider::PoolTest);

  WEBFR_RUN_TEST(webfr::test::parser::json::mapping::EnumTest);

  WEBFR_RUN_TEST(webfr::test::parser::json::mapping::UnorderedSetTest);

  WEBFR_RUN_TEST(webfr::test::parser::json::mapping::DeserializerTest);
  WEBFR_RUN_TEST(webfr::test::parser::json::mapping::DTOMapperPerfTest);
  WEBFR_RUN_TEST(webfr::test::parser::json::mapping::DTOMapperTest);

  WEBFR_RUN_TEST(webfr::test::encoding::Base64Test);
  WEBFR_RUN_TEST(webfr::test::encoding::UnicodeTest);

  WEBFR_RUN_TEST(webfr::test::network::UrlTest);

  WEBFR_RUN_TEST(webfr::test::network::ConnectionPoolTest);

  WEBFR_RUN_TEST(webfr::test::network::virtual_::PipeTest);
  WEBFR_RUN_TEST(webfr::test::network::virtual_::InterfaceTest);

  WEBFR_RUN_TEST(webfr::test::web::protocol::http::encoding::ChunkedTest);

  WEBFR_RUN_TEST(webfr::test::web::mime::multipart::StatefulParserTest);

  WEBFR_RUN_TEST(webfr::test::web::server::HttpRouterTest);
  WEBFR_RUN_TEST(webfr::test::web::server::api::ApiControllerTest);
  WEBFR_RUN_TEST(webfr::test::web::server::handler::AuthorizationHandlerTest);

  {

    webfr::test::web::PipelineTest test_virtual(0, 3000);
    test_virtual.run();

    webfr::test::web::PipelineTest test_port(8000, 3000);
    test_port.run();

  }

  {

    webfr::test::web::PipelineAsyncTest test_virtual(0, 3000);
    test_virtual.run();

    webfr::test::web::PipelineAsyncTest test_port(8000, 3000);
    test_port.run();

  }

  {

    webfr::test::web::FullTest test_virtual(0, 1000);
    test_virtual.run();

    webfr::test::web::FullTest test_port(8000, 5);
    test_port.run();

  }

  {

    webfr::test::web::FullAsyncTest test_virtual(0, 1000);
    test_virtual.run();

    webfr::test::web::FullAsyncTest test_port(8000, 5);
    test_port.run();

  }

  {

    webfr::test::web::FullAsyncClientTest test_virtual(0, 1000);
    test_virtual.run(20);

    webfr::test::web::FullAsyncClientTest test_port(8000, 5);
    test_port.run(1);

  }

  {

    webfr::test::web::ClientRetryTest test_virtual(0);
    test_virtual.run();

    webfr::test::web::ClientRetryTest test_port(8000);
    test_port.run();

  }

}

}

int main() {
  
  webfr::base::Environment::init();
  
  runTests();
  
  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D WEBFR_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << webfr::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << webfr::base::Environment::getObjectsCreated() << "\n\n";
  
  WEBFR_ASSERT(webfr::base::Environment::getObjectsCount() == 0);
  
  webfr::base::Environment::destroy();
  
  return 0;
}
