#include "ullrpch.h"
#include "log.h"

namespace Ullr {

  std::shared_ptr<spdlog::logger> Log::coreLogger;
  std::shared_ptr<spdlog::logger> Log::clientLogger;

  void Log::Init(spdlog::level::level_enum coreLevel, spdlog::level::level_enum clientLevel)
  {
    spdlog::set_pattern("%^[%H:%M:%S.%f] %n: %v%$");

    coreLogger = spdlog::stdout_color_mt("ULLR");
    coreLogger->set_level(coreLevel);
    coreLogger->info("Core logger started (level: {0})", spdlog::level::to_string_view(coreLevel));

    clientLogger = spdlog::stdout_color_mt("CLIENT"); 
    clientLogger->set_level(clientLevel);
    clientLogger->info("Client logger started (level: {0})", spdlog::level::to_string_view(clientLevel));
  }

}