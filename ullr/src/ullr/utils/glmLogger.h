#pragma once

// Include GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <ostream>

// TODO: GLM stream operators for logging should go in separate file
inline std::ostream& operator<<(std::ostream& os, const glm::vec3& vec)
{
  return os << "glm::vec3 [" << vec.x << ", " << vec.y << ", " << vec.z << "]";
}
inline std::ostream& operator<<(std::ostream& os, const glm::vec4& vec)
{
  return os << "glm::vec4 [" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << "]";
}
inline std::ostream& operator<<(std::ostream& os, const glm::mat4& mat)
{
  return os << "glm::mat4 [...]";
}
