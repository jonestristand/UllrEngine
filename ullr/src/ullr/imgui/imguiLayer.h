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

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;

    void Begin();
    void End();

  private:
    float time = 0.0;
    ImGuiIO* io;
    bool showFPS = true;
  };

}
