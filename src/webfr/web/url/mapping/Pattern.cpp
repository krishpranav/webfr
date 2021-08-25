#include "Pattern.hpp"
#include "webfr/core/data/stream/ChunkedBuffer.hpp"

namespace webfr { namespace web { namespace url { namespace mapping {

const char* Pattern::Part::FUNCTION_CONST = "const";
const char* Pattern::Part::FUNCTION_VAR = "var";
const char* Pattern::Part::FUNCTION_ANY_END = "tail";

std::shared_ptr<Pattern> Pattern::parse(p_char8 data, v_buff_size size){
  
  if(size <= 0){
    return nullptr;
  }
  
  auto result = Pattern::createShared();

  v_buff_size lastPos = 0;
  v_buff_size i = 0;
  
  while(i < size){
    
    v_char8 a = data[i];
    
    if(a == '/'){
      
      if(i - lastPos > 0){
        auto part = Part::createShared(Part::FUNCTION_CONST, webfr::String((const char*)&data[lastPos], i - lastPos, true));
        result->m_parts->pushBack(part);
      }
      
      lastPos = i + 1;
      
    } else if(a == '*'){
      lastPos = i + 1;
      if(size > lastPos){
        auto part = Part::createShared(Part::FUNCTION_ANY_END, webfr::String((const char*)&data[lastPos], size - lastPos, true));
        result->m_parts->pushBack(part);
      }else{
        auto part = Part::createShared(Part::FUNCTION_ANY_END, webfr::String((v_buff_size)0));
        result->m_parts->pushBack(part);
      }
      return result;
    
    } else if(a == '{'){
      
      lastPos = i + 1;
      while(i < size && data[i] != '}'){
        i++;
      }
      
      if(i > lastPos){
        auto part = Part::createShared(Part::FUNCTION_VAR, webfr::String((const char*)&data[lastPos], i - lastPos, true));
        result->m_parts->pushBack(part);
      }else{
        auto part = Part::createShared(Part::FUNCTION_VAR, webfr::String((v_buff_size)0));
        result->m_parts->pushBack(part);
      }
      
      lastPos = i + 1;
      
    }
    
    i++;
    
  }
  
  if(i - lastPos > 0){
    auto part = Part::createShared(Part::FUNCTION_CONST, webfr::String((const char*)&data[lastPos], i - lastPos, true));
    result->m_parts->pushBack(part);
  }
  
  return result;
}

std::shared_ptr<Pattern> Pattern::parse(const char* data){
  return parse((p_char8) data, std::strlen(data));
}

std::shared_ptr<Pattern> Pattern::parse(const webfr::String& data){
  return parse(data->getData(), data->getSize());
}
  
v_char8 Pattern::findSysChar(webfr::parser::Caret& caret) {
  auto data = caret.getData();
  for (v_buff_size i = caret.getPosition(); i < caret.getDataSize(); i++) {
    v_char8 a = data[i];
    if(a == '/' || a == '?') {
      caret.setPosition(i);
      return a;
    }
  }
  caret.setPosition(caret.getDataSize());
  return 0;
}
  
bool Pattern::match(const StringKeyLabel& url, MatchMap& matchMap) {
  
  webfr::parser::Caret caret(url.getData(), url.getSize());
  
  if(m_parts->count() == 0){
    
    if(caret.skipChar('/')){
      return false;
    }
    
    return true;
    
  }
  
  auto curr = m_parts->getFirstNode();
  
  while(curr != nullptr){
    const std::shared_ptr<Part>& part = curr->getData();
    curr = curr->getNext();
    caret.skipChar('/');
    
    if(part->function == Part::FUNCTION_CONST){
      
      if(!caret.isAtText(part->text->getData(), part->text->getSize(), true)){
        return false;
      }
      
      if(caret.canContinue() && !caret.isAtChar('/')){
        if(caret.isAtChar('?') && (curr == nullptr || curr->getData()->function == Part::FUNCTION_ANY_END)) {
          matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getDataSize() - caret.getPosition());
          return true;
        }
        return false;
      }
      
    }else if(part->function == Part::FUNCTION_ANY_END){
      if(caret.getDataSize() > caret.getPosition()){
        matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getDataSize() - caret.getPosition());
      }
      return true;
    }else if(part->function == Part::FUNCTION_VAR){
      
      if(!caret.canContinue()){
        return false;
      }

      auto label = caret.putLabel();
      v_char8 a = findSysChar(caret);
      if(a == '?') {
        if(curr == nullptr || curr->getData()->function == Part::FUNCTION_ANY_END) {
          matchMap.m_variables[part->text] = StringKeyLabel(url.getMemoryHandle(), label.getData(), label.getSize());
          matchMap.m_tail = StringKeyLabel(url.getMemoryHandle(), caret.getCurrData(), caret.getDataSize() - caret.getPosition());
          return true;
        }
        caret.findChar('/');
      }
      
      matchMap.m_variables[part->text] = StringKeyLabel(url.getMemoryHandle(), label.getData(), label.getSize());
      
    }
    
  }
  
  caret.skipChar('/');
  if(caret.canContinue()){
    return false;
  }
  
  return true;
  
}

webfr::String Pattern::toString() {
  auto stream = webfr::data::stream::ChunkedBuffer::createShared();
  auto curr = m_parts->getFirstNode();
  while (curr != nullptr) {
    const std::shared_ptr<Part>& part = curr->getData();
    curr = curr->getNext();
    if(part->function == Part::FUNCTION_CONST) {
      stream->writeSimple("/", 1);
      stream->writeSimple(part->text);
    } else if(part->function == Part::FUNCTION_VAR) {
      stream->writeSimple("/{", 2);
      stream->writeSimple(part->text);
      stream->writeSimple("}", 1);
    } else if(part->function == Part::FUNCTION_ANY_END) {
      stream->writeSimple("/*", 2);
    }
  }
  return stream->toString();
}
  
}}}}