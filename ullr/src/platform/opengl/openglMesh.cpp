#include "ullrpch.h"
#include "openglMesh.h"

#include "command/meshCommands.hpp"

#include "glad/glad.h"

namespace Ullr::Graphics::OpenGL {

  OpenGLMesh::OpenGLMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const TexturePtrList& textures, bool keepData, Mesh::RenderType renderType)
    : Mesh(vertices, indices, textures, keepData, renderType), vao(0), vbo(0), ebo(0)
  { }

  void OpenGLMesh::UploadMesh()
  {
    Command::UploadMesh::Dispatch(this);
  }

  void OpenGLMesh::clearLocalBuffers()
  {
    // Swap empty vectors into the vertices and indices buffers to empty and set their capacity to 0
    std::vector<Mesh::Vertex>().swap(this->vertices);
    std::vector<uint32>().swap(this->indices);
  }

  void OpenGLMesh::Render(Ullr::Graphics::Shader& shader)
  {
    int32 diffuseN = 1, specularN = 1, normalN = 1;
    for (int32 i = 0; i < this->textures.size(); i++) {

      std::string num, name = this->textures[i]->getType();

      if (name == "diffuse")
        num = std::to_string(diffuseN++);
      else if (name == "specular")
        num = std::to_string(specularN++);
      else if (name == "normal")
        num = std::to_string(normalN++);

      this->textures[i]->Bind(i); // Bind the texture to slot i
      shader.SetUniform("material." + name + num, i); // Pass slot i to the shader material struct
    }

    // Add actual render command
    Command::RenderMesh::Dispatch(this);
  }

}
