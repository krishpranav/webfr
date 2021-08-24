#include "FullAsyncClientTest.hpp"
#include "webfr/web/app/Client.hpp"
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

typedef webfr::web::protocol::http::incoming::Response IncomingResponse;

class TestComponent {
private:
  v_uint16 m_port;
public:

  TestComponent(v_uint16 port)
    : m_port(port)
  {}

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::async::Executor>, executor)([] {
    return std::make_shared<webfr::async::Executor>(4, 1, 1);
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

  WEBFR_CREATE_COMPONENT(std::shared_ptr<app::Client>, appClient)([] {

    WEBFR_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, clientConnectionProvider);
    WEBFR_COMPONENT(std::shared_ptr<webfr::data::mapping::ObjectMapper>, objectMapper);

    auto retryPolicy = std::make_shared<webfr::web::client::SimpleRetryPolicy>(5, std::chrono::seconds(1));

    auto requestExecutor = webfr::web::client::HttpRequestExecutor::createShared(clientConnectionProvider, retryPolicy);
    return app::Client::createShared(requestExecutor, objectMapper);

  }());

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClientCoroutine_getRootAsync

class ClientCoroutine_getRootAsync : public webfr::async::Coroutine<ClientCoroutine_getRootAsync> {
public:
  static std::atomic<v_int32> SUCCESS_COUNTER;
private:
  WEBFR_COMPONENT(std::shared_ptr<app::Client>, appClient);
public:

  Action act() override {
    return appClient->getRootAsync().callbackTo(&ClientCoroutine_getRootAsync::onResponse);
  }

  Action onResponse(const std::shared_ptr<IncomingResponse>& response) {
    WEBFR_ASSERT(response->getStatusCode() == 200 && "ClientCoroutine_getRootAsync");
    return response->readBodyToStringAsync().callbackTo(&ClientCoroutine_getRootAsync::onBodyRead);
  }

  Action onBodyRead(const webfr::String& body) {
    WEBFR_ASSERT(body == "Hello World Async!!!");
    ++ SUCCESS_COUNTER;
    return finish();
  }

  Action handleError(Error* error) override {
    if(error->is<webfr::AsyncIOError>()) {
      auto e = static_cast<webfr::AsyncIOError*>(error);
      WEBFR_LOGE("[FullAsyncClientTest::ClientCoroutine_getRootAsync::handleError()]", "AsyncIOError. %s, %d", e->what(), e->getCode());
    } else {
      WEBFR_LOGE("[FullAsyncClientTest::ClientCoroutine_getRootAsync::handleError()]", "Error. %s", error->what());
    }
    WEBFR_ASSERT(!"Error");
    return error;
  }

};

std::atomic<v_int32> ClientCoroutine_getRootAsync::SUCCESS_COUNTER(0);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClientCoroutine_postBodyAsync

class ClientCoroutine_postBodyAsync : public webfr::async::Coroutine<ClientCoroutine_postBodyAsync> {
public:
  static std::atomic<v_int32> SUCCESS_COUNTER;
private:
  WEBFR_COMPONENT(std::shared_ptr<app::Client>, appClient);
  WEBFR_COMPONENT(std::shared_ptr<webfr::data::mapping::ObjectMapper>, objectMapper);
public:

  Action act() override {
    return appClient->postBodyAsync("my_test_body").callbackTo(&ClientCoroutine_postBodyAsync::onResponse);
  }

  Action onResponse(const std::shared_ptr<IncomingResponse>& response) {
    WEBFR_ASSERT(response->getStatusCode() == 200 && "ClientCoroutine_postBodyAsync");
    return response->readBodyToDtoAsync<webfr::Object<app::TestDto>>(objectMapper).callbackTo(&ClientCoroutine_postBodyAsync::onBodyRead);
  }

  Action onBodyRead(const webfr::Object<app::TestDto>& body) {
    WEBFR_ASSERT(body);
    WEBFR_ASSERT(body->testValue == "my_test_body");
    ++ SUCCESS_COUNTER;
    return finish();
  }

  Action handleError(Error* error) override {
    if(error->is<webfr::AsyncIOError>()) {
      auto e = static_cast<webfr::AsyncIOError*>(error);
      WEBFR_LOGE("[FullAsyncClientTest::ClientCoroutine_postBodyAsync::handleError()]", "AsyncIOError. %s, %d", e->what(), e->getCode());
    } else {
      WEBFR_LOGE("[FullAsyncClientTest::ClientCoroutine_postBodyAsync::handleError()]", "Error. %s", error->what());
    }
    WEBFR_ASSERT(!"Error");
    return error;
  }

};

std::atomic<v_int32> ClientCoroutine_postBodyAsync::SUCCESS_COUNTER(0);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ClientCoroutine_echoBodyAsync

class ClientCoroutine_echoBodyAsync : public webfr::async::Coroutine<ClientCoroutine_echoBodyAsync> {
public:
  static std::atomic<v_int32> SUCCESS_COUNTER;
private:
  WEBFR_COMPONENT(std::shared_ptr<app::Client>, appClient);
  webfr::String m_data;
public:

  Action act() override {
    webfr::data::stream::ChunkedBuffer stream;
    for(v_int32 i = 0; i < webfr::data::buffer::IOBuffer::BUFFER_SIZE; i++) {
      stream.writeSimple("0123456789", 10);
    }
    m_data = stream.toString();
    return appClient->echoBodyAsync(m_data).callbackTo(&ClientCoroutine_echoBodyAsync::onResponse);
  }

  Action onResponse(const std::shared_ptr<IncomingResponse>& response) {
    WEBFR_ASSERT(response->getStatusCode() == 200 && "ClientCoroutine_echoBodyAsync");
    return response->readBodyToStringAsync().callbackTo(&ClientCoroutine_echoBodyAsync::onBodyRead);
  }

  Action onBodyRead(const webfr::String& body) {
    WEBFR_ASSERT(body == m_data);
    ++ SUCCESS_COUNTER;
    return finish();
  }

  Action handleError(Error* error) override {
    if(error) {
      if(error->is<webfr::AsyncIOError>()) {
        auto e = static_cast<webfr::AsyncIOError*>(error);
        WEBFR_LOGE("[FullAsyncClientTest::ClientCoroutine_echoBodyAsync::handleError()]", "AsyncIOError. %s, %d", e->what(), e->getCode());
      } else {
        WEBFR_LOGE("[FullAsyncClientTest::ClientCoroutine_echoBodyAsync::handleError()]", "Error. %s", error->what());
      }
    }
    WEBFR_ASSERT(!"Error");
    return error;
  }

};

std::atomic<v_int32> ClientCoroutine_echoBodyAsync::SUCCESS_COUNTER(0);

}

void FullAsyncClientTest::onRun() {

  TestComponent component(m_port);

  webfr::test::web::ClientServerTestRunner runner;

  runner.addController(app::ControllerAsync::createShared());

  runner.run([this, &runner] {

    WEBFR_COMPONENT(std::shared_ptr<webfr::async::Executor>, executor);

    ClientCoroutine_getRootAsync::SUCCESS_COUNTER = 0;
    ClientCoroutine_postBodyAsync::SUCCESS_COUNTER = 0;
    ClientCoroutine_echoBodyAsync::SUCCESS_COUNTER = 0;

    v_int32 iterations = m_connectionsPerEndpoint;

    for(v_int32 i = 0; i < iterations; i++) {
      executor->execute<ClientCoroutine_getRootAsync>();
      executor->execute<ClientCoroutine_postBodyAsync>();
      executor->execute<ClientCoroutine_echoBodyAsync>();
    }

    while(
      ClientCoroutine_getRootAsync::SUCCESS_COUNTER != -1 ||
      ClientCoroutine_postBodyAsync::SUCCESS_COUNTER != -1 ||
      ClientCoroutine_echoBodyAsync::SUCCESS_COUNTER != -1
    ) {

      WEBFR_LOGV("Client", "Root=%d, postBody=%d, echoBody=%d",
        ClientCoroutine_getRootAsync::SUCCESS_COUNTER.load(),
        ClientCoroutine_postBodyAsync::SUCCESS_COUNTER.load(),
        ClientCoroutine_echoBodyAsync::SUCCESS_COUNTER.load()
      );

      std::this_thread::sleep_for(std::chrono::milliseconds(100));

      if(ClientCoroutine_getRootAsync::SUCCESS_COUNTER == iterations){
        ClientCoroutine_getRootAsync::SUCCESS_COUNTER = -1;
        WEBFR_LOGV("Client", "getRootAsync - DONE!");
      }
      if(ClientCoroutine_postBodyAsync::SUCCESS_COUNTER == iterations){
        ClientCoroutine_postBodyAsync::SUCCESS_COUNTER = -1;
        WEBFR_LOGV("Client", "postBodyAsync - DONE!");
      }
      if(ClientCoroutine_echoBodyAsync::SUCCESS_COUNTER == iterations){
        ClientCoroutine_echoBodyAsync::SUCCESS_COUNTER = -1;
        WEBFR_LOGV("Client", "echoBodyAsync - DONE!");
      }
    }

    WEBFR_ASSERT(ClientCoroutine_getRootAsync::SUCCESS_COUNTER == -1); // -1 is success
    WEBFR_ASSERT(ClientCoroutine_postBodyAsync::SUCCESS_COUNTER == -1); // -1 is success
    WEBFR_ASSERT(ClientCoroutine_echoBodyAsync::SUCCESS_COUNTER == -1); // -1 is success

    executor->waitTasksFinished(); // Wait executor tasks before quit.
    executor->stop();

  }, std::chrono::minutes(10));

  WEBFR_COMPONENT(std::shared_ptr<webfr::async::Executor>, executor);
  executor->join();

}

}}}