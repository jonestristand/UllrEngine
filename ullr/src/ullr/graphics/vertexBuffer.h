#pragma once

#include "glad/glad.h"

namespace Ullr::Graphics {

  class VertexBuffer
  {
  public:
    VertexBuffer();
    ~VertexBuffer();

    void SetData(void* data, uint32 size);
    void Bind();

  private:
    GLuint vertexBufferId;
  };

}
