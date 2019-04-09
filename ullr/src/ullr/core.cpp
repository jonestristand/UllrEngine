#include "ullrpch.h"
#include "core.h"
#include "log.h"
#include "graphics/renderManager.h"

namespace Ullr {

  bool Core::StartupSuccess = true;

  /* static */void Core::Startup()
  {
    Ullr::Log::Init(spdlog::level::trace, spdlog::level::warn); // Start logging first

    UL_CORE_TRACE("=== Ullr Engine v" VERSION_STRING " Core startup ===============");

    Ullr::Core::PlatformStartup();

    // Create renderer
//    Graphics::RenderManager::Init();
  }

  /* static */void Core::Shutdown()
  {
    Ullr::Core::PlatformShutdown();

    UL_CORE_TRACE("=== Ullr Engine Core shutdown ======================");

    // Destroy renderer
//    Graphics::RenderManager::Shutdown();
  }

  Core::Core()
  {}


  Core::~Core()
  {}

}
