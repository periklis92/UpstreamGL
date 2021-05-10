#include "Components/Mesh.h"
#include "Graphics/MeshRenderer.h"
#include "System/Event.h"
#include "System/Application.h"
#include "System/Entry.h"
#include <System/File.h>

#include <Graphics/ShaderBuilder.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glad/glad.h>
#include <Graphics/MeshData.h>
#include "System/EventDispatcher.h"
#include "Debug/Logger.h"
#include "System/Window.h"
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <tuple>
#include "Debug/Console.h"
#include "System/Scheduler.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "Debug/Profiler.h"
#include "Debug/Debug.h"
#include <sstream>
#include "System/Hash.h"
#include <functional>
#include "Graphics/Animation.h"
#include "Scene/Node.h"
#include "Resources/MeshResource.h"
#include "Resources/AnimationResource.h"
#include "Components/Camera.h"
// #include "stb/stb_truetype.h"

class CameraController
	: public Component
{
	glm::vec2 MoveAxis{0.f};
	glm::vec2 RotateAxis{0.f};
	bool IsRotating{false};
	float MoveSpeed{4.f};
	float RotateSpeed{6.5f};
public:
	CameraController(Node* node)
	:Component(node) 
	{
		InputManager::GetInstance()->OnKeyboardKey() += InputKeyboardDelegate{ ConnectFunc<&CameraController::keyPressedCb>, this};
		InputManager::GetInstance()->OnMouseMove() += InputMouseMoveDelegate{ ConnectFunc<&CameraController::mouseMoveCb>, this};
		InputManager::GetInstance()->OnMouseButton() += InputMouseButtonDelegate{ ConnectFunc<&CameraController::mousePressCb>, this};
		Application::GetInstance()->GetScheduler()->Register(UpdateDelegate{ ConnectFunc<&CameraController::update>, this} );
	}

	void update(float deltaTime)
	{
		m_Node->Translate(m_Node->GetForwardDirection() * MoveAxis.y * deltaTime * MoveSpeed);
		m_Node->Translate(m_Node->GetRightDirection() * MoveAxis.x * deltaTime * MoveSpeed);
		
		auto frameRot = RotateAxis * deltaTime * RotateSpeed;
		RotateAxis -= frameRot;
		if (glm::length(RotateAxis) < .2f)
			RotateAxis = glm::vec2{0.f, 0.f};
		m_Node->Rotate(frameRot.x, glm::vec3{0.f, 1.f, 0.f});
		m_Node->Rotate(frameRot.y, m_Node->GetRightDirection());
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

class MyApp : public Application
{
public:
	MyApp(int argc, char** argv)
		: Application("MyApp", argc, argv)
	{

	}

	Shader shader;	
	MeshResource*		 model;
	AnimationResource* 	anim;
	float animTime = 0.f;
	Mesh* mesh;
	
	virtual void OnInitialize() override
	{
		Application::OnInitialize();
		
		File vertShader("./resources/simpleShader.vert");
		File fragShader("./resources/simpleShader.frag");

		shader = ShaderBuilder("")
			.AddShader(vertShader.ToString().c_str(), ShaderType::Vertex)
			.AddShader(fragShader.ToString().c_str(), ShaderType::Fragment)
			.Build();

		ComponentRegistry::RegisterComponent<Mesh>("mesh");
		auto& cameraNode = Director::GetInstance()->GetScene()->CreateNode("camera");
		cameraNode.AddComponent<Camera>();
		cameraNode.AddComponent<CameraController>();
		cameraNode.SetLocalPosition(glm::vec3(0, 0, 0));
 		model = new MeshResource("cube", "./resources/Cube.fbx");
		for (int i = 0; i < 20; ++i)
		{
			auto& cube = Director::GetInstance()->GetScene()->CreateNode("cube " + std::to_string(i));
			cube.SetLocalPosition(glm::vec3{-20.f + i * 2.f, 0, 0});
			mesh = dynamic_cast<Mesh*>(cube.AddComponent("mesh"));
			mesh->SetShader(&shader);
			mesh->SetMesh(model->GetMesh());
		}
	}

	float rotation = 0;
	virtual void OnTick(float deltaTime) override
	{
		// rotation += deltaTime;
		// vampire.Rotate(10.f * deltaTime, glm::vec3{0.f, 1.f, 0.f});
		
		
		// auto& clip = anim->GetClips()[0];
		// animTime += deltaTime * clip.GetTicksPerSecond();
		// if (animTime > clip.GetDuration())
		// {
		// 	animTime = 0.f;
		// }
		// mesh->UpdateFromAnimation(clip, animTime);
		// auto& cameraNode = Director::GetInstance()->GetScene()->GetNode("camera");
		// mesh->Render(cameraNode.GetComponent<Camera>());
	}

	virtual void OnGui() override
	{
		Application::OnGui();
		auto& cameraNode = Director::GetInstance()->GetScene()->GetNode("camera");
		auto pos = cameraNode.GetWorldPosition();
		ImGui::Begin("Camera Info", nullptr);
		ImGui::Text("Position:");
		ImGui::Text("Pos X: %.02f", pos.x);
		ImGui::Text("Pos Y: %.02f", pos.y);
		ImGui::Text("Pos Z: %.02f", pos.z);
		ImGui::Separator();
		auto rot = glm::degrees(cameraNode.GetWorldRotation());
		ImGui::Text("Rotation:");
		ImGui::Text("Euler X: %.02f", rot.x);
		ImGui::Text("Euler Y: %.02f", rot.y);
		ImGui::Text("Euler Z: %.02f", rot.z);
		ImGui::End();
	}
};

GLR_APP_IMPL(MyApp)
