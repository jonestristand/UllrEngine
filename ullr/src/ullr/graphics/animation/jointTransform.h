#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Ullr::Graphics::Animation
{

  struct JointTransform
  {
    glm::vec3 Translation;
    glm::quat Rotation;
  };

}
