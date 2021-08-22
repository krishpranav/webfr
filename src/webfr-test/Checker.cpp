// includes
#include "Checker.hpp"

namespace webfr { namespace test {

PerformanceChecker::PerformanceChecker(const char* tag)
  : m_tag(tag)
  , m_ticks(webfr::base::Environment::getMicroTickCount())
{}

PerformanceChecker::~PerformanceChecker(){
  v_int64 elapsedTicks = webfr::base::Environment::getMicroTickCount() - m_ticks;
  WEBFR_LOGD(m_tag, "%d(micro)", elapsedTicks);
}

v_int64 PerformanceChecker::getElapsedTicks(){
  return webfr::base::Environment::getMicroTickCount() - m_ticks;
}


ThreadLocalObjectsChecker::ThreadLocalObjectsChecker(const char* tag)
  : m_tag(tag)
  , m_objectsCount(webfr::base::Environment::getThreadLocalObjectsCount())
  , m_objectsCreated(webfr::base::Environment::getThreadLocalObjectsCreated())
{
}

ThreadLocalObjectsChecker::~ThreadLocalObjectsChecker(){
  
  v_counter leakingObjects = base::Environment::getThreadLocalObjectsCount() - m_objectsCount;
  v_counter objectsCreatedPerTest = base::Environment::getThreadLocalObjectsCreated() - m_objectsCreated;
  
  if(leakingObjects == 0){
    OATPP_LOGE(m_tag, "OK:\n created(obj): %d", objectsCreatedPerTest);
  }else{
    OATPP_LOGE(m_tag, "FAILED, leakingObjects = %d", leakingObjects);
    OATPP_ASSERT(false);
  }
  
}
  
}}