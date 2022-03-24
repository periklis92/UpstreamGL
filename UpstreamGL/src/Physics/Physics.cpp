#include <reactphysics3d/reactphysics3d.h>

#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/Math/ReactGLMHelper.h>
#include <UpstreamGL/Physics/Collision.h>
#include <UpstreamGL/Physics/Physics.h>
#include <UpstreamGL/Scene/Scene.h>

namespace upgl {
class CollisionListener : public ::reactphysics3d::EventListener {
  virtual void
  onContact(const ::reactphysics3d::CollisionCallback::CallbackData &callbackData) override {
    uint32_t numPairs = callbackData.getNbContactPairs();
    for (uint32_t i = 0; i < numPairs; ++i) {
      auto pair = callbackData.getContactPair(i);
      auto col1 = reinterpret_cast<RigidBody *>(pair.getBody1()->getUserData());
      auto col2 = reinterpret_cast<RigidBody *>(pair.getBody2()->getUserData());
      auto evType = pair.getEventType();
      CollisionType type{};

      switch (evType) {
      case ::reactphysics3d::CollisionCallback::ContactPair::EventType::ContactStay:
        type = CollisionType::Stay;
        break;
      case ::reactphysics3d::CollisionCallback::ContactPair::EventType::ContactExit:
        type = CollisionType::Exit;
        break;
      default:
        type = CollisionType::Enter;
        break;
      }

      /*col1->OnCollision().Publish(CollisionEvent{ col2, type });
      col2->OnCollision().Publish(CollisionEvent{ col1, type });*/
    }
  }
};

static CollisionListener __collisionListener;

static ::reactphysics3d::PhysicsCommon __common;

CollisionShape Physics::CreateBoxShape(const glm::vec3 &extent) {
  auto shape = __common.createBoxShape(ReactGLMHelper::ConvertFromGLM(extent));
  return CollisionShape{shape};
}

CollisionShape Physics::CreateSphereShape(float radius) {
  auto shape = __common.createSphereShape(radius);
  return CollisionShape{shape};
}

CollisionShape Physics::CreateCapsuleShape(float radius, float height) {
  auto shape = __common.createCapsuleShape(radius, height);

  return CollisionShape{shape};
}

bool Physics::Raycast(const Ray &ray, RaycastInfo &info, Scene *scene) {
  UPGL_ASSERT(scene);
  auto reactRay = ray.ToReact();
  RaycastListener listener(info);
  scene->GetPhysicsWorld().raycast(reactRay, listener.GetRaycastCallback());
  return info.RigidB != nullptr;
}

::reactphysics3d::PhysicsWorld *Physics::CreatePhysicsWorld() {
  auto world = __common.createPhysicsWorld();
  world->setEventListener(&__collisionListener);
  return world;
}

void Physics::DestroyPhysicsWorld(::reactphysics3d::PhysicsWorld *world) {
  __common.destroyPhysicsWorld(world);
}

::reactphysics3d::PhysicsCommon &Physics::GetPhysicsCommon() { return __common; }
} // namespace upgl