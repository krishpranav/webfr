#include "FullAsyncTest.hpp"
#include "webfr/web/app/Client.hpp"
#include "webfr/web/app/ControllerWithInterceptorsAsync.hpp"
#include "webfr/web/app/ControllerAsync.hpp"
#include "webfr/web/client/HttpRequestExecutor.hpp"
#include "webfr/web/server/AsyncHttpConnectionHandler.hpp"
#include "webfr/web/server/HttpRouter.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/network/tcp/server/ConnectionProvider.hpp"
#include "webfr/network/tcp/client/ConnectionProvider.hpp"
#include "webfr/network/virtual_/client/ConnectionProvider.hpp"
#include "webfr/network/virtual_/server/ConnectionProvider.hpp"
#include "webfr/network/virtual_/Interface.hpp"
#include "webfr/core/macro/component.hpp"
#include "webfr-test/web/ClientServerTestRunner.hpp"

namespace webfr { namespace test { namespace web {

namespace {

typedef webfr::web::mime::multipart::PartList PartList;
typedef webfr::web::protocol::http::outgoing::MultipartBody MultipartBody;

class TestComponent {
private:
  v_uint16 m_port;
public:

  TestComponent(v_uint16 port)
    : m_port(port)
  {}

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::async::Executor>, executor)([] {
    return std::make_shared<webfr::async::Executor>(1, 1, 1);
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::virtual_::Interface>, virtualInterface)([] {
    return webfr::network::virtual_::Interface::obtainShared("virtualhost");
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::ServerConnectionProvider>, serverConnectionProvider)([this] {

    if(m_port == 0) {
      WEBFR_COMPONENT(std::shared_ptr<webfr::network::virtual_::Interface>, interface);
      return std::static_pointer_cast<webfr::network::ServerConnectionProvider>(
        webfr::network::virtual_::server::ConnectionProvider::createShared(interface)
      );
    }

    return std::static_pointer_cast<webfr::network::ServerConnectionProvider>(
      webfr::network::tcp::server::ConnectionProvider::createShared({"localhost", m_port})
    );

  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::web::server::HttpRouter>, httpRouter)([] {
    return webfr::web::server::HttpRouter::createShared();
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::ConnectionHandler>, serverConnectionHandler)([] {
    WEBFR_COMPONENT(std::shared_ptr<webfr::web::server::HttpRouter>, router);
    WEBFR_COMPONENT(std::shared_ptr<webfr::async::Executor>, executor);
    return webfr::web::server::AsyncHttpConnectionHandler::createShared(router, executor);
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::data::mapping::ObjectMapper>, objectMapper)([] {
    return webfr::parser::json::mapping::ObjectMapper::createShared();
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, clientConnectionProvider)([this] {

    if(m_port == 0) {
      WEBFR_COMPONENT(std::shared_ptr<webfr::network::virtual_::Interface>, interface);
      return std::static_pointer_cast<webfr::network::ClientConnectionProvider>(
        webfr::network::virtual_::client::ConnectionProvider::createShared(interface)
      );
    }

    return std::static_pointer_cast<webfr::network::ClientConnectionProvider>(
      webfr::network::tcp::client::ConnectionProvider::createShared({"localhost", m_port})
    );

  }());

};

std::shared_ptr<PartList> createMultipart(const std::unordered_map<webfr::String, webfr::String>& map) {

  auto multipart = webfr::web::mime::multipart::PartList::createSharedWithRandomBoundary();

  for(auto& pair : map) {

    webfr::web::mime::multipart::Headers partHeaders;
    auto part = std::make_shared<webfr::web::mime::multipart::Part>(partHeaders);
    multipart->writeNextPartSimple(part);
    part->putHeader("Content-Disposition", "form-data; name=\"" + pair.first + "\"");
    part->setDataInfo(std::make_shared<webfr::data::stream::BufferInputStream>(pair.second));

  }

  return multipart;

}

}
  
void FullAsyncTest::onRun() {

  TestComponent component(m_port);

  webfr::test::web::ClientServerTestRunner runner;

  runner.addController(app::ControllerAsync::createShared());
  runner.addController(app::ControllerWithInterceptorsAsync::createShared());

  runner.run([this, &runner] {

    WEBFR_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, clientConnectionProvider);
    WEBFR_COMPONENT(std::shared_ptr<webfr::data::mapping::ObjectMapper>, objectMapper);

    auto requestExecutor = webfr::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);
    auto client = app::Client::createShared(requestExecutor, objectMapper);

    auto connection = client->getConnection();
    WEBFR_ASSERT(connection);

    v_int32 iterationsStep = m_iterationsPerStep;

    auto lastTick = webfr::base::Environment::getMicroTickCount();

    for(v_int32 i = 0; i < iterationsStep * 10; i ++) {

      //WEBFR_LOGV("i", "%d", i);

      { // test simple GET
        auto response = client->getRoot(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        WEBFR_ASSERT(value == "Hello World Async!!!");
      }
      
      { // test GET with path parameter
        auto response = client->getWithParams("my_test_param-Async", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testValue == "my_test_param-Async");
      }
      
      { // test GET with header parameter
        auto response = client->getWithHeaders("my_test_header-Async", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testValue == "my_test_header-Async");
      }
      
      { // test POST with body
        auto response = client->postBody("my_test_body-Async", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testValue == "my_test_body-Async");
      }

      { // test Big Echo with body
        webfr::data::stream::ChunkedBuffer stream;
        for(v_int32 i = 0; i < webfr::data::buffer::IOBuffer::BUFFER_SIZE; i++) {
          stream.writeSimple("0123456789", 10);
        }
        auto data = stream.toString();
        auto response = client->echoBody(data, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);

        auto returnedData = response->readBodyToString();

        WEBFR_ASSERT(returnedData);
        WEBFR_ASSERT(returnedData == data);
      }

      { // test Chunked body
        webfr::String sample = "__abcdefghijklmnopqrstuvwxyz-0123456789";
        v_int32 numIterations = 10;
        webfr::data::stream::ChunkedBuffer stream;
        for(v_int32 i = 0; i < numIterations; i++) {
          stream.writeSimple(sample->getData(), sample->getSize());
        }
        auto data = stream.toString();
        auto response = client->getChunked(sample, numIterations, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto returnedData = response->readBodyToString();
        WEBFR_ASSERT(returnedData);
        WEBFR_ASSERT(returnedData == data);
      }

      { // Multipart body

        std::unordered_map<webfr::String, webfr::String> map;
        map["value1"] = "Hello";
        map["value2"] = "World";
        auto multipart = createMultipart(map);

        auto body = std::make_shared<MultipartBody>(multipart);

        auto response = client->multipartTest(i + 1, body);
        WEBFR_ASSERT(response->getStatusCode() == 200);

        multipart = std::make_shared<webfr::web::mime::multipart::PartList>(response->getHeaders());

        webfr::web::mime::multipart::Reader multipartReader(multipart.get());
        multipartReader.setPartReader("value1", std::make_shared<webfr::web::mime::multipart::InMemoryPartReader>(10));
        multipartReader.setPartReader("value2", std::make_shared<webfr::web::mime::multipart::InMemoryPartReader>(10));

        response->transferBody(&multipartReader);

        WEBFR_ASSERT(multipart->getAllParts().size() == 2);
        auto part1 = multipart->getNamedPart("value1");
        auto part2 = multipart->getNamedPart("value2");

        WEBFR_ASSERT(part1);
        WEBFR_ASSERT(part2);

        WEBFR_ASSERT(part1->getInMemoryData() == "Hello");
        WEBFR_ASSERT(part2->getInMemoryData() == "World");

      }

      { // test interceptor GET
        auto response = client->getInterceptors(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        WEBFR_ASSERT(value == "Hello World Async!!!");
      }

      if((i + 1) % iterationsStep == 0) {
        auto ticks = webfr::base::Environment::getMicroTickCount() - lastTick;
        lastTick = webfr::base::Environment::getMicroTickCount();
        WEBFR_LOGV("i", "%d, tick=%d", i + 1, ticks);
      }
      
    }

    connection.reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

  }, std::chrono::minutes(10));


  WEBFR_COMPONENT(std::shared_ptr<webfr::async::Executor>, executor);
  executor->waitTasksFinished();
  executor->stop();
  executor->join();

}
  
}}}