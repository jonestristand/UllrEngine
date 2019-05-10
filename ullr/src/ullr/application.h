#pragma once
#include "defines.h"

#include "window.h"
#include "layerStack.h"
#include "graphics/renderManager.h"
#include "core/timeManager.h"

#include "imgui/imguiLayer.h" // TODO: Should not expose ImGuiLayer publicly - sol'n to store unique_pointers in layerStack so it has sole ownership
#include "graphics/camera.h"

#include "events/event.h"
#include "events/applicationEvents.h"
#include "events/keyboardEvents.h"

// Forward declaration for friend class
namespace Ullr::Graphics::Command {
  class RenderImGUI;
}

namespace Ullr {

  class Application
  {
    friend Graphics::Command::RenderImGUI;

  public:
    Application();
    virtual ~Application();
    inline static Application& Get() { return *Application::instance; }

    void Run();

    void OnEvent(Events::Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& getWindow() { return *(this->window); }

    inline double getTime() { return this->time; }
    inline double getFrameTime() { return this->frameTime; }

  private: // methods
    bool OnWindowClosed(Events::WindowClosedEvent& e);
    bool OnWindowResized(Events::WindowResizedEvent& e);
    bool OnKeyPressed(Events::KeyPressedEvent& e);

  protected: // fields
    std::shared_ptr<Graphics::RenderManager> renderManager;
    std::shared_ptr<TimeManager> timeManager;

    Graphics::Camera* currentCamera;

  private: // fields
    std::unique_ptr<Window> window;
    bool running = true;
    double time = 0.0;
    float frameTime = 0.0;

    LayerStack layerStack;
    ImGuiLayer* imguiLayer;

  private: // static fields
    static Application* instance;
  };

  // To be defined by client
  Application* CreateApplication();

}
