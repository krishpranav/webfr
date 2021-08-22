// includes
#include "CommandLineArgumentsTest.hpp"
#include "webfr/core/base/CommandLineArguments.hpp"
#include <cstring>

namespace webfr { namespace test { namespace base {
  
void CommandLineArgumentsTest::onRun() {
  
  int argc = 6;
  const char * argv[] = {"-k", "-c", "100", "-n", "500000", "http://127.0.0.1:8000/"};
  
  webfr::base::CommandLineArguments args(argc, argv);
  
  WEBFR_ASSERT(args.getArgumentIndex("-k") == 0);
  WEBFR_ASSERT(args.getArgumentIndex("-c") == 1);
  WEBFR_ASSERT(args.getArgumentIndex("100") == 2);
  WEBFR_ASSERT(args.getArgumentIndex("-n") == 3);
  WEBFR_ASSERT(args.getArgumentIndex("500000") == 4);
  WEBFR_ASSERT(args.getArgumentIndex("http://127.0.0.1:8000/") == 5);
  WEBFR_ASSERT(args.getArgumentIndex("not-existing-arg") == -1);
  
  WEBFR_ASSERT(args.hasArgument("-k"));
  WEBFR_ASSERT(args.hasArgument("not-existing-arg") == false);
  
  WEBFR_ASSERT(std::strcmp(args.getArgumentStartingWith("http"), "http://127.0.0.1:8000/") == 0);
  WEBFR_ASSERT(std::strcmp(args.getArgumentStartingWith("tcp", "tcp://default/"), "tcp://default/") == 0);
  
  WEBFR_ASSERT(std::strcmp(args.getNamedArgumentValue("-c"), "100") == 0);
  WEBFR_ASSERT(std::strcmp(args.getNamedArgumentValue("-c", nullptr), "100") == 0);
  
  WEBFR_ASSERT(std::strcmp(args.getNamedArgumentValue("-n"), "500000") == 0);
  WEBFR_ASSERT(std::strcmp(args.getNamedArgumentValue("--non-existing", "default"), "default") == 0);

}
  
}}}