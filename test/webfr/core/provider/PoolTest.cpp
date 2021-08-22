// icnludes
#include "PoolTest.hpp"
#include "webfr/core/provider/Pool.hpp"
#include "webfr/core/async/Executor.hpp"

#include <thread>

namespace webfr { namespace test { namespace core { namespace provider {

namespace {

class Resource {
public:

  virtual ~Resource() = default;

  virtual v_int64 myId() = 0;

};

class MyResource : public Resource {
private:
  v_int64 m_id;
public:

  MyResource(v_int64 number)
    : m_id(number)
  {}

  v_int64 myId() override {
    return m_id;
  }

};

class Provider : public webfr::provider::Provider<Resource> {
private:
  std::atomic<v_int64> m_id;
public:

  std::shared_ptr<Resource> get() override {
    return std::make_shared<MyResource>(++m_id);
  }

  async::CoroutineStarterForResult<const std::shared_ptr<Resource> &> getAsync() override {

    class GetCoroutine : public webfr::async::CoroutineWithResult<GetCoroutine, const std::shared_ptr<Resource>&> {
    private:
      Provider* m_provider;
    public:

      GetCoroutine(Provider* provider)
        : m_provider(provider)
      {}

      Action act() override {
        return _return(std::make_shared<MyResource>(++ m_provider->m_id));
      }

    };

    return GetCoroutine::startForResult(this);
  }

  void invalidate(const std::shared_ptr<Resource>& resource) override {
    (void) resource;
  }

  void stop() override {
    WEBFR_LOGD("Provider", "stop()");
  }

  v_int64 getIdCounter() {
    return m_id;
  }

};


struct AcquisitionProxy : public webfr::provider::AcquisitionProxy<Resource, AcquisitionProxy> {

  AcquisitionProxy(const std::shared_ptr<Resource>& resource, const std::shared_ptr<PoolInstance>& pool)
    : webfr::provider::AcquisitionProxy<Resource, AcquisitionProxy>(resource, pool)
  {}

  v_int64 myId() override {
    return _obj->myId();
  }

};

typedef webfr::provider::Pool<webfr::provider::Provider<Resource>, Resource, AcquisitionProxy> Pool;


class ClientCoroutine : public webfr::async::Coroutine<ClientCoroutine> {
private:
  std::shared_ptr<Pool> m_pool;
  std::shared_ptr<Resource> m_resource;
  bool m_invalidate;
public:

  ClientCoroutine(const std::shared_ptr<Pool>& pool, bool invalidate)
    : m_pool(pool)
    , m_invalidate(invalidate)
  {}

  Action act() override {
    return m_pool->getAsync().callbackTo(&ClientCoroutine::onGet);
  }

  Action onGet(const std::shared_ptr<Resource>& resource) {
    m_resource = resource;
    return waitFor(std::chrono::milliseconds(100)).next(yieldTo(&ClientCoroutine::onUse));
  }

  Action onUse() {
    if(m_invalidate) {
      m_pool->invalidate(m_resource);
    }
    return finish();
  }

};

void clientMethod(std::shared_ptr<Pool> pool, bool invalidate) {
  auto resource = pool->get();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  if(invalidate) {
    pool->invalidate(resource);
  }
}

}

void PoolTest::onRun() {

  webfr::async::Executor executor(10, 1, 1);

  auto provider = std::make_shared<Provider>();
  auto pool = Pool::createShared(provider, 10, std::chrono::seconds(1));


  std::list<std::thread> threads;

  WEBFR_LOGD(TAG, "Run 1");
  for(v_int32 i = 0; i < 100; i ++ ) {
    threads.push_back(std::thread(clientMethod, pool, false));
    executor.execute<ClientCoroutine>(pool, false);
  }

  WEBFR_ASSERT(pool->getCounter() == 10);
  WEBFR_LOGD(TAG, "Waiting...");
  std::this_thread::sleep_for(std::chrono::seconds(6));
  WEBFR_LOGD(TAG, "Pool counter=%d", pool->getCounter());
  WEBFR_ASSERT(pool->getCounter() == 0);

  WEBFR_LOGD(TAG, "Run 2");
  for(v_int32 i = 0; i < 100; i ++ ) {
    threads.push_back(std::thread(clientMethod, pool, false));
    executor.execute<ClientCoroutine>(pool, false);
  }

  WEBFR_ASSERT(pool->getCounter() == 10);
  WEBFR_LOGD(TAG, "Waiting...");
  std::this_thread::sleep_for(std::chrono::seconds(6));
  WEBFR_LOGD(TAG, "Pool counter=%d", pool->getCounter());
  WEBFR_ASSERT(pool->getCounter() == 0);

  for(std::thread& thread : threads) {
    thread.join();
  }

  executor.waitTasksFinished();

  WEBFR_LOGD(TAG, "counter=%d", provider->getIdCounter());
  WEBFR_ASSERT(provider->getIdCounter() == 20);

  pool->stop();

  executor.stop();
  executor.join();

  std::this_thread::sleep_for(std::chrono::milliseconds(200));

}

}}}}