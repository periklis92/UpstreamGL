#include <reactphysics3d/reactphysics3d.h>

#include <UpstreamGL/Components/Collider.h>
#include <UpstreamGL/Components/RigidBody.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/Math/ReactGLMHelper.h>
#include <UpstreamGL/Scene/Node.h>
#include <UpstreamGL/Scene/Scene.h>
#include <UpstreamGL/System/Application.h>
#include <UpstreamGL/System/Scheduler.h>

namespace upgl {
RigidBody::RigidBody(Node *node) : Component(node) {
  auto transform = m_Node->GetTransform();
  m_Body = m_Node->GetScene()->GetPhysicsWorld().createRigidBody(ReactGLMHelper::ConvertFromGLM(
      transform->GetWorldPosition(), transform->GetWorldRotationQuat()));
  UPGL_ASSERT(m_Body);
  m_Body->setUserData(this);
}

void RigidBody::AddCollider(const CollisionShape &shape, bool isTrigger, const glm::vec3 &position,
                            const glm::quat &rotation) {
  auto col = m_Body->addCollider(shape.GetReactShape(),
                                 ReactGLMHelper::ConvertFromGLM(position, rotation));
  col->setIsTrigger(isTrigger);
}

void RigidBody::SetColliderAsTrigger(int index, bool isTrigger) {
  auto col = m_Body->getCollider(index);
  UPGL_ASSERT(col);
  col->setIsTrigger(isTrigger);
}

void RigidBody::RemoveCollider(int index) {
  auto col = m_Body->getCollider(index);
  UPGL_ASSERT(col);
  m_Body->removeCollider(col);
}

void RigidBody::SetVelocity(const glm::vec3 &velocity) {
  m_Body->setLinearVelocity(ReactGLMHelper::ConvertFromGLM(velocity));
}

void RigidBody::SetAngularVelocity(const glm::vec3 &velocity) {
  m_Body->setAngularVelocity(ReactGLMHelper::ConvertFromGLM(velocity));
}

glm::vec3 RigidBody::GetVelocity() {
  return ReactGLMHelper::ConvertToGLM(m_Body->getLinearVelocity());
}

glm::vec3 RigidBody::GetAngularVelocity() {
  return ReactGLMHelper::ConvertToGLM(m_Body->getAngularVelocity());
}

void RigidBody::GetTransform(glm::vec3 &position, glm::quat &rotation) {
  auto trans = m_Body->getTransform();
  position = ReactGLMHelper::ConvertToGLM(trans.getPosition());
  rotation = ReactGLMHelper::ConvertToGLM(trans.getOrientation());
}

void RigidBody::SetTransform(const glm::vec3 &position, const glm::quat &rotation) {
  m_Body->setTransform(ReactGLMHelper::ConvertFromGLM(position, rotation));
}

void RigidBody::SetRigidBodyType(RigidBodyType type) {
  switch (type) {
  case RigidBodyType::Static:
    m_Body->setType(::reactphysics3d::BodyType::STATIC);
    break;
  case RigidBodyType::Dynamic:
    m_Body->setType(::reactphysics3d::BodyType::DYNAMIC);
    break;
  case RigidBodyType::Kinematic:
    m_Body->setType(::reactphysics3d::BodyType::KINEMATIC);
    break;
  }
}

void RigidBody::OnEnter() {
  Application::GetInstance()->GetScheduler()->RegisterFixedUpdate(
      UpdateDelegate{ConnectFunc<&RigidBody::__FixedUpdate>, this});
}

void RigidBody::OnExit() {
  Application::GetInstance()->GetScheduler()->UnregisterFixedUpdate(
      UpdateDelegate{ConnectFunc<&RigidBody::__FixedUpdate>, this});
  m_Node->GetScene()->GetPhysicsWorld().destroyRigidBody(m_Body);
}

void RigidBody::__FixedUpdate(float fixedDeltaTime) {
  auto transform = m_Node->GetTransform();
  auto &rTransform = m_Body->getTransform();
  transform->SetLocalPosition(ReactGLMHelper::ConvertToGLM(rTransform.getPosition()));
  transform->SetLocalRotation(ReactGLMHelper::ConvertToGLM(rTransform.getOrientation()));
}
} // namespace upgl