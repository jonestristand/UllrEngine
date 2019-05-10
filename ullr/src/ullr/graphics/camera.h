#pragma once

#include "glm/fwd.hpp"

namespace Ullr::Graphics {

  class Camera
  {
  public:
    Camera(const glm::vec3& position, const glm::vec3& target);
    virtual ~Camera();

    virtual glm::mat4 getViewMatrix();
    virtual inline glm::vec3 getPosition() { return this->position; };

    virtual inline glm::vec3 getFront() { return glm::normalize(this->direction); }
    virtual inline glm::vec3 getBack() { return -this->getFront(); }
    virtual inline glm::vec3 getLeft() { return glm::normalize(glm::cross(this->up, this->direction)); }
    virtual inline glm::vec3 getRight() { return -this->getLeft(); }
    virtual inline glm::vec3 getUp() { return glm::normalize(this->up); }
    virtual inline glm::vec3 getDown() { return -this->getUp(); }

    virtual void MoveTo(const glm::vec3& newPosition);
    virtual void Translate(float magnitude, const glm::vec3& direction);

    // TODO: fix this
    virtual void Yaw(float amount);
    virtual void Pitch(float amount);

  private:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 up;
    float yaw;
    float pitch;

    void recalcDirection();
  };

}
