#include "ullrpch.h"
#include "joint.h"

namespace Ullr::Graphics::Animation {

  Joint::Joint(uint32 index, const std::string& name, const glm::mat4& inverseBindTransform)
    :index(index), name(name), children(), animationTransform(1.0f), inverseBindTransform(inverseBindTransform)
  { }

  Joint::~Joint()
  {
    // Delete all child joints
    for (auto child : this->children)
      delete child;
  }
}
