#ifndef webfr_test_web_app_ControllerAsync_hpp
#define webfr_test_web_app_ControllerAsync_hpp

#include "./DTOs.hpp"
#include "webfr/web/mime/multipart/FileStreamProvider.hpp"
#include "webfr/web/mime/multipart/InMemoryPartReader.hpp"
#include "webfr/web/mime/multipart/Reader.hpp"
#include "webfr/web/mime/multipart/PartList.hpp"
#include "webfr/web/protocol/http/outgoing/MultipartBody.hpp"
#include "webfr/web/protocol/http/outgoing/StreamingBody.hpp"
#include "webfr/web/server/api/ApiController.hpp"
#include "webfr/parser/json/mapping/ObjectMapper.hpp"
#include "webfr/core/data/stream/FileStream.hpp"
#include "webfr/core/data/stream/Stream.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include "webfr/core/macro/codegen.hpp"
#include "webfr/core/macro/component.hpp"

namespace webfr { namespace test { namespace web { namespace app {

namespace multipart = webfr::web::mime::multipart;

class ControllerAsync : public webfr::web::server::api::ApiController {
private:
  static constexpr const char* TAG = "test::web::app::ControllerAsync";
public:
  ControllerAsync(const std::shared_ptr<ObjectMapper>& objectMapper)
    : webfr::web::server::api::ApiController(objectMapper)
  {}
public:
  
  static std::shared_ptr<ControllerAsync> createShared(const std::shared_ptr<ObjectMapper>& objectMapper = WEBFR_GET_COMPONENT(std::shared_ptr<ObjectMapper>)){
    return std::make_shared<ControllerAsync>(objectMapper);
  }
  
#include WEBFR_CODEGEN_BEGIN(ApiController)

  ENDPOINT_ASYNC("GET", "/", Root) {
    
    ENDPOINT_ASYNC_INIT(Root)
    
    Action act() {
      //WEBFR_LOGV(TAG, "GET '/'");
      return _return(controller->createResponse(Status::CODE_200, "Hello World Async!!!"));
    }

  };

  ENDPOINT_ASYNC("GET", "params/{param}", GetWithParams) {

    ENDPOINT_ASYNC_INIT(GetWithParams)

    Action act() {
      auto param = request->getPathVariable("param");
      //WEBFR_LOGV(TAG, "GET params/%s", param->c_str());
      auto dto = TestDto::createShared();
      dto->testValue = param;
      return _return(controller->createDtoResponse(Status::CODE_200, dto));
    }

  };

  ENDPOINT_ASYNC("GET", "headers", GetWithHeaders) {

    ENDPOINT_ASYNC_INIT(GetWithHeaders)

    Action act() {
      auto param = request->getHeader("X-TEST-HEADER");
      //WEBFR_LOGV(TAG, "GET headers {X-TEST-HEADER: %s}", param->c_str());
      auto dto = TestDto::createShared();
      dto->testValue = param;
      return _return(controller->createDtoResponse(Status::CODE_200, dto));
    }

  };

  ENDPOINT_ASYNC("POST", "body", PostBody) {

    ENDPOINT_ASYNC_INIT(PostBody)

    Action act() {
      //WEBFR_LOGV(TAG, "POST body. Reading body...");
      return request->readBodyToStringAsync().callbackTo(&PostBody::onBodyRead);
    }

    Action onBodyRead(const String& body) {
      //WEBFR_LOGV(TAG, "POST body %s", body->c_str());
      auto dto = TestDto::createShared();
      dto->testValue = body;
      return _return(controller->createDtoResponse(Status::CODE_200, dto));
    }

  };

  ENDPOINT_ASYNC("POST", "echo", Echo) {

    ENDPOINT_ASYNC_INIT(Echo)

    Action act() {
      //WEBFR_LOGV(TAG, "POST body(echo). Reading body...");
      return request->readBodyToStringAsync().callbackTo(&Echo::onBodyRead);
    }

    Action onBodyRead(const String& body) {
      //WEBFR_LOGV(TAG, "POST echo size=%d", body->getSize());
      return _return(controller->createResponse(Status::CODE_200, body));
    }

  };

  ENDPOINT_ASYNC("GET", "chunked/{text-value}/{num-iterations}", Chunked) {

    ENDPOINT_ASYNC_INIT(Chunked)

    class ReadCallback : public webfr::data::stream::ReadCallback {
    private:
      webfr::String m_text;
      v_int32 m_counter;
      v_int32 m_iterations;
      data::buffer::InlineReadData m_inlineData;
    public:

      ReadCallback(const webfr::String& text, v_int32 iterations)
        : m_text(text)
        , m_counter(0)
        , m_iterations(iterations)
        , m_inlineData(text->getData(), text->getSize())
      {}

      v_io_size read(void *buffer, v_buff_size count, async::Action& action) override {

        if(m_counter < m_iterations) {

          v_buff_size desiredToRead = m_inlineData.bytesLeft;

          if (desiredToRead > 0) {

            if (desiredToRead > count) {
              desiredToRead = count;
            }

            std::memcpy(buffer, m_inlineData.currBufferPtr, desiredToRead);
            m_inlineData.inc(desiredToRead);

            if (m_inlineData.bytesLeft == 0) {
              m_inlineData.set(m_text->getData(), m_text->getSize());
              m_counter++;
            }

            return desiredToRead;

          }

        }

        return 0;

      }

    };

    Action act() {
      webfr::String text = request->getPathVariable("text-value");
      auto numIterations = webfr::utils::conversion::strToInt32(request->getPathVariable("num-iterations")->c_str());

      auto body = std::make_shared<webfr::web::protocol::http::outgoing::StreamingBody>
        (std::make_shared<ReadCallback>(text, numIterations));

      return _return(OutgoingResponse::createShared(Status::CODE_200, body));
    }

  };

  ENDPOINT_ASYNC("POST", "test/multipart/{chunk-size}", MultipartTest) {

    ENDPOINT_ASYNC_INIT(MultipartTest)

    v_int32 m_chunkSize;
    std::shared_ptr<webfr::web::mime::multipart::PartList> m_multipart;

    Action act() override {

      m_chunkSize = webfr::utils::conversion::strToInt32(request->getPathVariable("chunk-size")->c_str());

      m_multipart = std::make_shared<webfr::web::mime::multipart::PartList>(request->getHeaders());
      auto multipartReader = std::make_shared<webfr::web::mime::multipart::AsyncReader>(m_multipart);

      multipartReader->setDefaultPartReader(std::make_shared<webfr::web::mime::multipart::AsyncInMemoryPartReader>(10));

      return request->transferBodyAsync(multipartReader).next(yieldTo(&MultipartTest::respond));

    }

    Action respond() {

      auto responseBody = std::make_shared<webfr::web::protocol::http::outgoing::MultipartBody>(m_multipart);
      return _return(OutgoingResponse::createShared(Status::CODE_200, responseBody));

    }

  };


  ENDPOINT_ASYNC("POST", "test/multipart-all", MultipartUpload) {

    ENDPOINT_ASYNC_INIT(MultipartUpload)

    /* Coroutine State */
    std::shared_ptr<multipart::PartList> m_multipart;

    Action act() override {

      m_multipart = std::make_shared<multipart::PartList>(request->getHeaders());
      auto multipartReader = std::make_shared<multipart::AsyncReader>(m_multipart);

      /* Configure to read part with name "part1" into memory */
      multipartReader->setPartReader("part1", multipart::createAsyncInMemoryPartReader(256 /* max-data-size */));

      /* Configure to stream part with name "part2" to file */
      multipartReader->setPartReader("part2", multipart::createAsyncFilePartReader("/Users/leonid/Documents/test/my-text-file.tf"));

      /* Configure to read all other parts into memory */
      multipartReader->setDefaultPartReader(multipart::createAsyncInMemoryPartReader(16 * 1024 /* max-data-size */));

      /* Read multipart body */
      return request->transferBodyAsync(multipartReader).next(yieldTo(&MultipartUpload::onUploaded));

    }

    Action onUploaded() {

      /* Print number of uploaded parts */
      WEBFR_LOGD("Multipart", "parts_count=%d", m_multipart->count());

      /* Get multipart by name */
      auto part1 = m_multipart->getNamedPart("part1");

      /* Asser part not-null */
      WEBFR_ASSERT_HTTP(part1, Status::CODE_400, "part1 is empty");

      /* Print value of "part1" */
      WEBFR_LOGD("Multipart", "part1='%s'", part1->getInMemoryData()->c_str());

      /* Get multipart by name */
      auto filePart = m_multipart->getNamedPart("part2");

      /* Asser part not-null */
      WEBFR_ASSERT_HTTP(filePart, Status::CODE_400, "part2 is empty");

      auto inputStream = filePart->getInputStream();

      // TODO - process file stream.

      return _return(controller->createResponse(Status::CODE_200, "OK"));

    }

  };


  class MPStream : public webfr::web::mime::multipart::Multipart {
  public:
    typedef webfr::web::mime::multipart::Part Part;
  private:
    v_uint32 counter = 0;
    bool m_wait = false;
  public:

    MPStream()
      : webfr::web::mime::multipart::Multipart(generateRandomBoundary())
    {}

    std::shared_ptr<Part> readNextPart(async::Action& action) override {

      if(counter == 10) {
        return nullptr;
      }

      if(m_wait) {
        m_wait = false;
        action = async::Action::createWaitRepeatAction(1000 * 1000 + webfr::base::Environment::getMicroTickCount());
        return nullptr;
      }

      m_wait = true;

      auto part = std::make_shared<Part>();
      part->putHeader(Header::CONTENT_TYPE, "text/html");

      webfr::String frameData;

//      if(counter % 2 == 0) {
//        frameData = "<html><body>0</body></html>";
//      } else {
//        frameData = "<html><body>1</body></html>";
//      }
//      part->setDataInfo(std::make_shared<webfr::data::stream::BufferInputStream>(frameData));

      if(counter % 2 == 0) {
        part->setDataInfo(std::make_shared<webfr::data::stream::FileInputStream>("/Users/leonid/Documents/test/frame1.jpg"));
      } else {
        part->setDataInfo(std::make_shared<webfr::data::stream::FileInputStream>("/Users/leonid/Documents/test/frame2.jpg"));
      }

      ++ counter;

      WEBFR_LOGD("Multipart", "Frame sent!");

      return part;

    }

    void writeNextPart(const std::shared_ptr<Part>& part, async::Action& action) override {
      throw std::runtime_error("No writes here!!!");
    }

  };

  ENDPOINT_ASYNC("GET", "multipart-stream", MultipartStream) {

    ENDPOINT_ASYNC_INIT(MultipartStream)

    Action act() {
      auto multipart = std::make_shared<MPStream>();
      auto body = std::make_shared<webfr::web::protocol::http::outgoing::MultipartBody>(
        multipart,
        "multipart/x-mixed-replace",
        true /* flush parts */
      );
      return _return(OutgoingResponse::createShared(Status::CODE_200, body));
    }

  };

#include WEBFR_CODEGEN_END(ApiController)
  
};
  
}}}}

#endif /* WEBFR_test_web_app_ControllerAsync_hpp */