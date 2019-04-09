#include "ullrpch.h"
#include "renderManager.h"

#include "glad/glad.h" // TODO: Move out to platform-specific renderer

#include "renderMacros.h"

namespace Ullr::Graphics {

  std::shared_ptr<RenderManager> RenderManager::instance;

  RenderManager::RenderManager()
    : commandQueue(1 * 1024 * 1024 /* 1 MB */)
  { }

  RenderManager::~RenderManager()
  {
    std::cout << "RenderManager Destructor" << std::endl;
  }

  std::shared_ptr<RenderManager> RenderManager::Create()
  {
    UL_CORE_ASSERT(!instance, "Renderer already created!");
    RenderManager::instance = std::make_shared<RenderManager>();

    return RenderManager::instance;
  }

  void RenderManager::Init()
  {
    // Responsible for picking correct RenderApi
  }

  void RenderManager::Shutdown()
  {
    std::cout << "RenderManager::Shutdown" << std::endl;
  }

  void* RenderManager::Submit(RenderCommandFn fn, uint32 size)
  {
    return this->commandQueue.Allocate(fn, size);
  }

  void RenderManager::Render()
  {
    RenderManager::instance->commandQueue.Execute();
  }

  void RenderManager::SetClearColor(float r, float g, float b)
  {
    DISPATCH_RENDER_FN3(SetClearColor, r, g, b, {
      glClearColor(r, g, b, 1.0f);
      });

  }

  void RenderManager::ClearBuffer()
  {
    DISPATCH_RENDER_FN(ClearBuffer, {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    });
  }

  void RenderManager::DrawIndexed(uint32 count)
  {
    DISPATCH_RENDER_FN1(DrawIndexed, count, {
      glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    });
  }

}
