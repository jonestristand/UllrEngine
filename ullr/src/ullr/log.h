#pragma once

#include "defines.h"

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Ullr {

  class  Log
  {
  public:
    ULLR_API static void Init(spdlog::level::level_enum coreLevel = spdlog::level::trace, spdlog::level::level_enum clientLevel = spdlog::level::trace);

    ULLR_API inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return coreLogger; }
    ULLR_API inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return clientLogger; }

  private:
    static std::shared_ptr<spdlog::logger> coreLogger;
    static std::shared_ptr<spdlog::logger> clientLogger;
  };

}

#ifndef ULLR_DIST

  // Core logger
  #define UL_CORE_FATAL(...)   ::Ullr::Log::GetCoreLogger()->critical(__VA_ARGS__)
  #define UL_CORE_ERROR(...)   ::Ullr::Log::GetCoreLogger()->error(__VA_ARGS__)
  #define UL_CORE_WARN(...)    ::Ullr::Log::GetCoreLogger()->warn(__VA_ARGS__)
  #define UL_CORE_INFO(...)    ::Ullr::Log::GetCoreLogger()->info(__VA_ARGS__)
  #define UL_CORE_TRACE(...)   ::Ullr::Log::GetCoreLogger()->trace(__VA_ARGS__)

  // Client logger
  #define UL_FATAL(...)        ::Ullr::Log::GetClientLogger()->critical(__VA_ARGS__)
  #define UL_ERROR(...)        ::Ullr::Log::GetClientLogger()->error(__VA_ARGS__)
  #define UL_WARN(...)         ::Ullr::Log::GetClientLogger()->warn(__VA_ARGS__)
  #define UL_INFO(...)         ::Ullr::Log::GetClientLogger()->info(__VA_ARGS__)
  #define UL_TRACE(...)        ::Ullr::Log::GetClientLogger()->trace(__VA_ARGS__)

#else

  // Remove all logging methods in distribution mode
  #define UL_CORE_FATAL(...)
  #define UL_CORE_ERROR(...)
  #define UL_CORE_WARN(...)
  #define UL_CORE_INFO(...)
  #define UL_CORE_TRACE(...)

  #define UL_FATAL(...)
  #define UL_ERROR(...)
  #define UL_WARN(...)
  #define UL_INFO(...)
  #define UL_TRACE(...)

#endif
