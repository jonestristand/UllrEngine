#include "ullrpch.h"
#include "renderer.h"

#include "glad/glad.h" // TODO: Move out to platform-specific renderer

#include "rendererMacros.h"

namespace Ullr::Graphics {

  Renderer* Renderer::instance;

  Renderer::Renderer()
    : commandQueue(10 * 1024 * 1024 /* 10 MB */)
  { }

  Renderer::~Renderer()
  { }

  void Renderer::Init()
  {
    UL_CORE_ASSERT(!instance, "Renderer already created!");
    Renderer::instance = new Renderer();
  }

  void Renderer::Shutdown()
  {
    delete Renderer::instance;
  }

  void* Renderer::Submit(RenderCommandFn fn, uint32 size)
  {
    return Renderer::instance->commandQueue.Allocate(fn, size);
  }

  void Renderer::Render()
  {
    Renderer::instance->commandQueue.Execute();
  }


  void Renderer::SetClearColor(float r, float g, float b)
  {
    DISPATCH_RENDER_FN3(SetClearColor, r, g, b, {
      glClearColor(r, g, b, 1.0f);
      });

  }

  void Renderer::ClearBuffer()
  {
    DISPATCH_RENDER_FN(ClearBuffer, {
      glClear(GL_COLOR_BUFFER_BIT);
      });
  }

}
