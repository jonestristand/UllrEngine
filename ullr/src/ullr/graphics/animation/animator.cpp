#include "ullrpch.h"
#include "animator.h"

#include "glm/gtx/matrix_interpolation.hpp"

namespace Ullr::Graphics::Animation {

  Animator::Animator(double duration, const std::vector<KeyFrameList>& keyFrameList)
    : duration(duration), keyFrameList(keyFrameList)
  {
  }

  void Animator::UpdateJoints(Joint* rootJoint, double time)
  {
    auto animTime = fmod(time / 1.0, this->duration); // Time in the animation loop

    this->applyPoseToJoints(rootJoint, animTime, glm::mat4(1.0f));
  }

  glm::mat4 Animator::interpolate(const JointTransform& matA, const JointTransform& matB, float amount)
  {
    auto finalTranslation = glm::mix(matA.Translation, matB.Translation, amount);
    auto finalRotation = glm::slerp(matA.Rotation, matB.Rotation, amount);

    glm::mat4 finalPoseTransform = glm::translate(glm::mat4(1.0f), finalTranslation) * glm::toMat4(finalRotation);

    return finalPoseTransform;
  }

  void Animator::applyPoseToJoints(Joint* joint, double animTime, glm::mat4 parentTransform)
  {
    // Get keyframes for current joint
    auto* currentKeyFrameList = &this->keyFrameList[joint->getIndex()];
    auto numKeyframes = currentKeyFrameList->getKeyframeCount();
    uint32 keyFrameA = 0, keyFrameB = 0;
    double timeA = 0.0, timeB = 0.0;

    for (int i = 0; i < numKeyframes; i++) {
      if (animTime > currentKeyFrameList->getKeyframeTime(i)) {
        timeA = currentKeyFrameList->getKeyframeTime(i);
        keyFrameA = i;
      }
    }
    keyFrameB = keyFrameA + 1;
    if(keyFrameB >= currentKeyFrameList->getKeyframeCount()) {
      keyFrameB = 0;
      timeB = this->duration;
    }
    else {
      timeB = currentKeyFrameList->getKeyframeTime(keyFrameB);
    }
    float interpolateAmount = (animTime - timeA) / (timeB - timeA);

    auto currentLocalTransform = this->interpolate(currentKeyFrameList->getJointTransform(keyFrameA), currentKeyFrameList->getJointTransform(keyFrameB), interpolateAmount);
    auto currentTransform = parentTransform * currentLocalTransform;

    for (Joint* child : joint->getChildren()) {
      applyPoseToJoints(child, animTime, currentTransform);
    }

    currentTransform = currentTransform * joint->getInverseBindTransform();

    joint->setAnimationTransform(currentTransform);
  }
}
