#pragma once

#include <limits>

#include <UpstreamGL/System/Delegate.h>
#include <glm/vec3.hpp>

namespace reactphysics3d {
struct Ray;
struct RaycastInfo;
class RaycastCallback;
} // namespace reactphysics3d

namespace upgl {
struct Physics;
class Node;
class RigidBody;

struct Ray {
  ::glm::vec3 From{0.f};
  ::glm::vec3 To{0.f};

  Ray(::glm::vec3 from, ::glm::vec3 to) : From(from), To(to) {}

  ::reactphysics3d::Ray ToReact() const;
};

struct RaycastInfo {
  RigidBody *RigidB{nullptr};
};

class RaycastListener {
public:
  ~RaycastListener();

private:
  RaycastListener(RaycastInfo &info);

  ::reactphysics3d::RaycastCallback *GetRaycastCallback();

  friend struct Physics;

  RaycastInfo &m_Info;
  ::reactphysics3d::RaycastCallback *__tmp;
};
} // namespace upgl