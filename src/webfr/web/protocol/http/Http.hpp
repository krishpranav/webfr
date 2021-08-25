#ifndef webfr_web_protocol_http_Http_hpp
#define webfr_web_protocol_http_Http_hpp

#include "webfr/network/tcp/Connection.hpp"

#include "webfr/web/protocol/CommunicationError.hpp"

#include "webfr/core/parser/Caret.hpp"
#include "webfr/core/data/share/LazyStringMap.hpp"
#include "webfr/core/collection/ListMap.hpp"
#include "webfr/core/Types.hpp"

#include <unordered_map>
#include <unordered_set>

namespace webfr { namespace web { namespace protocol { namespace http {

typedef webfr::data::share::LazyStringMultimap<webfr::data::share::StringKeyLabelCI_FAST> Headers;

typedef webfr::data::share::LazyStringMap<webfr::data::share::StringKeyLabel> QueryParams;

class Status{
public:

  static const Status CODE_100;

  static const Status CODE_101;

  static const Status CODE_102;

  static const Status CODE_200;// OK

  static const Status CODE_201;// Created

  static const Status CODE_202;// Accepted

  static const Status CODE_203;// Non-Authoritative Information

  static const Status CODE_204;// No Content

  static const Status CODE_205;// Reset Content

  static const Status CODE_206;// Partial Content

  static const Status CODE_207;// Multi-Status

  static const Status CODE_226;// IM Used

  static const Status CODE_300;// Multiple Choices

  static const Status CODE_301;// Moved Permanently

  static const Status CODE_302;// Moved Temporarily

  static const Status CODE_303;// See Other

  static const Status CODE_304;// Not Modified

  static const Status CODE_305;// Use Proxy

  static const Status CODE_306;// Reserved

  static const Status CODE_307;// Temporary Redirect

  static const Status CODE_400;// Bad Request

  static const Status CODE_401;// Unauthorized

  static const Status CODE_402;// Payment Required

  static const Status CODE_403;// Forbidden

  static const Status CODE_404;// Not Found

  /**
   * Method Not Allowed.
   */
  static const Status CODE_405;// Method Not Allowed

  /**
   * Not Acceptable.
   */
  static const Status CODE_406;// Not Acceptable

  /**
   * Proxy Authentication Required.
   */
  static const Status CODE_407;// Proxy Authentication Required

  /**
   * Request Timeout.
   */
  static const Status CODE_408;// Request Timeout

  /**
   * Conflict.
   */
  static const Status CODE_409;// Conflict

  /**
   * Gone
   */
  static const Status CODE_410;// Gone

  /**
   * Length Required.
   */
  static const Status CODE_411;// Length Required

  /**
   * Precondition Failed.
   */
  static const Status CODE_412;// Precondition Failed

  /**
   * Request Entity Too Large.
   */
  static const Status CODE_413;// Request Entity Too Large

  /**
   * Request-URI Too Large.
   */
  static const Status CODE_414;// Request-URI Too Large

  /**
   * Unsupported Media Type.
   */
  static const Status CODE_415;// Unsupported Media Type

  /**
   * Requested Range Not Satisfiable.
   */
  static const Status CODE_416;// Requested Range Not Satisfiable

  /**
   * Expectation Failed.
   */
  static const Status CODE_417;// Expectation Failed

  /**
   * Unprocessable Entity.
   */
  static const Status CODE_422;// Unprocessable Entity

  /**
   * Locked.
   */
  static const Status CODE_423;// Locked

  /**
   * Failed Dependency.
   */
  static const Status CODE_424;// Failed Dependency

  /**
   * Unordered Collection.
   */
  static const Status CODE_425;// Unordered Collection

  /**
   * Upgrade Required.
   */
  static const Status CODE_426;// Upgrade Required

  /**
   * Precondition Required.
   */
  static const Status CODE_428;// Precondition Required

  /**
   * Too Many Requests.
   */
  static const Status CODE_429;// Too Many Requests

  /**
   * Request Header Fields Too Large.
   */
  static const Status CODE_431;// Request Header Fields Too Large

  /**
   * Requested host unavailable.
   */
  static const Status CODE_434;// Requested host unavailable

  /**
   * Close connection withot sending headers.
   */
  static const Status CODE_444;// Close connection withot sending headers

  /**
   * Retry With.
   */
  static const Status CODE_449;// Retry With

  /**
   * Unavailable For Legal Reasons.
   */
  static const Status CODE_451;// Unavailable For Legal Reasons

  /**
   * Internal Server Error.
   */
  static const Status CODE_500;// Internal Server Error

  /**
   * Not Implemented.
   */
  static const Status CODE_501;// Not Implemented

  /**
   * Bad Gateway.
   */
  static const Status CODE_502;// Bad Gateway

  /**
   * Service Unavailable.
   */
  static const Status CODE_503;// Service Unavailable

  /**
   * Gateway Timeout.
   */
  static const Status CODE_504;// Gateway Timeout

  /**
   * HTTP Version Not Supported.
   */
  static const Status CODE_505;// HTTP Version Not Supported

  /**
   * Variant Also Negotiates.
   */
  static const Status CODE_506;// Variant Also Negotiates

  /**
   * Insufficient Storage.
   */
  static const Status CODE_507;// Insufficient Storage

  /**
   * Loop Detected.
   */
  static const Status CODE_508;// Loop Detected

  /**
   * Bandwidth Limit Exceeded.
   */
  static const Status CODE_509;// Bandwidth Limit Exceeded

  /**
   * Not Extended.
   */
  static const Status CODE_510;// Not Extended

  /**
   * Network Authentication Required.
   */
  static const Status CODE_511;// Network Authentication Required

  Status()
    : code(0)
    , description(nullptr)
  {}

  Status(v_int32 pCode, const char* pDesc)
    : code(pCode)
    , description(pDesc)
  {}

  v_int32 code;

  const char* description;
  
  bool operator==(const Status& other) const {
    return this->code == other.code;
  }
  
  bool operator!=(const Status& other) const {
    return this->code != other.code;
  }
  
};

class HttpError : public protocol::ProtocolError<Status> {
private:
  Headers m_headers;
public:

  HttpError(const Info& info, const webfr::String& message)
    : protocol::ProtocolError<Status>(info, message)
  {}

  HttpError(const Status& status, const webfr::String& message)
    : protocol::ProtocolError<Status>(Info(0, status), message)
  {}

  /**
   * Constructor.
   * @param status
   * @param message
   * @param headers
   */
  HttpError(const Status& status, const webfr::String& message, const Headers& headers)
    : protocol::ProtocolError<Status>(Info(0, status), message)
    , m_headers(headers)
  {}

  /**
   * Get headers
   * @return
   */
  const Headers& getHeaders() const {
    return m_headers;
  }
  
};

#define WEBFR_ASSERT_HTTP(COND, STATUS, MESSAGE) \
if(!(COND)) { throw webfr::web::protocol::http::HttpError(STATUS, MESSAGE); }

/**
 * Collection of HTTP Header constants.
 */
class Header {
public:

  /**
   * Possible values for headers.
   */
  class Value {
  public:
    static const char* const CONNECTION_CLOSE;
    static const char* const CONNECTION_KEEP_ALIVE;
    static const char* const CONNECTION_UPGRADE;
    
    static const char* const SERVER;
    static const char* const USER_AGENT;
    
    static const char* const TRANSFER_ENCODING_CHUNKED;
    static const char* const CONTENT_TYPE_APPLICATION_JSON;
  };
public:
  static const char* const ACCEPT;              // "Accept"
  static const char* const AUTHORIZATION;       // "Authorization"
  static const char* const WWW_AUTHENTICATE;    // "WWW-Authenticate"
  static const char* const CONNECTION;          // "Connection"
  static const char* const TRANSFER_ENCODING;   // "Transfer-Encoding"
  static const char* const CONTENT_ENCODING;    // "Content-Encoding"
  static const char* const CONTENT_LENGTH;      // "Content-Length"
  static const char* const CONTENT_TYPE;        // "Content-Type"
  static const char* const CONTENT_RANGE;       // "Content-Range"
  static const char* const RANGE;               // "Range"
  static const char* const HOST;                // "Host"
  static const char* const USER_AGENT;          // "User-Agent"
  static const char* const SERVER;              // "Server"
  static const char* const UPGRADE;             // "Upgrade"
  static const char* const CORS_ORIGIN;         // Access-Control-Allow-Origin
  static const char* const CORS_METHODS;        // Access-Control-Allow-Methods
  static const char* const CORS_HEADERS;        // Access-Control-Allow-Headers
  static const char* const CORS_MAX_AGE;        // Access-Control-Max-Age
  static const char* const ACCEPT_ENCODING;     // Accept-Encoding
};
  
class Range {
public:
  static const char* const UNIT_BYTES;
private:
  Range()
    : units(nullptr)
  {}
public:
  
  Range(const webfr::String& pUnits,
        v_int64 pStart,
        v_int64 pEnd)
    : units(pUnits)
    , start(pStart)
    , end(pEnd)
  {}
  
  webfr::String units;
  v_int64 start;
  v_int64 end;
  
  webfr::String toString() const;
  
  bool isValid() const {
    return units.get() != nullptr;
  }
  
  static Range parse(webfr::parser::Caret& caret);
  static Range parse(const webfr::String& str);
  
};
  
class ContentRange {
public:
  static const char* const UNIT_BYTES;
private:
  ContentRange()
    : units(nullptr)
  {}
public:
  
  ContentRange(const webfr::String& pUnits,
               v_int64 pStart,
               v_int64 pEnd,
               v_int64 pSize,
               bool pIsSizeKnown)
    : units(pUnits)
    , start(pStart)
    , end(pEnd)
    , size(pSize)
    , isSizeKnown(pIsSizeKnown)
  {}
  
  webfr::String units;
  v_int64 start;
  v_int64 end;
  v_int64 size;
  bool isSizeKnown;
  
  webfr::String toString() const;
  
  bool isValid() const {
    return units.get() != nullptr;
  }
  
  static ContentRange parse(webfr::parser::Caret& caret);
  static ContentRange parse(const webfr::String& str);
  
};

/**
 * Struct representing HTTP request starting line.
 * Example request starting line: `GET /path/to/resource/ HTTP/1.1`.
 */
struct RequestStartingLine {
  /**
   * Method as &id:webfr::data::share::StringKeyLabel;.
   */
  webfr::data::share::StringKeyLabel method; // GET, POST ...

  /**
   * Path as &id:webfr::data::share::StringKeyLabel;.
   */
  webfr::data::share::StringKeyLabel path;

  /**
   * Protocol as &id:webfr::data::share::StringKeyLabel;.
   */
  webfr::data::share::StringKeyLabel protocol;
};

/**
 * Struct representing HTTP response starting line.
 * Example response starting line: `HTTP/1.1 200 OK`.
 */
struct ResponseStartingLine {
  /**
   * Protocol as &id:webfr::data::share::StringKeyLabel;.
   */
  webfr::data::share::StringKeyLabel protocol;

  /**
   * Status code as v_int32.
   */
  v_int32 statusCode;

  /**
   * Description as &id:webfr::data::share::StringKeyLabel;.
   */
  webfr::data::share::StringKeyLabel description;
};

/**
 * Data contained in the value of one header.
 */
struct HeaderValueData {

  /**
   * value tokens.
   */
  std::unordered_set<data::share::StringKeyLabelCI> tokens;

  /**
   * Title params.
   */
  std::unordered_map<data::share::StringKeyLabelCI, data::share::StringKeyLabel> titleParams;

  /**
   * Get title parm value by key.
   * @param key
   * @return
   */
  webfr::String getTitleParamValue(const data::share::StringKeyLabelCI& key) const;

};

class Parser {
private:
  static webfr::data::share::StringKeyLabelCI_FAST parseHeaderNameLabel(const std::shared_ptr<webfr::base::StrBuffer>& headersText,
                                                                        webfr::parser::Caret& caret);
public:

  /**
   * Parse &l:RequestStartingLine;.
   * @param line - &l:RequestStartingLine;. Values will be set to line's fields.
   * @param headersText - `std::shared_ptr` to &id:webfr::base::StrBuffer; needed as a "memory handle" for
   * &l:RequestStartingLine; fields. See &id:webfr::data::share::MemoryLabel;.
   * @param caret - &id:webfr::parser::Caret;.
   * @param error - out parameter &l:Status;.
   */
  static void parseRequestStartingLine(RequestStartingLine& line,
                                       const std::shared_ptr<webfr::base::StrBuffer>& headersText,
                                       webfr::parser::Caret& caret,
                                       Status& error);

  /**
   * Parse &l:ResponseStartingLine;.
   * @param line - &l:ResponseStartingLine;. Values will be set to line's fields.
   * @param headersText - `std::shared_ptr` to &id:webfr::base::StrBuffer; needed as a "memory handle" for
   * &l:ResponseStartingLine; fields. See &id:webfr::data::share::MemoryLabel;.
   * @param caret - &id:webfr::parser::Caret;.
   * @param error - out parameter &l:Status;.
   */
  static void parseResponseStartingLine(ResponseStartingLine& line,
                                        const std::shared_ptr<webfr::base::StrBuffer>& headersText,
                                        webfr::parser::Caret& caret,
                                        Status& error);

  /**
   * Parse one header line. Example of the header line:
   * `"Content-Type: application/json\r\n"`.
   * @param headers - &l:Headers; map to put parsed header to.
   * @param headersText - `std::shared_ptr` to &id:webfr::base::StrBuffer; needed as a "memory handle" for
   * &l:Headers; values. See &id:webfr::data::share::MemoryLabel;.
   * @param caret - &id:webfr::parser::Caret;.
   * @param error - out parameter &l:Status;.
   */
  static void parseOneHeader(Headers& headers,
                             const std::shared_ptr<webfr::base::StrBuffer>& headersText,
                             webfr::parser::Caret& caret,
                             Status& error);

  /**
   * Parse HTTP headers to &l:Headers; map.
   * @param headers - &l:Headers; map to put parsed headers to.
   * @param headersText - `std::shared_ptr` to &id:webfr::base::StrBuffer; needed as a "memory handle" for
   * &l:Headers; values. See &id:webfr::data::share::MemoryLabel;.
   * @param caret - &id:webfr::parser::Caret;.
   * @param error - out parameter &l:Status;.
   */
  static void parseHeaders(Headers& headers,
                           const std::shared_ptr<webfr::base::StrBuffer>& headersText,
                           webfr::parser::Caret& caret,
                           Status& error);

  /**
   * Parse data that is contained in a one header.
   * @param data - out. parsed data.
   * @param headerValue - header value string.
   * @param separator - subvalues separator.
   */
  static void parseHeaderValueData(HeaderValueData& data, const webfr::data::share::StringKeyLabel& headerValue, v_char8 separator);

};

/**
 * Http utils.
 */
class Utils {
public:

  /**
   * Write headers map to stream.
   * @param headers
   * @param stream
   */
  static void writeHeaders(const Headers& headers, data::stream::ConsistentOutputStream* stream);

};
  
}}}}

namespace std {
  
  template<>
  struct hash<webfr::web::protocol::http::Status> {
    
    typedef webfr::web::protocol::http::Status argument_type;
    typedef v_uint64 result_type;
    
    result_type operator()(webfr::web::protocol::http::Status const& s) const noexcept {
      return s.code;
    }
    
  };
}

#endif 