#include "ullrpch.h"

#ifdef ULLR_PLATFORM_WINDOWS

#include "ullr/core.h"
#include "ullr/log.h"

#include <GLFW/glfw3.h>

namespace Ullr {

  static bool gGLFWInitialized = false;

  void Core::PlatformStartup() {
    UL_CORE_TRACE("Platform-specific startup: Windows");

    if (!gGLFWInitialized)
    {
      // TODO: glfwTerminate on system shutdown
      int32 success = glfwInit();
      if (!success) {
        UL_CORE_FATAL("Could not initialize GLFW!");
        Core::StartupSuccess = false;
      }
      else
        UL_CORE_TRACE("GLFW initialized");

      gGLFWInitialized = true;
    }

  }

  void Core::PlatformShutdown() {
    UL_CORE_TRACE("Platform-specific shutdown: Windows");

    if (gGLFWInitialized) {
      glfwTerminate();
      UL_CORE_TRACE("GLFW terminated");
    }
  }

}

#endif