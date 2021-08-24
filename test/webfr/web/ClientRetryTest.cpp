#include "ClientRetryTest.hpp"
#include "webfr/web/app/Client.hpp"
#include "webfr/web/app/ControllerWithInterceptors.hpp"
#include "webfr/web/app/Controller.hpp"
#include "webfr/web/app/BasicAuthorizationController.hpp"
#include "webfr/web/app/BearerAuthorizationController.hpp"
#include "webfr/web/client/HttpRequestExecutor.hpp"
#include "webfr/web/server/HttpConnectionHandler.hpp"
#include "webfr/web/server/HttpRouter.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/network/tcp/server/ConnectionProvider.hpp"
#include "webfr/network/tcp/client/ConnectionProvider.hpp"
#include "webfr/network/virtual_/client/ConnectionProvider.hpp"
#include "webfr/network/virtual_/server/ConnectionProvider.hpp"
#include "webfr/network/virtual_/Interface.hpp"
#include "webfr/network/ConnectionPool.hpp"
#include "webfr/core/macro/component.hpp"
#include "webfr-test/web/ClientServerTestRunner.hpp"
#include "webfr-test/Checker.hpp"

namespace webfr { namespace test { namespace web {

namespace {

typedef webfr::web::server::api::ApiController ApiController;

class TestServerComponent {
private:
  v_uint16 m_port;
public:

  TestServerComponent(v_uint16 port)
    : m_port(port)
  {}

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::ServerConnectionProvider>, serverConnectionProvider)([this] {

    if(m_port == 0) { 
      auto interface = webfr::network::virtual_::Interface::obtainShared("virtualhost");
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

};

class TestClientComponent {
private:
  v_uint16 m_port;
public:

  TestClientComponent(v_uint16 port)
    : m_port(port)
  {}

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, clientConnectionProvider)([this] {

    if(m_port == 0) {
      auto interface = webfr::network::virtual_::Interface::obtainShared("virtualhost");
      return std::static_pointer_cast<webfr::network::ClientConnectionProvider>(
        webfr::network::virtual_::client::ConnectionProvider::createShared(interface)
      );
    }

    return std::static_pointer_cast<webfr::network::ClientConnectionProvider>(
      webfr::network::tcp::client::ConnectionProvider::createShared({"localhost", m_port})
    );

  }());

};

void runServer(v_uint16 port, v_int32 delaySeconds, v_int32 iterations, bool stable, const std::shared_ptr<app::Controller>& controller) {

  TestServerComponent component(port);

  webfr::test::web::ClientServerTestRunner runner;

  runner.addController(controller);

  runner.run([&runner, delaySeconds, iterations, stable, controller] {

    for(v_int32 i = 0; i < iterations; i ++) {
      std::this_thread::sleep_for(std::chrono::seconds(delaySeconds));
      if(!stable) {
        controller->available = !controller->available;
        WEBFR_LOGI("Server", "Available=%d", (v_int32)controller->available.load());
      }
    }

  }, std::chrono::minutes(10));

  std::this_thread::sleep_for(std::chrono::seconds(1));

}

}

void ClientRetryTest::onRun() {

  TestClientComponent component(m_port);

  auto objectMapper = webfr::parser::json::mapping::ObjectMapper::createShared();
  auto controller = app::Controller::createShared(objectMapper);

  WEBFR_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, connectionProvider);

  {

    WEBFR_LOGI(TAG, "Test: no server available");
    webfr::test::PerformanceChecker checker("test: no server available");

    auto retryPolicy = std::make_shared<webfr::web::client::SimpleRetryPolicy>(2, std::chrono::seconds(1));
    auto requestExecutor = webfr::web::client::HttpRequestExecutor::createShared(connectionProvider, retryPolicy);
    auto client = app::Client::createShared(requestExecutor, objectMapper);

    auto response = client->getRoot();
    auto ticks = checker.getElapsedTicks();

    WEBFR_LOGD(TAG, "ticks=%d", ticks);

    if(m_port == 0) {

      WEBFR_ASSERT(response.get() == nullptr);
      WEBFR_ASSERT(ticks >= 2 * 1000 * 1000 /* 2s */);
      WEBFR_ASSERT(ticks < 3 * 1000 * 1000 /* 3s */);

    } else {

// TODO - investigate why it takes more than 2 seconds on windows to try to connect to unavailable host
#if !defined(WIN32) && !defined(_WIN32)

      WEBFR_ASSERT(response.get() == nullptr);
      WEBFR_ASSERT(ticks >= 2 * 1000 * 1000 /* 2s */);
      WEBFR_ASSERT(ticks < 3 * 1000 * 1000 /* 3s */);

#endif

    }

  }

  {

    WEBFR_LOGI(TAG, "Test: server pops up");
    webfr::test::PerformanceChecker checker("test: server pops up");

    auto retryPolicy = std::make_shared<webfr::web::client::SimpleRetryPolicy>(10 * 10, std::chrono::milliseconds(100));
    auto requestExecutor = webfr::web::client::HttpRequestExecutor::createShared(connectionProvider, retryPolicy);
    auto client = app::Client::createShared(requestExecutor, objectMapper);

    std::list<std::thread> threads;

    for(v_int32 i = 0; i < 100; i ++) {
      threads.push_back(std::thread([client]{
        auto response = client->getRoot();
        WEBFR_ASSERT(response && "Test: server pops up");
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto data = response->readBodyToString();
        WEBFR_ASSERT(data == "Hello World!!!");
      }));
    }

    WEBFR_LOGD(TAG, "Waiting for server to start...");
    std::this_thread::sleep_for(std::chrono::seconds(3));

    runServer(m_port, 2, 2, true, controller);

    for(std::thread& thread : threads) {
      thread.join();
    }

    auto ticks = checker.getElapsedTicks();
    WEBFR_ASSERT(ticks < 10 * 1000 * 1000 /* 10s */);

  }

  {

    WEBFR_LOGI(TAG, "Test: unstable server!");

    auto retryPolicy = std::make_shared<webfr::web::client::SimpleRetryPolicy>(-1, std::chrono::seconds(1));
    auto connectionPool = webfr::network::ClientConnectionPool::createShared(connectionProvider, 10, std::chrono::seconds(1));
    auto requestExecutor = webfr::web::client::HttpRequestExecutor::createShared(connectionPool, retryPolicy);
    auto client = app::Client::createShared(requestExecutor, objectMapper);

    std::list<std::thread> threads;

    std::thread clientThread([client]{

      v_int64 counter = 0;

      v_int64 tick0 = webfr::base::Environment::getMicroTickCount();

      while(webfr::base::Environment::getMicroTickCount() - tick0 < 10 * 1000 * 1000) {

        auto response = client->getAvailability();
        WEBFR_ASSERT(response && "Test: unstable server!");
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto data = response->readBodyToString();
        WEBFR_ASSERT(data == "Hello World!!!");
        counter ++;

        if(counter % 1000 == 0) {
          WEBFR_LOGD("client", "requests=%d", counter);
        }

      }

    });

    runServer(m_port, 2, 6, false, controller);

    clientThread.join();
    connectionPool->stop();

  }

  std::this_thread::sleep_for(std::chrono::seconds(2)); // wait connection pool.

}

}}}