#pragma once

#include "ullr/defines.h"

namespace Ullr::Graphics {

  class RenderCommandQueue
  {
  public:
    RenderCommandQueue(uint32 size);
    ~RenderCommandQueue();

    void* Allocate(uint32 size);
    void Execute();

  private:
    byte* cmdQueue;
    byte* cmdQueuePtr;
    uint32 cmdCount;

  };

}
