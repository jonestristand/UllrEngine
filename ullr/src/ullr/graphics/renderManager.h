#pragma once

#include "ullr/core/manager.h"

#include "renderCommandQueue.h"

namespace Ullr::Graphics {

  class RenderManager : public Ullr::Manager
  {
  public:
    typedef void(*RenderCommandFn)(void*);

    RenderManager();
    virtual ~RenderManager();

    static std::shared_ptr<RenderManager> Create();

    void Init() override;
    void Shutdown() override;

    void* Submit(RenderCommandFn fn, uint32 size);
    void Render();

    void SetClearColor(float r, float g, float b);
    void ClearBuffer();

    void DrawIndexed(uint32 count);

    inline static std::shared_ptr<RenderManager> Get() { return RenderManager::instance; }

  private:
    static std::shared_ptr <RenderManager> instance;

    RenderCommandQueue commandQueue;
  };

}
