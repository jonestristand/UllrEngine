#pragma once

#include "ullr/defines.h"
#include "ullr/graphics/texture.h"

namespace Ullr::Graphics::OpenGL {
  namespace Command {
    class LoadTexture;
    class UnloadTexture;
    class BindTexture;
  }

  class OpenGLTexture : public Texture
  {
    friend Command::LoadTexture;
    friend Command::UnloadTexture;
    friend Command::BindTexture;

  public:
    OpenGLTexture(const std::string& path);
    OpenGLTexture(const std::string& path, const std::string& type);
    OpenGLTexture(byte* textureData, int32 width, int32 height, const std::string& type);
    virtual ~OpenGLTexture() = default;

    void ReadFile() override;

    void LoadTexture() override;
    void UnloadTexture() override;

    void Bind(uint32 slot) override;
    
    inline GLuint getId() { return this->textureId; }

  private: // methods
    void clearLocalBuffer();

  private: // fields
    GLuint textureId;

    byte* localBuffer;
  };

}
