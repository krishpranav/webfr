#ifndef webfr_orm_Connection_hpp
#define webfr_orm_Connection_hpp

#include "webfr/core/base/Countable.hpp"

namespace webfr { namespace orm {

class Connection : public base::Countable {
public:

  /**
   * Virtual destructor.
   */
  virtual ~Connection() = default;

};

}}

#endif 