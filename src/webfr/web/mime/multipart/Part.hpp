#ifndef webfr_web_mime_multipart_Part_hpp
#define webfr_web_mime_multipart_Part_hpp

#include "webfr/core/data/share/LazyStringMap.hpp"
#include "webfr/core/data/stream/Stream.hpp"

namespace webfr { namespace web { namespace mime { namespace multipart {

class Part {
public:

  typedef oatpp::data::share::LazyStringMultimap<oatpp::data::share::StringKeyLabelCI_FAST> Headers;
private:
  oatpp::String m_name;
  oatpp::String m_filename;
  Headers m_headers;
  std::shared_ptr<data::stream::InputStream> m_inputStream;
  oatpp::String m_inMemoryData;
  v_int64 m_knownSize;
private:
  const char* m_tagName;
  std::shared_ptr<oatpp::base::Countable> m_tagObject;
public:

  Part(const Headers& headers,
       const std::shared_ptr<data::stream::InputStream>& inputStream,
       const oatpp::String& inMemoryData,
       v_int64 knownSize);

  Part(const Headers& headers);

  Part();

  void setDataInfo(const std::shared_ptr<data::stream::InputStream>& inputStream,
                   const oatpp::String& inMemoryData,
                   v_int64 knownSize);

  void setDataInfo(const std::shared_ptr<data::stream::InputStream>& inputStream);

  oatpp::String getName() const;

  oatpp::String getFilename() const;

  const Headers& getHeaders() const;

  oatpp::String getHeader(const oatpp::data::share::StringKeyLabelCI_FAST& headerName) const;

  void putHeader(const oatpp::data::share::StringKeyLabelCI_FAST& key, const oatpp::data::share::StringKeyLabel& value);

  bool putHeaderIfNotExists(const oatpp::data::share::StringKeyLabelCI_FAST& key, const oatpp::data::share::StringKeyLabel& value);

  std::shared_ptr<data::stream::InputStream> getInputStream() const;

  oatpp::String getInMemoryData() const;

  v_int64 getKnownSize() const;

  void setTag(const char* tagName, const std::shared_ptr<oatpp::base::Countable>& tagObject);

  const char* getTagName();

  std::shared_ptr<oatpp::base::Countable> getTagObject();

  void clearTag();

};

}}}}

#endif 