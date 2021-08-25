#ifndef webfr_web_server_handler_ErrorHandler_hpp
#define webfr_web_server_handler_ErrorHandler_hpp

#include "webfr/web/protocol/http/outgoing/Response.hpp"
#include "webfr/web/protocol/http/Http.hpp"

namespace webfr { namespace web { namespace server { namespace handler {

class ErrorHandler {
public:

  typedef web::protocol::http::Headers Headers;
public:

  virtual
  std::shared_ptr<protocol::http::outgoing::Response>
  handleError(const protocol::http::Status& status, const webfr::String& message, const Headers& headers) = 0;

  std::shared_ptr<protocol::http::outgoing::Response> handleError(const protocol::http::Status& status, const webfr::String& message);
  
};

/**
 * Default Error Handler.
 */
class DefaultErrorHandler : public webfr::base::Countable, public ErrorHandler {
public:
  /**
   * Constructor.
   */
  DefaultErrorHandler()
  {}
public:

  /**
   * Create shared DefaultErrorHandler.
   * @return - `std::shared_ptr` to DefaultErrorHandler.
   */
  static std::shared_ptr<DefaultErrorHandler> createShared() {
    return std::make_shared<DefaultErrorHandler>();
  }

  /**
   * Implementation of &l:ErrorHandler::handleError ();
   * @param status - &id:webfr::web::protocol::http::Status;.
   * @param message - &id:webfr::String;.
   * @return - &id:webfr::web::protocol::http::outgoing::Response;.
   */
  std::shared_ptr<protocol::http::outgoing::Response>
  handleError(const protocol::http::Status& status, const webfr::String& message, const Headers& headers) override;

};
  
}}}}

#endif 