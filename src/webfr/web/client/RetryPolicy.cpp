#include "RetryPolicy.hpp"

namespace webfr { namespace web { namespace client {

SimpleRetryPolicy::SimpleRetryPolicy(v_int64 maxAttempts,
                                     const std::chrono::duration<v_int64, std::micro>& delay,
                                     const std::unordered_set<v_int32>& httpCodes)
  : m_maxAttempts(maxAttempts)
  , m_delay(delay.count())
  , m_httpCodes(httpCodes)
{}

bool SimpleRetryPolicy::canRetry(const Context& context) {
  return context.attempt <= m_maxAttempts || m_maxAttempts == -1;
}

bool SimpleRetryPolicy::retryOnResponse(v_int32 responseStatusCode, const Context& context) {
  (void) context;
  return m_httpCodes.find(responseStatusCode) != m_httpCodes.end();
}

v_int64 SimpleRetryPolicy::waitForMicroseconds(const Context& context) {
  (void) context;
  return m_delay;
}

}}}