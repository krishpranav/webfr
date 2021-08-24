#include "HttpRouterTest.hpp"
#include "webfr/web/server/HttpRouter.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace test { namespace web { namespace server {

namespace {

typedef webfr::web::server::HttpRouterTemplate<v_int32> NumRouter;

}

void HttpRouterTest::onRun() {

  NumRouter router;
  router.route("GET", "ints/1", 1);
  router.route("GET", "ints/2", 2);
  router.route("GET", "ints/all/{value}", -1);

  router.route("POST", "ints/1", 1);
  router.route("POST", "ints/2", 2);
  router.route("POST", "ints/{value}", 3);
  router.route("POST", "ints/*", 4);
  router.route("POST", "*", -100);

  {
    WEBFR_LOGI(TAG, "Case 1");
    auto r = router.getRoute("GET", "ints/1");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 1);
  }

  {
    WEBFR_LOGI(TAG, "Case 2");
    auto r = router.getRoute("GET", "/ints/1");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 1);
  }

  {
    WEBFR_LOGI(TAG, "Case 3");
    auto r = router.getRoute("GET", "ints/1//");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 1);
  }

  {
    WEBFR_LOGI(TAG, "Case 4");
    auto r = router.getRoute("GET", "//ints///1//");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 1);
  }

  {
    WEBFR_LOGI(TAG, "Case 5");
    auto r = router.getRoute("GET", "ints/1/*");
    WEBFR_ASSERT(r.isValid() == false);
    WEBFR_ASSERT(!r);
  }

  {
    WEBFR_LOGI(TAG, "Case 6");
    auto r = router.getRoute("GET", "ints/2");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 2);
  }

  {
    WEBFR_LOGI(TAG, "Case 7");
    auto r = router.getRoute("GET", "ints/all/10");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == -1);
    WEBFR_ASSERT(r.getMatchMap().getVariables().size() == 1);
    WEBFR_ASSERT(r.getMatchMap().getVariable("value") == "10");
  }

  {
    WEBFR_LOGI(TAG, "Case 8");
    auto r = router.getRoute("GET", "//ints//all//10//");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == -1);
    WEBFR_ASSERT(r.getMatchMap().getVariables().size() == 1);
    WEBFR_ASSERT(r.getMatchMap().getVariable("value") == "10");
  }

  {
    WEBFR_LOGI(TAG, "Case 9");
    auto r = router.getRoute("GET", "//ints//all//10//*");
    WEBFR_ASSERT(r.isValid() == false);
    WEBFR_ASSERT(!r);
  }

  {
    WEBFR_LOGI(TAG, "Case 10");
    auto r = router.getRoute("POST", "ints/1");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 1);
  }

  {
    WEBFR_LOGI(TAG, "Case 11");
    auto r = router.getRoute("POST", "ints/2");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 2);
  }

  {
    WEBFR_LOGI(TAG, "Case 12");
    auto r = router.getRoute("POST", "ints/3");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 3);
    WEBFR_ASSERT(r.getMatchMap().getVariables().size() == 1);
    WEBFR_ASSERT(r.getMatchMap().getVariable("value") == "3");
  }

  {
    WEBFR_LOGI(TAG, "Case 13");
    auto r = router.getRoute("POST", "ints/3/10");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == 4);
    WEBFR_ASSERT(r.getMatchMap().getTail() == "3/10");
  }

  {
    WEBFR_LOGI(TAG, "Case 14");
    auto r = router.getRoute("POST", "abc");
    WEBFR_ASSERT(r.isValid());
    WEBFR_ASSERT(r);
    WEBFR_ASSERT(r.getEndpoint() == -100);
    WEBFR_ASSERT(r.getMatchMap().getTail() == "abc");
  }

}

}}}}