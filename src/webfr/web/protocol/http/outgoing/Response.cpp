#include "./Response.hpp"
#include "webfr/web/protocol/http/encoding/Chunked.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

Response::Response(const Status& status,
                   const std::shared_ptr<Body>& body)
  : m_status(status)
  , m_body(body)
{}

std::shared_ptr<Response> Response::createShared(const Status& status,
                                                 const std::shared_ptr<Body>& body) {
  return Shared_Outgoing_Response_Pool::allocateShared(status, body);
}

const Status& Response::getStatus() const {
  return m_status;
}

protocol::http::Headers& Response::getHeaders() {
  return m_headers;
}

void Response::putHeader(const webfr::String& key, const webfr::String& value) {
  m_headers.put(key, value);
}

bool Response::putHeaderIfNotExists(const webfr::String& key, const webfr::String& value) {
  return m_headers.putIfNotExists(key, value);
}

void Response::putHeader_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  m_headers.put(key, value);
}

bool Response::putHeaderIfNotExists_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  return m_headers.putIfNotExists(key, value);
}

webfr::String Response::getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const {
  return m_headers.get(headerName);
}

void Response::setConnectionUpgradeHandler(const std::shared_ptr<webfr::network::ConnectionHandler>& handler) {
  m_connectionUpgradeHandler = handler;
}

std::shared_ptr<webfr::network::ConnectionHandler> Response::getConnectionUpgradeHandler() {
  return m_connectionUpgradeHandler;
}

void Response::setConnectionUpgradeParameters(const std::shared_ptr<const ConnectionHandler::ParameterMap>& parameters) {
  m_connectionUpgradeParameters = parameters;
}

std::shared_ptr<const Response::ConnectionHandler::ParameterMap> Response::getConnectionUpgradeParameters() {
  return m_connectionUpgradeParameters;
}

void Response::send(data::stream::OutputStream* stream,
                    data::stream::BufferOutputStream* headersWriteBuffer,
                    http::encoding::EncoderProvider* contentEncoderProvider)
{

  v_int64 bodySize = -1;

  if(m_body){

    m_body->declareHeaders(m_headers);

    if(contentEncoderProvider == nullptr) {

      bodySize = m_body->getKnownSize();

      if (bodySize >= 0) {
        m_headers.put_LockFree(Header::CONTENT_LENGTH, utils::conversion::int64ToStr(bodySize));
      } else {
        m_headers.put_LockFree(Header::TRANSFER_ENCODING, Header::Value::TRANSFER_ENCODING_CHUNKED);
      }

    } else {
      m_headers.put_LockFree(Header::TRANSFER_ENCODING, Header::Value::TRANSFER_ENCODING_CHUNKED);
      m_headers.put_LockFree(Header::CONTENT_ENCODING, contentEncoderProvider->getEncodingName());
    }

  } else {
    m_headers.put_LockFree(Header::CONTENT_LENGTH, "0");
  }

  headersWriteBuffer->setCurrentPosition(0);

  headersWriteBuffer->writeSimple("HTTP/1.1 ", 9);
  headersWriteBuffer->writeAsString(m_status.code);
  headersWriteBuffer->writeSimple(" ", 1);
  headersWriteBuffer->writeSimple(m_status.description);
  headersWriteBuffer->writeSimple("\r\n", 2);

  http::Utils::writeHeaders(m_headers, headersWriteBuffer);

  headersWriteBuffer->writeSimple("\r\n", 2);

  if(m_body) {

    if(contentEncoderProvider == nullptr) {

      if (bodySize >= 0) {

        if(m_body->getKnownData() == nullptr) {
          headersWriteBuffer->flushToStream(stream);

          data::stream::transfer(m_body, stream, 0, headersWriteBuffer->getData(), headersWriteBuffer->getCapacity());
        } else { 
          if (bodySize + headersWriteBuffer->getCurrentPosition() < headersWriteBuffer->getCapacity()) {
            headersWriteBuffer->writeSimple(m_body->getKnownData(), bodySize);
            headersWriteBuffer->flushToStream(stream);
          } else {
            headersWriteBuffer->flushToStream(stream);
            stream->writeExactSizeDataSimple(m_body->getKnownData(), bodySize);
          }
        }
      } else {

        headersWriteBuffer->flushToStream(stream);

        http::encoding::EncoderChunked chunkedEncoder;

        data::stream::transfer(m_body, stream, 0, headersWriteBuffer->getData(), headersWriteBuffer->getCapacity(), &chunkedEncoder);

      }

    } else {

      headersWriteBuffer->flushToStream(stream);

      http::encoding::EncoderChunked chunkedEncoder;
      auto contentEncoder = contentEncoderProvider->getProcessor();

      data::buffer::ProcessingPipeline pipeline({
        contentEncoder,
        &chunkedEncoder
      });

      data::stream::transfer(m_body, stream, 0, headersWriteBuffer->getData(), headersWriteBuffer->getCapacity(), &pipeline);

    }

  } else {
    headersWriteBuffer->flushToStream(stream);
  }

}

webfr::async::CoroutineStarter Response::sendAsync(const std::shared_ptr<Response>& _this,
                                                   const std::shared_ptr<data::stream::OutputStream>& stream,
                                                   const std::shared_ptr<webfr::data::stream::BufferOutputStream>& headersWriteBuffer,
                                                   const std::shared_ptr<http::encoding::EncoderProvider>& contentEncoder)
{

  class SendAsyncCoroutine : public webfr::async::Coroutine<SendAsyncCoroutine> {
  private:
    std::shared_ptr<Response> m_this;
    std::shared_ptr<data::stream::OutputStream> m_stream;
    std::shared_ptr<webfr::data::stream::BufferOutputStream> m_headersWriteBuffer;
    std::shared_ptr<http::encoding::EncoderProvider> m_contentEncoderProvider;
  public:

    SendAsyncCoroutine(const std::shared_ptr<Response>& _this,
                       const std::shared_ptr<data::stream::OutputStream>& stream,
                       const std::shared_ptr<webfr::data::stream::BufferOutputStream>& headersWriteBuffer,
                       const std::shared_ptr<http::encoding::EncoderProvider>& contentEncoderProvider)
      : m_this(_this)
      , m_stream(stream)
      , m_headersWriteBuffer(headersWriteBuffer)
      , m_contentEncoderProvider(contentEncoderProvider)
    {}

    Action act() override {

      v_buff_size bodySize = -1;

      if(m_this->m_body){

        m_this->m_body->declareHeaders(m_this->m_headers);

        if(!m_contentEncoderProvider) {

          bodySize = m_this->m_body->getKnownSize();

          if (bodySize >= 0) {
            m_this->m_headers.put_LockFree(Header::CONTENT_LENGTH, utils::conversion::int64ToStr(bodySize));
          } else {
            m_this->m_headers.put_LockFree(Header::TRANSFER_ENCODING, Header::Value::TRANSFER_ENCODING_CHUNKED);
          }

        } else {
          m_this->m_headers.put_LockFree(Header::TRANSFER_ENCODING, Header::Value::TRANSFER_ENCODING_CHUNKED);
          m_this->m_headers.put_LockFree(Header::CONTENT_ENCODING, m_contentEncoderProvider->getEncodingName());
        }

      } else {
        m_this->m_headers.put_LockFree(Header::CONTENT_LENGTH, "0");
      }

      m_headersWriteBuffer->setCurrentPosition(0);

      m_headersWriteBuffer->writeSimple("HTTP/1.1 ", 9);
      m_headersWriteBuffer->writeAsString(m_this->m_status.code);
      m_headersWriteBuffer->writeSimple(" ", 1);
      m_headersWriteBuffer->writeSimple(m_this->m_status.description);
      m_headersWriteBuffer->writeSimple("\r\n", 2);

      http::Utils::writeHeaders(m_this->m_headers, m_headersWriteBuffer.get());

      m_headersWriteBuffer->writeSimple("\r\n", 2);

      if(m_this->m_body) {

        if(!m_contentEncoderProvider) {

          if (bodySize >= 0) {

            if (bodySize + m_headersWriteBuffer->getCurrentPosition() < m_headersWriteBuffer->getCapacity()) {

              m_headersWriteBuffer->writeSimple(m_this->m_body->getKnownData(), bodySize);
              return webfr::data::stream::BufferOutputStream::flushToStreamAsync(m_headersWriteBuffer, m_stream)
                .next(finish());

            } else {

              return webfr::data::stream::BufferOutputStream::flushToStreamAsync(m_headersWriteBuffer, m_stream)
                .next(m_stream->writeExactSizeDataAsync(m_this->m_body->getKnownData(), bodySize))
                .next(finish());
            }

          } else {

            auto chunkedEncoder = std::make_shared<http::encoding::EncoderChunked>();
            return webfr::data::stream::BufferOutputStream::flushToStreamAsync(m_headersWriteBuffer, m_stream)
              .next(data::stream::transferAsync(m_this->m_body, m_stream, 0, data::buffer::IOBuffer::createShared(), chunkedEncoder))
              .next(finish());

          }

        } else {

          auto chunkedEncoder = std::make_shared<http::encoding::EncoderChunked>();
          auto contentEncoder = m_contentEncoderProvider->getProcessor();

          auto pipeline = std::shared_ptr<data::buffer::ProcessingPipeline>(new data::buffer::ProcessingPipeline({
            contentEncoder,
            chunkedEncoder
          }));

          return webfr::data::stream::BufferOutputStream::flushToStreamAsync(m_headersWriteBuffer, m_stream)
            .next(data::stream::transferAsync(m_this->m_body, m_stream, 0, data::buffer::IOBuffer::createShared(), pipeline)
            . next(finish()));

        }

      } else {

        return webfr::data::stream::BufferOutputStream::flushToStreamAsync(m_headersWriteBuffer, m_stream)
          .next(finish());
      }

    }

  };

  return SendAsyncCoroutine::start(_this, stream, headersWriteBuffer, contentEncoder);

}

}}}}}