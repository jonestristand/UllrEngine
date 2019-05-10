#pragma once

#include "ullr/defines.h"
#include "jointTransform.h"

namespace Ullr::Graphics::Animation {

  class KeyFrameList
  {
  public:
    KeyFrameList();
    KeyFrameList(uint32 boneIndex);

    void AddKeyframe(double keyFrameTime, const JointTransform& transform);

    inline double getKeyframeTime(uint32 index) { return this->keyFrameTimes[index]; }
    inline JointTransform getJointTransform(uint32 index) { return index >= this->jointTransforms.size() ? JointTransform() : this->jointTransforms[index]; }
    inline uint32 getKeyframeCount() { return this->frameCount; }
    inline void setBoneIndex(uint32 index) { this->boneIndex = index; }
    
  private:
    uint32 boneIndex;
    uint32 frameCount;
    std::vector<double> keyFrameTimes;
    std::vector<JointTransform> jointTransforms;
  };

}
