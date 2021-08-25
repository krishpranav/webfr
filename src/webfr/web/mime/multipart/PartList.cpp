#include "PartList.hpp"

namespace webfr { namespace web { namespace mime { namespace multipart {

PartList::PartList(const webfr::String& boundary)
  : Multipart(boundary)
  , m_readIteratorInitialized(false)
{}

PartList::PartList(const Headers& requestHeaders)
  : Multipart(parseBoundaryFromHeaders(requestHeaders))
  , m_readIteratorInitialized(false)
{
  if(!getBoundary()) {
    throw std::runtime_error("[webfr::web::mime::multipart::PartList::PartList]: Error. No 'boundary' value found in headers.");
  }
}

std::shared_ptr<PartList> PartList::createSharedWithRandomBoundary(v_int32 boundarySize) {
  auto boundary = generateRandomBoundary(boundarySize);
  return std::make_shared<PartList>(boundary);
}

std::shared_ptr<Part> PartList::readNextPart(async::Action& action) {
  if(!m_readIteratorInitialized) {
    m_readIteratorInitialized = true;
    m_iterator = m_parts.begin();
  }
  if(m_iterator == m_parts.end()) {
    return nullptr;
  }
  return *m_iterator ++;
}

void PartList::writeNextPart(const std::shared_ptr<Part>& part, async::Action& action) {

  if(part->getName()) {
    auto it = m_namedParts.find(part->getName());
    if(it != m_namedParts.end()) {
      throw std::runtime_error("[webfr::web::mime::multipart::Multipart::addPart()]: Error. Part with such name already exists.");
    }
    m_namedParts.insert({part->getName(), part});
  }

  m_parts.push_back(part);

}

std::shared_ptr<Part> PartList::getNamedPart(const webfr::String& name) {

  auto it = m_namedParts.find(name);
  if(it != m_namedParts.end()) {
    return it->second;
  }

  return nullptr;

}

const std::list<std::shared_ptr<Part>>& PartList::getAllParts() {
  return m_parts;
}

v_int64 PartList::count() {
  return m_parts.size();
}

}}}}