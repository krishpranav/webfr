#include "DbClient.hpp"

#include "webfr/core/data/stream/BufferStream.hpp"

namespace webfr { namespace orm {

DbClient::DbClient(const std::shared_ptr<Executor>& executor)
  : m_executor(executor)
  , m_typeResolver(executor->createTypeResolver())
{}

std::shared_ptr<Connection> DbClient::getConnection() {
  return m_executor->getConnection();
}

void DbClient::setEnabledInterpretations(const std::vector<std::string>& enabledInterpretations) {
  m_typeResolver->setEnabledInterpretations(enabledInterpretations);
}

std::shared_ptr<const data::mapping::TypeResolver> DbClient::getTypeResolver() {
  return m_typeResolver;
}

data::share::StringTemplate DbClient::parseQueryTemplate(const webfr::String& name,
                                                         const webfr::String& text,
                                                         const Executor::ParamsTypeMap& paramsTypeMap,
                                                         bool prepare)
{
  return m_executor->parseQueryTemplate(name, text, paramsTypeMap, prepare);
}

std::shared_ptr<QueryResult> DbClient::execute(const data::share::StringTemplate& queryTemplate,
                                               const std::unordered_map<webfr::String, webfr::Void>& params,
                                               const std::shared_ptr<Connection>& connection)
{
  return m_executor->execute(queryTemplate, params, m_typeResolver, connection);
}

std::shared_ptr<QueryResult> DbClient::executeQuery(const webfr::String& query,
                                                    const std::unordered_map<webfr::String, webfr::Void>& params,
                                                    const std::shared_ptr<Connection>& connection)
{
  return m_executor->execute(query, params, m_typeResolver, connection);
}

Transaction DbClient::beginTransaction(const std::shared_ptr<Connection>& connection) {
  return Transaction(m_executor, connection);
}

}}