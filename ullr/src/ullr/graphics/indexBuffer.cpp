#include "ullrpch.h"
#include "indexBuffer.h"

#include "renderManager.h"
#include "renderMacros.h"

namespace Ullr::Graphics {

  IndexBuffer::IndexBuffer()
    :indexBufferId(0)
  {
    DISPATCH_RENDER_SELF_FN(CreateIBO, {
      glGenBuffers(1, &self->indexBufferId);
    });
  }

  IndexBuffer::~IndexBuffer()
  {
    DISPATCH_RENDER_SELF_FN(DeleteIBO, {
      glDeleteBuffers(1, &self->indexBufferId);
    });
  }

  void IndexBuffer::SetData(void* data, uint32 size)
  {
    DISPATCH_RENDER_SELF_FN2(SetIBOData, data, size, {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferId);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    });
  }

  void IndexBuffer::Bind()
  {
    DISPATCH_RENDER_SELF_FN(BindIBO, {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->indexBufferId);
    });
  }


}
