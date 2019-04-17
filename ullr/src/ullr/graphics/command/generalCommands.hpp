#pragma once

#include "renderCommand.hpp"
#include "ullr/graphics/renderManager.h"

#include "ullr/defines.h"
#include "ullr/log.h"

#include "glad/glad.h"

#include <string>

namespace Ullr::Graphics::Command
{
  // --------------------------------------------------------------------------
  // -- SetClearColor                                                        --
  // --------------------------------------------------------------------------
  class SetClearColor : Command::RenderCommand {
  public:
    SetClearColor(float r, float g, float b)
      :r(r), g(g), b(b)
    {}

    void Execute() override {
      glClearColor(r, g, b, 1.0f);
      UL_RQ_TRACE("[RenderQueue] -- Command::SetClearColor: <{0}, {1}, {2}>", r, g, b);
    }

  private:
    float r;
    float g;
    float b;

  public: //STATIC CREATOR
    static void Dispatch(float r, float g, float b) {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->SubmitToQueue(sizeof(SetClearColor));
      new (mem) SetClearColor(r, g, b);
    }
  };

  // --------------------------------------------------------------------------
  // -- ClearBuffer                                                          --
  // --------------------------------------------------------------------------
  class ClearBuffer : Command::RenderCommand {
  public:
    ClearBuffer()
    {}

    void Execute() override {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      UL_RQ_TRACE("[RenderQueue] -- Command::ClearBuffer");
    }

  public: //STATIC CREATOR
    static void Dispatch() {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->SubmitToQueue(sizeof(ClearBuffer));
      new (mem) ClearBuffer();
    }
  };

}
