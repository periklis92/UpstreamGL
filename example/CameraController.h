#pragma once
#include <UpstreamGL/UpstreamGL.h>

class CameraController
	: public Component
{
	glm::vec2 MoveAxis{0.f};
	glm::vec2 RotateAxis{0.f};
	bool IsRotating{false};
	float RotateSpeed{6.5f};
public:
	float MoveSpeed = 4.f;
	glm::vec3 MousePOsil;
	CameraController(Node* node)
	:Component(node) 
	{
		InputManager::GetInstance()->OnKeyboardKey() += InputKeyboardDelegate{ ConnectFunc<&CameraController::keyPressedCb>, this};
		InputManager::GetInstance()->OnMouseMove() += InputMouseMoveDelegate{ ConnectFunc<&CameraController::mouseMoveCb>, this};
		InputManager::GetInstance()->OnMouseButton() += InputMouseButtonDelegate{ ConnectFunc<&CameraController::mousePressCb>, this};
		Application::GetInstance()->GetScheduler()->RegisterUpdate(UpdateDelegate{ ConnectFunc<&CameraController::update>, this} );
	}

	void update(float deltaTime)
	{
		auto transform = m_Node->GetTransform();
		transform->Translate(transform->GetForwardDirection() * MoveAxis.y * deltaTime * MoveSpeed);
		transform->Translate(transform->GetRightDirection() * MoveAxis.x * deltaTime * MoveSpeed);
		
		auto frameRot = RotateAxis * deltaTime * RotateSpeed;
		RotateAxis -= frameRot;
		if (glm::length(RotateAxis) < .2f)
			RotateAxis = glm::vec2{0.f, 0.f};
		transform->Rotate(frameRot.x, glm::vec3{0.f, 1.f, 0.f});
		transform->Rotate(frameRot.y, transform->GetRightDirection());

		MousePOsil = m_Node->GetComponent<PerspectiveCamera>()->ScreenToWorldSpace(InputManager::GetInstance()->GetMousePosition());
	}

	const std::string GetComponentName() const override
	{
		return "CameraController";
	}

private:
	bool mouseMoveCb(const InputMouseMoveEvent* e)
	{
		if (IsRotating)
			RotateAxis += glm::vec2{e->DeltaX, e->DeltaY};
			
		return true;
	}
	bool mousePressCb(const InputMouseButtonEvent* e)
	{
		if (e->Button == MouseButton::RIGHT)
			IsRotating = e->IsPressed;
		return true;
	}
	bool keyPressedCb(const InputKeyboardEvent* e)
	{
		if (e->Key == KeyboardKey::W)
			if (e->IsPressed)
				MoveAxis.y = 1;
			else
				MoveAxis.y = 0;
		else if (e->Key == KeyboardKey::S)
			if (e->IsPressed)
				MoveAxis.y = -1;
			else
				MoveAxis.y = 0;
		else if (e->Key == KeyboardKey::A)
			if (e->IsPressed)
				MoveAxis.x = -1;
			else
				MoveAxis.x = 0;
		else if (e->Key == KeyboardKey::D)
			if (e->IsPressed)
				MoveAxis.x = 1;
			else
				MoveAxis.x = 0;
		else if (e->Key == KeyboardKey::LEFT_SHIFT)
			if (e->IsPressed)
				MoveSpeed = 10.f;
			else
				MoveSpeed = 4.f;

		return true;
	}

};