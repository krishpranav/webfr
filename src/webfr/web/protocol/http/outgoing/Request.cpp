#include "Request.hpp"
#include "webfr/web/protocol/http/encoding/Chunked.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"
#include "webfr/core/data/buffer/IOBuffer.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace outgoing {

Request::Request(const webfr::data::share::StringKeyLabel& method,
                 const webfr::data::share::StringKeyLabel& path,
                 const Headers& headers,
                 const std::shared_ptr<Body>& body)
  : m_method(method)
  , m_path(path)
  , m_headers(headers)
  , m_body(body)
{}

std::shared_ptr<Request> Request::createShared(const webfr::data::share::StringKeyLabel& method,
                                               const webfr::data::share::StringKeyLabel& path,
                                               const Headers& headers,
                                               const std::shared_ptr<Body>& body) {
  return Shared_Outgoing_Request_Pool::allocateShared(method, path, headers, body);
}

const webfr::data::share::StringKeyLabel& Request::getMethod() const {
  return m_method;
}

const webfr::data::share::StringKeyLabel& Request::getPath() const {
  return m_path;
}

protocol::http::Headers& Request::getHeaders() {
  return m_headers;
}

void Request::putHeader(const webfr::String& key, const webfr::String& value) {
  m_headers.put(key, value);
}

bool Request::putHeaderIfNotExists(const webfr::String& key, const webfr::String& value) {
  return m_headers.putIfNotExists(key, value);
}

void Request::putHeader_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  m_headers.put(key, value);
}

bool Request::putHeaderIfNotExists_Unsafe(const webfr::data::share::StringKeyLabelCI_FAST& key, const webfr::data::share::StringKeyLabel& value) {
  return m_headers.putIfNotExists(key, value);
}

webfr::String Request::getHeader(const webfr::data::share::StringKeyLabelCI_FAST& headerName) const{
  return m_headers.get(headerName);
}

std::shared_ptr<Body> Request::getBody() {
  return m_body;
}

void Request::send(data::stream::OutputStream* stream){

  v_buff_size bodySize = -1;

  if(m_body){

    m_body->declareHeaders(m_headers);

    bodySize = m_body->getKnownSize();

    if(bodySize >= 0) {
      m_headers.put_LockFree(Header::CONTENT_LENGTH, utils::conversion::int64ToStr(bodySize));
    } else {
      m_headers.put_LockFree(Header::TRANSFER_ENCODING, Header::Value::TRANSFER_ENCODING_CHUNKED);
    }

  } else {
    m_headers.put_LockFree(Header::CONTENT_LENGTH, "0");
  }

  webfr::data::stream::BufferOutputStream buffer(2048);

  buffer.writeSimple(m_method.getData(), m_method.getSize());
  buffer.writeSimple(" /", 2);
  buffer.writeSimple(m_path.getData(), m_path.getSize());
  buffer.writeSimple(" ", 1);
  buffer.writeSimple("HTTP/1.1", 8);
  buffer.writeSimple("\r\n", 2);

  http::Utils::writeHeaders(m_headers, &buffer);

  buffer.writeSimple("\r\n", 2);

  if(m_body) {

    if(bodySize >= 0) {

      if(bodySize + buffer.getCurrentPosition() < buffer.getCapacity()) {
        buffer.writeSimple(m_body->getKnownData(), bodySize);
        buffer.flushToStream(stream);
      } else {
        buffer.flushToStream(stream);
        stream->writeExactSizeDataSimple(m_body->getKnownData(), bodySize);
      }

    } else {

      buffer.flushToStream(stream);

      http::encoding::EncoderChunked chunkedEncoder;

      /* Reuse headers buffer */
      buffer.setCurrentPosition(0);
      data::stream::transfer(m_body, stream, 0, buffer.getData(), buffer.getCapacity(), &chunkedEncoder);

    }

  } else {
    buffer.flushToStream(stream);
  }
  
}

webfr::async::CoroutineStarter Request::sendAsync(std::shared_ptr<Request> _this,
                                                  const std::shared_ptr<data::stream::OutputStream>& stream)
{
  
  class SendAsyncCoroutine : public webfr::async::Coroutine<SendAsyncCoroutine> {
  private:
    std::shared_ptr<Request> m_this;
    std::shared_ptr<data::stream::OutputStream> m_stream;
    std::shared_ptr<webfr::data::stream::BufferOutputStream> m_headersWriteBuffer;
  public:
    
    SendAsyncCoroutine(std::shared_ptr<Request> request,
                       const std::shared_ptr<data::stream::OutputStream>& stream)
      : m_this(std::move(request))
      , m_stream(stream)
      , m_headersWriteBuffer(std::make_shared<webfr::data::stream::BufferOutputStream>())
    {}
    
    Action act() override {

      v_buff_size bodySize = -1;

      if(m_this->m_body){

        m_this->m_body->declareHeaders(m_this->m_headers);

        bodySize = m_this->m_body->getKnownSize();

        if(bodySize >= 0) {
          m_this->m_headers.put_LockFree(Header::CONTENT_LENGTH, utils::conversion::int64ToStr(bodySize));
        } else {
          m_this->m_headers.put_LockFree(Header::TRANSFER_ENCODING, Header::Value::TRANSFER_ENCODING_CHUNKED);
        }

      } else {
        m_this->m_headers.put_LockFree(Header::CONTENT_LENGTH, "0");
      }
      
      m_headersWriteBuffer->writeSimple(m_this->m_method.getData(), m_this->m_method.getSize());
      m_headersWriteBuffer->writeSimple(" /", 2);
      m_headersWriteBuffer->writeSimple(m_this->m_path.getData(), m_this->m_path.getSize());
      m_headersWriteBuffer->writeSimple(" ", 1);
      m_headersWriteBuffer->writeSimple("HTTP/1.1", 8);
      m_headersWriteBuffer->writeSimple("\r\n", 2);

      http::Utils::writeHeaders(m_this->m_headers, m_headersWriteBuffer.get());
      
      m_headersWriteBuffer->writeSimple("\r\n", 2);

      if(m_this->m_body) {

        if(bodySize >= 0) {

          if(bodySize + m_headersWriteBuffer->getCurrentPosition() < m_headersWriteBuffer->getCapacity()) {

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

        return webfr::data::stream::BufferOutputStream::flushToStreamAsync(m_headersWriteBuffer, m_stream)
          .next(finish());
      }
      
    }
    
  };
  
  return SendAsyncCoroutine::start(std::move(_this), stream);
  
}
  
}}}}}