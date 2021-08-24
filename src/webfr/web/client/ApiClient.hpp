#ifndef webfr_web_client_Client_hpp
#define webfr_web_client_Client_hpp

#include "./RequestExecutor.hpp"
#include "webfr/web/protocol/http/incoming/Response.hpp"
#include "webfr/web/protocol/http/outgoing/BufferBody.hpp"
#include "webfr/encoding/Base64.hpp"
#include "webfr/core/data/share/StringTemplate.hpp"
#include "webfr/core/data/mapping/ObjectMapper.hpp"
#include "webfr/core/Types.hpp"
#include "webfr/core/utils/ConversionUtils.hpp"
#include <string>
#include <list>
#include <unordered_map>

namespace webfr { namespace web { namespace client {

class ApiClient : public webfr::base::Countable {
public:
  static constexpr const char* const TAG = "Client";
public:
  /**
   * Convenience typedef for &id:webfr::web::protocol::http::Status;.
   */
  typedef webfr::web::protocol::http::Status Status;

  /**
   * Convenience typedef for &id:webfr::web::protocol::http::Header;.
   */
  typedef webfr::web::protocol::http::Header Header;
public:

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::String;.
   */
  typedef webfr::String String;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Int8;.
   */
  typedef webfr::Int8 Int8;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::UInt8;.
   */
  typedef webfr::UInt8 UInt8;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Int16;.
   */
  typedef webfr::Int16 Int16;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::UInt16;.
   */
  typedef webfr::UInt16 UInt16;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Int32;.
   */
  typedef webfr::Int32 Int32;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::UInt32;.
   */
  typedef webfr::UInt32 UInt32;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Int64;.
   */
  typedef webfr::Int64 Int64;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::UInt64;.
   */
  typedef webfr::UInt64 UInt64;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Float32;.
   */
  typedef webfr::Float32 Float32;

  /**
   * Convenience typedef for &id:atpp::data::mapping::type::Float64;.
   */
  typedef webfr::Float64 Float64;

  /**
   * Convenience typedef for &id:webfr::data::mapping::type::Boolean;.
   */
  typedef webfr::Boolean Boolean;

  template <class T>
  using Enum = webfr::data::mapping::type::Enum<T>;

  template <class T>
  using Object = webfr::Object<T>;
public:

  typedef webfr::data::share::StringTemplate StringTemplate;

  typedef webfr::web::protocol::http::Headers Headers;

  typedef webfr::web::protocol::http::incoming::Response Response;
public:

  typedef RequestExecutor::AsyncCallback AsyncCallback;
protected:

  struct PathTemplateExtra {
    webfr::String name;
    bool hasQueryParams;
  };

protected:

  StringTemplate parsePathTemplate(const webfr::String& name, const webfr::String& text);

  webfr::String formatPath(const StringTemplate& pathTemplate,
                           const std::unordered_map<webfr::String, webfr::String>& pathParams,
                           const std::unordered_map<webfr::String, webfr::String>& queryParams);

protected:
  std::shared_ptr<RequestExecutor> m_requestExecutor;
  std::shared_ptr<webfr::data::mapping::ObjectMapper> m_objectMapper;
  
public:
  ApiClient(const std::shared_ptr<RequestExecutor>& requestExecutor,
            const std::shared_ptr<webfr::data::mapping::ObjectMapper>& objectMapper)
    : m_requestExecutor(requestExecutor)
    , m_objectMapper(objectMapper)
  {}
public:
  
  static std::shared_ptr<ApiClient> createShared(const std::shared_ptr<RequestExecutor>& requestExecutor,
                                                 const std::shared_ptr<data::mapping::ObjectMapper>& objectMapper) {
    return std::make_shared<ApiClient>(requestExecutor, objectMapper);
  }
  
public:

  /**
   * Call &id:webfr::web::client::RequestExecutor::getConnection;.
   * @return - &id:webfr::web::client::RequestExecutor::ConnectionHandle;.
   */
  virtual std::shared_ptr<RequestExecutor::ConnectionHandle> getConnection();

  /**
   * Call &id:webfr::web::client::RequestExecutor::getConnectionAsync;.
   * @return - &id:webfr::async::CoroutineStarterForResult;.
   */
  virtual webfr::async::CoroutineStarterForResult<const std::shared_ptr<RequestExecutor::ConnectionHandle>&> getConnectionAsync();

  /**
   * Invalidate connection.
   * @param connectionHandle
   */
  void invalidateConnection(const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle);

  virtual std::shared_ptr<Response> executeRequest(const webfr::String& method,
                                                   const StringTemplate& pathTemplate,
                                                   const Headers& headers,
                                                   const std::unordered_map<webfr::String, webfr::String>& pathParams,
                                                   const std::unordered_map<webfr::String, webfr::String>& queryParams,
                                                   const std::shared_ptr<RequestExecutor::Body>& body,
                                                   const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle = nullptr);
  
  virtual webfr::async::CoroutineStarterForResult<const std::shared_ptr<Response>&>
  executeRequestAsync(const webfr::String& method,
                      const StringTemplate& pathTemplate,
                      const Headers& headers,
                      const std::unordered_map<webfr::String, webfr::String>& pathParams,
                      const std::unordered_map<webfr::String, webfr::String>& queryParams,
                      const std::shared_ptr<RequestExecutor::Body>& body,
                      const std::shared_ptr<RequestExecutor::ConnectionHandle>& connectionHandle = nullptr);

public:

  template<typename T>
  struct TypeInterpretation {

    static webfr::String toString(const webfr::String& typeName, const T& parameter) {

      (void) parameter;

      WEBFR_LOGE("[webfr::web::client::ApiClient::TypeInterpretation::toString()]",
                 "Error. No conversion from '%s' to '%s' is defined.", typeName->getData(), "webfr::String");

      throw std::runtime_error(
        "[webfr::web::client::ApiClient::TypeInterpretation::toString()]: Error. "
        "No conversion from '" + typeName->std_str() + "' to 'webfr::String' is defined. "
        "Please define type conversion."
      );

    }

  };

};

template<>
struct ApiClient::TypeInterpretation<webfr::String> {
  static webfr::String toString(const webfr::String &typeName, const webfr::String &parameter) {
    (void) typeName;
    return parameter;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::Int8> {
  static webfr::String toString(const webfr::String &typeName, const webfr::Int8 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::int32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::UInt8> {
  static webfr::String toString(const webfr::String &typeName, const webfr::UInt8 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::uint32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::Int16> {
  static webfr::String toString(const webfr::String &typeName, const webfr::Int16 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::int32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::UInt16> {
  static webfr::String toString(const webfr::String &typeName, const webfr::UInt16 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::uint32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::Int32> {
  static webfr::String toString(const webfr::String &typeName, const webfr::Int32 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::int32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::UInt32> {
  static webfr::String toString(const webfr::String &typeName, const webfr::UInt32 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::uint32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::Int64> {
  static webfr::String toString(const webfr::String &typeName, const webfr::Int64 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::int64ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::UInt64> {
  static webfr::String toString(const webfr::String &typeName, const webfr::UInt64 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::uint64ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::Float32> {
  static webfr::String toString(const webfr::String &typeName, const webfr::Float32 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::float32ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::Float64> {
  static webfr::String toString(const webfr::String &typeName, const webfr::Float64 &parameter) {
    (void) typeName;
    if (parameter) {
      return utils::conversion::float64ToStr(*parameter);
    }
    return nullptr;
  }
};

template<>
struct ApiClient::TypeInterpretation<webfr::Boolean> {
  static webfr::String toString(const webfr::String &typeName, const webfr::Boolean &parameter) {
    (void) typeName;
    if(parameter != nullptr) {
        return utils::conversion::boolToStr(*parameter);
    }
    return nullptr;
  }
};

template<class T, class I>
struct ApiClient::TypeInterpretation<data::mapping::type::EnumObjectWrapper<T, I>> {

  typedef data::mapping::type::EnumObjectWrapper<T, I> EnumOW;
  typedef typename I::UnderlyingTypeObjectWrapper UTOW;

  static webfr::String toString(const webfr::String &typeName, const EnumOW &parameter) {

    data::mapping::type::EnumInterpreterError error = data::mapping::type::EnumInterpreterError::OK;
    const auto& value = I::toInterpretation(parameter, error);

    switch(error){
      case data::mapping::type::EnumInterpreterError::OK: break;
      case data::mapping::type::EnumInterpreterError::CONSTRAINT_NOT_NULL:
        throw std::runtime_error(
          "[webfr::web::client::ApiClient::TypeInterpretation::toString()]: Error. Enum constraint violation - NotNull."
        );
      default:
        throw std::runtime_error(
          "[webfr::web::client::ApiClient::TypeInterpretation::toString()]: Error. Can't interpret Enum."
        );
    }

    return ApiClient::TypeInterpretation<UTOW>::toString(typeName, value.template staticCast<UTOW>());

  }

};
  
}}}

#endif 