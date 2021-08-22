#ifndef webfr_test_Checker_hpp
#define webfr_test_Checker_hpp

// includes
#include "webfr/core/base/Environment.hpp"

namespace webfr { namespace test {

class PerformanceChecker {
private:
  const char* m_tag;
  v_int64 m_ticks;
public:

  PerformanceChecker(const char* tag);

  ~PerformanceChecker();

  v_int64 getElapsedTicks();
    
};

class ThreadLocalObjectsChecker {
private:
  class MemoryPoolData {
  public:
    const char* name;
    v_int64 size;
    v_int64 objectsCount;
  };
private:
  const char* m_tag;
  v_counter m_objectsCount;
  v_counter m_objectsCreated;
public:

  ThreadLocalObjectsChecker(const char* tag);

  ~ThreadLocalObjectsChecker();
};
  
}}

#endif 