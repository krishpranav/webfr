#include "Utils.hpp"
#include "webfr/encoding/Unicode.hpp"
#include "webfr/encoding/Hex.hpp"

namespace webfr { namespace parser { namespace json{

v_buff_size Utils::calcEscapedStringSize(p_char8 data, v_buff_size size, v_buff_size& safeSize) {
  v_buff_size result = 0;
  v_buff_size i = 0;
  safeSize = size;
  while (i < size) {
    v_char8 a = data[i];
    if(a < 32) {
      i ++;
      if(a == '\b' || a == '\f' || a == '\n' || a == '\r' || a == '\t'){
        result += 2; 
      } else {
        result += 6; 
      }
    } else if(a < 128){
      i ++;
      if(a == '\"' || a == '\\' || a == '/'){
        result += 2; 
      } else {
        result ++;
      }
    } else {
      v_buff_size charSize = webfr::encoding::Unicode::getUtf8CharSequenceLength(a);
      if(charSize != 0) {
        if(i + charSize > size) {
          safeSize = i;
        }
        i += charSize;
        if(charSize < 4) {
          result += 6; 
        } else if(charSize == 4) {
          result += 12; 
        } else {
          result += 11; 
        }
      } else {
        
        i ++;
        result ++;
      }
    }
  }
  return result;
}

v_buff_size Utils::calcUnescapedStringSize(p_char8 data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition) {
  errorCode = 0;
  v_buff_size result = 0;
  v_buff_size i = 0;
  
  while (i < size) {
    v_char8 a = data[i];
    if(a == '\\'){
      
      if(i + 1 == size){
        errorCode = ERROR_CODE_INVALID_ESCAPED_CHAR;
        errorPosition = i;
        return 0;
      }
      
      v_char8 b = data[i + 1];
      
      if(b == '"' || b == '\\' || b == '/' || b == 'b' || b == 'f' || b == 'n' || b == 'r' || b == 't'){
        result += 1;
        i += 2;
      } else if(b == 'u'){
        
        if(i + 6 > size){
          errorCode = ERROR_CODE_INVALID_ESCAPED_CHAR;
          errorPosition = i;
          return 0;
        }
        
        if(data[i + 2] == '+') { 
          if(i + 11 > size){
            errorCode = ERROR_CODE_INVALID_ESCAPED_CHAR;
            errorPosition = i;
            return 0;
          }
          v_uint32 code;
          errorCode = encoding::Hex::readUInt32(&data[i + 3], code);
          if(errorCode != 0){
            errorPosition = i + 3;
            return 0;
          }
          i += 11;
          result += encoding::Unicode::getUtf8CharSequenceLengthForCode(code);
        } else {
          v_uint16 code;
          errorCode = encoding::Hex::readUInt16(&data[i + 2], code);
          if(errorCode != 0){
            errorPosition = i + 2;
            return 0;
          }
          
          if(code >= 0xD800 && code <= 0xDBFF){
            if(i + 12 > size){
              errorCode = ERROR_CODE_INVALID_SURROGATE_PAIR;
              errorPosition = i;
              return 0;
            }
            v_uint16 low;
            errorCode = encoding::Hex::readUInt16(&data[i + 8], low);
            if(errorCode != 0){
              errorPosition = i + 8;
              return 0;
            }
            
            if(low >= 0xDC00 && low <= 0xDFFF){
              v_uint32 bigCode = encoding::Unicode::utf16SurrogatePairToCode(code, low);
              i += 12;
              result += encoding::Unicode::getUtf8CharSequenceLengthForCode(bigCode);
            } else {
              errorCode = ERROR_CODE_INVALID_SURROGATE_PAIR;
              errorPosition = i;
              return 0;
            }
            
          } else {
            i += 6;
            result += encoding::Unicode::getUtf8CharSequenceLengthForCode(code);
          }
        }
        
      } else {
        errorCode = ERROR_CODE_INVALID_ESCAPED_CHAR;
        errorPosition = i;
        return 0;
      }
      
    } else {
      i ++;
      result ++;
    }
    
  }
  
  return result;
}
  
v_buff_size Utils::escapeUtf8Char(p_char8 sequence, p_char8 buffer){
  v_buff_size length;
  v_int32 code = webfr::encoding::Unicode::encodeUtf8Char(sequence, length);
  if(code < 0x00010000) {
    buffer[0] = '\\';
    buffer[1] = 'u';
    webfr::encoding::Hex::writeUInt16(v_uint16(code), &buffer[2]);
    return 6;
  } else if(code < 0x00200000) {
    v_int16 high;
    v_int16 low;
    webfr::encoding::Unicode::codeToUtf16SurrogatePair(code, high, low);
    buffer[0] = '\\';
    buffer[1] = 'u';
    webfr::encoding::Hex::writeUInt16(high, &buffer[2]);
    buffer[6] = '\\';
    buffer[7] = 'u';
    webfr::encoding::Hex::writeUInt16(low, &buffer[8]);
    return 12;
  } else {
    buffer[0] = '\\';
    buffer[1] = 'u';
    buffer[2] = '+';
    webfr::encoding::Hex::writeUInt32(code, &buffer[2]);
    return 11;
  }
}
  
webfr::String Utils::escapeString(p_char8 data, v_buff_size size, bool copyAsOwnData) {
  v_buff_size safeSize;
  v_buff_size escapedSize = calcEscapedStringSize(data, size, safeSize);
  if(escapedSize == size) {
    return String((const char*)data, size, copyAsOwnData);
  }
  auto result = String(escapedSize);
  p_char8 resultData = result->getData();
  v_buff_size pos = 0;

  {
    v_buff_size i = 0;
    while (i < safeSize) {
      v_char8 a = data[i];
      if (a < 32) {
        if (a == '\b') {
          resultData[pos] = '\\'; resultData[pos + 1] = 'b'; pos += 2;
        }
        else if (a == '\f') {
          resultData[pos] = '\\'; resultData[pos + 1] = 'f'; pos += 2;
        }
        else if (a == '\n') {
          resultData[pos] = '\\'; resultData[pos + 1] = 'n'; pos += 2;
        }
        else if (a == '\r') {
          resultData[pos] = '\\'; resultData[pos + 1] = 'r'; pos += 2;
        }
        else if (a == '\t') {
          resultData[pos] = '\\'; resultData[pos + 1] = 't'; pos += 2;
        }
        else {
          resultData[pos] = '\\';
          resultData[pos + 1] = 'u';
          webfr::encoding::Hex::writeUInt16(a, &resultData[pos + 2]);
          pos += 6;
        }
        i++;
      }
      else if (a < 128) {
        if (a == '\"') {
          resultData[pos] = '\\'; resultData[pos + 1] = '"'; pos += 2;
        }
        else if (a == '\\') {
          resultData[pos] = '\\'; resultData[pos + 1] = '\\'; pos += 2;
        }
        else if (a == '/') {
          resultData[pos] = '\\'; resultData[pos + 1] = '/'; pos += 2;
        }
        else {
          resultData[pos] = data[i];
          pos++;
        }
        i++;
      }
      else {
        v_buff_size charSize = webfr::encoding::Unicode::getUtf8CharSequenceLength(a);
        if (charSize != 0) {
          pos += escapeUtf8Char(&data[i], &resultData[pos]);
          i += charSize;
        }
        else {
          
          resultData[pos] = data[i];
          i++;
          pos++;
        }
      }
    }
  }
  
  if(size > safeSize){
    for(v_buff_size i = pos; i < result->getSize(); i ++){
      resultData[i] = '?';
    }
  }
  
  return result;
}

void Utils::unescapeStringToBuffer(p_char8 data, v_buff_size size, p_char8 resultData){
  
  v_buff_size i = 0;
  v_buff_size pos = 0;
  
  while (i < size) {
    v_char8 a = data[i];
    
    if(a == '\\'){
      v_char8 b = data[i + 1];
      if(b != 'u'){
        switch (b) {
          case '"': resultData[pos] = '"'; pos ++; break;
          case '\\': resultData[pos] = '\\'; pos ++; break;
          case '/': resultData[pos] = '/'; pos ++; break;
          case 'b': resultData[pos] = '\b'; pos ++; break;
          case 'f': resultData[pos] = '\f'; pos ++; break;
          case 'n': resultData[pos] = '\n'; pos ++; break;
          case 'r': resultData[pos] = '\r'; pos ++; break;
          case 't': resultData[pos] = '\t'; pos ++; break;
        }
        i += 2;
      } else {
        if(data[i + 2] == '+'){ 
          v_uint32 code;
          encoding::Hex::readUInt32(&data[i + 3], code);
          i += 11;
          pos += encoding::Unicode::decodeUtf8Char(code, &resultData[pos]);
        } else {
          
          v_uint16 code;
          encoding::Hex::readUInt16(&data[i + 2], code);
          
          if(code >= 0xD800 && code <= 0xDBFF){
            v_uint16 low;
            encoding::Hex::readUInt16(&data[i + 8], low);
            v_uint32 bigCode = encoding::Unicode::utf16SurrogatePairToCode(code, low);
            pos += encoding::Unicode::decodeUtf8Char(bigCode, &resultData[pos]);
            i += 12;
          } else {
            pos += encoding::Unicode::decodeUtf8Char(code, &resultData[pos]);
            i += 6;
          }
          
        }
      }
    } else {
      resultData[pos] = a;
      pos ++;
      i++;
    }
    
  }
  
}
  
webfr::String Utils::unescapeString(p_char8 data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition) {
  
  v_buff_size unescapedSize = calcUnescapedStringSize(data, size, errorCode, errorPosition);
  if(errorCode != 0){
    return nullptr;
  }
  auto result = String(unescapedSize);
  if(unescapedSize == size) {
    std::memcpy(result->getData(), data, size);
  } else {
    unescapeStringToBuffer(data, size, result->getData());
  }
  return result;
  
}
  
std::string Utils::unescapeStringToStdString(p_char8 data, v_buff_size size, v_int64& errorCode, v_buff_size& errorPosition){
  
  v_buff_size unescapedSize = calcUnescapedStringSize(data, size, errorCode, errorPosition);
  if(errorCode != 0){
    return "";
  }
  std::string result;
  result.resize(unescapedSize);
  if(unescapedSize == size) {
    std::memcpy((p_char8) result.data(), data, size);
  } else {
    unescapeStringToBuffer(data, size, (p_char8) result.data());
  }
  return result;
  
}
  
p_char8 Utils::preparseString(ParsingCaret& caret, v_buff_size& size){
  
  if(caret.canContinueAtChar('"', 1)){
    
    const p_char8 data = caret.getData();
    v_buff_size pos = caret.getPosition();
    v_buff_size pos0 = pos;
    v_buff_size length = caret.getDataSize();
    
    while (pos < length) {
      v_char8 a = data[pos];
      if(a == '"'){
        size = pos - pos0;
        return &data[pos0];
      } else if(a == '\\') {
        pos += 2;
      } else {
        pos ++;
      }
    }
    caret.setPosition(caret.getDataSize());
    caret.setError("[webfr::parser::json::Utils::preparseString()]: Error. '\"' - expected", ERROR_CODE_PARSER_QUOTE_EXPECTED);
  } else {
    caret.setError("[webfr::parser::json::Utils::preparseString()]: Error. '\"' - expected", ERROR_CODE_PARSER_QUOTE_EXPECTED);
  }
  
  return nullptr;
  
}
  
webfr::String Utils::parseString(ParsingCaret& caret) {
  
  v_buff_size size;
  p_char8 data = preparseString(caret, size);
  
  if(data != nullptr) {
  
    v_buff_size pos = caret.getPosition();
    
    v_int64 errorCode;
    v_buff_size errorPosition;
    auto result = unescapeString(data, size, errorCode, errorPosition);
    if(errorCode != 0){
      caret.setError("[webfr::parser::json::Utils::parseString()]: Error. Call to unescapeString() failed", errorCode);
      caret.setPosition(pos + errorPosition);
    } else {
      caret.setPosition(pos + size + 1);
    }
    
    return result;
    
  }
  
  return nullptr;
  
}
  
std::string Utils::parseStringToStdString(ParsingCaret& caret){
  
  v_buff_size size;
  p_char8 data = preparseString(caret, size);
  
  if(data != nullptr) {
    
    v_buff_size pos = caret.getPosition();
    
    v_int64 errorCode;
    v_buff_size errorPosition;
    const std::string& result = unescapeStringToStdString(data, size, errorCode, errorPosition);
    if(errorCode != 0){
      caret.setError("[webfr::parser::json::Utils::parseStringToStdString()]: Error. Call to unescapeStringToStdString() failed", errorCode);
      caret.setPosition(pos + errorPosition);
    } else {
      caret.setPosition(pos + size + 1);
    }
    
    return result;
    
  }
  
  return "";
  
}
    
  
}}}