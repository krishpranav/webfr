#include "PipelineTest.hpp"
#include "webfr/web/app/Controller.hpp"
#include "webfr/web/server/HttpConnectionHandler.hpp"
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
    return webfr::web::server::HttpConnectionHandler::createShared(router);
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
  "Content-Length: 14\r\n"
  "Connection: keep-alive\r\n"
  "Server: webfr/" WEBFR_VERSION "\r\n"
  "\r\n"
  "Hello World!!!";

}

void PipelineTest::onRun() {

  TestComponent component(m_port);

  webfr::test::web::ClientServerTestRunner runner;

  runner.addController(app::Controller::createShared());

  runner.run([this, &runner] {

    WEBFR_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, clientConnectionProvider);

    auto connection = clientConnectionProvider->get();
    connection->setInputStreamIOMode(webfr::data::stream::IOMode::BLOCKING);

    std::thread pipeInThread([this, connection] {

      webfr::data::stream::ChunkedBuffer pipelineStream;

      for (v_int32 i = 0; i < m_pipelineSize; i++) {
        pipelineStream << SAMPLE_IN;
      }

      auto dataToSend = pipelineStream.toString();
      WEBFR_LOGD(TAG, "Sending %d bytes", dataToSend->getSize());

      webfr::data::stream::BufferInputStream inputStream(dataToSend);

      webfr::data::buffer::IOBuffer ioBuffer;

      auto res = webfr::data::stream::transfer(&inputStream, connection.get(), 0, ioBuffer.getData(), ioBuffer.getSize());

    });

    std::thread pipeOutThread([this, connection] {

      webfr::String sample = SAMPLE_OUT;
      webfr::data::stream::BufferOutputStream receiveStream;
      webfr::data::buffer::IOBuffer ioBuffer;

      v_io_size transferSize = sample->getSize() * m_pipelineSize;

      WEBFR_LOGD(TAG, "want to Receive %d bytes", transferSize);
      auto res = webfr::data::stream::transfer(connection.get(), &receiveStream, transferSize, ioBuffer.getData(), ioBuffer.getSize());

      auto result = receiveStream.toString();

      WEBFR_ASSERT(result->getSize() == sample->getSize() * m_pipelineSize);
      //WEBFR_ASSERT(result == wantedResult); // headers may come in different order on different OSs

    });

    pipeOutThread.join();
    pipeInThread.join();

  }, std::chrono::minutes(10));

  std::this_thread::sleep_for(std::chrono::seconds(1));

}

}}}