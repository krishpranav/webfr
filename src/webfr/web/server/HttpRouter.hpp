#ifndef webfr_web_server_HttpRouter_hpp
#define webfr_web_server_HttpRouter_hpp

#include "./HttpRequestHandler.hpp"
#include "webfr/web/url/mapping/Router.hpp"

namespace webfr { namespace web { namespace server {

template<typename RouterEndpoint>
class HttpRouterTemplate : public webfr::base::Countable {
private:

  typedef data::share::StringKeyLabel StringKeyLabel;
public:

  typedef web::url::mapping::Router<RouterEndpoint> BranchRouter;

  typedef std::unordered_map<StringKeyLabel, std::shared_ptr<BranchRouter>> BranchMap;
protected:
  BranchMap m_branchMap;
protected:

  const std::shared_ptr<BranchRouter>& getBranch(const StringKeyLabel& name){
    auto it = m_branchMap.find(name);
    if(it == m_branchMap.end()){
      m_branchMap[name] = BranchRouter::createShared();
    }
    return m_branchMap[name];
  }

public:

  HttpRouterTemplate() = default;

  static std::shared_ptr<HttpRouterTemplate> createShared() {
    return std::make_shared<HttpRouterTemplate>();
  }

  void route(const webfr::String& method, const webfr::String& pathPattern, const RouterEndpoint& endpoint) {
    getBranch(method)->route(pathPattern, endpoint);
  }

  typename BranchRouter::Route getRoute(const StringKeyLabel& method, const StringKeyLabel& path){
    auto it = m_branchMap.find(method);
    if(it != m_branchMap.end()) {
      return m_branchMap[method]->getRoute(path);
    }
    return typename BranchRouter::Route();
  }

  void logRouterMappings() {
    for(auto it : m_branchMap) {
      it.second->logRouterMappings(it.first);
    }
  }
  
};

typedef HttpRouterTemplate<std::shared_ptr<HttpRequestHandler>> HttpRouter;
  
}}}

#endif