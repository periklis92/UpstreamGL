#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <UpstreamGL/Components/Camera.h>
#include <UpstreamGL/Debug/Console.h>
#include <UpstreamGL/Debug/Debug.h>
#include <UpstreamGL/Debug/Profiler.h>
#include <UpstreamGL/Scene/Director.h>
#include <UpstreamGL/System/Application.h>
#include <UpstreamGL/System/File.h>
#include <UpstreamGL/System/Scheduler.h>
#include <UpstreamGL/System/Timer.h>
#include <ios>

namespace upgl {
static void _oglLog(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length,
                    const char *message, const void *userData);

Application *Application::__AppInstance{nullptr};

Application::Application(const std::string &name, int argc, char **argv)
    : m_AppName(name), m_RootPath(), m_Window(nullptr), m_IsRunning(false), m_LogStream(),
      m_Console(nullptr) {
  m_RootPath = std::filesystem::path(argv[0]).remove_filename();
  UPGL_ASSERT(!__AppInstance);
  __AppInstance = this;

  int glInit = glfwInit();
  UPGL_ASSERT(glInit);
  m_Window = new Window(name.c_str());
  m_Window->MakeCurrent();

  m_Scheduler = new Scheduler();

  m_LogStream.open("Log.txt", std::fstream::out | std::fstream::app);
  Logger::SetStream(&m_LogStream);

  UPGL_LOG("application path: %s", m_RootPath.c_str());
  if (argc > 1)
    ParseArgs(argc, argv);

  UPGL_ASSERT(m_Window);
  glInit = gladLoadGL();
  UPGL_ASSERT(glInit);
}

Application::~Application() {
  m_LogStream.flush();
  m_LogStream.close();
  if (m_Window)
    delete m_Window;
  glfwTerminate();
  if (m_Console)
    delete m_Console;
  if (m_Scheduler)
    delete m_Scheduler;
}

Application *const Application::GetInstance() { return __AppInstance; }

void Application::OnInitialize() {
  m_Window->OnWindowClose() +=
      EventDelegate<WindowCloseEvent>{ConnectFunc<&Application::__WindowClosedCallback>, this};
  m_Window->OnWindowResize() +=
      EventDelegate<WindowResizeEvent>{ConnectFunc<&Application::__WindowResizeCallback>, this};

  __InitImGui();
}

void Application::OnGui() {
  if (m_Console)
    m_Console->Draw();
  UPGL_PROFILER_DRAW();
}

void Application::OnExit() { UPGL_PROFILER_EXPORT_FILE("ProfilerResults.txt"); }

int Application::MainLoop() {
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(_oglLog, 0);
  OnInitialize();

#ifdef UPGL_PROFILE
  UPGL_LOG("Profile is On!");
#endif

  m_IsRunning = true;
  UPGL_ASSERT(m_Window);
  Director::GetInstance()->GetScene()->Enter();
  Timer deltaTimer, fixedDeltaTimer;
  while (m_IsRunning) {
    UPGL_PROFILER_START("Main Loop");
    UPGL_PROFILER_START("Update Loop");

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    OnGui();

    m_LastDeltaTime = deltaTimer.GetSeconds();
    deltaTimer.Reset();

    Director::GetInstance()->Tick(m_LastDeltaTime);
    m_Scheduler->Update(m_LastDeltaTime);
    if (fixedDeltaTimer.GetMilliseconds() >= UPGL_FIXED_UPDATE_STEP) {
      Director::GetInstance()->FixedUpdate(UPGL_FIXED_UPDATE_STEP);
      m_Scheduler->FixedUpdate(UPGL_FIXED_UPDATE_STEP);
      fixedDeltaTimer.Reset();
    }
    UPGL_PROFILER_END();
    UPGL_PROFILER_START("Render Loop");

    Render();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    m_Window->Update();

    UPGL_PROFILER_END();
    UPGL_PROFILER_END();
  }
  Director::GetInstance()->GetScene()->Exit();
  OnExit();
  return 0;
}

int Application::Render() {

  glClearColor(.2f, .4f, .8f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  Camera *camera = Director::GetInstance()->GetScene()->GetMainCamera();
  if (!camera) {
    UPGL_LOG_ERROR("Renderer: No camera found!");
    UPGL_PROFILER_END();
    return 1;
  }
  Camera *uicamera = Director::GetInstance()->GetScene()->GetUICamera();

  Director::GetInstance()->GetMeshRenderer().Render(camera);
  Director::GetInstance()->GetUIRenderer().Render(uicamera);

  return 0;
}

void Application::ParseArgs(int argc, char **argv) {
  for (int i = 1; i < argc; ++i) {
    if (!std::strcmp(argv[i], "--console") || !std::strcmp(argv[i], "-c")) {
      m_Console = new Console();
    }
  }
}

bool Application::__WindowClosedCallback(const WindowCloseEvent *) {
  m_IsRunning = false;
  return false;
}

bool Application::__WindowResizeCallback(const WindowResizeEvent *e) {
  glViewport(0, 0, e->Width, e->Height);
  return true;
}

void Application::__InitImGui() {
  ImGui::CreateContext();

  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  ImGui_ImplGlfw_InitForOpenGL(GetWindow()->GetNativeHandle(), true);
  ImGui_ImplOpenGL3_Init("#version 460 core");
  // ImGui::StyleColorsDark();
  auto &style = ImGui::GetStyle();

  style.WindowPadding = ImVec2(15.f, 15.f);
  style.WindowRounding = 7.0f;
  style.FramePadding = ImVec2(5, 5);
  style.FrameRounding = 4.0f;
  style.ItemSpacing = ImVec2(12, 8);
  style.ItemInnerSpacing = ImVec2(8, 6);
  style.IndentSpacing = 25.0f;
  style.ScrollbarSize = 15.0f;
  style.ScrollbarRounding = 9.0f;
  style.GrabMinSize = 5.0f;
  style.GrabRounding = 3.0f;

  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.24f, 0.24f, 0.24f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.32f, 0.32f, 0.32f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.92f, 0.75f, 0.07f, .92f));
  ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.07f, 0.07f, 0.09f, .92f));
  ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.07f, 0.07f, 0.09f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.80f, 0.80f, 0.83f, 0.88f));
  ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(0.92f, 0.91f, 0.88f, 0.00f));
  ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.10f, 0.09f, 0.12f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.24f, 0.23f, 0.29f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.56f, 0.56f, 0.58f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.10f, 0.09f, 0.12f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(1.00f, 0.98f, 0.95f, 0.75f));
  ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.07f, 0.07f, 0.09f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.10f, 0.09f, 0.12f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.10f, 0.09f, 0.12f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.80f, 0.80f, 0.83f, 0.31f));
  ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.80f, 0.80f, 0.83f, 0.31f));
  ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.80f, 0.80f, 0.83f, 0.31f));
  ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.06f, 0.05f, 0.07f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.10f, 0.09f, 0.12f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.24f, 0.23f, 0.29f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.56f, 0.56f, 0.58f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.23f, 0.20f, 0.13f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.56f, 0.56f, 0.58f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.06f, 0.05f, 0.07f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_ResizeGrip, ImVec4(0.00f, 0.00f, 0.00f, 0.00f));
  ImGui::PushStyleColor(ImGuiCol_ResizeGripHovered, ImVec4(0.56f, 0.56f, 0.58f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_ResizeGripActive, ImVec4(0.06f, 0.05f, 0.07f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_PlotLines, ImVec4(0.22f, 0.59f, 0.38f, 0.63f));
  ImGui::PushStyleColor(ImGuiCol_PlotLinesHovered, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.40f, 0.39f, 0.38f, 0.63f));
  ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, ImVec4(0.25f, 1.00f, 0.00f, 0.43f));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.56f, 0.56f, 0.58f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.06f, 0.05f, 0.07f, 1.00f));
  ImGui::PushStyleColor(ImGuiCol_PlotHistogramHovered, ImVec4(0.25f, 1.00f, 0.00f, 1.00f));

  io.Fonts->AddFontDefault();
}

static void _oglLog(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int32_t length,
                    const char *message, const void *userData) {
  if (severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH) {
    UPGL_LOG_SVR(severity == GL_DEBUG_SEVERITY_MEDIUM ? LogEntry::Warning : LogEntry::Error,
                 "OPENGL: %s", message);
  }
}
} // namespace upgl