#include "ullrpch.h"
#include "shader.h"

#include "platform/platform.h"
#include "platform/opengl/openglShader.h"

namespace Ullr::Graphics
{

  Shader* Shader::CreateShader(const std::string& filename)
  {
    switch (GetGfxPlatform()) {
      case GfxPlatform::OpenGL: return new OpenGL::OpenGLShader(filename);
      case GfxPlatform::DirectX: UL_CORE_ASSERT(false, "DirectX support not yet implemented");
    }
  }

}
