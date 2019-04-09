#include "ullrpch.h"
#include "mesh.h"

#include "renderManager.h"
#include "renderMacros.h"

namespace Ullr::Graphics {

  Mesh::Mesh(float vertices[], uint32 vertexsize, void* indices, uint32 indexsize, uint32 vertexCount)
    : vertexArrayId(0), vertexCount(vertexCount)
  {  
    DISPATCH_RENDER_SELF_FN(CreateNewMesh, {
      // Create VAO
      glCreateVertexArrays(1, &self->vertexArrayId);
      glBindVertexArray(self->vertexArrayId);
    });
    
    this->vb = new VertexBuffer();
    this->ib = new IndexBuffer();
    
    this->vb->SetData(vertices, vertexsize);
    this->ib->SetData(indices, indexsize);
  }
  
  Mesh::~Mesh()
  {
    delete this->vb;
    delete this->ib;
  }

  void Mesh::Render()
  {
    DISPATCH_RENDER_SELF_FN(RenderMesh, {
      glBindVertexArray(self->vertexArrayId);
      glDrawElements(GL_TRIANGLES, self->vertexCount, GL_UNSIGNED_INT, nullptr);
    });
  }

}
