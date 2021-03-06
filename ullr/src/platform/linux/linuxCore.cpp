#include "ullrpch.h"

#ifdef ULLR_PLATFORM_LINUX

#include "ullr/core.h"
#include "ullr/log.h"

#include <GLFW/glfw3.h>

namespace Ullr {

  static bool gGLFWInitialized = false;

  void Core::PlatformStartup() {
    UL_CORE_TRACE("Platform-specific startup: Linux");

    if (!gGLFWInitialized)
    {
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
    UL_CORE_TRACE("Platform-specific shutdown: Linux");

    if (gGLFWInitialized) {
      glfwTerminate();
      UL_CORE_TRACE("GLFW terminated");
    }
  }

}

#endif
