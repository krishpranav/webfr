#ifndef webfr_web_mime_multipart_FileStreamProvider_hpp
#define webfr_web_mime_multipart_FileStreamProvider_hpp

#include "StreamPartReader.hpp"
#include "Reader.hpp"

namespace webfr { namespace web { namespace mime { namespace multipart {

class FileStreamProvider : public PartReaderStreamProvider {
public:

  typedef webfr::data::stream::OutputStream OutputStream;
  
  typedef webfr::data::stream::InputStream InputStream;
private:
  webfr::String m_filename;
public:

  FileStreamProvider(const webfr::String& filename);

  std::shared_ptr<OutputStream> getOutputStream(const std::shared_ptr<Part>& part) override;

  std::shared_ptr<InputStream> getInputStream(const std::shared_ptr<Part>& part) override;

};

class AsyncFileStreamProvider : public AsyncPartReaderStreamProvider {
public:

  typedef webfr::data::stream::OutputStream OutputStream;

  typedef webfr::data::stream::InputStream InputStream;
public:

private:
  webfr::String m_filename;
public:

  AsyncFileStreamProvider(const webfr::String& filename);

  async::CoroutineStarter getOutputStreamAsync(const std::shared_ptr<Part>& part,
                                               std::shared_ptr<data::stream::OutputStream>& stream) override;

  async::CoroutineStarter getInputStreamAsync(const std::shared_ptr<Part>& part,
                                              std::shared_ptr<data::stream::InputStream>& stream) override;

};

std::shared_ptr<PartReader> createFilePartReader(const webfr::String& filename, v_io_size maxDataSize = -1);

std::shared_ptr<AsyncPartReader> createAsyncFilePartReader(const webfr::String& filename, v_io_size maxDataSize = -1);

}}}}

#endif 