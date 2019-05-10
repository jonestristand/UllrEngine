#pragma once

#include "ullr/graphics/renderCommand.h"
#include "ullr/graphics/renderManager.h"

#include "ullr/defines.h"
#include "ullr/log.h"

#include "platform/opengl/openglTexture.h"

#include "glad/glad.h"

#include <string>

namespace Ullr::Graphics::OpenGL::Command
{
  // --------------------------------------------------------------------------
  // -- LoadTexture                                                          --
  // --------------------------------------------------------------------------
  class LoadTexture : RenderCommand {
  public:
    LoadTexture(OpenGLTexture* texObj)
      :texObj(texObj)
    {}

    void Execute() override {
      GLuint textureId;
      glGenTextures(1, &textureId);
      glBindTexture(GL_TEXTURE_2D, textureId);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texObj->getWidth(), texObj->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texObj->localBuffer);
      glGenerateMipmap(GL_TEXTURE_2D);

      glBindTexture(GL_TEXTURE_2D, 0);

      texObj->textureId = textureId;
      texObj->clearLocalBuffer();

      UL_RQ_TRACE("[RenderQueue] -- Command::LoadTexture (file '{0}' ==> textureid {1})", texObj->filePath, textureId);
    }

  private:
    OpenGLTexture* texObj;

  public: //STATIC CREATOR
    static void Dispatch(OpenGLTexture* texObj) {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->AllocateInQueue(sizeof(LoadTexture));
      new (mem) LoadTexture(texObj);
    }
  };

  // --------------------------------------------------------------------------
  // -- UnloadTexture                                                        --
  // --------------------------------------------------------------------------
  class UnloadTexture : RenderCommand {
  public:
    UnloadTexture(OpenGLTexture* texObj)
      :texObj(texObj)
    {}

    void Execute() override {
      glDeleteTextures(1, &texObj->textureId);

      UL_RQ_TRACE("[RenderQueue] -- Command::UnloadTexture (texture {0})", texObj->textureId);
    }

  private:
    OpenGLTexture* texObj;

  public: //STATIC CREATOR
    static void Dispatch(OpenGLTexture* texObj) {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->AllocateInQueue(sizeof(UnloadTexture));
      new (mem) UnloadTexture(texObj);
    }
  };

  // --------------------------------------------------------------------------
  // -- BindTexture                                                        --
  // --------------------------------------------------------------------------
  class BindTexture : RenderCommand {
  public:
    BindTexture(OpenGLTexture* texObj, uint32 slot)
      :texObj(texObj), slot(slot)
    {}

    void Execute() override {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, texObj->textureId);

      UL_RQ_TRACE("[RenderQueue] -- Command::BindTexture (texture {0}): slot {1}", texObj->textureId, slot);
    }

  private:
    OpenGLTexture* texObj;
    uint32 slot;

  public: //STATIC CREATOR
    static void Dispatch(OpenGLTexture* texObj, uint32 slot) {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->AllocateInQueue(sizeof(BindTexture));
      new (mem) BindTexture(texObj, slot);
    }
  };

}
