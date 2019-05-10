#include "ullrpch.h"
#include "keyFrameList.h"

namespace Ullr::Graphics::Animation {
  KeyFrameList::KeyFrameList()
    : keyFrameTimes(), jointTransforms(), boneIndex(0), frameCount(0)
  { }

  KeyFrameList::KeyFrameList(uint32 boneIndex)
    : keyFrameTimes(), jointTransforms(), boneIndex(boneIndex), frameCount(0)
  { }

  void KeyFrameList::AddKeyframe(double keyFrameTime, const JointTransform& transform)
  {
    this->keyFrameTimes.push_back(keyFrameTime);
    this->jointTransforms.push_back(transform);
    frameCount++;
  }
}
