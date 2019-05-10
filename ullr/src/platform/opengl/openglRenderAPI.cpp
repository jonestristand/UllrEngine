#include "ullrpch.h"
#include "openglRenderAPI.h"

#include "glad/glad.h"

#include "ullr/graphics/command/generalCommands.hpp"

namespace Ullr::Graphics::OpenGL {

  OpenGLRenderAPI::OpenGLRenderAPI()
  { }

  void OpenGLRenderAPI::Init()
  {
    // Enable antialiasing
    glEnable(GL_MULTISAMPLE);

    // Enable backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Enable the depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable alpha transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Enable point rendering
    glEnable(GL_PROGRAM_POINT_SIZE);
  }

  void OpenGLRenderAPI::Shutdown()
  {
  }

  void OpenGLRenderAPI::SetClearColor(float r, float g, float b)
  {
    Command::SetClearColor::Dispatch(r, g, b);
  }

  void OpenGLRenderAPI::ClearBuffer()
  {
    Command::ClearBuffer::Dispatch();
  }

}
