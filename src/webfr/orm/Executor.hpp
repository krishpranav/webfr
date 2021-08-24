#ifndef webfr_orm_Executor_hpp
#define webfr_orm_Executor_hpp

#include "Connection.hpp"
#include "QueryResult.hpp"
#include "webfr/core/data/mapping/TypeResolver.hpp"
#include "webfr/core/data/mapping/type/Type.hpp"
#include "webfr/core/data/share/MemoryLabel.hpp"
#include "webfr/core/data/share/StringTemplate.hpp"

namespace webfr { namespace orm {

class Executor {
public:
  typedef webfr::data::share::StringTemplate StringTemplate;
  typedef std::unordered_map<data::share::StringKeyLabel, const webfr::Type*> ParamsTypeMap;
protected:
  std::shared_ptr<data::mapping::TypeResolver> m_defaultTypeResolver;
public:

  Executor();

  virtual ~Executor() = default;

  std::shared_ptr<const data::mapping::TypeResolver> getDefaultTypeResolver();

  virtual std::shared_ptr<data::mapping::TypeResolver> createTypeResolver() = 0;


  virtual std::shared_ptr<Connection> getConnection() = 0;


  virtual StringTemplate parseQueryTemplate(const webfr::String& name,
                                            const webfr::String& text,
                                            const ParamsTypeMap& paramsTypeMap,
                                            bool prepare = false) = 0;

  
  virtual std::shared_ptr<QueryResult> execute(const StringTemplate& queryTemplate,
                                               const std::unordered_map<webfr::String, webfr::Void>& params,
                                               const std::shared_ptr<const data::mapping::TypeResolver>& typeResolver = nullptr,
                                               const std::shared_ptr<Connection>& connection = nullptr) = 0;


  virtual std::shared_ptr<QueryResult> execute(const webfr::String& query,
                                               const std::unordered_map<webfr::String, webfr::Void>& params,
                                               const std::shared_ptr<const data::mapping::TypeResolver>& typeResolver = nullptr,
                                               const std::shared_ptr<Connection>& connection = nullptr);


  virtual std::shared_ptr<QueryResult> begin(const std::shared_ptr<Connection>& connection = nullptr) = 0;


  virtual std::shared_ptr<QueryResult> commit(const std::shared_ptr<Connection>& connection) = 0;


  virtual std::shared_ptr<QueryResult> rollback(const std::shared_ptr<Connection>& connection) = 0;


  virtual v_int64 getSchemaVersion(const webfr::String& suffix = nullptr,
                                   const std::shared_ptr<Connection>& connection = nullptr) = 0;


  virtual void migrateSchema(const webfr::String& script,
                             v_int64 newVersion,
                             const webfr::String& suffix = nullptr,
                             const std::shared_ptr<Connection>& connection = nullptr) = 0;

};

}}

#endif 