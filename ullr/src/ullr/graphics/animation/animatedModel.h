#pragma once

#include "ullr/graphics/model.h"

#include "joint.h"
#include "keyFrameList.h"
#include "animator.h"
  
namespace Ullr::Graphics::Animation {

  class AnimatedModel : public Model
  {
  public:
    AnimatedModel();
    AnimatedModel(MeshPtrList& meshes, Joint* rootJoint, uint32 numJoints, std::unique_ptr<Animator>& animator);
    virtual ~AnimatedModel();

    void Render(Shader& shader) override;
    void Update(double time) override;

  private: // methods
    std::vector<glm::mat4> getJointTransforms();
    void addJointToArray(Joint* joint, std::vector<glm::mat4>& jointTransforms);

  private: // fields
    Joint* skeleton;
    uint32 jointCount;

    std::unique_ptr<Animator> animator;
  };

}

