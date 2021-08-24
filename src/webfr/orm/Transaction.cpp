#include "Transaction.hpp"

namespace webfr { namespace orm {

Transaction::Transaction(const base::ObjectHandle<Executor>& executor, const std::shared_ptr<Connection>& connection)
  : m_executor(executor)
  , m_open(true)
{
  auto res = executor->begin(connection);
  if(res->isSuccess()) {
    m_connection = res->getConnection();
  } else {
    m_open = false;
    throw std::runtime_error("[webfr::orm::Transaction::Transaction()]: "
                             "Error. Can't begin transaction - " + res->getErrorMessage()->std_str());
  }
}

Transaction::Transaction(Transaction&& other)
  : m_executor(std::move(other.m_executor))
  , m_open(other.m_open)
  , m_connection(std::move(other.m_connection))
{
  other.m_open = false;
}

Transaction::~Transaction() {
  if(m_open) {
    try {
      rollback();
    } catch (...) {
      
    }
  }
}

Transaction& Transaction::operator=(Transaction&& other) {

  m_executor = std::move(other.m_executor);
  m_open = other.m_open;
  m_connection = std::move(other.m_connection);

  other.m_open = false;

  return *this;

}

std::shared_ptr<Connection> Transaction::getConnection() const {
  return m_connection;
}

std::shared_ptr<QueryResult> Transaction::commit() {
  if(m_open) {
    m_open = false;
    return m_executor->commit(m_connection);
  } else {
    throw std::runtime_error("[webfr::orm::Transaction::commit()]: Error. The transaction is already closed.");
  }
}

std::shared_ptr<QueryResult> Transaction::rollback() {
  if(m_open) {
    m_open = false;
    return m_executor->rollback(m_connection);
  } else {
    throw std::runtime_error("[webfr::orm::Transaction::commit()]: Error. The transaction is already closed.");
  }
}

}}