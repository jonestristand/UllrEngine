#pragma once

#include "ullr/graphics/renderCommand.h"
#include "ullr/graphics/renderManager.h"

#include "ullr/defines.h"
#include "ullr/log.h"

#include "platform/opengl/openglMesh.h"

#include "glad/glad.h"

#include <string>

namespace Ullr::Graphics::OpenGL::Command
{

  // --------------------------------------------------------------------------
  // -- UploadMesh                                                             --
  // --------------------------------------------------------------------------
  class UploadMesh : RenderCommand {
  public:
    UploadMesh(OpenGLMesh* meshObj)
      :meshObj(meshObj)
    {}

    void Execute() override {
      GLuint vao, vbo, ebo;

      glGenVertexArrays(1, &vao);
      glGenBuffers(1, &vbo);
      glGenBuffers(1, &ebo);

      glBindVertexArray(vao);

      glBindBuffer(GL_ARRAY_BUFFER, vbo);
      glBufferData(GL_ARRAY_BUFFER, meshObj->vertices.size() * sizeof(OpenGLMesh::Vertex), &meshObj->vertices[0], GL_STATIC_DRAW);

      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshObj->indices.size() * sizeof(uint32), &meshObj->indices[0], GL_STATIC_DRAW);

      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OpenGLMesh::Vertex), (void*)0);
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(OpenGLMesh::Vertex), (void*)offsetof(Mesh::Vertex, Normal));
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(OpenGLMesh::Vertex), (void*)offsetof(Mesh::Vertex, TexCoords));
      glEnableVertexAttribArray(3);
      glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(OpenGLMesh::Vertex), (void*)offsetof(Mesh::Vertex, Color));
      glEnableVertexAttribArray(4);
      glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(OpenGLMesh::Vertex), (void*)offsetof(Mesh::Vertex, Tangent));
      glEnableVertexAttribArray(5);
      glVertexAttribIPointer(5, 4, GL_INT, sizeof(OpenGLMesh::Vertex), (void*)offsetof(Mesh::Vertex, BoneIds));
      glEnableVertexAttribArray(6);
      glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(OpenGLMesh::Vertex), (void*)offsetof(Mesh::Vertex, BoneWeights));

      glBindVertexArray(0);

      meshObj->vao = vao;
      meshObj->vbo = vbo;
      meshObj->ebo = ebo;

      if (!meshObj->keepData) {
        meshObj->clearLocalBuffers();
      }

      UL_RQ_TRACE("[RenderQueue] -- Command::UploadMesh (vao {0}, vbo {1}, ebo {2})", vao, vbo, ebo);
    }

  private:
    OpenGLMesh* meshObj;

  public: //STATIC CREATOR
    static void Dispatch(OpenGLMesh* meshObj) {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->AllocateInQueue(sizeof(UploadMesh));
      new (mem) UploadMesh(meshObj);
    }
  };

  // --------------------------------------------------------------------------
  // -- RenderMesh                                                           --
  // --------------------------------------------------------------------------
  class RenderMesh : RenderCommand {
  public:
    RenderMesh(OpenGLMesh* meshObj)
      : meshObj(meshObj)
    {}

    void Execute() override {
      int32 renderType = GL_TRIANGLES;

      switch (meshObj->renderType) {
      case Mesh::RenderType::FACES:
        renderType = GL_TRIANGLES;
        break;

      case Mesh::RenderType::POINTS:
        renderType = GL_POINTS;
        break;
      }

      glBindVertexArray(meshObj->vao);
      glDrawElements(renderType, meshObj->numFaces, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      UL_RQ_TRACE("[RenderQueue] -- Command::RenderMesh (vao {0}): {1} faces", meshObj->vao, meshObj->numFaces);
    }

  private:
    OpenGLMesh* meshObj;

  public: //STATIC CREATOR
    static void Dispatch(OpenGLMesh* meshObj) {
      auto mem = ::Ullr::Graphics::RenderManager::Get()->AllocateInQueue(sizeof(RenderMesh));
      new (mem) RenderMesh(meshObj);
    }
  };

}
