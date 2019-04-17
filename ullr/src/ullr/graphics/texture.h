#pragma once

#include "ullr/defines.h"

namespace Ullr::Graphics {

  class Texture
  {
  public:
    Texture(const std::string& path);
    Texture(const std::string& path, const std::string& type);
    virtual ~Texture();

    void LoadTexture();
    void UnloadTexture();

    void Bind(uint32 slot = 0);
    //void Unbind();

    inline uint32 getWidth() { return this->width; }
    inline uint32 getHeight() { return this->height; }
    inline std::string getType() { return this->type; }
    inline uint32 getId() { return this->textureId; }

  private:
    uint32 textureId;
    std::string filePath;
    std::string type;
    byte* localBuffer;
    int32 width, height, nChannels;
  };

}
