#include <UpstreamGL/UpstreamGL.h>
#include "CameraController.h"
#include "testMenu.h"
#include <imgui.h>
#include <cstdio>
typedef glm::vec2 Vector2;


class MyApp : public Application
{
public:
	MyApp(int argc, char** argv)
		: Application("MyApp", argc, argv)
	{
	}
	TestMenu* menu;
	Shader shader;	
	AnimationResource* anim;
	float animTime = 0.f;
	Mesh* mesh;
	
	virtual void OnInitialize() override
	{
		Application::OnInitialize();
		menu = new TestMenu();
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
	bool fs = false;

	virtual void OnGui() override
	{
		Application::OnGui();
		menu->Draw();
	}
};

UPGL_APP_IMPL(MyApp)