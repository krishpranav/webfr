#ifndef webfr_orm_Transaction_hpp
#define webfr_orm_Transaction_hpp

#include "Executor.hpp"

namespace webfr { namespace orm {

class Transaction {
private:
  base::ObjectHandle<Executor> m_executor;
  bool m_open;
private:
  std::shared_ptr<Connection> m_connection;
public:

  Transaction(const base::ObjectHandle<Executor>& executor, const std::shared_ptr<Connection>& connection = nullptr);

  Transaction(const Transaction& other) = delete;

  Transaction(Transaction&& other);

  virtual ~Transaction();

  Transaction& operator=(const Transaction& other) = delete;
  Transaction& operator=(Transaction&& other);

  std::shared_ptr<Connection> getConnection() const;

  std::shared_ptr<QueryResult> commit();

  std::shared_ptr<QueryResult> rollback();

};

}}

#endif