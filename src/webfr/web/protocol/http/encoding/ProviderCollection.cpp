#include "ProviderCollection.hpp"

namespace webfr { namespace web { namespace protocol { namespace http { namespace encoding {

void ProviderCollection::add(const std::shared_ptr<EncoderProvider>& provider) {
  m_providers[provider->getEncodingName()] = provider;
}

std::shared_ptr<EncoderProvider> ProviderCollection::get(const data::share::StringKeyLabelCI& encoding) const {
  auto it = m_providers.find(encoding);
  if(it != m_providers.end()) {
    return it->second;
  }
  return nullptr;
}

std::shared_ptr<EncoderProvider> ProviderCollection::get(const std::unordered_set<data::share::StringKeyLabelCI>& encodings) const {

  for(const auto& encoding : encodings) {
    auto provider = get(encoding);
    if(provider) {
      return provider;
    }
  }

  return nullptr;

}

}}}}}