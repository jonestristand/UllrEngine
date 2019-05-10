#include "ullrpch.h"
#include "model.h"

#include "texture.h"

namespace Ullr::Graphics {

  Model::Model()
    : translation(0.0, 0.0, 0.0), rotation(0.0, 0.0, 0.0, 0.0), scale(1.0, 1.0, 1.0)
  { }

  Model::Model(MeshPtrList& meshes)
    : Model()
  {
    this->meshes = meshes;
  }

  glm::mat4 Model::getModelMatrix()
  {
    glm::mat4 final = glm::translate(glm::mat4(1.0), this->translation) * glm::toMat4(this->rotation) * glm::scale(glm::mat4(1.0), this->scale);
    return final;
  }

  uint32 Model::getFaceCount()
  {
    uint32 total = 0;
    for (auto& mesh : this->meshes)
      total += mesh->getFaceCount();

    return total;
  }

  void Model::Render(Shader& shader)
  {
    // Bind the shader for this model
    shader.Bind();

    for (auto& i : this->meshes)
      i->Render(shader);
  }

  void Model::Update(double time)
  {}
}
