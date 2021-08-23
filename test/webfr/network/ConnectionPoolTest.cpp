#include "ConnectionPoolTest.hpp"
#include "webfr/network/ConnectionPool.hpp"
#include "webfr/core/async/Executor.hpp"

namespace webfr { namespace test { namespace network {

namespace {

typedef webfr::provider::Pool<
  webfr::network::ConnectionProvider,
  webfr::data::stream::IOStream,
  webfr::network::ConnectionAcquisitionProxy
> ConnectionPool;

class StubStream : public webfr::data::stream::IOStream, public webfr::base::Countable {
public:

  v_io_size write(const void *buff, v_buff_size count, async::Action& actions) override {
    throw std::runtime_error("It's a stub!");
  }

  v_io_size read(void *buff, v_buff_size count, async::Action& action) override {
    throw std::runtime_error("It's a stub!");
  }

  void setOutputStreamIOMode(webfr::data::stream::IOMode ioMode) override {
    throw std::runtime_error("It's a stub!");
  }

  webfr::data::stream::IOMode getOutputStreamIOMode() override {
    throw std::runtime_error("It's a stub!");
  }

  webfr::data::stream::Context& getOutputStreamContext() override {
    throw std::runtime_error("It's a stub!");
  }

  void setInputStreamIOMode(webfr::data::stream::IOMode ioMode) override {
    throw std::runtime_error("It's a stub!");
  }

  webfr::data::stream::IOMode getInputStreamIOMode() override {
    throw std::runtime_error("It's a stub!");
  }

  webfr::data::stream::Context& getInputStreamContext() override {
    throw std::runtime_error("It's a stub!");
  }

};

class StubStreamProvider : public webfr::network::ConnectionProvider {
public:

  StubStreamProvider()
    : counter(0)
  {}

  std::atomic<v_int64> counter;

  std::shared_ptr<webfr::data::stream::IOStream> get() override {
    ++ counter;
    return std::make_shared<StubStream>();
  }

  webfr::async::CoroutineStarterForResult<const std::shared_ptr<webfr::data::stream::IOStream>&> getAsync() override {

    class ConnectionCoroutine : public webfr::async::CoroutineWithResult<ConnectionCoroutine, const std::shared_ptr<webfr::data::stream::IOStream>&> {
    public:

      Action act() override {
        return _return(std::make_shared<StubStream>());
      }

    };

    ++ counter;
    return ConnectionCoroutine::startForResult();

  }

  void stop() override {
    // DO NOTHING
  }

  void invalidate(const std::shared_ptr<webfr::data::stream::IOStream>& connection) override {
    (void)connection;
    // DO Nothing.
  }

};

class ClientCoroutine : public webfr::async::Coroutine<ClientCoroutine> {
private:
  std::shared_ptr<ConnectionPool> m_pool;
  std::shared_ptr<webfr::data::stream::IOStream> m_connection;
  v_int32 m_repeats;
  bool m_invalidate;
public:

  ClientCoroutine(const std::shared_ptr<ConnectionPool>& pool, bool invalidate)
    : m_pool(pool)
    , m_repeats(0)
    , m_invalidate(invalidate)
  {}

  Action act() override {
    return m_pool->getAsync().callbackTo(&ClientCoroutine::onConnection);
  }

  Action onConnection(const std::shared_ptr<webfr::data::stream::IOStream>& connection) {
    m_connection = connection;
    return yieldTo(&ClientCoroutine::useConnection);
  }

  Action useConnection() {
    if(m_repeats < 1) {
      m_repeats ++;
      return waitFor(std::chrono::milliseconds(100)).next(yieldTo(&ClientCoroutine::useConnection));
    }
    if(m_invalidate) {
      m_pool->invalidate(m_connection);
    }
    return finish();
  }

};

void clientMethod(std::shared_ptr<ConnectionPool> pool, bool invalidate) {
  auto connection = pool->get();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  if(invalidate) {
    pool->invalidate(connection);
  }
}

}

void ConnectionPoolTest::onRun() {

  webfr::async::Executor executor(1, 1, 1);

  auto connectionProvider = std::make_shared<StubStreamProvider>();
  auto pool = ConnectionPool::createShared(connectionProvider, 10 /* maxConnections */, std::chrono::seconds(10) /* maxConnectionTTL */);

  std::list<std::thread> threads;

  for(v_int32 i = 0; i < 100; i ++ ) {

    threads.push_back(std::thread(clientMethod, pool, false));
    executor.execute<ClientCoroutine>(pool, false);

  }

  for(std::thread& thread : threads) {
    thread.join();
  }

  executor.waitTasksFinished();

  WEBFR_LOGD(TAG, "connections_counter=%d", connectionProvider->counter.load());
  WEBFR_ASSERT(connectionProvider->counter <= 10);

  pool->stop();

  executor.stop();
  executor.join();

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

}

}}}