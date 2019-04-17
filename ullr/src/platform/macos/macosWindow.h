#pragma once
#include "ullr/defines.h"
#include "ullr/window.h"

#include "GLFW/glfw3.h"

namespace Ullr {

  class MacOSWindow : public Window
  {
  public:
    MacOSWindow(const WindowProps& props);
    virtual ~MacOSWindow();

    void Update() override;

    inline unsigned int32 getWidth() const override { return this->data.Width; }
    inline unsigned int32 getHeight() const override { return this->data.Height; }
    inline bool getIsVsync() const override { return this->data.VSync; }
    inline WindowMode getWindowMode() const override { return this->data.Mode; }

    inline void setEventCallback(const EventCallbackFn& callback) override { this->data.EventCallback = callback; }
    void setVSync(bool enabled) override;
    void setWindowMode(const WindowMode& mode, uint32 width, uint32 height) override;

  private: // Methods
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

  private: // Fields
    GLFWwindow* glfwWindow;
    GLFWmonitor* primaryMonitor; // Stores a reference to the primary monitor
    GLFWvidmode baseVideoMode; // Stores the underlying video mode being used by the OS

    struct macosWindowData {
      std::string Title;
      uint32 Width, Height;
      bool VSync;
      WindowMode Mode;

      EventCallbackFn EventCallback;
    };
    macosWindowData data;

    struct windowedParams {
      uint32 Width, Height;
      int32 XPos, YPos;
    };
    windowedParams oldWindowedParams;
  };

}
