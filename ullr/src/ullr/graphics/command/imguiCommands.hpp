#pragma once

#include "ullr/graphics/renderCommand.h"
#include "ullr/graphics/renderManager.h"

#include "ullr/defines.h"
#include "ullr/log.h"
#include "ullr/application.h"

#include <string>

namespace Ullr::Graphics::Command
{
  // --------------------------------------------------------------------------
  // -- RenderImGUI                                                          --
  // --------------------------------------------------------------------------
  class RenderImGUI : RenderCommand
  {
  public:
    RenderImGUI(Application* self)
      :self(self)
    {}

    void Execute() override {
      // Render ImGui interface
      self->imguiLayer->Begin();

      for (Layer* layer : self->layerStack)
        layer->OnImGuiRender();

      self->imguiLayer->End();

      UL_RQ_TRACE("[RenderQueue] -- Command::RenderImGUI");
    }

  private:
    Application* self;

  public: //STATIC CREATOR
    static void Dispatch(Application* self) {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->AllocateInQueue(sizeof(RenderImGUI));
      new (mem) RenderImGUI(self);
    }
  };

}
