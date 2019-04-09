#pragma once

#include "vertexBuffer.h"
#include "indexBuffer.h"

#include "glad/glad.h"

namespace Ullr::Graphics {

  class Mesh
  {
  public:
    Mesh(float vertices[], uint32 vertexsize, void* indices, uint32 indexsize, uint32 vertexCount);
    ~Mesh();

    void Render();

  private:
    VertexBuffer* vb;
    IndexBuffer* ib;
    GLuint vertexArrayId;
    GLuint vertexCount;
  };

}

