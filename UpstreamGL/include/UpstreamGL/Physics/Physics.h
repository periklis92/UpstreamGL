#pragma once

#include <UpstreamGL/Physics/Raycast.h>
#include <glm/vec3.hpp>

namespace reactphysics3d {
class CollisionShape;
class PhysicsCommon;
class PhysicsWorld;
class EventListener;
} // namespace reactphysics3d

namespace upgl {
struct Physics;
class RigidBody;
class Scene;

class CollisionShape {
public:
  CollisionShape() {}
  CollisionShape(const CollisionShape &shape) : m_RShape(shape.m_RShape) {}
  CollisionShape &operator=(const CollisionShape &shape) {
    m_RShape = shape.m_RShape;
    return *this;
  }

  ::reactphysics3d::CollisionShape *GetReactShape() const { return m_RShape; }

private:
  CollisionShape(::reactphysics3d::CollisionShape *shape) : m_RShape(shape) {}

  friend struct Physics;

private:
  ::reactphysics3d::CollisionShape *m_RShape{nullptr};
};

struct Physics {
public:
  static CollisionShape CreateBoxShape(const glm::vec3 &extent);
  static CollisionShape CreateSphereShape(float radius);
  static CollisionShape CreateCapsuleShape(float radius, float height);
  static bool Raycast(const Ray &ray, RaycastInfo &info, Scene *scene);

private:
  static ::reactphysics3d::PhysicsWorld *CreatePhysicsWorld();
  static void DestroyPhysicsWorld(::reactphysics3d::PhysicsWorld *world);
  static ::reactphysics3d::PhysicsCommon &GetPhysicsCommon();
  static ::reactphysics3d::EventListener &GetEventListener();

  friend class RigidBody;
  friend class Scene;
};
} // namespace upgl