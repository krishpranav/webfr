// includes
#include "LockTest.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"
#include "webfr/core/async/Executor.hpp"
#include "webfr/core/async/Lock.hpp"
#include <thread>
#include <list>

namespace webfr { namespace test { namespace async {

namespace {

static constexpr v_int32 NUM_SYMBOLS = 20;

class Buff {
private:
  webfr::data::stream::ChunkedBuffer *m_buffer;
  std::mutex m_mutex;
public:

  Buff(webfr::data::stream::ChunkedBuffer *buffer)
    : m_buffer(buffer) {}

  void writeChar(char c) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_buffer->writeCharSimple(c);
  }

};

class TestCoroutine : public webfr::async::Coroutine<TestCoroutine> {
private:
  char m_symbol;
  Buff *m_buff;
  webfr::async::LockGuard m_lockGuard;
  v_int32 m_counter;
public:

  TestCoroutine(char symbol, Buff *buff, webfr::async::Lock *lock)
    : m_symbol(symbol), m_buff(buff), m_lockGuard(lock), m_counter(0) {}

  Action act() override {
    return m_lockGuard.lockAsync().next(yieldTo(&TestCoroutine::writeSymbol));
    //return yieldTo(&TestCoroutine::writeSymbol);
  }

  Action writeSymbol() {
    if (m_counter < NUM_SYMBOLS) {
      m_counter++;
      m_buff->writeChar(m_symbol);
      return repeat();
    }
    m_lockGuard.unlock();
    return finish();
  }

};

class NotSynchronizedCoroutine : public webfr::async::Coroutine<NotSynchronizedCoroutine> {
private:
  char m_symbol;
  Buff *m_buff;
  v_int32 m_counter;
public:

  NotSynchronizedCoroutine(char symbol, Buff *buff)
    : m_symbol(symbol), m_buff(buff), m_counter(0)
  {}

  Action act() override {
    if (m_counter < NUM_SYMBOLS) {
      m_counter++;
      m_buff->writeChar(m_symbol);
      return repeat();
    }
    return finish();
  }

};

class TestCoroutine2 : public webfr::async::Coroutine<TestCoroutine2> {
private:
  char m_symbol;
  Buff *m_buff;
  webfr::async::Lock *m_lock;
public:

  TestCoroutine2(char symbol, Buff *buff, webfr::async::Lock *lock)
    : m_symbol(symbol)
    , m_buff(buff)
    , m_lock(lock)
  {}

  Action act() override {
    return webfr::async::synchronize(m_lock, NotSynchronizedCoroutine::start(m_symbol, m_buff)).next(finish());
  }

};

void testMethod(char symbol, Buff *buff, webfr::async::Lock *lock) {

  std::lock_guard<webfr::async::Lock> lockGuard(*lock);
  for (v_int32 i = 0; i < NUM_SYMBOLS; i++) {
    buff->writeChar(symbol);
  }

}

bool checkSymbol(char symbol, const char* data, v_buff_size size) {

  for (v_buff_size i = 0; i < size; i++) {

    if (data[i] == symbol && size - i >= NUM_SYMBOLS) {

      for (v_buff_size j = 0; j < NUM_SYMBOLS; j++) {

        if (data[i + j] != symbol) {
          WEBFR_LOGD("aaa", "j pos=%d", j);
          return false;
        }

      }

      return true;

    }

  }

  WEBFR_LOGD("aaa", "No symbol found");
  return false;

}

bool checkSymbol(char symbol, const webfr::String& str) {
  return checkSymbol(symbol, (const char*)str->getData(), str->getSize());
}

}


void LockTest::onRun() {

  webfr::async::Lock lock;
  webfr::data::stream::ChunkedBuffer buffer;
  Buff buff(&buffer);

  webfr::async::Executor executor(10, 1, 1);

  for (v_int32 c = 0; c <= 127; c++) {
    executor.execute<TestCoroutine>((char)c, &buff, &lock);
  }

  for (v_int32 c = 128; c <= 200; c++) {
    executor.execute<TestCoroutine2>((char)c, &buff, &lock);
  }

  std::list<std::thread> threads;

  for (v_int32 c = 201; c <= 255; c++) {
    threads.push_back(std::thread(testMethod, (char)c, &buff, &lock));
  }

  for (std::thread &thread : threads) {
    thread.join();
  }

  executor.waitTasksFinished();
  executor.stop();
  executor.join();

  auto result = buffer.toString();

  for (v_int32 c = 0; c <= 255; c++) {
    bool check = checkSymbol((char)c, result);
    if(!check) {
      v_int32 code = c;
      auto str = webfr::String((const char*)&c, 1, true);
      WEBFR_LOGE(TAG, "Failed for symbol %d, '%s'", code, str->getData());
    }
    WEBFR_ASSERT(check);
  }

}

}}}