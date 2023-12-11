#pragma once

#include <spdlog/spdlog.h>

#include "framework/utils/memory/container.h"
#include "framework/utils/singleton/public_singleton.h"
#include "framework/utils/helper.h"
#include "framework/utils/types/types.h"

namespace ante {
//inline std::string HrToString(long hr) {
//    char s_str[64] = {};
//    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<u32>(hr));
//    return std::string(s_str);
//}

class Log : public Singleton<Log> {
  public:
    enum class loglevel : u8 { debug, info, warn, error, fatal };

    Log() noexcept;
    ~Log() noexcept override;

    DELETE_COPY_MOVE(Log);

    template <typename... args> inline void debug(args &&..._args) const noexcept {
        m_logger->debug(std::forward<args>(_args)...);
    }

    template <typename... args> inline void info(args &&..._args) const noexcept {
        m_logger->info(std::forward<args>(_args)...);
    }

    template <typename... args> inline void warn(args &&..._args) const noexcept {
        m_logger->warn(std::forward<args>(_args)...);
    }

    template <typename... args> inline void error(args &&..._args) const noexcept {
        m_logger->error(std::forward<args>(_args)...);
    }
    // template <typename... args> inline void fatal(args &&..._args) const noexcept {
    //     m_logger->fatal(std::forward<args>(_args)...);
    // }
    void SetLogLevel(loglevel level) noexcept;
    // TODO(hyl5): add function name
    // void set_pattern() noexcept {
    //   m_logger->set_pattern("[%H:%M:%S %z] [%^%L%$] [thread %t] %v");
    // }
  private:
    std::shared_ptr<spdlog::logger> m_logger;
};

#define LOG_DEBUG(...) ante::Log::get().debug(__VA_ARGS__);

#define LOG_INFO(...) ante::Log::get().info(__VA_ARGS__);

#define LOG_WARN(...) ante::Log::get().warn(__VA_ARGS__);

#define LOG_ERROR(...) ante::Log::get().error(__VA_ARGS__);

#define LOG_FATAL(...) ante::Log::get().fatal(__VA_ARGS__);

//#define CHECK_VK_RESULT(res) ante::Log::get().CheckVulkanResult(res, __FUNCTION__, __LINE__);
//
//#define CHECK_DX_RESULT(res) ante::Log::get().CheckDXResult(res, __FUNCTION__, __LINE__);

#define SET_LOG_LEVEL(level) ante::Log::get().SetLogLevel(level);

} // namespace ante