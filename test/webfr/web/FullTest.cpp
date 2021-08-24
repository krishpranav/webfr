#include "FullTest.hpp"
#include "webfr/web/app/Client.hpp"
#include "webfr/web/app/ControllerWithInterceptors.hpp"
#include "webfr/web/app/Controller.hpp"
#include "webfr/web/app/BasicAuthorizationController.hpp"
#include "webfr/web/app/BearerAuthorizationController.hpp"
#include "webfr/web/client/HttpRequestExecutor.hpp"
#include "webfr/web/server/HttpConnectionHandler.hpp"
#include "webfr/web/server/HttpRouter.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/network/tcp/server/ConnectionProvider.hpp"
#include "webfr/network/tcp/client/ConnectionProvider.hpp"
#include "webfr/network/virtual_/client/ConnectionProvider.hpp"
#include "webfr/network/virtual_/server/ConnectionProvider.hpp"
#include "webfr/network/virtual_/Interface.hpp"
#include "webfr/core/macro/component.hpp"
#include "webfr-test/web/ClientServerTestRunner.hpp"

namespace webfr { namespace test { namespace web {

namespace {

typedef webfr::web::mime::multipart::PartList PartList;
typedef webfr::web::protocol::http::outgoing::MultipartBody MultipartBody;

class TestComponent {
private:
  v_uint16 m_port;
public:

  TestComponent(v_uint16 port)
    : m_port(port)
  {}

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::virtual_::Interface>, virtualInterface)([] {
    return webfr::network::virtual_::Interface::obtainShared("virtualhost");
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::ServerConnectionProvider>, serverConnectionProvider)([this] {

    if(m_port == 0) { 
      WEBFR_COMPONENT(std::shared_ptr<webfr::network::virtual_::Interface>, interface);
      return std::static_pointer_cast<webfr::network::ServerConnectionProvider>(
        webfr::network::virtual_::server::ConnectionProvider::createShared(interface)
      );
    }

    return std::static_pointer_cast<webfr::network::ServerConnectionProvider>(
      webfr::network::tcp::server::ConnectionProvider::createShared({"localhost", m_port})
    );

  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::web::server::HttpRouter>, httpRouter)([] {
    return webfr::web::server::HttpRouter::createShared();
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::ConnectionHandler>, serverConnectionHandler)([] {
    WEBFR_COMPONENT(std::shared_ptr<webfr::web::server::HttpRouter>, router);
    return webfr::web::server::HttpConnectionHandler::createShared(router);
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::data::mapping::ObjectMapper>, objectMapper)([] {
    return webfr::parser::json::mapping::ObjectMapper::createShared();
  }());

  WEBFR_CREATE_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, clientConnectionProvider)([this] {

    if(m_port == 0) {
      WEBFR_COMPONENT(std::shared_ptr<webfr::network::virtual_::Interface>, interface);
      return std::static_pointer_cast<webfr::network::ClientConnectionProvider>(
        webfr::network::virtual_::client::ConnectionProvider::createShared(interface)
      );
    }

    return std::static_pointer_cast<webfr::network::ClientConnectionProvider>(
      webfr::network::tcp::client::ConnectionProvider::createShared({"localhost", m_port})
    );

  }());

};

std::shared_ptr<PartList> createMultipart(const std::unordered_map<webfr::String, webfr::String>& map) {

  auto multipart = webfr::web::mime::multipart::PartList::createSharedWithRandomBoundary();

  for(auto& pair : map) {

    webfr::web::mime::multipart::Headers partHeaders;
    auto part = std::make_shared<webfr::web::mime::multipart::Part>(partHeaders);
    multipart->writeNextPartSimple(part);
    part->putHeader("Content-Disposition", "form-data; name=\"" + pair.first + "\"");
    part->setDataInfo(std::make_shared<webfr::data::stream::BufferInputStream>(pair.second));

  }

  return multipart;

}

}
  
void FullTest::onRun() {

  TestComponent component(m_port);

  webfr::test::web::ClientServerTestRunner runner;

  runner.addController(app::Controller::createShared());
  runner.addController(app::ControllerWithInterceptors::createShared());
  runner.addController(app::DefaultBasicAuthorizationController::createShared());
  runner.addController(app::BasicAuthorizationController::createShared());
  runner.addController(app::BearerAuthorizationController::createShared());

  runner.run([this, &runner] {

    WEBFR_COMPONENT(std::shared_ptr<webfr::network::ClientConnectionProvider>, clientConnectionProvider);
    WEBFR_COMPONENT(std::shared_ptr<webfr::data::mapping::ObjectMapper>, objectMapper);

    auto requestExecutor = webfr::web::client::HttpRequestExecutor::createShared(clientConnectionProvider);
    auto client = app::Client::createShared(requestExecutor, objectMapper);

    auto connection = client->getConnection();
    WEBFR_ASSERT(connection);

    v_int32 iterationsStep = m_iterationsPerStep;

    auto lastTick = webfr::base::Environment::getMicroTickCount();

    for(v_int32 i = 0; i < iterationsStep * 10; i ++) {

      { 
        auto response = client->getRoot(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        WEBFR_ASSERT(value == "Hello World!!!");
      }

      { 
        auto response = client->getCors(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        WEBFR_ASSERT(value == "Ping");
        auto header = response->getHeader(webfr::web::protocol::http::Header::CORS_ORIGIN);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "*");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_METHODS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "GET, POST, OPTIONS");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_HEADERS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization");
      }

      { 
        auto response = client->optionsCors(connection);
        WEBFR_ASSERT(response->getStatusCode() == 204);
        auto header = response->getHeader(webfr::web::protocol::http::Header::CORS_ORIGIN);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "*");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_METHODS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "GET, POST, OPTIONS");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_HEADERS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization");
      }

      { 
        auto response = client->getCorsOrigin(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        WEBFR_ASSERT(value == "Pong");
        auto header = response->getHeader(webfr::web::protocol::http::Header::CORS_ORIGIN);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "127.0.0.1");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_METHODS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "GET, POST, OPTIONS");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_HEADERS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization");
      }

      { 
        auto response = client->getCorsOriginMethods(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        WEBFR_ASSERT(value == "Ping");
        auto header = response->getHeader(webfr::web::protocol::http::Header::CORS_ORIGIN);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "127.0.0.1");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_METHODS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "GET, OPTIONS");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_HEADERS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "DNT, User-Agent, X-Requested-With, If-Modified-Since, Cache-Control, Content-Type, Range, Authorization");
      }

      { 
        auto response = client->getCorsOriginMethodsHeader(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        WEBFR_ASSERT(value == "Pong");
        auto header = response->getHeader(webfr::web::protocol::http::Header::CORS_ORIGIN);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "127.0.0.1");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_METHODS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "GET, OPTIONS");
        header = response->getHeader(webfr::web::protocol::http::Header::CORS_HEADERS);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "X-PWNT");
      }

      { 
        auto response = client->getWithParams("my_test_param", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testValue == "my_test_param");
      }

      { 
        auto response = client->getWithQueries("webfr", 1, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testValue == "name=webfr&age=1");
      }

      { // test GET with optional query parameters
        auto response = client->getWithOptQueries("webfr", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testValue == "name=webfr&age=101");
      }

      { // test GET with query parameters
        auto response = client->getWithQueriesMap("value1", 32, 0.32f, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testMap);
        WEBFR_ASSERT(dto->testMap->size() == 3);
        WEBFR_ASSERT(dto->testMap["key1"] == "value1");
        WEBFR_ASSERT(dto->testMap["key2"] == "32");
        WEBFR_ASSERT(dto->testMap["key3"] == webfr::utils::conversion::float32ToStr(0.32f));
      }

      { // test GET with header parameter
        auto response = client->getWithHeaders("my_test_header", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testValue == "my_test_header");
      }

      { // test POST with body
        auto response = client->postBody("my_test_body", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dto = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dto);
        WEBFR_ASSERT(dto->testValue == "my_test_body");
      }

      { // test POST with dto body
        auto dtoIn = app::TestDto::createShared();
        dtoIn->testValueInt = i;
        auto response = client->postBodyDto(dtoIn, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto dtoOut = response->readBodyToDto<webfr::Object<app::TestDto>>(objectMapper.get());
        WEBFR_ASSERT(dtoOut);
        WEBFR_ASSERT(dtoOut->testValueInt == i);
      }

      { // test Enum as String

        WEBFR_ASSERT(webfr::Enum<app::AllowedPathParams>::getEntries().size() == 2);

        webfr::Enum<app::AllowedPathParams> v = app::AllowedPathParams::HELLO;
        auto response = client->getHeaderEnumAsString(v, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
      }

      { // test Enum as String
        webfr::Enum<app::AllowedPathParams> v = app::AllowedPathParams::HELLO;
        auto response = client->getHeaderEnumAsNumber(v, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
      }

      { // test Big Echo with body
        webfr::data::stream::ChunkedBuffer stream;
        for(v_int32 i = 0; i < webfr::data::buffer::IOBuffer::BUFFER_SIZE; i++) {
          stream.writeSimple("0123456789", 10);
        }
        auto data = stream.toString();
        auto response = client->echoBody(data, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto returnedData = response->readBodyToString();
        WEBFR_ASSERT(returnedData);
        WEBFR_ASSERT(returnedData == data);
      }

      {
        auto response = client->headerValueSet("   VALUE_1, VALUE_2,  VALUE_3", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
      }

      {
        auto response = client->getDefaultHeaders1(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
      }

      {
        auto response = client->getDefaultHeaders2("some param", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
      }

      { // test custom authorization handler with custom authorization object
        auto response = client->defaultBasicAuthorization("foo:bar", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
      }

      { // test call of an endpoint that requiers authorization headers, but we don't send one
        auto response = client->defaultBasicAuthorizationWithoutHeader();
        WEBFR_ASSERT(response->getStatusCode() == 401);
        webfr::String body = response->readBodyToString();
        WEBFR_ASSERT(body == "server=webfr/" WEBFR_VERSION "\n"
                             "code=401\n"
                             "description=Unauthorized\n"
                             "message=Authorization Required\n");
        // should also add the WWW-Authenticate header when Authorization is missing
        auto header = response->getHeader(webfr::web::protocol::http::Header::WWW_AUTHENTICATE);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "Basic realm=\"default-test-realm\"");
      }

      { // test custom authorization handler with custom authorization object
        auto response = client->customBasicAuthorization("foo:bar", connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
      }

      { // test call of an endpoint that requiers authorization headers, but we don't send one
        auto response = client->customBasicAuthorizationWithoutHeader();
        WEBFR_ASSERT(response->getStatusCode() == 401);
        webfr::String body = response->readBodyToString();
        WEBFR_ASSERT(body == "server=webfr/" WEBFR_VERSION "\n"
                             "code=401\n"
                             "description=Unauthorized\n"
                             "message=Authorization Required\n");
        // should also add the WWW-Authenticate header when Authorization is missing
        auto header = response->getHeader(webfr::web::protocol::http::Header::WWW_AUTHENTICATE);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "Basic realm=\"custom-test-realm\"");
      }

      { // test custom authorization handler with custom authorization object with unknown credentials where the
        // handler returns nullptr
        auto response = client->customBasicAuthorization("john:doe");
        webfr::String body = response->readBodyToString();
        WEBFR_ASSERT(response->getStatusCode() == 401);
        WEBFR_ASSERT(body == "server=webfr/" WEBFR_VERSION "\n"
                             "code=401\n"
                             "description=Unauthorized\n"
                             "message=Unauthorized\n");
        // should also add the WWW-Authenticate header when Authorization is missing or wrong
        auto header = response->getHeader(webfr::web::protocol::http::Header::WWW_AUTHENTICATE);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header->startsWith("Basic realm=\"custom-test-realm\""));
      }

      { // test custom authorization handler with custom authorization method
        webfr::String token = "4e99e8c12de7e01535248d2bac85e732";
        auto response = client->bearerAuthorization(token);
        webfr::String body = response->readBodyToString();
        WEBFR_ASSERT(response->getStatusCode() == 200);
      }

      { // test custom authorization handler with custom authorization object with unknown credentials where the
        // handler returns nullptr
        webfr::String token = "900150983cd24fb0d6963f7d28e17f72";
        auto response = client->bearerAuthorization(token);
        webfr::String body = response->readBodyToString();
        WEBFR_ASSERT(response->getStatusCode() == 401);
        WEBFR_ASSERT(body == "server=webfr/" WEBFR_VERSION "\n"
                             "code=401\n"
                             "description=Unauthorized\n"
                             "message=Unauthorized\n");
        // should also add the WWW-Authenticate header when Authorization is missing or wrong
        auto header = response->getHeader(webfr::web::protocol::http::Header::WWW_AUTHENTICATE);
        WEBFR_ASSERT(header);
        WEBFR_ASSERT(header == "Bearer realm=\"custom-bearer-realm\"");
      }

      { // test Chunked body
        webfr::String sample = "__abcdefghijklmnopqrstuvwxyz-0123456789";
        v_int32 numIterations = 10;
        webfr::data::stream::ChunkedBuffer stream;
        for(v_int32 i = 0; i < numIterations; i++) {
          stream.writeSimple(sample->getData(), sample->getSize());
        }
        auto data = stream.toString();
        auto response = client->getChunked(sample, numIterations, connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto returnedData = response->readBodyToString();
        WEBFR_ASSERT(returnedData);
        WEBFR_ASSERT(returnedData == data);
      }

      { // Multipart body

        std::unordered_map<webfr::String, webfr::String> map;
        map["value1"] = "Hello";
        map["value2"] = "World";
        auto multipart = createMultipart(map);

        auto body = std::make_shared<MultipartBody>(multipart);

        auto response = client->multipartTest(i + 1, body);
        WEBFR_ASSERT(response->getStatusCode() == 200);

        multipart = std::make_shared<webfr::web::mime::multipart::PartList>(response->getHeaders());

        webfr::web::mime::multipart::Reader multipartReader(multipart.get());
        multipartReader.setPartReader("value1", std::make_shared<webfr::web::mime::multipart::InMemoryPartReader>(10));
        multipartReader.setPartReader("value2", std::make_shared<webfr::web::mime::multipart::InMemoryPartReader>(10));

        response->transferBody(&multipartReader);

        WEBFR_ASSERT(multipart->getAllParts().size() == 2);
        auto part1 = multipart->getNamedPart("value1");
        auto part2 = multipart->getNamedPart("value2");

        WEBFR_ASSERT(part1);
        WEBFR_ASSERT(part2);

        WEBFR_ASSERT(part1->getInMemoryData() == "Hello");
        WEBFR_ASSERT(part2->getInMemoryData() == "World");

      }

      { // test interceptors
        auto response = client->getInterceptors(connection);
        WEBFR_ASSERT(response->getStatusCode() == 200);
        auto value = response->readBodyToString();
        WEBFR_ASSERT(value == "Hello World!!!");
      }

      if((i + 1) % iterationsStep == 0) {
        auto ticks = webfr::base::Environment::getMicroTickCount() - lastTick;
        lastTick = webfr::base::Environment::getMicroTickCount();
        WEBFR_LOGV("i", "%d, tick=%d", i + 1, ticks);
      }

    }

  }, std::chrono::minutes(10));

  std::this_thread::sleep_for(std::chrono::seconds(1));

}
  
}}}