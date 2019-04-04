#pragma once

#include "renderCommandQueue.h"

namespace Ullr::Graphics {

  class Renderer
  {
  public:
    typedef void(*RenderCommandFn)(void*);

    Renderer();
    ~Renderer();

    static void Init();
    static void Shutdown();

    static void SetClearColor(float r, float g, float b);
    static void ClearBuffer();

    inline static Renderer& Get() { return *Renderer::instance; }

    static void* Submit(RenderCommandFn fn, uint32 size);

    static void Render();

  private:
    static Renderer* instance;

    RenderCommandQueue commandQueue;
  };

}
