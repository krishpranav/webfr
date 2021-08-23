#include "UnicodeTest.hpp"
#include "webfr/encoding/Hex.hpp"
#include "webfr/encoding/Unicode.hpp"

namespace webfr { namespace test { namespace encoding {
  
namespace {

void writeBinaryInt(v_int32 value){
  
  v_char8 buff [37];
  buff[36] = '\0';
  v_int32 index = 0;
  for(v_int32 i = 0; i < 36; i++){
    if((i + 1) % 9 == 0){
      buff[i] = ',';
    } else {
      v_int32 unit = 1 << index;
      if((unit & value) == 0){
        buff[i] = '0';
      } else {
        buff[i] = '1';
      }
      index++;
    }
  }
  
  WEBFR_LOGV("bin", "value='%s'", (const char*) &buff);
  
}
  
}
  
void UnicodeTest::onRun(){
  
  v_char8 buff[128];
  v_buff_size cnt;
  
  for(v_int32 c = 128; c < 2048; c ++){
    auto size = webfr::encoding::Unicode::decodeUtf8Char(c, buff);
    WEBFR_ASSERT(size == 2);
    auto code = webfr::encoding::Unicode::encodeUtf8Char(buff, cnt);
    WEBFR_ASSERT(cnt == 2);
    WEBFR_ASSERT(code == c);
  }
  
  for(v_int32 c = 2048; c < 65536; c ++){
    auto size = webfr::encoding::Unicode::decodeUtf8Char(c, buff);
    WEBFR_ASSERT(size == 3);
    auto code = webfr::encoding::Unicode::encodeUtf8Char(buff, cnt);
    WEBFR_ASSERT(cnt == 3);
    WEBFR_ASSERT(code == c);
  }

  
  for(v_int32 c = 65536; c < 2097152; c ++){
    auto size = webfr::encoding::Unicode::decodeUtf8Char(c, buff);
    WEBFR_ASSERT(size == 4);
    auto code = webfr::encoding::Unicode::encodeUtf8Char(buff, cnt);
    WEBFR_ASSERT(cnt == 4);
    WEBFR_ASSERT(code == c);
  }
  
  for(v_int32 c = 2097152; c < 67108864; c ++){
    auto size = webfr::encoding::Unicode::decodeUtf8Char(c, buff);
    WEBFR_ASSERT(size == 5);
    auto code = webfr::encoding::Unicode::encodeUtf8Char(buff, cnt);
    WEBFR_ASSERT(cnt == 5);
    WEBFR_ASSERT(code == c);
  }

  for (v_int64 c = 67108864; c < 2147483647; c = c + 100) {
    auto size = webfr::encoding::Unicode::decodeUtf8Char((v_int32) c, buff);
    WEBFR_ASSERT(size == 6);
    auto code = webfr::encoding::Unicode::encodeUtf8Char(buff, cnt);
    WEBFR_ASSERT(cnt == 6);
    WEBFR_ASSERT(code == c);
  }
  
  p_char8 sequence = (p_char8)"𐐷";
  auto code = webfr::encoding::Unicode::encodeUtf8Char(sequence, cnt);
  
  v_int16 high;
  v_int16 low;
  webfr::encoding::Unicode::codeToUtf16SurrogatePair(code, high, low);
  auto check = webfr::encoding::Unicode::utf16SurrogatePairToCode(high, low);
  writeBinaryInt(code);
  writeBinaryInt(check);
  WEBFR_ASSERT(code == check);
  
  for(v_int32 c = 0x010000; c <= 0x10FFFF; c++) {
    webfr::encoding::Unicode::codeToUtf16SurrogatePair(code, high, low);
    check = webfr::encoding::Unicode::utf16SurrogatePairToCode(high, low);
    WEBFR_ASSERT(code == check);
  }

}
  
}}}