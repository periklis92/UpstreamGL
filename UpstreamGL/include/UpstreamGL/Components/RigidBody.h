#pragma once

#include "Component.h"
#include <UpstreamGL/Physics/Physics.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

namespace reactphysics3d {
class CollisionShape;
class CollisionBody;
class RigidBody;
} // namespace reactphysics3d

namespace upgl {
class Node;

enum class RigidBodyType { Static, Dynamic, Kinematic };

class RigidBody : public Component {
public:
  RigidBody(Node *node);

  virtual void OnEnter() override;
  virtual void OnExit() override;

  ::reactphysics3d::RigidBody &GetRigidBody() { return *m_Body; }

  void AddCollider(const CollisionShape &shape, bool isTrigger = false,
                   const glm::vec3 &position = glm::vec3(0.f),
                   const glm::quat &rotation = glm::identity<glm::quat>());

  void SetColliderAsTrigger(int index, bool isTrigger);
  void RemoveCollider(int index);

  void SetVelocity(const glm::vec3 &velocity);
  void SetAngularVelocity(const glm::vec3 &velocity);

  glm::vec3 GetVelocity();
  glm::vec3 GetAngularVelocity();

  void GetTransform(glm::vec3 &position, glm::quat &rotation);
  void SetTransform(const glm::vec3 &position, const glm::quat &rotation);

  void SetRigidBodyType(RigidBodyType type);

  const std::string GetComponentName() const override { return "RigidBody"; }

private:
  void __FixedUpdate(float fixedDeltaTime);

  ::reactphysics3d::RigidBody *m_Body{nullptr};
};
} // namespace upgl