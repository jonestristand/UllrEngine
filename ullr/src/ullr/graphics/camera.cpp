#include "ullrpch.h"
#include "camera.h"

namespace Ullr::Graphics {

  // TODO: This should not implement any one particular camera and should instead be the base class for different kinds of camera (e.g. FPS, follow, etc.)
  Camera::Camera(const glm::vec3& position, const glm::vec3& direction)
    : position(position), direction(direction), up(0.0f, 1.0f, 0.0f), yaw(90.0f), pitch(0.0f)
  { }


  Camera::~Camera()
  {
  }

  void Camera::MoveTo(const glm::vec3& newPosition)
  {
    this->position = newPosition;
  }

  void Camera::Translate(float magnitude, const glm::vec3& direction)
  {
    this->position += (magnitude * glm::normalize(direction));
  }

  // TODO: Yaw and Pitch should be one function so direction vector only recaculates once
  void Camera::Yaw(float amount)
  {
    this->yaw += amount;

    recalcDirection();
  }

  void Camera::Pitch(float amount)
  {
    this->pitch += amount;

    if (this->pitch > 89.0f)
      this->pitch = 89.0f;
    if (this->pitch < -89.0f)
      this->pitch = -89.0f;
      
    recalcDirection();
  }

  void Camera::recalcDirection()
  {
    glm::vec3 newDir;
    newDir.x = cosf(glm::radians(this->yaw)) * cosf(glm::radians(this->pitch));
    newDir.y = sinf(glm::radians(this->pitch));
    newDir.z = -sinf(glm::radians(this->yaw)) * cosf(glm::radians(this->pitch));

    this->direction = glm::normalize(newDir);
  }

  glm::mat4 Camera::getViewMatrix()
  {
    auto target = this->position + this->direction;
    return glm::lookAt(this->position, this->position + this->direction, glm::vec3(0.0f, 1.0f, 0.0f));
  }

}
