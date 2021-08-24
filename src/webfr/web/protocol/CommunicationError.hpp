#ifndef webfr_web_protocol_CommunicationError_hpp
#define webfr_web_protocol_CommunicationError_hpp

#include "webfr/core/IODefinitions.hpp"

namespace webfr { namespace web { namespace protocol {

class CommunicationError : public std::runtime_error {
private:
  oatpp::v_io_size m_ioStatus;
  oatpp::String m_message;
public:

  CommunicationError(oatpp::v_io_size ioStatus, const oatpp::String& message);


  oatpp::v_io_size getIOStatus();

  oatpp::String& getMessage();
  
};

template<class Status>
struct ProtocolErrorInfo {

  ProtocolErrorInfo()
    : ioStatus(0)
  {}

  ProtocolErrorInfo(oatpp::v_io_size pIOStatus, const Status& pStatus)
    : ioStatus(pIOStatus)
    , status(pStatus)
  {}

  oatpp::v_io_size ioStatus;

  Status status;

};

template<class Status>
class ProtocolError : public CommunicationError {
public:
  
  typedef ProtocolErrorInfo<Status> Info;
private:
  Info m_info;
public:

  ProtocolError(const Info& info, const oatpp::String& message)
    : CommunicationError(info.ioStatus, message)
    , m_info(info)
  {}

  Info getInfo() {
    return m_info;
  }
  
};


/**
 * Protocol Error template.
 * @tparam Status - arbitrary data type.
 */
template<class Status>
class AsyncProtocolError : public oatpp::AsyncIOError {
public:
  /**
   * Cenvenience typedef for ProtocolErrorInfo
   */
  typedef ProtocolErrorInfo<Status> Info;
private:
  Info m_info;
  oatpp::String m_message;
public:

  /**
   * Constructor.
   * @param info - &l:ProtocolError::Info ;.
   * @param message - error message.
   */
  AsyncProtocolError(const Info& info, const oatpp::String& message)
    : oatpp::AsyncIOError("AsyncProtocolError", info.ioStatus)
    , m_info(info)
    , m_message(message)
  {}

  /**
   * Error message.
   * @return - error message.
   */
  oatpp::String getMessage() {
    return m_message;
  }

  /**
   * Get error info.
   * @return - error info.
   */
  Info getInfo() {
    return m_info;
  }

};
  
}}}

#endif /* oatpp_web_protocol_CommunicationError_hpp */