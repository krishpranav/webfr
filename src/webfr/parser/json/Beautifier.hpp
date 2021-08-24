#ifndef webfr_parser_json_Beautifier_hpp
#define webfr_parser_json_Beautifier_hpp

#include "webfr/core/data/stream/Stream.hpp"

namespace webfr { namespace parser { namespace json {

class Beautifier : public webfr::data::stream::ConsistentOutputStream {
public:
  typedef webfr::data::stream::ConsistentOutputStream ConsistentOutputStream;
  typedef webfr::data::stream::IOMode IOMode;
  typedef webfr::data::stream::Context Context;
private:
  ConsistentOutputStream* m_outputStream;
  webfr::String m_indent;
  webfr::String m_newLine;
private:
  v_int32 m_level;
  bool m_wantIndent;
  bool m_isCharEscaped;
  bool m_isInString;
private:
  void writeIndent(ConsistentOutputStream* outputStream);
public:
  
  Beautifier(ConsistentOutputStream* outputStream, const webfr::String& indent, const webfr::String& newLine);

  v_io_size write(const void *data, v_buff_size count, async::Action& action) override;

  void setOutputStreamIOMode(IOMode ioMode) override;

  IOMode getOutputStreamIOMode() override;

  Context& getOutputStreamContext() override;

};

}}}

#endif 