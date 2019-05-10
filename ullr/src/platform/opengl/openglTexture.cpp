#include "ullrpch.h"
#include "openglTexture.h"

#include "stb_image.h"
#include "glad/glad.h"

#include "command/textureCommands.hpp"

namespace Ullr::Graphics::OpenGL {

  OpenGLTexture::OpenGLTexture(const std::string& path, const std::string& type)
    : Texture(path, type), textureId(0), localBuffer(nullptr)
  { }

  OpenGLTexture::OpenGLTexture(byte * textureData, int32 width, int32 height, const std::string & type)
    : Texture(type, width, height), textureId(0), localBuffer(textureData)
  { }

  OpenGLTexture::OpenGLTexture(const std::string& path)
    : OpenGLTexture(path, "diffuse")
  { }

  void OpenGLTexture::ReadFile()
  {
    stbi_set_flip_vertically_on_load(false); // Make sure not to both flip the texture and flip the UV coords in model loading
    this->localBuffer = stbi_load(this->filePath.c_str(), &this->width, &this->height, &this->numChannels, 4);
  }

  void OpenGLTexture::LoadTexture()
  {
    Command::LoadTexture::Dispatch(this);
  }

  void OpenGLTexture::UnloadTexture()
  {
    Command::UnloadTexture::Dispatch(this);
  }

  void OpenGLTexture::Bind(uint32 slot)
  {
    Command::BindTexture::Dispatch(this, slot);
  }

  // ==== Private Methods =====================================================

  void OpenGLTexture::clearLocalBuffer()
  {
    if (this->localBuffer)
      stbi_image_free(this->localBuffer);
  }
}
