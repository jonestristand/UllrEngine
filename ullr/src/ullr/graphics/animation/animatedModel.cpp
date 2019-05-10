#include "ullrpch.h"
#include "animatedModel.h"

namespace Ullr::Graphics::Animation {

  AnimatedModel::AnimatedModel()
    : Model(), skeleton(nullptr), jointCount(0)
  { }

  AnimatedModel::AnimatedModel(MeshPtrList& meshes, Joint* rootJoint, uint32 numJoints, std::unique_ptr<Animator>& animator)
    : Model(meshes), skeleton(std::move(rootJoint)), jointCount(numJoints), animator(std::move(animator))
  { }

  AnimatedModel::~AnimatedModel()
  {
    delete this->skeleton;
  }

  void AnimatedModel::Render(Shader& shader)
  {
    // Bind the shader for this model
    shader.Bind();

    auto joints = this->getJointTransforms();

    // TODO: use programlocations instead of strings
    for (int i = 0; i < this->jointCount; i++) {
      shader.SetUniform("jointTransforms[" + std::to_string(i) + "]", joints[i]);
    }

    for (auto& i : this->meshes) {
      i->Render(shader);
    }
  }

  void AnimatedModel::Update(double time)
  {
    this->animator->UpdateJoints(this->skeleton, time);
  }

  std::vector<glm::mat4> AnimatedModel::getJointTransforms()
  {
    std::vector<glm::mat4> jointMatrices;
    jointMatrices.resize(this->jointCount);

    addJointToArray(this->skeleton, jointMatrices);

    return jointMatrices;
  }

  void AnimatedModel::addJointToArray(Joint* joint, std::vector<glm::mat4>& jointTransforms)
  {
    jointTransforms[joint->getIndex()] = joint->getAnimationTransform();
    for (auto child : joint->getChildren())
      addJointToArray(child, jointTransforms);
  }


}
