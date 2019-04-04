#include "ullrpch.h"

#include "renderCommandQueue.h"

namespace Ullr::Graphics {

  RenderCommandQueue::RenderCommandQueue(uint32 size)
    : cmdCount(0)
  {
    this->cmdQueue = new byte[size];
    this->cmdQueuePtr = this->cmdQueue; // Set current queue pointer to start of memory space
    memset(this->cmdQueue, 0, size);
  }


  RenderCommandQueue::~RenderCommandQueue()
  {
    delete[] this->cmdQueue;
  }

  void* RenderCommandQueue::Allocate(RenderCommandFn fn, uint32 size)
  {
    // Allocate and save function pointer
    *(RenderCommandFn*)this->cmdQueuePtr = fn;
    this->cmdQueuePtr += sizeof(RenderCommandFn);
    ++this->cmdCount;

    // Save data block size
    *(uint32*)this->cmdQueuePtr = size;
    this->cmdQueuePtr += sizeof(uint32);

    // And skip ahead to end of data block for next function
    this->cmdQueuePtr += size;

    return this->cmdQueuePtr - size; // Return start of memory block
  }

  void RenderCommandQueue::Execute()
  {
    UL_CORE_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", this->cmdCount, (this->cmdQueuePtr - this->cmdQueue));

    byte* buffer = this->cmdQueue;

    for (uint32 i = 0; i < this->cmdCount; ++i) {
      RenderCommandFn function = *(RenderCommandFn*)buffer;
      buffer += sizeof(RenderCommandFn);

      uint32 size = *(uint32*)buffer;
      buffer += sizeof(size);

      // Invoke queued function
      function(buffer);

      buffer += size;
    }

    this->cmdQueuePtr = this->cmdQueue; // Reset to beginning for next buffer
    this->cmdCount = 0;
  }

}
