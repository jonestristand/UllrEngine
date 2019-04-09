#include "ullrpch.h"
#include "vertexBuffer.h"

#include "renderManager.h"
#include "renderMacros.h"

namespace Ullr::Graphics {

  VertexBuffer::VertexBuffer()
    :vertexBufferId(0)
  {
    DISPATCH_RENDER_SELF_FN(CreateVBO, {
      glGenBuffers(1, &self->vertexBufferId);
    });
  }


  VertexBuffer::~VertexBuffer()
  {
    DISPATCH_RENDER_SELF_FN(DeleteDVBO, {
      glDeleteBuffers(1, &self->vertexBufferId);
    });
  }

  void Ullr::Graphics::VertexBuffer::SetData(void* data, uint32 size)
  {
    DISPATCH_RENDER_SELF_FN2(SetVBOData, data, size, {
      glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferId);
      glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

      
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
      glEnableVertexAttribArray(0);
      
      //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
      //glEnableVertexAttribArray(1);
      
      glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);

    });
  }

  void Ullr::Graphics::VertexBuffer::Bind()
  {
    DISPATCH_RENDER_SELF_FN(BindVBO, {
      glBindBuffer(GL_ARRAY_BUFFER, self->vertexBufferId);
    });
  }


}
