#include <reactphysics3d/mathematics/Quaternion.h>
#include <reactphysics3d/mathematics/Transform.h>
#include <reactphysics3d/mathematics/Vector3.h>

#include <UpstreamGL/Math/ReactGLMHelper.h>

namespace upgl {
::reactphysics3d::Transform ReactGLMHelper::ConvertFromGLM(const glm::vec3 &position,
                                                           const glm::quat rotation) {
  return ::reactphysics3d::Transform(
      ::reactphysics3d::Vector3{position.x, position.y, position.z},
      ::reactphysics3d::Quaternion{rotation.x, rotation.y, rotation.z, rotation.w});
}

::reactphysics3d::Vector3 ReactGLMHelper::ConvertFromGLM(const glm::vec3 &vector) {
  return ::reactphysics3d::Vector3{vector.x, vector.y, vector.z};
}

::reactphysics3d::Quaternion ReactGLMHelper::ConvertFromGLM(const glm::quat &quaternion) {
  return ::reactphysics3d::Quaternion{quaternion.x, quaternion.y, quaternion.z, quaternion.w};
}

glm::vec3 ReactGLMHelper::ConvertToGLM(const ::reactphysics3d::Vector3 &vector) {
  return glm::vec3{vector.x, vector.y, vector.z};
}

glm::quat ReactGLMHelper::ConvertToGLM(const ::reactphysics3d::Quaternion &quaternion) {
  return glm::quat{quaternion.w, quaternion.x, quaternion.y, quaternion.z};
}
} // namespace upgl