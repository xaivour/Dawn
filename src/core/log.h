#pragma once

#include <memory>
#include <spdlog/spdlog.h>

namespace Dawn
{
    class Log
    {
    public:
        static void initLog();
        
        static std::shared_ptr<spdlog::logger>& getInternalLogger() { return internalLogger; }
        static std::shared_ptr<spdlog::logger>& getClientLogger() { return clientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> internalLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;
    };
    
#define DAWN_INTERNAL_ERROR(...)    ::Dawn::Log::getInternalLogger()->error(__VA_ARGS__)
#define DAWN_INTERNAL_WARN(...)     ::Dawn::Log::getInternalLogger()->warn(__VA_ARGS__)
#define DAWN_INTERNAL_INFO(...)     ::Dawn::Log::getInternalLogger()->info(__VA_ARGS__)
#define DAWN_INTERNAL_TRACE(...)    ::Dawn::Log::getInternalLogger()->trace(__VA_ARGS__)
#define DAWN_INTERNAL_CRITICAL(...) ::Dawn::Log::getInternalLogger()->critical(__VA_ARGS__)
    
#define DAWN_ERROR(...)     ::Dawn::Log::getClientLogger()->error(__VA_ARGS__)
#define DAWN_WARN(...)      ::Dawn::Log::getClientLogger()->warn(__VA_ARGS__)
#define DAWN_INFO(...)      ::Dawn::Log::getClientLogger()->info(__VA_ARGS__)
#define DAWN_TRACE(...)     ::Dawn::Log::getClientLogger()->trace(__VA_ARGS__)
#define DAWN_CRITICAL(...)  ::Dawn::Log::getClientLogger()->critical(__VA_ARGS__)
    
#define DAWN_INTERNAL_ASSERT(x, ...) { if(!(x)) { DAWN_INTERNAL_ERROR("Assertion Failed ({0}): {1}", #x, __VA_ARGS__); }}
#define DAWN_ASSERT(x, ...) { if(!(x)) { DAWN_ERROR("Assertion Failed ({0}): {1}", #x, __VA_ARGS__); }}
    
}
