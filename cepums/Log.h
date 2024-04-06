#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

#include "Core.h"

// Engine log macros
#define LOG_DEBUG(...) ::Cepums::Log::getLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::Cepums::Log::getLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Cepums::Log::getLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Cepums::Log::getLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Cepums::Log::getLogger()->critical(__VA_ARGS__)

namespace Cepums {

class Log {
   public:
    static void init();
    inline static std::shared_ptr<spdlog::logger>& getLogger() {
        if (!s_logger) {
            abort();
        };
        return s_logger;
    }

   private:
    static std::shared_ptr<spdlog::logger> s_logger;
};
} // namespace Cepums
