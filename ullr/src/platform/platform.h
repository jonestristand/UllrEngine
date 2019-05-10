#pragma once

namespace Ullr
{

  enum OSPlatform {
    Windows = 0x01,
    Linux = 0x02,
    MacOS = 0x04,
  };

  enum class GfxPlatform {
    OpenGL = 0x01,
    DirectX = 0x02
    // Vulkan = 0x04 // not supported
  };

  // TODO: Fix this, will end up abstracted into a class
  static OSPlatform GetOSPlatform() {

#if defined ULLR_PLATFORM_WINDOWS
    return OSPlatform::Windows;
#elif defined ULLR_PLATFORM_LINUX
    return OSPlatform::Linux;
#elif defined ULLR_PLATFORM_MACOS
    return OSPlatform::MacOS;
#endif

  }

  static GfxPlatform GetGfxPlatform() {
    return GfxPlatform::OpenGL;
  }

}
