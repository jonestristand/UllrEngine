#pragma once

#include "ullr/defines.h"

namespace Ullr::Graphics::Animation {

  class Joint
  {
  public:
    Joint(uint32 index, const std::string& name, const glm::mat4& localBindTransform); // Not sure if ASSIMP loads this in model space or not
    ~Joint();

    inline void AddChild(Joint* childJoint) { this->children.push_back(childJoint); }

    inline glm::mat4 getAnimationTransform() { return this->animationTransform; }
    inline void setAnimationTransform(glm::mat4& animTransform) { this->animationTransform = animTransform; }
    inline glm::mat4 getInverseBindTransform() { return this->inverseBindTransform; }
    inline uint32 getIndex() { return this->index; }
    inline std::vector<Joint*>& getChildren() { return this->children; }

  private: // fields
    std::vector<Joint*> children;

    uint32 index;
    std::string name;

    glm::mat4 animationTransform; // transform in model space for pose
    glm::mat4 inverseBindTransform; // model space, bind = original transform of the joint
  };

}
