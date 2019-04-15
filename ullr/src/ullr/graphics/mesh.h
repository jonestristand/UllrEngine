#pragma once

#include "texture.h"
#include "shader.h"

namespace Ullr::Graphics {

  class Mesh
  {
  public:
    struct Vertex {
      glm::vec3 Position;
      glm::vec3 Normal;
      glm::vec2 TexCoords;
    };

  public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const std::vector<Texture>& textures);
    ~Mesh();

    void Render(Ullr::Graphics::Shader& shader);

  private:
    std::vector<Vertex>   vertices;
    std::vector<uint32>   indices;
    std::vector<Texture>  textures;

    uint32 vao;
    uint32 vbo;
    uint32 ebo;
  };

}

