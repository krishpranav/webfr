#include "FileStreamProvider.hpp"
#include "webfr/core/data/stream/FileStream.hpp"

namespace webfr { namespace web { namespace mime { namespace multipart {

FileStreamProvider::FileStreamProvider(const webfr::String& filename)
  : m_filename(filename)
{}

std::shared_ptr<FileStreamProvider::OutputStream> FileStreamProvider::getOutputStream(const std::shared_ptr<Part>& part) {
  (void)part;
  return std::make_shared<data::stream::FileOutputStream>(m_filename->c_str());
}

std::shared_ptr<FileStreamProvider::InputStream> FileStreamProvider::getInputStream(const std::shared_ptr<Part>& part) {
  (void)part;
  return std::make_shared<data::stream::FileInputStream>(m_filename->c_str());
}


AsyncFileStreamProvider::AsyncFileStreamProvider(const webfr::String& filename)
  : m_filename(filename)
{}

async::CoroutineStarter AsyncFileStreamProvider::getOutputStreamAsync(const std::shared_ptr<Part>& part,
                                                                      std::shared_ptr<data::stream::OutputStream>& stream)
{
  (void)part;
  stream = std::make_shared<data::stream::FileOutputStream>(m_filename->c_str());
  return nullptr;
}


async::CoroutineStarter AsyncFileStreamProvider::getInputStreamAsync(const std::shared_ptr<Part>& part,
                                                                     std::shared_ptr<data::stream::InputStream>& stream)
{
  (void)part;
  stream = std::make_shared<data::stream::FileInputStream>(m_filename->c_str());
  return nullptr;
}

std::shared_ptr<PartReader> createFilePartReader(const webfr::String& filename, v_io_size maxDataSize) {
  auto provider = std::make_shared<FileStreamProvider>(filename);
  auto reader = std::make_shared<StreamPartReader>(provider, maxDataSize);
  return reader;
}

std::shared_ptr<AsyncPartReader> createAsyncFilePartReader(const webfr::String& filename, v_io_size maxDataSize) {
  auto provider = std::make_shared<AsyncFileStreamProvider>(filename);
  auto reader = std::make_shared<AsyncStreamPartReader>(provider, maxDataSize);
  return reader;
}

}}}}