#ifndef webfr_web_server_rest_Endpoint_hpp
#define webfr_web_server_rest_Endpoint_hpp

#include "webfr/web/server/HttpRequestHandler.hpp"
#include <list>
#include <unordered_map>
#include <functional>

namespace webfr { namespace web { namespace server { namespace api {

class Endpoint : public webfr::base::Countable {
public:

  /**
   * Convenience typedef for &id:webfr::web::server::HttpRequestHandler;.
   */
  typedef webfr::web::server::HttpRequestHandler RequestHandler;
public:

  /**
   * Info holds API documentation information about endpoint
   */
  class Info : public webfr::base::Countable {
  public:

    /**
     * Param holds info about parameter
     */
    struct Param {
      
      Param();
      Param(const webfr::String& pName, webfr::data::mapping::type::Type* pType);

      webfr::String name;
      webfr::data::mapping::type::Type* type;

      webfr::String description;
      webfr::Boolean required = true;
      webfr::Boolean deprecated = false;
      webfr::Boolean allowEmptyValue;
      std::list<std::pair<webfr::String, webfr::Any>> examples;

      Param& addExample(const webfr::String& title, const webfr::Any& example) {
        examples.push_back({title, example});
        return *this;
      }

    };

    /**
     * Parameters container
     */
    class Params {
    private:
      std::list<webfr::String> m_order;
      std::unordered_map<webfr::String, Param> m_params;
    public:

      const std::list<webfr::String>& getOrder() const;

      Param& add(const webfr::String& name, webfr::data::mapping::type::Type* type);

      template<class T>
      Param& add(const webfr::String& name) {
        return add(name, T::Class::getType());
      }

      Param& operator [](const webfr::String& name);

    };

    struct ContentHints {
      webfr::String contentType;
      webfr::data::mapping::type::Type* schema;
      webfr::String description;
      std::list<std::pair<webfr::String, webfr::Any>> examples;

      ContentHints& addExample(const webfr::String& title, const webfr::Any& example) {
        examples.push_back({title, example});
        return *this;
      }
    };
    
  public:

    /**
     * Constructor;
     */
    Info();

    /**
     * Create shared Info.
     * @return
     */
    static std::shared_ptr<Info> createShared();

    /**
     * Endpoint name.
     */
    webfr::String name;

    /**
     * Endpoint summary.
     */
    webfr::String summary;

    /**
     * Endpoint description.
     */
    webfr::String description;

    /**
     * Endpoint path.
     */
    webfr::String path;

    /**
     * HTTP method.
     */
    webfr::String method;

    /**
     * Authorization.
     */
    webfr::String authorization;

    /**
     * Hide endpoint from the documentation.
     */
    webfr::Boolean hide;

    /**
     * Tags to group endpoints in the documentation.
     */
    std::list<webfr::String> tags;

    /**
     * Body info.
     */
    Param body;

    /**
     * Body content type.
     */
    webfr::String bodyContentType;

    /**
     * Consumes.
     */
    std::list<ContentHints> consumes;

    /**
     * Security Requirements
     */
    std::unordered_map<webfr::String, std::shared_ptr<std::list<webfr::String>>> securityRequirements;

    /**
     * Headers.
     */
    Params headers;

    /**
     * Path variables.
     */
    Params pathParams;

    /**
     * Query params.
     */
    Params queryParams;
    
    /**
     *  ResponseCode to {ContentType, Type} mapping.
     *  Example responses[Status::CODE_200] = {"application/json", MyDto::ObjectWrapper::Class::getType()};
     */
    std::unordered_map<webfr::web::protocol::http::Status, ContentHints> responses;
    
    webfr::String toString();

    /**
     * Add "consumes" info to endpoint.
     * @tparam T
     * @param contentType
     */
    template<class Wrapper>
    ContentHints& addConsumes(const webfr::String& contentType, const webfr::String& description = webfr::String()) {
      consumes.push_back({contentType, Wrapper::Class::getType(), description});
      return consumes.back();
    }

    /**
     * Add response info to endpoint
     * @tparam Wrapper
     * @param status
     * @param contentType
     * @param responseDescription
     */
    template<class Wrapper>
    ContentHints& addResponse(const webfr::web::protocol::http::Status& status, const webfr::String& contentType, const webfr::String& responseDescription = webfr::String()) {
      auto& hint = responses[status];
      hint.contentType = contentType;
      hint.description = responseDescription.get() == nullptr ? status.description : responseDescription;
      hint.schema = Wrapper::Class::getType();
      return hint;
    }

    /**
     * Add security requirement.
     * @param requirement
     * @param scopes
     */
    void addSecurityRequirement(const webfr::String &requirement, const std::shared_ptr<std::list<webfr::String>> &scopes = nullptr) {
      securityRequirements[requirement] = scopes;
    }

    /**
     * Add tag.
     * @param tag
     */
    void addTag(const webfr::String& tag) {
      tags.push_back(tag);
    }

  };
public:

  Endpoint(const std::shared_ptr<RequestHandler>& pHandler, const std::function<std::shared_ptr<Endpoint::Info>()>& infoBuilder);
  
  static std::shared_ptr<Endpoint> createShared(const std::shared_ptr<RequestHandler>& handler,
                                                const std::function<std::shared_ptr<Endpoint::Info>()>& infoBuilder);
  
  const std::shared_ptr<RequestHandler> handler;

  std::shared_ptr<Info> info();

private:
  std::shared_ptr<Info> m_info;
  std::function<std::shared_ptr<Endpoint::Info>()> m_infoBuilder;
  
};
  
}}}}

#endif 