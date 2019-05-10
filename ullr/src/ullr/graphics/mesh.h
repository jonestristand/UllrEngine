#pragma once

#include "texture.h"
#include "shader.h"

namespace Ullr::Graphics {

  class Mesh; // Forward define for typedef
  typedef Mesh* MeshPtr; // Mesh can only be owned by one model
  typedef std::vector<MeshPtr> MeshPtrList;

  class Mesh
  {
  public:
    struct Vertex {
      glm::vec3 Position;
      glm::vec3 Normal;
      glm::vec2 TexCoords;
      glm::vec3 Color;
      glm::vec3 Tangent;
      glm::ivec4 BoneIds;
      glm::vec4 BoneWeights;
    };

    enum class RenderType {
      FACES,
      POINTS
    };

  public:
    // TODO: constructors should be protected so that objects aren't create
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const TexturePtrList& textures, bool keepData, RenderType renderType)
      : vertices(vertices), indices(indices), textures(textures), numFaces(indices.size()), keepData(keepData), renderType(renderType)
    {}

    virtual void UploadMesh() = 0;
    virtual void Render(Ullr::Graphics::Shader& shader) = 0;

    inline uint32 getFaceCount() { return this->numFaces; }

  protected:
    std::vector<Vertex>   vertices;
    std::vector<uint32>   indices;
    TexturePtrList        textures;

    uint32 numFaces;
    RenderType renderType;

    bool keepData; // whether to keep data in class after it is uploaded to GPU

  public: // STATIC (implement per-platform)
    static Mesh* CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const TexturePtrList& textures, bool keepData = false, RenderType renderType = RenderType::FACES);
  };

}

