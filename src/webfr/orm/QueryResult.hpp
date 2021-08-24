#ifndef webfr_orm_QueryResult_hpp
#define webfr_orm_QueryResult_hpp

#include "Connection.hpp"
#include "webfr/core/Types.hpp"

namespace webfr { namespace orm {

class QueryResult {
public:

  virtual ~QueryResult() = default;

  virtual std::shared_ptr<Connection> getConnection() const = 0;

  virtual bool isSuccess() const = 0;

  virtual webfr::String getErrorMessage() const = 0;

  virtual v_int64 getPosition() const = 0;

  virtual v_int64 getKnownCount() const = 0;

  virtual bool hasMoreToFetch() const = 0;

  virtual webfr::Void fetch(const webfr::Type* const resultType, v_int64 count) = 0;

  template<class Wrapper>
  Wrapper fetch(v_int64 count = -1) {
    return fetch(Wrapper::Class::getType(), count).template staticCast<Wrapper>();
  }

};

}}

#endif 