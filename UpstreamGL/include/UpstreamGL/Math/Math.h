#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace upgl {
struct Math {
  static glm::vec3 GetTranslation(const glm::mat4 &matrix);
  static glm::vec3 GetScale(const glm::mat4 &matrix);
  static glm::mat3 GetRotationMatrix(const glm::mat4 &matrix);
  static glm::quat GetRotationQuat(const glm::mat4 &matrix);
};
} // namespace upgl