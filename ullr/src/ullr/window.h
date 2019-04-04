#pragma once

#include "defines.h"

#include "events/event.h"

namespace Ullr {

  enum WindowMode
  {
    WINDOWED = 0,
    FULL_SCREEN = 1,
    BORDERLESS = 2
  };
  static const char* WindowModeString[] = { "WINDOWED", "FULL_SCREEN", "BORDERLESS" };

  struct WindowProps
  {
    std::string Title;
    uint32 Width;
    uint32 Height;

    bool VSync;

    WindowMode Mode;

    WindowProps(const std::string& title = "Ullr Engine", uint32 width = 1280, uint32 height = 720, WindowMode mode = WindowMode::WINDOWED, bool vsync = true)
      :Title(title), Width(width), Height(height), Mode(mode), VSync(vsync)
    {}
  };

  class ULLR_API Window
  {
  public:
    using EventCallbackFn = std::function<void(Events::Event&)>;

    virtual ~Window() {}

    virtual void Update() = 0;

    virtual uint32 getWidth() const = 0;
    virtual uint32 getHeight() const = 0;
    virtual bool getIsVsync() const = 0;
    virtual WindowMode getWindowMode() const = 0;

    virtual void setEventCallback(const EventCallbackFn& callback) = 0;
    virtual void setVSync(bool enabled) = 0;
    virtual void setWindowMode(const WindowMode& Mode, uint32 width = 0, uint32 height = 0) = 0;

    virtual void* GetNativeWindow() const = 0;

    // Static method, implemented per platform
    static Window* Create(const WindowProps& props = WindowProps());

  };

}
