#ifndef webfr_test_web_app_Client_hpp
#define webfr_test_web_app_Client_hpp

#include "./DTOs.hpp"

#include "webfr/web/client/ApiClient.hpp"
#include "webfr/web/protocol/http/outgoing/MultipartBody.hpp"
#include "webfr/encoding/Base64.hpp"
#include "webfr/core/macro/codegen.hpp"

namespace webfr { namespace test { namespace web { namespace app {
  
class Client : public webfr::web::client::ApiClient {
public:
  typedef webfr::web::protocol::http::outgoing::MultipartBody MultipartBody;
public:

#include WEBFR_CODEGEN_BEGIN(ApiClient)
  
  API_CLIENT_INIT(Client)
  
  API_CALL("GET", "/", getRoot)
  API_CALL("GET", "/availability", getAvailability)
  API_CALL("GET", "/cors", getCors)
  API_CALL("OPTIONS", "/cors", optionsCors)
  API_CALL("GET", "/cors-origin", getCorsOrigin)
  API_CALL("GET", "/cors-origin-methods", getCorsOriginMethods)
  API_CALL("GET", "/cors-origin-methods-headers", getCorsOriginMethodsHeader)
  API_CALL("GET", "params/{param}", getWithParams, PATH(String, param))
  API_CALL("GET", "queries", getWithQueries, QUERY(String, name), QUERY(Int32, age))
  API_CALL("GET", "queries/optional", getWithOptQueries, QUERY(String, name))
  API_CALL("GET", "queries/map", getWithQueriesMap, QUERY(String, key1), QUERY(Int32, key2), QUERY(Float32, key3))
  API_CALL("GET", "headers", getWithHeaders, HEADER(String, param, "X-TEST-HEADER"))
  API_CALL("POST", "body", postBody, BODY_STRING(String, body))
  API_CALL("POST", "body-dto", postBodyDto, BODY_DTO(Object<TestDto>, body))

  API_CALL("GET", "enum/as-string", getHeaderEnumAsString, HEADER(Enum<AllowedPathParams>::AsString, enumValue, "enum"))
  API_CALL("GET", "enum/as-number", getHeaderEnumAsNumber, HEADER(Enum<AllowedPathParams>::AsNumber, enumValue, "enum"))

  API_CALL("POST", "echo", echoBody, BODY_STRING(String, body))
  API_CALL("GET", "header-value-set", headerValueSet, HEADER(String, valueSet, "X-VALUE-SET"))

  API_CALL("GET", "default-basic-authorization", defaultBasicAuthorization, AUTHORIZATION_BASIC(String, authString))
  API_CALL("GET", "default-basic-authorization", defaultBasicAuthorizationWithoutHeader)
  API_CALL("GET", "basic-authorization", customBasicAuthorization, AUTHORIZATION_BASIC(String, authString))
  API_CALL("GET", "basic-authorization", customBasicAuthorizationWithoutHeader)

  API_CALL("GET", "bearer-authorization", bearerAuthorization, AUTHORIZATION(String, authString, "Bearer"))

  API_CALL("GET", "chunked/{text-value}/{num-iterations}", getChunked, PATH(String, text, "text-value"), PATH(Int32, numIterations, "num-iterations"))
  API_CALL("POST", "test/multipart/{chunk-size}", multipartTest, PATH(Int32, chunkSize, "chunk-size"), BODY(std::shared_ptr<MultipartBody>, body))

  API_CALL("GET", "test/interceptors", getInterceptors)

  API_CALL_HEADERS(getDefaultHeaders1) {
    headers.put("X-DEFAULT", "hello_1");
  }
  API_CALL("GET", "default_headers", getDefaultHeaders1)

  API_CALL_HEADERS(getDefaultHeaders2) {
    headers.put("X-DEFAULT", "hello_2");
  }
  API_CALL("GET", "default_headers/{param}", getDefaultHeaders2, PATH(String, param))

  API_CALL_ASYNC("GET", "/", getRootAsync)
  API_CALL_ASYNC("GET", "/", getRootAsyncWithCKA, HEADER(String, connection, "Connection"))
  API_CALL_ASYNC("GET", "params/{param}", getWithParamsAsync, PATH(String, param))
  API_CALL_ASYNC("GET", "queries", getWithQueriesAsync, QUERY(String, name), QUERY(Int32, age))
  API_CALL_ASYNC("GET", "queries/map", getWithQueriesMapAsync, QUERY(String, key1), QUERY(Int32, key2), QUERY(Float32, key3))
  API_CALL_ASYNC("GET", "headers", getWithHeadersAsync, HEADER(String, param, "X-TEST-HEADER"))
  API_CALL_ASYNC("POST", "body", postBodyAsync, BODY_STRING(String, body))
  API_CALL_ASYNC("POST", "echo", echoBodyAsync, BODY_STRING(String, body))

  API_CALL_ASYNC("GET", "header-value-set", headerValueSetAsync, HEADER(String, valueSet, "X-VALUE-SET"))

  API_CALL_ASYNC("GET", "chunked/{text-value}/{num-iterations}", getChunkedAsync, PATH(String, text, "text-value"), PATH(Int32, numIterations, "num-iterations"))

  API_CALL_HEADERS(GetDefaultHeaders3) {
    headers.put("X-DEFAULT", "hello_3");
  }
  API_CALL_ASYNC("GET", "default_headers", GetDefaultHeaders3)

  API_CALL_HEADERS(GetDefaultHeaders4) {
    headers.put("X-DEFAULT", "hello_4");
  }
  API_CALL_ASYNC("GET", "default_headers/{param}", GetDefaultHeaders4, PATH(String, param))

#include WEBFR_CODEGEN_END(ApiClient)
};
  
}}}}

#endif /* WEBFR_test_web_app_Client_hpp */