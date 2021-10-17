#include <UpstreamGL/UpstreamGL.h>
#include <imgui.h>

typedef glm::vec2 Vector2;

class CameraController
	: public Component
{
	Vector2 MoveAxis{0.f};
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
class MyApp : public Application
{
public:
	MyApp(int argc, char** argv)
		: Application("MyApp", argc, argv)
	{
	}

	Shader shader;	
	AnimationResource* anim;
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
		cameraNode.AddComponent<PerspectiveCamera>();
		cameraNode.AddComponent<CameraController>();
		auto transform = cameraNode.GetTransform();
		transform->SetLocalPosition(glm::vec3(0, 0, 0));
		MeshResource model("cube", "./resources/Cube.fbx");

		for (int i = 0; i < 20; ++i)
		{
			auto& cube = Director::GetInstance()->GetScene()->CreateNode("cube " + std::to_string(i));
			auto cubetransform = cube.GetTransform();
			cubetransform->SetLocalPosition(glm::vec3{-20.f + i * 2.f, 0, 0});
			mesh = dynamic_cast<Mesh*>(cube.AddComponent("mesh"));
			mesh->SetShader(&shader);
			mesh->SetMesh(model);
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
		auto camController = cameraNode.GetComponent<CameraController>();
		auto transform = cameraNode.GetTransform();
		auto pos = transform->GetWorldPosition();
		ImGui::Begin("Camera Info", nullptr);
		ImGui::Text("Position:");
		ImGui::Text("Pos X: %.02f", pos.x);
		ImGui::Text("Pos Y: %.02f", pos.y);
		ImGui::Text("Pos Z: %.02f", pos.z);
		ImGui::Separator();
		auto rot = glm::degrees(transform->GetWorldRotation());
		ImGui::Text("Rotation:");
		ImGui::Text("Euler X: %.02f", rot.x);
		ImGui::Text("Euler Y: %.02f", rot.y);
		ImGui::Text("Euler Z: %.02f", rot.z);
		ImGui::Separator();
		ImGui::Text("Mouse World Pos:");
		ImGui::Text("X: %.04f", camController->MousePOsil.x);
		ImGui::Text("Y: %.04f", camController->MousePOsil.y);
		ImGui::Text("Z: %.04f", camController->MousePOsil.z);
		ImGui::Separator();
		ImGui::Text("Mouse Screen Pos:");
		auto mpos = InputManager::GetInstance()->GetMousePosition();
		ImGui::Text("X: %.04f", mpos.x);
		ImGui::Text("Y: %.04f", mpos.y);
		ImGui::End();
	}
};

GLR_APP_IMPL(MyApp)
