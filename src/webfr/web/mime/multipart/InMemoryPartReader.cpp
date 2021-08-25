#include "InMemoryPartReader.hpp"
#include "webfr/core/data/stream/BufferStream.hpp"

namespace webfr { namespace web { namespace mime { namespace multipart {

const char* const InMemoryPartReader::TAG_NAME = "[webfr::web::mime::multipart::InMemoryPartReader::TAG]";

InMemoryPartReader::InMemoryPartReader(v_io_size maxDataSize)
  : m_maxDataSize(maxDataSize)
{}

void InMemoryPartReader::onNewPart(const std::shared_ptr<Part>& part) {

  auto tag = part->getTagObject();

  if(tag) {
    throw std::runtime_error("[webfr::web::mime::multipart::InMemoryPartReader::onNewPart()]: Error. "
                             "Part tag object is not nullptr. Seems like this part is already being processed by another reader.");
  }

  auto buffer = webfr::data::stream::ChunkedBuffer::createShared();
  part->setTag(TAG_NAME, buffer);

}

void InMemoryPartReader::onPartData(const std::shared_ptr<Part>& part, p_char8 data, webfr::v_io_size size) {

  auto tag = part->getTagObject();
  if(!tag) {
    throw std::runtime_error("[webfr::web::mime::multipart::InMemoryPartReader::onPartData()]: Error. "
                             "Part tag object is nullptr.");

  }

  if(part->getTagName() != TAG_NAME) {
    throw std::runtime_error("[webfr::web::mime::multipart::InMemoryPartReader::onPartData()]: Error. "
                             "Wrong tag name. Seems like this part is already being processed by another part reader.");

  }

  auto buffer = std::static_pointer_cast<webfr::data::stream::ChunkedBuffer>(tag);

  if(size > 0) {
    if(buffer->getSize() + size > m_maxDataSize) {
      WEBFR_LOGE("[webfr::web::mime::multipart::InMemoryPartReader::onPartData()]", "Error. Part size exceeds specified maxDataSize=%d", m_maxDataSize);
      throw std::runtime_error("[webfr::web::mime::multipart::InMemoryPartReader::onPartData()]: Error. Part size exceeds specified maxDataSize");
    }
    buffer->writeSimple(data, size);
  } else {
    auto fullData = buffer->toString();
    buffer->clear();
    part->clearTag();
    auto stream = std::make_shared<data::stream::BufferInputStream>(fullData.getPtr(), fullData->getData(), fullData->getSize());
    part->setDataInfo(stream, fullData, fullData->getSize());
  }

}

const char* const AsyncInMemoryPartReader::TAG_NAME = "[webfr::web::mime::multipart::AsyncInMemoryPartReader::TAG]";

AsyncInMemoryPartReader::AsyncInMemoryPartReader(v_io_size maxDataSize)
  : m_maxDataSize(maxDataSize)
{}

async::CoroutineStarter AsyncInMemoryPartReader::onNewPartAsync(const std::shared_ptr<Part>& part) {

  auto tag = part->getTagObject();

  if(tag) {
    throw std::runtime_error("[webfr::web::mime::multipart::AsyncInMemoryPartReader::onNewPartAsync()]: Error. "
                             "Part tag object is not nullptr. Seems like this part is already being processed by another part reader.");
  }

  auto buffer = webfr::data::stream::ChunkedBuffer::createShared();
  part->setTag(TAG_NAME, buffer);

  return nullptr;

}

async::CoroutineStarter AsyncInMemoryPartReader::onPartDataAsync(const std::shared_ptr<Part>& part, p_char8 data, webfr::v_io_size size) {

  auto tag = part->getTagObject();
  if(!tag) {
    throw std::runtime_error("[webfr::web::mime::multipart::AsyncInMemoryPartReader::onPartDataAsync()]: Error. "
                             "Part tag object is nullptr.");

  }

  if(part->getTagName() != TAG_NAME) {
    throw std::runtime_error("[webfr::web::mime::multipart::AsyncInMemoryPartReader::onNewPartAsync()]: Error. "
                             "Wrong tag name. Seems like this part is already being processed by another part reader.");

  }

  auto buffer = std::static_pointer_cast<webfr::data::stream::ChunkedBuffer>(tag);

  if(size > 0) {
    if(buffer->getSize() + size > m_maxDataSize) {
      WEBFR_LOGE("[webfr::web::mime::multipart::AsyncInMemoryPartReader::onPartDataAsync()]", "Error. Part size exceeds specified maxDataSize=%d", m_maxDataSize);
      throw std::runtime_error("[webfr::web::mime::multipart::AsyncInMemoryPartReader::onPartDataAsync()]: Error. Part size exceeds specified maxDataSize");
    }
    buffer->writeSimple(data, size);
  } else {
    auto fullData = buffer->toString();
    buffer->clear();
    part->clearTag();
    auto stream = std::make_shared<data::stream::BufferInputStream>(fullData.getPtr(), fullData->getData(), fullData->getSize());
    part->setDataInfo(stream, fullData, fullData->getSize());
  }
  return nullptr;
}

std::shared_ptr<PartReader> createInMemoryPartReader(v_io_size maxDataSize) {
  return std::make_shared<InMemoryPartReader>(maxDataSize);
}


std::shared_ptr<AsyncPartReader> createAsyncInMemoryPartReader(v_io_size maxDataSize) {
  return std::make_shared<AsyncInMemoryPartReader>(maxDataSize);
}

}}}}