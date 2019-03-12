#include <spdlog/sinks/stdout_color_sinks.h>
#include "log.h"

namespace Dawn
{
    std::shared_ptr<spdlog::logger> Log::internalLogger{};
    std::shared_ptr<spdlog::logger> Log::clientLogger{};
    
    void Log::initLog()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        internalLogger = spdlog::stderr_color_mt("DAWN");
        clientLogger = spdlog::stderr_color_mt("APP");
    }
}
