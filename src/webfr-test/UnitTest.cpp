// includes
#include "UnitTest.hpp"
#include "webfr/core/base/memory/MemoryPool.hpp"
#include <chrono>

namespace webfr { namespace test {

void UnitTest::run(v_int32 times) {
  
  WEBFR_LOGI(TAG, "\033[1mSTART\033[0m...");
  
  v_counter objectsCount = base::Environment::getObjectsCount();
  v_counter objectsCreated = base::Environment::getObjectsCreated();
  
  v_int64 ticks = base::Environment::getMicroTickCount();
  
  for(v_int32 i = 0; i < times; i++){
    onRun();
  }
  
  v_int64 millis = base::Environment::getMicroTickCount() - ticks;
  
  v_counter leakingObjects = base::Environment::getObjectsCount() - objectsCount;
  v_counter objectsCreatedPerTest = base::Environment::getObjectsCreated() - objectsCreated;
  
  if(leakingObjects == 0){
    WEBFR_LOGI(TAG, "\033[1mFINISHED\033[0m - \033[1;32msuccess!\033[0m");
    WEBFR_LOGI(TAG, "\033[33m%d(micro), %d(objs)\033[0m\n", millis, objectsCreatedPerTest);
  }else{

    WEBFR_LOGE(TAG, "\033[1mFINISHED\033[0m - \033[1;31mfailed\033[0m, leakingObjects = %d", leakingObjects);
    
    auto POOLS = webfr::base::memory::MemoryPool::POOLS;
    auto it = POOLS.begin();
    while (it != POOLS.end()) {
      auto pool = it->second;
      if(pool->getObjectsCount() != 0) {
        WEBFR_LOGV("Pool", "name: '%s' [%d(objs)]", pool->getName().c_str(), pool->getObjectsCount());
      }
      it ++;
    }

    exit(EXIT_FAILURE);

  }
  
}

}}