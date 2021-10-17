#pragma once

#include "InputEvent.h"
#include <glm/glm.hpp>
#include <UpstreamGL/System/Application.h>
#include <UpstreamGL/System/Window.h>

class InputManager
{
public:
	inline EventDispatcher<InputKeyboardEvent>& OnKeyboardKey()
	{
		return m_KeyboardDispatcher;
	}

	inline EventDispatcher<InputMouseButtonEvent>& OnMouseButton()
	{
		return m_MouseButtonDispatcher;
	}

	inline EventDispatcher<InputMouseMoveEvent>& OnMouseMove()
	{
		return m_MouseMoveDispatcher;
	}

	inline glm::vec2 GetMousePosition() const
	{
		glm::vec2 pos{0.f};
		Application::GetInstance()->GetWindow()->GetMousePosition(pos.x, pos.y);
		return pos;
	}

	static InputManager* GetInstance() 
	{
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