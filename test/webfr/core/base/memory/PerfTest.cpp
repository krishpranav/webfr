// includes
#include "PerfTest.hpp"
#include "webfr/core/collection/LinkedList.hpp"
#include "webfr/core/Types.hpp"
#include "webfr/core/concurrency/Thread.hpp"
#include <list>

namespace webfr { namespace test { namespace memory {
  
namespace {
  
  class TestBase {
  public:
    
    static void* operator new(std::size_t sz) {
      if(sz != sizeof(TestBase)){
        throw std::runtime_error("wrong object size");
      }
      return ::operator new(sz);
    }
    
    static void operator delete(void* ptr, std::size_t sz) {
      if(sz != sizeof(TestBase)){
        webfr::base::Environment::log(2, "PerfTest::TestBase", "[ERROR|CRITICAL]: MemoryPool malfunction. Deleting object of wrong size"); \
      }
      ::operator delete(ptr);
    }
    
  };
  
  class TestChild : public webfr::base::Countable, public TestBase {
  public:

    static void* operator new(std::size_t sz) {
      return ::operator new(sz);
    }

    static void operator delete(void* ptr, std::size_t sz) {
      (void)sz;
      ::operator delete(ptr);
    }

  };
  
  class Task : public webfr::base::Countable {
  private:
    std::shared_ptr<TestBase> m_shared;
  public:
    
    Task(const std::shared_ptr<TestBase>& shared)
      : m_shared(shared)
    {}
    
    void run() {
      for(v_int32 i = 0; i < 10; i ++) {
        std::shared_ptr<TestBase> shared(new TestChild());
      }
    }
  };
  
}
  
void PerfTest::onRun() {
  
  v_int32 iterations = 1;
  v_int32 threadCount = 100;
  
  for(int i = 0; i < iterations; i++) {

    std::list<std::thread> threads;
    
    for(v_int32 n = 0; n < threadCount; n++) {
      std::shared_ptr<TestBase> shared;
      threads.push_back(std::thread(&Task::run, Task(shared)));
    }

    for(auto& thread : threads) {
      thread.join();
    }
    
  }

}
  
}}}