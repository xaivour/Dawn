#pragma once

#include <spdlog/spdlog.h>
#include <memory>
#include "core/types.h"

namespace dawn {
namespace log_internal {

extern std::shared_ptr<spdlog::logger> _internal_logger;
extern std::shared_ptr<spdlog::logger> _client_logger;

void init_log();

inline std::shared_ptr<spdlog::logger>& internal_logger() {
  return _internal_logger;
}
inline std::shared_ptr<spdlog::logger>& client_logger() {
  return _client_logger;
}
}  // namespace log_internal

#define DE_INTERNAL_ERROR(...) \
  ::dawn::log_internal::internal_logger()->error(__VA_ARGS__)
#define DE_INTERNAL_WARN(...) \
  ::dawn::log_internal::internal_logger()->warn(__VA_ARGS__)
#define DE_INTERNAL_INFO(...) \
  ::dawn::log_internal::internal_logger()->info(__VA_ARGS__)
#define DE_INTERNAL_TRACE(...) \
  ::dawn::log_internal::internal_logger()->trace(__VA_ARGS__)
#define DE_INTERNAL_CRITICAL(...) \
  ::dawn::log_internal::internal_logger()->critical(__VA_ARGS__)

#define DE_ERROR(...) ::dawn::log_internal::client_logger()->error(__VA_ARGS__)
#define DE_WARN(...) ::dawn::log_internal::client_logger()->warn(__VA_ARGS__)
#define DE_INFO(...) ::dawn::log_internal::client_logger()->info(__VA_ARGS__)
#define DE_TRACE(...) ::dawn::log_internal::client_logger()->trace(__VA_ARGS__)
#define DE_CRITICAL(...) \
  ::dawn::log_internal::client_logger()->critical(__VA_ARGS__)

}  // namespace dawn
