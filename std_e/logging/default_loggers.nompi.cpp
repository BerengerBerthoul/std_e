#include "std_e/logging/log.hpp"

using namespace std_e;

namespace {

bool init_default_loggers() {
  add_logger({"terminal",full,std::make_unique<stdout_printer>()});
  add_logger({"file"    ,full,std::make_unique<file_printer>()  });
  return true;
}

// constant here just to trigger initialization
const bool _ = init_default_loggers();

}
