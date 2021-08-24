#ifndef webfr_parser_json_Utils_hpp
#define webfr_parser_json_Utils_hpp

#include "webfr/core/parser/Caret.hpp"
#include "webfr/core/Types.hpp"
#include <string>

namespace webfr { namespace parser { namespace json {

class Utils {
public:

  static constexpr v_int64 ERROR_CODE_INVALID_ESCAPED_CHAR = 1;

  static constexpr v_int64 ERROR_CODE_INVALID_SURROGATE_PAIR = 2;

  static constexpr v_int64 ERROR_CODE_PARSER_QUOTE_EXPECTED = 3;

public:
  typedef webfr::String String;
  typedef webfr::parser::Caret ParsingCaret;
private:
  static v_buff_size escapeUtf8Char(p_char8 sequence, p_char8 buffer);
  static v_buff_size calcEscapedStringSize(p_char8 data, v_buff_size size, v_buff_size& safeSize);
  static v_buff_size calcUnescapedStringSize(p_char8 data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition);
  static void unescapeStringToBuffer(p_char8 data, v_buff_size size, p_char8 resultData);
  static p_char8 preparseString(ParsingCaret& caret, v_buff_size& size);
public:

  static String escapeString(p_char8 data, v_buff_size size, bool copyAsOwnData = true);

  static String unescapeString(p_char8 data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition);

  static std::string unescapeStringToStdString(p_char8 data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition);

  static String parseString(ParsingCaret& caret);

  static std::string parseStringToStdString(ParsingCaret& caret);
  
};
  
}}}

#endif 