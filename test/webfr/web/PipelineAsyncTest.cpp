#include "PipelineAsyncTest.hpp"
#include "webfr/web/app/ControllerAsync.hpp"
#include "webfr/web/server/AsyncHttpConnectionHandler.hpp"
#include "webfr/web/server/HttpRouter.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/network/tcp/server/ConnectionProvider.hpp"
#include "webfr/network/tcp/client/ConnectionProvider.hpp"
#include "webfr/network/virtual_/client/ConnectionProvider.hpp"
#include "webfr/network/virtual_/server/ConnectionProvider.hpp"
#include "webfr/network/virtual_/Interface.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"
#include "webfr/core/macro/component.hpp"
#include "webfr-test/web/ClientServerTestRunner.hpp"

namespace webfr { namespace test { namespace web {

namespace {

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

const char* const SAMPLE_IN =
  "GET / HTTP/1.1\r\n"
  "Connection: keep-alive\r\n"
  "Content-Length: 0\r\n"
  "\r\n";

const char* const SAMPLE_OUT =
  "HTTP/1.1 200 OK\r\n"
  "Content-Length: 20\r\n"
  "Connection: keep-alive\r\n"
  "Server: webfr/" WEBFR_VERSION "\r\n"
  "\r\n"
  "Hello World Async!!!";

}

void PipelineAsyncTest::onRun() {

  TestComponent component(m_port);

  webfr::test::web::ClientServerTestRunner runner;

  runner.addController(app::ControllerAsync::createShared());

  runner.run([this, &runner] {

    WEBFR_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, clientConnectionProvider);

    auto connection = clientConnectionProvider->get();
    connection->setInputStreamIOMode(webfr::data::stream::IOMode::BLOCKING);

    std::thread pipeInThread([this, connection] {

      webfr::data::stream::ChunkedBuffer pipelineStream;

      for (v_int32 i = 0; i < m_pipelineSize; i++) {
        pipelineStream << SAMPLE_IN;
      }

      webfr::data::stream::BufferInputStream inputStream(pipelineStream.toString());

      webfr::data::buffer::IOBuffer ioBuffer;

      webfr::data::stream::transfer(&inputStream, connection.get(), 0, ioBuffer.getData(), ioBuffer.getSize());

    });

    std::thread pipeOutThread([this, connection] {

      webfr::String sample = SAMPLE_OUT;
      webfr::data::stream::ChunkedBuffer receiveStream;
      webfr::data::buffer::IOBuffer ioBuffer;

      auto res = webfr::data::stream::transfer(connection.get(), &receiveStream, sample->getSize() * m_pipelineSize, ioBuffer.getData(), ioBuffer.getSize());

      auto result = receiveStream.toString();

      WEBFR_ASSERT(result->getSize() == sample->getSize() * m_pipelineSize);

    });

    pipeOutThread.join();
    pipeInThread.join();

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

  }, std::chrono::minutes(10));

  WEBFR_COMPONENT(std::shared_ptr<webfr::async::Executor>, executor);

  executor->waitTasksFinished();
  executor->stop();
  executor->join();

}

}}}