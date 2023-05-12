#pragma once

#include <spdlog/spdlog.h>

#include <framework/01core/singleton/public_singleton.h>
#include <framework/01core/memory/memory.h>
#include <windows.h>

enum VkResult;

namespace ant
{
    inline ant::str HrToString(HRESULT hr)
    {
        char s_str[64] = {};
        sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
        return ant::str(s_str);
    }

    class Log : public Singleton<Log>
    {
      public:
        enum loglevel : u8 { debug, info, warn, error, fatal };

      public:
        Log() noexcept;
        ~Log() noexcept;

        template<typename... args> inline void Debug(args &&..._args) const noexcept
        {
            m_logger->debug(std::forward<args>(_args)...);
        }

        template<typename... args> inline void Info(args &&..._args) const noexcept
        {
            m_logger->info(std::forward<args>(_args)...);
        }

        template<typename... args> inline void Warn(args &&..._args) const noexcept
        {
            m_logger->warn(std::forward<args>(_args)...);
        }

        template<typename... args> inline void Error(args &&..._args) const noexcept
        {
            m_logger->error(std::forward<args>(_args)...);
        }
        template<typename... args> inline void Fatal(args &&..._args) const noexcept
        {
            // m_logger->fatal(std::forward<args>(_args)...);
        }
        void CheckVulkanResult(VkResult _res, const char *func_name, int line) const noexcept;

        void CheckDXResult(HRESULT hr, const char *func_name, int line) const noexcept;

      private:
        std::shared_ptr<spdlog::logger> m_logger;
    };

#define LOG_DEBUG(...) ant::Log::get().Debug("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_INFO(...) ant::Log::get().Info("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_WARN(...) ant::Log::get().Warn("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_ERROR(...) ant::Log::get().Error("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define LOG_FATAL(...) ant::Log::get().Fatal("[" + ant::str(__FUNCTION__) + "] " + __VA_ARGS__);

#define CHECK_VK_RESULT(res) ant::Log::get().CheckVulkanResult(res, __FUNCTION__, __LINE__);

#define CHECK_DX_RESULT(res) ant::Log::get().CheckDXResult(res, __FUNCTION__, __LINE__);

}// namespace ant