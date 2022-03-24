#include <reactphysics3d/reactphysics3d.h>

#include <UpstreamGL/Math/ReactGLMHelper.h>
#include <UpstreamGL/Physics/Raycast.h>

namespace upgl {
::reactphysics3d::Ray Ray::ToReact() const {
  return ::reactphysics3d::Ray{ReactGLMHelper::ConvertFromGLM(From),
                               ReactGLMHelper::ConvertFromGLM(To)};
}

class __listen : public ::reactphysics3d::RaycastCallback {
public:
  __listen(RaycastInfo &i) : _i(i) {}
  ::reactphysics3d::decimal
  notifyRaycastHit(const ::reactphysics3d::RaycastInfo &raycastInfo) override {
    auto col = raycastInfo.collider;
    if (col) {
      auto node = reinterpret_cast<RigidBody *>(raycastInfo.body->getUserData());
      _i.RigidB = node;
    }
    return 0.0f;
  }
  RaycastInfo &_i;
};

RaycastListener::RaycastListener(RaycastInfo &info) : m_Info(info) { __tmp = new __listen(m_Info); }

RaycastListener::~RaycastListener() {
  if (__tmp)
    delete __tmp;
}

::reactphysics3d::RaycastCallback *RaycastListener::GetRaycastCallback() { return __tmp; }
} // namespace upgl