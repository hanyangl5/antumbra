#pragma once

#include <spdlog/spdlog.h>

#include <framework/01core/memory/memory.h>
#include <framework/01core/singleton/public_singleton.h>
#include <framework/01core/utils/utils.h>

enum VkResult;

namespace ant {
inline ant::str HrToString(long hr) {
    char s_str[64] = {};
    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<u32>(hr));
    return ant::str(s_str);
}

class Log : public Singleton<Log> {
  public:
    enum class loglevel : u8 { debug, info, warn, error, fatal };

  public:
    Log() noexcept;
    ~Log() noexcept;

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
    template <typename... args> inline void fatal(args &&..._args) const noexcept {
        // m_logger->fatal(std::forward<args>(_args)...);
    }
    //void CheckVulkanResult(VkResult _res, const char *func_name, int line) const noexcept;

    //void CheckDXResult(long hr, const char *func_name, int line) const noexcept;

    void SetLogLevel(loglevel level) noexcept;

  private:
    std::shared_ptr<spdlog::logger> m_logger;
};

#define LOG_DEBUG(...) ant::Log::get().debug("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_INFO(...) ant::Log::get().info("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_WARN(...) ant::Log::get().warn("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_ERROR(...) ant::Log::get().error("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_FATAL(...) ant::Log::get().fatal("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

//#define CHECK_VK_RESULT(res) ant::Log::get().CheckVulkanResult(res, __FUNCTION__, __LINE__);
//
//#define CHECK_DX_RESULT(res) ant::Log::get().CheckDXResult(res, __FUNCTION__, __LINE__);

#define SET_LOG_LEVEL(level) ant::Log::get().SetLogLevel(level);

} // namespace ant