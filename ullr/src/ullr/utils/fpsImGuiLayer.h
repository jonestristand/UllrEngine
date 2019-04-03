#pragma once

#include "ullr/layer.h"

#include "imgui.h"

namespace Ullr::Utils {

  class FpsImGuiLayer : public Layer
  {
  public:
    FpsImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;


    void OnImGuiRender() override;

  private:
    ImGuiIO* io;
    bool showFPS;
  };

}
