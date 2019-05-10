#pragma once

#include "ullr/graphics/mesh.h"
#include "ullr/graphics/texture.h"
#include "ullr/graphics/shader.h"

namespace Ullr::Graphics::OpenGL {
  namespace Command {
    class UploadMesh;
    class RenderMesh;
  }

  class OpenGLMesh : public Mesh
  {
    friend Command::UploadMesh;
    friend Command::RenderMesh;

  public:
    OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const TexturePtrList& textures, bool keepData, Mesh::RenderType renderType);

    void UploadMesh() override;
    void Render(Ullr::Graphics::Shader& shader) override;

  private: // methods
    void clearLocalBuffers();

  private: // fields
    uint32 vao;
    uint32 vbo;
    uint32 ebo;
  };

}

