#pragma once

#include "texture.h"
#include "mesh.h"
#include "shader.h"

namespace Ullr::Graphics {

  class Model
  {
  public:
    Model();
    Model(MeshPtrList& meshes);
    virtual ~Model() = default;

    virtual void Render(Shader& shader);
    virtual void Update(double time);

    inline void setTranslation(const glm::vec3& translation) { this->translation = translation; }
    inline void setRotation(const glm::vec3& axis, float magnitude) { this->rotation = glm::angleAxis(magnitude, axis); }
    inline void setScale(const glm::vec3& scale) { this->scale = scale; }

    glm::mat4 getModelMatrix();

    uint32 getFaceCount();

  protected: // fields
    MeshPtrList meshes;

    glm::vec3 translation;
    glm::quat rotation;
    glm::vec3 scale;
  };

}

