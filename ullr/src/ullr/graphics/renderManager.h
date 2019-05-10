#pragma once

#include "ullr/core/manager.h"

#include "renderAPI.h"
#include "renderCommandQueue.h"

namespace Ullr::Graphics {

  class RenderManager : public Ullr::Manager
  {
  public:
    RenderManager(uint32 cmdQueueSize);
    virtual ~RenderManager() = default;

    void Init() override;
    void Shutdown() override;

    void* AllocateInQueue(uint32 size);
    void Render();

    //virtual GfxPlatform getType() = 0;

    void SetClearColor(float r, float g, float b);
    void ClearBuffer();

  protected: // fields
    RenderCommandQueue commandQueue;
    std::shared_ptr<RenderAPI> renderApi;

  public: // STATIC
    static std::shared_ptr<RenderManager> Create(uint32 cmdQueueSize);
    inline static std::shared_ptr<RenderManager> Get() { return RenderManager::instance; }

  private: // STATIC
    static std::shared_ptr<RenderManager> instance;
  };

}
