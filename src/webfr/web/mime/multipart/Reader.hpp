#ifndef webfr_web_mime_multipart_Reader_hpp
#define webfr_web_mime_multipart_Reader_hpp

#include "Multipart.hpp"
#include "StatefulParser.hpp"

#include <unordered_map>

namespace webfr { namespace web { namespace mime { namespace multipart {

class PartReader {
public:

  virtual ~PartReader() = default;

  virtual void onNewPart(const std::shared_ptr<Part>& part) = 0;

  virtual void onPartData(const std::shared_ptr<Part>& part, p_char8 data, oatpp::v_io_size size) = 0;

};

class AsyncPartReader {
public:

  virtual ~AsyncPartReader() = default;

  virtual async::CoroutineStarter onNewPartAsync(const std::shared_ptr<Part>& part) = 0;

  virtual async::CoroutineStarter onPartDataAsync(const std::shared_ptr<Part>& part, p_char8 data, oatpp::v_io_size size) = 0;

};

class Reader; 

typedef std::unordered_map<oatpp::String, std::shared_ptr<PartReader>> PartReadersMap;

class PartsParser : public StatefulParser::Listener {
  friend Reader;
private:
  PartReadersMap m_readers;
  std::shared_ptr<PartReader> m_defaultReader;
  std::shared_ptr<PartReader> m_currReader;
  Multipart* m_multipart;
  std::shared_ptr<Part> m_currPart;
public:

  PartsParser(Multipart* multipart);

  PartsParser(Multipart* multipart, const PartReadersMap& readersMap);

  void onPartHeaders(const Headers& partHeaders) override;

  void onPartData(p_char8 data, v_buff_size size) override;

  void setPartReader(const oatpp::String& partName, const std::shared_ptr<PartReader>& reader);

  void setDefaultPartReader(const std::shared_ptr<PartReader>& reader);

};

class AsyncReader; 

typedef std::unordered_map<oatpp::String, std::shared_ptr<AsyncPartReader>> AsyncPartReadersMap;

class AsyncPartsParser : public StatefulParser::AsyncListener {
  friend AsyncReader;
private:
  async::CoroutineStarter onPartDone(const std::shared_ptr<Part>& part);
private:
  AsyncPartReadersMap m_readers;
  std::shared_ptr<AsyncPartReader> m_defaultReader;
  std::shared_ptr<AsyncPartReader> m_currReader;
  Multipart* m_multipart;
  std::shared_ptr<Part> m_currPart;
public:

  AsyncPartsParser(Multipart* multipart);

  AsyncPartsParser(Multipart* multipart, const AsyncPartReadersMap& readersMap);

  async::CoroutineStarter onPartHeadersAsync(const Headers& partHeaders) override;

  async::CoroutineStarter onPartDataAsync(p_char8 data, v_buff_size size) override;

  void setPartReader(const oatpp::String& partName, const std::shared_ptr<AsyncPartReader>& reader);

  void setDefaultPartReader(const std::shared_ptr<AsyncPartReader>& reader);

};

class Reader : public oatpp::data::stream::WriteCallback {
private:
  std::shared_ptr<PartsParser> m_partsParser;
  StatefulParser m_parser;
public:

  Reader(Multipart* multipart);

  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;

  void setPartReader(const oatpp::String& partName, const std::shared_ptr<PartReader>& reader);

  void setDefaultPartReader(const std::shared_ptr<PartReader>& reader);

};

class AsyncReader : public oatpp::data::stream::WriteCallback {
private:
  std::shared_ptr<AsyncPartsParser> m_partsParser;
  StatefulParser m_parser;
  std::shared_ptr<Multipart> m_multipart;
public:

  AsyncReader(const std::shared_ptr<Multipart>& multipart);

  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;

  void setPartReader(const oatpp::String& partName, const std::shared_ptr<AsyncPartReader>& reader);

  void setDefaultPartReader(const std::shared_ptr<AsyncPartReader>& reader);

};

}}}}

#endif 