#include "ullrpch.h"
#include "texture.h"

#include "platform/platform.h"
#include "platform/opengl/openglTexture.h"

namespace Ullr::Graphics
{

  Texture* Texture::CreateTexture(const std::string& filename, const std::string& type)
  {
    switch (GetGfxPlatform())
    {
      case GfxPlatform::OpenGL: return new OpenGL::OpenGLTexture(filename, type);
      case GfxPlatform::DirectX: UL_CORE_ASSERT(false, "DirectX support not yet implemented");
    }
  }

  Texture* Texture::CreateTexture(byte* fileData, int32 width, int32 height, const std::string& type)
  {
    switch (GetGfxPlatform())
    {
    case GfxPlatform::OpenGL: return new OpenGL::OpenGLTexture(fileData, width, height, type);
    case GfxPlatform::DirectX: UL_CORE_ASSERT(false, "DirectX support not yet implemented");
    }
  }

}
