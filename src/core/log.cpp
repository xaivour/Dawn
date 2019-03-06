#include <spdlog/sinks/stdout_color_sinks.h>
#include "log.h"

namespace Dawn
{
    std::shared_ptr<spdlog::logger> Log::s_internalLogger{};
    std::shared_ptr<spdlog::logger> Log::s_clientLogger{};
    
    void Log::initLog()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        s_internalLogger = spdlog::stderr_color_mt("DAWN");
        s_clientLogger = spdlog::stderr_color_mt("APP");
    }
}
