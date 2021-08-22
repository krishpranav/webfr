#ifndef webfr_test_web_ClientServerTestRunner_hpp
#define webfr_test_web_ClientServerTestRunner_hpp

#include "webfr/web/server/api/ApiController.hpp"
#include "webfr/web/server/HttpRouter.hpp"
#include "webfr/network/Server.hpp"
#include "webfr/network/ConnectionProvider.hpp"
#include "webfr/core/macro/component.hpp"
#include <list>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>

namespace webfr { namespace test { namespace web {

class ClientServerTestRunner {
public:
  typedef webfr::web::server::HttpRouter HttpRouter;
  typedef webfr::web::server::api::ApiController ApiController;
private:
  std::shared_ptr<webfr::network::Server> m_server;
  std::list<std::shared_ptr<ApiController>> m_controllers;
  WEBFR_COMPONENT(std::shared_ptr<HttpRouter>, m_router);
  WEBFR_COMPONENT(std::shared_ptr<webfr::network::ServerConnectionProvider>, m_connectionProvider);
  WEBFR_COMPONENT(std::shared_ptr<webfr::network::ConnectionHandler>, m_connectionHandler);
public:

  std::shared_ptr<HttpRouter> getRouter() {
    return m_router;
  }

  /**
   * @param controller
   */
  void addController(const std::shared_ptr<ApiController>& controller) {
    controller->addEndpointsToRouter(m_router);
    m_controllers.push_back(controller);
  }

  std::shared_ptr<webfr::network::Server> getServer() {
    return m_server;
  }

  template<typename Lambda>
  void run(
    const Lambda& lambda,
    const std::chrono::duration<v_int64, std::micro>& timeout = std::chrono::hours(12)
  ) {

    auto startTime = std::chrono::system_clock::now();
    std::atomic<bool> running(true);
    std::mutex timeoutMutex;
    std::condition_variable timeoutCondition;
    bool runConditionForLambda = true;

    m_server = std::make_shared<webfr::network::Server>(m_connectionProvider, m_connectionHandler);
    WEBFR_LOGD("\033[1;34mClientServerTestRunner\033[0m", "\033[1;34mRunning server on port %s. Timeout %lld(micro)\033[0m",
               m_connectionProvider->getProperty("port").toString()->c_str(),
               timeout.count());

    std::function<bool()> condition = [&runConditionForLambda](){
        return runConditionForLambda;
    };

    std::thread serverThread([&condition, this]{
      m_server->run(condition);
    });

    std::thread clientThread([&runConditionForLambda, this, &lambda]{

      lambda();
      runConditionForLambda = false;
      m_connectionHandler->stop();
      m_connectionProvider->stop();

    });

    std::thread timerThread([&timeout, &startTime, &running, &timeoutMutex, &timeoutCondition]{

      std::unique_lock<std::mutex> lock(timeoutMutex);
      while(running) {
        timeoutCondition.wait_for(lock, std::chrono::seconds(1));
        auto elapsed = std::chrono::system_clock::now() - startTime;
        WEBFR_ASSERT("ClientServerTestRunner: Error. Timeout." && elapsed < timeout);
      }

    });

    serverThread.join();
    clientThread.join();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - startTime);
    WEBFRR_LOGD("\033[1;34mClientServerTestRunner\033[0m", "\033[1;34mFinished with time %lld(micro). Stopping server...\033[0m", elapsed.count());

    running = false;
    timeoutCondition.notify_one();

    timerThread.join();

  }

};

}}}

#endif 