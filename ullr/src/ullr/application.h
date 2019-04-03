#pragma once
#include "defines.h"

#include "window.h"
#include "layerStack.h"
#include "imgui/imguiLayer.h" // TODO: Should not expose ImGuiLayer publicly - sol'n to store unique_pointers in layerStack so it has sole ownership

#include "events/event.h"
#include "events/applicationEvents.h"
#include "events/keyboardEvents.h"

namespace Ullr {

  class ULLR_API Application
  {
  public:
    Application();
    virtual ~Application();
    inline static Application& Get() { return *Application::instance; }

    void Run();

    void OnEvent(Events::Event& e);

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* layer);

    inline Window& getWindow() { return *(this->window); }
  private: // methods
    bool OnWindowClosed(Events::WindowClosedEvent& e);
    bool OnKeyPressed(Events::KeyPressedEvent& e);

  private: // fields
    std::unique_ptr<Window> window;
    bool running = true;

    LayerStack layerStack;
    ImGuiLayer* imguiLayer;

  private: // static fields
    static Application* instance;
  };

  // To be defined by client
  Application* CreateApplication();

}
