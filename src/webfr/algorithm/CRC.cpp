#include "CRC.hpp"

namespace webfr { namespace algorithm {
  
const p_uint32 CRC32::TABLE_04C11DB7 = generateTable(0x04C11DB7);
  
v_uint32 CRC32::bitReverse(v_uint32 poly) {
  v_uint32 result = 0;
  for(v_int32 i = 0; i < 32; i ++) {
    if((poly & (1 << i)) > 0) {
      result |= 1 << (31 - i);
    }
  }
  return result;
}
  
p_uint32 CRC32::generateTable(v_uint32 poly) {
  
  p_uint32 result = new v_uint32[256];
  v_uint32 polyReverse = bitReverse(poly);
  v_uint32 value;
  
  for(v_int32 i = 0; i < 256; i++) {
    value = i;
    for (v_int32 bit = 0; bit < 8; bit++) {
      if (value & 1) {
        value = (value >> 1) ^ polyReverse;
      } else {
        value = (value >> 1);
      }
    }
    
    result[i] = value;
    
  }
  
  return result;
  
}
  
v_uint32 CRC32::calc(const void *buffer, v_buff_size size, v_uint32 crc, v_uint32 initValue, v_uint32 xorOut, p_uint32 table) {
  
  p_uint8 data = (p_uint8) buffer;
  crc = crc ^ initValue;
  
  for(v_buff_size i = 0; i < size; i++) {
    crc = table[(crc & 0xFF) ^ data[i]] ^ (crc >> 8);
  }
  
  return crc ^ xorOut;
}
  
}}