#include <reactphysics3d/reactphysics3d.h>

#include <UpstreamGL/Components/Collider.h>
#include <UpstreamGL/Components/Transform.h>
#include <UpstreamGL/Math/ReactGLMHelper.h>
#include <UpstreamGL/Scene/Node.h>
#include <UpstreamGL/Scene/Scene.h>

namespace upgl {
Collider::Collider(Node *node) : Component(node) {}

void Collider::CreateBoxShape(const glm::vec3 &extent) {
  auto &common = m_Node->GetScene()->GetPhysicsCommon();
  if (m_Shape)
    __ClearShape();
  m_Shape = common.createBoxShape(ReactGLMHelper::ConvertFromGLM(extent));
  m_ShapeType = ColliderShape::Box;
}

void Collider::OnEnter() {
  auto transform = m_Node->GetTransform();
  m_Body = m_Node->GetScene()->GetPhysicsWorld().createCollisionBody(ReactGLMHelper::ConvertFromGLM(
      transform->GetWorldPosition(), transform->GetWorldRotationQuat()));
  auto col = m_Body->addCollider(m_Shape, reactphysics3d::Transform::identity());
  col->setUserData(this);
  m_Body->setUserData(this);
}

void Collider::OnExit() {
  m_Node->GetScene()->GetPhysicsWorld().destroyCollisionBody(m_Body);
  if (m_Shape)
    __ClearShape();
}

void Collider::__ClearShape() {
  if (m_Body->getNbColliders() > 0)
    m_Body->removeCollider(m_Body->getCollider(0));
  auto &common = m_Node->GetScene()->GetPhysicsCommon();
  switch (m_ShapeType) {
  case ColliderShape::Box:
    common.destroyBoxShape(dynamic_cast<reactphysics3d::BoxShape *>(m_Shape));
    break;
  default:
    break;
  }
}
} // namespace upgl