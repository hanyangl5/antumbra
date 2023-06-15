#include "log.h"

// standard libraries

// third party libraries
#ifdef WIN32

#include <Windows.h>
#endif // WIN32

#include <spdlog/sinks/stdout_color_sinks.h> // or "../stdout_sinks.h" if no colors needed

// project headers

namespace ant {

Log::Log() noexcept {
    m_logger = spdlog::stdout_color_mt("antlog");
    spdlog::set_default_logger(m_logger);
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif // !NDEBUG
}

Log::~Log() noexcept {
    m_logger->flush();
    spdlog::drop_all();
}

//void Log::CheckVulkanResult(VkResult _res, const char *func_name, int line) const noexcept {
//    if (_res != VK_SUCCESS) {
//        m_logger->error("[function: {}], [line: {}], vulkan result checking failed", func_name, line);
//    }
//    assert(_res == VK_SUCCESS);
//}
//
//void Log::CheckDXResult(long hr, const char *func_name, int line) const noexcept {
//    if (FAILED(hr)) {
//        m_logger->error("[function: {}], [line: {}], directx result checking failed: HRESULT {:08X}", func_name, line,
//                        static_cast<u32>(hr));
//    }
//}

void Log::SetLogLevel(loglevel level) noexcept {
    switch (level) {
    case ant::Log::loglevel::debug:
        spdlog::set_level(spdlog::level::debug);
        break;
    case ant::Log::loglevel::info:
        spdlog::set_level(spdlog::level::info);
        break;
    case ant::Log::loglevel::warn:
        spdlog::set_level(spdlog::level::warn);
        break;
    case ant::Log::loglevel::error:
        spdlog::set_level(spdlog::level::err);
        break;
    case ant::Log::loglevel::fatal:

        spdlog::set_level(spdlog::level::critical);
        break;
    default:
        break;
    }
}

} // namespace ant