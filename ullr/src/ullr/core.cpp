#include "ullrpch.h"
#include "core.h"
#include "log.h"
#include "graphics/renderer.h"

namespace Ullr {

  bool Core::StartupSuccess = true;

  /* static */void Core::Startup()
  {
    Ullr::Log::Init(spdlog::level::warn, spdlog::level::warn); // Start logging first

    UL_CORE_TRACE("=== Ullr Engine v" VERSION_STRING " Core startup ===============");

    Ullr::Core::PlatformStartup();

    // Create renderer
    Graphics::Renderer::Init();
  }

  /* static */void Core::Shutdown()
  {
    Ullr::Core::PlatformShutdown();

    UL_CORE_TRACE("=== Ullr Engine Core shutdown ======================");

    // Destroy renderer
    Graphics::Renderer::Shutdown();
  }

  Core::Core()
  {}


  Core::~Core()
  {}

}
