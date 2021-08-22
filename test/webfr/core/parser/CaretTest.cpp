// includes
#include "CaretTest.hpp"
#include "webfr/core/parser/Caret.hpp"

namespace webfr { namespace test { namespace parser {

  namespace {
    typedef webfr::parser::Caret Caret;
  }

void CaretTest::onRun() {

  {
    Caret caret(" \t\n\r\f \t\n\r\f \t\n\r\fhello!\t\n\r\f");
    WEBFR_ASSERT(caret.skipBlankChars());
    WEBFR_ASSERT(caret.isAtChar('h'));
    WEBFR_ASSERT(caret.isAtText("hello!"));
    WEBFR_ASSERT(caret.isAtText("hello!", true));
    WEBFR_ASSERT(caret.skipBlankChars() == false); // false because no other char found
    WEBFR_ASSERT(caret.canContinue() == false);
    WEBFR_ASSERT(caret.getPosition() == caret.getDataSize());
  }

  {
    Caret caret(" \t\n\r\f \t\n\r\f \t\n\r\fhello!\t\n\r\f");
    WEBFR_ASSERT(caret.findText("hello!"));
    WEBFR_ASSERT(caret.isAtText("hello!"));
    WEBFR_ASSERT(caret.isAtTextNCS("HELLO!"));
    WEBFR_ASSERT(caret.isAtTextNCS("HELLO!", true));
    WEBFR_ASSERT(caret.skipBlankChars() == false); // false because no other char found
    WEBFR_ASSERT(caret.canContinue() == false);
    WEBFR_ASSERT(caret.getPosition() == caret.getDataSize());
  }

  {
    Caret caret(" \t\n\r\f \t\n\r\f \t\n\r\fhello!\t\n\r\f");
    WEBFR_ASSERT(caret.findText("hello world!") == false);
    WEBFR_ASSERT(caret.canContinue() == false);
    WEBFR_ASSERT(caret.getPosition() == caret.getDataSize());
  }

  {
    Caret caret("\r\n'let\\'s'\r\n'play'");
    WEBFR_ASSERT(caret.findRN());
    WEBFR_ASSERT(caret.skipRN());

    auto label = caret.parseStringEnclosed('\'', '\'', '\\');
    WEBFR_ASSERT(label);
    WEBFR_ASSERT(label.toString() == "let\\'s");

    WEBFR_ASSERT(caret.skipRN());

    label = caret.parseStringEnclosed('\'', '\'', '\\');
    WEBFR_ASSERT(label);
    WEBFR_ASSERT(label.toString() == "play");

    WEBFR_ASSERT(caret.canContinue() == false);
    WEBFR_ASSERT(caret.getPosition() == caret.getDataSize());
  }

}

}}}