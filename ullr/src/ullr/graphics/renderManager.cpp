#include "ullrpch.h"
#include "renderManager.h"

#include "platform/platform.h"
#include "platform/opengl/openglRenderAPI.h"

namespace Ullr::Graphics {

  RenderManager::RenderManager(uint32 cmdQueueSize)
    : commandQueue(cmdQueueSize /* in Bytes */)
  {
    switch (GetGfxPlatform())
    {
    case GfxPlatform::OpenGL: this->renderApi = std::make_shared<OpenGL::OpenGLRenderAPI>(); break;
    case GfxPlatform::DirectX: UL_CORE_ASSERT(false, "DirectX support not yet implemented"); break;
    }
  }

  void RenderManager::Init()
  {
    this->renderApi->Init();
  }

  void RenderManager::Shutdown()
  {
    this->renderApi->Shutdown();
  }


  void* RenderManager::AllocateInQueue(uint32 size)
  {
    return this->commandQueue.Allocate(size);
  }

  void RenderManager::Render()
  {
    RenderManager::instance->commandQueue.Execute();
  }

  void RenderManager::SetClearColor(float r, float g, float b)
  {
    this->renderApi->SetClearColor(r, g, b);
  }

  void RenderManager::ClearBuffer()
  {
    this->renderApi->ClearBuffer();
  }

  // Static
  std::shared_ptr<RenderManager> RenderManager::instance = nullptr;

  std::shared_ptr<RenderManager> RenderManager::Create(uint32 cmdQueueSize)
  {
    RenderManager::instance = std::make_shared<RenderManager>(cmdQueueSize);

    return RenderManager::instance;
  }
}
