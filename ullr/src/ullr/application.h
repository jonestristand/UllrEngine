#pragma once
#include "defines.h"

#include "window.h"
#include "events/event.h"
#include "events/applicationEvents.h"
#include "events/keyboardEvents.h"

namespace Ullr {
  
  class ULLR_API Application
  {
  public:
    Application();
    virtual ~Application();

    void Run();
    
    void OnEvent(Events::Event& e);

  private: // methods
    bool OnWindowClosed(Events::WindowClosedEvent& e);
    bool OnKeyPressed(Events::KeyPressedEvent& e);

  private: // fields
    std::unique_ptr<Window> window;
    bool running = true;
  };

  // To be defined by client
  Application* CreateApplication();

}

