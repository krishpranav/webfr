#ifndef webfr_algorithm_CRC_hpp
#define webfr_algorithm_CRC_hpp

#include "webfr/core/base/Environment.hpp"
#include "webfr/encoding/Hex.hpp"

namespace webfr { namespace algorithm {

class CRC32 {
public:

  static const p_uint32 TABLE_04C11DB7;
public:

  static v_uint32 bitReverse(v_uint32 poly);
  
  static p_uint32 generateTable(v_uint32 poly);

  /**
   * @param buffer
   * @param size
   * @param crc
   * @param initValue
   * @param xorOut
   * @param table
   */
  static v_uint32 calc(const void *buffer, v_buff_size size, v_uint32 crc = 0, v_uint32 initValue = 0xFFFFFFFF, v_uint32 xorOut = 0xFFFFFFFF, p_uint32 table = TABLE_04C11DB7);
  
};
    
}}

#endif