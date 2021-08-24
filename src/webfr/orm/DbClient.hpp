#ifndef webfr_orm_DbClient_hpp
#define webfr_orm_DbClient_hpp

#include "Executor.hpp"
#include "Transaction.hpp"
#include "webfr/core/data/stream/Stream.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace orm {

class DbClient {
private:
  std::shared_ptr<Executor> m_executor;
protected:
  std::shared_ptr<data::mapping::TypeResolver> m_typeResolver;
public:

  /**
   * @param executor - &id:oatpp::orm::Executor;.
   */
  DbClient(const std::shared_ptr<Executor>& executor);

  virtual ~DbClient() = default;

  std::shared_ptr<Connection> getConnection();

  void setEnabledInterpretations(const std::vector<std::string>& enabledInterpretations);

  std::shared_ptr<const data::mapping::TypeResolver> getTypeResolver();

  data::share::StringTemplate parseQueryTemplate(const oatpp::String& name,
                                                 const oatpp::String& text,
                                                 const Executor::ParamsTypeMap& paramsTypeMap,
                                                 bool prepare = false);

  std::shared_ptr<QueryResult> execute(const data::share::StringTemplate& queryTemplate,
                                       const std::unordered_map<oatpp::String, oatpp::Void>& params,
                                       const std::shared_ptr<Connection>& connection = nullptr);

  std::shared_ptr<QueryResult> executeQuery(const oatpp::String& query,
                                            const std::unordered_map<oatpp::String, oatpp::Void>& params,
                                            const std::shared_ptr<Connection>& connection = nullptr);

  /**
   * @param connection 
   */
  Transaction beginTransaction(const std::shared_ptr<Connection>& connection = nullptr);

};

}}

#endif