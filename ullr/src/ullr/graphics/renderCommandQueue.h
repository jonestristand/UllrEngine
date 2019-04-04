#pragma once

#include "ullr/defines.h"

namespace Ullr::Graphics {

  class RenderCommandQueue
  {
  public:
    typedef void(*RenderCommandFn)(void*);

    RenderCommandQueue(uint32 size);
    ~RenderCommandQueue();

    void* Allocate(RenderCommandFn fn, uint32 size);
    void Execute();

  private:
    byte* cmdQueue;
    byte* cmdQueuePtr;
    uint32 cmdCount;

  };

}
