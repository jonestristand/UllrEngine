#pragma once

#include "ullr/layer.h"

#include "ullr/events/event.h"
#include "ullr/events/applicationEvents.h"
#include "ullr/events/keyboardEvents.h"
#include "ullr/events/mouseEvents.h"

struct ImGuiIO; // Placeholder for ImGuiIO struct defined in imgui library

namespace Ullr {

  class ULLR_API ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();
    virtual ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    void OnUpdate();
    void OnEvent(Events::Event& event);

  private:
    bool OnMouseButtonPressedEvent(Events::MouseButtonPressedEvent& e);
    bool OnMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& e);
    bool OnMouseMovedEvent(Events::MouseMovedEvent& e);
    bool OnMouseScrolledEvent(Events::MouseScrolledEvent& e);

    bool OnKeyTypedEvent(Events::KeyTypedEvent& e);
    bool OnKeyPressedEvent(Events::KeyPressedEvent& e);
    bool OnKeyReleasedEvent(Events::KeyReleasedEvent& e);

    bool OnWindowResizedEvent(Events::WindowResizedEvent& e);

  private:
    float time = 0.0;
    ImGuiIO* io;
    bool showFPS = true;
  };

}
