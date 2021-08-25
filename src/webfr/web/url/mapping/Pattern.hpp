#ifndef webfr_web_url_mapping_Pattern_hpp
#define webfr_web_url_mapping_Pattern_hpp

#include "webfr/core/data/share/MemoryLabel.hpp"
#include "webfr/core/collection/LinkedList.hpp"
#include "webfr/core/parser/Caret.hpp"
#include <unordered_map>

namespace webfr { namespace web { namespace url { namespace mapping {
  
class Pattern : public base::Countable{
private:
  typedef webfr::data::share::StringKeyLabel StringKeyLabel;
public:
  
  class MatchMap {
    friend Pattern;
  public:
    typedef std::unordered_map<StringKeyLabel, StringKeyLabel> Variables;
  private:
    Variables m_variables;
    StringKeyLabel m_tail;
  public:
    
    MatchMap() {}
    
    MatchMap(const Variables& vars, const StringKeyLabel& urlTail)
      : m_variables(vars)
      , m_tail(urlTail)
    {}
    
    webfr::String getVariable(const StringKeyLabel& key) const {
      auto it = m_variables.find(key);
      if(it != m_variables.end()) {
        return it->second.toString();
      }
      return nullptr;
    }
    
    webfr::String getTail() const {
      return m_tail.toString();
    }

    const Variables& getVariables() const {
      return m_variables;
    }
    
  };
  
private:
  
  class Part : public base::Countable{
  public:
    Part(const char* pFunction, const webfr::String& pText)
      : function(pFunction)
      , text(pText)
    {}
  public:
    
    static const char* FUNCTION_CONST;
    static const char* FUNCTION_VAR;
    static const char* FUNCTION_ANY_END;
    
    static std::shared_ptr<Part> createShared(const char* function, const webfr::String& text){
      return std::make_shared<Part>(function, text);
    }
    
    const char* function;
    const webfr::String text;
    
  };
  
private:
  std::shared_ptr<webfr::collection::LinkedList<std::shared_ptr<Part>>> m_parts;
private:
  v_char8 findSysChar(webfr::parser::Caret& caret);
public:
  Pattern()
    : m_parts(webfr::collection::LinkedList<std::shared_ptr<Part>>::createShared())
  {}
public:
  
  static std::shared_ptr<Pattern> createShared(){
    return std::make_shared<Pattern>();
  }
  
  static std::shared_ptr<Pattern> parse(p_char8 data, v_buff_size size);
  static std::shared_ptr<Pattern> parse(const char* data);
  static std::shared_ptr<Pattern> parse(const webfr::String& data);
  
  bool match(const StringKeyLabel& url, MatchMap& matchMap);
  
  webfr::String toString();
  
};
  
}}}}

#endif 