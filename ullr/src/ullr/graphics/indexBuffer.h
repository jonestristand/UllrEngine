#pragma once

#include "glad/glad.h"

namespace Ullr::Graphics {

  class IndexBuffer
  {
  public:
    IndexBuffer();
    ~IndexBuffer();

    void SetData(void* data, uint32 size);
    void Bind();

  private:
    GLuint indexBufferId;
  };

}
