#ifndef webfr_web_client_RetryPolicy_hpp
#define webfr_web_client_RetryPolicy_hpp

#include "webfr/core/Types.hpp"

#include <unordered_set>

namespace webfr { namespace web { namespace client {

class RetryPolicy {
public:

  struct Context {

    v_int64 attempt = 0;
  };

public:

  virtual ~RetryPolicy() = default;

  virtual bool canRetry(const Context& context) = 0;

  virtual bool retryOnResponse(v_int32 responseStatusCode, const Context& context) = 0;

  virtual v_int64 waitForMicroseconds(const Context& context) = 0;

};

class SimpleRetryPolicy : public RetryPolicy {
private:
  v_int64 m_maxAttempts;
  v_int64 m_delay;
  std::unordered_set<v_int32> m_httpCodes;

public:

  SimpleRetryPolicy(v_int64 maxAttempts,
                    const std::chrono::duration<v_int64, std::micro>& delay,
                    const std::unordered_set<v_int32>& httpCodes = {503});

  bool canRetry(const Context& context) override;

  bool retryOnResponse(v_int32 responseStatusCode, const Context& context) override;

  v_int64 waitForMicroseconds(const Context& context) override;

};

}}}

#endif 