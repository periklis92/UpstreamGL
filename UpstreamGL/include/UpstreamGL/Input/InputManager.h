#pragma once

#include <UpstreamGL/System/Application.h>
#include <UpstreamGL/System/Window.h>

#include "InputEvent.h"

namespace upgl {
class InputManager {
public:
  EventDispatcher<InputKeyboardEvent> &OnKeyboardKey() { return m_KeyboardDispatcher; }

  EventDispatcher<InputMouseButtonEvent> &OnMouseButton() { return m_MouseButtonDispatcher; }

  EventDispatcher<InputMouseMoveEvent> &OnMouseMove() { return m_MouseMoveDispatcher; }

  void GetMousePosition(float &x, float &y) {
    Application::GetInstance()->GetWindow()->GetMousePosition(x, y);
  }

  static InputManager *GetInstance() {
    static InputManager __InputManager;
    return &__InputManager;
  }

private:
  InputManager() = default;

private:
  EventDispatcher<InputKeyboardEvent> m_KeyboardDispatcher;
  EventDispatcher<InputMouseButtonEvent> m_MouseButtonDispatcher;
  EventDispatcher<InputMouseMoveEvent> m_MouseMoveDispatcher;
};
} // namespace upgl