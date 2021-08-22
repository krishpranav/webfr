// includes
#include "LoggerTest.hpp"

namespace webfr { namespace test { namespace base {

WEBFR_LOG_CATEGORY(LoggerTest::TESTCATEGORY, "LogCategory", true);

void LoggerTest::onRun() {

  auto logger = std::static_pointer_cast<webfr::base::DefaultLogger>(webfr::base::Environment::getLogger());

  WEBFR_LOGV("LoggerTest", "Verbose Log");
  WEBFR_LOGD("LoggerTest", "Debug Log");
  WEBFR_LOGI("LoggerTest", "Info Log");
  WEBFR_LOGW("LoggerTest", "Warning Log");
  WEBFR_LOGE("LoggerTest", "Error Log");

  WEBFR_LOGI("LoggerTest", " --- Disabling Debug Log");
  logger->disablePriority(webfr::base::DefaultLogger::PRIORITY_D);
  WEBFR_ASSERT(!logger->isLogPriorityEnabled(webfr::base::DefaultLogger::PRIORITY_D))

  WEBFR_LOGV("LoggerTest", "Verbose Log");
  WEBFR_LOGD("LoggerTest", "Debug Log");
  WEBFR_LOGI("LoggerTest", "Info Log");
  WEBFR_LOGW("LoggerTest", "Warning Log");
  WEBFR_LOGE("LoggerTest", "Error Log");

  WEBFR_LOGI("LoggerTest", " --- Enabling Debug Log again");
  logger->enablePriority(webfr::base::DefaultLogger::PRIORITY_D);
  WEBFR_ASSERT(logger->isLogPriorityEnabled(webfr::base::DefaultLogger::PRIORITY_D))

  WEBFR_LOGV("LoggerTest", "Verbose Log");
  WEBFR_LOGD("LoggerTest", "Debug Log");
  WEBFR_LOGI("LoggerTest", "Info Log");
  WEBFR_LOGW("LoggerTest", "Warning Log");
  WEBFR_LOGE("LoggerTest", "Error Log");

  WEBFR_LOGI(TESTCATEGORY, " --- Log-Test with category");
  WEBFR_LOGV(TESTCATEGORY, "Verbose Log");
  WEBFR_LOGD(TESTCATEGORY, "Debug Log");
  WEBFR_LOGI(TESTCATEGORY, "Info Log");
  WEBFR_LOGW(TESTCATEGORY, "Warning Log");
  WEBFR_LOGE(TESTCATEGORY, "Error Log");

  WEBFR_LOGI(TESTCATEGORY, " --- Disabling Debug Log for category");
  TESTCATEGORY.disablePriority(webfr::base::DefaultLogger::PRIORITY_D);
  WEBFR_ASSERT(!TESTCATEGORY.isLogPriorityEnabled(webfr::base::DefaultLogger::PRIORITY_D))
  WEBFR_LOGV(TESTCATEGORY, "Verbose Log");
  WEBFR_LOGD(TESTCATEGORY, "Debug Log");
  WEBFR_LOGI(TESTCATEGORY, "Info Log");
  WEBFR_LOGW(TESTCATEGORY, "Warning Log");
  WEBFR_LOGE(TESTCATEGORY, "Error Log");

}

}}}