#include "ullrpch.h"

#ifdef ULLR_PLATFORM_WINDOWS

#include <glad/glad.h>

#include "windowsWindow.h"
#include "ullr/events/applicationEvents.h"
#include "ullr/events/keyboardEvents.h"
#include "ullr/events/mouseEvents.h"

#include "ullr/core.h"
#include "ullr/defines.h"

namespace Ullr {


  Window* Window::Create(const WindowProps& props)
  {
    return new WindowsWindow(props);
  }

  WindowsWindow::WindowsWindow(const WindowProps& props)
  {
    this->Init(props);
  }


  WindowsWindow::~WindowsWindow()
  {
    this->Shutdown();
  }

  void WindowsWindow::Init(const WindowProps& props)
  {
    if (!Ullr::Core::StartupSuccess)
      return;

    this->data.Title = props.Title;
    this->data.Width = props.Width;
    this->data.Height = props.Height;

    // Set window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OGL_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OGL_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    UL_CORE_INFO("Creating window '{0}' ({1}, {2})", props.Title, props.Width, props.Height);

    this->primaryMonitor = glfwGetPrimaryMonitor();

    this->baseVideoMode = *(glfwGetVideoMode(this->primaryMonitor));
    UL_CORE_TRACE("Storing underlying OS video mode: {0}x{1}@{2}Hz (r{3}g{4}b{5})",
      this->baseVideoMode.width,
      this->baseVideoMode.height,
      this->baseVideoMode.refreshRate,
      this->baseVideoMode.redBits,
      this->baseVideoMode.greenBits,
      this->baseVideoMode.blueBits);

    this->glfwWindow = glfwCreateWindow((int32)props.Width, (int32)props.Height, props.Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(this->glfwWindow); // OpenGL context created here

    // Change to windowed, fullscreen, or fullscreen borderless
    // Store current information about the window location and size (starting location/size)
    this->oldWindowedParams.Width = props.Width;
    this->oldWindowedParams.Height = props.Height;
    glfwGetWindowPos(this->glfwWindow, &(this->oldWindowedParams.XPos), &(this->oldWindowedParams.YPos));
    this->setWindowMode(props.Mode, 0, 0);

    // Load OpenGL with GLAD
    int32 status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    UL_CORE_ASSERT(status, "Failed to initialize Glad!");

    glfwSetWindowUserPointer(this->glfwWindow, &this->data); // Does this send correct address?
    this->setVSync(props.VSync);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(this->glfwWindow, [](GLFWwindow* window, int32 width, int32 height) {
      windowsWindowData& data = *(windowsWindowData*)glfwGetWindowUserPointer(window);

      data.Width = width;
      data.Height = height;

      Events::WindowResizedEvent e(width, height);
      data.EventCallback(e);
    });

    glfwSetWindowCloseCallback(this->glfwWindow, [](GLFWwindow* window) {
      windowsWindowData& data = *(windowsWindowData*)glfwGetWindowUserPointer(window);

      Events::WindowClosedEvent e;
      data.EventCallback(e);
    });

    glfwSetKeyCallback(this->glfwWindow, [](GLFWwindow* window, int32 key, int32 scancode, int32 action, int32 mods)
    {
      windowsWindowData& data = *(windowsWindowData*)glfwGetWindowUserPointer(window);
      switch (action)
      {
        case GLFW_PRESS: {
          Events::KeyPressedEvent event(key, 0);
          data.EventCallback(event);
          break;
        }

        case GLFW_RELEASE: {
          Events::KeyReleasedEvent event(key);
          data.EventCallback(event);
          break;
        }

        case GLFW_REPEAT: {
          Events::KeyPressedEvent event(key, 1);
          data.EventCallback(event);
          break;
        }
      }
    });

    glfwSetCharCallback(this->glfwWindow, [](GLFWwindow* window, uint32 keycode)
    {
      windowsWindowData& data = *(windowsWindowData*)glfwGetWindowUserPointer(window);

      Events::KeyTypedEvent event(keycode);
      data.EventCallback(event);
    });

    glfwSetMouseButtonCallback(this->glfwWindow, [](GLFWwindow* window, int32 button, int32 action, int32 mods)
    {
      windowsWindowData& data = *(windowsWindowData*)glfwGetWindowUserPointer(window);

      switch (action)
      {
        case GLFW_PRESS: {
          Events::MouseButtonPressedEvent event(button);
          data.EventCallback(event);
          break;
        }

        case GLFW_RELEASE: {
          Events::MouseButtonReleasedEvent event(button);
          data.EventCallback(event);
          break;
        }
      }
    });

    glfwSetScrollCallback(this->glfwWindow, [](GLFWwindow* window, double xOffset, double yOffset)
    {
      windowsWindowData& data = *(windowsWindowData*)glfwGetWindowUserPointer(window);

      Events::MouseScrolledEvent event((float)xOffset, (float)yOffset);
      data.EventCallback(event);
    });

    glfwSetCursorPosCallback(this->glfwWindow, [](GLFWwindow* window, double xPos, double yPos)
    {
      windowsWindowData& data = *(windowsWindowData*)glfwGetWindowUserPointer(window);

      Events::MouseMovedEvent event((float)xPos, (float)yPos);
      data.EventCallback(event);
    });
  }

  void WindowsWindow::Shutdown()
  {
    glfwDestroyWindow(this->glfwWindow);
  }

  void WindowsWindow::Update()
  {
    glfwPollEvents();
    glfwSwapBuffers(this->glfwWindow);
  }

  void WindowsWindow::setVSync(bool enabled)
  {
    if (enabled)
      glfwSwapInterval(1);
    else
      glfwSwapInterval(0);

    this->data.VSync = enabled;
  }

  void WindowsWindow::setWindowMode(const WindowMode & mode, uint32 width = 0, uint32 height = 0)
  {
    if (!this->glfwWindow) // Ensure there is a window to work on
      return;
    if (mode == this->data.Mode) // Nothing to do as this is not a change
      return;

    // If currently windowed, stash the current size and position of the window
    if (this->data.Mode == WindowMode::WINDOWED) {
      this->oldWindowedParams.Width = this->data.Width;
      this->oldWindowedParams.Height = this->data.Height;
      glfwGetWindowPos(this->glfwWindow, &(this->oldWindowedParams.XPos), &(this->oldWindowedParams.YPos));

    }

    GLFWmonitor* monitor = nullptr;

    if (mode == WindowMode::BORDERLESS) {
      // For borderless full screen, the new width and height will be the video mode width and height
      width = this->baseVideoMode.width;
      height = this->baseVideoMode.height;
      monitor = this->primaryMonitor;
    }
    else if (mode == WindowMode::WINDOWED && (width == 0 || height == 0)) {
      // For windowed, use old window height and width if none provided
      width = this->oldWindowedParams.Width;
      height = this->oldWindowedParams.Height;
      // monitor = nullptr;
    }
    else if (mode == WindowMode::FULL_SCREEN) {
      if (width == 0 || height == 0) {
        width = this->data.Width;
        height = this->data.Height;
      }
      monitor = this->primaryMonitor;
    }

    // Width and height will change
    this->data.Width = width;
    this->data.Height = height;

    // Trigger resize event
    if (this->data.EventCallback) {
      Events::WindowResizedEvent e(width, height);
      this->data.EventCallback(e);
    }

    // Record new window mode
    this->data.Mode = mode;

    UL_CORE_INFO("Changing window mode to {0}: [{1}, {2}]", Ullr::WindowModeString[static_cast<int>(mode)], width, height);

    glfwSetWindowMonitor(this->glfwWindow, monitor, this->oldWindowedParams.XPos, this->oldWindowedParams.YPos, width, height, this->baseVideoMode.refreshRate);
  }
}

#endif
