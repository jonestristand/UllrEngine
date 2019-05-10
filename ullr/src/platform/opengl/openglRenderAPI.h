#pragma once

#include "ullr/graphics/renderApi.h"

#include "ullr/graphics/renderCommandQueue.h"

namespace Ullr::Graphics::OpenGL {

  class OpenGLRenderAPI : public RenderAPI
  {
  public:
    OpenGLRenderAPI();
    ~OpenGLRenderAPI() = default;

    void Init() override;
    void Shutdown() override;

    void SetClearColor(float r, float g, float b);
    void ClearBuffer();
  };

}
