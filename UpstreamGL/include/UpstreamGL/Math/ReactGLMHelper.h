#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace reactphysics3d {
class Transform;
struct Vector3;
struct Quaternion;
}; // namespace reactphysics3d

namespace upgl {
class ReactGLMHelper {
public:
  static ::reactphysics3d::Transform ConvertFromGLM(const glm::vec3 &position,
                                                    const glm::quat rotation);
  static ::reactphysics3d::Vector3 ConvertFromGLM(const glm::vec3 &vector);
  static ::reactphysics3d::Quaternion ConvertFromGLM(const glm::quat &quaternion);

  static glm::vec3 ConvertToGLM(const ::reactphysics3d::Vector3 &vector);
  static glm::quat ConvertToGLM(const ::reactphysics3d::Quaternion &quaternion);
};
} // namespace upgl