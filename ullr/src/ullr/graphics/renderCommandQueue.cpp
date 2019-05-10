#include "ullrpch.h"
#include "renderCommandQueue.h"

#include "renderCommand.h"

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

  void* RenderCommandQueue::Allocate(uint32 size)
  {
    // Save functor data block size
    *(uint32*)this->cmdQueuePtr = size;
    this->cmdQueuePtr += sizeof(uint32);

    // And skip ahead to end of data block for next function
    this->cmdQueuePtr += size;

    // Increment the command counter
    ++this->cmdCount;

    return this->cmdQueuePtr - size; // Return start of memory block
  }

  void RenderCommandQueue::Execute()
  {
    UL_RQ_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", this->cmdCount, (this->cmdQueuePtr - this->cmdQueue));

    byte* buffer = this->cmdQueue;

    for (uint32 i = 0; i < this->cmdCount; ++i) {
      uint32 size = *(uint32*)buffer;
      buffer += sizeof(size);

      // Retrieve command functor
      RenderCommand* cmd = (RenderCommand*)buffer;

      // Invoke queued command
      cmd->Execute();

      // Cleanup command
      cmd->~RenderCommand();

      // Move to next command
      buffer += size;
    }

    this->cmdQueuePtr = this->cmdQueue; // Reset to beginning for next buffer
    this->cmdCount = 0;
  }

}
