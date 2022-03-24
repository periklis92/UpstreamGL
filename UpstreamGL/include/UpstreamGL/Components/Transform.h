#pragma once

#include <cassert>
#include <cstdint>
#include <list>
#include <string>

#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <UpstreamGL/Components/Component.h>
#include <UpstreamGL/Debug/Debug.h>

namespace upgl {
class Transform : public Component {
public:
  Transform(Node *node) : Component(node) {}

  virtual const ::std::string GetComponentName() const override { return "Transform"; }

  void AddChild(Transform *const &child) {
    UPGL_ASSERT(child);
    child->m_Parent = this;
  }

  void SetParent(Transform *const &parent) { m_Parent = parent; }
  Transform *GetParent() const { return m_Parent; }

  void SetLocalTransform(const ::glm::mat4 &transform);
  void SetWorldTransform(const ::glm::mat4 &transform);

  void SetLocalPosition(::glm::vec3 position);
  void SetLocalRotation(::glm::quat rotation);
  void SetLocalRotation(::glm::vec3 rotationDegrees);
  void SetLocalScale(::glm::vec3 scale);

  void SetWorldPosition(::glm::vec3 position);
  void SetWorldRotation(::glm::quat rotationDegrees);
  void SetWorldRotation(::glm::vec3 rotation);
  void SetWorldScale(::glm::vec3 scale);

  void Translate(::glm::vec3 translation);
  void Rotate(float angle, ::glm::vec3 axis);
  void Rotate(::glm::vec3 eulerAngles);
  void Scale(::glm::vec3 scale);

  const ::glm::mat4 GetLocalTransform() const;
  const ::glm::vec3 GetLocalPosition() const;
  const ::glm::quat GetLocalRotationQuat() const;
  const ::glm::vec3 GetLocalRotation() const;
  const ::glm::vec3 GetLocalScale() const;

  const ::glm::mat4 GetWorldTransform() const;
  const ::glm::vec3 GetWorldPosition(bool applyLocal = true) const;
  const ::glm::quat GetWorldRotationQuat(bool applyLocal = true) const;
  const ::glm::vec3 GetWorldRotation(bool applyLocal = true) const;
  const ::glm::vec3 GetWorldScale(bool applyLocal = true) const;

  const ::glm::vec3 GetForwardDirection() const;
  const ::glm::vec3 GetUpDirection() const;
  const ::glm::vec3 GetRightDirection() const;

private:
  Transform *m_Parent{nullptr};
  //::std::list<Transform*> m_Children{};

  ::glm::vec3 m_LocalPosition{0, 0, 0};
  ::glm::quat m_LocalRotation{::glm::identity<::glm::quat>()};
  ::glm::vec3 m_LocalScale{1, 1, 1};
};
} // namespace upgl