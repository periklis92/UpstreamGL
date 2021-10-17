#include <UpstreamGL/System/Window.h>
#include <UpstreamGL/Input/InputManager.h>
#include <GLFW/glfw3.h>
#include <UpstreamGL/Debug/Debug.h>
#include <glad/glad.h>

Window::Window(const char* title, WindowSettings settings)
	:m_GLFWHandle(nullptr)
{
	glfwWindowHint(GLFW_RED_BITS, settings.RedBits);
	glfwWindowHint(GLFW_GREEN_BITS, settings.GreenBits);
	glfwWindowHint(GLFW_BLUE_BITS, settings.BlueBits);
	glfwWindowHint(GLFW_ALPHA_BITS, settings.AplhaBits);
	glfwWindowHint(GLFW_DEPTH_BITS, settings.DepthBits);
	glfwWindowHint(GLFW_STENCIL_BITS, settings.StencilBits);
	glfwWindowHint(GLFW_RESIZABLE, settings.Resizable);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	m_GLFWHandle = glfwCreateWindow(settings.Resolution.Width, settings.Resolution.Height, title, NULL, NULL);
	assert(m_GLFWHandle);
	glfwSetWindowUserPointer(m_GLFWHandle, this);
	glfwSetKeyCallback(m_GLFWHandle, Window::__KeyCallback);	
	glfwSetMouseButtonCallback(m_GLFWHandle, Window::__MouseButtonCallback);
	glfwSetWindowCloseCallback(m_GLFWHandle, Window::__WindowCloseCallback);
	glfwSetWindowSizeCallback(m_GLFWHandle, Window::__WindowSizeCallback);
	glfwSetCursorPosCallback(m_GLFWHandle, Window::__MousePosCallback);
	
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	assert(monitor);
	int count = 0;
	const GLFWvidmode* vidModes = glfwGetVideoModes(monitor, &count);
	assert(count);
	m_SupportedResolutions = new WindowResolution[count];
	m_NumSupportedResolutions = count;
	for (int i = 0; i < count; ++i)
	{
		m_SupportedResolutions[i].Width = vidModes[i].width;
		m_SupportedResolutions[i].Height = vidModes[i].height;
		m_SupportedResolutions[i].RefreshRate = vidModes[i].refreshRate;
	}
}

Window::~Window()
{
	if (m_SupportedResolutions)
		delete[] m_SupportedResolutions;
	glfwDestroyWindow(m_GLFWHandle);
}

const WindowResolution* Window::GetSupportedResolutions(int& numResolutions)
{
	numResolutions = m_NumSupportedResolutions;
	return m_SupportedResolutions;
}

void Window::Update()
{
	glfwSwapBuffers(m_GLFWHandle);
	glfwPollEvents();
}

bool Window::ShouldClose()
{
	return glfwWindowShouldClose(m_GLFWHandle);
}

void Window::MakeCurrent()
{
	glfwMakeContextCurrent(m_GLFWHandle);
}

void Window::SetFullscreen(bool fullscreen)
{
	if (!fullscreen)
	{
		glfwSetWindowMonitor(m_GLFWHandle, NULL, 32, 32, m_Settings.Resolution.Width-32, m_Settings.Resolution.Height-32, m_Settings.Resolution.RefreshRate);
		glfwSetWindowAttrib(m_GLFWHandle, GLFW_RESIZABLE, GLFW_TRUE);
	} 
	else
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor)
		{
			UPGL_LOG_ERROR("Unable to go to fullscreen! No monitors found.");
			return;
		}
		const GLFWvidmode* vid = glfwGetVideoMode(monitor);
		glfwSetWindowMonitor(m_GLFWHandle, monitor, 0, 0, vid->width, vid->height, vid->refreshRate);
		m_Settings.Resolution.Width = vid->width;
		m_Settings.Resolution.Height = vid->height;
		m_Settings.Resolution.RefreshRate = vid->refreshRate;
	}
	m_Settings.Fullscreen = fullscreen;
}

void Window::SetResolution(const WindowResolution& resolution)
{	
	if (!m_Settings.Fullscreen)
	{
		int x = 0, y = 0;
		glfwGetWindowPos(m_GLFWHandle, &x, &y);
		glfwSetWindowMonitor(m_GLFWHandle, NULL, x, y, resolution.Width, resolution.Height, resolution.RefreshRate);
	} 
	else
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		if (!monitor)
		{
			UPGL_LOG_ERROR("Unable to change resolution! No monitors found.");
			return;
		}
		glfwSetWindowMonitor(m_GLFWHandle, monitor, 0, 0, resolution.Width, resolution.Height, resolution.RefreshRate);
	}
	m_Settings.Resolution = resolution;
}

void Window::SetVSync(bool vsync)
{
	m_Settings.VSync = vsync;
	if (vsync)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void Window::Close()
{
	glfwSetWindowShouldClose(m_GLFWHandle, GLFW_TRUE);
}

void Window::__KeyCallback(GLFWwindow* glwin, int key, int, int action, int)
{
	if (action == GLFW_REPEAT) return;
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	InputManager::GetInstance()->OnKeyboardKey().Publish(InputKeyboardEvent{ (KeyboardKey)key, action == GLFW_PRESS ? true : false });
}

void Window::__WindowCloseCallback(GLFWwindow* glwin)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	window->m_OnWindowClose.Publish(WindowCloseEvent());
}

void Window::__WindowSizeCallback(GLFWwindow* glwin, int width, int height)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	window->m_OnWindowResize.Publish(WindowResizeEvent(width, height));
}

void Window::__MouseButtonCallback(GLFWwindow* glwin, int button, int action, int )
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	InputManager::GetInstance()->OnMouseButton().Publish(InputMouseButtonEvent{ (MouseButton)button, action == GLFW_PRESS ? true : false });
}

void Window::__MousePosCallback(GLFWwindow* glwin, double x, double y)
{
	Window* window = static_cast<Window*>(glfwGetWindowUserPointer(glwin));
	assert(window);
	InputManager::GetInstance()->OnMouseMove().Publish(InputMouseMoveEvent{ (float)x - window->m_LastMouseX, (float)y - window->m_LastMouseY });
	window->m_LastMouseX = static_cast<float>(x);
	window->m_LastMouseY = static_cast<float>(y);
}