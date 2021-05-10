#include "Application.h"
#include <cassert>
#include <algorithm>
#include <fstream>
#include <cereal/cereal.hpp>
#include <cereal/archives/xml.hpp>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <fstream>
#include "Window.h"
#include "File.h"
#include "Debug/Logger.h"
#include "Debug/Console.h"
#include "Scheduler.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <Debug/Profiler.h>
#include "Debug/Debug.h"
#include "Scene/Director.h"
#include "Components/Camera.h"

static void _oglLog(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userData);

Application* Application::__AppInstance{nullptr};

Application::Application(const std::string& name, int argc, char** argv)
	:m_AppName(name), m_RootPath(), m_Window(nullptr), m_IsRunning(false)
{
	m_RootPath = std::filesystem::path(argv[0]).remove_filename();
	assert(!__AppInstance);
	__AppInstance = this;

	int glInit = glfwInit();
	assert(glInit);
	m_Window = new Window(name.c_str());
	m_Window->MakeCurrent();

	m_Scheduler = new Scheduler();
	m_Console = new Console();

	GLR_LOG("application path: %ls", m_RootPath.c_str());
	/*if (argc > 1)
		ParseArgs(argc, argv);*/

	assert(m_Window);
	glInit = gladLoadGL();
	assert(glInit);
}

Application::~Application()
{	
	if (m_Window)
		delete m_Window;
	glfwTerminate();
	if (m_Console)
		delete m_Console;
	if (m_Scheduler)
		delete m_Scheduler;
		
}

Application* const Application::GetInstance()
{
	return __AppInstance;
}

void Application::OnInitialize()
{
	m_Window->OnWindowClose() += EventDelegate<WindowCloseEvent>{ConnectFunc<&Application::__WindowClosedCallback>, this};
	m_Window->OnWindowResize() += EventDelegate<WindowResizeEvent>{ ConnectFunc<&Application::__WindowResizeCallback>, this };

	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(GetWindow()->GetNativeHandle(), true);
	ImGui_ImplOpenGL3_Init("#version 460 core");
	ImGui::StyleColorsDark();
	io.Fonts->AddFontDefault();
}

void Application::OnGui()
{
	if (m_Console)
		m_Console->Draw();
	GLR_PROFILER_DRAW();
}

void Application::OnExit()
{
	GLR_PROFILER_EXPORT_FILE("ProfilerResults.txt");
}

int Application::MainLoop()
{
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(_oglLog, 0);
	OnInitialize();
	#ifdef GLR_PROFILE
	Logger::GetInstance()->Log("Profile is On!");
	#endif
	m_IsRunning = true;
	assert(m_Window);
	Director::GetInstance()->GetScene()->Enter();
	m_LastFrameTime = static_cast<float>(glfwGetTime());
	while (m_IsRunning)
	{
		GLR_PROFILER_START("Update Loop");

		float currentTime = glfwGetTime();
		m_DeltaTime = currentTime - m_LastFrameTime;
		m_LastFrameTime = currentTime;
		m_Scheduler->Update(m_DeltaTime);
		OnTick(m_DeltaTime);

		GLR_PROFILER_END();

		Render();

		GLR_PROFILER_START("Gui");

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		OnGui();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		GLR_PROFILER_END();

		m_Window->Update();
		
	}
	OnExit();
	return 0;
}

int Application::Render()
{
	GLR_PROFILER_START("Render Loop");
	
	glClearColor(.2f, .4f, .8f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera* camera = Director::GetInstance()->GetScene()->GetMainCamera();
	if (!camera)
	{
		GLR_LOG_ERROR("Renderer: No camera found!");
		GLR_PROFILER_END();
		return 1;
	}

	Director::GetInstance()->GetMeshRenderer().Render(camera);

	GLR_PROFILER_END();
	return 0;	
}

void Application::ParseArgs(int argc, wchar_t** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		if (!wcscmp(argv[i], L"-console"))
		{
			m_Console = new Console();
		}
	}
	
}

bool Application::__WindowClosedCallback(const WindowCloseEvent*)
{
	m_IsRunning = false;
	return false;
}

bool Application::__WindowResizeCallback(const WindowResizeEvent* e)
{
	glViewport(0, 0, e->Width, e->Height);
	return true;
}


static void _oglLog(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length, const char* message, const void* userData)
{
	if (severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH)
	{
		std::string str;
		str.resize(length + 8);
		sprintf(&str[0], "OPENGL: %s",message);
		Logger::GetInstance()->Log(str, severity == GL_DEBUG_SEVERITY_MEDIUM? LogEntry::Warning : LogEntry::Error);
	}
}
