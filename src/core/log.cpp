#include "log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace dawn {

namespace log_internal {
std::shared_ptr<spdlog::logger> _internal_logger{};
std::shared_ptr<spdlog::logger> _client_logger{};

void init_log() {
  spdlog::set_pattern("%^[%T] %n: %v%$");
  _internal_logger = spdlog::stderr_color_mt("DAWN");
  _client_logger = spdlog::stderr_color_mt("APP");
}

}  // namespace log_internal
}  // namespace dawn
