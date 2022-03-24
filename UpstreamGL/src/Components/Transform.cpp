#include <algorithm>
#include <glm/gtx/euler_angles.hpp>

#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/Math/Math.h>

namespace upgl {
void Transform::SetLocalTransform(const glm::mat4 &transform) {
  m_LocalPosition = Math::GetTranslation(transform);
  m_LocalRotation = Math::GetRotationQuat(transform);
  m_LocalScale = Math::GetScale(transform);
}

void Transform::SetWorldTransform(const glm::mat4 &transform) {
  SetWorldPosition(Math::GetTranslation(transform));
  SetWorldRotation(Math::GetRotationQuat(transform));
  SetWorldScale(Math::GetScale(transform));
}

void Transform::SetLocalPosition(glm::vec3 position) { m_LocalPosition = position; }

void Transform::SetLocalRotation(glm::quat rotation) { m_LocalRotation = rotation; }

void Transform::SetLocalRotation(glm::vec3 rotationDegrees) {
  m_LocalRotation = glm::identity<glm::quat>();
  m_LocalRotation =
      glm::rotate(m_LocalRotation, glm::radians(rotationDegrees.x), glm::vec3(1, 0, 0));
  m_LocalRotation =
      glm::rotate(m_LocalRotation, glm::radians(rotationDegrees.y), glm::vec3(0, 1, 0));
  m_LocalRotation =
      glm::rotate(m_LocalRotation, glm::radians(rotationDegrees.z), glm::vec3(0, 0, 1));
}

void Transform::SetLocalScale(glm::vec3 scale) { m_LocalScale = scale; }

void Transform::SetWorldPosition(glm::vec3 position) {
  m_LocalPosition = position - (GetWorldPosition() - m_LocalPosition);
}

void Transform::SetWorldRotation(glm::quat rotation) {
  m_LocalRotation =
      rotation * (rotation * glm::inverse(GetWorldRotationQuat() * glm::inverse(m_LocalRotation)));
}

void Transform::SetWorldRotation(glm::vec3 rotationDegrees) {
  auto rotQ = glm::identity<glm::quat>();
  rotQ = glm::rotate(rotQ, glm::radians(rotationDegrees.x), glm::vec3(1, 0, 0));
  rotQ = glm::rotate(rotQ, glm::radians(rotationDegrees.y), glm::vec3(0, 1, 0));
  rotQ = glm::rotate(rotQ, glm::radians(rotationDegrees.z), glm::vec3(0, 0, 1));
  SetWorldRotation(rotQ);
}

void Transform::SetWorldScale(glm::vec3 scale) {
  m_LocalScale = scale / (GetWorldScale() / m_LocalScale);
}

void Transform::Translate(glm::vec3 translation) { m_LocalPosition += translation; }

void Transform::Rotate(float angle, glm::vec3 axis) {
  m_LocalRotation = glm::rotate(m_LocalRotation, glm::radians(angle), axis);
}

void Transform::Rotate(glm::vec3 eulerAngles) {
  m_LocalRotation *= glm::quat(glm::radians(eulerAngles));
}

void Transform::Scale(glm::vec3 scale) { m_LocalScale *= scale; }

const glm::mat4 Transform::GetLocalTransform() const {
  glm::mat4 trans = glm::translate(glm::mat4(1.f), m_LocalPosition);
  glm::mat4 scale = glm::scale(glm::mat4(1.f), m_LocalScale);
  glm::mat4 rot = glm::toMat4(m_LocalRotation);
  return trans * rot * scale;
}

const glm::vec3 Transform::GetLocalPosition() const { return m_LocalPosition; }

const glm::quat Transform::GetLocalRotationQuat() const { return m_LocalRotation; }

const glm::vec3 Transform::GetLocalRotation() const { return glm::eulerAngles(m_LocalRotation); }

const glm::vec3 Transform::GetLocalScale() const { return m_LocalScale; }

const glm::mat4 Transform::GetWorldTransform() const {
  glm::mat4 world = glm::mat4(1.f);
  Transform *parent = m_Parent;
  if (parent)
    world = parent->GetWorldTransform();
  world *= GetLocalTransform();
  return world;
}

const glm::vec3 Transform::GetWorldPosition(bool applyLocal) const {
  glm::vec3 worldPos = glm::vec3(0.f);
  Transform *parent = m_Parent;
  if (parent)
    worldPos += parent->GetWorldPosition();

  if (applyLocal)
    worldPos += m_LocalPosition;

  return worldPos;
}

const glm::quat Transform::GetWorldRotationQuat(bool applyLocal) const {
  glm::quat worldRot = glm::identity<glm::quat>();
  Transform *parent = m_Parent;
  if (parent)
    worldRot *= parent->GetWorldRotationQuat();

  if (applyLocal)
    worldRot *= m_LocalRotation;

  return worldRot;
}

const glm::vec3 Transform::GetWorldRotation(bool applyLocal) const {
  return glm::eulerAngles(GetWorldRotationQuat());
}

const glm::vec3 Transform::GetWorldScale(bool applyLocal) const {
  glm::vec3 worldScale = glm::vec3(1.f);
  Transform *parent = m_Parent;
  if (parent)
    worldScale *= parent->m_LocalScale;

  if (applyLocal)
    worldScale *= m_LocalScale;

  return worldScale;
}

const glm::vec3 Transform::GetForwardDirection() const {
  return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[2]);
}

const glm::vec3 Transform::GetUpDirection() const {
  return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[1]);
}

const glm::vec3 Transform::GetRightDirection() const {
  return glm::normalize(glm::inverse(glm::mat4_cast(GetWorldRotationQuat()))[0]);
}
} // namespace upgl