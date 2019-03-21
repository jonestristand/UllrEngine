#pragma once

#include "defines.h"

namespace Ullr {
  
  class ULLR_API Core
  {
  public:
    static void Startup();
    static void Shutdown();

    static void PlatformStartup();
    static void PlatformShutdown();

    static bool StartupSuccess;

  private:
    Core();
    virtual ~Core();
  };

}

