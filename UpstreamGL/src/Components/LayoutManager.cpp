#include <UpstreamGL/Components/LayoutManager.h>
#include <UpstreamGL/Scene/Director.h>
#include <UpstreamGL/System/Application.h>
#include <UpstreamGL/System/Scheduler.h>

namespace upgl {
LayoutManager::LayoutManager(Node *node) : Component(node) {
  auto canvas = m_Node->FindComponentInParent<Canvas>();
  UPGL_ASSERT(canvas);
  canvas->OnRecalculate() += {ConnectFunc<&LayoutManager::__Recalculate>, this};
}

void LayoutManager::OnEnter() { Component::OnEnter(); }

void LayoutManager::OnExit() {
  Component::OnExit();

  auto canvas = m_Node->FindComponentInParent<Canvas>();
  UPGL_ASSERT(canvas);
  canvas->OnRecalculate() -= {ConnectFunc<&LayoutManager::__Recalculate>, this};
}

bool LayoutManager::__Recalculate(const CanvasRecalculateEvent *e) {
  glm::vec3 curPosition = m_Node->GetTransform()->GetLocalPosition();
  glm::vec3 refPosition{0.f};

  switch (m_Layout.HAnchorPoint) {
  case HorizontalAnchorPoint::Left:
    refPosition.x = 0.f;
    break;
  case HorizontalAnchorPoint::Right:
    refPosition.x = (float)e->NewWidth;
    break;
  default:
    refPosition.x = (float)e->NewWidth / 2.f;
    break;
  }

  switch (m_Layout.VAnchorPoint) {
  case VerticalAnchorPoint::Bottom:
    refPosition.y = 0.f;
    break;
  case VerticalAnchorPoint::Top:
    refPosition.y = (float)e->NewHeight;
    break;
  default:
    refPosition.y = (float)e->NewHeight / 2.f;
    break;
  }

  auto transform = m_Node->GetTransform();

  if (m_Layout.AutoResize) {
    glm::vec3 newScale = glm::vec3{(float)e->NewWidth / (float)e->RefWidth,
                                   (float)e->NewHeight / (float)e->RefHeight, 1.f};
    if (m_Layout.KeepAspect)
      newScale.y = newScale.x;
    transform->SetLocalScale(newScale);
  }

  transform->SetLocalPosition(refPosition + m_Layout.Position);

  return true;
}
} // namespace upgl