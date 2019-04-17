#pragma once

#include "ullr/layer.h"

#include "ullr/events/event.h"
#include "ullr/events/applicationEvents.h"
#include "ullr/events/keyboardEvents.h"
#include "ullr/events/mouseEvents.h"

struct ImGuiIO; // Placeholder for ImGuiIO struct defined in imgui library

namespace Ullr {

  class ImGuiLayer : public Layer
  {
  public:
    ImGuiLayer();
    virtual ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    void Begin();
    void End();

  private:
    ImGuiIO* io;
    bool showFPS = true;
  };

}
