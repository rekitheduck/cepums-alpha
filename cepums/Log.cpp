#include "Log.h"
#include "cepumspch.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Cepums {

std::shared_ptr<spdlog::logger> Log::s_logger;

void Log::init() {
    spdlog::set_pattern("%^[%T] %v%$");
    s_logger = spdlog::stdout_color_mt("Cepums-Alpha");
    s_logger->set_level(spdlog::level::trace);
}
} // namespace Cepums
