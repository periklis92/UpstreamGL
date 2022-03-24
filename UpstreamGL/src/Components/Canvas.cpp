#include <UpstreamGL/Components/Canvas.h>
#include <UpstreamGL/Scene/Director.h>
#include <UpstreamGL/Scene/Node.h>
#include <UpstreamGL/System/Application.h>
#include <UpstreamGL/System/Event.h>

namespace upgl {
Canvas::Canvas(Node *node) : Component(node) {}

void Canvas::OnEnter() {
  auto app = Application::GetInstance();
  int w, h;
  app->GetWindow()->GetSize(w, h);
  m_Node->GetTransform()->SetLocalRotation({.0f, 180.f, .0f});

  if (m_CanvasOptions.Space == CanvasSpace::ScreenSpace) {
    app->GetWindow()->OnWindowResize() += {ConnectFunc<&Canvas::__WindowResizeCallback>, this};

    if (m_CanvasOptions.RefSize.x == 0.f || m_CanvasOptions.RefSize.y == 0.f) {
      m_CanvasOptions.RefSize = glm::vec2{w, h};
    }
  }

  auto e = WindowResizeEvent(w, h);
  __WindowResizeCallback(&e);
}

void Canvas::OnExit() {
  auto app = Application::GetInstance();

  if (m_CanvasOptions.Space == CanvasSpace::ScreenSpace)
    app->GetWindow()->OnWindowResize() -= {ConnectFunc<&Canvas::__WindowResizeCallback>, this};
}

bool Canvas::__WindowResizeCallback(const WindowResizeEvent *e) {
  m_RecalculateDispatcher.Publish(CanvasRecalculateEvent{
      e->Width, e->Height, (int)m_CanvasOptions.RefSize.x, (int)m_CanvasOptions.RefSize.y});
  return true;
}
} // namespace upgl