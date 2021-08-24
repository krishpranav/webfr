#include "CommunicationError.hpp"

namespace webfr { namespace web { namespace protocol {
  
CommunicationError::CommunicationError(webfr::v_io_size ioStatus, const webfr::String& message)
  :std::runtime_error(message->std_str())
  , m_ioStatus(ioStatus)
  , m_message(message)
{}
  
webfr::v_io_size CommunicationError::getIOStatus() {
  return m_ioStatus;
}

webfr::String& CommunicationError::getMessage(){
  return m_message;
}
  
}}}