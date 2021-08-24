#include "Executor.hpp"

namespace webfr { namespace orm {

Executor::Executor()
  : m_defaultTypeResolver(std::make_shared<data::mapping::TypeResolver>())
{}

std::shared_ptr<const data::mapping::TypeResolver> Executor::getDefaultTypeResolver() {
  return m_defaultTypeResolver;
}

std::shared_ptr<QueryResult> Executor::execute(const webfr::String& query,
                                               const std::unordered_map<webfr::String, webfr::Void>& params,
                                               const std::shared_ptr<const data::mapping::TypeResolver>& typeResolver,
                                               const std::shared_ptr<Connection>& connection)
{
  const auto& qt = parseQueryTemplate(nullptr, query, {}, false);
  return execute(qt, params, typeResolver, connection);
}

}}