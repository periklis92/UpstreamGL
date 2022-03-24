#include <UpstreamGL/Math/Math.h>

namespace upgl {
glm::vec3 Math::GetTranslation(const glm::mat4 &matrix) {
  glm::vec3 position = matrix[3];
  return position;
}

glm::vec3 Math::GetScale(const glm::mat4 &matrix) {
  float sx = glm::length(matrix[0]);
  float sy = glm::length(matrix[1]);
  float sz = glm::length(matrix[2]);
  return glm::vec3(sx, sy, sz);
}

glm::mat3 Math::GetRotationMatrix(const glm::mat4 &matrix) {
  glm::vec3 scale = GetScale(matrix);
  glm::mat3 rot = glm::mat3(0.f);
  rot[0].x = matrix[0].x / scale.x;
  rot[0].y = matrix[0].y / scale.x;
  rot[0].z = matrix[0].z / scale.x;
  rot[1].x = matrix[1].x / scale.y;
  rot[1].y = matrix[1].y / scale.y;
  rot[1].z = matrix[1].z / scale.y;
  rot[2].x = matrix[2].x / scale.z;
  rot[2].y = matrix[2].y / scale.z;
  rot[2].z = matrix[2].z / scale.z;

  return rot;
}

glm::quat Math::GetRotationQuat(const glm::mat4 &matrix) {
  return glm::toQuat(GetRotationMatrix(matrix));
}
} // namespace upgl