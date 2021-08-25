#ifndef webfr_web_url_mapping_Router_hpp
#define webfr_web_url_mapping_Router_hpp

#include "./Pattern.hpp"
#include "webfr/core/Types.hpp"
#include <utility>
#include <list>

namespace webfr { namespace web { namespace url { namespace mapping {

template<typename Endpoint>
class Router : public base::Countable {
private:

  typedef std::pair<std::shared_ptr<Pattern>, Endpoint> Pair;

  typedef webfr::data::share::StringKeyLabel StringKeyLabel;
public:

  class Route {
  private:
    bool m_valid;
    Endpoint m_endpoint;
    Pattern::MatchMap m_matchMap;
  public:

    Route()
      : m_valid(false)
    {}

    Route(const Endpoint& endpoint, Pattern::MatchMap&& matchMap)
      : m_valid(true)
      , m_endpoint(endpoint)
      , m_matchMap(matchMap)
    {}

    const Endpoint& getEndpoint() {
      return m_endpoint;
    }

    const Pattern::MatchMap& getMatchMap() {
      return m_matchMap;
    }

    bool isValid() {
      return m_valid;
    }
    
    explicit operator bool() const {
      return m_valid;
    }
    
  };
  
private:
  std::list<Pair> m_endpointsByPattern;
public:
  
  static std::shared_ptr<Router> createShared(){
    return std::make_shared<Router>();
  }

  void route(const webfr::String& pathPattern, const Endpoint& endpoint) {
    auto pattern = Pattern::parse(pathPattern);
    m_endpointsByPattern.push_back({pattern, endpoint});
  }

  Route getRoute(const StringKeyLabel& path){

    for(auto& pair : m_endpointsByPattern) {
      Pattern::MatchMap matchMap;
      if(pair.first->match(path, matchMap)) {
        return Route(pair.second, std::move(matchMap));
      }
    }

    return Route();
  }
  
  void logRouterMappings(const webfr::data::share::StringKeyLabel &branch) {

    for(auto& pair : m_endpointsByPattern) {
      auto mapping = pair.first->toString();
      WEBFR_LOGD("Router", "url '%s %s' -> mapped", (const char*)branch.getData(), (const char*) mapping->getData());
    }

  }
  
};
  
}}}}

#endif 