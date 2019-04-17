#include "ullrpch.h"
#include "texture.h"

#include "stb_image.h"
#include "glad/glad.h"

//#include "renderManager.h"
#include "command/textureCommands.hpp"

namespace Ullr::Graphics {

  Texture::Texture(const std::string& path, const std::string& type)
    : textureId(0), filePath(path), type(type), localBuffer(nullptr), width(0), height(0), nChannels(0)
  { }

  Texture::Texture(const std::string& path)
    : Texture(path, "diffuse")
  { }

  Texture::~Texture()
  { }

  void Texture::LoadTexture()
  {
    auto self = this;
    // TODO: Debug this
    //DISPATCH_RENDER_SELF_FN(CreateTexture, {
      stbi_set_flip_vertically_on_load(true);
      self->localBuffer = stbi_load(self->filePath.c_str(), &self->width, &self->height, &self->nChannels, 4);

      glGenTextures(1, &self->textureId);
      glBindTexture(GL_TEXTURE_2D, self->textureId);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, self->localBuffer);
      glGenerateMipmap(GL_TEXTURE_2D);

      glBindTexture(GL_TEXTURE_2D, 0);
      if (self->localBuffer)
        stbi_image_free(self->localBuffer);
    //});
  }

  void Texture::UnloadTexture()
  {
    Command::UnloadTexture::Dispatch(this->textureId);
  }

  void Texture::Bind(uint32 slot)
  {
    Command::BindTexture::Dispatch(this->textureId, slot);
  }

}
