
//#include <string>
//#include <cstdio>
//#include <cstdlib>
//#include "Graphics/ShaderBuilder.h"
//#include "GLFW/glfw3.h"
//#include <System/AssetLoader.h>
//#include <System/File.h>
//#include <glad/glad.h>
//#include <glm/glm.hpp>
//#include <glm/ext/matrix_clip_space.hpp>
//#include <glm/ext/matrix_transform.hpp>
//#include "Graphics/Mesh.h"
//#include "Graphics/Animation.h"
//#include <imgui.h>
//#include <imgui_impl_glfw.h>
//#include <imgui_impl_opengl3.h>
//int main(int argc, char* argv[])
//{
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_SAMPLES, 16);
//    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
//    GLFWwindow* glfwHandle = glfwCreateWindow(600, 600, "Test", NULL, NULL);
//    if (glfwHandle == nullptr)
//    {
//        printf("Failed to create window");
//        glfwTerminate();
//        exit(-1);
//    }
//    
//    glfwMakeContextCurrent(glfwHandle);
//
//	GLHelper::InitializeOpenGL();
//
//    ImGui::CreateContext();
//
//    ImGuiIO& io = ImGui::GetIO(); (void)io;
//    ImGui_ImplGlfw_InitForOpenGL(glfwHandle, true);
//    ImGui_ImplOpenGL3_Init("#version 460 core");
//    //ImGui_ImplGlfw_NewFrame();
//    
//    ImGui::StyleColorsDark();
//
//    io.Fonts->AddFontDefault();
//
//    File vertShader("./resources/simpleSkinnedShader.vert");
//    File fragShader("./resources/simpleShader.frag");
//    
//    glm::mat4 proj = glm::perspective(90.f, 1.f, .3f, 1000.f);
//    glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 10.f, -80.f));
//
//    //view = glm::rotate(view, glm::radians(-90.f), glm::vec3(0, 1, 0));
//    
//	Shader shader = ShaderBuilder("")
//		.AddShader(vertShader.ToString().c_str(), ShaderType::Vertex)
//        .AddShader(fragShader.ToString().c_str(), ShaderType::Fragment)
//		.Build();
//
//    Asset3D* asset = AssetLoader::Load3DAsset("./resources/dancing_vampire.dae", "peri");
//    shader.Bind();
//    //shader.SetMat4("_ModelViewProjection", proj * view);
//
//    float animTime = 0.f;
//    double deltaTime = glfwGetTime(), showTime = 0;
//    double showInterval = 1, showTimer = 0;
//    double totalTime = 0;
//    uint32_t timesCounted = 0;
//
//    //glEnable(GL_CULL_FACE);
//    //glCullFace(GL_BACK);
//    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//    float f = 0;
//    bool open = true;
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
//    double startTime = 0, lastTime = 0;
//
//    while (!glfwWindowShouldClose(glfwHandle))
//    {
//        startTime = glfwGetTime();
//        deltaTime = startTime - lastTime;
//        lastTime = startTime;
//        totalTime += deltaTime;
//        timesCounted++;
//        showTimer += deltaTime;
//        if (showTimer >= showInterval)
//        {
//            showTime = deltaTime;
//            showTimer = 0;
//        }
//        glClearColor(.2f, .4f, .8f, 1.f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//
//        ImGui_ImplOpenGL3_NewFrame();
//        ImGui_ImplGlfw_NewFrame();
//        ImGui::NewFrame();
//        bool show_app_main_menu_bar = true;
//        bool tab_item1 = true, tab_item2 = false;
//        if (open)
//        {
//            ImGui::SetNextWindowSize(ImVec2{ 600, 600 });
//            ImGui::SetNextWindowBgAlpha(1.f);
//            ImGui::SetNextWindowPos(ImVec2{ 0, 0 });
//            ImGui::Begin("Performance", &open, ImGuiWindowFlags_NoMove 
//                | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_HorizontalScrollbar );
//            
//            ImGui::Text("Performance:");
//            ImGui::Separator();
//
//            if (ImGui::BeginTabBar("tabs"))
//            {
//                if (ImGui::BeginTabItem("tab item 1"))
//                {
//                    if (ImGui::BeginMenu("hi"))
//                    {
//
//                        ImGui::MenuItem("Main menu bar");
//                        ImGui::EndMenu();
//                    }
//                    ImGui::EndTabItem();
//                }
//
//                if (ImGui::BeginTabItem("tab item 2"))
//                {
//                    ImGui::Separator();
//                    ImGui::Text("Main Loop: %.02fms", showTime * 1000.);
//                    ImGui::EndTabItem();
//                }
//
//                ImGui::EndTabBar();
//            }
//            ImGui::End();
//        }
//
//        AnimationClip* clip = asset->Animations.begin()->second;
//        animTime += deltaTime * clip->GetTicksPerSecond();
//        if (animTime > clip->GetDuration())
//        {
//            printf("Animation finished in: %f s\n", animTime / clip->GetTicksPerSecond());
//            animTime = 0.f;
//        }
//        
//        for (const auto& mesh : asset->Meshes)
//        {
//            AABB aabb = mesh.second->GetAABB();
//            glm::vec3 size = aabb.GetSize();
//
//            auto bt = mesh.second->GetArmature()->GetBoneTransforms();
//            shader.SetMat4("_BoneTransforms", bt.size(), bt.data());
//            glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
//            model = glm::scale(model, glm::vec3(.2f, .2f, .2f));
//            mesh.second->GetArmature()->UpdateFromAnimation(*clip, animTime);
//            shader.SetMat4("_ModelViewProjection", proj * view * model);
//            mesh.second->Bind();
//            mesh.second->Draw();
//        }
//        
//        glfwPollEvents();
//
//        ImGui::Render();
//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//        glfwSwapBuffers(glfwHandle);
//
//    }
//
//
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplGlfw_Shutdown();
//    ImGui::DestroyContext();
//
//    printf("Avg frame time: %lfms\n", (totalTime / timesCounted) * 1000.);
//    
//    glfwTerminate();
//	return 0;
//}
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

class CameraController
	: public Component
{
	glm::vec2 MoveAxis{0.f};
	glm::vec2 RotateAxis{0.f};
	bool IsRotating = false;
	float MoveSpeed = 4.f;
	float RotateSpeed = 6.5f;
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

int unbound(int)
{
	return 1;
}

class MyApp : public Application
{
public:
	MyApp(int argc, wchar_t** argv)
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
