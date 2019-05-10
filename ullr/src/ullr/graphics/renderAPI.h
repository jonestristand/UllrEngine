#pragma once

namespace Ullr::Graphics {

  class RenderAPI
  {
  public:
    virtual ~RenderAPI() = default;

    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void SetClearColor(float r, float g, float b) = 0;
    virtual void ClearBuffer() = 0;

  };

}

