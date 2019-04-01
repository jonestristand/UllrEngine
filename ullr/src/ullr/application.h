#pragma once
#include "defines.h"

#include "window.h"
#include "layerStack.h"

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

    inline Window& GetWindow() { return *(this->window); }
  private: // methods
    bool OnWindowClosed(Events::WindowClosedEvent& e);
    bool OnKeyPressed(Events::KeyPressedEvent& e);

  private: // fields
    std::unique_ptr<Window> window;
    bool running = true;

    LayerStack layerStack;

  private: // static fields
    static Application* instance;
  };

  // To be defined by client
  Application* CreateApplication();

}

