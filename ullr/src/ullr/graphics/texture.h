#pragma once

#include "ullr/defines.h"
#include "glad/glad.h"

namespace Ullr::Graphics {

  class Texture
  {
  public:
    Texture(const std::string path);
    virtual ~Texture();

    void Bind(uint32 slot = 0);
    void Unbind();

    inline uint32 getWidth() { return this->width; }
    inline uint32 getHeight() { return this->height; }

  private:
    GLuint textureId;
    std::string filePath;
    byte* localBuffer;
    int32 width, height, nChannels;
  };

}
