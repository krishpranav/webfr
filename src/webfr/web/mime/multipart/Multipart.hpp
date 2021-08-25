#ifndef webfr_web_mime_multipart_Multipart_hpp
#define webfr_web_mime_multipart_Multipart_hpp

#include "Part.hpp"
#include <list>

namespace webfr { namespace web { namespace mime { namespace multipart {

typedef webfr::data::share::LazyStringMultimap<webfr::data::share::StringKeyLabelCI_FAST> Headers;

class Multipart {
private:
  webfr::String m_boundary;
public:

  Multipart(const webfr::String& boundary);

  virtual ~Multipart() = default;

  webfr::String getBoundary();

  virtual std::shared_ptr<Part> readNextPart(async::Action& action) = 0;

  virtual void writeNextPart(const std::shared_ptr<Part>& part, async::Action& action) = 0;

  std::shared_ptr<Part> readNextPartSimple();

  void writeNextPartSimple(const std::shared_ptr<Part>& part);

public:

  static webfr::String generateRandomBoundary(v_int32 boundarySize = 15);

  static webfr::String parseBoundaryFromHeaders(const Headers& requestHeaders);

};



}}}}


#endif 