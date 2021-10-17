#pragma once

#include "InputEvent.h"

class InputManager
{
public:
	EventDispatcher<InputKeyboardEvent>& OnKeyboardKey()
	{
		return m_KeyboardDispatcher;
	}

	EventDispatcher<InputMouseButtonEvent>& OnMouseButton()
	{
		return m_MouseButtonDispatcher;
	}

	EventDispatcher<InputMouseMoveEvent>& OnMouseMove()
	{
		return m_MouseMoveDispatcher;
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

	float m_LastMouseX{0.f}, m_LastMouseY{0.f};
};