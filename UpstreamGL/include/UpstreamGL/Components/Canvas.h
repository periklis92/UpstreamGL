#pragma once

#include <UpstreamGL/Components/Component.h>
#include <UpstreamGL/Graphics/Font.h>
#include <UpstreamGL/Graphics/Renderable.h>
#include <UpstreamGL/System/Event.h>
#include <UpstreamGL/System/EventDispatcher.h>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace upgl {
class Node;
struct WindowResizeEvent;

enum class CanvasSpace { ScreenSpace, WorldSpace };

struct CanvasRecalculateEvent : public Event {
  UPGL_IMPL_EVT_BODY(CanvasRecalculateEvent);
  CanvasRecalculateEvent(int width, int height, int refWidth, int refHeight)
      : Event(), NewWidth(width), NewHeight(height), RefWidth(refWidth), RefHeight(refHeight) {}

  int NewWidth, NewHeight;
  int RefWidth, RefHeight;
};

using CanvasRecalculateCallback = EventDelegate<CanvasRecalculateEvent>;

struct CanvasOptions {
  CanvasSpace Space{};
  ::glm::vec2 RefSize{.0f, .0f};
};

class Canvas : public Component {
public:
  Canvas(Node *node);

  virtual void OnEnter() override;
  virtual void OnExit() override;

  void SetCanvasSpace(CanvasSpace space) { m_CanvasOptions.Space = space; }
  CanvasSpace GetCanvasSpace() const { return m_CanvasOptions.Space; }

  void SetCanvasRefSize(::glm::vec2 size) { m_CanvasOptions.RefSize = size; }
  ::glm::vec2 GetCanvasRefSize() const { return m_CanvasOptions.RefSize; }

  virtual const ::std::string GetComponentName() const override { return "Canvas"; }

  EventDispatcher<CanvasRecalculateEvent> &OnRecalculate() { return m_RecalculateDispatcher; }

private:
  CanvasOptions m_CanvasOptions{};
  EventDispatcher<CanvasRecalculateEvent> m_RecalculateDispatcher;

  virtual bool __WindowResizeCallback(const WindowResizeEvent *e);
};
} // namespace upgl