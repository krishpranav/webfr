// includes
#include "Base64Test.hpp"
#include "webfr/encoding/Base64.hpp"

namespace webfr { namespace test { namespace encoding {
  
void Base64Test::onRun() {

  webfr::String message = "oat++ web framework";
  webfr::String messageEncoded = "b2F0Kysgd2ViIGZyYW1ld29yaw==";
  
  {
    webfr::String encoded = webfr::encoding::Base64::encode(message);
    WEBFR_LOGV(TAG, "encoded='%s'", encoded->c_str());
    WEBFR_ASSERT(encoded->equals(messageEncoded.get()));
    webfr::String decoded = webfr::encoding::Base64::decode(encoded);
    WEBFR_ASSERT(message->equals(decoded.get()));
  }
  
  {
    webfr::String encoded = webfr::encoding::Base64::encode(message, webfr::encoding::Base64::ALPHABET_BASE64_URL_SAFE);
    WEBFR_LOGV(TAG, "encoded='%s'", encoded->c_str());
    webfr::String decoded = webfr::encoding::Base64::decode(encoded, webfr::encoding::Base64::ALPHABET_BASE64_URL_SAFE_AUXILIARY_CHARS);
    WEBFR_ASSERT(message->equals(decoded.get()));
  }

}
  
}}}