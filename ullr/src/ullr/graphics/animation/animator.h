#pragma once

#include "keyFrameList.h"
#include "joint.h"
#include "jointTransform.h"

namespace Ullr::Graphics::Animation {

  class Animator
  {
  public:
    Animator(double duration, const std::vector<KeyFrameList>& keyFrames);
    ~Animator() = default;

    void UpdateJoints(Joint* rootJoint, double time);

  private: // methods
    void applyPoseToJoints(Joint* joint, double animTime, glm::mat4 parentTransform);
    glm::mat4 interpolate(const JointTransform& matA, const JointTransform& matB, float amount);

  private: // fields
    double duration;
    std::vector<KeyFrameList> keyFrameList;
  };

}
