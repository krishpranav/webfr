#ifndef webfr_web_mime_multipart_InMemoryPartReader_hpp
#define webfr_web_mime_multipart_InMemoryPartReader_hpp

#include "Reader.hpp"

namespace webfr { namespace web { namespace mime { namespace multipart {

class InMemoryPartReader : public PartReader {
private:
  static const char* const TAG_NAME;
private:
  webfr::v_io_size m_maxDataSize;
public:

  InMemoryPartReader(v_io_size maxDataSize = 64 * 1024);

  void onNewPart(const std::shared_ptr<Part>& part) override;

  void onPartData(const std::shared_ptr<Part>& part, p_char8 data, webfr::v_io_size size) override;

};

class AsyncInMemoryPartReader : public AsyncPartReader {
private:
  static const char* const TAG_NAME;
private:
  webfr::v_io_size m_maxDataSize;
public:

  AsyncInMemoryPartReader(v_io_size maxDataSize = 64 * 1024);

  async::CoroutineStarter onNewPartAsync(const std::shared_ptr<Part>& part) override;

  async::CoroutineStarter onPartDataAsync(const std::shared_ptr<Part>& part, p_char8 data, webfr::v_io_size size) override;

};

std::shared_ptr<PartReader> createInMemoryPartReader(v_io_size maxDataSize = 64 * 1024);

std::shared_ptr<AsyncPartReader> createAsyncInMemoryPartReader(v_io_size maxDataSize = 64 * 1024);

}}}}

#endif 