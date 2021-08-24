#include "ApiControllerTest.hpp"
#include "webfr/web/server/api/ApiController.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"
#include "webfr/core/macro/codegen.hpp"

namespace webfr { namespace test { namespace web { namespace server { namespace api {

namespace {

class Controller : public webfr::web::server::api::ApiController {
public:
  Controller(const std::shared_ptr<ObjectMapper>& objectMapper)
    : webfr::web::server::api::ApiController(objectMapper)
  {}
public:

  static std::shared_ptr<Controller> createShared(const std::shared_ptr<ObjectMapper>& objectMapper){
    return std::make_shared<Controller>(objectMapper);
  }

#include WEBFR_CODEGEN_BEGIN(ApiController)

  ENDPOINT_INFO(root) {
    info->summary = "root_summary";
    info->addResponse<String>(Status::CODE_200, "text/plain", "test1-success");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("GET", "/", root) {
    return createResponse(Status::CODE_200, "test1");
  }

  ENDPOINT_INFO(pathParams) {
    info->pathParams["param1"].description = "this is param1";
    info->queryParams.add<String>("q1").description = "query param";
    info->headers.add<String>("X-TEST-HEADER").description = "TEST-HEADER-PARAM";
  }
  ENDPOINT("GET", "path/{param1}/{param2}", pathParams,
           PATH(String, param1),
           PATH(String, param2)) {
    return createResponse(Status::CODE_200, "test2");
  }

  ENDPOINT_INFO(queryParams) {
    info->queryParams["param1"].description = "this is param1";
  }
  ENDPOINT("GET", "query", queryParams,
           QUERY(String, param1),
           QUERY(String, param2)) {
    return createResponse(Status::CODE_200, "test3");
  }

#include WEBFR_CODEGEN_END(ApiController)

};

}

// methods/fields with "Z__" prefix are for internal use only.
// Do not use these methods/fields for user-tests as naming can be changed

void ApiControllerTest::onRun() {

  typedef webfr::web::protocol::http::Status Status;

  Controller controller(nullptr);
  webfr::data::stream::BufferOutputStream headersOutBuffer;

  {
    auto endpoint = controller.Z__ENDPOINT_root;
    WEBFR_ASSERT(endpoint);
    WEBFR_ASSERT(endpoint->info()->summary == "root_summary");

    auto r200 = endpoint->info()->responses[Status::CODE_200];
    WEBFR_ASSERT(r200.contentType == "text/plain");
    WEBFR_ASSERT(r200.schema == webfr::String::Class::getType());
    WEBFR_ASSERT(r200.description == "test1-success");

    auto r404 = endpoint->info()->responses[Status::CODE_404];
    WEBFR_ASSERT(r404.contentType == "text/plain");
    WEBFR_ASSERT(r404.description == "Not Found");
    WEBFR_ASSERT(r404.schema == webfr::String::Class::getType());

    auto response = controller.root();
    WEBFR_ASSERT(response->getStatus().code == 200);

    webfr::data::stream::ChunkedBuffer stream;
    response->send(&stream, &headersOutBuffer, nullptr);

    WEBFR_LOGD(TAG, "response:\n---\n%s\n---\n", stream.toString()->c_str());

  }

  {
    auto endpoint = controller.Z__ENDPOINT_pathParams;
    WEBFR_ASSERT(endpoint);
    WEBFR_ASSERT(!endpoint->info()->summary);

    WEBFR_ASSERT(endpoint->info()->pathParams["param1"].name == "param1");
    WEBFR_ASSERT(endpoint->info()->pathParams["param1"].description == "this is param1");

    WEBFR_ASSERT(endpoint->info()->pathParams["param2"].name == "param2");
    WEBFR_ASSERT(!endpoint->info()->pathParams["param2"].description);

    WEBFR_ASSERT(endpoint->info()->queryParams["q1"].name == "q1");
    WEBFR_ASSERT(endpoint->info()->queryParams["q1"].description == "query param");

    WEBFR_ASSERT(endpoint->info()->headers["X-TEST-HEADER"].name == "X-TEST-HEADER");
    WEBFR_ASSERT(endpoint->info()->headers["X-TEST-HEADER"].description == "TEST-HEADER-PARAM");

    auto response = controller.pathParams("p1", "p2");
    WEBFR_ASSERT(response->getStatus().code == 200);

    webfr::data::stream::ChunkedBuffer stream;
    response->send(&stream, &headersOutBuffer, nullptr);

    WEBFR_LOGD(TAG, "response:\n---\n%s\n---\n", stream.toString()->c_str());

  }

  {
    auto endpoint = controller.Z__ENDPOINT_queryParams;
    WEBFR_ASSERT(endpoint);
    WEBFR_ASSERT(!endpoint->info()->summary);

    WEBFR_ASSERT(endpoint->info()->queryParams["param1"].name == "param1");
    WEBFR_ASSERT(endpoint->info()->queryParams["param1"].description == "this is param1");

    WEBFR_ASSERT(endpoint->info()->queryParams["param2"].name == "param2");
    WEBFR_ASSERT(!endpoint->info()->queryParams["param2"].description);

    auto response = controller.queryParams("p1", "p2");
    WEBFR_ASSERT(response->getStatus().code == 200);

    webfr::data::stream::ChunkedBuffer stream;
    response->send(&stream, &headersOutBuffer, nullptr);

    WEBFR_LOGD(TAG, "response:\n---\n%s\n---\n", stream.toString()->c_str());

  }

}

}}}}}