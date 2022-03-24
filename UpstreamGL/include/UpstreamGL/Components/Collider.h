#pragma once

#include "Component.h"
#include <glm/vec3.hpp>

namespace reactphysics3d {
class CollisionShape;
class CollisionBody;
} // namespace reactphysics3d

namespace upgl {
class Node;

enum class ColliderShape { None, Box, Capsule, Sphere, Mesh, SkinnedMesh };

class Collider : public Component {
public:
  Collider(Node *node);

  void CreateBoxShape(const glm::vec3 &extent);

  virtual void OnEnter() override;
  virtual void OnExit() override;

  reactphysics3d::CollisionShape &GetCollisionShape() { return *m_Shape; }
  reactphysics3d::CollisionBody &GetCollisionBody() { return *m_Body; }

  const std::string GetComponentName() const override { return "Collider"; }

private:
  void __ClearShape();

private:
  reactphysics3d::CollisionShape *m_Shape{nullptr};
  reactphysics3d::CollisionBody *m_Body{nullptr};
  ColliderShape m_ShapeType{ColliderShape::None};
};
} // namespace upgl