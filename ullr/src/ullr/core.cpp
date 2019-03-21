#include "ullrpch.h"
#include "core.h"
#include "log.h"

namespace Ullr {

  bool Core::StartupSuccess = true;

  /* static */void Core::Startup()
  {
    Ullr::Log::Init(); // Start logging first

    UL_CORE_TRACE("=== Ullr Engine v" VERSION_STRING " Core startup ===============");

    Ullr::Core::PlatformStartup();
  }

  /* static */void Core::Shutdown()
  {
    Ullr::Core::PlatformShutdown();

    UL_CORE_TRACE("=== Ullr Engine Core shutdown ======================");
  }

  Core::Core()
  {}


  Core::~Core()
  {}

}