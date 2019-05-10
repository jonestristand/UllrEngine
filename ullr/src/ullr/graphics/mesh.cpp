#include "ullrpch.h"
#include "mesh.h"

#include "platform/platform.h"
#include "platform/opengl/openglMesh.h"

namespace Ullr::Graphics
{

  MeshPtr Mesh::CreateMesh(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices, const TexturePtrList& textures, bool keepData, RenderType renderType)
  {
    switch (GetGfxPlatform()) {
      case GfxPlatform::OpenGL: return new OpenGL::OpenGLMesh(vertices, indices, textures, keepData, renderType); //return std::make_unique<OpenGL::OpenGLMesh>(vertices, indices, textures, keepData);
      case GfxPlatform::DirectX: UL_CORE_ASSERT(false, "DirectX support not yet implemented");
    }
  }

}
