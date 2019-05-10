#pragma once

#include "ullr/defines.h"

namespace Ullr::Graphics {

  class Texture
  {
  public:
    Texture(const std::string& type, int32 width, int32 height)
      : Texture("", type)
    {
      this->width = width;
      this->height = height;
    }
    Texture(const std::string& path, const std::string& type)
      : filePath(path), type(type), width(0), height(0), numChannels(0)
    {}

    virtual ~Texture() { }

    virtual void ReadFile() = 0;

    virtual void LoadTexture() = 0;
    virtual void UnloadTexture() = 0;

    virtual void Bind(uint32 slot = 0) = 0;

    inline uint32 getWidth() { return this->width; }
    inline uint32 getHeight() { return this->height; }
    inline std::string getType() { return this->type; }

    inline std::string getToString() const { return this->filePath; }

  protected:
    std::string filePath;
    std::string type;
    int32 width, height, numChannels;

  public: // STATIC (implement per-platform)
    static Texture* CreateTexture(const std::string& filename, const std::string& type = "diffuse");
    static Texture* CreateTexture(byte* fileData, int32 width, int32 height, const std::string& type = "diffuse");
  };

  inline std::ostream& operator<<(std::ostream& os, const Texture& t)
  {
    return os << t.getToString();
  }

  typedef std::shared_ptr<Texture> TexturePtr;
  typedef std::vector<TexturePtr> TexturePtrList;
}
