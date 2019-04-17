#pragma once

#include "renderCommand.hpp"
#include "ullr/graphics/renderManager.h"

#include "ullr/defines.h"
#include "ullr/log.h"

#include "glad/glad.h"

#include <string>

namespace Ullr::Graphics::Command
{
  // --------------------------------------------------------------------------
  // -- RenderMesh                                                           --
  // --------------------------------------------------------------------------
  class RenderMesh : Command::RenderCommand {
  public:
    RenderMesh(uint32 vao, uint32 num)
      :vao(vao), num(num)
    {}

    void Execute() override {
      glBindVertexArray(vao);
      glDrawElements(GL_TRIANGLES, num, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      UL_RQ_TRACE("[RenderQueue] -- Command::RenderMesh (vao {0}): {1} faces", vao, num);
    }

  private:
    uint32 vao;
    uint32 num;

  public: //STATIC CREATOR
    static void Dispatch(uint32 vao, uint32 num) {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->SubmitToQueue(sizeof(RenderMesh));
      new (mem) RenderMesh(vao, num);
    }
  };

}
